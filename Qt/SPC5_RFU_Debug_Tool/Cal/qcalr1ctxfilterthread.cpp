#include "QCalR1CTxFilterThread.h"
#include "qcalr1ctxfilterdlg.h"
#include "qr1ctxfiltermodel.h"

QCalR1CTxFilterThread::QCalR1CTxFilterThread(CalParam *param, QObject *parent) : QCalBaseThread(param,parent)
{

}

void QCalR1CTxFilterThread::run()
{
    try {
        CAL_THREAD_START("Tx Filter",(RF_TX_FILTER_PTS +(int32_t)(IF_TX_FILTER_160M_PTS)));

        QwtTxRfFrData *dataRfFr_0 = (QwtTxRfFrData *)(calParam.plotData_0);
        QwtTxRfFrData *dataRfFr_1 = (QwtTxRfFrData *)(calParam.plotData_1);
        QwtTxIfFrData *dataIfFr = (QwtTxIfFrData *)(calParam.plotData_2);
        QR1CTxFilterModel *model_80 = (QR1CTxFilterModel *)(calParam.model_0);
        QR1CTxFilterModel *model_160 = (QR1CTxFilterModel *)(calParam.model_1);
        tx_rf_fr_table::data_f_t tempDataRfFr;
        tx_if_fr_table::data_f_t tempDataIfFr;
        tx_filter_80m_table::data_f_t tempData_80;
        tx_filter_160m_table::data_f_t tempData_160;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
        sp2401_r1a *pSP2401 = calParam._sp2401;
        bool fast = calParam.justRebuildCoef;
        uint64_t txFreq = 0;

        //80M : 100M-43M ~~~ 6G+43M,step 2M
        //160M: 100M-83M ~~~ 6G+83M,step 2M

        if (!fast) {
            THREAD_CHECK_BOX("Tx<===>Power Meter");

            Instr.init();
            Instr.has_pm();
            Instr.pm_reset();

            tx_lol_table_r1cd::data_m_t dataLOL;
            tx_sb_table_r1cd::data_m_t dataSB;
            pSP1401->get_cal_file()->m_tx_lol->get(2000000000,&dataLOL);
            pSP1401->get_cal_file()->m_tx_sb->get(2000000000,&dataSB);
            pSP2401->set_tx_dc_offset((uint16_t)(dataLOL.dc_i),(uint16_t)(dataLOL.dc_q));
            pSP2401->set_tx_phase_rotate_I((double)(dataSB.th));
            pSP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
            pSP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
            pSP2401->set_dds1(0.0);
            pSP2401->set_duc_dds(0.0);
            pSP2401->set_tx_filter_sw(false);

            double att0 = 0.0;
            double att1 = 0.0;
            double att2 = 0.0;
            double att3 = 0.0;
            double d_gain = -5.0;
            tx_pwr_op_table_r1cd::guess_base(att0,att1,att2,att3,d_gain);
            pSP1401->set_io_mode(sp1401::OUTPUT);
            pSP1401->set_tx_att(att0,att1,att2,att3);
            pSP2401->set_tx_pwr_comp(d_gain);
            pSP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
            pSP1401->set_rx_att_019_sw(sp1401::RX_ATT_19);
            pSP1401->set_rx_att(31.0,31.0,31.0);
            msleep(10);

            for (int32_t i = 0;i < RF_TX_FILTER_PTS_0;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL
                tempDataRfFr.freq = RF_TX_FILTER_FREQ_STAR_0 + i * RF_TX_FILTER_FREQ_STEP;
                txFreq = tempDataRfFr.freq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : tempDataRfFr.freq;
                pSP1401->set_tx_freq(txFreq);
                pSP1401->set_tx_filter_sw(sp1401_r1c::LPF,sp1401_r1c::LPF);
                Instr.pm_set_freq((double)(txFreq));
                msleep(10);
                Instr.pm_get_pwr(tempDataRfFr.pwr);
                pSP1401->get_cal_file()->add(cal_file::TX_RF_FR_0,(tx_rf_fr_table::data_f_t *)&tempDataRfFr);
                dataRfFr_0->calTable()->append(tempDataRfFr);
                emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_0);
                SET_PROG_POS(i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            pSP1401->get_cal_file()->w(cal_file::TX_RF_FR_0);
            pSP1401->get_cal_file()->m_tx_rf_fr_0->save_as("c:\\tx_filter_rf_fr_0.txt");

            for (int32_t i = 0;i < RF_TX_FILTER_PTS_1;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL
                tempDataRfFr.freq = RF_TX_FILTER_FREQ_STAR_1 + i * RF_TX_FILTER_FREQ_STEP;
                txFreq = tempDataRfFr.freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : tempDataRfFr.freq;
                pSP1401->set_tx_freq(txFreq);
                pSP1401->set_tx_filter_sw(sp1401_r1c::BPF,sp1401_r1c::BPF);
                Instr.pm_set_freq((double)(txFreq));
                msleep(10);
                Instr.pm_get_pwr(tempDataRfFr.pwr);
                pSP1401->get_cal_file()->add(cal_file::TX_RF_FR_1,(tx_rf_fr_table::data_f_t *)&tempDataRfFr);
                dataRfFr_1->calTable()->append(tempDataRfFr);
                emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_1);
                SET_PROG_POS(RF_TX_FILTER_PTS_0 + i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            pSP1401->get_cal_file()->w(cal_file::TX_RF_FR_1);
            pSP1401->get_cal_file()->m_tx_rf_fr_1->save_as("c:\\tx_filter_rf_fr_1.txt");

            pSP1401->set_tx_freq(2000000000);
            msleep(10);
            for (int32_t i = 0;i < IF_TX_FILTER_160M_PTS;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL
                tempDataIfFr.freq = IF_TX_FILTER_160M_FREQ_STAR + i * IF_TX_FILTER_FREQ_STEP;
                pSP2401->set_duc_dds(tempDataIfFr.freq);
                Instr.pm_set_freq((double)2000000000 + tempDataIfFr.freq);
                Sleep(10);
                Instr.pm_get_pwr(tempDataIfFr.pwr);
                pSP1401->get_cal_file()->add(cal_file::TX_IF_FR,(tx_if_fr_table::data_f_t *)&tempDataIfFr);
                dataIfFr->calTable()->append(tempDataIfFr);
                emit update(QModelIndex(),QModelIndex(),cal_file::TX_IF_FR);
                SET_PROG_POS(RF_TX_FILTER_PTS + i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            pSP1401->get_cal_file()->w(cal_file::TX_IF_FR);
            pSP1401->get_cal_file()->m_tx_if_fr->save_as("c:\\tx_filter_if_fr.txt");
        }

        //80M
        FILE *fp_fr = NULL;
        FILE *fp_real = NULL;
        FILE *fp_imag = NULL;
        char pathFr[MAX_PATH];
        char pathReal[MAX_PATH];
        char pathImag[MAX_PATH];

        sprintf(pathFr,"C:\\CSECal\\tx_filter_80.txt");
        fp_fr = fopen(pathFr,"w");
        if (NULL == fp_fr) {
            QString msg = QString("could not open %1").arg(pathFr);
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }

        tx_rf_fr_table::data_f_t tempDataRFFr_80[(uint32_t)(IF_TX_FILTER_80M_PTS)];	//(2G-43M)~~~(2G+43M),step 2M
        tx_if_fr_table::data_f_t tempDataIFFr_80[(uint32_t)(IF_TX_FILTER_80M_PTS)];	//-43M~~~43M,step 2M
        int32_t idx = 0;

        pSP1401->get_cal_file()->set_bw(_80M);
        pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_0);
        for (uint64_t freq = (uint64_t)(2000000000.0 + IF_TX_FILTER_80M_FREQ_STAR);freq <= (uint64_t)(2000000000.0 + IF_TX_FILTER_80M_FREQ_STOP);freq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_tx_rf_fr_0->get(freq,&tempDataRFFr_80[idx]);
            idx ++;
        }
        idx = 0;
        pSP1401->get_cal_file()->map2buf(cal_file::TX_IF_FR);
        for (double freq = IF_TX_FILTER_80M_FREQ_STAR;freq <= IF_TX_FILTER_80M_FREQ_STOP;freq += IF_TX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_tx_if_fr->get(freq,&tempDataIFFr_80[idx]);
            idx ++;
        }
        for (idx = 0;idx < IF_TX_FILTER_80M_PTS;idx ++)
            tempDataIFFr_80[idx].pwr -= tempDataRFFr_80[idx].pwr;		//real IF frequency response,-43M~~~43M

        pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_0);
        uint64_t cwFreqCalled = 0;
        for (uint64_t cwFreq = RF_TX_FREQ_STAR;cwFreq < 3100000000;cwFreq += RF_TX_FREQ_STEP_CALLED) {
            cwFreqCalled = cwFreq < 100000000 ? 100000000 : cwFreq;
            idx = 0;
            for (uint64_t freq = (uint64_t)(cwFreqCalled + IF_TX_FILTER_80M_FREQ_STAR);freq <= (uint64_t)(cwFreqCalled + IF_TX_FILTER_80M_FREQ_STOP);freq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
                pSP1401->get_cal_file()->m_tx_rf_fr_0->get(freq,&tempDataRFFr_80[idx]);
                fprintf(fp_fr,"%.6f ",tempDataRFFr_80[idx].pwr + tempDataIFFr_80[idx].pwr);
                idx ++;
            }
            fprintf(fp_fr,"\n");
        }
        pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_1);
        for (uint64_t cwFreq = 3100000000;cwFreq <= RF_TX_FREQ_STOP;cwFreq += RF_TX_FREQ_STEP_CALLED) {
            idx = 0;
            for (uint64_t freq = (uint64_t)(cwFreq + IF_TX_FILTER_80M_FREQ_STAR);freq <= (uint64_t)(cwFreq + IF_TX_FILTER_80M_FREQ_STOP);freq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
                pSP1401->get_cal_file()->m_tx_rf_fr_1->get(freq,&tempDataRFFr_80[idx]);
                fprintf(fp_fr,"%.6f ",tempDataRFFr_80[idx].pwr + tempDataIFFr_80[idx].pwr);
                idx ++;
            }
            fprintf(fp_fr,"\n");
        }
        fclose(fp_fr);
        fp_fr = NULL;

        //160M
        sprintf(pathFr,"C:\\CSECal\\tx_filter_160.txt");
        fp_fr = fopen(pathFr,"w");
        if (NULL == fp_fr) {
            QString msg = QString("could not open %1").arg(pathFr);
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }

        tx_rf_fr_table::data_f_t tempDataRFFr_160[(uint32_t)(IF_TX_FILTER_160M_PTS)];	//(2G-83M)~~~(2G+83M),step 2M
        tx_if_fr_table::data_f_t tempDataIFFr_160[(uint32_t)(IF_TX_FILTER_160M_PTS)];	//-83M~~~83M,step 2M

        idx = 0;
        pSP1401->get_cal_file()->set_bw(_160M);
        pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_0);
        for (uint64_t freq = (uint64_t)(2000000000.0 + IF_TX_FILTER_160M_FREQ_STAR);freq <= (uint64_t)(2000000000.0 + IF_TX_FILTER_160M_FREQ_STOP);freq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_tx_rf_fr_0->get(freq,&tempDataRFFr_160[idx]);
            idx ++;
        }
        idx = 0;
        pSP1401->get_cal_file()->map2buf(cal_file::TX_IF_FR);
        for (double freq = IF_TX_FILTER_160M_FREQ_STAR;freq <= IF_TX_FILTER_160M_FREQ_STOP;freq += IF_TX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_tx_if_fr->get(freq,&tempDataIFFr_160[idx]);
            idx ++;
        }
        for (idx = 0;idx < IF_TX_FILTER_160M_PTS;idx ++)
            tempDataIFFr_160[idx].pwr -= tempDataRFFr_160[idx].pwr;		//real IF frequency response,-83M~~~83M


        pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_0);
        cwFreqCalled = 0;
        for (uint64_t cwFreq = RF_TX_FREQ_STAR;cwFreq < 3100000000;cwFreq += RF_TX_FREQ_STEP_CALLED) {
            cwFreqCalled = cwFreq < 100000000 ? 100000000 : cwFreq;
            idx = 0;
            for (uint64_t freq = (uint64_t)(cwFreqCalled + IF_TX_FILTER_160M_FREQ_STAR);freq <= (uint64_t)(cwFreqCalled + IF_TX_FILTER_160M_FREQ_STOP);freq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
                pSP1401->get_cal_file()->m_tx_rf_fr_0->get(freq,&tempDataRFFr_160[idx]);
                fprintf(fp_fr,"%.6f ",tempDataRFFr_160[idx].pwr + tempDataIFFr_160[idx].pwr);
                idx ++;
            }
            fprintf(fp_fr,"\n");
        }
        pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_1);
        for (uint64_t cwFreq = 3100000000;cwFreq <= RF_TX_FREQ_STOP;cwFreq += RF_TX_FREQ_STEP_CALLED) {
            idx = 0;
            for (uint64_t freq = (uint64_t)(cwFreq + IF_TX_FILTER_160M_FREQ_STAR);freq <= (uint64_t)(cwFreq + IF_TX_FILTER_160M_FREQ_STOP);freq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
                pSP1401->get_cal_file()->m_tx_rf_fr_1->get(freq,&tempDataRFFr_160[idx]);
                fprintf(fp_fr,"%.6f ",tempDataRFFr_160[idx].pwr + tempDataIFFr_160[idx].pwr);
                idx ++;
            }
            fprintf(fp_fr,"\n");
        }
        fclose(fp_fr);
        fp_fr = NULL;

        //build coef
        char firExePath[MAX_PATH] = {0};
        memset(firExePath,0,sizeof(firExePath));
        ::GetModuleFileNameA(NULL,firExePath,MAX_PATH);
        *strrchr(firExePath,'\\') = 0;
        strcat(firExePath,"\\tx_filter.exe");
        if (exeFirProcess(firExePath)) {
            DeleteFileA(pathFr);
            QString msg = QString("%1").arg(Log.get_last_err());
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }

        //80M
        sprintf(pathReal,"C:\\CSECal\\tx_filter_80_coef_real.txt");
        sprintf(pathImag,"C:\\CSECal\\tx_filter_80_coef_imag.txt");
        fp_real = fopen(pathReal,"r");
        fp_imag = fopen(pathImag,"r");
        if (NULL == fp_real) {
            Log.set_last_err("could not open %s",pathReal);
            QString msg = QString("%1").arg(Log.get_last_err());
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }
        if (NULL == fp_imag) {
            Log.set_last_err("could not open %s",pathImag);
            QString msg = QString("%1").arg(Log.get_last_err());
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }
        idx = 0;
        for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_CALLED) {
            memset(&tempData_80,0,sizeof(tempData_80));
            for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
                fscanf(fp_real,"%hd,",&(tempData_80.coef_real[i]));
            fscanf(fp_real,"\n");

            for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
                fscanf(fp_imag,"%hd,",&(tempData_80.coef_imag[i]));
            fscanf(fp_imag,"\n");

            tempData_80.freq = freq;
            tempData_80.time = getCurTime();
            model_80->calTable()->replace(idx,tempData_80);
            pSP1401->get_cal_file()->add(cal_file::TX_FILTER_80,(tx_filter_80m_table::data_f_t *)&tempData_80);
            emit update(model_80->index(idx,0),model_80->index(idx,7),cal_file::TX_FILTER_80);
            idx ++;
        }
        pSP1401->get_cal_file()->w(cal_file::TX_FILTER_80);
        fclose(fp_real);
        fclose(fp_imag);
        fp_real = NULL;
        fp_imag = NULL;
        DeleteFileA(pathFr);
        DeleteFileA(pathReal);
        DeleteFileA(pathImag);

        //160M
        sprintf(pathReal,"C:\\CSECal\\tx_filter_160_coef_real.txt");
        sprintf(pathImag,"C:\\CSECal\\tx_filter_160_coef_imag.txt");
        fp_real = fopen(pathReal,"r");
        fp_imag = fopen(pathImag,"r");
        if (NULL == fp_real) {
            Log.set_last_err("could not open %s",pathReal);
            QString msg = QString("%1").arg(Log.get_last_err());
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }
        if (NULL == fp_imag) {
            Log.set_last_err("could not open %s",pathImag);
            QString msg = QString("%1").arg(Log.get_last_err());
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }
        idx = 0;
        for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_CALLED) {
            memset(&tempData_160,0,sizeof(tempData_160));
            for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
                fscanf(fp_real,"%hd,",&(tempData_160.coef_real[i]));
            fscanf(fp_real,"\n");

            for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
                fscanf(fp_imag,"%hd,",&(tempData_160.coef_imag[i]));
            fscanf(fp_imag,"\n");

            tempData_160.freq = freq;
            tempData_160.time = getCurTime();
            model_160->calTable()->replace(idx,tempData_160);
            pSP1401->get_cal_file()->add(cal_file::TX_FILTER_160,(tx_filter_160m_table::data_f_t *)&tempData_160);
            emit update(model_160->index(idx,0),model_160->index(idx,7),cal_file::TX_FILTER_160);
            idx ++;
        }
        pSP1401->get_cal_file()->w(cal_file::TX_FILTER_160);
        fclose(fp_real);
        fclose(fp_imag);
        fp_real = NULL;
        fp_imag = NULL;
        DeleteFileA(pathFr);
        DeleteFileA(pathReal);
        DeleteFileA(pathImag);

        if (fast)
            SET_PROG_POS(RF_TX_FILTER_PTS +(int32_t)(IF_TX_FILTER_160M_PTS));

        CAL_THREAD_ABOART
    }catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}


QExportR1CTxFilterThread::QExportR1CTxFilterThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CTxFilterThread::run()
{
    INIT_PROG("Exporting Tx Filter",100);

    int32_t idx = 0;
    tx_rf_fr_table::data_f_t tempDataRfFr;;
    tx_if_fr_table::data_f_t tempDataIFFr;
    tx_filter_80m_table::data_f_t  tempData_80;
    tx_filter_160m_table::data_f_t tempData_160;

    QwtTxRfFrData *dataRfFr_0 = (QwtTxRfFrData *)(calParam.plotData_0);
    QwtTxRfFrData *dataRfFr_1 = (QwtTxRfFrData *)(calParam.plotData_1);
    QwtTxIfFrData *dataIfFr = (QwtTxIfFrData *)(calParam.plotData_2);
    QR1CTxFilterModel *model_80 = (QR1CTxFilterModel *)(calParam.model_0);
    QR1CTxFilterModel *model_160 = (QR1CTxFilterModel *)(calParam.model_1);
    tx_filter_80m_table::data_f_t coefData_80;
    tx_filter_160m_table::data_f_t coefData_160;
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

    pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_0);
    for (uint64_t freq = RF_TX_FILTER_FREQ_STAR_0;freq <= RF_TX_FILTER_FREQ_STOP_0;freq += RF_TX_FILTER_FREQ_STEP) {
        pSP1401->get_cal_file()->m_tx_rf_fr_0->get(freq,&tempDataRfFr);
        dataRfFr_0->calTable()->append(tempDataRfFr);
    }
    pSP1401->get_cal_file()->map2buf(cal_file::TX_RF_FR_1);
    for (uint64_t freq = RF_TX_FILTER_FREQ_STAR_1;freq <= RF_TX_FILTER_FREQ_STOP_1;freq += RF_TX_FILTER_FREQ_STEP) {
        pSP1401->get_cal_file()->m_tx_rf_fr_1->get(freq,&tempDataRfFr);
        dataRfFr_1->calTable()->append(tempDataRfFr);
    }
    pSP1401->get_cal_file()->map2buf(cal_file::TX_IF_FR);
    for (double freq = IF_TX_FILTER_160M_FREQ_STAR;freq <= IF_TX_FILTER_160M_FREQ_STOP;freq += IF_TX_FILTER_FREQ_STEP) {
        pSP1401->get_cal_file()->m_tx_if_fr->get(freq,&tempDataIFFr);
        dataIfFr->calTable()->append(tempDataIFFr);
    }

    pSP1401->get_cal_file()->set_bw(_80M);
    pSP1401->get_cal_file()->map2buf(cal_file::TX_FILTER_80);
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_CALLED) {
        pSP1401->get_cal_file()->m_tx_filter_80m->get(freq,&tempData_80);
        model_80->calTable()->replace(idx,tempData_80);
        idx ++;
    }

    idx = 0;
    pSP1401->get_cal_file()->set_bw(_160M);
    pSP1401->get_cal_file()->map2buf(cal_file::TX_FILTER_160);
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_CALLED) {
        pSP1401->get_cal_file()->m_tx_filter_160m->get(freq,&tempData_160);
        model_160->calTable()->replace(idx,tempData_160);
        idx ++;
    }

    emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_0);
    emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_1);
    emit update(QModelIndex(),QModelIndex(),cal_file::TX_IF_FR);
    emit update(model_80->index(0,0),model_80->index(RF_TX_FREQ_PTS_CALLED,7),cal_file::TX_FILTER_80);
    emit update(model_160->index(0,0),model_160->index(RF_TX_FREQ_PTS_CALLED,7),cal_file::TX_FILTER_160);

    SET_PROG_POS(100);
    THREAD_ABORT
}
