#pragma once
#include "Resource.h"
#include "MsgLogListCtrl.h"
#include "RegLogListCtrl.h"
#include "LogProgressCtrl.h"

class CLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogDlg)
public:
	CLogDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogDlg();

// 对话框数据
	enum { IDD = IDD_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMsgLogListCtrl m_MsgList;
	CRegLogListCtrl m_RegList;
	CLogProgressCtrl m_Progress;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void InitProgress(char *pName,int32_t iPts,uint32_t uiThreadID = 0);
	void SetProgress(int32_t iPos,uint32_t uiThreadID);
	afx_msg void OnLvnGetdispinfoMessageList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoRegList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT AddMsgList(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT AddRegList(WPARAM wParam,LPARAM lParam);
};

void InitSinglePos(char *pName,int32_t iPts,uint32_t uiThreadID = 0);

template <class PosType>
inline void SetPos(PosType iPos,uint32_t uiThreadID)
{
	g_pLogDlg->SetProgress((int32_t)iPos,uiThreadID);
}