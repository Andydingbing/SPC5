#pragma once
#include "CalBaseThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CCalRxSgpmmThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalRxSgpmmThread)
public:
	typedef struct Result {
		int32_t m_iFreqIdx;
		int32_t m_iPowIdx;
		CSGPMMFile::Point m_Pt;
	}Result;
public:
	CCalRxSgpmmThread();           // 动态创建所使用的受保护的构造函数
	CCalRxSgpmmThread(CWnd *pParent);
	virtual ~CCalRxSgpmmThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


