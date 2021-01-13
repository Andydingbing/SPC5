#include "sp9500x_cal_tx_pwr_thread.h"
#include "sp9500x_cal_tx_pwr_childwidgets.h"
#include "cal_widget.h"
#include "spec.h"
#include "test_data.hpp"
#include "algo_math.hpp"

using namespace std;
using namespace ns_sp1403;
using namespace ns_sp2406;
using namespace ns_sp9500x;
using namespace NS_SP9500X;
using namespace set_helper;

void Q_Cal_TX_Pwr_Thread::cal()
{
    CAL_THREAD_START("TX Filter",(1));

//    setTempRange(2.0);

    Q_Cal_TX_Pwr_Widget *p = dynamic_cast<Q_Cal_TX_Pwr_Widget *>(parent());
    Q_TX_Pwr_Config_Delegate *delegate = p->childs->configDelegate;

    bool cal_TX0_Output = delegate->checkBoxTX0_Output->isChecked();
    bool cal_TX0_IO = delegate->checkBoxTX0_IO->isChecked();
    bool cal_TX1 = delegate->checkBoxTX1->isChecked();
    int i = 0;

    freqString = p->ui->textEditRFFreqs->toPlainText().toStdString();

    parse(freqString,freqRange);

    THREAD_CHECK_BOX("TX<===>Power Meter");

    Instr.init();
    Instr.has_pm();
    Instr.pm_reset();

    init();

    uint64_t freq = 0;
    tx_pwr_table_t::data_f_t data;

    for (set<uint64_t>::iterator iter = freqRange.freq.begin();iter != freqRange.freq.end();++iter,++i) {
        THREAD_TEST_CANCEL

        freq = *iter;

        SP1403->tx_state(freq,OUTPUT,data);

        SP1403->set_tx_freq(freq);
        Instr.pm_set_freq(freq);

        coarseTuning(freq,data);
        fineTuning(freq,data);

        data.set_key(freq);
        SP1403->cal_file()->add(cal_table_t::TX0_Pwr_Output,&data);
        emit uiUpdate(i,i,cal_table_t::TX0_Pwr_Output);
    }

    SP1403->cal_file()->persistence(cal_table_t::TX0_Pwr_Output);

    THREAD_TEST_CANCEL;
    THREAD_TEST_CANCEL;
}

void Q_Cal_TX_Pwr_Thread::coarseTuning(const uint64_t freq,data_f_tx_pwr &data)
{
    const double target = SP1403->tx_base_pwr(freq,OUTPUT);
    double pwrPM = 0.0;
    double offset = 0.0;
    double att0 = data_f_tx_pwr::att_double(data.att[0]);
    int count = 0;

    SP2406->set_dl_pwr_comp(data.d_gain);
    SP1403->set_tx_state(TX1,data);

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.5) {
        att0 -= offset;
        att0 = linear_quantify(0.0,0.5,att0);
        att0 = att0 < 0.0 ? 0.0 : att0;
        data.set_att(0,att0);

        SP1403_R1B->set_att(rd::sp1403_r1b::att_t::TX1_ATT0,att0);
        msleep(50);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10) {
            break;
        }
    }
}

void Q_Cal_TX_Pwr_Thread::fineTuning(const uint64_t freq,data_f_tx_pwr &data)
{
    const double target = SP1403->tx_base_pwr(freq,OUTPUT);
    double pwrPM = 0.0;
    double offset = 0.0;
    int count = 0;

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.02) {
        data.d_gain += float(offset);

        SP2406->set_dl_pwr_comp(data.d_gain);
        msleep(50);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10) {
            break;
        }
    }
}

void Q_Cal_TX_Pwr_Thread::init()
{
    SP2406->set_dl_src(dl_src_t::Disable);
    SP2406->set_dl_src(0,dl_src_t::DDS0);
    SP2406->set_dl_src_dds0_gain(-12.0);
    SP2406->set_dl_cpri_sr(dl_cpri_sr_t::_12288);
    SP2406->set_dl_lpf_en(false);
    SP2406->set_dl_hbf0_en(true);
    SP2406->set_dl_hbf1_en(true);
    SP2406->set_dl_cf_en(false);
    SP2406->set_dl_jesd_src(dl_jesd_src_t::DUC);
    SP2406->set_dl_pwr_comp(-10.0);

    SP1403->set_io_mode(OUTPUT);
    msleep(10);
}

void Q_Cal_TX_Pwr_Thread::check()
{

}


void Q_Exp_TX_Pwr_Thread::run()
{
    initProgress("Exporting TX Power",100);
    addProgressPos(100);
    THREAD_ENDED
}
