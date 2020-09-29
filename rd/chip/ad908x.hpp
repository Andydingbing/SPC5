#ifndef RD_CHIP_AD908X_HPP
#define RD_CHIP_AD908X_HPP

#include "stdint_common.h"
#include <boost/function.hpp>

class ad908x
{
public:
    int32_t set_adc_cddc_page(const uint32_t cddc) const;
    int32_t set_adc_ddc_coarse_nco_ftw(const uint32_t cddc,const double freq) const;
    int32_t get_adc_ddc_coarse_nco_ftw(const uint32_t cddc,double &freq) const;

public:
    int32_t set_dac_duc_nco_ftw(const uint32_t dac,const double freq) const;

public:
    boost::function<int32_t(const uint16_t addr,const uint8_t data)> set_reg;
    boost::function<int32_t(const uint16_t addr,uint8_t &data)> get_reg;
};

#endif // RD_CHIP_AD908X_HPP
