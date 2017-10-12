#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "StretchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CStretchDlg, CDialog)

CStretchDlg::CStretchDlg(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_CtrlInfo.clear();
}

CStretchDlg::~CStretchDlg()
{
}

void CStretchDlg::SetControlInfo(int iCtrlID,SizeType Type,int iCtrlColumn,int iLColumn,int iRColumn,int iCtrlRow,int iTRow,int iBRow)
{
	CtrlInfo Info;
	Info.m_iCtrlID = iCtrlID;
	Info.m_Type = Type;
	Info.m_iCtrlColumn = iCtrlColumn;
	Info.m_iLColumn = iLColumn;
	Info.m_iRColumn = iRColumn;
	Info.m_iCtrlRow = iCtrlRow;
	Info.m_iTRow = iTRow;
	Info.m_iBRow = iBRow;	
	GetDlgItem(iCtrlID)->GetWindowRect(&Info.m_crIniSize);	//for this must be called in OnInitDialog(),so no assert here,users must make sure the ctrl id is right
	ScreenToClient(&Info.m_crIniSize);
	m_CtrlInfo.push_back(Info);
}

void CStretchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStretchDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CStretchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	int iDeltaX = cx - m_crWnd.Width();
	int iDeltaY = cy - m_crWnd.Height();

	CWnd *pWnd;
	CRect crCtrl;
	int iCtrlCnt = (int)(m_CtrlInfo.size());
	int iCtrlIdx = 0;	

	HDWP hdwp = BeginDeferWindowPos(iCtrlCnt);
	for (int i = 0;i < iCtrlCnt;i ++) {
		iCtrlIdx = iCtrlCnt - 1 - i;
		pWnd = GetDlgItem(m_CtrlInfo[iCtrlIdx].m_iCtrlID);
		if (!(::IsWindow(pWnd->GetSafeHwnd())))
			continue;
		crCtrl = m_CtrlInfo[iCtrlIdx].m_crIniSize;
		switch (m_CtrlInfo[iCtrlIdx].m_Type) {
			case UnChange : continue;
			case FixL_xStretch : {
				crCtrl.right += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iRColumn;
				break;
								 }
			case FixL_yStretch : {
				crCtrl.bottom += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iBRow;
				break;
								 }
			case FixL_xyStretch : {
				crCtrl.right += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iRColumn;
				crCtrl.bottom += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iBRow;
				break;
								  }
			case ChgL_Fixx : {
				crCtrl.left += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iLColumn;
				crCtrl.right = crCtrl.left + m_CtrlInfo[iCtrlIdx].m_crIniSize.Width();
				break;
							 }
			case ChgL_xStretch : {
				crCtrl.left += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iLColumn;
				crCtrl.right += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iRColumn;
				break;
								 }
			case ChgL_xyStretch : {
				crCtrl.left += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iLColumn;
				crCtrl.right += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iRColumn;
				crCtrl.bottom += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iBRow;
				break;
								  }
			case ChgT_Fixx : {
				crCtrl.top += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iTRow;
				crCtrl.bottom = crCtrl.top + m_CtrlInfo[iCtrlIdx].m_crIniSize.Height();
				break;
							 }
			case ChgT_xStretch : {
				crCtrl.right += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iRColumn;
				crCtrl.top += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iTRow;
				crCtrl.bottom = crCtrl.top + m_CtrlInfo[iCtrlIdx].m_crIniSize.Height();
				break;
								 }
			case ChgLT_Fixx : {
				crCtrl.left += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iLColumn;
				crCtrl.right = crCtrl.left + m_CtrlInfo[iCtrlIdx].m_crIniSize.Width();
				crCtrl.top += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iTRow;
				crCtrl.bottom = crCtrl.top + m_CtrlInfo[iCtrlIdx].m_crIniSize.Height();
				break;
							  }
			case ChgLT_xStretch : {
				crCtrl.left += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iLColumn;
				crCtrl.right += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iRColumn;
				crCtrl.top += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iTRow;
				crCtrl.bottom = crCtrl.top + m_CtrlInfo[iCtrlIdx].m_crIniSize.Height();
				break;
								  }
			case ChgLT_xyStretch : {
				crCtrl.left += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iLColumn;
				crCtrl.right += iDeltaX / m_CtrlInfo[iCtrlIdx].m_iCtrlColumn * m_CtrlInfo[iCtrlIdx].m_iRColumn;
				crCtrl.top += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iTRow;
				crCtrl.bottom += iDeltaY / m_CtrlInfo[iCtrlIdx].m_iCtrlRow * m_CtrlInfo[iCtrlIdx].m_iBRow;
				break;
								   }
			default : continue;
		}
		DeferWindowPos(hdwp,pWnd->m_hWnd,NULL,crCtrl.left,crCtrl.top,crCtrl.Width(),crCtrl.Height(),SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	EndDeferWindowPos(hdwp);
}

LRESULT CStretchDlg::UpdateDataFalse(WPARAM wParam,LPARAM lParam)
{
	UpdateData(FALSE);
	return 0;
}

LRESULT CStretchDlg::UpdateDataTrue(WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);
	return 0;
}