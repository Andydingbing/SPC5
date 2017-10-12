#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestAlgo5355Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef int32_t (CTestAlgo5355Thread::*pBeginTestAlgorithm)();

IMPLEMENT_DYNCREATE(CTestAlgo5355Thread, CWinThread)

CTestAlgo5355Thread::Result::Result()
{
	m_iFreqIdx = 0;
	m_uiFreq = 0;
	m_bResult = FALSE;
	m_dDeviation = -100.0;
	m_uiINT = 0;
	m_uiFRAC1 = 0;
	m_uiFRAC2 = 0;
	m_uiMOD1 = 0;
	m_uiMOD2 = 0;
}

CTestAlgo5355Thread::CTestAlgo5355Thread()
{
}

CTestAlgo5355Thread::CTestAlgo5355Thread(CWnd *pParent)
{
	m_pParent = pParent;
	m_iFuncIdx = 0;
}

CTestAlgo5355Thread::~CTestAlgo5355Thread()
{
}

BOOL CTestAlgo5355Thread::InitInstance()
{
	return TRUE;
}

int CTestAlgo5355Thread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTestAlgo5355Thread, CWinThread)
END_MESSAGE_MAP()


int CTestAlgo5355Thread::Run()
{
	pBeginTestAlgorithm tBeginTest[] = {&CTestAlgo5355Thread::TestVCO5355};
	(this->*tBeginTest[m_iFuncIdx])();
	return 0;
}

int32_t CTestAlgo5355Thread::TestVCO5355()
{
#define POST_CLR delete []pFreq;

	CTestAlgo5355Dlg *pParent = (CTestAlgo5355Dlg *)m_pParent;

	uint64_t *pFreq = new uint64_t[SERIE_SIZE(3400000000,6800000000,ADF5355_FREQ_SPACE)];
	CString strFreqStar = pParent->m_strFreqStar;
	CString strFreqStop = pParent->m_strFreqStop;
	CString strFreqStep = pParent->m_strFreqStep;
	int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)pFreq);

	uint32_t uiRefFreq = 100000000;
	uint32_t _INT = 0,_FRAC1 = 0,_FRAC2 = 0,_MOD1 = 0,_MOD2 = 0;
	double dActualFreq = 0.0;

	Result Data;

	InitSinglePos("Testing ADF5355 Frequency Formula",iPts);

	for (int32_t i = 0;i < iPts;i ++) {
		TEST_THREAD_TEST_CANCEL_S(POST_CLR);
		adf5355_freq_formula(pFreq[i],uiRefFreq,false,false,1,ADF5355_FREQ_SPACE,_INT,_FRAC1,_FRAC2,_MOD1,_MOD2);
		adf5355_freq_formula(_INT,_FRAC1,_FRAC2,_MOD1,_MOD2,uiRefFreq,dActualFreq);
		Data.m_iFreqIdx = i;
		Data.m_uiFreq = pFreq[i];
		Data.m_bResult = (dActualFreq == (double)(pFreq[i]) ? TRUE : FALSE);
		Data.m_dDeviation = dActualFreq - (double)(pFreq[i]);
		Data.m_uiINT = _INT;
		Data.m_uiFRAC1 = _FRAC1;
		Data.m_uiFRAC2 = _FRAC2;
		Data.m_uiMOD1 = _MOD1;
		Data.m_uiMOD2 = _MOD2;

		pParent->m_Data.at(Data.m_iFreqIdx) = Data;
		if (FALSE == Data.m_bResult)
			pParent->m_ErrData.push_back(Data);

		if (!(i % 50))
			::PostMessage(pParent->GetSafeHwnd(),WM_CSE_TEST_ALGORITHM_VCO5355_SHOW,0,(LPARAM)(&Data));
		SetPos(i + 1,m_nThreadID);
	}
	::PostMessage(pParent->GetSafeHwnd(),WM_CSE_TEST_ALGORITHM_VCO5355_SHOW,0,(LPARAM)(&Data));
	TEST_THREAD_ABOART_S(POST_CLR);
	return 0;
}