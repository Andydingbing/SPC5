#include "QCalR1CTxSBThread.h"
#include "qcalr1ctxsbdlg.h"

QCalR1CTxSBThread::QCalR1CTxSBThread(CalParam *param, QObject *parent) : QCalBaseThread(param,parent)
{

}

void QCalR1CTxSBThread::run()
{
    try {
        CAL_THREAD_START("Tx Sideband",100);

        QR1CTxSBModel *model = (QR1CTxSBModel *)(calParam.model_0);
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
        sp2401_r1a *pSP2401 = calParam._sp2401;
        bool useSA = (CalParam::M_Spectrum == calParam.methodSB ? true : false);

        double thM = 0.0;
        uint16_t am_i_m = 8192;
        uint16_t am_q_m = 8192;
        double pwrSb = 0.0;

        tx_sb_table_r1cd::data_f_t data;
        data.use_sa = useSA;

        if (useSA)
            THREAD_CHECK_BOX(QString("TX<===>Spectrum"));

        pSP1401->set_tx_att(10.0,10.0,10.0,0.0);
        pSP2401->set_da_sw(sp2401_r1a::FROM_TO_RF);
        pSP2401->set_ad_sw(sp2401_r1a::FROM_TO_RF);
        pSP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
        pSP2401->set_dds1(20000000.0);
        pSP2401->set_tx_pwr_comp(0.0);
        pSP2401->set_tx_phase_rotate_I(0.0);
        pSP2401->set_tx_amplitude_balance(8192,8192);
        pSP2401->set_tx_filter_sw(false);

        if (useSA) {
            calUseSA(thM,am_i_m,am_q_m,pwrSb);
            CAL_THREAD_TEST_CANCEL
        }
        else {
            calUseLoop(thM,am_i_m,am_q_m,pwrSb);
            CAL_THREAD_TEST_CANCEL
        }

        data.th     = thM;
        data.am_i   = am_i_m;
        data.am_q   = am_q_m;
        data.pwr    = pwrSb;
        pSP1401->get_temp4_tx_lo2(&data.temp[0]);
        pSP1401->get_temp5_tx_lo1(&data.temp[1]);
        pSP1401->get_temp6_tx_pa4(&data.temp[2]);
        pSP1401->get_temp7_tx_pa3(&data.temp[3]);
        data.time = getCurTime();
        pSP1401->get_cal_file()->add(cal_file::TX_SB,(tx_sb_table_r1cd::data_f_t *)&data);
        pSP1401->get_cal_file()->w(cal_file::TX_SB);

        for (int32_t i = 0;i < SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED);i ++) {
            data.freq = RF_TX_FREQ_STAR + i * RF_TX_FREQ_STEP_CALLED;
            model->calTable()->replace(i,data);
            emit update(model->index(i,0),model->index(i,10));
        }
        SET_PROG_POS(100);
        CAL_THREAD_ABOART
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}

void QCalR1CTxSBThread::calUseSA(double &thM, uint16_t &am_i_m, uint16_t &am_q_m, double &pwrSb)
{
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    sp2401_r1a *pSP2401 = calParam._sp2401;
    uint64_t freq = 2400000000;
    double pwrSA = 0.0;
    int16_t dc_i = 0,dc_q = 0;
    float tempLOL = 0.0;

    double thL = -10.0;
    double thR = 10.0;
    thM = 0.0;
    uint16_t am_i_l = 8192 - 500,am_i_r = 8192 + 500;
    uint16_t am_q_l = 8192 - 500,am_q_r = 8192 + 500;
    am_i_m = 8192;
    am_q_m = 8192;

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_ref(-10.0);
    Instr.sa_set_span(1e6);
    Instr.sa_set_rbw(50e3,false);
    Instr.sa_set_vbw(10e3,false);
//    Instr.sa_set_avg_trace(true,3);

    pSP1401->set_io_mode(sp1401::OUTPUT);
    pSP1401->set_tx_freq(freq);
    pSP2401->set_tx_phase_rotate_I(0.0);
    pSP2401->set_tx_amplitude_balance(8192,8192);
    pSP2401->set_duc_dds(0.0);
    pSP1401->get_cal_file()->m_tx_lol->get(freq,dc_i,dc_q,tempLOL);
    pSP2401->set_tx_dc_offset(dc_i,dc_q);

    Instr.sa_set_cf((double)(freq - 20000000));
    Instr.sa_set_ref(-10.0);
//    Instr.sa_set_avg_trace(false,0);
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);

    while ((thR - thL) >= 1) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        thM = round((thL + thR) / 2.0,1);
        if (derivativeTh_SA(thM,1,pwrSb) > 0)
            thR = thM;
        else
            thL = thM;
        CAL_THREAD_TEST_PAUSE_E
    }
    Instr.sa_set_ref(-40.0);
//    Instr.sa_set_avg_trace(true,3);

    thL = thM - 0.5;
    thR = thM + 0.5;
    while ((thR - thL) >= 1) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        thM = round((thL + thR) / 2.0,1);
        if (derivativeTh_SA(thM,0.1,pwrSb) > 0)
            thR = thM;
        else
            thL = thM;
        CAL_THREAD_TEST_PAUSE_E
    }

    double thM_min = thM;
    for (thM = thL; thM <= thR; thM += 0.1) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        pSP2401->set_tx_phase_rotate_I(thM);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(pwrSA);
        if (pwrSA < pwrSb) {
            pwrSb = pwrSA;
            thM_min = thM;
        }
        CAL_THREAD_TEST_PAUSE_E
    }
    thM = thM_min;
    pSP2401->set_tx_phase_rotate_I(thM);

    while ((am_i_r - am_i_l) >= 2 ) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        am_i_m = (uint16_t)((am_i_l + am_i_r) / 2);
        if (derivativeAmI_SA(am_i_m,am_q_m,10,pwrSb) > 0)
            am_i_r = am_i_m;
        else
            am_i_l = am_i_m;
        if (pwrSb < -90.0) {
            threadLock.unlock();
            break;
        }
        CAL_THREAD_TEST_PAUSE_E
    }
    while ((am_q_r - am_q_l) >= 2) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        am_q_m = (uint16_t)((am_q_l + am_q_r) / 2);
        if (derivativeAmQ_SA(am_i_m,am_q_m,10,pwrSb) > 0)
            am_q_r = am_q_m;
        else
            am_q_l = am_q_m;
        if (pwrSb < -90.0) {
            threadLock.unlock();
            break;
        }
        CAL_THREAD_TEST_PAUSE_E
    }

    thM_min = thM;
    thL = thM - 0.5;
    thR = thM + 0.5;
    for (thM = thL; thM <= thR; thM += 0.1) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        pSP2401->set_tx_phase_rotate_I(thM);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(pwrSA);
        if (pwrSA < pwrSb) {
            pwrSb = pwrSA;
            thM_min = thM;
        }
        CAL_THREAD_TEST_PAUSE_E
    }
    thM = thM_min;
    pSP2401->set_tx_phase_rotate_I(thM);

    for (int16_t retry = 0;retry < 2;retry ++) {
        CAL_THREAD_TEST_PAUSE_S
        thM_min = thM;
        thL = thM - 0.2;
        thR = thM + 0.2;
        for (thM = thL; thM <= thR; thM += 0.1) {
            pSP2401->set_tx_phase_rotate_I(thM);
            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA);
            if (pwrSA < pwrSb) {
                pwrSb = pwrSA;
                thM_min = thM;
            }
        }
        thM = thM_min;
        pSP2401->set_tx_phase_rotate_I(thM);

        if (derivativeAmI_SA(am_i_m,am_q_m,1,pwrSb) > 0) {
            while (-- am_i_m) {
                if (derivativeAmI_SA(am_i_m,am_q_m,1,pwrSb) < 0)
                    break;
            }
        }
        else {
            while (++ am_i_m) {
                if (derivativeAmI_SA(am_i_m,am_q_m,1,pwrSb) > 0)
                    break;
            }
        }

        if (derivativeAmQ_SA(am_i_m,am_q_m,1,pwrSb) > 0) {
            while (-- am_q_m) {
                if (derivativeAmQ_SA(am_i_m,am_q_m,1,pwrSb) < 0)
                    break;
            }
        }
        else {
            while (++ am_q_m) {
                if (derivativeAmQ_SA(am_i_m,am_q_m,1,pwrSb) > 0)
                    break;
            }
        }
        if (pwrSb < -90.0) {
            threadLock.unlock();
            break;
        }
        CAL_THREAD_TEST_PAUSE_E
    }
}

void QCalR1CTxSBThread::calUseLoop(double &thM, uint16_t &am_i_m, uint16_t &am_q_m, double &pwrSb)
{
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    sp2401_r1a *pSP2401 = calParam._sp2401;
    uint64_t freq = 2400000000;
    int16_t dc_i = 0,dc_q = 0;
    float tempLOL = 0.0;
    double pwr[512] = {0.0};
    pwrSb = 100.0;

    pSP1401->set_io_mode(sp1401::LOOP);
    pSP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
    pSP1401->set_rx_att_019_sw(sp1401::RX_ATT_0);
    pSP1401->set_rx_att1(25.0);
    pSP1401->set_rx_att2(10.0);
    pSP1401->set_rx_att3(0.0);
    pSP2401->set_ddc(-92640000.0);
    pSP2401->set_rx_filter_nb();

    pSP1401->set_pwr_meas_src(basic_sp1401::PWR_MEAS_FREE_RUN,false);
    pSP1401->set_pwr_meas_samples(327680);

    CAL_THREAD_TEST_CANCEL
    double stepTh = 0.0;
    uint16_t stepAm = 0;

    double thL = -5.0;
    double thR = 5.0;
    thM = 0.0;
    uint16_t am_i_l = 8192 - 500,am_i_r = 8192 + 500;
    uint16_t am_q_l = 8192 - 500,am_q_r = 8192 + 500;
    am_i_m = 8192;
    am_q_m = 8192;

    pSP1401->set_rx_freq(freq - 20000000);
    pSP1401->set_tx_freq(freq);
    pSP2401->set_tx_phase_rotate_I(0.0);
    pSP2401->set_tx_amplitude_balance(8192,8192);
    pSP1401->get_cal_file()->m_tx_lol->get(freq,dc_i,dc_q,tempLOL);
    pSP2401->set_tx_dc_offset(dc_i,dc_q);
    msleep(50);

    CAL_THREAD_TEST_CANCEL
    stepTh = 2.0;
    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinThI_Rx(stepTh,0.5,&thL,&thR,&thM,pwr);

    stepTh = 0.5;
    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinThI_Rx(stepTh,0.4,&thL,&thR,&thM,pwr);

    CAL_THREAD_TEST_CANCEL
    stepTh = 0.1;
    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinThI_Rx(stepTh,1.0,&thL,&thR,&thM,pwr);

    CAL_THREAD_TEST_CANCEL
    stepAm = 20;
    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinAmI_Rx(stepAm,1,&am_i_l,&am_i_r,&am_i_m,&am_q_m,pwr);
    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinAmQ_Rx(stepAm,1,&am_q_l,&am_q_r,&am_i_m,&am_q_m,pwr);

    stepAm = 5;
    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinAmI_Rx(stepAm,1,&am_i_l,&am_i_r,&am_i_m,&am_q_m,pwr);
    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinAmQ_Rx(stepAm,1,&am_q_l,&am_q_r,&am_i_m,&am_q_m,pwr);

    memset(pwr,0,sizeof(pwr));
    pwrSb = getMinThI_Rx(stepTh,1.0,&thL,&thR,&thM,pwr);

    CAL_THREAD_TEST_CANCEL
    stepAm = 1;
    for (int32_t retry = 0;retry < 3;retry ++) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL
        memset(pwr,0,sizeof(pwr));
        pwrSb = getMinAmI_Rx(stepAm,2,&am_i_l,&am_i_r,&am_i_m,&am_q_m,pwr);
        memset(pwr,0,sizeof(pwr));
        pwrSb = getMinAmQ_Rx(stepAm,2,&am_q_l,&am_q_r,&am_i_m,&am_q_m,pwr);
        CAL_THREAD_TEST_PAUSE_E
    }
}

double QCalR1CTxSBThread::derivativeTh_SA(double th, double delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};
    calParam._sp2401->set_tx_phase_rotate_I(th - delta);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);

    calParam._sp2401->set_tx_phase_rotate_I(th);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);
    pwr = pwrSA[1];

    return ((pwrSA[1] - pwrSA[0]) / delta);
}

double QCalR1CTxSBThread::derivativeAmI_SA(uint16_t am_i, uint16_t am_q, uint16_t delta, double &pwr)
{
    double pwrSA[2] = {0.0,0.0};

    calParam._sp2401->set_tx_amplitude_balance(am_i - delta,am_q);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);

    calParam._sp2401->set_tx_amplitude_balance(am_i,am_q);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);
    pwr = pwrSA[1];

    return ((pwrSA[1] - pwrSA[0]) / delta);
}

double QCalR1CTxSBThread::derivativeAmQ_SA(uint16_t am_i, uint16_t am_q, uint16_t delta, double &pwr)
{
    double pwrSA[2] = {0,0};

     calParam._sp2401->set_tx_amplitude_balance(am_i,am_q - delta);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[0]);

     calParam._sp2401->set_tx_amplitude_balance(am_i,am_q);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(pwrSA[1]);
    pwr = pwrSA[1];

    return ((pwrSA[1] - pwrSA[0]) / delta);
}

double QCalR1CTxSBThread::getMinThI_Rx(double step, double lrCoef, double *thL, double *thR, double *thM, double *pwr)
{
    double pwrMin = (double)LONG_MAX;
    int16_t idx = 0,idxMin = 0;

    for (*thM = *thL;*thM <= *thR;*thM += step) {
        calParam._sp2401->set_tx_phase_rotate_I(*thM);
        measOnce_Rx(&pwr[idx]);
        if (pwr[idx] < pwrMin) {
            pwrMin = pwr[idx];
            idxMin = idx;
        }
        idx ++;
    }
    *thM = *thL + idxMin * step;
    *thL = *thM - step * lrCoef;
    *thR = *thM + step * lrCoef;
    return pwrMin;
}

double QCalR1CTxSBThread::getMinAmI_Rx(uint16_t step, uint16_t lrCoef, uint16_t *am_i_l, uint16_t *am_i_r, uint16_t *am_i_m, uint16_t *am_q_m, double *pwr)
{
    double pwrMin = (double)LONG_MAX;
    int16_t idx = 0,idxMin = 0;

    for (*am_i_m = *am_i_l;*am_i_m <= *am_i_r;*am_i_m += step) {
        calParam._sp2401->set_tx_amplitude_balance(*am_i_m,*am_q_m);
        measOnce_Rx(&pwr[idx]);
        if (pwr[idx] < pwrMin) {
            pwrMin = pwr[idx];
            idxMin = idx;
        }
        idx ++;
    }
    *am_i_m = *am_i_l + idxMin * step;
    *am_i_l = *am_i_m - step * lrCoef;
    *am_i_r = *am_i_m + step * lrCoef;
    return pwrMin;
}

double QCalR1CTxSBThread::getMinAmQ_Rx(uint16_t step, uint16_t lrCoef, uint16_t *am_q_l, uint16_t *am_q_r, uint16_t *am_i_m, uint16_t *am_q_m, double *pwr)
{
    double pwrMin = (double)LONG_MAX;
    int16_t idx = 0,idxMin = 0;

    for (*am_q_m = *am_q_l;*am_q_m <= *am_q_r;*am_q_m += step) {
        calParam._sp2401->set_tx_amplitude_balance(*am_i_m,*am_q_m);
        measOnce_Rx(&pwr[idx]);
        if (pwr[idx] < pwrMin) {
            pwrMin = pwr[idx];
            idxMin = idx;
        }
        idx ++;
    }
    *am_q_m = *am_q_l + idxMin * step;
    *am_q_l = *am_q_m - step * lrCoef;
    *am_q_r = *am_q_m + step * lrCoef;
    return pwrMin;
}

void QCalR1CTxSBThread::measOnce_Rx(double *pwrAvg)
{
    double pwr[5] = {0.0};
    basic_sp1401::pwr_meas_state_t state = basic_sp1401::PMS_RUNNING;

    for (int32_t avgIdx = 0;avgIdx < 3;avgIdx ++) {
        calParam._sp1401->pwr_meas_abort();
        while (state != basic_sp1401::PMS_IDLE)
            calParam._sp1401->get_pwr_meas_state(state);
        calParam._sp1401->pwr_meas_start();
        while (state != basic_sp1401::PMS_DONE) {
            calParam._sp1401->get_pwr_meas_state(state);
            if (state == basic_sp1401::PMS_TT)
                break;
        }
        calParam._sp1401->get_pwr_meas_pwr(pwr[avgIdx]);
        *pwrAvg += pwr[avgIdx];
    }
    *pwrAvg /= 2.0;
}

QExportR1CTxSBThread::QExportR1CTxSBThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CTxSBThread::run()
{
    INIT_PROG("Exporting Tx Sideband",100);
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    QR1CTxSBModel *model = (QR1CTxSBModel *)(calParam.model_0);
    tx_sb_table_r1cd::data_f_t data;
    int idx = 0;

    pSP1401->get_cal_file()->map2buf(cal_file::TX_SB);
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_CALLED) {
        pSP1401->get_cal_file()->m_tx_sb->get(freq,&data);
        model->calTable()->replace(idx,data);
        idx ++;
    }

    emit update(model->index(0,0),model->index(RF_TX_FREQ_PTS_CALLED,10));

    SET_PROG_POS(100);
    THREAD_ABORT
}
