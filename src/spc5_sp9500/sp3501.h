#ifndef SP3501_H
#define SP3501_H

#define SP3501 sp3501::instance()

#include "rd.h"
#include "liblog.h"
#include "libbd.h"
//#include <boost/noncopyable.hpp>

namespace sp_rd {

class RD_API sp3501 : boost::noncopyable
{
public:
    struct fan_map_t {
        uint32_t _fpga_addr;
        uint32_t _addr;
        uint32_t _fan_sel;

        fan_map_t(uint32_t fpga_addr,uint32_t addr,uint32_t fan_sel) :
            _fpga_addr(fpga_addr),_addr(addr),_fan_sel(fan_sel) {}
    };
public:
    sp3501();
    ~sp3501();
    static sp3501 &instance();
public:
    int32_t get_s6_ver(uint32_t &ver);
    int32_t open_board();
    int32_t close_board();

    int32_t vol_9119(uint16_t val);
    int32_t vol_9119(double vol);

    int32_t set_fan(uint32_t idx,uint32_t speed);
    int32_t set_fan(uint32_t idx,int32_t speed) { return set_fan(idx,uint32_t(speed)); }

    int32_t set_fan(uint32_t speed);
    int32_t set_fan(int32_t speed) { return set_fan(uint32_t(speed)); }

    int32_t autoFanControlRoutinue(double *RFtemp = nullptr,uint8_t *speed = nullptr);
public:
    vi_pci_dev *m_s6;
};

} //namespace sp_rd

#endif // SP3501_H
