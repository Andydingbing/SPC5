#pragma once
#include "TextClrListCtrl.h"

class CHeaderCtrlEx : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderCtrlEx)

public:
	CHeaderCtrlEx();
	virtual ~CHeaderCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes);
};


class CTestListCtrl : public CTextClrListCtrl
{
	DECLARE_DYNAMIC(CTestListCtrl)

public:
	CTestListCtrl();
	virtual ~CTestListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
	CHeaderCtrlEx m_ctrlHeader;
};


