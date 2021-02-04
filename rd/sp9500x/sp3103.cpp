#include "sp3103.h"

using namespace std;
using namespace rd;

sp3103::sp3103(uint32_t rfu_idx):
    _rfu_idx(rfu_idx),
    _v9(nullptr),
    _s6(nullptr),
    _is_program_rfu_v9(false),
    _is_program_bbu_v9(false)
{

}

sp3103::~sp3103()
{

}

void sp3103::freq_to_sub8_rrh(const uint64_t freq,uint64_t &sub8,uint64_t &rrh)
{
    if (freq > FREQ_M(7500)) {
        sub8 = FREQ_M(7500);
        rrh  = freq;
    } else {
        sub8 = freq;
        rrh  = 0;
    }
}
