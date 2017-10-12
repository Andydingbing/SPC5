#pragma once

class CTestAlgo5355Thread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestAlgo5355Thread)

public:
	typedef struct Result {
		int32_t m_iFreqIdx;
		uint64_t m_uiFreq;
		BOOL m_bResult;
		double m_dDeviation;
		uint32_t m_uiINT;
		uint32_t m_uiFRAC1;
		uint32_t m_uiFRAC2;
		uint32_t m_uiMOD1;
		uint32_t m_uiMOD2;
	public:
		Result();
	}Result;
protected:
	CTestAlgo5355Thread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestAlgo5355Thread();
public:
	CTestAlgo5355Thread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
public:
	int32_t TestVCO5355();
// 	int32_t TestVCO1197();
// 	int32_t TestDDC();
// 	int32_t TestDUC();
};


