#include "qcalr1ctxloleakthread.h"
#include "qcalr1ctxloleakdlg.h"

QCalR1CTxLOLeakThread::QCalR1CTxLOLeakThread(CalParam *param, QObject *parent) : QCalBaseThread(param,parent)
{

}

void QCalR1CTxLOLeakThread::run()
{
    try {
        CAL_THREAD_START("TX LO Leakage",100);

        QR1CTxLOLeakModel *model = (QR1CTxLOLeakModel *)(calParam.model_0);
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp3501 *pSP3501 = calParam._sp3501;
        bool calX9119 = calParam.calX9119;
        bool useSA = (CalParam::M_Spectrum == calParam.methodLOLeak);

        uint64_t freq = 2400000000;
        int16_t dc_i_m = 0;
        int16_t dc_q_m = 0;

        tx_lol_table_r1cd::data_f_t data;
        data.use_sa = useSA;

        if (calX9119 || useSA)
            THREAD_CHECK_BOX(QString("Tx<===>Spectrum"));

        pSP1401->set_tx_att(10.0,10.0,10.0,0.0);
        pSP2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
        pSP2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
        pSP2401->set_dds_src(sp2401_r1a::ALL_0);
        pSP2401->set_tx_pwr_comp(0.0);
        pSP2401->set_tx_dc_offset(0,0);
        pSP2401->set_tx_filter_sw(false);

        if (calX9119) {
            CAL_THREAD_TEST_CANCEL
            double mkrFreq = 0.0;
            double mkrPwr  = -100;
            x9119_table::data_t Xdata;
            pSP1401->set_io_mode(OUTPUT);
            pSP1401->set_tx_freq(freq);
            Instr.init();
            Instr.has_sa();
            Instr.sa_reset();
            Instr.sa_set_cf(double(freq));
            Instr.sa_set_span(5000.0);
            Instr.sa_set_rbw(200.0,false);
            Instr.sa_set_vbw(300.0,false);
            Instr.sa_set_ref(-10.0);
            for (int16_t value = 0;value < 1023;value ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL
                pSP3501->vol_9119(value);
                Instr.sa_sweep_once();
                Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
                Instr.sa_get_marker_freq(mkrFreq);
                Instr.sa_get_marker_pwr(mkrPwr);
                if (abs(mkrFreq - double(freq)) < 1.0 && mkrPwr >= -70.0) {
                    Xdata.value = value;
                    pSP1401->get_cal_file()->add(cal_file::X9119,(x9119_table::data_t *)(&Xdata));
                    pSP1401->get_cal_file()->w(cal_file::X9119);
                    threadLock.unlock();
                    break;
                }
                CAL_THREAD_TEST_PAUSE_E
            }
            Instr.close();
        }

        if (useSA) {
            calUseSA(dc_i_m,dc_q_m,data.pwr);
            CAL_THREAD_TEST_CANCEL
        }
        else {
            calUseLoop(dc_i_m,dc_q_m,data.pwr);
            CAL_THREAD_TEST_CANCEL
        }

        data.freq = freq;
        data.dc_i = dc_i_m;
        data.dc_q = dc_q_m;
        pSP1401->get_temp4_tx_lo2(&data.temp[0]);
        pSP1401->get_temp5_tx_lo1(&data.temp[1]);
        pSP1401->get_temp6_tx_pa4(&data.temp[2]);
        pSP1401->get_temp7_tx_pa3(&data.temp[3]);
        data.time = getCurTime();
        pSP1401->get_cal_file()->add(cal_file::TX_LOL,(tx_lol_table_r1cd::data_f_t *)&data);
        pSP1401->get_cal_file()->w(cal_file::TX_LOL);

        for (int32_t i = 0;i < RF_TX_FREQ_PTS_CALLED;i ++) {
            data.freq = RF_TX_FREQ_STAR + i * RF_TX_FREQ_STEP_CALLED;
            model->calTable()->replace(i,data);
            emit update(model->index(i,0),model->index(i,9));
        }
        SET_PROG_POS(100);
        CAL_THREAD_ABOART
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}

void QCalR1CTxLOLeakThread::calUseSA(int16_t &dc_i_m, int16_t &dc_q_m,double &pwrLOL)
{
    CAL_THREAD_TEST_CANCEL

    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    sp2401_r1a *pSP2401 = calParam._sp2401;

    pSP1401->set_io_mode(OUTPUT);

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(10e3);
    Instr.sa_set_ref(0.0);

    uint64_t freq = 2400000000;
    int16_t dc_i_l = -10000,dc_i_r = 10000;
    int16_t dc_q_l = -10000,dc_q_r = 10000;
    int16_t step = 0;
    double pwrSA = 0.0;

    pSP1401->set_tx_freq(freq);
    pSP2401->set_tx_dc_offset(dc_i_m,dc_q_m);

    Instr.sa_set_cf((double)freq);
    Instr.sa_set_ref(0.0);
    Instr.sa_sweep_once();
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);

    step = 200;
    while ((dc_i_r - dc_i_l) > 4) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        dc_i_m = (int16_t)round((dc_i_l + dc_i_r) / 2.0,0);
        if (derivativeDCOffsetI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_i_r = dc_i_m;
        else
            dc_i_l = dc_i_m;
        if (pwrLOL < -80.0) {
            threadLock.unlock();
            break;
        }
        CAL_THREAD_TEST_PAUSE_E
    }
    while ((dc_q_r - dc_q_l) > 4) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        dc_q_m = (int16_t)round((dc_q_l + dc_q_r) / 2.0,0);
        if (derivativeDCOffsetQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_q_r = dc_q_m;
        else
            dc_q_l = dc_q_m;
        if (pwrLOL < -80.0) {
            threadLock.unlock();
            break;
        }
        CAL_THREAD_TEST_PAUSE_E
    }

    step = 20;
    dc_i_l = dc_i_m - 100;
    dc_i_r = dc_i_m + 100;
    if (pwrLOL <= -40.0)
        Instr.sa_set_ref(-40.0);
    while ((dc_i_r - dc_i_l) > 4) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        if (pwrLOL < -90.0) {
            threadLock.unlock();
            break;
        }
        dc_i_m = (int16_t)round((dc_i_l + dc_i_r) / 2.0,0);
        if (derivativeDCOffsetI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_i_r = dc_i_m;
        else
            dc_i_l = dc_i_m;
        CAL_THREAD_TEST_PAUSE_E
    }
    dc_q_l = dc_q_m - 100;
    dc_q_r = dc_q_m + 100;
    while ((dc_q_r - dc_q_l) > 4) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        if (pwrLOL < -90.0) {
            threadLock.unlock();
            break;
        }
        dc_q_m = (int16_t)round((dc_q_l + dc_q_r) / 2.0,0);
        if (derivativeDCOffsetQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
            dc_q_r = dc_q_m;
        else
            dc_q_l = dc_q_m;
        CAL_THREAD_TEST_PAUSE_E
    }

    if (pwrLOL <= -40.0)
        Instr.sa_set_ref(-40.0);
    step = 1;
    for (int16_t retry = 0;retry < 1;retry ++) {
        CAL_THREAD_TEST_PAUSE_S
        if (derivativeDCOffsetI_SA(dc_i_m,dc_q_m,1,pwrLOL) > 0) {
            while ( -- dc_i_m) {
                if (derivativeDCOffsetI_SA(dc_i_m,dc_q_m,step,pwrLOL) < 0)
                    break;
            }
        }
        else {
            while ( ++ dc_i_m) {
                if (derivativeDCOffsetI_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
                    break;
            }
        }

        if (derivativeDCOffsetQ_SA(dc_i_m,dc_q_m,1,pwrLOL) > 0) {
            while ( -- dc_q_m) {
                if (derivativeDCOffsetQ_SA(dc_i_m,dc_q_m,step,pwrLOL) < 0)
                    break;
            }
        }
        else {
            while ( ++ dc_q_m) {
                if (derivativeDCOffsetQ_SA(dc_i_m,dc_q_m,step,pwrLOL) > 0)
                    break;
            }
        }
        CAL_THREAD_TEST_PAUSE_E
    }

    int16_t DC_I_M_Min = dc_i_m;
    int16_t DC_Q_M_Min = dc_q_m;

    for (int retry = 0;retry < 1;retry ++) {
        dc_i_l = dc_i_m - 5;
        dc_i_r = dc_i_m + 5;
        for (dc_i_m = dc_i_l; dc_i_m <= dc_i_r; dc_i_m ++) {
            CAL_THREAD_TEST_PAUSE_S
            pSP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);
            if (pwrSA < pwrLOL) {
                pwrLOL = pwrSA;
                DC_I_M_Min = dc_i_m;
            }
            CAL_THREAD_TEST_PAUSE_E
        }
        dc_i_m = DC_I_M_Min;
        for (dc_q_m = dc_q_l; dc_q_m <= dc_q_r; dc_q_m ++) {
            CAL_THREAD_TEST_PAUSE_S
            pSP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);
            if (pwrSA < pwrLOL) {
                pwrLOL = pwrSA;
                DC_Q_M_Min = dc_q_m;
            }
            CAL_THREAD_TEST_PAUSE_E
        }
        dc_q_m = DC_Q_M_Min;
    }
    Instr.close();
}

void QCalR1CTxLOLeakThread::calUseLoop(int16_t &dc_i_m, int16_t &dc_q_m,double &pwrLOL)
{
    CAL_THREAD_TEST_CANCEL

    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    sp2401_r1a *pSP2401 = calParam._sp2401;

    pSP1401->set_io_mode(LOOP);
    pSP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
    pSP1401->set_rx_att_019_sw(sp1401::RX_ATT_0);
    pSP2401->set_ddc(-92640000.0);

    uint64_t freq = 2400000000;
    int16_t dc_i_l = -10000,dc_i_r = 10000;
    int16_t dc_q_l = -10000,dc_q_r = 10000;
    int16_t step = 0;
    int64_t adLOL[512] = {0},adMin = LONG_MAX;

    pSP1401->set_rx_freq(freq);
    pSP1401->set_rx_att1(20.0);
    pSP1401->set_rx_att2(20.0);
    pSP1401->set_rx_att3(0.0);
    pSP1401->set_tx_freq(freq);
    pSP2401->set_tx_dc_offset(dc_i_m,dc_q_m);
    msleep(50);

    CAL_THREAD_TEST_CANCEL
    step = 200;
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetI_Rx(step,1,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetQ_Rx(step,1,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);

    CAL_THREAD_TEST_CANCEL
    step = 20;
    pSP1401->set_rx_att1(10.0);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetI_Rx(step,1,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetQ_Rx(step,1,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);

    CAL_THREAD_TEST_CANCEL
    step = 5;
    pSP1401->set_rx_att2(0.0);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetI_Rx(step,1,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
    memset(adLOL,0,sizeof(adLOL));
    adMin = getMinDCOffsetQ_Rx(step,1,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);

    CAL_THREAD_TEST_CANCEL
    step = 1;
    pSP1401->set_rx_att1(0.0);
    for (int32_t retry = 0;retry < 3;retry ++) {
        CAL_THREAD_TEST_CANCEL
        memset(adLOL,0,sizeof(adLOL));
        adMin = getMinDCOffsetI_Rx(step,2,&dc_i_l,&dc_i_r,&dc_i_m,&dc_q_m,adLOL);
        memset(adLOL,0,sizeof(adLOL));
        adMin = getMinDCOffsetQ_Rx(step,2,&dc_q_l,&dc_q_r,&dc_i_m,&dc_q_m,adLOL);
    }
    pwrLOL = ad2dBc(_0dBFS,adMin);
}

double QCalR1CTxLOLeakThread::derivativeDCOffsetI_SA(int16_t dc_i_m, int16_t dc_q_m, int16_t delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};
    calParam._sp2401->set_tx_dc_offset((uint16_t)(dc_i_m + delta),(uint16_t)dc_q_m);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);

    calParam._sp2401->set_tx_dc_offset((uint16_t)dc_i_m,(uint16_t)dc_q_m);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);
    pwr = pwrSA[0];

    return (pwrSA[1] - pwrSA[0]) / (double)delta;
}

double QCalR1CTxLOLeakThread::derivativeDCOffsetQ_SA(int16_t dc_i_m, int16_t dc_q_m, int16_t delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};
    calParam._sp2401->set_tx_dc_offset((uint16_t)dc_i_m,(uint16_t)(dc_q_m + delta));
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);

    calParam._sp2401->set_tx_dc_offset((uint16_t)dc_i_m,(uint16_t)dc_q_m);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);
    pwr = pwrSA[0];

    return (pwrSA[1] - pwrSA[0]) / (double)delta;
}

int64_t QCalR1CTxLOLeakThread::getMinDCOffsetI_Rx(int16_t step, int16_t lr_coef, int16_t *dc_i_l, int16_t *dc_i_r, int16_t *dc_i_m, int16_t *dc_q_m, int64_t *ad)
{
    int64_t adMin = LONG_MAX;
    int16_t idx = 0,idxMin = 0;
    for (*dc_i_m = *dc_i_l;*dc_i_m <= *dc_i_r;*dc_i_m += step) {
        CAL_THREAD_TEST_PAUSE_S
        calParam._sp2401->set_tx_dc_offset(*dc_i_m,*dc_q_m);
        calParam._sp1401->get_ads5474(ad[idx]);
        if (ad[idx] < adMin) {
            adMin = ad[idx];
            idxMin = idx;
        }
        idx ++;
        CAL_THREAD_TEST_PAUSE_E
    }
    *dc_i_m = *dc_i_l + idxMin * step;
    *dc_i_l = *dc_i_m - step * lr_coef;
    *dc_i_r = *dc_i_m + step * lr_coef;
    return adMin;
}

int64_t QCalR1CTxLOLeakThread::getMinDCOffsetQ_Rx(int16_t step, int16_t lr_coef, int16_t *dc_q_l, int16_t *dc_q_r, int16_t *dc_i_m, int16_t *dc_q_m, int64_t *ad)
{
    int64_t adMin = LONG_MAX;
    int16_t idx = 0,idxMin = 0;
    for (*dc_q_m = *dc_q_l;*dc_q_m <= *dc_q_r;*dc_q_m += step) {
        CAL_THREAD_TEST_PAUSE_S
        calParam._sp2401->set_tx_dc_offset(*dc_i_m,*dc_q_m);
        calParam._sp1401->get_ads5474(ad[idx]);
        if (ad[idx] < adMin) {
            adMin = ad[idx];
            idxMin = idx;
        }
        idx ++;
        CAL_THREAD_TEST_PAUSE_E
    }
    *dc_q_m = *dc_q_l + idxMin * step;
    *dc_q_l = *dc_q_m - step * lr_coef;
    *dc_q_r = *dc_q_m + step * lr_coef;
    return adMin;
}


QExportR1CTxLOLeakThread::QExportR1CTxLOLeakThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CTxLOLeakThread::run()
{
    INIT_PROG("Exporting Tx LO Leakage",100);
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    QR1CTxLOLeakModel *model = (QR1CTxLOLeakModel *)(calParam.model_0);
    tx_lol_table_r1cd::data_f_t data;
    int idx = 0;

    pSP1401->get_cal_file()->map2buf(cal_file::TX_LOL);
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_CALLED) {
        pSP1401->get_cal_file()->m_tx_lol->get(freq,&data);
        model->calTable()->replace(idx,data);
        idx ++;
    }

    emit update(model->index(0,0),model->index(RF_TX_FREQ_PTS_CALLED,9));

    SET_PROG_POS(100);
    THREAD_ABORT
}
