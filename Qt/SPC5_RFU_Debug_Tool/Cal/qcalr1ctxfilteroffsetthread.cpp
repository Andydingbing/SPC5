#include "qcalr1ctxfilteroffsetthread.h"
#include "qcalr1ctxfilteroffsetdlg.h"
#include "qr1ctxfilteroffsetmodel.h"

QCalR1CTxFilterOffsetThread::QCalR1CTxFilterOffsetThread(CalParam *param, QObject *parent) :
    QCalBaseThread(param,parent)
{

}

void QCalR1CTxFilterOffsetThread::run()
{
#define POST_CLEAR pSP1401->get_cal_file()->set_bw(_160M);

    try {
        QString strFreqStar = calParam.rfFreqStar;
        QString strFreqStop = calParam.rfFreqStop;
        QString strFreqStep = calParam.rfFreqStep;
        uint64_t freq[RF_TX_FREQ_PTS_CALLED];
        uint64_t freqStar[16];
        uint64_t freqStop[16];
        memset(freq,0,sizeof(freq));
        memset(freqStar,0,sizeof(freqStar));
        memset(freqStop,0,sizeof(freqStop));
        int pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

        CAL_THREAD_START("Tx Filter Offset",pts);

        CalParam::CalIOMode calMode = calParam.mode;

        if (calOP(calMode))
            THREAD_CHECK_BOX("Tx<===>Spectrum");
        if (calIO(calMode) && !calOP(calMode))
            THREAD_CHECK_BOX("IO<===>Spectrum");

        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

        Instr.init();
        Instr.has_sa();
        Instr.sa_reset();
        Instr.sa_set_span(20000.0);
        Instr.sa_set_rbw(1000.0,true);
        Instr.sa_set_vbw(1000.0,true);
        Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0);

        double pwr[2] = {0.0,0.0};
        double dds1 = 0;
        int secBfr = -1;
        int secCur = 0;

        tx_lol_table_r1cd::data_m_t	dataLOL;
        tx_sb_table_r1cd::data_m_t	dataSB;
        tx_filter_160m_table::data_m_t dataFilter_160;
        tx_filter_80m_table::data_m_t  dataFilter_80;
        double coefReal[TX_FILTER_ORDER_2I] = {0.0};
        double coefImag[TX_FILTER_ORDER_2I] = {0.0};

        pSP1401->get_cal_file()->set_bw(_80M);
        pSP1401->get_cal_file()->map2buf(cal_file::TX_FILTER_160);
        pSP1401->get_cal_file()->m_tx_filter_160m->map2mem();

        pSP1401->set_rx_att_019_sw(sp1401::RX_ATT_19);
        pSP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
        pSP1401->set_rx_att(30.0,30.0,30.0);
        pSP2401->set_tx_filter_sw(true);
        pSP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
        pSP2401->set_dds1(dds1);
        pSP2401->set_duc_dds(0.0);

        pSP1401->get_cal_file()->m_tx_sb->get(freq[0],&dataSB);
        pSP1401->get_cal_file()->m_tx_lol->get(freq[0],&dataLOL);
        pSP2401->set_tx_phase_rotate_I(double(dataSB.th));
        pSP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
        pSP2401->set_tx_dc_offset(uint16_t(dataLOL.dc_i),uint16_t(dataLOL.dc_q));

        tx_filter_offset_table::data_f_t data;

        if (calOP(calMode)) {
            QR1CTxFilterOffsetModel *modelOP = (QR1CTxFilterOffsetModel *)(calParam.model_0);
            pSP1401->set_io_mode(OUTPUT);

            tx_pwr_op_table_r1cd::data_m_t dataPwr;

            for (int32_t i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);

                pSP1401->set_tx_freq(freq[i]);
                Instr.sa_set_cf(freq[i] + dds1);

                pSP1401->get_cal_file()->m_tx_pwr_op->get(freq[i],R1C_TX_BASE_POWER_OP,&dataPwr);
                pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter_160);
                pSP1401->get_cal_file()->m_tx_filter_80m->get(freq[i],&dataFilter_80);
                dataFilter_160._2double(coefReal,coefImag);

                pSP1401->set_tx_att0(dataPwr.att0 / 2.0);
                pSP1401->set_tx_att1(dataPwr.att1 / 2.0);
                pSP1401->set_tx_att2(dataPwr.att2 / 2.0);
                pSP1401->set_tx_att3(dataPwr.att3 / 2.0);
                pSP2401->set_tx_pwr_comp(double(dataPwr.d_gain));
                pSP2401->set_tx_filter(coefReal,coefImag);
                msleep(50);

                Instr.sa_sweep_once();
                Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
                Instr.sa_get_marker_pwr(pwr[0]);

                dataFilter_80._2double(coefReal,coefImag);
                pSP2401->set_tx_filter(coefReal,coefImag);
                msleep(50);

                Instr.sa_sweep_once();
                Instr.sa_get_marker_pwr(pwr[1]);

                data.freq = freq[i];
                data.offset = pwr[0] - pwr[1];
                pSP1401->get_temp4_tx_lo2(&data.temp[0]);
                pSP1401->get_temp5_tx_lo1(&data.temp[1]);
                pSP1401->get_temp6_tx_pa4(&data.temp[2]);
                pSP1401->get_temp7_tx_pa3(&data.temp[3]);
                data.time = getCurTime();
                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                if (secCur != secBfr) {
                    modelOP->iterTable()->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                    secBfr = secCur;
                }
                modelOP->calTable()->replace(i,data);
                modelOP->iterTable()->at(secCur)->addOneData();
                pSP1401->get_cal_file()->add(cal_file::TX_FILTER_OFFSET_OP_80,&data);
                emit update(modelOP->index(i,0),modelOP->index(i,7),cal_file::TX_FILTER_OFFSET_OP_80,secCur);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::TX_FILTER_OFFSET_OP_80);
            pSP1401->get_cal_file()->m_tx_filter_offset_op_80m->save_as("c:\\tx_filter_off_op_80.txt");
        }

        if (calOP(calMode) && calIO(calMode))
            THREAD_CHECK_BOX("Output Complete,IO<===>Spectrum");

        secBfr = -1;
        secCur = 0;

        if (calIO(calMode)) {
            Instr.sa_reset();
            Instr.sa_set_span(20000.0);
            Instr.sa_set_rbw(1000.0,true);
            Instr.sa_set_vbw(1000.0,true);
            Instr.sa_set_ref(R1C_TX_BASE_POWER_IO + 20.0);

            QR1CTxFilterOffsetModel *modelIO = (QR1CTxFilterOffsetModel *)(calParam.model_1);
            pSP1401->set_io_mode(IO);

            tx_pwr_io_table_r1cd::data_m_t dataPwr;

            for (int32_t i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);

                pSP1401->set_tx_freq(freq[i]);
                Instr.sa_set_cf(freq[i] + dds1);

                pSP1401->get_cal_file()->m_tx_pwr_io->get(freq[i],R1C_TX_BASE_POWER_OP,&dataPwr);
                pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter_160);
                pSP1401->get_cal_file()->m_tx_filter_80m->get(freq[i],&dataFilter_80);
                dataFilter_160._2double(coefReal,coefImag);

                pSP1401->set_tx_att0(dataPwr.att0 / 2.0);
                pSP1401->set_tx_att1(dataPwr.att1 / 2.0);
                pSP1401->set_tx_att2(dataPwr.att2 / 2.0);
                pSP1401->set_tx_att3(dataPwr.att3 / 2.0);
                pSP2401->set_tx_pwr_comp(double(dataPwr.d_gain));
                pSP2401->set_tx_filter(coefReal,coefImag);
                msleep(50);

                Instr.sa_sweep_once();
                Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
                Instr.sa_get_marker_pwr(pwr[0]);

                dataFilter_80._2double(coefReal,coefImag);
                pSP2401->set_tx_filter(coefReal,coefImag);
                msleep(50);

                Instr.sa_sweep_once();
                Instr.sa_get_marker_pwr(pwr[1]);

                data.freq = freq[i];
                data.offset = pwr[0] - pwr[1];
                pSP1401->get_temp4_tx_lo2(&data.temp[0]);
                pSP1401->get_temp5_tx_lo1(&data.temp[1]);
                pSP1401->get_temp6_tx_pa4(&data.temp[2]);
                pSP1401->get_temp7_tx_pa3(&data.temp[3]);
                data.time = getCurTime();
                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                if (secCur != secBfr) {
                    modelIO->iterTable()->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                    secBfr = secCur;
                }
                modelIO->calTable()->replace(i,data);
                modelIO->iterTable()->at(secCur)->addOneData();
                pSP1401->get_cal_file()->add(cal_file::TX_FILTER_OFFSET_IO_80,&data);
                emit update(modelIO->index(i,0),modelIO->index(i,7),cal_file::TX_FILTER_OFFSET_IO_80,secCur);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::TX_FILTER_OFFSET_IO_80);
            pSP1401->get_cal_file()->m_tx_filter_offset_io_80m->save_as("c:\\tx_filter_off_io_80.txt");
           }
        CAL_THREAD_ABOART_S(POST_CLEAR);
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}

QExportR1CTxFilterOffsetThread::QExportR1CTxFilterOffsetThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CTxFilterOffsetThread::run()
{
    INIT_PROG("Exporting Tx Filter Offset",100);

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

    tx_filter_offset_table::data_f_t data;

    pSP1401->get_cal_file()->set_bw(_80M);

    if (calOP(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::TX_FILTER_OFFSET_OP_80);
        QR1CTxFilterOffsetModel *modelOP = (QR1CTxFilterOffsetModel *)(calParam.model_0);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_tx_filter_offset_op_80m->get(freq[i],&data);
            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            if (secCur != secBfr) {
                modelOP->iterTable()->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                secBfr = secCur;
            }
            modelOP->calTable()->replace(i,data);
            modelOP->iterTable()->at(secCur)->addOneData();
        }

        emit update(modelOP->index(0,0),modelOP->index(pts,7),cal_file::TX_FILTER_OFFSET_OP_80,secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::TX_FILTER_OFFSET_IO_80);
        QR1CTxFilterOffsetModel *modelIO = (QR1CTxFilterOffsetModel *)(calParam.model_1);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_tx_filter_offset_io_80m->get(freq[i],&data);
            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            if (secCur != secBfr) {
                modelIO->iterTable()->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                secBfr = secCur;
            }
            modelIO->calTable()->replace(i,data);
            modelIO->iterTable()->at(secCur)->addOneData();
        }

        emit update(modelIO->index(0,0),modelIO->index(pts,7),cal_file::TX_FILTER_OFFSET_IO_80,secCur);
    }

    pSP1401->get_cal_file()->set_bw(_160M);

    SET_PROG_POS(100);
    THREAD_ABORT
}
