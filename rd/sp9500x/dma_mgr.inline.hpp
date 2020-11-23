#include "../sp9500x/dma_mgr.hpp"
#include "reg_def_sp9500x.h"
#include "memory_physical.hpp"
#include "sleep_common.h"
#include "pci_dev_vi.h"
#include <boost/timer/timer.hpp>

using namespace rd;

ns_sp9500x::dma_mgr::~dma_mgr()
{
    _memory_dl.release();
    _memory_ul.release();
}

int32_t ns_sp9500x::dma_mgr::init(const uint32_t dl_samples,const uint32_t ul_samples)
{
    INT_CHECK(_memory_dl.allocate(dl_samples * 4,nullptr,true));
    INT_CHECK(_memory_ul.allocate(ul_samples * 4,nullptr,true));

    SP9500X_RFU_V9_REG_DECL(0x0180);
    SP9500X_RFU_V9_REG_DECL(0x0181);
    SP9500X_RFU_V9_REG_DECL(0x0182);
    SP9500X_RFU_V9_REG_DECL(0x0183);
    SP9500X_RFU_V9_REG_DECL(0x0184);
    SP9500X_RFU_V9_REG_DECL(0x0185);
    SP9500X_RFU_V9_REG_DECL(0x0186);
    SP9500X_RFU_V9_REG_DECL(0x0187);
    SP9500X_RFU_V9_REG_DECL(0x0188);

    // DL / FPGA Read
    SP9500X_RFU_V9_REG(0x0186).addr_low  = _memory_dl.phy_addr(0) & 0x00000000ffffffff;
    SP9500X_RFU_V9_REG(0x0187).addr_high = _memory_dl.phy_addr(0) >> 32;
    SP9500X_RFU_V9_REG(0x0188).length    = _memory_dl.block_size(0);
    SP9500X_RFU_V9_W(0x0186);
    SP9500X_RFU_V9_W(0x0187);
    SP9500X_RFU_V9_W(0x0188);

    // UL / FPGA Write
    SP9500X_RFU_V9_REG(0x0181).cnt = _memory_ul.total_block();
    SP9500X_RFU_V9_W(0x0181);

    for (uint32_t i = 0;i < _memory_ul.total_block();++i) {
        SP9500X_RFU_V9_REG(0x0182).n = i;
        SP9500X_RFU_V9_REG(0x0183).addr_low  = _memory_ul.phy_addr(i) & 0x00000000ffffffff;
        SP9500X_RFU_V9_REG(0x0184).addr_high = _memory_ul.phy_addr(i) >> 32;
        SP9500X_RFU_V9_REG(0x0185).length    = _memory_ul.block_size(i);
        SP9500X_RFU_V9_W(0x0182);
        SP9500X_RFU_V9_W(0x0183);
        SP9500X_RFU_V9_W(0x0184);
        SP9500X_RFU_V9_W(0x0185);
        SP9500X_RFU_V9_OP(0x0180);
    }

    INT_CHECK(set_fpga_ddr_addr_r(0));
    INT_CHECK(set_fpga_ddr_addr_w(0x100000000));
    INT_CHECK(set_fpga_w_samples(245760));
    INT_CHECK(set_fpga_w_timeout_time(2000000));
    return 0;
}

RD_INLINE bool is_timeout(boost::timer::cpu_timer &time,uint32_t timeout)
{
    return uint32_t(time.elapsed().wall / 1000) > timeout;
}

int32_t ns_sp9500x::dma_mgr::fpga_r(const uint32_t samples)
{
    SP9500X_RFU_V9_REG_DECL(0x018e);
    SP9500X_RFU_V9_REG_DECL(0x018f);
    SP9500X_RFU_V9_REG_DECL(0x0192);

    uint32_t byte_cur[5] = {0};
    const uint32_t byte_target = samples << 2;
    const uint32_t timeout = 6000000;
    boost::timer::cpu_timer time;

    Log.stdprintf("DMA:FPGA prepare read samples:%d\n",samples);

    SP9500X_RFU_V9_REG(0x018f).length = byte_target;
    SP9500X_RFU_V9_W(0x018f);
    SP9500X_RFU_V9_OP(0x018e);

    /* wait interrupt
    for (int i = 0; i < 3; i++) {
        ret = WaitForSingleObject(g_Event, 1000);
        Log.stdprintf("WaitForSingleObject:%d\n",ret);

        CSE_K7_R(0x0021);
        Log.stdprintf("REG(0x0021) = %d\n",REG(0x0021).dma_rd_counter);
        if (ret != WAIT_TIMEOUT)
        break;
    }*/

    SP9500X_RFU_V9_R(0x0192);
    while (SP9500X_RFU_V9_REG(0x0192).byte == 0) {
        SP9500X_RFU_V9_R(0x0192);

        if (is_timeout(time,timeout)) {
            Log.set_last_err("DMA:FPGA read timeout.");
            return -1;
        }
    }

    while(1) {
        for (int32_t i = 0;i < 5;++i) {
            SP9500X_RFU_V9_R(0x0192);
            byte_cur[i] = SP9500X_RFU_V9_REG(0x0192).byte;

            if (byte_cur[i] == byte_target) {
                Log.stdprintf("DMA:FPGA read complete.\n");
                return 0;
            }
            if (i > 0 && (byte_cur[i] == byte_cur[i - 1])) {
                Log.set_last_err("DMA:FPGA read stop @byte:%d",byte_cur[i]);
                return -1;
            }
            if (byte_cur[i] > byte_target) {
                Log.set_last_err("DMA:FPGA read exceed,bytes:%d(need:%d",byte_cur[i],byte_target);
                return -1;
            }
        }

        if (is_timeout(time,timeout)) {
            Log.set_last_err("DMA:FPGA read timeout.");
            return -1;
        }
    }
}

int32_t ns_sp9500x::dma_mgr::fpga_w() const
{
    INT_CHECK(fpga_w_start());
    return fpga_w_trans();
}

int32_t ns_sp9500x::dma_mgr::fpga_w_start() const
{
    SP9500X_RFU_V9_REG_DECL(0x0189);

    SP9500X_RFU_V9_OP(0x0189);
    return 0;
}

int32_t ns_sp9500x::dma_mgr::fpga_w_trans() const
{
    SP9500X_RFU_V9_REG_DECL(0x018a);
    SP9500X_RFU_V9_REG_DECL(0x018d);

    uint32_t byte_cur[5] = { 0 };
    uint32_t byte_target = 0;

    bool is_timeout = false;

    INT_CHECK(get_fpga_w_samples(byte_target));

    byte_target <<= 2;

    SP9500X_RFU_V9_R(0x018d);
    while (SP9500X_RFU_V9_REG(0x018d).byte == 0) {
        SP9500X_RFU_V9_R(0x018d);

//        if (is_timeout(time,timeout)) {
//            Log.set_last_err("DMA:FPGA write timeout.");
//            return -1;
//        }
        INT_CHECK(is_fpga_w_timeout(is_timeout));
        if (is_timeout) {
            Log.set_last_err("DMA:FPGA write timeout.");
            return -1;
        }
    }

    while(1) {
        for (int32_t i = 0;i < 5;++i) {
            SP9500X_RFU_V9_R(0x018d);
            byte_cur[i] = SP9500X_RFU_V9_REG(0x018d).byte;

            if (byte_cur[i] == byte_target) {
                return 0;
            }
            if (i > 0 && (byte_cur[i] == byte_cur[i - 1])) {
                Log.add_msg("DMA:FPGA read waiting... @byte:%d",byte_cur[i]);
                continue;
            }
            if (byte_cur[i] > byte_target) {
                Log.set_last_err("DMA:FPGA read exceed,bytes:%d(need:%d",byte_cur[i],byte_target);
                return -1;
            }
        }

//        if (is_timeout(time,timeout)) {
//            Log.set_last_err("DMA:FPGA read timeout.");
//            return -1;
//        }
        INT_CHECK(is_fpga_w_timeout(is_timeout));
        if (is_timeout) {
            Log.set_last_err("DMA:FPGA write timeout.");
            return -1;
        }
    }
}

int32_t ns_sp9500x::dma_mgr::set_fpga_ddr_addr_r(const uint64_t addr)
{
    SP9500X_RFU_V9_REG_DECL(0x0190);
    SP9500X_RFU_V9_REG_DECL(0x0191);

    SP9500X_RFU_V9_REG(0x0190).addr_low  = addr & 0x00000000ffffffff;
    SP9500X_RFU_V9_REG(0x0191).addr_high = addr >> 32;
    SP9500X_RFU_V9_W(0x0190);
    SP9500X_RFU_V9_W(0x0191);
    return 0;
}

int32_t ns_sp9500x::dma_mgr::get_fpga_ddr_addr_r(uint64_t &addr)
{
    SP9500X_RFU_V9_REG_DECL(0x0190);
    SP9500X_RFU_V9_REG_DECL(0x0191);

    SP9500X_RFU_V9_R(0x0190);
    SP9500X_RFU_V9_R(0x0191);
    addr = (SP9500X_RFU_V9_REG(0x0190).addr_low);
    addr |= uint64_t(SP9500X_RFU_V9_REG(0x0191).addr_high) << 32;
    return 0;
}

int32_t ns_sp9500x::dma_mgr::set_fpga_ddr_addr_w(const uint64_t addr)
{
    SP9500X_RFU_V9_REG_DECL(0x018b);
    SP9500X_RFU_V9_REG_DECL(0x018c);
    SP9500X_RFU_V9_REG_DECL(0x024d);
    SP9500X_RFU_V9_REG_DECL(0x024e);

    SP9500X_RFU_V9_REG(0x018b).addr_low  = addr & 0x00000000ffffffff;
    SP9500X_RFU_V9_REG(0x018c).addr_high = addr >> 32;
    SP9500X_RFU_V9_REG(0x024d).addr_low  = SP9500X_RFU_V9_REG(0x018b).addr_low;
    SP9500X_RFU_V9_REG(0x024e).addr_high = SP9500X_RFU_V9_REG(0x018c).addr_high;
    SP9500X_RFU_V9_W(0x018b);
    SP9500X_RFU_V9_W(0x018c);
    SP9500X_RFU_V9_W(0x024d);
    SP9500X_RFU_V9_W(0x024e);
    return 0;
}

int32_t ns_sp9500x::dma_mgr::get_fpga_ddr_addr_w(uint64_t &addr)
{
    SP9500X_RFU_V9_REG_DECL(0x018b);
    SP9500X_RFU_V9_REG_DECL(0x018c);

    SP9500X_RFU_V9_R(0x018b);
    SP9500X_RFU_V9_R(0x018c);
    addr = (SP9500X_RFU_V9_REG(0x018b).addr_low);
    addr |= uint64_t(SP9500X_RFU_V9_REG(0x018c).addr_high) << 32;
    return 0;
}

int32_t ns_sp9500x::dma_mgr::set_fpga_w_samples(const uint32_t samples) const
{
    SP9500X_RFU_V9_REG_DECL(0x018a);

    SP9500X_RFU_V9_REG(0x018a).length = samples << 2;
    SP9500X_RFU_V9_W(0x018a);
    return 0;
}

int32_t ns_sp9500x::dma_mgr::get_fpga_w_samples(uint32_t &samples) const
{
    SP9500X_RFU_V9_REG_DECL(0x018a);

    SP9500X_RFU_V9_R(0x018a);
    samples = SP9500X_RFU_V9_REG(0x018a).length >> 2;
    return 0;
}

int32_t ns_sp9500x::dma_mgr::set_fpga_w_timeout_time(const uint32_t us) const
{
    SP9500X_RFU_V9_REG_DECL(0x0247);

    SP9500X_RFU_V9_REG(0x0247).time = unsigned(1.0 / 491.52 * us);
    SP9500X_RFU_V9_W(0x0247);
    return 0;
}

int32_t ns_sp9500x::dma_mgr::get_fpga_w_timeout_time(uint32_t &us) const
{
    SP9500X_RFU_V9_REG_DECL(0x0247);

    SP9500X_RFU_V9_R(0x0247);
    us = SP9500X_RFU_V9_REG(0x0247).time * 49152 / 100;
    return 0;
}

int32_t ns_sp9500x::dma_mgr::is_fpga_w_timeout(bool &timeout) const
{
    SP9500X_RFU_V9_REG_DECL(0x0247);

    SP9500X_RFU_V9_R(0x0247);
    timeout = SP9500X_RFU_V9_REG(0x0247).timeout;
    return 0;
}

int32_t ns_sp9500x::dma_mgr::dump(int16_t *I,int16_t *Q) const
{
    uint32_t samples = 0;
    INT_CHECK(get_fpga_w_samples(samples));
    return dump(I,Q,samples);
}

int32_t ns_sp9500x::dma_mgr::dump(int16_t *I,int16_t *Q,const uint32_t samples) const
{
    if (samples == 0 || I == nullptr || Q == nullptr) {
        return 0;
    }

    for (uint32_t i = 0;i < samples;++i) {
        I[i] = int16_t(ul()[i] & 0x0000ffff);
        Q[i] = int16_t(ul()[i] >> 16);
    }

    return 0;
}

int32_t ns_sp9500x::dma_mgr::dump(const char *path) const
{
    uint32_t samples = 0;
    INT_CHECK(get_fpga_w_samples(samples));
    return dump(path,samples);
}

int32_t	ns_sp9500x::dma_mgr::dump(const char *path,const uint32_t samples) const
{
    FILE *fp = fopen(path,"w");

    if (fp == nullptr) {
        Log.set_last_err("Can't open %s",path);
        return -1;
    }

    int16_t I = 0;
    int16_t Q = 0;

    for (uint32_t i = 0;i < samples;i ++) {
        I = int16_t(ul()[i] & 0x0000ffff);
        Q = int16_t(ul()[i] >> 16);
        fprintf(fp,"%6d,%6d\n",I,Q);
    }

    fclose(fp);
    return 0;
}

int32_t ns_sp9500x::dma_mgr::test_case(const uint64_t samples)
{
    SP9500X_RFU_V9_REG_DECL(0x018b);
    SP9500X_RFU_V9_REG_DECL(0x018c);

    SP9500X_RFU_V9_REG_DECL(0x0190);
    SP9500X_RFU_V9_REG_DECL(0x0191);

    const uint32_t samples_dl_max = _memory_dl.block_size(0) / 4;

    uint64_t samples_left = samples;
    uint64_t samples_transing = 0;
    uint64_t samples_transed = 0;

    while (1) {
        // FPGA DMA Read
        samples_left = samples;
        samples_transing = 0;
        samples_transed = 0;

        while (samples_left > 0) {
            samples_transing = samples_left > samples_dl_max ? samples_dl_max : samples_left;

            SP9500X_RFU_V9_REG(0x0190).addr_low  = (samples_transed * 4) & 0x00000000ffffffff;
            SP9500X_RFU_V9_REG(0x0191).addr_high = (samples_transed * 4) >> 32;
            SP9500X_RFU_V9_W(0x0190);
            SP9500X_RFU_V9_W(0x0191);

            for (uint64_t i = 0;i < samples_transing;++i) {
                dl()[i] = (samples_transed + i) & 0x00000000ffffffff;
            }

            INT_CHECK(fpga_r(uint32_t(samples_transing)));

            samples_transed += samples_transing;
            samples_left -= samples_transing;
        }

        // FPGA DMA Write
        samples_left = samples;
        samples_transing = 0;
        samples_transed = 0;

        for (uint64_t i = 0;i < samples;++i) {
            ul()[i] = 0;
        }

        INT_CHECK(set_fpga_w_samples(uint32_t(samples)));
        INT_CHECK(fpga_w());

        for (uint64_t i = 0;i < samples;++i) {
            if (ul()[i] != i) {
                Log.stdprintf("Error:%x(Expect) != %x(Actual)",samples_transed + i,ul()[i]);
                return -1;
            }
        }
    }
}


