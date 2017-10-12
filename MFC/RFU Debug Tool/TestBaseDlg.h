#pragma once
#include "AttachThreadDlg.h"

//r1a/b
#define DECLARE_CSE_TEST_R1A_DLG()									\
public:																\
	afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam);	\
	virtual BOOL PreTranslateMessage(MSG* pMsg);					\
	afx_msg void OnSize(UINT nType, int cx, int cy);

#define IMPLEMENT_CSE_TEST_R1A_DLG(class_name)							\
	void class_name::OnSize(UINT nType, int cx, int cy)					\
	{																	\
		CStretchDlg::OnSize(nType, cx, cy);								\
	}																	\
	BOOL class_name::PreTranslateMessage(MSG* pMsg)						\
	{																	\
		if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)	\
			return TRUE;												\
		if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam)	\
			return TRUE;												\
		else															\
			return CTestR1ABaseDlg::PreTranslateMessage(pMsg);			\
	}

class CTestR1ABaseDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CTestR1ABaseDlg)
public:
	CTestR1ABaseDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1ABaseDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam) = 0;
public:
	CSP2401R1A *m_pSP2401;
	CSP1401R1A *m_pSP1401;
};

//r1c
#define DECLARE_CSE_TEST_R1C_DLG()										\
	public:																\
		afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam);	\
		virtual BOOL PreTranslateMessage(MSG* pMsg);					\
		afx_msg void OnSize(UINT nType, int cx, int cy);

#define IMPLEMENT_CSE_TEST_R1C_DLG(class_name)							\
	void class_name::OnSize(UINT nType, int cx, int cy)					\
	{																	\
		CStretchDlg::OnSize(nType, cx, cy);								\
	}																	\
	BOOL class_name::PreTranslateMessage(MSG* pMsg)						\
	{																	\
		if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)	\
			return TRUE;												\
		if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam)	\
			return TRUE;												\
		else															\
			return CTestR1CBaseDlg::PreTranslateMessage(pMsg);			\
	}

class CTestR1CBaseDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CTestR1CBaseDlg)
public:
	CTestR1CBaseDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1CBaseDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam) = 0;
public:
	CSP2401R1A *m_pSP2401;
	CSP1401R1C *m_pSP1401;
};