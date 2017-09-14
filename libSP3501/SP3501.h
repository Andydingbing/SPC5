#ifndef __SP3501_H__
#define __SP3501_H__

#define SP3501 CSP3501::Instance()

#include "stdint.h"
#include "libLog.h"
#include "libBusDriver.h"

class CSP3501
{
public:
	typedef struct BlowerMap {
		int32_t m_iFpgaAddr;
		int32_t m_iAddr;
		int32_t m_iBlowerSel;
	public:
		BlowerMap(int32_t iFpgaAddr,int32_t iAddr,int32_t iBlowerSel) {
			m_iFpgaAddr = iFpgaAddr;
			m_iAddr = iAddr;
			m_iBlowerSel = iBlowerSel;	
		}
	}BlowerMap;
public:
	CSP3501();
	~CSP3501();
	static CSP3501 &Instance();
public:
	int32_t Get_S6_Version(uint32_t &S6Ver);
	int32_t OpenBoard();
	int32_t CloseBoard();

	int32_t Vol9119(int16_t Val);
	int32_t Vol9119(double Vol);
	int32_t SetBlower(int32_t iBlower,int32_t iSpeed);
	int32_t SetBlower(int32_t iSpeed);
public:
    viPCIDev *m_pS6;
};

#endif
