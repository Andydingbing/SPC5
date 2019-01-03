//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef WD_PCI_DEV_H
#define WD_PCI_DEV_H

#include "pci_dev.h"

namespace sp_rd {

class RD_API wd_pci_dev : public sp_rd::pci_dev
{
public:
    wd_pci_dev(void);
    wd_pci_dev(const char *des);
    virtual ~wd_pci_dev(void);

public:
    virtual int32_t get_devs(vector<string> &devs);
    virtual int32_t init(const string dev) { return 0; }
    virtual int32_t init(const string dev, uint32_t vid, uint32_t did) { return 0; }
    virtual int32_t close() { return 0; }

public:
    virtual int32_t w8(addr_space_t addr, uint32_t offset, uint8_t data) { return 0; }
    virtual int32_t w16(addr_space_t addr, uint32_t offset, uint16_t data) { return 0; }
    virtual int32_t w32(addr_space_t addr, uint32_t offset, uint32_t data) { return 0; }
    virtual int32_t w32(addr_space_t addr, uint32_t offset, uint32_t len, uint32_t *buf) { return 0; }
    virtual int32_t r8(addr_space_t addr, uint32_t offset, uint8_t *data) { return 0; }
    virtual int32_t r16(addr_space_t addr, uint32_t offset, uint16_t *data) { return 0; }
    virtual int32_t r32(addr_space_t addr, uint32_t offset, uint32_t *data) { return 0; }
};

} //namespace sp_rd

#endif // WD_PCI_DEV_H
