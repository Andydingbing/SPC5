#pragma once
#include "CalR1CTxLOLeakageThread.h"
#include "CalR1CTxSideBandThread.h"
#include "CalR1CTxFilterThread.h"
#include "CalR1CTxPowerThread.h"
#include "CalR1CTxAttThread.h"
#include "CalR1CTxFilOffThread.h"
#include "CalR1CRxFilterThread.h"
#include "CalR1CRxRefThread.h"
#include "CalR1CRxAttThread.h"
#include "CalR1CRxFilOffThread.h"

class CCalR1CThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CThread)
public:
	CCalR1CThread();
	CCalR1CThread(CWnd *pParent);
	virtual ~CCalR1CThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<CCalBaseThread *> m_Thread;
	virtual int Run();
};


