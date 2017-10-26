#pragma once
#include "MainTabCtrl.h"

class CWICOView : public CView
{
protected: // �������л�����
	CWICOView();
	DECLARE_DYNCREATE(CWICOView)

// ����
public:
	CWICODoc* GetDocument() const;

// ����
public:
	CMainTabCtrl *m_pTabCtrl;
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CWICOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // WICOView.cpp �еĵ��԰汾
inline CWICODoc* CWICOView::GetDocument() const
   { return reinterpret_cast<CWICODoc*>(m_pDocument); }
#endif

