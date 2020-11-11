#include "sp9500x_sp1403.h"
#include "reg_def_sp9500x.h"

using namespace std;
using namespace rd;
using namespace rd::ns_sp1403;
using namespace boost::gregorian;
using namespace boost::filesystem;

ns_sp9500x::sp1403::~sp1403()
{
    SAFE_DEL(_cal_file);
}

int32_t ns_sp9500x::sp1403::open_board()
{
    SAFE_NEW(_cal_file,ns_sp9500x::cal_file);

    return rd::sp1403::open_board();
}

int32_t ns_sp9500x::sp1403::set_ad998x_reg(const uint16_t addr,const uint8_t data)
{
    SP9500X_RFU_V9_REG_DECL(0x0841);

    SP9500X_RFU_V9_REG(0x0841).addr = addr;
    SP9500X_RFU_V9_REG(0x0841).data = data;
    SP9500X_RFU_V9_REG(0x0841).wr = 0;
    SP9500X_RFU_V9_OP(0x0841);
    SP9500X_RFU_V9_WAIT_IDLE(0x0841,0,1000);
    return 0;
}

int32_t ns_sp9500x::sp1403::get_ad998x_reg(const uint16_t addr,uint8_t &data)
{
    SP9500X_RFU_V9_REG_DECL(0x0841);
    SP9500X_RFU_V9_REG_DECL(0x0842);

    data = 0;
    SP9500X_RFU_V9_REG(0x0841).addr = addr;
    SP9500X_RFU_V9_REG(0x0841).wr = 1;
    SP9500X_RFU_V9_OP(0x0841);
    SP9500X_RFU_V9_WAIT_IDLE(0x0841,0,1000);
    SP9500X_RFU_V9_R(0x0842);
    data = SP9500X_RFU_V9_REG(0x0842).data;
    return 0;
}
