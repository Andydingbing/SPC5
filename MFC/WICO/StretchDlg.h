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
		UnChange = 0,		//���ֲ���
		FixL_xStretch,		//�ؼ�����뱣�ֲ��䣬��������
		FixL_yStretch,		//�ؼ�����뱣�ֲ��䣬��������
		FixL_xyStretch,		//�ؼ�����뱣�ֲ��䣬������������
		ChgL_Fixx,			//��ˮƽ�ƶ���������
		ChgL_xStretch,		//�ؼ�ˮƽ�ƶ�����������
		ChgL_xyStretch,		//�ؼ�ˮƽ�ƶ���������������
		ChgT_Fixx,			//����ֱ�ƶ���������
		ChgT_xStretch,		//�ؼ���ֱ�ƶ�����������
		ChgLT_Fixx,			//�ؼ�ˮƽ��ֱ�ƶ���������
		ChgLT_xStretch,		//�ؼ�ˮƽ��ֱ�ƶ�����������
		ChgLT_xyStretch		//�ؼ�ˮƽ��ֱ�ƶ���������������
	};
	typedef struct CtrlInfo {
		int m_iCtrlID;		//ID
		int m_iCtrlColumn;	//�ؼ������е���������������Ҫ����Ŀؼ�������
		int m_iLColumn;		//ָ�ÿؼ���߿������ж�����Ҫ����Ŀؼ�
		int m_iRColumn;		//ָ�ÿؼ��ұ߿������ж�����Ҫ����Ŀؼ�
		int m_iCtrlRow;		//�ؼ������е���������������Ҫ����Ŀؼ�������
		int m_iTRow;		//ָ�ÿؼ����߿��ϱ��ж�����Ҫ����Ŀؼ�
		int m_iBRow;		//ָ�ÿؼ��ױ߿��ϱ��ж�����Ҫ����Ŀؼ�
		SizeType m_Type;	//Size Type
		CRect m_crIniSize;	//Initial Size
	}ControlInfo;
public:
	CRect m_crWnd;
	vector<CtrlInfo> m_CtrlInfo;
public:
	CStretchDlg(UINT nIDTemplate,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStretchDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetControlInfo(int iCtrlID,SizeType Type,int iCtrlColumn = 1,int iLColumn = 1,int iRColumn = 1,int iCtrlRow = 1,int iTRow = 1,int iBRow = 1);
	afx_msg LRESULT UpdateDataFalse(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT UpdateDataTrue(WPARAM wParam,LPARAM lParam);
};
