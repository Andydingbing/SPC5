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

#ifndef RD_VI_MEM_IO_H
#define RD_VI_MEM_IO_H

#include "rd.h"
#include "visa.h"

namespace sp_rd {

/*!
 * A physical memory I/O class based on NI-VISA
 */
class RD_API vi_mem_io : public noncopyable
{
public:
    vi_mem_io();
    virtual ~vi_mem_io();
    vi_mem_io & operator = (vi_mem_io &);

public:
    int32_t alloc(uint32_t size);
    int32_t release();
    void releases();

	uint64_t phy_addr();

public:
    virtual int32_t w8(uint8_t *buf, uint32_t size);
    virtual int32_t w16(uint16_t *buf, uint32_t size);
    virtual int32_t w32(uint32_t *buf, uint32_t size);
    virtual int32_t r8(uint8_t *buf, uint32_t size);
    virtual int32_t r16(uint16_t *buf, uint32_t size);
    virtual int32_t r32(uint32_t *buf, uint32_t size);

public:
	void set_intr_entry(ViStatus(*entry)(ViSession session, ViEventType type, ViEvent context, ViAddr user_handle));

private:
    static ViSession g_ddr_session;     // The global session.
    ViBusAddress m_phy_addr;            // The physical address after  align
    ViBusAddress m_phy_addr_bfr_align;  // The physical address before align

private:
    //! The function pointer of the interrupt entry function.
	ViStatus(*m_intr_entry)(ViSession, ViEventType, ViEvent, ViAddr);
};

} // namespace sp_rd

#endif // RD_VI_MEM_IO_H
