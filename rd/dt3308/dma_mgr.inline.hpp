#include "../dt3308/dma_mgr.hpp"
#include "reg_def_dt3308_f.h"
#include "liblog.h"
#include "libbd.h"
#include "sleep_common.h"
#include <boost/timer/timer.hpp>

using namespace rd;

ns_dt3308::dma_mgr::dma_mgr(pci_dev *k7) :
    m_k7(k7)
{
    m_dmar = new mem_io_udp();
    for (uint32_t i = 0;i < r_blocks;i ++) {
        m_dmaw[i] = new mem_io_udp();
    }

    m_usr_spc = new uint32_t[r_block_samples];
}

ns_dt3308::dma_mgr::~dma_mgr()
{
    SAFE_DEL(m_usr_spc);
}

uint32_t *ns_dt3308::dma_mgr::usr_spc() const
{
    return m_usr_spc;
}

int32_t ns_dt3308::dma_mgr::init()
{
    INT_CHECK(w_alloc());
    return 0;
}

int32_t ns_dt3308::dma_mgr::fpga_w()
{
    int32_t res = fpga_w_trans();
    INT_CHECK(fpga_w_abort());
    return res;
}

int32_t ns_dt3308::dma_mgr::fpga_w_trans()
{
    KUS_REG_DECLARE(0x80000c);

    KUS_REG_DECLARE(0x800008);

    KUS_REG(0x800008).op = 0;
    KUS_W(0x800008);

    KUS_REG(0x800008).op = 1;
    KUS_W(0x800008);

    KUS_WAIT_IDLE(0x80000c,1,INT_MAX);

    uint32_t addr = 0x200000;
    uint32_t samples = 0;

    get_fpga_w_samples(samples);

    uint32_t total_blocks = samples / r_block_samples;
    uint32_t samples_left = samples;
    uint32_t samples_reading = 0;
    uint32_t samples_last_block = samples % r_block_samples;

    if (samples_last_block != 0) {
        total_blocks ++;
    }

    for (uint32_t i = 0;i < total_blocks;i ++) {
        addr = (0x200000 + i * r_block_samples / 1024) * 4;
        samples_reading = (samples_left > r_block_samples ? r_block_samples : samples_left);

//        Gpib_Read_Dma_Fpga(nullptr,addr,m_dmaw[i]->addr(),samples_reading * 4);

        samples_left -= samples_reading;
    }
    return 0;
}

int32_t ns_dt3308::dma_mgr::fpga_w_abort()
{
    KUS_REG_DECLARE(0x800008);
    KUS_REG(0x800008).op = 0;
    KUS_W(0x800008);
    return 0;
}

int32_t ns_dt3308::dma_mgr::set_fpga_w_samples(uint32_t samples)
{
    KUS_REG_DECLARE(0x800009);

    KUS_REG(0x800009).length = samples * 4;
    KUS_W(0x800009);
    return 0;
}

int32_t ns_dt3308::dma_mgr::get_fpga_w_samples(uint32_t &samples)
{
    KUS_REG_DECLARE(0x800009);

    KUS_R(0x800009);
    samples = KUS_REG(0x800009).length / 4;
    return 0;
}

int32_t ns_dt3308::dma_mgr::iq2buf(int16_t *I,int16_t *Q)
{
    uint32_t samples = 0;
    INT_CHECK(get_fpga_w_samples(samples));
    return iq2buf(I,Q,samples);
}

int32_t ns_dt3308::dma_mgr::iq2buf(int16_t *I,int16_t *Q,uint32_t samples)
{
    if (samples == 0 || I == nullptr || Q == nullptr) {
        return 0;
    }

    uint32_t iq_data = 0;
    for (uint32_t i = 0;i < samples;i ++) {
        iq_data = ((uint32_t *)(m_dmaw[0]->addr()))[i];
        I[i] = iq_data >> 16;
        Q[i] = iq_data & 0x0000ffff;
    }
    return 0;
}

int32_t ns_dt3308::dma_mgr::w_alloc()
{
    for (uint32_t i = 0; i < r_block_samples; i++) {
        m_usr_spc[i] = 0x0;
    }

    for (uint32_t i = 0; i < r_blocks; i++) {
        if (m_dmaw[i]->allocate(r_block_samples * 4) < 0) {
            Log.set_last_err("dma write memory alloc @block%d error", i);
            return -1;
        }
        if (m_dmaw[i]->w32(m_usr_spc, r_block_samples)) {
            Log.set_last_err("dma write memory write default valued @block%d error", i);
            return -1;
        }
    }
    return 0;
}
