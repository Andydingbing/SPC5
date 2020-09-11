#ifndef SP9500X_RRH_H
#define SP9500X_RRH_H

#include "frontend.h"
#include <iostream>
#include "enum.h"

namespace rd { namespace ns_sp9500x { namespace ns_rrh {

typedef enum path_t { TRX0, TRX1 } port_t;

} // namespace rrh

class RD_API rrh : public frontend
{
public:
    typedef boost::shared_ptr<rrh> sptr;

    rrh(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~rrh() {}

public:
    bool connect(const std::list<pci_dev *> &ctrller);
    bool is_connected();

    int32_t open_board();
    int32_t close_board();

    int32_t get_ctrller_ver(const std::string &des,uint32_t &ver);

    pci_dev *v9() const { return _v9; }

    int32_t set_pwr_en(const bool en);
    int32_t get_pwr_en(bool &en) const;

    int32_t get_sn(uint32_t &sn) const;

    int32_t set_io_mode(const io_mode_t) { return 0; }
    int32_t set_io_mode(const ns_rrh::port_t port,const io_mode_t mode) const;
    int32_t set_io_mode(const io_mode_t mode_0,const io_mode_t mode_1) const;
    int32_t get_io_mode(const ns_rrh::port_t port,io_mode_t &mode) const;
    int32_t get_io_mode(io_mode_t &mode_0,io_mode_t &mode_1) const;

    int32_t set_tx_freq(const uint64_t freq);
    int32_t set_tx_pwr(const double pwr) const;
    int32_t get_tx_pwr(double &pwr) const;

    int32_t set_rx_freq(const uint64_t freq);
    int32_t set_rx_ref(const double pwr) const;
    int32_t get_rx_ref(double &pwr) const;

    int32_t set_if_loop_sw(const bool en) const;
    int32_t get_if_loop_sw(bool &en) const;

    int32_t get_det(uint16_t &det) const;

    int32_t set_reg(const uint8_t addr,const uint32_t data) const;
    int32_t get_reg(const uint8_t addr,uint32_t &data) const;

protected:
    pci_dev *_v9;
    uint64_t &_tx_freq;
    uint64_t &_rx_freq;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // SP9500X_RRH_H
