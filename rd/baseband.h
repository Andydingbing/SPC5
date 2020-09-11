#ifndef BASEBAND_H
#define BASEBAND_H

#include "libbd.h"
#include <list>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>

namespace rd {

class RD_API baseband : boost::noncopyable
{
public:
    typedef boost::shared_ptr<baseband> sptr;

    virtual ~baseband() {}
    virtual bool connect(const std::list<pci_dev *> &ctrller);

    virtual int32_t open_board() { return 0; }
    virtual int32_t close_board() { return 0; }

public:
    virtual int32_t set_da_freq(double) { return 0; }
    virtual int32_t set_ad_freq(double) { return 0; }

    virtual int32_t set_dl_pwr_comp(int32_t) { return 0; }
    virtual int32_t set_dl_pwr_comp(double) { return 0; }

    virtual int32_t set_dl_dc_offset(uint16_t ,uint16_t ) { return 0; }
    virtual int32_t set_dl_phase_adj(double,double) { return 0; }
    virtual int32_t set_duc(const double) { return 0; }

    virtual int32_t set_ul_pwr_comp(int32_t) { return 0; }
    virtual int32_t set_ddc(const double) { return 0; }

private:
    std::list<pci_dev *> _ctrller_pci;
};

} // namespace rd

#endif // BASEBAND_H
