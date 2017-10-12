#pragma once
#include "CSETabCtrl.h"
#include "ChartCtrl.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CRxFilterTabCtrl : public CCSETabCtrl
{
	DECLARE_DYNAMIC(CCalR1CRxFilterTabCtrl)

public:
	CCalR1CRxFilterTabCtrl();
	virtual ~CCalR1CRxFilterTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<RxFilter_80M_TableR1C::DataF>  m_Data_80;
	vector<RxFilter_160M_TableR1C::DataF> m_Data_160;
	CChartCtrl *m_pChartRFFr;
	CChartCtrl *m_pChartIFFr;
	CChartLineSerie *m_pLineSerieRFFr;
	CChartLineSerie *m_pLineSerieIFFr;
	CProtectEyeListCtrl *m_pList_80;
	CProtectEyeListCtrl *m_pList_160;
	void AttachChild();
	void ResetShowCtrl();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};