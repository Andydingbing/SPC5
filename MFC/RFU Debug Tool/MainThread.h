#pragma once
#include "CSEWinThread.h"

class CMainThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CMainThread)

protected:
	CMainThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
public:
	CMainThread(CWnd *pParent,int32_t iFuncIdx);
	virtual ~CMainThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int InitDevice();
};