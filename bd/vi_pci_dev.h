//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef VI_PCI_DEV_H
#define VI_PCI_DEV_H

#include "pci_dev.h"
#include "visa.h"

namespace sp_rd {

/*!
 * An implementation of pci_dev.
 * \see class pci_dev
 */
class RD_API vi_pci_dev : public sp_rd::pci_dev
{
public:
    vi_pci_dev(void);
    vi_pci_dev(const char *des);
    virtual ~vi_pci_dev(void);

public:
    virtual int32_t get_devs(vector<string> &devs);
    virtual int32_t init(const string dev);
    virtual int32_t init(const ViRsrc dev);
    virtual int32_t init(const string dev, uint32_t vid, uint32_t did);
    virtual int32_t init(const ViRsrc dev, uint32_t vid, uint32_t did);
    virtual int32_t close();

    virtual int32_t w8(addr_space_t addr, uint32_t offset, uint8_t data);
    virtual int32_t w16(addr_space_t addr, uint32_t offset, uint16_t data);
    virtual int32_t w32(addr_space_t addr, uint32_t offset, uint32_t data);
    virtual int32_t w32(addr_space_t addr, uint32_t offset, uint32_t len, uint32_t *buf);
    virtual int32_t r8(addr_space_t addr, uint32_t offset, uint8_t *data);
    virtual int32_t r16(addr_space_t addr, uint32_t offset, uint16_t *data);
    virtual int32_t r32(addr_space_t addr, uint32_t offset, uint32_t *data);

    /*!
     * \brief "low level access write 32 bits" to the device.
     * \param offset address offset
     * \param data data to be written
     * \return 0 if success,other value if fail
     */
    virtual int32_t lla_w32(uint32_t offset,uint32_t data);

    /*!
     * \brief "low level access read 32 bits" from the device.
     * \param offset address offset
     * \param data data read from the device
     * \return 0 if success,other value if fail
     */
    virtual int32_t lla_r32(uint32_t offset,uint32_t *data);

public:
    /*!
     * \brief Open default resource manager,specific for NI-VISA.
     * \return 0 if success,other value if fail
     */
    static int32_t open_default_rm();

    /*!
     * \brief Get the default global resouce manager for later usage.
     * \return the default global resouce manager
     */
    static ViSession get_default_rm();

private:
    ViSession m_session;
    ViAddr	  m_map_addr;
};

} //namespace sp_rd

#endif // VI_PCI_DEV_H
