#include "qtestr1crxfreqresponseThread.h"
#include "qtestr1crxfreqresponsedlg.h"
#include "qr1crxfreqresponsemodel.h"
#include "algorithm.h"

QTestR1CRxFreqResponseThread::QTestR1CRxFreqResponseThread(TestParam *param, QObject *parent) :
    QTestBaseThread(param,parent)
{

}

void QTestR1CRxFreqResponseThread::run()
{

    try {

            sp2401_r1a *pSP2401 = calParam._sp2401;
            bool fast = calParam.justRebuildCoef;
            uint64_t rxFreq = 0;
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
            int secBfr = -1;
            int secCur = 0;

            TEST_THREAD_START("Rx RF&IF Response",(pts +(int32_t)(IF_RX_FILTER_160M_PTS)));


                THREAD_CHECK_BOX("Rx<===>Signal Generator");

                Instr.init();
                //Instr.has_pm();
                Instr.has_sg();
                //Instr.pm_reset();
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
                rx_ref_op_table_r1cd::guess_base(0,ref,lnaAtt,att019,att1,att2,att3);
                rx_if_fr_table::data_f_t data;
                ref -= 15;

                DYNAMIC_SP1401_R1CE_CAL->set_io_mode(OUTPUT);
                DYNAMIC_SP1401_R1CE_CAL->set_rx_lna_att_sw((sp1401::rx_lna_att_t)(lnaAtt));
                DYNAMIC_SP1401_R1CE_CAL->set_rx_att_019_sw((sp1401::rx_att_019_t)(att019));
                DYNAMIC_SP1401_R1CE_CAL->set_rx_att(att1,att2,att3);
                DYNAMIC_SP1401_R1CE_CAL->set_pwr_meas_src(basic_sp1401::PWR_MEAS_FREE_RUN,false);
                DYNAMIC_SP1401_R1CE_CAL->set_pwr_meas_timeout(6553600);
                DYNAMIC_SP1401_R1CE_CAL->set_pwr_meas_samples(102400);
                DYNAMIC_SP1401_R1CE_CAL->set_tx_att(30.0,30.0,30.0,30.0);
                DYNAMIC_SP1401_R1CE_CAL->set_rx_if_filter_sw(basic_sp1401::_160MHz);
                pSP2401->set_ddc(-92640000.0);
                pSP2401->set_rx_pwr_comp(0);
                pSP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
                pSP2401->set_rx_filter_default();
                msleep(10);
                if (calParam.testFlag = 1) {
                    QR1CRxFreqResponseModel *modelRF = (QR1CRxFreqResponseModel *)(calParam.model_0);
                    if (testOP(calMode)) {
                         DYNAMIC_SP1401_R1CE_CAL->set_io_mode(OUTPUT);
                    }else
                    {
                         DYNAMIC_SP1401_R1CE_CAL->set_io_mode(IO);
                    }
                    for (qint32 i = 0;i < pts;i ++) {
                        CAL_THREAD_TEST_CANCEL
                        Instr.sg_set_cw(freq[i]);
                        data.freq = freq[i];
                         DYNAMIC_SP1401_R1CE_CAL->set_rx_freq(freq[i]);
                        msleep(100);
                        data.pwr = getRxPwr();
                        secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,freq[i]);
                        if (secCur != secBfr) {
                            modelRF->iterTable()->at(secCur)->locate2CalTable(modelRF->calTable()->begin() + i);
                            secBfr = secCur;
                        }
                        modelRF->iterTable()->at(secCur)->addOneData();
                        modelRF->calTable()->replace(i,data);
                        emit update(QModelIndex(),QModelIndex(),cal_file::RX_RF_FR);
                        SET_PROG_POS(i + 1);
                    }
                }
                QR1CRxFreqResponseModel *modelIF = (QR1CRxFreqResponseModel *)(calParam.model_1);
                DYNAMIC_SP1401_R1CE_CAL->set_rx_freq(2000000000);
                msleep(10);
                for (qint32 i = 0;i < IF_RX_FILTER_160M_PTS;i ++) {
                    CAL_THREAD_TEST_CANCEL
                            data.freq = IF_RX_FILTER_160M_FREQ_STAR + i * IF_RX_FILTER_FREQ_STEP;
                    Instr.sg_set_cw(2000000000+data.freq);
                    msleep(50);
                    data.pwr = getRxPwr();

                    if (i == 0) {
                        modelIF->iterTable()->at(0)->locate2CalTable(modelIF->calTable()->begin() + i);

                    }
                    modelIF->iterTable()->at(0)->addOneData();
                    modelIF->calTable()->replace(i,data);

                    emit update(QModelIndex(),QModelIndex(),cal_file::RX_IF_FR);
                    SET_PROG_POS(pts+i + 1);
                }


            CAL_THREAD_ABOART
    }
    catch (sp_rd::runtime_error &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.what()));
        CAL_THREAD_ABOART
    }
}


void QTestR1CRxFreqResponseThread::ajustSG(double freq,qint32 pwr)
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

double QTestR1CRxFreqResponseThread::getRxPwr()
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






QExportR1CRxFreqResponseThread::QExportR1CRxFreqResponseThread(TestParam *param, QObject *parent) : QTestExportBaseThread(param,parent)
{

}

void QExportR1CRxFreqResponseThread::run()
{
    INIT_PROG("Exporting Tx Filter Offset",100);

    SET_PROG_POS(100);
    THREAD_ABORT
}
//QGetsetR1CRxFreqResponseThread::QGetsetR1CRxFreqResponseThread(TestParam *param, QObject *parent) : QTestGetsetBaseThread(param,parent)
//{

//}
//void QGetsetR1CRxFreqResponseThread::run()
//{
//    CAL_THREAD_START("RxFreqResponse",100);
//    THREAD_ABORT
//}
