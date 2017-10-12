#pragma once

class CRFR1AGuardThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CRFR1AGuardThread)
public:
	typedef struct Result {
		int32_t m_iIdx;	//decide what color to show in the title
		COleDateTime m_Time;
		double m_dTemp[MAX_RF * 2];
	public:
		Result();
	}Result;
protected:
	CRFR1AGuardThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CRFR1AGuardThread();
public:
	CRFR1AGuardThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


