#pragma once
#include "afxwin.h"

class CWaveVisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaveVisionDlg)

public:
	CWaveVisionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaveVisionDlg();

// �Ի�������
	enum { IDD = IDD_WAVEVISION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_WaveVision_IQDataPath;
	CEdit m_WaveVision_wvPath;
	afx_msg void OnBnClickedSelIQDataPath();
	afx_msg void OnBnClickedSelwvPath();
	CComboBox m_WaveVision_BITSPERSAMPLE;
	UINT m_WaveVision_NumberOfSamples;
	CString m_WaveVision_SampleFreq;
	afx_msg void OnBnClickedStartwvTrans();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL, CString iqPath = "", UINT samples = 8192);
	CString m_WaveVision_IQdatapath;
	virtual BOOL OnInitDialog();
};
