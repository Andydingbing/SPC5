#ifndef SP9500PRO_SP1403_H
#define SP9500PRO_SP1403_H

#include "sp9500x_sp1403.h"
#include "cal_file_sp9500x.h"
#include "ad908x.hpp"

namespace rd {
namespace ns_sp9500pro {

class RD_API sp1403 : public ns_sp9500x::sp1403
{
public:
    DECL_SPTR(sp1403)

    sp1403() : ns_sp9500x::sp1403() {}
    virtual ~sp1403();

    virtual int32_t open_board() OVERRIDE;
};

} // namespace ns_sp9500pro
} // namespace rd

#endif // SP9500PRO_SP1403_H
