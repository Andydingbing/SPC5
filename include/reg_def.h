#ifndef INCLUDE_REG_DEF_H
#define INCLUDE_REG_DEF_H

#define CONFIG_FILE_PATH "config.ini"

#include "liblog.h"
#include <boost/current_function.hpp>

/*
 * 1 address
 *
 * union reg0x... {
 *     struct reg_1_t {
 *         a : x;
 *         b : x; field(some_chip)
 *     } r1;
 *     struct reg_2_t {
 *         a : x;
 *         b : x;
 *     } r2;
 *     ...
 *     uintx_t data;
 * };
 *
 *
 * 2 addresses
 *
 * struct reg0x...0x..._t {
 *     uintx_t addr[4];
 *     union reg {
 *         struct reg_1_t {
 *             a : x;
 *             b : x; field(some_chip)
 *         } r1;
 *         struct reg_2_t {
 *             a : x;
 *             b : x;
 *         } r2;
 *         ...
 *         uintx_t data;
 *     } u_data;
 * };
 *
 */

#define field(chips)

#define REG_DEF_PREFIX_8(addr)  union reg##addr {
#define REG_DEF_SUFFIX_8        uint8_t  data; };

#define REG_DEF_PREFIX_16(addr) union reg##addr {
#define REG_DEF_SUFFIX_16       uint16_t data; };

#define REG_DEF_PREFIX_32(addr) union reg##addr {
#define REG_DEF_SUFFIX_32       uint32_t data; };

#define REG_DEF_PREFIX_64(addr) union reg##addr {
#define REG_DEF_SUFFIX_64       uint64_t data; };

#define REG_DEF_PREFIX_8_2(addr0,addr1)  struct reg##addr0##addr1##_t { uint8_t  addr[4]; union reg {
#define REG_DEF_SUFFIX_8_2               uint8_t data;  } u_data; };

#define REG_DEF_PREFIX_16_2(addr0,addr1) struct reg##addr0##addr1##_t { uint16_t addr[4]; union reg {
#define REG_DEF_SUFFIX_16_2              uint16_t data; } u_data; };

#define REG_DEF_PREFIX_32_2(addr0,addr1) struct reg##addr0##addr1##_t { uint32_t addr[4]; union reg {
#define REG_DEF_SUFFIX_32_2              uint32_t data; } u_data; };

#define REG_DEF_PREFIX_64_2(addr0,addr1) struct reg##addr0##addr1##_t { uint64_t addr[4]; union reg {
#define REG_DEF_SUFFIX_64_2              uint64_t data; } u_data; };

#define REG_PREFIX struct reg_t {
#define REG_SUFFIX } r;

#define REG_1_PREFIX(chip_or_field) struct reg_1_t {
#define REG_1_SUFFIX(chip_or_field) } r_##chip_or_field;

#define REG_2_PREFIX(chip_or_field) struct reg_2_t {
#define REG_2_SUFFIX(chip_or_field) } r_##chip_or_field;

#define REG_3_PREFIX(chip_or_field) struct reg_3_t {
#define REG_3_SUFFIX(chip_or_field) } r_##chip_or_field;

// Compatible registers or chips
#define REG_DEF_CPT_PREFIX_8(addr)  REG_DEF_PREFIX_8(addr)  REG_PREFIX
#define REG_DEF_CPT_SUFFIX_8        REG_SUFFIX REG_DEF_SUFFIX_8

#define REG_DEF_CPT_PREFIX_16(addr) REG_DEF_PREFIX_16(addr) REG_PREFIX
#define REG_DEF_CPT_SUFFIX_16       REG_SUFFIX REG_DEF_SUFFIX_16

#define REG_DEF_CPT_PREFIX_32(addr) REG_DEF_PREFIX_32(addr) REG_PREFIX
#define REG_DEF_CPT_SUFFIX_32       REG_SUFFIX REG_DEF_SUFFIX_32

#define REG_DEF_CPT_PREFIX_64(addr) REG_DEF_PREFIX_64(addr) REG_PREFIX
#define REG_DEF_CPT_SUFFIX_64       REG_SUFFIX REG_DEF_SUFFIX_64

#define REG_DEF_CPT_PREFIX_8_2(addr0,addr1)  REG_DEF_PREFIX_8_2(addr0,addr1)  REG_PREFIX
#define REG_DEF_CPT_SUFFIX_8_2               REG_SUFFIX REG_DEF_SUFFIX_8_2

#define REG_DEF_CPT_PREFIX_16_2(addr0,addr1) REG_DEF_PREFIX_16_2(addr0,addr1) REG_PREFIX
#define REG_DEF_CPT_SUFFIX_16_2              REG_SUFFIX REG_DEF_SUFFIX_16_2

#define REG_DEF_CPT_PREFIX_32_2(addr0,addr1) REG_DEF_PREFIX_32_2(addr0,addr1) REG_PREFIX
#define REG_DEF_CPT_SUFFIX_32_2              REG_SUFFIX REG_DEF_SUFFIX_32_2

#define REG_DEF_CPT_PREFIX_64_2(addr0,addr1) REG_DEF_PREFIX_64_2(addr0,addr1) REG_PREFIX
#define REG_DEF_CPT_SUFFIX_64_2              REG_SUFFIX REG_DEF_SUFFIX_64_2

#define REG_DECL_INIT(ns,addr,init) union ns::reg##addr ns##r##addr; ns##r##addr.data = init; NEED_SEMICOLON
#define REG_DECL(ns,addr) REG_DECL_INIT(ns,addr,0)
#define REG_DATA(ns,addr) ns##r##addr.data
#define REG_CLER(ns,addr) REG_DATA(ns,addr) = 0;
#define REG(ns,addr)  ns##r##addr.r
#define REG_OF(ns,addr,chip_or_field) ns##r##addr.r_##chip_or_field

#define REG_DECL_INIT_2(ns,addr0,addr1,init) \
    struct ns::reg##addr0##addr1##_t ns##r##addr0##addr1; \
    ns##r##addr0##addr1.addr[0] = addr1; \
    ns##r##addr0##addr1.addr[1] = addr0; \
    ns##r##addr0##addr1.addr[2] = addr1; \
    ns##r##addr0##addr1.addr[3] = addr0; \
    ns##r##addr0##addr1.u_data.data = init; NEED_SEMICOLON

#define REG_DECL_2(ns,addr0,addr1) REG_DECL_INIT_2(ns,addr0,addr1,0)
#define REG_DATA_2(ns,addr0,addr1) ns##r##addr0##addr1.u_data.data
#define REG_CLER_2(ns,addr0,addr1) REG_DATA_2(ns,addr0,addr1) = 0;
#define REG_2(ns,addr0,addr1) ns##r##addr0##addr1.u_data.r
#define REG_OF_2(ns,addr0,addr1,chip_or_field) ns##r##addr0##addr1.r_##chip_or_field

// IO
#define PCIE_DEV_W(dev,bar,ns,addr) \
    do {int32_t ret = 0; \
        if ((ret = dev->w32(bar,addr<<2,REG_DATA(ns,addr)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,REG_DATA(ns,addr)); \
    } while (0); NEED_SEMICOLON

#define PCIE_DEV_R(dev,bar,ns,addr) \
    do {int32_t ret = 0; \
        if ((ret = dev->r32(bar,addr<<2,&REG_DATA(ns,addr)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,0xffffffff,REG_DATA(ns,addr)); \
    } while (0); NEED_SEMICOLON

#define PCIE_DEV_W_2(dev,bar,ns,addr0,addr1) \
    do {int32_t ret = 0; \
        uint32_t addr = ns##r##addr0##addr1.addr[_rf_idx]; \
        if ((ret = dev->w32(bar,addr<<2,REG_DATA_2(ns,addr0,addr1)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,REG_DATA_2(ns,addr0,addr1)); \
    }while (0); NEED_SEMICOLON

#define PCIE_DEV_R_2(dev,bar,ns,addr0,addr1) \
    do {int32_t ret = 0; \
        uint32_t addr = ns##r##addr0##addr1.addr[_rf_idx]; \
        if ((ret = dev->r32(bar,addr<<2,&REG_DATA_2(ns,addr0,addr1)))) { \
            Log.set_last_err("%s:%s line%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
            return ret; \
        } \
        Log.add_reg(ret,dev->descriptor(),addr,0xffffffff,REG_DATA_2(ns,addr0,addr1)); \
    }while (0); NEED_SEMICOLON

#define PCIE_DEV_SET_BITS(dev,addr,bits,data) \
    dev##_R(addr); \
    dev##_REG(addr).bits = data; \
    dev##_W(addr);

#define PCIE_DEV_SET_BITS_2(dev,addr0,addr1,bits,data) \
    dev##_R_2(addr0,addr1); \
    dev##_REG_2(addr0,addr1).bits = data; \
    dev##_W_2(addr0,addr1);

// Rising edge
#define CTRLLER_RE(ctrller,addr,bit) \
    do {ctrller##_REG(addr).bit = 0; \
        ctrller##_W(addr); \
        ctrller##_REG(addr).bit = 1; \
        ctrller##_W(addr); \
    } while (0); NEED_SEMICOLON

#define CTRLLER_RE_2(ctrller,addr0,addr1,bit) \
    do {ctrller##_REG_2(addr0,addr1).bit = 0; \
        ctrller##_W_2(addr0,addr1); \
        ctrller##_REG_2(addr0,addr1).bit = 1; \
        ctrller##_W_2(addr0,addr1); \
    } while (0); NEED_SEMICOLON

// State machine
#define CTRLLER_WAIT_IDLE(ctrller,addr,bit,flag,times) \
    do {bool idle = false; \
        ctrller##_R(addr); \
        for (int32_t i = 0;i < times;++i) { \
            if (flag == ctrller##_REG(addr).bit) { \
                idle = true; \
                break; \
            } \
            ctrller##_R(addr); \
        } \
        if (!idle) { \
            Log.set_last_err("Reg%#06x wait idle timeout",addr); \
            return -1; \
        } \
    } while (0); NEED_SEMICOLON

#define CTRLLER_WAIT_IDLE_2(ctrller,addr0,addr1,bit,flag,times) \
    do {bool idle = false; \
        ctrller##_R_2(addr0,addr1); \
        for (int32_t i = 0;i < times;++i) { \
            if (flag == ctrller##_REG_2(addr0,addr1).bit) { \
                idle = true; \
                break; \
            } \
            ctrller##_R_2(addr0,addr1); \
        } \
        if (!idle) { \
            Log.set_last_err("Reg%#06x%#06x wait idle timeout",addr0,addr1); \
            return -1; \
        } \
    } while (0); NEED_SEMICOLON

#endif // INCLUDE_REG_DEF_H
