#ifndef FRONTEND_H
#define FRONTEND_H

#include "rd.h"
#include "cal_table.h"
#include "test_data.hpp"
#include "enum.h"
#include <list>
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>
#include <boost/core/ignore_unused.hpp>

namespace rd {

BETTER_ENUM(io_mode, int32_t, IO, OUTPUT, LOOP, INPUT, CLOSE)

class RD_API frontend : boost::noncopyable
{
public:
    typedef boost::shared_ptr<frontend> sptr;

    enum rf_ch_t { CH_TX = 0, CH_RX = 1 };

public:
    frontend();
    frontend(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~frontend() {}
    uint32_t rf_idx() const { return _rf_idx; }
    uint32_t rfu_idx() const { return _rfu_idx; }

    virtual bool connect(const std::list<pci_dev *> &ctrller);
    virtual bool is_connected() { return false; }

    virtual int32_t open_board();
    virtual int32_t close_board() { return 0; }

    virtual int32_t get_ctrller_ver(const std::string &des,uint32_t &ver)
    { boost::ignore_unused(des,ver); return 0; }

    virtual int32_t hw_ver() const { return -1; }

    virtual int32_t get_sn_major(std::string &sn) { boost::ignore_unused(sn); return 0; }

    virtual int32_t set_io_mode(const io_mode_t mode) { boost::ignore_unused(mode); return 0; }
    virtual io_mode_t io_mode() const { return OUTPUT; }

    virtual int32_t set_tx_freq(const uint64_t freq) { boost::ignore_unused(freq); return 0; }
    virtual uint64_t tx_freq() { return *(_tx_freqs.begin()); }
    virtual std::string tx_freq_string() { return freq_string_from_uint64_t(tx_freq()); }

    virtual int32_t set_tx_att(const double att,const int32_t port = 0)
    { boost::ignore_unused(att,port); return 0; }

    virtual int32_t set_tx_mod_en(bool en) { boost::ignore_unused(en); return 0; }

    virtual int32_t set_pwr_en(const bool en) { boost::ignore_unused(en); return 0; }

    virtual int32_t set_rx_freq(const uint64_t freq) { boost::ignore_unused(freq); return 0; }
    virtual uint64_t rx_freq() { return *(_rx_freqs.begin()); }
    virtual std::string rx_freq_string() { return freq_string_from_uint64_t(rx_freq()); }

public:
    virtual double tx_base_pwr(const uint64_t freq,const io_mode_t mode) const
    { boost::ignore_unused(freq,mode); return 0.0; }

    void set_tx_en_tc(const bool en) { _en_tx_tc = en; }
    void set_rx_en_tc(const bool en) { _en_rx_tc = en; }
    bool is_tx_en_tc() const { return _en_tx_tc; }
    bool is_rx_en_tc() const { return _en_rx_tc; }

    virtual void tx_att_states(std::list<common_atts_t> &states) { states.clear(); }
    virtual void tx_att_states(std::list<common_atts_t> &states,std::list<bool> &checked)
    { states.clear(); checked.clear(); }

    virtual void rx_att_states(std::list<common_atts_t> &states) { states.clear(); }
    virtual void rx_att_states(std::list<common_atts_t> &states,std::list<bool> &checked)
    { states.clear(); checked.clear(); }

public:
    template <typename ftp_call_back_t>
    static void set_ftp_retry_call_back(ftp_call_back_t f) { _ftp_retry_call_back = f; }

public:
    uint32_t _rf_idx;
    uint32_t _rfu_idx;
    std::list<pci_dev *> _ctrller_pci;
    std::list<common_lo_t *> _lo_tx;
    std::list<common_lo_t *> _lo_rx;
    std::list<uint64_t> _tx_freqs;
    std::list<uint64_t> _rx_freqs;

    bool _en_tx_tc; // Enable T/RX power temperature compansate?
    bool _en_rx_tc;

    static boost::function<bool()> _ftp_retry_call_back;
};

} // namespace rd

#endif // FRONTEND_H
