#pragma once
#include "CalBaseDlg.h"
#include "CalR1CRxFilOffTabCtrl.h"

class CCalR1CRxFilOffDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CRxFilOffDlg)

public:
	CCalR1CRxFilOffDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalR1CRxFilOffDlg();

// �Ի�������
	enum { IDD = IDD_CAL_R1C_RX_FILOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	CCalR1CRxFilOffTabCtrl m_TabCtrl;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	CComboBox m_CalIOMode;
	virtual BOOL OnInitDialog();
	void ResetShowCtrl();
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLvnGetdispinfoListRxR1cFilOffOp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListRxR1cFilOffIo(NMHDR *pNMHDR, LRESULT *pResult);
};
