#pragma once
#include "resource.h"
#include "AttachThreaddlg.h"
#include "TextClrListCtrl.h"

//CMFS : Complete Machine Program Bit
class CTestCMPBDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CTestCMPBDlg)

public:
	CTestCMPBDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestCMPBDlg();

// �Ի�������
	enum { IDD = IDD_TEST_CMPB };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_ATTACHTHREAD_DLG()
public:
	UINT m_uiRepeatTimes;
	UINT m_uiOneBlockSize;
	UINT m_uiDelayms;
	CTextClrListCtrl m_DevList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedReScanDevice();
	afx_msg void OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedStart();
	LRESULT ShowData(WPARAM wParam,LPARAM lParam);
};
