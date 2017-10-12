#pragma once
#include "AttachThreadDlg.h"

//r1a/b
#define DECLARE_CSE_CAL_R1A_DLG()									\
public:																\
	afx_msg virtual void OnBnClickedStartCal();						\
	afx_msg virtual void OnBnClickedGetAndSet();					\
	afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam);	\
	afx_msg virtual void OnBnClickedExportCalFile();				\
	virtual BOOL PreTranslateMessage(MSG* pMsg);					\
	afx_msg void OnSize(UINT nType, int cx, int cy);				\
	afx_msg virtual void OnEnChangeFreqEdit();						\
	afx_msg virtual LRESULT SP3301Change(WPARAM wParam,LPARAM lParam);

#define IMPLEMENT_CSE_CAL_R1A_DLG(class_name)							\
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
			return CCalR1ABaseDlg::PreTranslateMessage(pMsg);			\
	}																	\
	void class_name::OnEnChangeFreqEdit()								\
	{																	\
		UpdateData(True);												\
		uint32_t uiRfIdx = m_pSP1401->GetRfIdx();						\
		ALL_DLG->m_pCalR1ATxLOLeakageDlg[uiRfIdx].m_strFreq = this->m_strFreq;		\
		ALL_DLG->m_pCalR1ATxSidebandDlg[uiRfIdx].m_strFreq = this->m_strFreq;		\
		ALL_DLG->m_pCalR1ATxAttDlg[uiRfIdx].m_strFreq = this->m_strFreq;			\
		::PostMessage(ALL_DLG->m_pCalR1ATxLOLeakageDlg[uiRfIdx].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);\
		::PostMessage(ALL_DLG->m_pCalR1ATxSidebandDlg[uiRfIdx].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);	\
		::PostMessage(ALL_DLG->m_pCalR1ATxAttDlg[uiRfIdx].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);		\
	}

class CCalR1ABaseDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CCalR1ABaseDlg)
public:
	CCalR1ABaseDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	CCalR1ABaseDlg(UINT nIDTemplate, CString strFreq, CWnd* pParent = NULL);
	virtual ~CCalR1ABaseDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg virtual void OnBnClickedStartCal() = 0;
	afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam) = 0;
	afx_msg virtual void OnBnClickedGetAndSet();
	afx_msg virtual void OnBnClickedExportCalFile();
	afx_msg virtual LRESULT SP3301Change(WPARAM wParam,LPARAM lParam);
	virtual void ResetShowCtrl();
public:
	CSP2401R1A *m_pSP2401;
	CSP1401R1A *m_pSP1401;
	CString m_strFreq;
};

//r1c
#define DECLARE_CSE_CAL_R1C_DLG()									\
public:																\
	afx_msg virtual void OnBnClickedStartCal();						\
	afx_msg virtual void OnBnClickedGetAndSet();					\
	afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam);	\
	afx_msg virtual void OnBnClickedExportCalFile();				\
	virtual BOOL PreTranslateMessage(MSG* pMsg);					\
	afx_msg void OnSize(UINT nType, int cx, int cy);				\
	afx_msg virtual void OnEnChangeFreqEdit();						\
	afx_msg virtual LRESULT SP3301Change(WPARAM wParam,LPARAM lParam);

#define IMPLEMENT_CSE_CAL_R1C_DLG(class_name)							\
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
			return CCalR1CBaseDlg::PreTranslateMessage(pMsg);			\
	}																	\
	void class_name::OnEnChangeFreqEdit()								\
	{																	\
		UpdateData(True);												\
		::SendMessage(GetParent()->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);\
	}

class CCalR1CBaseDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CCalR1CBaseDlg)
public:
	CCalR1CBaseDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	CCalR1CBaseDlg(UINT nIDTemplate, CString strFreq, CWnd* pParent = NULL);
	virtual ~CCalR1CBaseDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg virtual void OnBnClickedStartCal() = 0;
	afx_msg virtual LRESULT ShowData(WPARAM wParam,LPARAM lParam) = 0;
	afx_msg virtual void OnBnClickedGetAndSet();
	afx_msg virtual void OnBnClickedExportCalFile();
	afx_msg virtual LRESULT SP3301Change(WPARAM wParam,LPARAM lParam);
	virtual void ResetShowCtrl();
public:
	CSP2401R1A *m_pSP2401;
	CSP1401R1C *m_pSP1401;
	CString m_strFreq;
};

enum CalIOMode {
	CAL_IO = 0,
	CAL_OP,
	CAL_IO_OP
};

BOOL CalOP(CalIOMode Mode);
BOOL CalIO(CalIOMode Mode);