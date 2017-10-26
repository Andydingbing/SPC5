#pragma once
#include <vector>

#define DECLARE_WICO_STRETCH_DLG()					\
	afx_msg void OnSize(UINT nType, int cx, int cy);\
	virtual BOOL PreTranslateMessage(MSG* pMsg);

#define IMPLEMENT_WICO_STRETCH_DLG(class_name)							\
	void class_name::OnSize(UINT nType, int cx, int cy)					\
	{																	\
		CStretchDlg::OnSize(nType, cx, cy);								\
	}																	\
	BOOL class_name::PreTranslateMessage(MSG* pMsg)						\
	{																	\
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	\
			return TRUE;												\
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)	\
			return TRUE;												\
		else															\
			return CDialog::PreTranslateMessage(pMsg);					\
	}

using namespace std;

class CStretchDlg : public CDialog
{
	DECLARE_DYNAMIC(CStretchDlg)
public:
	enum SizeType {
		UnChange = 0,		//保持不变
		FixL_xStretch,		//控件左对齐保持不变，横向拉伸
		FixL_yStretch,		//控件左对齐保持不变，纵向拉伸
		FixL_xyStretch,		//控件左对齐保持不变，横向纵向拉伸
		ChgL_Fixx,			//仅水平移动，不拉伸
		ChgL_xStretch,		//控件水平移动，横向拉伸
		ChgL_xyStretch,		//控件水平移动，横向纵向拉伸
		ChgT_Fixx,			//仅垂直移动，不拉伸
		ChgT_xStretch,		//控件垂直移动，横向拉伸
		ChgLT_Fixx,			//控件水平垂直移动，不拉伸
		ChgLT_xStretch,		//控件水平垂直移动，横向拉伸
		ChgLT_xyStretch		//控件水平垂直移动，横向纵向拉伸
	};
	typedef struct CtrlInfo {
		int m_iCtrlID;		//ID
		int m_iCtrlColumn;	//控件所在行的列数，仅包含需要拉伸的控件的列数
		int m_iLColumn;		//指该控件左边框的左边有多少需要拉伸的控件
		int m_iRColumn;		//指该控件右边框的左边有多少需要拉伸的控件
		int m_iCtrlRow;		//控件所在列的行数，仅包含需要拉伸的控件的列数
		int m_iTRow;		//指该控件顶边框上边有多少需要拉伸的控件
		int m_iBRow;		//指该控件底边框上边有多少需要拉伸的控件
		SizeType m_Type;	//Size Type
		CRect m_crIniSize;	//Initial Size
	}ControlInfo;
public:
	CRect m_crWnd;
	vector<CtrlInfo> m_CtrlInfo;
public:
	CStretchDlg(UINT nIDTemplate,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStretchDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetControlInfo(int iCtrlID,SizeType Type,int iCtrlColumn = 1,int iLColumn = 1,int iRColumn = 1,int iCtrlRow = 1,int iTRow = 1,int iBRow = 1);
	afx_msg LRESULT UpdateDataFalse(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT UpdateDataTrue(WPARAM wParam,LPARAM lParam);
};
