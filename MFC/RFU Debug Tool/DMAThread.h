#pragma once

class CDMAThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CDMAThread)

protected:
	CDMAThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


