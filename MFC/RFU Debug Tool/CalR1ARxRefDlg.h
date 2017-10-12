#pragma once
#include "CalBaseDlg.h"
#include "ProtectEyeListCtrl.h"

class CCalR1ARxRefDlg : public CCalR1ABaseDlg
{
	DECLARE_DYNAMIC(CCalR1ARxRefDlg)
public:
	CCalR1ARxRefDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalR1ARxRefDlg();

// �Ի�������
	enum { IDD = IDD_CAL_R1A_RX_REF };
public:
	vector<RxRefTableR1A::DataF> m_Data;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1A_DLG()
public:
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	virtual BOOL OnInitDialog();
	double m_dRef;
	double m_dPower;
	CString m_strLNAAtt;
	double m_dAtt1;
	int m_iAtt2;
	int m_i0dBFS;
	CComboBox m_IOMode;
	CProtectEyeListCtrl m_ListOP;
	CProtectEyeListCtrl m_ListIO;
	afx_msg void OnLvnGetdispinfoList1RxCalData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList2RxCalData(NMHDR *pNMHDR, LRESULT *pResult);
	void ResetShowCtrl();
	CComboBox m_CalIOMode;
	afx_msg LRESULT Done(WPARAM wParam,LPARAM lParam);
};
