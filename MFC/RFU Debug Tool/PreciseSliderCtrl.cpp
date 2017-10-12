// PreciseSliderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "PreciseSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPreciseSliderCtrl

IMPLEMENT_DYNAMIC(CPreciseSliderCtrl, CSliderCtrl)

CPreciseSliderCtrl::CPreciseSliderCtrl() : m_bDragging(false)
										 , m_bDragChanged(false)
{

}

CPreciseSliderCtrl::~CPreciseSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CPreciseSliderCtrl, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CPreciseSliderCtrl 消息处理程序



void CPreciseSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDragging = true;
	m_bDragChanged = false;
	SetCapture();
	SetFocus();
	if (SetThumb(point)) {
		m_bDragChanged = true;
		PostMessageToParent(TB_THUMBTRACK);
	}

//	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CPreciseSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bDragging) {
		if (SetThumb(point)) {
			m_bDragChanged = true;
			PostMessageToParent(TB_THUMBTRACK);
		}
	}
	else {
		CSliderCtrl::OnMouseMove(nFlags, point);		
	}
}

void CPreciseSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bDragging) {
		m_bDragging = false;
		::ReleaseCapture();
		if (SetThumb(point)) {
			PostMessageToParent(TB_THUMBTRACK);
			m_bDragChanged = true;
		}
		if (m_bDragChanged) {
			PostMessageToParent(TB_THUMBPOSITION);
			m_bDragChanged = false;
		}
	}
	else {
		CSliderCtrl::OnLButtonUp(nFlags, point);
	}
}

bool CPreciseSliderCtrl::SetThumb(const CPoint& pt)
{
	const int nMin = GetRangeMin();
	const int nMax = GetRangeMax() + 1;
	CRect rc;
	GetChannelRect(rc);
	double dPos;
	double dCorrectionFactor = 0.0;
	if (GetStyle() & TBS_VERT) {
		//note:there is a bug in GetChannelRect,it gets the orientation of the rectangle mixed up
		dPos = (double)(pt.y - rc.left) / (rc.right - rc.left);
	}
	else {
		dPos = (double)(pt.x - rc.left) / (rc.right - rc.left);
	}
	//This correction factor is needed when you click inbetween tick marks
	//so that the thumb will move to the nearest one
	dCorrectionFactor = 0.5 * (1 - dPos) - 0.5 * dPos;
	int nNewPos = (int)(nMin + (nMax - nMin) * dPos + dCorrectionFactor);
	const bool bChanged = (nNewPos != GetPos());
	if (bChanged)
		SetPos(nNewPos);
	return bChanged;
}

void CPreciseSliderCtrl::PostMessageToParent(const int nTBCode) const
{
	CWnd *pWnd = GetParent();
	if (pWnd)
		pWnd->PostMessage(WM_HSCROLL,(WPARAM)((GetPos() << 16) | nTBCode),(LPARAM)GetSafeHwnd());
}