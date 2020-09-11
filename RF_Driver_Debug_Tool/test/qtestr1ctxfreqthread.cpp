#include "qtestr1ctxfreqthread.h"
#include "qtestr1ctxfreqdlg.h"
#include "qr1ctxfreqmodel.h"
#include "algorithm.h"

QTestR1CTxFreqThread::QTestR1CTxFreqThread(TestParam *param, QObject *parent) : QTestBaseThread(param,parent)
{

}

void QTestR1CTxFreqThread::run()
{
    try {
        TEST_THREAD_START("Tx Filter",(RF_TX_TEST_FILTER_PTS +RF_TX_TEST_FILTER_PTS+(int32_t)(IF_TX_FILTER_160M_PTS)));

        QwtTxRfFrTestData *dataRfFr_0 = (QwtTxRfFrTestData *)(calParam.plotData_0);
        QwtTxRfFrTestData *dataRfFr_1 = (QwtTxRfFrTestData *)(calParam.plotData_1);
        QwtTxIfFrTestData *dataIfFr = (QwtTxIfFrTestData *)(calParam.plotData_2);
        tx_rf_fr_table::data_f_t tempDataRfFr;
        tx_if_fr_table::data_f_t tempDataIfFr;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp3301 *pSP3301 = calParam._sp3301;
        uint32_t uiRfIdx = calParam.tabIdxRf;
        uint64_t txFreq = 0;
        int32_t TestPower = 0;
        TestParam::TestIOMode calMode = calParam.mode;
        if (testOP(calMode))
        {
            THREAD_CHECK_BOX("Tx<===>Power Meter");
            INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,OUTPUT));
            TestPower = -5;
            openLog("发频响测试_OUT-射频通道%d.", uiRfIdx);
        }
        if (testIO(calMode))
        {
            THREAD_CHECK_BOX("IO<===>Power Meter");
            INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,IO));
            TestPower = -12;
            openLog("发频响测试_IO-射频通道%d.", uiRfIdx);
        }

            Instr.init();
            Instr.has_pm();
            Instr.pm_reset();
            //            pSP2401->set_dds1(0.0);
            //            pSP2401->set_duc_dds(0.0);

            pSP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
            pSP1401->set_rx_att_019_sw(sp1401::RX_ATT_19);
            pSP1401->set_rx_att(31.0,31.0,31.0);
            pSP2401->set_tx_filter_sw(true);
            INT_CHECKV(pSP3301->rf_set_tx_src(uiRfIdx,sp2401_r1a::SINGLE_TONE));
            msleep(10);
            for (int32_t i = 0;i < RF_TX_TEST_FILTER_PTS;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL
                tempDataRfFr.freq = RF_TX_FREQ_STAR*10 + i * RF_TX_FILTER_FREQ_STEP;
                txFreq = tempDataRfFr.freq < RF_TX_FREQ_STAR*10 ? RF_TX_FREQ_STAR*10 : tempDataRfFr.freq;
                txFreq = tempDataRfFr.freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : tempDataRfFr.freq;
                INT_CHECKV(pSP3301->rf_set_tx_freq(uiRfIdx,txFreq));
                INT_CHECKV(pSP3301->rf_set_tx_pwr(uiRfIdx,TestPower));
                Instr.pm_set_freq((double)(txFreq));
                msleep(20);
                Instr.pm_get_pwr(tempDataRfFr.pwr);
                tempDataRfFr.pwr-=TestPower;
                dataRfFr_0->calTable()->append(tempDataRfFr);
                writeLog("%.1f\t%.3f\t%.3f \n\n", double(txFreq/10e5), tempDataRfFr.pwr,((double)TestPower)-tempDataRfFr.pwr);
                emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_0);
                SET_PROG_POS(i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            TestPower = calParam.getTxBasePower.toInt();

            for (int32_t i = 0;i < RF_TX_TEST_FILTER_PTS;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL
                tempDataRfFr.freq = RF_TX_FREQ_STAR*10 + i * RF_TX_FILTER_FREQ_STEP;
                txFreq = tempDataRfFr.freq > RF_TX_FREQ_STAR*10 ? RF_TX_FREQ_STOP*10 : tempDataRfFr.freq;
                txFreq = tempDataRfFr.freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : tempDataRfFr.freq;
                INT_CHECKV(pSP3301->rf_set_tx_freq(uiRfIdx,txFreq));
                INT_CHECKV(pSP3301->rf_set_tx_pwr(uiRfIdx,TestPower));
                Instr.pm_set_freq((double)(txFreq));
                msleep(20);
                Instr.pm_get_pwr(tempDataRfFr.pwr);
                tempDataRfFr.pwr-=TestPower;
                dataRfFr_1->calTable()->append(tempDataRfFr);
                emit update(QModelIndex(),QModelIndex(),cal_file::TX_RF_FR_1);
                writeLog("%.1f\t%.3f\t%.3f \n\n", double(txFreq/10e5), tempDataRfFr.pwr,((double)TestPower)-tempDataRfFr.pwr);
                SET_PROG_POS(RF_TX_TEST_FILTER_PTS + i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            TestPower = -10;
            str2freq(calParam.getTxCompenFiltFreq.toStdString().c_str(),txFreq);
            INT_CHECKV(pSP3301->rf_set_tx_freq(uiRfIdx,2000000000));
            msleep(10);
            for (int32_t i = 0;i < IF_TX_FILTER_160M_PTS;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL
                tempDataIfFr.freq = IF_TX_FILTER_160M_FREQ_STAR + i * IF_TX_FILTER_FREQ_STEP;
                pSP2401->set_duc_dds(tempDataIfFr.freq);
                INT_CHECKV(pSP3301->rf_set_tx_pwr(uiRfIdx,TestPower));
                Instr.pm_set_freq((double)2000000000 + tempDataIfFr.freq);
                Sleep(20);
                Instr.pm_get_pwr(tempDataIfFr.pwr);
                tempDataIfFr.pwr-=TestPower;
                dataIfFr->calTable()->append(tempDataIfFr);
                emit update(QModelIndex(),QModelIndex(),cal_file::TX_IF_FR);
                writeLog("%.3f\t%.3f\t%.3f \n\n", double(tempDataIfFr.freq/10e5), tempDataRfFr.pwr,((double)TestPower)-tempDataRfFr.pwr);
                SET_PROG_POS(RF_TX_TEST_FILTER_PTS * 2 + i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            closeLog();
            CAL_THREAD_ABOART
    }
    catch (sp_rd::runtime_error &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.what()));
        CAL_THREAD_ABOART
    }
}

