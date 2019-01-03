#include "qcalr1ctxattthread.h"
#include "qcalr1ctxattdlg.h"

QCalR1CTxAttThread::QCalR1CTxAttThread(CalParam *param, QObject *parent) : QCalBaseThread(param,parent)
{

}

void QCalR1CTxAttThread::run()
{
    try {
        QString strFreqStar = calParam.rfFreqStar;
        QString strFreqStop = calParam.rfFreqStop;
        QString strFreqStep = calParam.rfFreqStep;
        quint64 freq[RF_TX_FREQ_PTS_CALLED];
        quint64 freqStar[16];
        quint64 freqStop[16];
        memset(freq,0,sizeof(freq));
        memset(freqStar,0,sizeof(freqStar));
        memset(freqStop,0,sizeof(freqStop));
        qint32 pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

        CAL_THREAD_START("Tx Att",pts);

        CalParam::CalIOMode calMode = calParam.mode;

        if (calOP(calMode))
            THREAD_CHECK_BOX("Tx<===>Spectrum");
        if (calIO(calMode) && !calOP(calMode))
            THREAD_CHECK_BOX("IO<===>Spectrum");

        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

        double pwrSA[2] = {0.0,0.0};
        double pwr = R1C_TX_BASE_POWER_OP;
        qint32 idxOffset = 0;
        qint32 idxOffsetStar = 0;
        qint32 secBfr = -1;
        qint32 secCur = 0;

        tx_lol_table_r1cd::data_m_t dataLOL;
        tx_sb_table_r1cd::data_m_t dataSB;
        tx_filter_160m_table::data_m_t dataFilter;
        double coefReal[TX_FILTER_ORDER_2I] = {0.0};
        double coefImag[TX_FILTER_ORDER_2I] = {0.0};

        Instr.init();
        Instr.has_sa();
        Instr.sa_reset();
        Instr.sa_set_span(20000.0);
        Instr.sa_set_rbw(1000.0,true);
        Instr.sa_set_vbw(1000.0,true);

        pSP1401->get_cal_file()->set_bw(_160M);
        pSP1401->set_rx_att_019_sw(sp1401::RX_ATT_19);
        pSP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
        pSP1401->set_rx_att(30.0,30.0,30.0);
        pSP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
        pSP2401->set_dds1(20000000.0);
        pSP2401->set_duc_dds(0.0);
        pSP2401->set_tx_filter_sw(true);

        pSP1401->get_cal_file()->m_tx_sb->get(freq[0],&dataSB);
        pSP1401->get_cal_file()->m_tx_lol->get(freq[0],&dataLOL);
        pSP2401->set_tx_phase_rotate_I((double)(dataSB.th));
        pSP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
        pSP2401->set_tx_dc_offset((uint16_t)(dataLOL.dc_i),(uint16_t)(dataLOL.dc_q));

        if (calOP(calMode)) {
            tx_att_op_table_r1cd::data_f_t data;
            tx_pwr_op_table_r1cd::data_m_t dataPwrOP;
            QR1CTxAttOPModel *model_op = (QR1CTxAttOPModel *)(calParam.model_0);

            pSP1401->set_io_mode(sp1401::OUTPUT);

            Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 25.0);		//10 ~ -5
            Instr.sa_set_en_preamp(false);
            Instr.sa_set_mech_att(true);
            Instr.sa_set_avg_trace(false,0);

            for (int32_t i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL

                data.freq = freq[i];
                pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_tx_filter(coefReal,coefImag);
                pSP1401->set_tx_freq(freq[i]);
                pSP1401->get_cal_file()->m_tx_pwr_op->get(freq[i],R1C_TX_BASE_POWER_OP,&dataPwrOP);
                pSP1401->set_tx_att0(dataPwrOP.att0 / 2.0);
                pSP1401->set_tx_att1(dataPwrOP.att1 / 2.0);
                pSP1401->set_tx_att2(dataPwrOP.att2 / 2.0);
                pSP1401->set_tx_att3(dataPwrOP.att3 / 2.0);
                pSP2401->set_tx_pwr_comp(dataPwrOP.d_gain);
                msleep(2);

                Instr.sa_set_cf(freq[i] + 20000000.0);
                Instr.sa_sweep_once();
                Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
                Instr.sa_get_marker_pwr(pwrSA[0]);

                idxOffset = idxOffsetStar;
                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                for (pwr = R1C_TX_ATT_OP_POWER_STAR;pwr > R1C_TX_BASE_POWER_OP;pwr --) {
                    CAL_THREAD_TEST_CANCEL
                    
                    pSP1401->get_cal_file()->m_tx_pwr_op->get(freq[i],pwr,&dataPwrOP);
                    pSP1401->set_tx_att0(dataPwrOP.att0 / 2.0);
                    pSP1401->set_tx_att1(dataPwrOP.att1 / 2.0);
                    pSP1401->set_tx_att2(dataPwrOP.att2 / 2.0);
                    pSP1401->set_tx_att3(dataPwrOP.att3 / 2.0);
                    pSP2401->set_tx_pwr_comp(dataPwrOP.d_gain);
                    msleep(2);
                    Instr.sa_sweep_once();
                    Instr.sa_get_marker_pwr(pwrSA[1]);

                    data.offset[idxOffset] = (float)((pwr - R1C_TX_BASE_POWER_OP) - (pwrSA[1] - pwrSA[0]));

                    if (secCur != secBfr)
                        model_op->iterTable(idxOffset)->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                    model_op->calTable()->replace(i, data);
                    model_op->iterTable(idxOffset)->at(secCur)->addOneData();
                    emit update(model_op->index(i * R1C_TX_ATT_OP_POWER_PTS, 0),
                                model_op->index((i + 1) * R1C_TX_ATT_OP_POWER_PTS, 13),
                                cal_file::TX_ATT_OP,
                                secCur);

                    idxOffset ++;
                }
                secBfr = secCur;

                pSP1401->get_temp4_tx_lo2(&data.temp[0]);
                pSP1401->get_temp5_tx_lo1(&data.temp[1]);
                pSP1401->get_temp6_tx_pa4(&data.temp[2]);
                pSP1401->get_temp7_tx_pa3(&data.temp[3]);
                data.time = getCurTime();

                pSP1401->get_cal_file()->add(cal_file::TX_ATT_OP,&data);
                SET_PROG_POS(i + 1);
            }
            idxOffsetStar = idxOffset;
            pSP1401->get_cal_file()->w(cal_file::TX_ATT_OP);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0);
            msleep(10);
            calOneSecOP(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_OP,R1C_TX_BASE_POWER_OP - 20.0);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0 - 20.0);
            msleep(10);
            calOneSecOP(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 1,R1C_TX_BASE_POWER_OP - 20.0 * 2);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0 - 20.0 * 2);
            msleep(10);
            calOneSecOP(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 2,R1C_TX_BASE_POWER_OP - 20.0 * 3);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0 - 20.0 * 3);
            Instr.sa_set_en_preamp(true);
            Instr.sa_set_mech_att(false,0);
            Instr.sa_set_avg_trace(true,3);
            msleep(10);
            calOneSecOP(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 3,R1C_TX_BASE_POWER_OP - 20.0 * 4);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0 - 20.0 * 4);
            Instr.sa_set_avg_trace(true,6);
            msleep(10);
            calOneSecOP(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 4,R1C_TX_ATT_OP_POWER_STOP);
        }

        if (calOP(calMode) && calIO(calMode)) {
            THREAD_CHECK_BOX("Output Complete,IO<===>Sepctrum");
        }

        if (calIO(calMode)) {
            tx_att_io_table_r1cd::data_f_t data;
            tx_pwr_io_table_r1cd::data_m_t dataPwrIO;
            QR1CTxAttIOModel *model_io = (QR1CTxAttIOModel *)(calParam.model_1);

            pSP1401->set_io_mode(sp1401::IO);

            Instr.sa_reset();
            Instr.sa_set_span(20000.0);
            Instr.sa_set_rbw(1000.0,true);
            Instr.sa_set_vbw(1000.0,true);
            Instr.sa_set_en_preamp(false);
            Instr.sa_set_mech_att(true);
            Instr.sa_set_avg_trace(false,0);

            idxOffset = idxOffsetStar = 0;
            secBfr = -1;
            secCur = 0;

            Instr.sa_set_ref(R1C_TX_BASE_POWER_IO + 25.0);		//0 ~ -12
            Instr.sa_set_en_preamp(false);
            Instr.sa_set_mech_att(true);
            Instr.sa_set_avg_trace(false,0);

            for (int32_t i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL

                data.freq = freq[i];
                pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_tx_filter(coefReal,coefImag);
                pSP1401->set_tx_freq(freq[i]);
                Instr.sa_set_cf(freq[i] + 20000000.0);

                pSP1401->get_cal_file()->m_tx_pwr_io->get(freq[i],R1C_TX_BASE_POWER_IO,&dataPwrIO);
                pSP1401->set_tx_att0(dataPwrIO.att0 / 2.0);
                pSP1401->set_tx_att1(dataPwrIO.att1 / 2.0);
                pSP1401->set_tx_att2(dataPwrIO.att2 / 2.0);
                pSP1401->set_tx_att3(dataPwrIO.att3 / 2.0);
                pSP2401->set_tx_pwr_comp(dataPwrIO.d_gain);
                msleep(2);

                Instr.sa_sweep_once();
                Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
                Instr.sa_get_marker_pwr(pwrSA[0]);

                idxOffset = idxOffsetStar;
                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                for (pwr = R1C_TX_ATT_IO_POWER_STAR;pwr > R1C_TX_BASE_POWER_IO;pwr --) {
                    CAL_THREAD_TEST_CANCEL

                    pSP1401->get_cal_file()->m_tx_pwr_io->get(freq[i],pwr,&dataPwrIO);
                    pSP1401->set_tx_att0(dataPwrIO.att0 / 2.0);
                    pSP1401->set_tx_att1(dataPwrIO.att1 / 2.0);
                    pSP1401->set_tx_att2(dataPwrIO.att2 / 2.0);
                    pSP1401->set_tx_att3(dataPwrIO.att3 / 2.0);
                    pSP2401->set_tx_pwr_comp(dataPwrIO.d_gain);
                    msleep(2);

                    Instr.sa_sweep_once();
                    Instr.sa_get_marker_pwr(pwrSA[1]);
                    data.offset[idxOffset] = (float)((pwr - R1C_TX_BASE_POWER_IO) - (pwrSA[1] - pwrSA[0]));

                    if (secCur != secBfr)
                        model_io->iterTable(idxOffset)->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                    model_io->calTable()->replace(i, data);
                    model_io->iterTable(idxOffset)->at(secCur)->addOneData();
                    emit update(model_io->index(i * R1C_TX_ATT_IO_POWER_PTS, 0),
                                model_io->index((i + 1) * R1C_TX_ATT_IO_POWER_PTS, 13),
                                cal_file::TX_ATT_IO,
                                secCur);

                    idxOffset ++;
                }
                secBfr = secCur;

                pSP1401->get_temp4_tx_lo2(&data.temp[0]);
                pSP1401->get_temp5_tx_lo1(&data.temp[1]);
                pSP1401->get_temp6_tx_pa4(&data.temp[2]);
                pSP1401->get_temp7_tx_pa3(&data.temp[3]);
                data.time = getCurTime();

                pSP1401->get_cal_file()->add(cal_file::TX_ATT_IO,&data);
                SET_PROG_POS(i + 1);
            }
            idxOffsetStar = idxOffset;
            pSP1401->get_cal_file()->w(cal_file::TX_ATT_IO);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_IO + 20.0);
            msleep(10);
            calOneSecIO(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_IO,R1C_TX_BASE_POWER_IO - 20.0);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_IO + 20.0 - 20.0);
            msleep(10);
            calOneSecIO(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_IO - 20.0 * 1,R1C_TX_BASE_POWER_IO - 20.0 * 2);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_IO + 20.0 - 20.0 * 2);
            Instr.sa_set_en_preamp(true);
            Instr.sa_set_mech_att(false,0);
            Instr.sa_set_avg_trace(true,3);
            msleep(10);
            calOneSecIO(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_IO - 20.0 * 2,R1C_TX_BASE_POWER_IO - 20.0 * 3);

            CAL_THREAD_TEST_CANCEL
            Instr.sa_set_ref(R1C_TX_BASE_POWER_IO + 20.0 - 20.0 * 3);
            Instr.sa_set_avg_trace(true,6);
            msleep(10);
            calOneSecIO(pts,freq,freqStar,freqStop,&idxOffsetStar,R1C_TX_BASE_POWER_IO - 20.0 * 3,R1C_TX_ATT_IO_POWER_STOP);
        }
        CAL_THREAD_ABOART
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
    return;
}

void QCalR1CTxAttThread::calOneSecOP(int32_t pts, uint64_t *freq, uint64_t *freqStar, uint64_t *freqStop, int32_t *idxOffsetStar, double pwrStar, double pwrStop)
{
    tx_att_op_table_r1cd::data_f_t data;
    tx_pwr_op_table_r1cd::data_m_t dataPwrOP;
    tx_filter_160m_table::data_m_t dataFilter;
    QR1CTxAttOPModel *model_op = (QR1CTxAttOPModel *)(calParam.model_0);
    double coefReal[TX_FILTER_ORDER_2I] = {0.0};
    double coefImag[TX_FILTER_ORDER_2I] = {0.0};

    double pwrStep = R1C_TX_ATT_STEP;
    double pwrSA[2] = {0.0,0.0};
    double pwr = 0.0;
    qint32 idxOffset = 0;
    qint32 secBfr = -1;
    qint32 secCur = 0;
    float offsetBase = 0.0;

    sp2401_r1a *pSP2401 = calParam._sp2401;
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

    for (int32_t i = 0;i < pts;i ++) {
        CAL_THREAD_TEST_CANCEL

        data.freq = freq[i];
        pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter);
        dataFilter._2double(coefReal,coefImag);
        pSP2401->set_tx_filter(coefReal,coefImag);
        pSP1401->set_tx_freq(freq[i]);
        Instr.sa_set_cf(freq[i] + 20000000.0);

        pSP1401->get_cal_file()->m_tx_att_op->get(freq[i],&data);
        offsetBase = (R1C_TX_BASE_POWER_OP == pwrStar ? 0.0f : data.offset[*idxOffsetStar - 1]);
        pSP1401->get_cal_file()->m_tx_pwr_op->get(freq[i],pwrStar,&dataPwrOP);
        pSP1401->set_tx_att0(dataPwrOP.att0 / 2.0);
        pSP1401->set_tx_att1(dataPwrOP.att1 / 2.0);
        pSP1401->set_tx_att2(dataPwrOP.att2 / 2.0);
        pSP1401->set_tx_att3(dataPwrOP.att3 / 2.0);
        pSP2401->set_tx_pwr_comp(dataPwrOP.d_gain);
        msleep(2);

        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
        Instr.sa_get_marker_pwr(pwrSA[0]);

        idxOffset = *idxOffsetStar;
        secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
        for (pwr = pwrStar + pwrStep;pwr >= pwrStop;pwr += pwrStep) {
            CAL_THREAD_TEST_CANCEL

            pSP1401->get_cal_file()->m_tx_pwr_op->get(freq[i],pwr,&dataPwrOP);
            pSP1401->set_tx_att0(dataPwrOP.att0 / 2.0);
            pSP1401->set_tx_att1(dataPwrOP.att1 / 2.0);
            pSP1401->set_tx_att2(dataPwrOP.att2 / 2.0);
            pSP1401->set_tx_att3(dataPwrOP.att3 / 2.0);
            pSP2401->set_tx_pwr_comp(dataPwrOP.d_gain);
            msleep(2);

            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA[1]);

            data.offset[idxOffset] = (float)((pwr - pwrStar) - (pwrSA[1] - pwrSA[0]));
            data.offset[idxOffset] += offsetBase;

            if (secCur != secBfr)
                model_op->iterTable(idxOffset)->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
            model_op->calTable()->replace(i, data);
            model_op->iterTable(idxOffset)->at(secCur)->addOneData();
            emit update(model_op->index(i * R1C_TX_ATT_OP_POWER_PTS, 0),
                        model_op->index((i + 1) * R1C_TX_ATT_OP_POWER_PTS, 13),
                        cal_file::TX_ATT_OP,
                        secCur);

            idxOffset ++;
        }
        secBfr = secCur;

        pSP1401->get_temp4_tx_lo2(&data.temp[0]);
        pSP1401->get_temp5_tx_lo1(&data.temp[1]);
        pSP1401->get_temp6_tx_pa4(&data.temp[2]);
        pSP1401->get_temp7_tx_pa3(&data.temp[3]);
        data.time = getCurTime();

        pSP1401->get_cal_file()->add(cal_file::TX_ATT_OP,&data);
        SET_PROG_POS(i + 1);
    }
    *idxOffsetStar = idxOffset;
    pSP1401->get_cal_file()->w(cal_file::TX_ATT_OP);
    return;
}

void QCalR1CTxAttThread::calOneSecIO(int32_t pts, uint64_t *freq, uint64_t *freqStar, uint64_t *freqStop, int32_t *idxOffsetStar, double pwrStar, double pwrStop)
{
    tx_att_io_table_r1cd::data_f_t data;
    tx_pwr_io_table_r1cd::data_m_t dataPwrIO;
    tx_filter_160m_table::data_m_t dataFilter;
    QR1CTxAttIOModel *model_io = (QR1CTxAttIOModel *)(calParam.model_1);
    double coefReal[TX_FILTER_ORDER_2I] = {0.0};
    double coefImag[TX_FILTER_ORDER_2I] = {0.0};

    double pwrStep = R1C_TX_ATT_STEP;
    double pwrSA[2] = {0.0,0.0};
    double pwr = 0.0;
    qint32 idxOffset = 0;
    qint32 secBfr = -1;
    qint32 secCur = 0;
    float offsetBase = 0.0;

    sp2401_r1a *pSP2401 = calParam._sp2401;
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

    for (int32_t i = 0;i < pts;i ++) {
        CAL_THREAD_TEST_CANCEL

        data.freq = freq[i];
        pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter);
        dataFilter._2double(coefReal,coefImag);
        pSP2401->set_tx_filter(coefReal,coefImag);
        pSP1401->set_tx_freq(freq[i]);
        Instr.sa_set_cf(freq[i] + 20000000.0);

        pSP1401->get_cal_file()->m_tx_att_io->get(freq[i],&data);
        offsetBase = R1C_TX_BASE_POWER_IO == pwrStar ? 0.0f : data.offset[*idxOffsetStar - 1];
        pSP1401->get_cal_file()->m_tx_pwr_io->get(freq[i],pwrStar,&dataPwrIO);
        pSP1401->set_tx_att0(dataPwrIO.att0 / 2.0);
        pSP1401->set_tx_att1(dataPwrIO.att1 / 2.0);
        pSP1401->set_tx_att2(dataPwrIO.att2 / 2.0);
        pSP1401->set_tx_att3(dataPwrIO.att3 / 2.0);
        pSP2401->set_tx_pwr_comp(dataPwrIO.d_gain);
        msleep(2);

        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
        Instr.sa_get_marker_pwr(pwrSA[0]);

        idxOffset = *idxOffsetStar;
        secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
        for (pwr = pwrStar + pwrStep;pwr >= pwrStop;pwr += pwrStep) {
            CAL_THREAD_TEST_CANCEL

            pSP1401->get_cal_file()->m_tx_pwr_io->get(freq[i],pwr,&dataPwrIO);
            pSP1401->set_tx_att0(dataPwrIO.att0 / 2.0);
            pSP1401->set_tx_att1(dataPwrIO.att1 / 2.0);
            pSP1401->set_tx_att2(dataPwrIO.att2 / 2.0);
            pSP1401->set_tx_att3(dataPwrIO.att3 / 2.0);
            pSP2401->set_tx_pwr_comp(dataPwrIO.d_gain);
            msleep(2);

            Instr.sa_sweep_once();
            Instr.sa_get_marker_pwr(pwrSA[1]);
            data.offset[idxOffset] = (float)((pwr - pwrStar) - (pwrSA[1] - pwrSA[0]));
            data.offset[idxOffset] += offsetBase;

            if (secCur != secBfr)
                model_io->iterTable(idxOffset)->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
            model_io->calTable()->replace(i, data);
            model_io->iterTable(idxOffset)->at(secCur)->addOneData();
            emit update(model_io->index(i * R1C_TX_ATT_IO_POWER_PTS, 0),
                        model_io->index((i + 1) * R1C_TX_ATT_IO_POWER_PTS, 13),
                        cal_file::TX_ATT_IO,
                        secCur);

            idxOffset ++;
        }
        secBfr = secCur;

        pSP1401->get_temp4_tx_lo2(&data.temp[0]);
        pSP1401->get_temp5_tx_lo1(&data.temp[1]);
        pSP1401->get_temp6_tx_pa4(&data.temp[2]);
        pSP1401->get_temp7_tx_pa3(&data.temp[3]);
        data.time = getCurTime();

        pSP1401->get_cal_file()->add(cal_file::TX_ATT_IO,&data);
        SET_PROG_POS(i + 1);
    }
    *idxOffsetStar = idxOffset;
    pSP1401->get_cal_file()->w(cal_file::TX_ATT_IO);
    return;
}

QExportR1CTxAttThread::QExportR1CTxAttThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CTxAttThread::run()
{
    INIT_PROG("Exporting Tx Att",100);

    QString strFreqStar = calParam.rfFreqStar;
    QString strFreqStop = calParam.rfFreqStop;
    QString strFreqStep = calParam.rfFreqStep;
    CalParam::CalIOMode calMode = calParam.mode;
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    int secBfr = -1;
    int secCur = 0;

    quint64 freq[RF_TX_FREQ_PTS_CALLED];
    quint64 freqStar[16];
    quint64 freqStop[16];
    memset(freq,0,sizeof(freq));
    memset(freqStar,0,sizeof(freqStar));
    memset(freqStop,0,sizeof(freqStop));
    int pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

    if (calOP(calMode)) {
        tx_att_op_table_r1cd::data_f_t dataOP;
        QR1CTxAttOPModel *modelOP = (QR1CTxAttOPModel *)(calParam.model_0);

        pSP1401->get_cal_file()->map2buf(cal_file::TX_ATT_OP);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_tx_att_op->get(freq[i], &dataOP);

            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < R1C_TX_ATT_OP_POWER_PTS;j ++) {
                if (secCur != secBfr)
                    modelOP->iterTable(j)->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                modelOP->iterTable(j)->at(secCur)->addOneData();
            }

            modelOP->calTable()->replace(i, dataOP);
            secBfr = secCur;
        }

        emit update(modelOP->index(0, 0),
                    modelOP->index(pts * R1C_TX_ATT_OP_POWER_PTS, 13),
                    cal_file::TX_ATT_OP,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        tx_att_io_table_r1cd::data_f_t dataIO;
        QR1CTxAttIOModel *modelIO = (QR1CTxAttIOModel *)(calParam.model_1);

        pSP1401->get_cal_file()->map2buf(cal_file::TX_ATT_IO);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_tx_att_io->get(freq[i], &dataIO);

            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < R1C_TX_ATT_IO_POWER_PTS;j ++) {
                if (secCur != secBfr)
                    modelIO->iterTable(j)->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                modelIO->iterTable(j)->at(secCur)->addOneData();
            }

            modelIO->calTable()->replace(i, dataIO);
            secBfr = secCur;
        }

        emit update(modelIO->index(0, 0),
                    modelIO->index(pts * R1C_TX_ATT_IO_POWER_PTS, 13),
                    cal_file::TX_ATT_IO,
                    secCur);
    }

    SET_PROG_POS(100);
    THREAD_ABORT
}
