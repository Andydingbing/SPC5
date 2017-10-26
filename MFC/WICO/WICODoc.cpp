
// WICODoc.cpp : CWICODoc 类的实现
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


// CWICODoc 构造/析构

CWICODoc::CWICODoc()
{
	// TODO: 在此添加一次性构造代码

}

CWICODoc::~CWICODoc()
{
}

BOOL CWICODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CWICODoc 序列化

void CWICODoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CWICODoc 诊断

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


// CWICODoc 命令
