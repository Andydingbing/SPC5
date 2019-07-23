#ifndef SP3501_H
#define SP3501_H

#define SP3501 sp3501::instance()

#include "rd.h"
#include "liblog.h"
#include "libbd.h"

namespace sp_rd {

class RD_API sp3501
{
public:
    typedef struct blower_map_t {
        int32_t fpga_addr;
        int32_t addr;
        int32_t blower_sel;
	public:
        blower_map_t(int32_t fpga_addr,int32_t addr,int32_t blower_sel) {
            this->fpga_addr = fpga_addr;
            this->addr = addr;
            this->blower_sel = blower_sel;
		}
    }blower_map_t;
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
    int32_t set_blower(int32_t idx,int32_t speed);
    int32_t set_blower(int32_t speed);
    int32_t autoFanControlRoutinue(double *RFtemp = nullptr, uint8_t *speed = nullptr);
public:
    vi_pci_dev *m_s6;
};

} //namespace sp_rd

#endif // SP3501_H
