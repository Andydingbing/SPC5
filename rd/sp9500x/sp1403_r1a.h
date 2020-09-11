#ifndef SP1403_R1A_H
#define SP1403_R1A_H

#include "sp1403.h"

#define DECL_SW(sw_name,sw_enum) \
    int32_t set_##sw_name(const sw_enum &sw) const; \
    int32_t get_##sw_name(sw_enum &sw) const;

namespace rd { namespace ns_sp1403 { namespace r1a {
    BETTER_ENUM(tx_sw1_t, uint32_t,
                TX_SW1_300_3000,
                TX_SW1_3000_8000)

    BETTER_ENUM(tx_sw2_t, uint32_t,
                TX_SW2_300_3000,
                TX_SW2_3000_8000)

    BETTER_ENUM(tx_sw3_t, uint32_t,
                TX_SW3_6000_8000,
                TX_SW3_3000_6000)

    BETTER_ENUM(tx_sw4_t, uint32_t,
                TX_SW4_4800_6000,
                TX_SW4_3000_4800)

    BETTER_ENUM(tx0_sw5_t, uint32_t,
                TO_OUT,
                TO_IO)

    BETTER_ENUM(tx0_sw6_t, uint32_t,
                TO_LOOP,
                TO_OUT)

    BETTER_ENUM(tx0_sw7_t, uint32_t,
                TO_OFF,
                TO_OUT)

    BETTER_ENUM(tx0_sw8_t, uint32_t,
                TO_OFF,
                TO_LOOP)

    BETTER_ENUM(tx0_sw9_t, uint32_t,
                TO_TX1_LOOP,
                TO_TX0_IN)

    BETTER_ENUM(tx0_sw10_t, uint32_t,
                TO_RX,
                TO_TX0_LOOP)

    BETTER_ENUM(tx0_sw11_t, uint32_t,
                TO_RX,
                TO_OFF)

    BETTER_ENUM(tx0_sw12_t, uint32_t,
                TO_RX,
                TO_OFF)

    BETTER_ENUM(tx1_sw5_t, uint32_t,
                TO_LOOP,
                TO_OUT)

    BETTER_ENUM(tx1_sw6_t, uint32_t,
                TO_OFF,
                TO_OUT)

    BETTER_ENUM(rx_sw1_t, uint32_t,
                TO_ATT,
                TO_LNA)

    BETTER_ENUM(rx_sw2_t, uint32_t,
                TO_ATT_0,
                TO_ATT_10)

    BETTER_ENUM(rx_sw3_t, uint32_t,
                _300_1000,
                _1000_8000)

    BETTER_ENUM(rx_lna_att_t, uint32_t,
                RX_ATT_0 = 0,
                RX_ATT_10 = 1,
                Reserved,
                RX_LNA = 3)

    BETTER_ENUM(temp_t, uint32_t,
                TX_LO1 = 0,
                TX0_SW = 1,
                TX0_PA = 4,
                RX_LNA = 5,
                TX1_PA = 6)

    enum lo_t { LO_BEGIN = 0,TX_LMX2594_0 = LO_BEGIN,TX_LMX2594_1,RX_LMX2594_0,LO_MAX };

} // namespace r1a
} // namespace ns_sp1403

class RD_API sp1403_r1a : public sp1403
{
public:
    typedef boost::shared_ptr<sp1403_r1a> sptr;

    typedef ns_sp1403::path_t path_t;
    typedef ns_sp1403::port_t port_t;
    typedef ns_sp1403::led_t  led_t;

    typedef ns_sp1403::r1a::tx_sw1_t   tx_sw1_t;
    typedef ns_sp1403::r1a::tx_sw2_t   tx_sw2_t;
    typedef ns_sp1403::r1a::tx_sw3_t   tx_sw3_t;
    typedef ns_sp1403::r1a::tx_sw4_t   tx_sw4_t;
    typedef ns_sp1403::r1a::tx0_sw5_t  tx0_sw5_t;
    typedef ns_sp1403::r1a::tx0_sw6_t  tx0_sw6_t;
    typedef ns_sp1403::r1a::tx0_sw7_t  tx0_sw7_t;
    typedef ns_sp1403::r1a::tx0_sw8_t  tx0_sw8_t;
    typedef ns_sp1403::r1a::tx0_sw9_t  tx0_sw9_t;
    typedef ns_sp1403::r1a::tx0_sw10_t tx0_sw10_t;
    typedef ns_sp1403::r1a::tx0_sw11_t tx0_sw11_t;
    typedef ns_sp1403::r1a::tx0_sw12_t tx0_sw12_t;
    typedef ns_sp1403::r1a::tx1_sw5_t  tx1_sw5_t;
    typedef ns_sp1403::r1a::tx1_sw6_t  tx1_sw6_t;

    typedef ns_sp1403::r1a::rx_sw1_t rx_sw1_t;
    typedef ns_sp1403::r1a::rx_sw2_t rx_sw2_t;
    typedef ns_sp1403::r1a::rx_sw3_t rx_sw3_t;
    typedef ns_sp1403::rx_sw4_t      rx_sw4_t;

    typedef ns_sp1403::r1a::temp_t temp_t;

    sp1403_r1a(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~sp1403_r1a() {}

    enum att_t {
        TX0_ATT0,TX0_ATT1,TX0_ATT2,TX0_ATT3,
        TX1_ATT0,TX1_ATT1,TX1_ATT2,TX1_ATT3,
        RX_ATT0,RX_ATT1,ATT_MAX
    };

public:
    bool is_connected();
    int32_t open_board();
    int32_t close_board();

    int32_t hw_ver() const { return ns_sp1403::hw_ver_t::R1A; }

    int32_t set_io_mode(const io_mode_t mode);
    int32_t set_io_mode(const ns_sp1403::port_t port,const io_mode_t mode);

    int32_t set_led(const port_t port,const led_t &led) const;
    int32_t get_led(const port_t port,led_t &led) const;

    int32_t set_tx_freq(const uint64_t freq);

    int32_t set_att(const att_t att,const double value) const;
    int32_t get_att(const att_t att,double &value) const;

    int32_t set_rx_freq(const uint64_t freq);
    int32_t set_rx_lna_att_sw(const ns_sp1403::r1a::rx_lna_att_t sw) const;

    int32_t set_rx_bw(const ns_sp1403::rx_bw_t bw) const { return set_rx_sw4(bw); }
    int32_t get_rx_bw(ns_sp1403::rx_bw_t &bw) const { return get_rx_sw4(bw); }

    int32_t get_rx_lna_att_sw(ns_sp1403::r1a::rx_lna_att_t &sw) const;
    int32_t get_rx_att0(double &att);
    int32_t get_rx_att1(double &att);

    int32_t get_temp(const temp_t &idx,double &temp) const;

    int32_t init_lo(const ns_sp1403::r1a::lo_t lo);
    int32_t set_lo(const ns_sp1403::r1a::lo_t lo,const uint64_t freq);

    int32_t set_lo_reg(const ns_sp1403::r1a::lo_t lo,const uint8_t addr,const uint16_t data);
    int32_t get_lo_reg(const ns_sp1403::r1a::lo_t lo,const uint8_t addr,uint16_t &data);

    common_lo_t *lo(const ns_sp1403::r1a::lo_t id) const
    { return (common_lo_t *)(&_tx_lmx2594_0 + id * sizeof(common_lo_t *)); }

    common_lo_t *tx_lmx2594_0() const { return _tx_lmx2594_0; }
    common_lo_t *tx_lmx2594_1() const { return _tx_lmx2594_1; }
    common_lo_t *rx_lmx2594_0() const { return _rx_lmx2594_0; }

    std::string lo_freq_string(const ns_sp1403::r1a::lo_t id)
    const { return freq_string_from_uint64_t(lo(id)->freq); }

    std::string tx_lmx2594_0_freq_string() const
    { return _tx_lmx2594_0 == nullptr ? "" : freq_string_from_uint64_t(_tx_lmx2594_0->freq); }

    std::string tx_lmx2594_1_freq_string() const
    { return _tx_lmx2594_1 == nullptr ? "" : freq_string_from_uint64_t(_tx_lmx2594_1->freq); }

    std::string rx_lmx2594_0_freq_string() const
    { return _rx_lmx2594_0 == nullptr ? "" : freq_string_from_uint64_t(_rx_lmx2594_0->freq); }

    int32_t set_tx_lmx2594_0_reg(const uint8_t addr,const uint16_t data);
    int32_t get_tx_lmx2594_0_reg(const uint8_t addr,uint16_t &data);
    int32_t set_tx_lmx2594_1_reg(const uint8_t addr,const uint16_t data);
    int32_t get_tx_lmx2594_1_reg(const uint8_t addr,uint16_t &data);

    int32_t set_rx_lmx2594_0_reg(const uint8_t addr,const uint16_t data);
    int32_t get_rx_lmx2594_0_reg(const uint8_t addr,uint16_t &data);

    int32_t set_s6_reg(const uint8_t addr,const uint16_t data) const;
    int32_t get_s6_reg(const uint8_t addr,uint16_t &data) const;

    DECL_SW(tx0_sw1,tx_sw1_t)
    DECL_SW(tx0_sw2,tx_sw2_t)
    DECL_SW(tx0_sw3,tx_sw3_t)
    DECL_SW(tx0_sw4,tx_sw4_t)
    DECL_SW(tx0_sw5,tx0_sw5_t)
    DECL_SW(tx0_sw6,tx0_sw6_t)
    DECL_SW(tx0_sw7,tx0_sw7_t)
    DECL_SW(tx0_sw8,tx0_sw8_t)
    DECL_SW(tx0_sw9,tx0_sw9_t)
    DECL_SW(tx0_sw10,tx0_sw10_t)
    DECL_SW(tx0_sw11,tx0_sw11_t)
    DECL_SW(tx0_sw12,tx0_sw12_t)

    DECL_SW(tx1_sw1,tx_sw1_t)
    DECL_SW(tx1_sw2,tx_sw2_t)
    DECL_SW(tx1_sw3,tx_sw3_t)
    DECL_SW(tx1_sw4,tx_sw4_t)
    DECL_SW(tx1_sw5,tx1_sw5_t)
    DECL_SW(tx1_sw6,tx1_sw6_t)

    DECL_SW(rx_sw1,rx_sw1_t)
    DECL_SW(rx_sw2,rx_sw2_t)
    DECL_SW(rx_sw3,rx_sw3_t)
    DECL_SW(rx_sw4,rx_sw4_t)

public:
    void tx_freq_to_lo(const uint64_t freq);
    void rx_freq_to_lo(const uint64_t freq);

private:
    common_lo_t *_tx_lmx2594_0;
    common_lo_t *_tx_lmx2594_1;
    common_lo_t *_rx_lmx2594_0;
};

} // namespace rd

#endif // SP1403_R1A_H
