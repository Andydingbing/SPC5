
// WICO.h : WICO Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#define APPNAME	_T("SPC5 WICO Tool")

#define SAFE_DESTROY_DLG(ptr_dlg)	\
	if (ptr_dlg) {					\
		ptr_dlg->DestroyWindow();	\
		delete ptr_dlg;				\
		ptr_dlg = NULL;				\
	}

#include "resource.h"       // ������
#include <GdiPlus.h>
#include "TxDlg.h"
#include "RxDlg.h"

#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;
// CWICOApp:
// �йش����ʵ�֣������ WICO.cpp
//

class CWICOApp : public CWinApp
{
public:
	CWICOApp();

public:
	ULONG_PTR m_gdiplusToken;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CWICOApp theApp;
extern CTxDlg *g_pTxDlg;
extern CRxDlg *g_pRxDlg;
