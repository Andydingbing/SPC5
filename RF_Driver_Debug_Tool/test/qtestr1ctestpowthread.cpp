#include "qtestr1ctestpowthread.h"
#include "qtestr1ctestpowdlg.h"
#include "Limit.h"
#include <QDebug>
QTestR1CTestPowThread::QTestR1CTestPowThread(TestParam *param, QObject *parent) : QTestBaseThread(param,parent)
  , m_maxPower(-5)
  , m_minPower(-100)
  , m_stepPower(1)
{

}

void QTestR1CTestPowThread::run()
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
        TEST_THREAD_START("Tx Pow",pts);

        TestParam::TestIOMode calMode = calParam.mode;
        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
        sp3301 *pSP3301 = calParam._sp3301;
        uint32_t uiRfIdx = calParam.tabIdxRf;

        tx_lol_table_r1cd::data_m_t dataLOL;
        tx_sb_table_r1cd::data_m_t dataSB;
        data_f_t data;
        Instr.init();
        Instr.has_sa();
        Instr.sa_reset();
        Instr.sa_set_span(20000.0);
        Instr.sa_set_rbw(1000.0,true);
        Instr.sa_set_vbw(1000.0,true);
        pSP2401->set_dds1(0.0);
        pSP2401->set_duc_dds(0.0);

        pSP2401->set_tx_filter_sw(true);
        INT_CHECKV(pSP3301->rf_set_tx_src(uiRfIdx,sp2401_r1a::SINGLE_TONE));
        if (testOP(calMode)) {
            THREAD_CHECK_BOX("Tx<===>Spectrum");
            openLog("发精度测试_OUT-射频通道%d.", uiRfIdx);

            INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,OUTPUT));
            Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 25.0);
            Instr.sa_set_en_preamp(false);
            Instr.sa_set_mech_att(true);
            Instr.sa_set_avg_trace(false,0);
            for (int32_t i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL

                        if(test(freq[i]/10e5,i,data,true)<0)
                {
                    closeLog();
                    THREAD_CHECK_BOX("Line loss exceeded 4dB, Do you want to continue?.");
                    return;
                }
                SET_PROG_POS(i + 1);
            }
        }
        if (testOP(calMode) && testIO(calMode)) {
            THREAD_CHECK_BOX("Output Complete,IO<===>Sepctrum");
        }
        if (testIO(calMode)) {
            THREAD_CHECK_BOX("IO<===>Spectrum");

             openLog("发精度测试_IO-射频通道%d.", uiRfIdx);

            INT_CHECKV(pSP3301->rf_set_io_mode(uiRfIdx,IO));
            Instr.sa_set_ref(R1C_TX_BASE_POWER_IO + 25.0);
            Instr.sa_set_en_preamp(false);
            Instr.sa_set_mech_att(true);
            Instr.sa_set_avg_trace(false,0);
            for (int32_t i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL

                        if(test(freq[i]/10e5,i,data,false)<0)
                {
                    closeLog();
                    THREAD_CHECK_BOX("Line loss exceeded 4dB, Do you want to continue?.");
                    return;
                }
                SET_PROG_POS(i + 1);
            }
        }
        closeLog();
        CAL_THREAD_ABOART
    }
    catch (sp_rd::runtime_error &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.what()));
        CAL_THREAD_ABOART
    }
    return;
}

int QTestR1CTestPowThread::test(double freqMHz,int32_t Freidx,data_f_t &data,bool io_mode_b)
{

    int      saRefLevel = (int)m_maxPower + 5;
    int      saRefLevelCurr = saRefLevel;
    double   saRefLevelOffset = 0;
    double   saPreampOffset = 0;
    BOOL     isOpenPreamp = FALSE;
    double   testPower;
    int      i;
    double   saPwr, errPwr;
    double   cableloss = 0;
    sp3301 *pSP3301 = calParam._sp3301;
    uint32_t uiRfIdx = calParam.tabIdxRf;
    QR1CTestPowIOModel *model_io;
    QR1CTestPowOPModel *model_op;

    TestParam::TestIOMode calMode = calParam.mode;
    if (testOP(calMode))
        model_op = (QR1CTestPowOPModel *)(calParam.model_0);
    if (testIO(calMode))
        model_io = (QR1CTestPowIOModel *)(calParam.model_1);

    INT_CHECK(pSP3301->rf_set_tx_freq(uiRfIdx,freqMHz*10e5));
    INT_CHECK(pSP3301->rf_set_tx_pwr(uiRfIdx,m_maxPower));

    Instr.sa_set_cf((freqMHz) * 1e6);
    Instr.sa_set_span(1e4);
    Instr.sa_set_ref(saRefLevel);
    Instr.sa_set_rbw(30.0,true);
    Instr.sa_sweep_once();
    Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
    Instr.sa_set_det(sa::DET_AVERAGE);
    Instr.sa_set_en_preamp(true);
    cableloss = initSpectrumAnalyzerOffset(freqMHz, (int)m_maxPower + 5);
#if DEBUG_ACC
    FILE *fp = fopen("./acc_debug.txt", "a");
#endif
    if (cableloss > 4 )
    {
        return -2;
    }

    for (testPower = m_maxPower, i = 0; testPower > m_minPower - 0.1; testPower -= m_stepPower, i++) {

        INT_CHECK(pSP3301->rf_set_tx_pwr(uiRfIdx,testPower));
        if (saRefLevel - testPower > 20) {
            saRefLevel = (int)testPower + 5;
            if (saRefLevel <= -70) {
                saRefLevel = -70;
            }
        }

        if (saRefLevelCurr != saRefLevel) {
            double saPwr1, saPwr2;

            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(saPwr1);

            Instr.sa_set_ref(saRefLevel);
            if (!isOpenPreamp && testPower <= -60) {
                Instr.sa_set_avg_trace(true, 3);
                Instr.sa_set_mech_att(false, 2);
//				Sleep(500);
            }
//			Sleep(200);

            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(saPwr2);

            saRefLevelOffset += saPwr1 - saPwr2;
#if DEBUG_ACC
    fprintf(fp, "switch reflevel from %d to %d\n", saRefLevelCurr, saRefLevel);
    fprintf(fp, "reflevel %d power %f\n", saRefLevelCurr, saPwr1);
    fprintf(fp, "reflevel %d power %f\n", saRefLevel, saPwr2);
#endif
            saRefLevelCurr = saRefLevel;
        }

        Instr.sa_sweep_once();
        Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
        Instr.sa_get_marker_pwr(saPwr);
#ifdef DEBUG_ACC
        fprintf(fp, "tx power %f, measure power %f\n", testPower, saPwr);
#endif

        saPwr += saRefLevelOffset + saPreampOffset + cableloss;
        errPwr = saPwr - testPower;

        writeLog("%.1f\t%.1f\t%.3f\t%.3f\t%.3f \n\n", freqMHz, testPower,cableloss,saPwr, errPwr);
        writeLog("\n");
        data.ActualPower = saPwr;
        data.cableloss   = cableloss;
        data.freq        = freqMHz;
        data.PowOffset[Freidx][i]   = errPwr;
        data.TargetPower = testPower;


        if(io_mode_b == true)
        {
            if (testPower == m_maxPower)
                model_op->iterTable(Freidx)->at(0)->locate2CalTable(model_op->calTable()->begin());

            model_op->calTable()->replace(i, data);
            model_op->iterTable(Freidx)->at(0)->addOneData();
          emit update(model_op->index(i * R1C_TX_ATT_OP_POWER_PTS, 0),
                      model_op->index((i + 1) * R1C_TX_ATT_OP_POWER_PTS, 13),
                      cal_file::TX_ATT_OP, 0);
        }
        if(io_mode_b == false)
        {
            if (testPower == m_maxPower)
              model_io->iterTable(Freidx)->at(0)->locate2CalTable(model_io->calTable()->begin());
          model_io->calTable()->replace(i, data);
          model_io->iterTable(Freidx)->at(0)->addOneData();
          emit update(model_io->index(i * R1C_TX_ATT_OP_POWER_PTS, 0),
                      model_io->index((i + 1) * R1C_TX_ATT_OP_POWER_PTS, 13),
                      cal_file::TX_ATT_IO, 0);
        }

        if (testPower > -90 && LIMIT.fail("发精度测试.", errPwr))
            m_result = FAIL;
    }

    Instr.sa_set_avg_trace(false,0);
    Instr.sa_set_det(sa::DET_AUTO);
    Instr.sa_set_en_preamp(false);
    Instr.sa_set_mech_att(true);

#ifdef DEBUG_ACC
    fclose(fp);
#endif

    return 0;
}
double QTestR1CTestPowThread::initSpectrumAnalyzerOffset(double freqMHz, int refLevel) { 
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    sp3301 *pSP3301 = calParam._sp3301;
    uint32_t uiRfIdx = calParam.tabIdxRf;
    int32_t txPowerBase;
    double saPwr, saPwr2;
    double offset;

    TestParam::TestIOMode calMode = calParam.mode;
    if (testOP(calMode))
        txPowerBase = R1C_TX_BASE_POWER_OP;
    if (testIO(calMode) && !testOP(calMode))
        txPowerBase = R1C_TX_BASE_POWER_IO;

    int32_t currRefLevel = txPowerBase + 5;
    INT_CHECK(pSP3301->rf_set_tx_pwr(uiRfIdx,txPowerBase));
    Instr.sa_set_ref(txPowerBase + 5);
    Instr.sa_sweep_once();
    Instr.sa_get_marker_pwr(saPwr);

#if DEBUG_ACC
    fprintf(fp, "freq = %f, tx power = %d\n", freqMHz, txPowerBase);
    fprintf(fp, "reflevel = %d, power = %f\n", txPowerBase + 5, saPwr);
#endif
    offset = txPowerBase - saPwr;

    while (currRefLevel != refLevel) {
        if (currRefLevel > refLevel) {
            currRefLevel -= (currRefLevel - refLevel > 10 ? 10 : currRefLevel - refLevel);
        } else if (currRefLevel < refLevel) {
            currRefLevel += (refLevel - currRefLevel > 10 ? 10 : refLevel  - currRefLevel);
        }
        Instr.sa_set_ref(currRefLevel);
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(saPwr2);

        offset += (saPwr - saPwr2);
        INT_CHECK(pSP3301->rf_set_tx_pwr(uiRfIdx,currRefLevel - 5));
        Instr.sa_sweep_once();
        Instr.sa_get_marker_pwr(saPwr);
#if DEBUG_ACC
    fprintf(fp, "reflevel = %d, power = %f\n", currRefLevel, saPwr2);
    fprintf(fp, "reflevel = %d, power = %f\n", currRefLevel - 5, saPwr);
#endif
    }
#if DEBUG_ACC
    fclose(fp);
#endif

    return offset;
}


void QTestR1CTestPowThread::calOneSecIO(int32_t pts, uint64_t *freq, uint64_t *freqStar, uint64_t *freqStop, int32_t *idxOffsetStar, double pwrStar, double pwrStop)
{

    return;
}

QExportR1CTestPowThread::QExportR1CTestPowThread(TestParam *param, QObject *parent) : QTestExportBaseThread(param,parent)
{

}

void QExportR1CTestPowThread::run()
{
    INIT_PROG("Exporting Tx Att",100);

    SET_PROG_POS(100);
    THREAD_ABORT
}
//QGetsetR1CTestPowThread::QGetsetR1CTestPowThread(TestParam *param, QObject *parent) : QTestGetsetBaseThread(param,parent)
//{

//}
//void QGetsetR1CTestPowThread::run()
//{
//    CAL_THREAD_START("TestPow",100);
//    THREAD_ABORT
//}
