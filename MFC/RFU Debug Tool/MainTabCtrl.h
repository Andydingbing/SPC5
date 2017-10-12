#pragma once
#include "LogDlg.h"
#include "LogViewerDlg.h"

#include "ChartCtrl.h"
#include "ChartLineSerie.h"

#include "RFR1ADlg.h"
#include "RFR1AGuardDlg.h"
#include "RFR1CDlg.h"
#include "RFR1ADDlg.h"
#include "RFR1CDDlg.h"
#include "RFARBDlg.h"
#include "RFIQCaptureDlg.h"

#include "BBDlg.h"
#include "BBArbDlg.h"
#include "BBIQCaptureDlg.h"
#include "DMADlg.h"
#include "FPGADlg.h"

#include "CalR1ATxLOLeakageDlg.h"
#include "CalR1ATxSideBandDlg.h"
#include "CalR1ATxAttDlg.h"
#include "CalR1ATxPowerDlg.h"
#include "CalRxSgpmmDlg.h"
#include "CalR1ARxRefDlg.h"

#include "CalR1CDlg.h"
#include "CalR1CTxLOLeakageDlg.h"
#include "CalR1CTxSidebandDlg.h"
#include "CalR1CTxFilterDlg.h"
#include "CalR1CTxPowerDlg.h"
#include "CalR1CTxAttDlg.h"
#include "CalR1CTxFilOffDlg.h"
#include "CalR1CRxFilterDlg.h"
#include "CalR1CRxRefDlg.h"
#include "CalR1CRxAttDlg.h"
#include "CalR1CRxFilOffDlg.h"

#include "TestR1ASBFSDlg.h"
#include "TestR1ATxFRDlg.h"
#include "TestR1ARxFRDlg.h"
#include "TestR1AX9119Dlg.h"
#include "TestR1ATSDlg.h"

#include "TestR1CTxFRDlg.h"
#include "TestR1CRxFRDlg.h"
#include "TestR1CBTRDlg.h"
#include "TestR1CTTBDlg.h"

class CMainTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMainTabCtrl)

public:
	CMainTabCtrl();
	virtual ~CMainTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CRFR1ADlg		*m_pRFR1ADlg;
	CRFR1AGuardDlg  *m_pRFR1AGuardDlg;
	CRFR1CDlg		*m_pRFR1CDlg;
	CRFR1ADDlg		*m_pRFR1ADDlg;
	CRFR1CDDlg		*m_pRFR1CDDlg;
	CRFArbDlg		*m_pRFArbDlg;
	CRFIQCaptureDlg *m_pRFIQCapDlg;

	CBBDlg			*m_pBBDlg;
	CBBArbDlg		*m_pBBArbDlg;
	CBBIQCaptureDlg *m_pBBIQCapDlg;
	CDMADlg			*m_pDMADlg;
	CFPGADlg		*m_pFPGADlg;

	CCalR1ATxLOLeakageDlg	*m_pCalR1ATxLOLeakageDlg;
	CCalR1ATxSideBandDlg	*m_pCalR1ATxSidebandDlg;
	CCalR1ATxAttDlg			*m_pCalR1ATxAttDlg;
	CCalR1ATxPowerDlg		*m_pCalR1ATxPowerDlg;
	CCalRxSgpmmDlg			*m_pCalR1ARxSgpmmDlg;
	CCalR1ARxRefDlg			*m_pCalR1ARxRefDlg;

	CCalR1CDlg				*m_pCalR1CDlg;
	CCalR1CTxLOLeakageDlg	*m_pCalR1CTxLOLeakageDlg;
	CCalR1CTxSidebandDlg	*m_pCalR1CTxSidebandDlg;
	CCalR1CTxFilterDlg		*m_pCalR1CTxFilterDlg;
	CCalR1CTxPowerDlg		*m_pCalR1CTxPowerDlg;
	CCalR1CTxAttDlg			*m_pCalR1CTxAttDlg;
	CCalR1CTxFilOffDlg		*m_pCalR1CTxFilOffDlg;
	CCalR1CRxFilterDlg		*m_pCalR1CRxFilterDlg;
	CCalR1CRxRefDlg			*m_pCalR1CRxRefDlg;
	CCalR1CRxAttDlg			*m_pCalR1CRxAttDlg;
	CCalR1CRxFilOffDlg		*m_pCalR1CRxFilOffDlg;

	CTestR1ASBFSDlg		*m_pTestR1ASBFSDlg;
	CTestR1ATxFRDlg		*m_pTestR1ATxFRDlg;
	CTestR1ARxFRDlg		*m_pTestR1ARxFRDlg;
	CTestR1AX9119Dlg	*m_pTestR1AX9119Dlg;
	CTestR1ATSDlg		*m_pTestR1ATSDlg;

	CTestR1CTxFRDlg	*m_pTestR1CTxFRDlg;
	CTestR1CRxFRDlg	*m_pTestR1CRxFRDlg;
	CTestR1CBTRDlg	*m_pTestR1CBTRDlg;
	CTestR1CTTBDlg	*m_pTestR1CTTBDlg;

	CSP3301 *m_pSP3301;
	CSP3501 *m_pSP3501;

	void DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CreateAllDlg();
	void UpdateParamInChildDlg();
};