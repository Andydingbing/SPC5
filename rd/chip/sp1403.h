#ifndef SP1403_H
#define SP1403_H

#include "frontend.h"
#include <iostream>
#include "enum.h"

/*
 *
 *                                  frontend
 *                                     |
 *                                     |
 *                          +------- sp1403 -------+
 *                          |                      |
 *                          |                      |
 *              +----- sp1403_r1a -----+       sp1403_r1b/c/...
 *              |                      |           |
 *              |                      |         ......
 *    sp1403_r1a(sp9500x)    sp1403_r1a(sp9500pro)
 */

namespace rd {
namespace ns_sp1403 {
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
    typedef ns_sp1403::port_t port_t;
    typedef ns_sp1403::path_t path_t;
    typedef ns_sp1403::led_t  led_t;
    typedef ns_sp1403::lo_t   lo_t;

public:
    sp1403(uint32_t rf_idx,uint32_t rfu_idx);

    virtual int32_t open_board();

    virtual int32_t set_io_mode(const io_mode_t) = 0;
    virtual int32_t set_io_mode(const ns_sp1403::port_t,const io_mode_t) = 0;

    virtual int32_t set_led(const port_t,const led_t &) const = 0;
    virtual int32_t get_led(const port_t,led_t &) const = 0;

    virtual int32_t set_s6_reg(const uint8_t addr,const uint16_t data) const = 0;
    virtual int32_t get_s6_reg(const uint8_t addr,uint16_t &data) const = 0;

    io_mode_t io_mode() const { return _io_mode_tx0; }
    io_mode_t io_mode(const ns_sp1403::port_t port)
    { return *(static_cast<io_mode_t *>(&_io_mode_tx0 + port)); }

public:
    io_mode_t _io_mode_tx0;
    io_mode_t _io_mode_tx1;
    io_mode_t _io_mode_rx;
    uint64_t &_tx_freq;
    uint64_t &_rx_freq;
};

} // namespace rd

#endif // SP1403_H
