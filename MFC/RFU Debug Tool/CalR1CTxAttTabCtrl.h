#pragma once
#include "CSETabCtrl.h"
#include "ChartCtrl.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CTxAttTabCtrl : public CCSETabCtrl
{
	DECLARE_DYNAMIC(CCalR1CTxAttTabCtrl)

public:
	CCalR1CTxAttTabCtrl();
	virtual ~CCalR1CTxAttTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<TxAttOPTableR1C::DataF> m_DataOP;
	vector<TxAttIOTableR1C::DataF> m_DataIO;
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