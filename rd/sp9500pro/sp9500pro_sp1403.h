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

//    int32_t prepare_cal(const cal_table_t table)
//    { return _cal_file->prepare_cal(table); }

//    int32_t prepare_cal(const cal_table_t table,const std::set<uint64_t> &freqs,const bool exp = false)
//    { return _cal_file->prepare_cal(table,freqs,exp); }

public:
//    virtual int32_t set_tx_state(const port_t port,const data_f_tx_pwr &data) const
//    { ignore_unused(port,data); return 0; }

//    virtual void tx_state(const uint64_t freq,const io_mode_t mode,data_f_tx_pwr &state) const
//    { ignore_unused(freq,mode,state); }

//    virtual void tx_state(const double att,const data_f_tx_pwr &base_state,data_f_tx_pwr &state) const
//    { ignore_unused(att,base_state,state); }

protected:
//    ns_sp9500x::cal_file *_cal_file;
};

} // namespace ns_sp9500pro
} // namespace rd

#endif // SP9500PRO_SP1403_H
