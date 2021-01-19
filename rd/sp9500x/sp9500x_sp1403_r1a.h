#ifndef SP9500X_SP1403_R1A_H
#define SP9500X_SP1403_R1A_H

#include "sp1403_r1a.h"
#include "sp9500x_sp1403.h"

#define DECL_SW_OVERRIDE(sw_name,sw_enum) \
    int32_t set_##sw_name(const sw_enum &sw) const OVERRIDE; \
    int32_t get_##sw_name(sw_enum &sw) const OVERRIDE;

#define DECL_SW(sw_name,sw_enum) \
    int32_t set_##sw_name(const sw_enum &sw) const; \
    int32_t get_##sw_name(sw_enum &sw) const;

#define IMPL_SW(class_name,reg,sw_name,sw_enum) \
int32_t class_name::set_##sw_name(const sw_enum &sw) const \
{ \
    SP1403_S6_REG_DECL(reg); \
    INT_CHECK(get_s6_reg(reg,SP1403_S6_REG_DATA(reg))); \
    SP1403_S6_REG(reg).sw_name = sw; \
    INT_CHECK(set_s6_reg(reg,SP1403_S6_REG_DATA(reg))); \
    return 0; \
} \
int32_t class_name::get_##sw_name(sw_enum &sw) const \
{ \
    SP1403_S6_REG_DECL(reg); \
    INT_CHECK(get_s6_reg(reg,SP1403_S6_REG_DATA(reg))); \
    sw = sw_enum::_from_integral(SP1403_S6_REG(reg).sw_name); \
    return 0; \
}

namespace rd {
namespace ns_sp9500x {

class RD_API sp1403_r1a : virtual public rd::sp1403_r1a, public ns_sp9500x::sp1403
{
public:
    typedef boost::shared_ptr<sp1403_r1a> sptr;

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

    typedef ns_sp1403::r1a::rx_bw_t      rx_bw_t;
    typedef ns_sp1403::r1a::rx_sw1_t     rx_sw1_t;
    typedef ns_sp1403::r1a::rx_sw2_t     rx_sw2_t;
    typedef ns_sp1403::r1a::rx_sw3_t     rx_sw3_t;
    typedef ns_sp1403::r1a::rx_sw4_t     rx_sw4_t;
    typedef ns_sp1403::r1a::rx_lna_att_t rx_lna_att_t;

    typedef ns_sp1403::r1a::temp_t temp_t;
    typedef ns_sp1403::r1a::att_t  att_t;

    sp1403_r1a(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~sp1403_r1a() {}

public:
    bool connect(const std::list<pci_dev *> &ctrller) OVERRIDE;
    bool is_connected() OVERRIDE;
    virtual int32_t open_board() OVERRIDE;
    virtual int32_t close_board() OVERRIDE;

    int32_t hw_ver() const OVERRIDE
    { return ns_sp1403::hw_ver_t::R1A; }

    int32_t get_ctrller_ver(const std::string &des,uint32_t &ver) OVERRIDE;

    int32_t set_io_mode(const io_mode_t mode) OVERRIDE;
    virtual int32_t set_io_mode(const ns_sp1403::port_t port,const io_mode_t mode) OVERRIDE;

    int32_t set_led(const port_t port,const led_t &led) const OVERRIDE;
    int32_t get_led(const port_t port,led_t &led) const OVERRIDE;

    virtual int32_t set_tx_freq(const uint64_t freq) OVERRIDE;

    int32_t set_att(const att_t att,const double value) const OVERRIDE;
    int32_t get_att(const att_t att,double &value) const OVERRIDE;

    virtual int32_t set_rx_freq(const uint64_t freq) OVERRIDE;
    int32_t set_rx_lna_att_sw(const rx_lna_att_t sw) const OVERRIDE;
    int32_t get_rx_lna_att_sw(ns_sp1403::r1a::rx_lna_att_t &sw) const OVERRIDE;

    virtual int32_t set_rx_bw(const rx_bw_t bw) const  OVERRIDE
    { return set_rx_sw4(bw); }

    virtual int32_t get_rx_bw(rx_bw_t &bw) const  OVERRIDE
    { return get_rx_sw4(bw); }

    int32_t set_att(const uint8_t idx,const double &att) const OVERRIDE
    { return set_att(att_t(idx),att); }

    int32_t get_att(const uint8_t idx,double &att) const OVERRIDE
    { return get_att(att_t(idx),att); }

    int32_t get_temp(const temp_t &idx,double &temp) const OVERRIDE;

    int32_t init_lo(const lo_t lo) OVERRIDE;
    int32_t set_lo(const lo_t lo,const uint64_t freq) OVERRIDE;

    int32_t set_lo_reg(const lo_t lo,const uint8_t addr,const uint16_t data) OVERRIDE;
    int32_t get_lo_reg(const lo_t lo,const uint8_t addr,uint16_t &data) OVERRIDE;

    int32_t set_tx_lmx2594_0_reg(const uint8_t addr,const uint16_t data) OVERRIDE;
    int32_t get_tx_lmx2594_0_reg(const uint8_t addr,uint16_t &data) OVERRIDE;
    int32_t set_tx_lmx2594_1_reg(const uint8_t addr,const uint16_t data) OVERRIDE;
    int32_t get_tx_lmx2594_1_reg(const uint8_t addr,uint16_t &data) OVERRIDE;

    int32_t set_rx_lmx2594_0_reg(const uint8_t addr,const uint16_t data) OVERRIDE;
    int32_t get_rx_lmx2594_0_reg(const uint8_t addr,uint16_t &data) OVERRIDE;

    int32_t set_s6_reg(const uint8_t addr,const uint16_t data) const OVERRIDE;
    int32_t get_s6_reg(const uint8_t addr,uint16_t &data) const OVERRIDE;

    DECL_SW_OVERRIDE(tx0_sw1,tx_sw1_t)
    DECL_SW_OVERRIDE(tx0_sw2,tx_sw2_t)
    DECL_SW_OVERRIDE(tx0_sw3,tx_sw3_t)
    DECL_SW_OVERRIDE(tx0_sw4,tx_sw4_t)
    DECL_SW_OVERRIDE(tx0_sw5,tx0_sw5_t)
    DECL_SW_OVERRIDE(tx0_sw6,tx0_sw6_t)
    DECL_SW_OVERRIDE(tx0_sw7,tx0_sw7_t)
    DECL_SW_OVERRIDE(tx0_sw8,tx0_sw8_t)
    DECL_SW_OVERRIDE(tx0_sw9,tx0_sw9_t)
    DECL_SW_OVERRIDE(tx0_sw10,tx0_sw10_t)
    DECL_SW_OVERRIDE(tx0_sw11,tx0_sw11_t)
    DECL_SW_OVERRIDE(tx0_sw12,tx0_sw12_t)

    DECL_SW_OVERRIDE(tx1_sw1,tx_sw1_t)
    DECL_SW_OVERRIDE(tx1_sw2,tx_sw2_t)
    DECL_SW_OVERRIDE(tx1_sw3,tx_sw3_t)
    DECL_SW_OVERRIDE(tx1_sw4,tx_sw4_t)
    DECL_SW_OVERRIDE(tx1_sw5,tx1_sw5_t)
    DECL_SW_OVERRIDE(tx1_sw6,tx1_sw6_t)

    DECL_SW_OVERRIDE(rx_sw1,rx_sw1_t)
    DECL_SW_OVERRIDE(rx_sw2,rx_sw2_t)
    DECL_SW_OVERRIDE(rx_sw3,rx_sw3_t)
    DECL_SW_OVERRIDE(rx_sw4,rx_sw4_t)

public:
    virtual int32_t set_tx_state(const port_t port,const data_f_tx_pwr &data) const OVERRIDE;

    virtual double tx_base_pwr(const uint64_t freq,const io_mode_t mode) const OVERRIDE;

    virtual void tx_state(const uint64_t freq,const io_mode_t mode,data_f_tx_pwr &state) const OVERRIDE;

    virtual void tx_state(const uint64_t freq,const io_mode_t mode,const data_f_tx_pwr &base_state,data_f_tx_pwr &state) const OVERRIDE;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // SP9500X_SP1403_R1A_H
