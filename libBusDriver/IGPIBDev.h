#ifndef __VI_GPIBDEV_H__
#define __VI_GPIBDEV_H__

#define BOOL_CHECK(func) if(!(func)) {return false;}
#define BOOL_CHECK_S(func,post) if(!(func)) {post;return false;}

#ifdef __GNUC__
#pragma GCC diagnostic ignored  "-Wunused-parameter"
#pragma GCC diagnostic ignored  "-Wunused-variable"
#endif

#include "stdint_common.h"
#include "visa.h"

class IGPIBDev
{
public:
	IGPIBDev(void);
	virtual ~IGPIBDev(void);
public:
	virtual int32_t GetDefaultPriAddr() = 0;
	virtual int32_t GetDefaultSecAddr() = 0;
	virtual char* GetDes() = 0;
public:
	virtual bool Init(ViRsrc strDev);
	virtual bool Write(const char *format,...);
	virtual bool Read(char *pBuf,uint32_t uiMaxLength,uint32_t uiTo = 3000);
private:
	ViSession m_viSession;
};

#endif
