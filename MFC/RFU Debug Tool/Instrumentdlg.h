#pragma once
#include "afxwin.h"

class CInstrumentDlg : public CDialog
{
	DECLARE_DYNAMIC(CInstrumentDlg)

public:
	CInstrumentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInstrumentDlg();

// 对话框数据
	enum { IDD = IDD_INSTRUMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	BOOL Create();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedInitInstrButton();
	afx_msg void OnBnClickedCloseInstrButton();
	afx_msg void OnBnClickedSaResetButton();
	CString m_SA_CF;
	afx_msg void OnBnClickedSaSetcfButton();
	afx_msg void OnBnClickedSaSetspanButton();
	CString m_SA_SPAN;
	afx_msg void OnBnClickedSaSetrefButton();
	double  m_SA_REF;
	afx_msg void OnBnClickedSaSetrbwButton();
	CString m_SA_RBW;
	CString m_SA_VBW;
	afx_msg void OnBnClickedSaSetvbwButton();
	afx_msg void OnBnClickedSaPeakhighestButton();
	CString m_SA_PEAK_FREQ;
	CString m_SA_PEAK_PWR;
	afx_msg void OnBnClickedSaGetmarkerButton();
	CString m_SA_MARKER_FREQ;
	CString m_SA_MARKER_PWR;
	afx_msg void OnBnClickedSaPeaksearchButton();
	CComboBox m_SA_PEAKSEARCH_TYPE;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSaSetpreampButton();
	CComboBox m_SA_PREAMP;
	afx_msg void OnBnClickedSaSweeponceButton();
	afx_msg void OnBnClickedSaSetaverageButton();
	int m_SA_AVERAGE_COUNT;
	afx_msg void OnBnClickedSaSetmechattButton();
	CString m_SA_MECHATT;
	afx_msg void OnBnClickedSgResetButton();
	afx_msg void OnBnClickedSgRfopButton();
	BOOL m_SG_RFOP_ENABLE;
	BOOL m_SG_MOD_ENABLE;
	afx_msg void OnBnClickedSgModButton();
	afx_msg void OnBnClickedSgSetcwButton();
	CString m_SG_CW;
	afx_msg void OnBnClickedSgSetplevelButton();
	double m_SG_PL;
	afx_msg void OnBnClickedPmResetButton();
	CString m_PM_FREQ;
	afx_msg void OnBnClickedPmSetfreqButton();
	afx_msg void OnBnClickedPmGetpwrButton();
	CString m_PM_PWR;
	afx_msg void OnBnClickedCancel();
protected:
	virtual void OnOK();
};
