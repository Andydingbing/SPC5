//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef PCI_DEV_H
#define PCI_DEV_H

#include "rd.h"
#include <vector>
#include <string>

using namespace std;

namespace sp_rd {

/*!
 * \brief The pure virtual class of all PCI/PCIe devices,such as vi_pci_dev and wd_pci_dev.
 * \details How to use :
 * 1, call \see get_devs to get all avalible devices
 * 2, call \see init to init the specific device
 * 3, now you can operation I/O from/to the device
 */
class RD_API pci_dev
{
public:
    /*!
     * PCI/PCIe address space.Transformed from NI-VISA
     */
    enum addr_space_t {
		AS_LOCAL = 0,
		AS_A16 = 1,
		AS_A24 = 2,
		AS_A32 = 3,
		AS_A64 = 4,
		AS_ALLOC = 9,
        AS_CFG = 10,    //! PCI/PCIe Config Space
        AS_BAR0 = 11,   //! PCI/PCIe BAR0 Address Space
        AS_BAR1 = 12,   //! PCI/PCIe BAR1 Address Space
        AS_BAR2 = 13,   //! PCI/PCIe BAR2 Address Space
        AS_BAR3 = 14,   //! PCI/PCIe BAR3 Address Space
        AS_BAR4 = 15,   //! PCI/PCIe BAR4 Address Space
        AS_BAR5 = 16,   //! PCI/PCIe BAR5 Address Space
		AS_OPAQUE = 0xFFFF
	};

public:
    pci_dev(void){}
    virtual ~pci_dev(void){}

public:
    /*!
     * \brief Get all PCI/PCIe devices.If linked to NI_VISA,the string format will be "PXI?::0::INSTR".
     * For more detailed information,please refer to "NI_VISA Documentation".
     * \param devs all PCI/PCIe devices reference
     * \return 0 if success,other value if fail
     */
    virtual int32_t get_devs(vector<string> &devs) = 0;

    /*!
     * \brief Initialize the device.
     * \param dev string get from function get_devs
     * \see get_devs
     * \return 0 if success,other value if fail
     */
    virtual int32_t init(const string dev) = 0;

    /*!
     * \brief Initialize the device with VID and DID
     * \param dev string get from function get_devs
     * \see get_devs
     * \param vid device's VID
     * \param did device's DID
     * \return 0 if success,other value if fail
     */
    virtual int32_t init(const string dev, uint32_t vid, uint32_t did) = 0;

    /*!
     * \brief Close the device
     * \return 0 if success,other value if fail
     */
    virtual int32_t close() = 0;

    /*!
     * \brief Get the user defined description of this device,such as "RFU_K7","CXU_S6","CEU_DSP".
     * \return the description
     */
    virtual char *get_des(){return m_des;}

public:
    /*!
     * \brief Write 8 bits to the device.
     * \param addr address space
     * \param offset address offset
     * \param data data to be writted
     * \return 0 if success,other value if fail
     */
    virtual int32_t w8(addr_space_t addr, uint32_t offset, uint8_t data) = 0;

    /*!
     * \brief Write 16 bits to the device.
     * \see w8
     */
    virtual int32_t w16(addr_space_t addr, uint32_t offset, uint16_t data) = 0;

    /*!
     * \brief Write 32 bits to the device.
     * \see w8
     */
    virtual int32_t w32(addr_space_t addr, uint32_t offset, uint32_t data) = 0;

    /*!
     * \brief Write from a local buf.
     * \param addr address space
     * \param offset address offset
     * \param len length of the data(how many 32bits/4bytes)
     * \param buf pointer of the data
     * \return 0 if success,other value if fail
     */
    virtual int32_t w32(addr_space_t addr, uint32_t offset, uint32_t len, uint32_t *buf) = 0;

    /*!
     * \brief Read 8 bits from the device.
     * \param addr address space
     * \param offset address offset
     * \param data data read from the device
     * \return 0 if success,other value if fail
     */
    virtual int32_t r8(addr_space_t addr, uint32_t offset, uint8_t *data) = 0;

    /*!
     * \brief Read 16 bits from the device.
     * \see r8
     */
    virtual int32_t r16(addr_space_t addr, uint32_t offset, uint16_t *data) = 0;

    /*!
     * \brief Read 32 bits from the device.
     * \see r32
     */
    virtual int32_t r32(addr_space_t addr, uint32_t offset, uint32_t *data) = 0;

protected:
    char m_des[16]; //! protected characters of the device's description
};

} //namespace sp_rd

#endif // PCI_DEV_H
