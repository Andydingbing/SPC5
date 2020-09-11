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

#ifndef RD_MEM_IO_VI_H
#define RD_MEM_IO_VI_H

#include "pci_dev.h"
#include "visa.h"

class RD_API vi_mem_io : public mem_io
{
public:
    vi_mem_io();
    virtual ~vi_mem_io() {}

public:
    int32_t allocate(uint32_t size);
    int32_t release();
    void releases();
	uint64_t phy_addr();

    int32_t w8(uint8_t *buf,uint32_t size);
    int32_t w16(uint16_t *buf,uint32_t size);
    int32_t w32(uint32_t *buf,uint32_t size);
    int32_t r8(uint8_t *buf,uint32_t size);
    int32_t r16(uint16_t *buf,uint32_t size);
    int32_t r32(uint32_t *buf,uint32_t size);

	void set_intr_entry(ViStatus(*entry)(ViSession session, ViEventType type, ViEvent context, ViAddr user_handle));

private:
    static ViSession g_ddr_session;     // Global session
    ViBusAddress m_phy_addr;            // Physical address after  align
    ViBusAddress m_phy_addr_bfr_align;  // Physical address before align

    // Interrupt entry.
    ViStatus(*m_intr_entry)(ViSession,ViEventType,ViEvent,ViAddr);
};

namespace sp_rd {

typedef ::mem_io mem_io;

class RD_API vi_mem_io : public noncopyable
{
public:
    vi_mem_io() { _mem = new ::vi_mem_io; }
    vi_mem_io & operator = (vi_mem_io &mem) { return mem; }
    virtual ~vi_mem_io() { SAFE_DEL(_mem); }

public:
    int32_t alloc(uint32_t size) { return _mem->allocate(size); }
    int32_t release() { return _mem->release(); }
    void releases() { return _mem->releases(); }
    uint64_t phy_addr() { return _mem->phy_addr(); }

    virtual int32_t w8(uint8_t *buf,uint32_t size)   { return _mem->w8(buf,size);  }
    virtual int32_t w16(uint16_t *buf,uint32_t size) { return _mem->w16(buf,size); }
    virtual int32_t w32(uint32_t *buf,uint32_t size) { return _mem->w32(buf,size); }
    virtual int32_t r8(uint8_t *buf,uint32_t size)   { return _mem->r8(buf,size);  }
    virtual int32_t r16(uint16_t *buf,uint32_t size) { return _mem->r16(buf,size); }
    virtual int32_t r32(uint32_t *buf,uint32_t size) { return _mem->r32(buf,size); }

private:
    ::vi_mem_io *_mem;
};

} // namespace sp_rd

#endif // RD_MEM_IO_VI_H
