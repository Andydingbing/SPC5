#pragma once
#include "CalBaseDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CCalR1ATxPowerDlg : public CCalR1ABaseDlg
{
	DECLARE_DYNCREATE(CCalR1ATxPowerDlg)
public:
	CCalR1ATxPowerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1ATxPowerDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1A_TX_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1A_DLG()
public:
	CChartCtrl *m_pChart;
	typedef vector<CChartLineSerie *> LineSerieVector;
	LineSerieVector m_OPLineSerie[4];
	LineSerieVector m_IOLineSerie[4];
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	double m_dDGain;
	int m_iAtt1;
	int m_iAtt2;
	CString m_strPAAtt;
	int m_iAtt3;
	double m_dPower;
	CComboBox m_IOMode;
	CComboBox m_CalIOMode;
	void ResetShowCtrl();
	virtual BOOL OnInitDialog();
};
