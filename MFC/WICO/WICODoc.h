
// WICODoc.h : CWICODoc ��Ľӿ�
//


#pragma once


class CWICODoc : public CDocument
{
protected: // �������л�����
	CWICODoc();
	DECLARE_DYNCREATE(CWICODoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CWICODoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


