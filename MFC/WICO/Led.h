#pragma once

class CLed : public CWnd
{
	DECLARE_DYNAMIC(CLed)

public:
	CLed(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLed();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	static BOOL RegisterWndClass(HINSTANCE hInstance = NULL);
	afx_msg void OnPaint();
	void SetClr(COLORREF clr);
private:
	COLORREF m_Clr;
};
