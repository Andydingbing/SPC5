
// WICODoc.cpp : CWICODoc ���ʵ��
//

#include "stdafx.h"
#include "WICO.h"

#include "WICODoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWICODoc

IMPLEMENT_DYNCREATE(CWICODoc, CDocument)

BEGIN_MESSAGE_MAP(CWICODoc, CDocument)
END_MESSAGE_MAP()


// CWICODoc ����/����

CWICODoc::CWICODoc()
{
	// TODO: �ڴ����һ���Թ������

}

CWICODoc::~CWICODoc()
{
}

BOOL CWICODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CWICODoc ���л�

void CWICODoc::Serialize(CArchive& ar)
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


// CWICODoc ���

#ifdef _DEBUG
void CWICODoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWICODoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWICODoc ����
