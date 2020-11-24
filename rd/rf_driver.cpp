#include "rf_driver.h"
#include "gen_ini_file.inline.hpp"
#include "algo_math.inline.hpp"
#include "algo_chip_pll.inl.hpp"
#include "memory_physical.inline.hpp"
#include "memory_allocator.inline.hpp"
#include "../dt3308/dma_mgr.inline.hpp"
#include "../sp9500x/dma_mgr.inline.hpp"

static const char *ver = "1.1.15328";

int32_t RF_DriverVersion(const char *Version)
{
    Version = ver;
    return 0;
}
