#pragma once
#include "CSETabCtrl.h"
#include "ChartCtrl.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CTxFilterTabCtrl : public CCSETabCtrl
{
	DECLARE_DYNAMIC(CCalR1CTxFilterTabCtrl)

public:
	CCalR1CTxFilterTabCtrl();
	virtual ~CCalR1CTxFilterTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<TxFilter_80M_TableR1C::DataF>  m_Data_80;
	vector<TxFilter_160M_TableR1C::DataF> m_Data_160;
	CChartCtrl *m_pChartRFFr;
	CChartCtrl *m_pChartIFFr;
	CChartLineSerie *m_pLineSerieRFFr0;
	CChartLineSerie *m_pLineSerieRFFr1;
	CChartLineSerie *m_pLineSerieIFFr;
	CProtectEyeListCtrl *m_pList_80;
	CProtectEyeListCtrl *m_pList_160;
	void AttachChild();
	void ResetShowCtrl();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};