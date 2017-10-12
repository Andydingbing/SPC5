#pragma once
#include "CalBaseThread.h"

class CCalR1ATxPowerThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1ATxPowerThread)
public:
	typedef struct CalTxPowerResult : public TxPowerTableR1A::DataF {
		int32_t m_iSec;
	};
protected:
	CCalR1ATxPowerThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
public:	
	CCalR1ATxPowerThread(CWnd *pParent);
	virtual ~CCalR1ATxPowerThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxPower();
};

class CExportR1ATxPowerThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1ATxPowerThread)

protected:
	CExportR1ATxPowerThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
public:
	CExportR1ATxPowerThread(CWnd *pParent);
	virtual ~CExportR1ATxPowerThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};