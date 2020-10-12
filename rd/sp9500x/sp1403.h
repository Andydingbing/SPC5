#ifndef SP1403_H
#define SP1403_H

#include "frontend.h"
#include "cal_file_sp9500x.h"
#include "arb_reader.h"
#include "test_data.hpp"
#include <list>
#include "freq_string.hpp"
#include "traits.hpp"
#include <iostream>
#include "enum.h"
#include "ad908x.hpp"

namespace rd { namespace ns_sp1403 {
    BETTER_ENUM(hw_ver_t, int32_t,
                HW_ERROR = -1,
                R1A = 0,
                R1B = 1,
                HW_VER_MAX)

    typedef enum path_t { TX0, TX1, RX } port_t;

    BETTER_ENUM(led_t, uint8_t, Red, Green)

    enum lo_t {
        LO_BEGIN = 0,
        TX_LMX2594_0 = LO_BEGIN,
        TX_LMX2594_1,
        RX_LMX2594_0,
        LO_MAX
    };
} // namespace sp1403

class RD_API sp1403 : public frontend
{
public:
    typedef boost::shared_ptr<sp1403> sptr;

    typedef ns_sp1403::lo_t lo_t;

public:
    sp1403(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~sp1403();

    bool connect(const std::list<pci_dev *> &ctrller);

    int32_t open_board();
    int32_t close_board();

    io_mode_t io_mode() const { return _io_mode_tx0; }
    io_mode_t io_mode(const ns_sp1403::port_t port)
    { return *(static_cast<io_mode_t *>(&_io_mode_tx0 + port)); }

    int32_t get_ctrller_ver(const std::string &des,uint32_t &ver);

    pci_dev *v9() const { return _v9; }
    ns_sp9500x::cal_file *cal_file() const { return _cal_file; }

    static uint32_t ass_ordinal(uint32_t ordinal);
    static int32_t is_valid_sn(const char *sn);

    static ns_sp1403::hw_ver_t parse_hw_ver(const std::string &sn);

public:
    int32_t set_ad998x_reg(const uint16_t addr,const uint8_t data);
    int32_t get_ad998x_reg(const uint16_t addr,uint8_t &data);

public:
    int32_t get_ads5474(int64_t &ad);
    int32_t get_ads5474_manual(int64_t &ad);

protected:
    ns_sp9500x::cal_file *_cal_file;
    pci_dev *_v9;
    ad908x _ad908x;
    io_mode_t _io_mode_tx0;
    io_mode_t _io_mode_tx1;
    io_mode_t _io_mode_rx;
    uint64_t &_tx_freq;
    uint64_t &_rx_freq;
};

} // namespace rd

#endif // SP1403_H
