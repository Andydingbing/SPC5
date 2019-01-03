#include "QCalR1CRxFilterThread.h"
#include "qcalr1crxfilterdlg.h"
#include "qr1crxfiltermodel.h"

QCalR1CRxFilterThread::QCalR1CRxFilterThread(CalParam *param, QObject *parent) : QCalBaseThread(param,parent)
{

}

void QCalR1CRxFilterThread::run()
{
    try {
        CAL_THREAD_START("Rx Filter",(RF_RX_FILTER_PTS +(int32_t)(IF_RX_FILTER_160M_PTS)));

        QwtRxRfFrData *dataRfFr = (QwtRxRfFrData *)(calParam.plotData_0);
        QwtRxIfFrData *dataIfFr = (QwtRxIfFrData *)(calParam.plotData_1);
        QR1CRxFilterModel_80M  *model_80  = (QR1CRxFilterModel_80M *)(calParam.model_0);
        QR1CRxFilterModel_160M *model_160 = (QR1CRxFilterModel_160M *)(calParam.model_1);
        rx_rf_fr_table::data_f_t tempDataRfFr;
        rx_if_fr_table::data_f_t tempDataIfFr;
        rx_filter_80m_table::data_f_t tempData_80;
        rx_filter_160m_table::data_f_t tempData_160;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
        sp2401_r1a *pSP2401 = calParam._sp2401;
        bool fast = calParam.justRebuildCoef;
        uint64_t rxFreq = 0;

        //80M : 100M-43M ~~~ 6G+43M,step 2M
        //160M: 100M-83M ~~~ 6G+83M,step 2M

        if (!fast) {
            THREAD_CHECK_BOX("Rx<===>Z28<===>Signal Generator");

            Instr.init();
            Instr.has_pm();
            Instr.has_sg();
            Instr.pm_reset();
            Instr.sg_reset();
            Instr.sg_set_en_mod(false);
            Instr.sg_set_pl(-60.0);
            Instr.sg_set_en_output(true);

            qint32 ref = 0;
            qint32 lnaAtt = 0;
            qint32 att019 = 0;
            double att1 = 0.0;
            double att2 = 0.0;
            double att3 = 0.0;

            rx_ref_op_table_r1cd::guess_base(1,ref,lnaAtt,att019,att1,att2,att3);
            ref -= 15;
//			att1 -= 5.0;
            pSP1401->set_io_mode(OUTPUT);
            pSP1401->set_rx_lna_att_sw((sp1401::rx_lna_att_t)(lnaAtt));
            pSP1401->set_rx_att_019_sw((sp1401::rx_att_019_t)(att019));
            pSP1401->set_rx_att(att1,att2,att3);
            pSP1401->set_pwr_meas_src(basic_sp1401::PWR_MEAS_FREE_RUN,false);
            pSP1401->set_pwr_meas_timeout(6553600);
            pSP1401->set_pwr_meas_samples(102400);
            pSP1401->set_tx_att(30.0,30.0,30.0,30.0);
            pSP1401->set_rx_if_filter_sw(basic_sp1401::_160MHz);
            pSP2401->set_ddc(-92640000.0);
            pSP2401->set_rx_pwr_comp(0);
            pSP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
            pSP2401->set_rx_filter_default();
            msleep(10);

            for (qint32 i = 0;i < RF_RX_FILTER_PTS;i ++) {
                CAL_THREAD_TEST_CANCEL
                tempDataRfFr.freq = RF_RX_FILTER_FREQ_STAR + i * RF_RX_FILTER_FREQ_STEP;
                rxFreq = tempDataRfFr.freq < RF_RX_FREQ_STAR ? RF_RX_FREQ_STAR : tempDataRfFr.freq;
                rxFreq = rxFreq > RF_RX_FREQ_STOP ? RF_RX_FREQ_STOP : rxFreq;
                ajustSG(double(rxFreq),ref);
                pSP1401->set_rx_freq(rxFreq);
                msleep(50);
                tempDataRfFr.pwr = getRxPwr();
                pSP1401->get_cal_file()->add(cal_file::RX_RF_FR,&tempDataRfFr);
                dataRfFr->calTable()->append(tempDataRfFr);
                emit update(QModelIndex(),QModelIndex(),cal_file::RX_RF_FR);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_RF_FR);
            pSP1401->get_cal_file()->m_rx_rf_fr->save_as("c:\\rx_filter_rf_fr.txt");

            pSP1401->set_rx_freq(2000000000);
            msleep(10);
            for (qint32 i = 0;i < IF_RX_FILTER_160M_PTS;i ++) {
                CAL_THREAD_TEST_CANCEL
                tempDataIfFr.freq = IF_RX_FILTER_160M_FREQ_STAR + i * IF_RX_FILTER_FREQ_STEP;
                ajustSG(2000000000 + tempDataIfFr.freq,ref);
                msleep(50);
                tempDataIfFr.pwr = getRxPwr();
                pSP1401->get_cal_file()->add(cal_file::RX_IF_FR,&tempDataIfFr);
                dataIfFr->calTable()->append(tempDataIfFr);
                emit update(QModelIndex(),QModelIndex(),cal_file::RX_IF_FR);
                SET_PROG_POS(RF_RX_FILTER_PTS + i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_IF_FR);
            pSP1401->get_cal_file()->m_rx_if_fr->save_as("c:\\rx_filter_if_fr.txt");
        }

        //80M
        pSP1401->get_cal_file()->set_bw(_80M);
        FILE *fp_fr = NULL;
        FILE *fp_real = NULL;
        FILE *fp_imag = NULL;
        char pathFr[MAX_PATH];
        char pathReal[MAX_PATH];
        char pathImag[MAX_PATH];

        sprintf(pathFr,"C:\\CSECal\\rx_filter_80.txt");
        fp_fr = fopen(pathFr,"w");
        if (NULL == fp_fr) {
            QString msg = QString("could not open %1").arg(pathFr);
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }

        rx_rf_fr_table::data_f_t tempDataRFFr_80[(quint32)(IF_RX_FILTER_80M_PTS)];	//(2G-43M)~~~(2G+43M),step 2M
        rx_if_fr_table::data_f_t tempDataIFFr_80[(quint32)(IF_RX_FILTER_80M_PTS)];	//-43M~~~43M,step 2M
        int32_t idx = 0;

        pSP1401->get_cal_file()->map2buf(cal_file::RX_RF_FR);
        for (double freq = 2e9 + IF_RX_FILTER_80M_FREQ_STAR;freq <= 2e9 + IF_RX_FILTER_80M_FREQ_STAR;freq += IF_RX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_rx_rf_fr->get(quint64(freq),&tempDataRFFr_80[idx]);
            idx ++;
        }
        idx = 0;
        pSP1401->get_cal_file()->map2buf(cal_file::RX_IF_FR);
        for (double freq = IF_RX_FILTER_80M_FREQ_STAR;freq <= IF_RX_FILTER_80M_FREQ_STAR;freq += IF_RX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_rx_if_fr->get(freq,&tempDataIFFr_80[idx]);
            idx ++;
        }

        for (idx = 0;idx < IF_RX_FILTER_80M_PTS;idx ++)
            tempDataIFFr_80[idx].pwr -= tempDataRFFr_80[idx].pwr;		//real IF frequency response,-43M~~~43M

        pSP1401->get_cal_file()->map2buf(cal_file::RX_RF_FR);
        quint64 cwFreqCalled = 0;
        for (quint64 cwFreq = RF_RX_FREQ_STAR;cwFreq <= RF_RX_FREQ_STOP;cwFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
            cwFreqCalled = cwFreq < 100000000 ? 100000000 : cwFreq;
            idx = 0;
            for (quint64 freq = quint64(cwFreqCalled + IF_RX_FILTER_80M_FREQ_STAR);freq <= quint64(cwFreqCalled + IF_RX_FILTER_80M_FREQ_STOP);freq += quint64(IF_RX_FILTER_FREQ_STEP)) {
                pSP1401->get_cal_file()->m_rx_rf_fr->get(freq,&tempDataRFFr_80[idx]);
                fprintf(fp_fr,"%.6f ",tempDataRFFr_80[idx].pwr + tempDataIFFr_80[idx].pwr);
                idx ++;
            }
            fprintf(fp_fr,"\n");
        }
        fclose(fp_fr);
        fp_fr = NULL;

        //160M
        pSP1401->get_cal_file()->set_bw(_160M);
        sprintf(pathFr,"C:\\CSECal\\rx_filter_160.txt");
        fp_fr = fopen(pathFr,"w");
        if (NULL == fp_fr) {
            QString msg = QString("could not open %1").arg(pathFr);
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }

        rx_rf_fr_table::data_f_t tempDataRFFr_160[(uint32_t)(IF_RX_FILTER_160M_PTS)];	//(2G-83M)~~~(2G+83M),step 2M
        rx_if_fr_table::data_f_t tempDataIFFr_160[(uint32_t)(IF_RX_FILTER_160M_PTS)];	//-83M~~~83M,step 2M

        idx = 0;
        pSP1401->get_cal_file()->map2buf(cal_file::RX_RF_FR);
        for (double freq = 2e9 + IF_RX_FILTER_160M_FREQ_STAR;freq <= 2e9 + IF_RX_FILTER_160M_FREQ_STOP;freq += IF_RX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_rx_rf_fr->get(quint64(freq),&tempDataRFFr_160[idx]);
            idx ++;
        }
        idx = 0;
        pSP1401->get_cal_file()->map2buf(cal_file::RX_IF_FR);
        for (double freq = IF_RX_FILTER_160M_FREQ_STAR;freq <= IF_RX_FILTER_160M_FREQ_STOP;freq += IF_RX_FILTER_FREQ_STEP) {
            pSP1401->get_cal_file()->m_rx_if_fr->get(freq,&tempDataIFFr_160[idx]);
            idx ++;
        }

        for (idx = 0;idx < IF_RX_FILTER_160M_PTS;idx ++)
            tempDataIFFr_160[idx].pwr -= tempDataRFFr_160[idx].pwr;		//real IF frequency response,-83M~~~83M

        pSP1401->get_cal_file()->map2buf(cal_file::RX_RF_FR);
        cwFreqCalled = 0;
        for (uint64_t cwFreq = RF_RX_FREQ_STAR;cwFreq <= RF_RX_FREQ_STOP;cwFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
            cwFreqCalled = cwFreq < 100000000 ? 100000000 : cwFreq;
            idx = 0;
            for (quint64 freq = quint64(cwFreqCalled + IF_RX_FILTER_160M_FREQ_STAR);freq <= quint64(cwFreqCalled + IF_RX_FILTER_160M_FREQ_STOP);freq += quint64(IF_RX_FILTER_FREQ_STEP)) {
                pSP1401->get_cal_file()->m_rx_rf_fr->get(freq,&tempDataRFFr_160[idx]);
                fprintf(fp_fr,"%.6f ",tempDataRFFr_160[idx].pwr + tempDataIFFr_160[idx].pwr);
                idx ++;
            }
            fprintf(fp_fr,"\n");
        }
        fclose(fp_fr);
        fp_fr = NULL;

        //build coef
        char firExePaht[MAX_PATH] = {0};
        memset(firExePaht,0,sizeof(firExePaht));
        ::GetModuleFileNameA(NULL,firExePaht,MAX_PATH);
        *strrchr(firExePaht,'\\') = 0;
        strcat(firExePaht,"\\rx_filter.exe");
        if (exeFirProcess(firExePaht)) {
            DeleteFileA(pathFr);
            QString msg = QString("%1").arg(Log.get_last_err());
            THREAD_ERROR_BOX(msg);
            CAL_THREAD_ABOART
        }

        //80M
        sprintf(pathReal,"C:\\CSECal\\rx_filter_80_coef_real.txt");
        sprintf(pathImag,"C:\\CSECal\\rx_filter_80_coef_imag.txt");
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
        for (quint64 freq = RF_RX_FREQ_STAR;freq <= RF_RX_FREQ_STOP;freq += RF_RX_FREQ_STEP_CALLED_R1C) {
            memset(&tempData_80,0,sizeof(tempData_80));
            for (qint32 i = 0;i < RX_FILTER_ORDER;i ++)
                fscanf(fp_real,"%hd,",&(tempData_80.coef_real[i]));
            fscanf(fp_real,"\n");

            for (int32_t i = 0;i < RX_FILTER_ORDER;i ++)
                fscanf(fp_imag,"%hd,",&(tempData_80.coef_imag[i]));
            fscanf(fp_imag,"\n");

            tempData_80.freq = freq;
            tempData_80.time = getCurTime();

            model_80->calTable()->replace(idx,tempData_80);
            pSP1401->get_cal_file()->add(cal_file::RX_FILTER_80,&tempData_80);
            emit update(model_80->index(idx,0),model_80->index(idx,7),cal_file::RX_FILTER_80);
            idx ++;
        }
        pSP1401->get_cal_file()->w(cal_file::RX_FILTER_80);
        fclose(fp_real);
        fclose(fp_imag);
        fp_real = fp_imag = NULL;
        DeleteFileA(pathFr);
        DeleteFileA(pathReal);
        DeleteFileA(pathImag);

        //160M
        sprintf(pathReal,"C:\\CSECal\\rx_filter_160_coef_real.txt");
        sprintf(pathImag,"C:\\CSECal\\rx_filter_160_coef_imag.txt");
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
        for (quint64 freq = RF_RX_FREQ_STAR;freq <= RF_RX_FREQ_STOP;freq += RF_RX_FREQ_STEP_CALLED_R1C) {
            memset(&tempData_160,0,sizeof(tempData_160));
            for (int32_t i = 0;i < RX_FILTER_ORDER;i ++)
                fscanf(fp_real,"%hd,",&(tempData_160.coef_real[i]));
            fscanf(fp_real,"\n");

            for (int32_t i = 0;i < RX_FILTER_ORDER;i ++)
                fscanf(fp_imag,"%hd,",&(tempData_160.coef_imag[i]));
            fscanf(fp_imag,"\n");

            tempData_160.freq = freq;
            tempData_160.time = getCurTime();

            model_160->calTable()->replace(idx,tempData_160);
            pSP1401->get_cal_file()->add(cal_file::RX_FILTER_160,&tempData_160);
            emit update(model_160->index(idx,0),model_160->index(idx,7),cal_file::RX_FILTER_160);
            idx ++;
        }
        pSP1401->get_cal_file()->w(cal_file::RX_FILTER_160);
        fclose(fp_real);
        fclose(fp_imag);
        fp_real = fp_imag = NULL;
        DeleteFileA(pathFr);
        DeleteFileA(pathReal);
        DeleteFileA(pathImag);

        if (fast)
            SET_PROG_POS(RF_RX_FILTER_PTS +(int32_t)(IF_RX_FILTER_160M_PTS));

        CAL_THREAD_ABOART
    }catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}

void QCalR1CRxFilterThread::ajustSG(double freq,qint32 pwr)
{
    double pmPwr = 0.0;
    Instr.pm_set_freq(freq);
    Instr.sg_set_cw(freq);
    Instr.sg_set_pl(pwr + PM_IL);
    msleep(2);

    for (qint32 i = 0;i < 10;i ++) {
        Instr.pm_get_pwr(freq,pmPwr);
        if (abs(pwr - pmPwr) < 0.05)
            break;
        Instr.sg_set_pl(pwr + PM_IL + (pwr - pmPwr));
        msleep(2);
    }
}

double QCalR1CRxFilterThread::getRxPwr()
{
    double pwr = 0.0;
    basic_sp1401::pwr_meas_state_t state = basic_sp1401::PMS_IDLE;
    calParam._sp1401->pwr_meas_abort();
    calParam._sp1401->pwr_meas_start();

    while (state != basic_sp1401::PMS_DONE) {
        calParam._sp1401->get_pwr_meas_state(state);
        if (state == basic_sp1401::PMS_TT)
            return -100.0;
    }
    calParam._sp1401->get_pwr_meas_pwr(pwr);
    return pwr;
}


QExportR1CRxFilterThread::QExportR1CRxFilterThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CRxFilterThread::run()
{
    INIT_PROG("Exporting Rx Filter",100);

    qint32 idx = 0;
    rx_rf_fr_table::data_f_t tempDataRfFr;;
    rx_if_fr_table::data_f_t tempDataIFFr;
    rx_filter_80m_table::data_f_t  tempData_80;
    rx_filter_160m_table::data_f_t tempData_160;

    QwtRxRfFrData *dataRfFr = (QwtRxRfFrData *)(calParam.plotData_0);
    QwtRxIfFrData *dataIfFr = (QwtRxIfFrData *)(calParam.plotData_1);
    QR1CRxFilterModel_80M  *model_80 = (QR1CRxFilterModel_80M *)(calParam.model_0);
    QR1CRxFilterModel_160M *model_160 = (QR1CRxFilterModel_160M *)(calParam.model_1);
    rx_filter_80m_table::data_f_t coefData_80;
    rx_filter_160m_table::data_f_t coefData_160;
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

    pSP1401->get_cal_file()->map2buf(cal_file::RX_RF_FR);
    for (quint64 freq = RF_RX_FILTER_FREQ_STAR;freq <= RF_RX_FILTER_FREQ_STOP;freq += RF_RX_FILTER_FREQ_STEP) {
        pSP1401->get_cal_file()->m_rx_rf_fr->get(freq,&tempDataRfFr);
        dataRfFr->calTable()->append(tempDataRfFr);
    }
    pSP1401->get_cal_file()->map2buf(cal_file::RX_IF_FR);
    for (double freq = IF_RX_FILTER_160M_FREQ_STAR;freq <= IF_RX_FILTER_160M_FREQ_STOP;freq += IF_RX_FILTER_FREQ_STEP) {
        pSP1401->get_cal_file()->m_rx_if_fr->get(freq,&tempDataIFFr);
        dataIfFr->calTable()->append(tempDataIFFr);
    }

    pSP1401->get_cal_file()->set_bw(_80M);
    pSP1401->get_cal_file()->map2buf(cal_file::RX_FILTER_80);
    for (quint64 freq = RF_RX_FREQ_STAR;freq <= RF_RX_FREQ_STOP;freq += RF_RX_FREQ_STEP_CALLED_R1C) {
        pSP1401->get_cal_file()->m_rx_filter_80m->get(freq,&tempData_80);
        model_80->calTable()->replace(idx,tempData_80);
        idx ++;
    }

    idx = 0;
    pSP1401->get_cal_file()->set_bw(_160M);
    pSP1401->get_cal_file()->map2buf(cal_file::RX_FILTER_160);
    for (quint64 freq = RF_RX_FREQ_STAR;freq <= RF_RX_FREQ_STOP;freq += RF_RX_FREQ_STEP_CALLED_R1C) {
        pSP1401->get_cal_file()->m_rx_filter_160m->get(freq,&tempData_160);
        model_160->calTable()->replace(idx,tempData_160);
        idx ++;
    }

    emit update(QModelIndex(),QModelIndex(),cal_file::RX_RF_FR);
    emit update(QModelIndex(),QModelIndex(),cal_file::RX_IF_FR);
    emit update(model_80->index(0,0),model_80->index(RF_RX_FREQ_PTS_CALLED_R1C,7),cal_file::RX_FILTER_80);
    emit update(model_160->index(0,0),model_160->index(RF_RX_FREQ_PTS_CALLED_R1C,7),cal_file::RX_FILTER_160);

    SET_PROG_POS(100);
    THREAD_ABORT
}
