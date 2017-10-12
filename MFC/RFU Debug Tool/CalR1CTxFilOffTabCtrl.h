#pragma once
#include "CSETabCtrl.h"
#include "ChartCtrl.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CTxFilOffTabCtrl : public CCSETabCtrl
{
	DECLARE_DYNAMIC(CCalR1CTxFilOffTabCtrl)

public:
	CCalR1CTxFilOffTabCtrl();
	virtual ~CCalR1CTxFilOffTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	vector<TxFilterOffTableR1C::DataF> m_DataOP_80;
	vector<TxFilterOffTableR1C::DataF> m_DataIO_80;
	CChartCtrl *m_pChartOP_80;
	CChartCtrl *m_pChartIO_80;
	vector<CChartLineSerie *> m_OPLineSerie_80;
	vector<CChartLineSerie *> m_IOLineSerie_80;
	CProtectEyeListCtrl *m_pListOP_80;
	CProtectEyeListCtrl *m_pListIO_80;
	void AttachChild();
	void ResetShowCtrl(CalIOMode Mode);
	void ClearLineSerieOP();
	void ClearLineSerieIO();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};