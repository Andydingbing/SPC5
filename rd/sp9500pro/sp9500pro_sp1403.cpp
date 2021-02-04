#include "sp9500pro_sp1403.h"
#include "reg_def_sp9500x.h"

using namespace std;
using namespace rd;
using namespace rd::ns_sp1403;
using namespace boost::gregorian;
using namespace boost::filesystem;

ns_sp9500pro::sp1403::~sp1403()
{
    SAFE_DEL(_cal_file);
}

int32_t ns_sp9500pro::sp1403::open_board()
{
    SAFE_NEW(_cal_file,ns_sp9500x::cal_file);

    string sn;
    INT_CHECK(get_sn_major(sn));
    _cal_file->set_name("0001");
    INT_CHECK(_cal_file->open());

    return rd::sp1403::open_board();
}
