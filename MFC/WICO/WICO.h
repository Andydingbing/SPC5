
// WICO.h : WICO 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#define APPNAME	_T("SPC5 WICO Tool")

#define SAFE_DESTROY_DLG(ptr_dlg)	\
	if (ptr_dlg) {					\
		ptr_dlg->DestroyWindow();	\
		delete ptr_dlg;				\
		ptr_dlg = NULL;				\
	}

#include "resource.h"       // 主符号
#include <GdiPlus.h>
#include "TxDlg.h"
#include "RxDlg.h"

#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;
// CWICOApp:
// 有关此类的实现，请参阅 WICO.cpp
//

class CWICOApp : public CWinApp
{
public:
	CWICOApp();

public:
	ULONG_PTR m_gdiplusToken;
// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CWICOApp theApp;
extern CTxDlg *g_pTxDlg;
extern CRxDlg *g_pRxDlg;
