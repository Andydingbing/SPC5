#ifndef DT3308_DMA_MGR_H
#define DT3308_DMA_MGR_H

#include "rd.h"
#include <boost/smart_ptr.hpp>

class pci_dev;
class mem_io;

namespace rd {

namespace ns_dt3308 {

class RD_API dma_mgr
{
public:
    typedef boost::shared_ptr<dma_mgr> sptr;
    dma_mgr(pci_dev *k7);
    ~dma_mgr();

public:
    uint32_t *usr_spc() const;
    int32_t init();

    int32_t fpga_w();
    int32_t fpga_w_trans();
    int32_t fpga_w_abort();
    int32_t set_fpga_w_samples(uint32_t samples);
    int32_t get_fpga_w_samples(uint32_t &samples);

    int32_t iq2buf(int16_t *I,int16_t *Q);
    int32_t iq2buf(int16_t *I,int16_t *Q,uint32_t samples);

private:
    int32_t w_alloc();

private:
    static const uint32_t r_blocks = 1;
    static const uint32_t r_block_samples = 512 * 1024;

    pci_dev *m_k7;
    mem_io *m_dmar;
    mem_io *m_dmaw[r_blocks];
    uint32_t *m_usr_spc;
};

} // namespace dt3308
} // namespace rd

#endif // DT3308_DMA_MGR_H
