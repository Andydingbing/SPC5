#pragma once
#include "CSETabCtrl.h"
#include "ChartCtrl.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CRxAttTabCtrl : public CCSETabCtrl
{
	DECLARE_DYNAMIC(CCalR1CRxAttTabCtrl)

public:
	CCalR1CRxAttTabCtrl();
	virtual ~CCalR1CRxAttTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<RxAttOPTableR1C::DataF> m_DataOP;
	vector<RxAttIOTableR1C::DataF> m_DataIO;
	CChartCtrl *m_pChartOP;
	CChartCtrl *m_pChartIO;
	typedef vector<CChartLineSerie *> LineSerieVector;
	vector<LineSerieVector> m_OPLineSerie;
	vector<LineSerieVector> m_IOLineSerie;
	CProtectEyeListCtrl *m_pListOP;
	CProtectEyeListCtrl *m_pListIO;
	void AttachChild();
	void ResetShowCtrl(CalIOMode Mode);
	void ClearLineSerieOP();
	void ClearLineSerieIO();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};