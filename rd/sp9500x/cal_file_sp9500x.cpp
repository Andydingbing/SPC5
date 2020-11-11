#include "cal_file_sp9500x.h"

using namespace std;
using namespace rd;
using namespace rd::ns_sp9500x;

string cal_file::path() const
{
#ifdef RD_PLATFORM_WIN32
    return "C:\\rd\\sp9500x";
#elif defined RD_PLAT_FORM_LINUX
    return "";
#endif
}
