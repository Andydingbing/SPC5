#ifndef SP9500X_SP1403_H
#define SP9500X_SP1403_H

#include "sp1403.h"
#include "cal_file_sp9500x.h"
#include "ad908x.hpp"

namespace rd {
namespace ns_sp9500x {

class RD_API sp1403 : virtual public rd::sp1403
{
public:
    typedef boost::shared_ptr<sp1403> sptr;

    sp1403(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~sp1403();

    pci_dev *v9() const { return _v9; }
    ns_sp9500x::cal_file *cal_file() const { return _cal_file; }

    int32_t set_ad998x_reg(const uint16_t addr,const uint8_t data);
    int32_t get_ad998x_reg(const uint16_t addr,uint8_t &data);

protected:
    ns_sp9500x::cal_file *_cal_file;
    pci_dev *_v9;
    ad908x _ad908x;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // SP9500X_SP1403_H
