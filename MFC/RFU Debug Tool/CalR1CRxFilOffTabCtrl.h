#pragma once
#include "CSETabCtrl.h"
#include "ChartCtrl.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CRxFilOffTabCtrl : public CCSETabCtrl
{
	DECLARE_DYNAMIC(CCalR1CRxFilOffTabCtrl)

public:
	CCalR1CRxFilOffTabCtrl();
	virtual ~CCalR1CRxFilOffTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<RxFilterOffTableR1C::DataF> m_DataOP_80;
	vector<RxFilterOffTableR1C::DataF> m_DataIO_80;
	CChartCtrl *m_pChartOP_80;
	CChartCtrl *m_pChartIO_80;
	vector<CChartLineSerie *> m_OPLineSerie_80[3];
	vector<CChartLineSerie *> m_IOLineSerie_80[3];
	CProtectEyeListCtrl *m_pListOP_80;
	CProtectEyeListCtrl *m_pListIO_80;
	void AttachChild();
	void ResetShowCtrl(CalIOMode Mode);
	void ClearLineSerieOP();
	void ClearLineSerieIO();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};