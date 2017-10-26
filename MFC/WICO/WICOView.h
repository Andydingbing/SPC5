#pragma once
#include "MainTabCtrl.h"

class CWICOView : public CView
{
protected: // 仅从序列化创建
	CWICOView();
	DECLARE_DYNCREATE(CWICOView)

// 属性
public:
	CWICODoc* GetDocument() const;

// 操作
public:
	CMainTabCtrl *m_pTabCtrl;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CWICOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // WICOView.cpp 中的调试版本
inline CWICODoc* CWICOView::GetDocument() const
   { return reinterpret_cast<CWICODoc*>(m_pDocument); }
#endif

