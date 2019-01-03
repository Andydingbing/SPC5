#include "wd_pci_dev.h"
#include <stdio.h>
#include "windrvr.h"
#include "wdc_lib.h"
#include "status_strings.h"
#include "wdc_defs.h"

#define LICENSE "6C3CC2CFE89E7AD042444C655646F21A0BF3FDEB.EMBRACE"

using namespace sp_rd;

wd_pci_dev::wd_pci_dev(void)
{
//	WDC_DriverOpen(WDC_DRV_OPEN_DEFAULT,LICENSE);
}

wd_pci_dev::wd_pci_dev(const char *des)
{
//	memset(m_des,0,sizeof(m_des));
//  strcpy(m_des,des);
//	WDC_DriverOpen(WDC_DRV_OPEN_DEFAULT,LICENSE);
}

wd_pci_dev::~wd_pci_dev(void)
{
}

int32_t wd_pci_dev::get_devs(vector<string> &devs)
{
//    WDC_PCI_SCAN_RESULT scan_result;
    uint32_t status = WD_STATUS_SUCCESS;
//    char buf[256] = {0};
    string dev = "";

//    if ((status = WDC_PciScanDevices(0,0,&scan_result)) != WD_STATUS_SUCCESS)
//        return status;
//    for (uint32_t i = 0;i < scan_result.dwNumDevices;i ++) {
//        memset(buf,0,sizeof(buf));
//        dev.clear();
//        sprintf(buf,"device:%#x,VID:%6#x,DID:%6#x",i,scan_result.deviceId[i].dwVendorId,scan_result.deviceId[i].dwDeviceId);
//        dev = buf;
//        devs.push_back(dev);
//    }
    return status;
}
