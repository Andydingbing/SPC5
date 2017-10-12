#pragma once
#include <windows.h>
#include "MainDlg.h"
#include "LogDlg.h"
#include "InstrumentDlg.h"
#include "LogConfigDlg.h"
#include "ARBIQChartDlg.h"
#include "WaveVisionDlg.h"
#include "X9119Dlg.h"

class CCSEView : public CView
{
protected: // 仅从序列化创建
	CCSEView();
	DECLARE_DYNCREATE(CCSEView)

// 属性
public:
	CCSEDoc* GetDocument() const;

// 操作
public:
	CX9119Dlg m_X9119Dlg;
	CInstrumentDlg m_InstrumentDlg;
	CLogConfigDlg m_LogConfigDlg;
	CArbIQChartDlg m_ArbIQChartDlg;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CCSEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOpenIQChartDlg();
	afx_msg void OnOpenWaveVisionDlg();
	afx_msg void OnStartAllIQCapture();
	afx_msg void OnStopAllIQCapture();
	afx_msg void OnOpenInstrDlg();
	afx_msg void OnOpenLogConfigDlg();
	afx_msg void OnOpenCalc();
	afx_msg void OnOpenNIIOTrace();
	afx_msg void SP3301Init();
	afx_msg void OnDeviceReboot();
	afx_msg void OnDeviceShutdown();
	afx_msg void OnPCIERestart();
	virtual void OnInitialUpdate();
	afx_msg LRESULT SendMessageToIQChartDlg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSelSP33010();
	afx_msg void OnUpdateSP33010(CCmdUI *pCmdUI);
	afx_msg void OnSelSP33011();
	afx_msg void OnUpdateSP33011(CCmdUI *pCmdUI);
	afx_msg void OnSelSP33012();
	afx_msg void OnUpdateSP33012(CCmdUI *pCmdUI);
	afx_msg void OnSelSP33013();
	afx_msg void OnUpdateSP33013(CCmdUI *pCmdUI);
	afx_msg void OnSelSP33014();
	afx_msg void OnUpdateSP33014(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTuneVcxo();
	afx_msg void OnTestAlgorithm5355();
	afx_msg void OnOpenLogViewerDlg();
	afx_msg void OnTestProgrambit();
	afx_msg void OnTestFittingInterp();
};

#ifndef _DEBUG  // CSEView.cpp 中的调试版本
inline CCSEDoc* CCSEView::GetDocument() const
   { return reinterpret_cast<CCSEDoc*>(m_pDocument); }
#endif

