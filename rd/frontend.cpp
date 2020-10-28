#include "frontend.h"
#include "ftp.h"
#include <boost/filesystem/path.hpp>

using namespace std;
using namespace rd;
using namespace boost::filesystem;

boost::function<bool()> frontend::_ftp_retry_call_back = boost::function<bool()>();

frontend::frontend()
{
    _rf_idx = 0;
    _rfu_idx = 0;
    _en_tx_tc = false;
    _en_rx_tc = false;
}

frontend::frontend(uint32_t rf_idx,uint32_t rfu_idx)
{
    _rf_idx = rf_idx;
    _rfu_idx = rfu_idx;

    _tx_freqs.push_back(uint64_t());
    _rx_freqs.push_back(uint64_t());

    _en_tx_tc = false;
    _en_rx_tc = false;
}

bool frontend::connect(const list<pci_dev *> &ctrller)
{
    list<pci_dev *>::const_iterator iter;
    for (iter = ctrller.begin();iter != ctrller.end();iter ++) {
        _ctrller_pci.push_back(*iter);
    }
    return true;
}

int32_t frontend::open_board()
{
    list<common_lo_t *>::iterator iter_lo;
    list<uint64_t>::iterator iter_freq;

    for (iter_lo = _lo_tx.begin();iter_lo != _lo_tx.end();++iter_lo) {
        (*iter_lo)->freq = 0;
        (*iter_lo)->is_opened = false;
    }
    for (iter_lo = _lo_rx.begin();iter_lo != _lo_rx.end();++iter_lo) {
        (*iter_lo)->freq = 0;
        (*iter_lo)->is_opened = false;
    }
    for (iter_freq = _tx_freqs.begin();iter_freq != _tx_freqs.end();++iter_freq) {
        *iter_freq = 0;
    }
    for (iter_freq = _rx_freqs.begin();iter_freq != _rx_freqs.end();++iter_freq) {
        *iter_freq = 0;
    }
    return 0;
}
