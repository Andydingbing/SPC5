#ifndef SP9500PRO_SP1403_R1A_H
#define SP9500PRO_SP1403_R1A_H

#include "sp9500x_sp1403_r1a.h"

namespace rd {
namespace ns_sp9500pro {

class RD_API sp1403_r1a : public ns_sp9500x::sp1403_r1a
{
public:
    DECL_SPTR(sp1403_r1a)

    sp1403_r1a(uint32_t rf_idx,uint32_t rfu_idx) :
        rd::sp1403(rf_idx,rfu_idx),
        ns_sp9500x::sp1403_r1a(rf_idx,rfu_idx){ }

    virtual ~sp1403_r1a() {}
};

} // namespace ns_sp9500pro
} // namespace rd

#endif // SP9500PRO_SP1403_R1A_H
