// CSEDoc.cpp : CCSEDoc ���ʵ��
//

#include "stdafx.h"
#include "RFU_Debug_Tool.h"

#include "RFU_Debug_ToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCSEDoc

IMPLEMENT_DYNCREATE(CCSEDoc, CDocument)

BEGIN_MESSAGE_MAP(CCSEDoc, CDocument)
END_MESSAGE_MAP()


// CCSEDoc ����/����

CCSEDoc::CCSEDoc():m_bSP33010Enable(true)
				  ,m_bSP33011Enable(false)
				  ,m_bSP33012Enable(false)
				  ,m_bSP33013Enable(false)
				  ,m_bSP33014Enable(false)
{
	// TODO: �ڴ����һ���Թ������

}

CCSEDoc::~CCSEDoc()
{
}

BOOL CCSEDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	CDocument::SetTitle(_T("CSE"));
	return TRUE;
}




// CCSEDoc ���л�

void CCSEDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CCSEDoc ���

#ifdef _DEBUG
void CCSEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCSEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCSEDoc ����
