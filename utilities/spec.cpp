#include "spec.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
using namespace sp_rd;
using namespace boost;
using namespace boost::property_tree;

double spec::test_rf_tx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_TX_Freq_Res_Test.Max");
}

double spec::test_rf_tx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_TX_Freq_Res_Test.Min");
}

double spec::test_rf_tx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_TX_Freq_Res_Test.FreqRes");
}

double spec::test_if_tx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_TX_Freq_Res_Test.Max");
}

double spec::test_if_tx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_TX_Freq_Res_Test.Min");
}

double spec::test_if_tx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_TX_Freq_Res_Test.FreqRes");
}

double spec::test_rf_rx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_RX_Freq_Res_Test.Max");
}

double spec::test_rf_rx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_RX_Freq_Res_Test.Min");
}

double spec::test_rf_rx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_RX_Freq_Res_Test.FreqRes");
}

double spec::test_if_rx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_RX_Freq_Res_Test.Max");
}

double spec::test_if_rx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_RX_Freq_Res_Test.Min");
}

double spec::test_if_rx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_RX_Freq_Res_Test.FreqRes");
}

void spec::test_tx_phase_noise_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("TX_Phase_Noise.FreqStar");
    freq.stop = pt.get<string>("TX_Phase_Noise.FreqStop");
    freq.step = pt.get<string>("TX_Phase_Noise.FreqStep");
}

void spec::test_tx_phase_noise_offset(string &offset)
{
    ptree pt;
    read_ini(path(),pt);
    offset = pt.get<string>("TX_Phase_Noise.Offset");
}

void spec::test_tx_phase_noise_span(string &span)
{
    ptree pt;
    read_ini(path(),pt);
    span = pt.get<string>("TX_Phase_Noise.Span");
}

void spec::test_tx_phase_noise_rbw(string &rbw)
{
    ptree pt;
    read_ini(path(),pt);
    rbw = pt.get<string>("TX_Phase_Noise.RBW");
}

double spec::test_tx_phase_noise(uint64_t freq,double offset)
{
    ptree pt;
    read_ini(path(),pt);

    if (freq <= 2000000000) {
        if (offset <= 1000.0) { return pt.get<double>("TX_Phase_Noise.@1K@50_2000"); }
        if (offset <= 10000.0) { return pt.get<double>("TX_Phase_Noise.@10K@50_2000"); }
        if (offset <= 100000.0) { return pt.get<double>("TX_Phase_Noise.@100K@50_2000"); }
    } else if (freq <= 6000000000) {
        if (offset <= 1000.0) { return pt.get<double>("TX_Phase_Noise.@1K@2000_6000"); }
        if (offset <= 10000.0) { return pt.get<double>("TX_Phase_Noise.@10K@2000_6000"); }
        if (offset <= 100000.0) { return pt.get<double>("TX_Phase_Noise.@100K@2000_6000"); }
    }
    return 0.0;
}

double spec::cal_tx_lol()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_LOLeakage.LOLeakage");
}

double spec::cal_tx_sb()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Sideband.Sideband");
}

void spec::cal_tx_filter_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("TX_Filter.FreqStar");
    freq.stop = pt.get<string>("TX_Filter.FreqStop");
    freq.step = pt.get<string>("TX_Filter.FreqStep");
}

double spec::cal_tx_filter_flatness()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Filter.Flatness");
}

double spec::cal_tx_filter_ripple()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Filter.Ripple");
}

string spec::path()
{
    return filesystem::initial_path<filesystem::path>().string()
            + "/"
            + "RF_Spec.ini";
}
