#include "sp3501.h"
#include "reg_def.h"
#include "gen_ini_file.h"
#include "algorithm.h"
#include <string>
#include "algo_chip.h"
#include "cal_table.h"
#include "sleep_common.h"

using namespace sp_rd;

sp3501::sp3501()
{
    m_s6 = new vi_pci_dev("s6");
}

sp3501::~sp3501()
{
    SAFE_DEL(m_s6);
}

sp3501 &sp3501::instance()
{
    static sp3501 g_sp3501;
    return g_sp3501;
}

int32_t sp3501::get_s6_ver(uint32_t &ver)
{
	return 0;
}

int32_t sp3501::open_board()
{
    char str_product_form[64];
    char str_rsrc_section[64];
    char str_dev_key[64];
    char str_rsrc_name[32];
    gen_ini_file ini_file(CONFIG_FILE_PATH);

    memset(str_product_form,0,ARRAY_SIZE(str_product_form));
    memset(str_rsrc_section,0,ARRAY_SIZE(str_rsrc_section));
    memset(str_dev_key,0,ARRAY_SIZE(str_dev_key));
    memset(str_rsrc_name,0,ARRAY_SIZE(str_rsrc_name));
    ini_file.get_config_str_value("Product Form","Form",str_product_form);
    strcpy(str_rsrc_section,"RESOURCE");
    strcat(str_rsrc_section,str_product_form);

    ini_file.get_config_str_value(str_rsrc_section,"CXU_S6",str_rsrc_name);

    INT_CHECK(m_s6->init(str_rsrc_name,0x10ee,0x3501));
	return 0;
}

int32_t sp3501::close_board()
{
	return 0;
}

int32_t sp3501::vol_9119(uint16_t val)
{
    CXU_S6_REG_DECLARE(0x000e);
    CXU_S6_REG_DECLARE(0x000f);
    CXU_S6_REG(0x000f).sign = 0;
    CXU_S6_REG(0x000f).val = val;
    CXU_S6_W(0x000f);
    CXU_S6_OP(0x000e);
	return 0;
}

int32_t sp3501::vol_9119(double vol)
{
    return vol_9119(ns_x9119::voltage_to_tap(vol));
}

int32_t sp3501::set_fan(uint32_t idx,uint32_t speed)
{
    CXU_S6_REG_DECLARE(0x0012);
    CXU_S6_REG_DECLARE(0x0013);

    fan_map_t blower_map[NUM_FAN] = {
        fan_map_t(0x0012,0x0032,0),
        fan_map_t(0x0012,0x0032,1),
        fan_map_t(0x0012,0x0033,0),
        fan_map_t(0x0012,0x0033,1),
        fan_map_t(0x0012,0x0034,0),
        fan_map_t(0x0012,0x0034,1),
        fan_map_t(0x0013,0x0032,0),
        fan_map_t(0x0013,0x0032,1),
        fan_map_t(0x0013,0x0033,0),
        fan_map_t(0x0013,0x0033,1),
        fan_map_t(0x0013,0x0034,0),
        fan_map_t(0x0013,0x0034,1)
    };
    if (idx < 6) {
        CXU_S6_REG(0x0012).speed = speed;
        CXU_S6_REG(0x0012).addr = blower_map[idx]._addr;
        CXU_S6_REG(0x0012).blower = blower_map[idx]._fan_sel;
        CXU_S6_OP(0x0012);
    } else {
        CXU_S6_REG(0x0013).speed = speed;
        CXU_S6_REG(0x0013).addr = blower_map[idx]._addr;
        CXU_S6_REG(0x0013).blower = blower_map[idx]._fan_sel;
        CXU_S6_OP(0x0013);
    }
    sleep_ms(10);
    return 0;
}

int32_t sp3501::set_fan(uint32_t speed)
{
    for (uint32_t i = 0;i < NUM_FAN;i ++) {
        INT_CHECK(set_fan(i,speed));
    }
	return 0;
}

int32_t sp3501::autoFanControlRoutinue(double *RFtemp, uint8_t *speed)
{
    static uint8_t current = 0;
    double fan,rf = 0;
    rf = *RFtemp;

    if (rf < 38) {
        fan = 10;
    } else if (rf < 40) {
        fan = 10 + (rf - 38) / 2 * 70;
    } else if (rf < 43) {
        fan = 80 + (rf - 40) / 3 * 30;
    } else if (rf < 50) {
        fan = 110 + (rf - 43) / 7 * 50;
    } else if (rf < 60) {
        fan = 160 + (rf - 50) / 10 * 40;
    } else {
        fan = 240;
    }
    fan = (int)(fan / 10.0 + 0.5) * 10;

    if (speed)
        *speed = (uint8_t)fan;

    if (current == 0 || current != (uint8_t)fan) {
        set_fan(int32_t (*speed));
        current = (uint8_t)fan;

    }

    return 0;
}
