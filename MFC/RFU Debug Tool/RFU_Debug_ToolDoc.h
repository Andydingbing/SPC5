// CSEDoc.h : CCSEDoc ��Ľӿ�
//


#pragma once


class CCSEDoc : public CDocument
{
protected: // �������л�����
	CCSEDoc();
	DECLARE_DYNCREATE(CCSEDoc)

// ����
public:

// ����
public:
	bool m_bSP33010Enable;
	bool m_bSP33011Enable;
	bool m_bSP33012Enable;
	bool m_bSP33013Enable;
	bool m_bSP33014Enable;
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CCSEDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


