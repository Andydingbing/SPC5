#pragma once

class CTestR1ATSThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1ATSThread)

protected:
	CTestR1ATSThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTestR1ATSThread();
public:
	CTestR1ATSThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<double> m_SGPower;
	virtual int Run();
};


