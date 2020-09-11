#include "qtestr1crxattthread.h"
#include "qtestr1crxattdlg.h"

QTestR1CRxAttThread::QTestR1CRxAttThread(TestParam *param, QObject *parent) :
    QTestBaseThread(param,parent),
    sgPwr(-160),
    pmIL(PM_IL)
{

}

void QTestR1CRxAttThread::run()
{
    try {
        QString strFreqStar = calParam.rfFreqStar;
        QString strFreqStop = calParam.rfFreqStop;
        QString strFreqStep = calParam.rfFreqStep;
        quint64 freq[RF_RX_FREQ_PTS_CALLED_R1C];
        quint64 freqStar[16];
        quint64 freqStop[16];
        memset(freq,0,sizeof(freq));
        memset(freqStar,0,sizeof(freqStar));
        memset(freqStop,0,sizeof(freqStop));
        qint32 pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

        TEST_THREAD_START("Rx Pow Test",pts);
        THREAD_CHECK_BOX("Rx<===>Z28<===>Signal Generator");
        TestParam::TestIOMode calMode = calParam.mode;

        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
        sp3301 *pSP3301 = calParam._sp3301;
        uint32_t uiRfIdx = calParam.tabIdxRf;
        qint32 attIdx = 0;
        qint32 ref = 30;
        qint32 secCur = 0;
        double_t pwr = 0;
        Instr.init();
        Instr.has_pm();
        Instr.has_sg();
        Instr.pm_reset();
        Instr.sg_reset();
        Instr.sg_set_en_mod(false);
        Instr.sg_set_pl(-60.0);
        Instr.sg_set_en_output(true);

        DYNAMIC_SP1401_R1CE_CAL->set_pwr_meas_src(basic_sp1401::PWR_MEAS_FREE_RUN,false);
        INT_CHECKV(pSP3301->rf_set_tx_state(uiRfIdx,false));
        openLog("接收精度测试_射频通道%d.", uiRfIdx);
        if (testOP(calMode)) {
            CAL_THREAD_TEST_CANCEL
            writeLog("OUT口测试\n\n");
            rdata_f_t data;
            qint32 j = 0;
            QR1CRxAttTestOPModel *model = (QR1CRxAttTestOPModel *)(calParam.model_0);
            INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,OUTPUT));


            msleep(10);
            for (qint32 i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL
                INT_CHECKV(pSP3301->rf_set_rx_freq(uiRfIdx,freq[i]));
                for (ref = 30,j = 0;ref >= -40;ref --,j++) {
                    CAL_THREAD_TEST_CANCEL
                    INT_CHECKV(pSP3301->rf_set_rx_level(uiRfIdx,ref));
                    qint32 signal = ref;
                    if (signal > 10)
                        signal = 10;
                    ajustSG(double(freq[i]),signal);
                    msleep(10);
                    pwr = getRxPwr();
                    pwr += ref;
                    data.freq = freq[i];
                    data.ref = ref;
                    data.RefOffset[i][j] = pwr - signal;
                    if (ref == 30)
                        model->iterTable(i)->at(0)->locate2CalTable(model->calTable()->begin());
                    model->iterTable(i)->at(0)->addOneData();
                    model->calTable()->replace(j,data);

                    emit update(model->index(j ,0),
                                model->index(j ,0),
                                cal_file::RX_ATT_OP,
                                secCur);
                    writeLog("%.2f\t %d\t %.3f\t %.3f \n\n", double(freq[i]/10e5), ref,pwr,data.RefOffset[i][j]);
                }

                SET_PROG_POS(i + 1);
            }

        }
        if (testIO(calMode)) {
            CAL_THREAD_TEST_CANCEL
            writeLog("IO口测试\n\n");
            rdata_f_t data;
            qint32 j = 0;
            QR1CRxAttTestIOModel *model = (QR1CRxAttTestIOModel *)(calParam.model_1);
            INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,IO));

            msleep(10);
            for (qint32 i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL
                INT_CHECKV(pSP3301->rf_set_rx_freq(uiRfIdx,freq[i]));
                for (ref = 30,j = 0;ref >= -40;ref --,j++) {
                    CAL_THREAD_TEST_CANCEL
                    INT_CHECKV(pSP3301->rf_set_rx_level(uiRfIdx,ref));
                    qint32 signal = ref;
                    if (signal > 10)
                        signal = 10;
                    ajustSG(double(freq[i]),signal);
                    msleep(10);
                    pwr = getRxPwr();
                    pwr += ref;
                    data.freq = freq[i];
                    data.ref = ref;
                    data.RefOffset[i][j] = pwr - signal;
                    if (ref == 30)
                        model->iterTable(i)->at(0)->locate2CalTable(model->calTable()->begin());
                    model->iterTable(i)->at(0)->addOneData();
                    model->calTable()->replace(j,data);
                    emit update(model->index(i * attIdx,0),
                                model->index((i + 1) * attIdx,14),
                                cal_file::RX_ATT_IO,
                                secCur);
                    SET_PROG_POS(i + 1);
                    writeLog("%.2f\t %d\t %.3f\t %.3f \n\n", double(freq[i]/10e5), ref,pwr,data.RefOffset[i][j]);
                }
            }
            closeLog();
        CAL_THREAD_ABOART
    }
   }
    catch (sp_rd::runtime_error &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.what()));
        CAL_THREAD_ABOART
    }
}

double QTestR1CRxAttThread::getRxPwr()
{
    double pwr = 0.0;
    basic_sp1401::pwr_meas_state_t state = basic_sp1401::PMS_IDLE;
    DYNAMIC_SP1401_R1CE_CAL->pwr_meas_abort();
    DYNAMIC_SP1401_R1CE_CAL->pwr_meas_start();

    while (state != basic_sp1401::PMS_DONE) {
        calParam._sp1401->get_pwr_meas_state(state);
        if (state == basic_sp1401::PMS_TT)
            return -100.0;
    }
    DYNAMIC_SP1401_R1CE_CAL->get_pwr_meas_pwr(pwr);
    return pwr;
}

void QTestR1CRxAttThread::ajustSG(double freq,qint32 pwr)
{
    if (pwr == sgPwr)
        return;
    double pmPwr = 0.0;
    Instr.sg_set_cw(freq);
    Instr.sg_set_pl(pwr + pmIL);
    msleep(5);
    Instr.pm_set_freq(freq);
    for (qint32 i = 0;i < 10;i ++) {
        Instr.pm_get_pwr(freq,pmPwr);
        if (abs(pwr - pmPwr) < 0.01)
            break;
        Instr.sg_set_pl(pwr + pmIL + (pwr - pmPwr));
        msleep(5);
        pmIL += (pwr - pmPwr);
    }
    msleep(5);
    sgPwr = pwr;
}


