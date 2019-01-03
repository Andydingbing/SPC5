//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef VI_MEM_IO_H
#define VI_MEM_IO_H

#include "rd.h"
#include "visa.h"

namespace sp_rd {

//! A physical memory I/O class based on NI-VISA
class RD_API vi_mem_io
{
public:
    vi_mem_io(void);
    virtual ~vi_mem_io(void);
    vi_mem_io & operator = (vi_mem_io &);
public:
    /*!
     * \brief Alloc local physical memory.
     * \param size size(how many 4bytes)
     * \return 0 if success,other value if fail
     */
    int32_t alloc(uint32_t size);

    /*!
     * \brief Release local physical memory.
     * \return 0 if success,other value if fail
     */
    int32_t release();

    /*!
     * \brief Get the physical address of the beginning of the memory.
     * \return the address
     */
	uint64_t phy_addr();

public:
    /*!
     * \brief Write 8 bits every time to the memory from buf.
     * \param buf pointer of the data
     * \param size size of the data(how many 8bits/1byte)
     * \return 0 if success,other value if fail
     * \see w16
     * \see w32
     */
    virtual int32_t w8(uint8_t *buf, uint32_t size);

    /*!
     * \brief Write 16 bits every time to the memory from buf.
     * \param buf pointer of the data
     * \param size size of the data(how many 16bits/2bytes)
     * \return 0 if success,other value if fail
     * \see w8
     * \see w32
     */
    virtual int32_t w16(uint16_t *buf, uint32_t size);

    /*!
     * \brief Write 32 bits every time to the memory from buf.
     * \param buf pointer of the data
     * \param size size of the data(how many 32bits/4bytes)
     * \return 0 if success,other value if fail
     * \see w8
     * \see w16
     */
    virtual int32_t w32(uint32_t *buf, uint32_t size);

    /*!
     * \brief Read 8 bits every time from the memory.
     * \param buf data read from the memory
     * \param size size of the data(how many 8bits/1byte)
     * \return 0 if success,other value if fail
     * \see r16
     * \see r32
     */
    virtual int32_t r8(uint8_t *buf, uint32_t size);

    /*!
     * \brief Read 16 bits every time from the memory.
     * \param buf data read from the memory
     * \param size size of the data(how many 16bits/2bytes)
     * \return 0 if success,other value if fail
     * \see r8
     * \see r32
     */
    virtual int32_t r16(uint16_t *buf, uint32_t size);

    /*!
     * \brief Read 32 bits every time from the memory.
     * \param buf data read from the memory
     * \param size size of the data(how many 32bits/4bytes)
     * \return 0 if success,other value if fail
     * \see w8
     * \see w16
     */
    virtual int32_t r32(uint32_t *buf, uint32_t size);

public:
    /*!
     * \brief Set the interrupt entry function.
     */
	void set_intr_entry(ViStatus(*entry)(ViSession session, ViEventType type, ViEvent context, ViAddr user_handle));

private:
    static ViSession g_ddr_session;     //! The global session.
    ViBusAddress m_phy_addr;            //! The physical address after  align
    ViBusAddress m_phy_addr_bfr_align;  //! The physical address before align

private:
    //! The function pointer of the interrupt entry function.
	ViStatus(*m_intr_entry)(ViSession, ViEventType, ViEvent, ViAddr);
};

} //namespace sp_rd

#endif // VI_MEM_IO_H
