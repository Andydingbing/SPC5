#pragma once

class CDMAThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CDMAThread)

protected:
	CDMAThread();           // 动态创建所使用的受保护的构造函数
public:
	CDMAThread(CWnd *pParent);
	virtual ~CDMAThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
public:
	int32_t TestWR();
};


