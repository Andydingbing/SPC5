#pragma once
#include "MsgLogListCtrl.h"
#include "RegLogListCtrl.h"

class CLogViewerDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogViewerDlg)

public:
	CLogViewerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogViewerDlg();

// �Ի�������
	enum { IDD = IDD_LOG_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnGetdispinfoMessageList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoRegList(NMHDR *pNMHDR, LRESULT *pResult);
	CMsgLogListCtrl m_MsgList;
	CRegLogListCtrl m_RegList;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT AddMsgList(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT AddRegList(WPARAM wParam,LPARAM lParam);
};
