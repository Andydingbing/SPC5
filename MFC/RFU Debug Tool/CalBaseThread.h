#pragma once
#include "CSEWinThread.h"

class CCalBaseThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CCalBaseThread)
public:
	CCalBaseThread();
	virtual ~CCalBaseThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	void GetADS5474(ISP1401 *pSP1401,int64_t &iAD,int32_t iAvgTime = 3);
};


