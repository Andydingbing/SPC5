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

namespace sp_rd {

class RD_API gpib_dev : public noncopyable
{
public:
    gpib_dev();
    virtual ~gpib_dev();

public:
    virtual int32_t get_default_pri_addr() = 0;
    virtual int32_t get_default_sec_addr() = 0;
    virtual std::string get_descriptor() = 0;

public:
    virtual bool init(const std::string &dev);
    virtual bool w(const std::string &scpi) const;
    virtual bool r(std::string &buf, uint32_t length, uint32_t to = 3000) const;

private:
    unsigned long _session;
};

} // namespace sp_rd

#endif // RD_GPIB_DEV_H
