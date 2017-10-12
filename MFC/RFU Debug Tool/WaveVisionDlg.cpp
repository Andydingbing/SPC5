#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "WaveVisionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CWaveVisionDlg, CDialog)

CWaveVisionDlg::CWaveVisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveVisionDlg::IDD, pParent)
	, m_WaveVision_NumberOfSamples()
	, m_WaveVision_SampleFreq(_T("368.64"))
	, m_WaveVision_IQdatapath(_T(""))
{

}

CWaveVisionDlg::~CWaveVisionDlg()
{
}

void CWaveVisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_WaveVision_IQDataPath);
	DDX_Control(pDX, IDC_EDIT12, m_WaveVision_wvPath);
	DDX_Control(pDX, IDC_COMBO2, m_WaveVision_BITSPERSAMPLE);
	DDX_Text(pDX, IDC_EDIT3, m_WaveVision_NumberOfSamples);
	DDX_Text(pDX, IDC_EDIT4, m_WaveVision_SampleFreq);
	DDX_Text(pDX, IDC_EDIT1, m_WaveVision_IQdatapath);
}


BEGIN_MESSAGE_MAP(CWaveVisionDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CWaveVisionDlg::OnBnClickedSelIQDataPath)
	ON_BN_CLICKED(IDC_BUTTON63, &CWaveVisionDlg::OnBnClickedSelwvPath)
	ON_BN_CLICKED(IDC_BUTTON2, &CWaveVisionDlg::OnBnClickedStartwvTrans)
END_MESSAGE_MAP()


void CWaveVisionDlg::OnBnClickedSelIQDataPath()
{
	UpdateData(TRUE);
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK) {
		m_WaveVision_IQDataPath.SetWindowText(IQFile.GetPathName());
		m_WaveVision_IQdatapath = IQFile.GetPathName();
//		ClipBoard.IQCap_Path = IQFile.GetPathName();
	}
	UpdateData(FALSE);
}

void CWaveVisionDlg::OnBnClickedSelwvPath()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFileDialog wvFile(TRUE);
	if (wvFile.DoModal() == IDOK)
		m_WaveVision_wvPath.SetWindowText(wvFile.GetPathName());
	UpdateData(FALSE);
}

void CWaveVisionDlg::OnBnClickedStartwvTrans()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString buf;
	CString wv;
	FILE *fp_iq;
	FILE *fp_wv;
	uint32_t samples = m_WaveVision_NumberOfSamples;
	uint16_t *data = new uint16_t[samples];
	uint16_t *data_bak = data;

	InitSinglePos("转换成wv格式",samples);
	buf.Format("Generated by:");
	wv += (buf + _T("\r\n"));
	buf.Format("    National Semiconductor's WaveVision 4.0");
	wv += (buf + _T("\r\n"));
	buf.Format("    2900 Semiconductor Dr.");
	wv += (buf + _T("\r\n"));
	buf.Format("    Santa Clara, CA 95051\n");
	wv += (buf + _T("\r\n"));
	buf.Format("Spec: 4.3");
	wv += (buf + _T("\r\n"));
	buf.Format("    Collection method: Sampled");
	wv += (buf + _T("\r\n"));
	buf.Format("    Bits per Sample: %d",(m_WaveVision_BITSPERSAMPLE.GetCurSel() + 8));
	wv += (buf + _T("\r\n"));
	buf.Format("    Number of samples: %d",samples);
	wv += (buf + _T("\r\n"));
	buf.Format("    Sample Delta: %f s",9.522190525426879E-9);
	wv += (buf + _T("\r\n"));
	buf.Format("    Sample Frequency:  %sMHz",m_WaveVision_SampleFreq);
	wv += (buf + _T("\r\n"));

	time_t t;
	tm *timeinfo;
	char time_buf[64];
	time(&t);
	timeinfo = localtime(&t);
	sprintf(time_buf,"%d-%d-%d %d:%d:%d",(timeinfo->tm_year + 1979),(timeinfo->tm_mon + 1),timeinfo->tm_mday,
		timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	buf.Format("Date: %s",time_buf);
	wv += (buf + _T("\r\n"));

	buf.Format("Hardware: WaveVision 4.4 & ADC12C105 Evaluation Board (Rev. 3)");
	wv += (buf + _T("\r\n"));
	buf.Format("Comments: No comments\n");
	wv += (buf + _T("\r\n"));
	
	buf.Format("SINAD: 67.2444153802252");
	wv += (buf + _T("\r\n"));
	buf.Format("SNR: 67.36912298214793");
	wv += (buf + _T("\r\n"));
	buf.Format("THD: -82.72553380786711");
	wv += (buf + _T("\r\n"));
	buf.Format("SFDR: 75.13193847959758");
	wv += (buf + _T("\r\n"));
	buf.Format("ENOB: 10.877809863824785\n");
	wv += (buf + _T("\r\n"));
 
	buf.Format("    Data follows");
	wv += (buf + _T("\r\n"));
	buf.Format("Code");
	wv += (buf + _T("\r\n"));

	CString iqdatapath;
	m_WaveVision_IQDataPath.GetWindowText(iqdatapath);
	fp_iq = fopen((LPSTR)(LPCTSTR)iqdatapath,"r");
	if (fp_iq == NULL) {
		Log->AddMsgList(-1,"文件不存在");
		return;
	}
	for (uint32_t i = 0;i < samples;i ++) {
		fscanf(fp_iq,"%hd",&data[i]);
		buf.Format("%d",data[i]);
		wv += (buf + _T("\r\n"));
		SetPos(i,0);
	}
	fclose(fp_iq);


	buf.Format("End");
	wv += (buf + _T("\r\n"));

	CString wvpath;
	m_WaveVision_wvPath.GetWindowText(wvpath);
	fp_wv = fopen((LPSTR)(LPCTSTR)wvpath,"w");
	if (fp_wv == NULL) {
		Log->AddMsgList(-1,"文件打开错误");
		return;
	}
	fprintf(fp_wv,"%s",wv);
	fclose(fp_wv);

	data_bak = &data[0];
	delete [] data_bak;
	SetPos(samples,0);
	UpdateData(FALSE);
}

BOOL CWaveVisionDlg::Create(UINT nIDTemplate, CWnd* pParentWnd, CString iqPath, UINT samples)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_WaveVision_IQdatapath = iqPath;
	m_WaveVision_NumberOfSamples = samples;

	return CDialog::Create(nIDTemplate, pParentWnd);
}

BOOL CWaveVisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_WaveVision_BITSPERSAMPLE.SetCurSel(8);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
