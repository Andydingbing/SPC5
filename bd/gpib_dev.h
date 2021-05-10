/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RD_GPIB_DEV_H
#define RD_GPIB_DEV_H

#include "rd.h"
#include <string>

class RD_API gpib_dev : public boost::noncopyable
{
public:
    gpib_dev();
    virtual ~gpib_dev() {}

public:
    virtual int32_t get_default_pri_addr() = 0;
    virtual int32_t get_default_sec_addr() = 0;
    virtual std::string get_descriptor() = 0;

public:
    virtual bool init(const std::string &dev);
    virtual bool w(const std::string &scpi) const;
    virtual bool w_block(const std::string &scpi) const;
    virtual bool r(std::string &buf,uint32_t length,uint32_t to = 3000) const;
    virtual bool r_to_bool(bool &result) const;
    virtual bool r_to_uint8(uint8_t &result) const;
    virtual bool r_to_uint16(uint16_t &result) const;
    virtual bool r_to_uint32(uint32_t &result) const;
    virtual bool r_to_float(float &result) const;
    virtual bool r_to_double(double &result) const;

private:
    unsigned long _session;
};

#endif // RD_GPIB_DEV_H
