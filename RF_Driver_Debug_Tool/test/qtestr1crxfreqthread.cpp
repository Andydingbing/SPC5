#include "test/qtestr1crxfreqThread.h"
#include "test/qtestr1crxfreqdlg.h"
#include "test/qr1crxfreqmodel.h"

QTestR1CRxFreqThread::QTestR1CRxFreqThread(TestParam *param, QObject *parent) :
    QTestBaseThread(param,parent)
{

}

void QTestR1CRxFreqThread::run()
{

    try {

            sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
            sp2401_r1a *pSP2401 = calParam._sp2401;
            sp3301 *pSP3301 = calParam._sp3301;
            QString strFreqStar = calParam.rfFreqStar;
            QString strFreqStop = calParam.rfFreqStop;
            QString strFreqStep = calParam.rfFreqStep;
            uint64_t freq[RF_TX_FREQ_PTS_CALLED];
            uint64_t freqStar[16];
            uint64_t freqStop[16];
            uint32_t uiRfIdx = calParam.tabIdxRf;
            memset(freq,0,sizeof(freq));
            memset(freqStar,0,sizeof(freqStar));
            memset(freqStop,0,sizeof(freqStop));
            int pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

            TEST_THREAD_START("Rx RF&IF Freq Response",(pts +(int32_t)(IF_RX_FILTER_160M_PTS)));


                THREAD_CHECK_BOX("Rx<===>Signal Generator");

                Instr.init();
                Instr.has_pm();
                Instr.has_sg();
                Instr.pm_reset();
                Instr.sg_reset();
                Instr.sg_set_en_mod(false);
                Instr.sg_set_pl(-10.0);
                Instr.sg_set_en_output(true);

                qint32 ref = 0;
                qint32 lnaAtt = 0;
                qint32 att019 = 0;
                double att1 = 0.0;
                double att2 = 0.0;
                double att3 = 0.0;
                TestParam::TestIOMode calMode = calParam.mode;
                rx_ref_op_table_r1cd::guess_base(1,ref,lnaAtt,att019,att1,att2,att3);
                rx_if_fr_table::data_f_t data;


                INT_CHECKV(pSP3301->rf_set_tx_state(uiRfIdx,false));
                INT_CHECKV(pSP3301->rf_set_rx_bw(uiRfIdx,bw_t::_160M));
                msleep(10);
                openLog("接收机扫频测试_射频通道%d.", uiRfIdx);
                QR1CRxFreqModel *modelRF = (QR1CRxFreqModel *)(calParam.model_0);
                if (testOP(calMode)) {
                    INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,OUTPUT));
                    writeLog("OUT口\n");
                }else
                {
                    INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,IO));
                    writeLog("IO口\n");
                }
                writeLog("射频频响数据\n");
                for (qint32 i = 0;i < pts;i ++) {
                    CAL_THREAD_TEST_CANCEL
                    Instr.sg_set_cw(freq[i]);
                    msleep(50);
                    ajustSG(double(freq[i]),ref);
                    INT_CHECKV(pSP3301->rf_set_rx_freq(uiRfIdx,freq[i]));
                    INT_CHECKV(pSP3301->rf_set_rx_level(uiRfIdx,ref));
                    data.freq = freq[i];
                    msleep(30);
                    data.pwr = getRxPwr();
                    writeLog("%.1f\t%d\t%.3f\t%.3f \n\n", double(freq[i]/10e5), ref,data.pwr+ref,data.pwr);

                    if (i == 0) {
                        modelRF->iterTable()->at(0)->locate2CalTable(modelRF->calTable()->begin());
                    }
                    modelRF->iterTable()->at(0)->addOneData();
                    modelRF->calTable()->replace(i,data);
                    emit update(QModelIndex(),QModelIndex(),cal_file::RX_RF_FR);
                    SET_PROG_POS(i + 1);
                }
                writeLog("中频频响数据\n");
                QR1CRxFreqModel *modelIF = (QR1CRxFreqModel *)(calParam.model_1);

                INT_CHECKV(pSP3301->rf_set_rx_freq(uiRfIdx,2000000000));
                INT_CHECKV(pSP3301->rf_set_rx_level(uiRfIdx,ref));
                msleep(10);
                for (qint32 i = 0;i < (IF_RX_FILTER_160M_PTS * 2);i ++) {
                    CAL_THREAD_TEST_CANCEL
                            data.freq = IF_RX_FILTER_160M_FREQ_STAR + (i* IF_RX_FILTER_FREQ_STEP)/2;//
                    Instr.sg_set_cw(2000000000+data.freq);
                    ajustSG(2000000000+data.freq,ref);
                    msleep(30);
                    data.pwr = getRxPwr();
                    writeLog("%.1f\t%d\t%.3f \n\n", double(freq[i]/10e5), ref,data.pwr);
                    if (i == 0) {
                        modelIF->iterTable()->at(0)->locate2CalTable(modelIF->calTable()->begin());
                    }
                    modelIF->iterTable()->at(0)->addOneData();
                    modelIF->calTable()->replace(i,data);

                    emit update(QModelIndex(),QModelIndex(),cal_file::RX_IF_FR);
                    SET_PROG_POS(pts+i + 1);
                }

                closeLog();
                CAL_THREAD_ABOART
    }
    catch (sp_rd::runtime_error &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.what()));
        CAL_THREAD_ABOART
    }
}


void QTestR1CRxFreqThread::ajustSG(double freq,qint32 pwr)
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

double QTestR1CRxFreqThread::getRxPwr()
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

