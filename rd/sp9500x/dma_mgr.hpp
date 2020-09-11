#ifndef SP9500X_DMA_MGR_H
#define SP9500X_DMA_MGR_H

#include "rd.h"
#include "memory_allocator.hpp"
#include <boost/smart_ptr.hpp>

class pci_dev;

namespace rd { namespace ns_sp9500x {

class RD_API dma_mgr : boost::noncopyable
{
public:
    typedef boost::shared_ptr<dma_mgr> sptr;
    ~dma_mgr();

    void set_ctrller(pci_dev *ctrller) { _v9 = ctrller; }

public:
    const uint32_t *dl() const { return _memory_dl.logic(); }
    const uint32_t *ul() const { return _memory_ul.logic(); }

    uint32_t *dl() { return _memory_dl.logic(); }
    uint32_t *ul() { return _memory_ul.logic(); }

    memory_allocator *dl_memory() { return &_memory_dl; }
    memory_allocator *ul_memory() { return &_memory_ul; }

    int32_t init(const uint32_t dl_samples,const uint32_t ul_samples);

public:
    int32_t fpga_r(const uint32_t samples);
    int32_t fpga_w() const;
    int32_t fpga_w_start() const;
    int32_t fpga_w_trans() const;

    int32_t set_fpga_ddr_addr_r(const uint64_t addr);
    int32_t get_fpga_ddr_addr_r(uint64_t &addr);
    int32_t set_fpga_ddr_addr_w(const uint64_t addr);
    int32_t get_fpga_ddr_addr_w(uint64_t &addr);

    int32_t set_fpga_w_samples(const uint32_t samples) const;
    int32_t get_fpga_w_samples(uint32_t &samples) const;

    int32_t set_fpga_w_timeout_time(const uint32_t us) const;
    int32_t get_fpga_w_timeout_time(uint32_t &us) const;
    int32_t is_fpga_w_timeout(bool &timeout) const;

    int32_t dump(int16_t *I,int16_t *Q) const;
    int32_t dump(int16_t *I,int16_t *Q,const uint32_t samples) const;
    int32_t dump(const char *path) const;
    int32_t dump(const char *path,const uint32_t samples) const;

public:
    int32_t test_case(const uint64_t samples);

private:
    pci_dev *_v9;
    memory_allocator _memory_dl;
    memory_allocator _memory_ul;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // SP9500X_DMA_MGR_H
