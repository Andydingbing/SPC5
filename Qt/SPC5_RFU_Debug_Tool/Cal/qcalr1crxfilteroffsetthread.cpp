#include "qcalr1crxfilteroffsetthread.h"
#include "qcalr1crxfilteroffsetdlg.h"
#include "qr1crxfilteroffsetmodel.h"

QCalR1CRxFilterOffsetThread::QCalR1CRxFilterOffsetThread(CalParam *param, QObject *parent) :
    QCalBaseThread(param,parent)
{

}

void QCalR1CRxFilterOffsetThread::run()
{
#define POST_CLEAR pSP1401->get_cal_file()->set_bw(_160M);

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

        CAL_THREAD_START("Rx Filter Offset",pts);
        THREAD_CHECK_BOX("Rx<===>Z28<===>Signal Generator");

        CalParam::CalIOMode calMode = calParam.mode;

        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

        qint64 ad[2] = {0,0};
        double pwrSG[3] = {10.0 + PM_IL,10.0 + PM_IL,-20.0 + PM_IL};
        for (qint32 i = 0;i < ARRAY_SIZE(pwrSG);i ++)	//for preventing saturation
            pwrSG[i] -= 10.0;

        qint32 secBfr = -1;
        qint32 secCur = 0;

        rx_filter_160m_table::data_m_t dataFilter_160;
        rx_filter_80m_table::data_m_t  dataFilter_80;
        double coefReal[RX_FILTER_ORDER] = {0.0};
        double coefImag[RX_FILTER_ORDER] = {0.0};

        Instr.init();
        Instr.has_pm();
        Instr.has_sg();
        Instr.pm_reset();
        Instr.sg_reset();
        Instr.sg_set_en_mod(false);
        Instr.sg_set_pl(-60.0);
        Instr.sg_set_en_output(true);

        pSP1401->get_cal_file()->set_bw(_80M);
        pSP1401->get_cal_file()->map2buf(cal_file::RX_FILTER_160);
        pSP1401->get_cal_file()->m_rx_filter_160m->map2mem();

        pSP1401->set_tx_att(30.0,30.0,30.0,30.0);
        pSP1401->set_rx_if_filter_sw(basic_sp1401::_160MHz);
        pSP2401->set_tx_pwr_comp(-5.0);
        pSP2401->set_rx_pwr_comp(0);
        pSP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);

        rx_filter_offset_table::data_f_t data;
        const qint32 offsetCnt = ARRAY_SIZE(data.offset);

        if (calOP(calMode)) {
            QR1CRxFilterOffsetModel *model = (QR1CRxFilterOffsetModel *)(calParam.model_0);
            pSP1401->set_io_mode(OUTPUT);

            rx_ref_op_table_r1cd::data_m_t dataRef;

            for (qint32 i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
                Instr.sg_set_cw(double(freq[i]));
                pSP1401->set_rx_freq(freq[i]);
                pSP1401->get_cal_file()->m_rx_ref_op->get_base(freq[i],&dataRef);
                pSP1401->get_cal_file()->m_rx_filter_160m->get(freq[i],&dataFilter_160);
                pSP1401->get_cal_file()->m_rx_filter_80m->get(freq[i],&dataFilter_80);

                for (qint32 j = 0;j < ARRAY_SIZE(data.offset);j ++) {
                    CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
                    Instr.sg_set_pl(pwrSG[j]);
                    dataFilter_160._2double(coefReal,coefImag);
                    pSP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
                    pSP2401->set_rx_filter(coefReal,coefImag);
                    pSP1401->set_rx_lna_att_sw(sp1401::rx_lna_att_t(dataRef.state[j].lna_att));
                    pSP1401->set_rx_att_019_sw(sp1401::rx_att_019_t(dataRef.state[j].att_019));
                    pSP1401->set_rx_att1(double(dataRef.state[j].att1));
                    pSP1401->set_rx_att2(double(dataRef.state[j].att2));
                    pSP1401->set_rx_att3(double(dataRef.state[j].att3));
                    pSP2401->set_rx_pwr_comp(dataRef.state[j].ad_offset);
                    msleep(50);

                    getADS5474(pSP1401,ad[0]);

                    dataFilter_80._2double(coefReal,coefImag);
                    pSP2401->set_rx_filter_sw(sp2401_r1a::_3D);
                    pSP2401->set_rx_filter(coefReal,coefImag);
                    msleep(50);

                    getADS5474(pSP1401,ad[1]);
                    data.offset[j] = _0dBFS - dBc2ad(_0dBFS,ad2dBc(ad[1],ad[0]));
                }

                data.freq = freq[i];
                pSP1401->get_temp0_rx_lo2(&data.temp[0]);
                pSP1401->get_temp1_rx_lna(&data.temp[1]);
                pSP1401->get_temp2_rx_lo1(&data.temp[2]);
                pSP1401->get_temp3_rx_pa1(&data.temp[3]);
                data.time = getCurTime();

                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                for (qint32 j = 0;j < offsetCnt;j ++) {
                    if(secCur != secBfr)
                        model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                    model->iterTable(j)->at(secCur)->addOneData();
                }
                secBfr = secCur;

                model->calTable()->replace(i,data);
                pSP1401->get_cal_file()->add(cal_file::RX_FILTER_OFFSET_OP_80,&data);
                emit update(model->index(i,0),
                            model->index((i + 1),7),
                            cal_file::RX_FILTER_OFFSET_OP_80,
                            secCur);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_FILTER_OFFSET_OP_80);
            pSP1401->get_cal_file()->m_rx_filter_offset_op_80m->save_as("c:\\rx_filter_off_op_80.txt");
        }

        secBfr = -1;
        secCur = 0;

        if (calIO(calMode)) {
            QR1CRxFilterOffsetModel *model = (QR1CRxFilterOffsetModel *)(calParam.model_1);
            pSP1401->set_io_mode(OUTPUT);

            rx_ref_io_table_r1cd::data_m_t dataRef;

            for (qint32 i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
                Instr.sg_set_cw(double(freq[i]));
                pSP1401->set_rx_freq(freq[i]);
                pSP1401->get_cal_file()->m_rx_ref_op->get_base(freq[i],&dataRef);
                pSP1401->get_cal_file()->m_rx_filter_160m->get(freq[i],&dataFilter_160);
                pSP1401->get_cal_file()->m_rx_filter_80m->get(freq[i],&dataFilter_80);

                for (qint32 j = 0;j < ARRAY_SIZE(data.offset);j ++) {
                    CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
                    Instr.sg_set_pl(pwrSG[j]);
                    dataFilter_160._2double(coefReal,coefImag);
                    pSP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
                    pSP2401->set_rx_filter(coefReal,coefImag);
                    pSP1401->set_rx_lna_att_sw(sp1401::rx_lna_att_t(dataRef.state[j].lna_att));
                    pSP1401->set_rx_att_019_sw(sp1401::rx_att_019_t(dataRef.state[j].att_019));
                    pSP1401->set_rx_att1(double(dataRef.state[j].att1));
                    pSP1401->set_rx_att2(double(dataRef.state[j].att2));
                    pSP1401->set_rx_att3(double(dataRef.state[j].att3));
                    pSP2401->set_rx_pwr_comp(dataRef.state[j].ad_offset);
                    msleep(50);

                    getADS5474(pSP1401,ad[0]);

                    dataFilter_80._2double(coefReal,coefImag);
                    pSP2401->set_rx_filter_sw(sp2401_r1a::_3D);
                    pSP2401->set_rx_filter(coefReal,coefImag);
                    msleep(50);

                    getADS5474(pSP1401,ad[1]);
                    data.offset[j] = _0dBFS - dBc2ad(_0dBFS,ad2dBc(ad[1],ad[0]));
                }

                data.freq = freq[i];
                pSP1401->get_temp0_rx_lo2(&data.temp[0]);
                pSP1401->get_temp1_rx_lna(&data.temp[1]);
                pSP1401->get_temp2_rx_lo1(&data.temp[2]);
                pSP1401->get_temp3_rx_pa1(&data.temp[3]);
                data.time = getCurTime();

                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                for (qint32 j = 0;j < offsetCnt;j ++) {
                    if(secCur != secBfr)
                        model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                    model->iterTable(j)->at(secCur)->addOneData();
                }
                secBfr = secCur;

                model->calTable()->replace(i,data);
                pSP1401->get_cal_file()->add(cal_file::RX_FILTER_OFFSET_IO_80,&data);
                emit update(model->index(i,0),
                            model->index((i + 1),7),
                            cal_file::RX_FILTER_OFFSET_IO_80,
                            secCur);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_FILTER_OFFSET_IO_80);
            pSP1401->get_cal_file()->m_rx_filter_offset_io_80m->save_as("c:\\rx_filter_off_io_80.txt");
        }
        CAL_THREAD_ABOART_S(POST_CLEAR);
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}

QExportR1CRxFilterOffsetThread::QExportR1CRxFilterOffsetThread(CalParam *param, QObject *parent) :
    QExportBaseThread(param,parent)
{

}

void QExportR1CRxFilterOffsetThread::run()
{
    INIT_PROG("Exporting Rx Filter Offset",100);

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
    qint32 pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

    rx_filter_offset_table::data_f_t data;
    const qint32 offsetCnt = ARRAY_SIZE(data.offset);

    pSP1401->get_cal_file()->set_bw(_80M);

    if (calOP(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::RX_FILTER_OFFSET_OP_80);
        QR1CRxFilterOffsetModel *model = (QR1CRxFilterOffsetModel *)(calParam.model_0);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_rx_filter_offset_op_80m->get(freq[i],&data);

            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < offsetCnt;j ++) {
                if(secCur != secBfr)
                    model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                model->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;

            model->calTable()->replace(i,data);
        }

        emit update(model->index(0,0),
                    model->index(pts * offsetCnt,7),
                    cal_file::RX_FILTER_OFFSET_OP_80,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::RX_FILTER_OFFSET_IO_80);
        QR1CRxFilterOffsetModel *model = (QR1CRxFilterOffsetModel *)(calParam.model_1);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_rx_filter_offset_io_80m->get(freq[i],&data);

            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < offsetCnt;j ++) {
                if(secCur != secBfr)
                    model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                model->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;

            model->calTable()->replace(i,data);
        }

        emit update(model->index(0,0),
                    model->index(pts * offsetCnt,7),
                    cal_file::RX_FILTER_OFFSET_IO_80,
                    secCur);
    }

    pSP1401->get_cal_file()->set_bw(_160M);

    SET_PROG_POS(100);
    THREAD_ABORT
}
