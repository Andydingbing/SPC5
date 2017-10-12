#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "HMC1197Dlg.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define READ_REG(addr)												\
	uint32_t uiRegData = 0;											\
	m_pSP1401->GetTxLO1Reg(addr,uiRegData);							\
	m_strReg##addr##Read.Format("%#08x d%d",uiRegData,uiRegData);	\
	UpdateData(FALSE);

#define WRITE_REG(addr)												\
	UpdateData(TRUE);												\
	uint32_t uiRegData = strtoul(m_strReg##addr##Write,NULL,16);	\
	m_pSP1401->SetTxLO1Reg(uiRegData);

IMPLEMENT_DYNAMIC(CHMC1197Dlg, CDialog)

CHMC1197Dlg::CHMC1197Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHMC1197Dlg::IDD, pParent)
	, m_strReg0x00Read(_T(""))
	, m_sSoftReset(0)
	, m_bSPIChipEnable(TRUE)
	, m_bKeepBiasOn(FALSE)
	, m_bKeepPFDOn(FALSE)
	, m_bKeepCPOn(FALSE)
	, m_bKeepRefBufferOn(FALSE)
	, m_bKeepVCOOn(FALSE)
	, m_bKeepGPODriverOn(FALSE)
	, m_strReg0x01Read(_T(""))
	, m_iRDiv(1)
	, m_iINT(39)
	, m_iFRAC(671089)
	, m_iReg0x05Write(0)
	, m_bSyncSPIMode(FALSE)
	, m_bExactFreqMode(TRUE)
	, m_bBypassDSM(FALSE)
	, m_bAutoseedDSM(TRUE)
	, m_bDSMEnable(TRUE)
	, m_strReg0x06Read(_T(""))
	, m_bLDEnable(TRUE)
	, m_sLDTraining(0)
	, m_bCSPEnable(TRUE)
	, m_strReg0x07Read(_T(""))
	, m_bGPOLDOSDOEnable(TRUE)
	, m_bVCOPsBiasEnable(TRUE)
	, m_bHighRef(FALSE)
	, m_strReg0x08Read(_T(""))
	, m_bOffsetUpEnable(TRUE)
	, m_bOffsetDownEnable(TRUE)
	, m_bHikCPMode(FALSE)
	, m_strReg0x09Read(_T(""))
	, m_bAutoCalDisable(FALSE)
	, m_bAutoRelockOnce(FALSE)
	, m_strReg0x0ARead(_T(""))
	, m_bPDUpOutput(TRUE)
	, m_bPDDownOutput(TRUE)
	, m_bForceCPUp(FALSE)
	, m_bForceCPDown(FALSE)
	, m_bForceCPMidRail(FALSE)
	, m_strReg0x0BRead(_T(""))
	, m_iChannelsPerFPD(25)
	, m_bGPOTestData(FALSE)
	, m_bPreventAutomuxSDO(FALSE)
	, m_bDisablePFET(FALSE)
	, m_bDisableNFET(FALSE)
	, m_strReg0x0FRead(_T(""))
	, m_iVCOTuneCurve(16)
	, m_bVCOTuningBusy(FALSE)
	, m_strReg0x10Read(_T(""))
	, m_iSARErrorMagnitude(524287)
	, m_bSARError(FALSE)
	, m_bGPOOut(FALSE)
	, m_bLockDetectOut(FALSE)
	, m_strReg0x13Read(_T(""))
	, m_sAuxGPOValues(0)
	, m_bPhaseSync(TRUE)
	, m_strReg0x14Read(_T(""))
	, m_bVCOManualCal(FALSE)
	, m_iCapacitorSwitchSet(16)
	, m_bManualVCOTune(FALSE)
	, m_bAutoScaleCPCurrent(TRUE)
	, m_strReg0x15Read(_T(""))
	, m_strReg0x16Read(_T(""))
	, m_bVCOSubsysMasterEnable(TRUE)
	, m_bVCOEnable(TRUE)
	, m_bExternalVCOBufferEnable(FALSE)
	, m_bPLLBufferEnable(TRUE)
	, m_bLO1OutputBufferEnable(FALSE)
	, m_bLO2OutputBufferEnable(TRUE)
	, m_bExternalVCOInputEnable(FALSE)
	, m_bPreLockMute(FALSE)
	, m_strReg0x17Read(_T(""))
	, m_iReg0x19Write(0xaaa)
	, m_iReg0x1AWrite(0xb29d0b)
	, m_strFreqRF(_T("976M"))
	, m_strFreqVCO(_T("3904M"))
	, m_dFreqError(0.0)
	, m_strReg0x02Read(_T(""))
	, m_strReg0x03Read(_T(""))
	, m_strReg0x04Read(_T(""))
	, m_strReg0x05Read(_T(""))
	, m_strReg0x0CRead(_T(""))
	, m_strReg0x11Read(_T(""))
	, m_strReg0x12Read(_T(""))
	, m_strReg0x18Read(_T(""))
	, m_strReg0x19Read(_T(""))
	, m_strReg0x1ARead(_T(""))
	, m_strReg0x00Write(_T(""))
	, m_strReg0x01Write(_T(""))
	, m_strReg0x02Write(_T(""))
	, m_strReg0x03Write(_T(""))
	, m_strReg0x04Write(_T(""))
	, m_strReg0x05Write(_T(""))
	, m_strReg0x06Write(_T(""))
	, m_strReg0x07Write(_T(""))
	, m_strReg0x08Write(_T(""))
	, m_strReg0x09Write(_T(""))
	, m_strReg0x0aWrite(_T(""))
	, m_strReg0x0bWrite(_T(""))
	, m_strReg0x0cWrite(_T(""))
	, m_strReg0x0dWrite(_T(""))
	, m_strReg0x0eWrite(_T(""))
	, m_strReg0x0fWrite(_T(""))
	, m_strReg0x10Write(_T(""))
	, m_strReg0x11Write(_T(""))
	, m_strReg0x12Write(_T(""))
	, m_strReg0x13Write(_T(""))
	, m_strReg0x14Write(_T(""))
	, m_strReg0x15Write(_T(""))
	, m_strReg0x16Write(_T(""))
	, m_strReg0x17Write(_T(""))
	, m_strReg0x18Write(_T(""))
	, m_strReg0x19Write(_T(""))
	, m_strReg0x1aWrite(_T(""))
	, m_strReg0x1bWrite(_T(""))
	, m_strReg0x1cWrite(_T(""))
	, m_strReg0x1dWrite(_T(""))
	, m_strReg0x1eWrite(_T(""))
	, m_strReg0x1fWrite(_T(""))
{
	m_pSP1401 = NULL;
}

CHMC1197Dlg::~CHMC1197Dlg()
{
}

void CHMC1197Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_REG0x00, m_XGroupBoxReg0x00);
	DDX_Control(pDX, IDC_STATIC_REG0x01, m_XGroupBoxReg0x01);
	DDX_Control(pDX, IDC_STATIC_REG0x02, m_XGroupBoxReg0x02);
	DDX_Control(pDX, IDC_STATIC_REG0x03, m_XGroupBoxReg0x03);
	DDX_Control(pDX, IDC_STATIC_REG0x04, m_XGroupBoxReg0x04);
	DDX_Control(pDX, IDC_STATIC_REG0x05, m_XGroupBoxReg0x05);
	DDX_Control(pDX, IDC_STATIC_REG0x06, m_XGroupBoxReg0x06);
	DDX_Control(pDX, IDC_STATIC_REG0x07, m_XGroupBoxReg0x07);
	DDX_Control(pDX, IDC_STATIC_REG0x08, m_XGroupBoxReg0x08);
	DDX_Control(pDX, IDC_STATIC_REG0x09, m_XGroupBoxReg0x09);
	DDX_Control(pDX, IDC_STATIC_REG0x0A, m_XGroupBoxReg0x0A);
	DDX_Control(pDX, IDC_STATIC_REG0x0B, m_XGroupBoxReg0x0B);
	DDX_Control(pDX, IDC_STATIC_REG0x0C, m_XGroupBoxReg0x0C);
	DDX_Control(pDX, IDC_STATIC_REG0x0F, m_XGroupBoxReg0x0F);
	DDX_Control(pDX, IDC_STATIC_REG0x10, m_XGroupBoxReg0x10);
	DDX_Control(pDX, IDC_STATIC_REG0x11, m_XGroupBoxReg0x11);
	DDX_Control(pDX, IDC_STATIC_REG0x12, m_XGroupBoxReg0x12);
	DDX_Control(pDX, IDC_STATIC_REG0x13, m_XGroupBoxReg0x13);
	DDX_Control(pDX, IDC_STATIC_REG0x14, m_XGroupBoxReg0x14);
	DDX_Control(pDX, IDC_STATIC_REG0x15, m_XGroupBoxReg0x15);
	DDX_Control(pDX, IDC_STATIC_REG0x16, m_XGroupBoxReg0x16);
	DDX_Control(pDX, IDC_STATIC_REG0x17, m_XGroupBoxReg0x17);
	DDX_Control(pDX, IDC_STATIC_REG0x18, m_XGroupBoxReg0x18);
	DDX_Control(pDX, IDC_STATIC_REG0x19, m_XGroupBoxReg0x19);
	DDX_Control(pDX, IDC_STATIC_REG0x1A, m_XGroupBoxReg0x1A);
	DDX_Control(pDX, IDC_STATIC_REG0x21, m_XGroupBoxReg0x21);
	DDX_Control(pDX, IDC_STATIC_QUICK_SET, m_XGroupBoxQuickSet);
	DDX_Text(pDX, IDC_EDIT1, m_strReg0x00Read);
	DDX_Text(pDX, IDC_EDIT11, m_sSoftReset);
	DDX_Control(pDX, IDC_COMBO2, m_ChipEnablePin);
	DDX_Check(pDX, IDC_CHECK7, m_bSPIChipEnable);
	DDX_Check(pDX, IDC_CHECK8, m_bKeepBiasOn);
	DDX_Check(pDX, IDC_CHECK9, m_bKeepPFDOn);
	DDX_Check(pDX, IDC_CHECK10, m_bKeepCPOn);
	DDX_Check(pDX, IDC_CHECK34, m_bKeepRefBufferOn);
	DDX_Check(pDX, IDC_CHECK35, m_bKeepVCOOn);
	DDX_Check(pDX, IDC_CHECK36, m_bKeepGPODriverOn);
	DDX_Text(pDX, IDC_EDIT26, m_strReg0x01Read);
	DDX_Text(pDX, IDC_EDIT12, m_iRDiv);
	DDX_Text(pDX, IDC_EDIT90, m_iINT);
	DDX_Text(pDX, IDC_EDIT91, m_iFRAC);
	DDX_Text(pDX, IDC_EDIT92, m_iReg0x05Write);
	DDX_Control(pDX, IDC_COMBO3, m_DSMType);
	DDX_Check(pDX, IDC_CHECK37, m_bSyncSPIMode);
	DDX_Check(pDX, IDC_CHECK38, m_bExactFreqMode);
	DDX_Check(pDX, IDC_CHECK39, m_bBypassDSM);
	DDX_Check(pDX, IDC_CHECK40, m_bAutoseedDSM);
	DDX_Check(pDX, IDC_CHECK41, m_bDSMEnable);
	DDX_Text(pDX, IDC_EDIT99, m_strReg0x06Read);
	DDX_Control(pDX, IDC_COMBO6, m_LDCounts);
	DDX_Check(pDX, IDC_CHECK42, m_bLDEnable);
	DDX_Text(pDX, IDC_EDIT93, m_sLDTraining);
	DDX_Check(pDX, IDC_CHECK43, m_bCSPEnable);
	DDX_Text(pDX, IDC_EDIT100, m_strReg0x07Read);
	DDX_Check(pDX, IDC_CHECK44, m_bGPOLDOSDOEnable);
	DDX_Check(pDX, IDC_CHECK45, m_bVCOPsBiasEnable);
	DDX_Check(pDX, IDC_CHECK46, m_bHighRef);
	DDX_Control(pDX, IDC_COMBO31, m_SDOOutputLevel);
	DDX_Text(pDX, IDC_EDIT101, m_strReg0x08Read);
	DDX_Control(pDX, IDC_COMBO32, m_CPDownGain);
	DDX_Control(pDX, IDC_COMBO39, m_CPUpGain);
	DDX_Control(pDX, IDC_COMBO40, m_CPOffset);
	DDX_Check(pDX, IDC_CHECK47, m_bOffsetUpEnable);
	DDX_Check(pDX, IDC_CHECK48, m_bOffsetDownEnable);
	DDX_Check(pDX, IDC_CHECK49, m_bHikCPMode);
	DDX_Text(pDX, IDC_EDIT102, m_strReg0x09Read);
	DDX_Control(pDX, IDC_COMBO41, m_VtuneResolution);
	DDX_Check(pDX, IDC_CHECK50, m_bAutoCalDisable);
	DDX_Control(pDX, IDC_COMBO42, m_FSMVSPIClock);
	DDX_Check(pDX, IDC_CHECK51, m_bAutoRelockOnce);
	DDX_Text(pDX, IDC_EDIT103, m_strReg0x0ARead);
	DDX_Control(pDX, IDC_COMBO43, m_PDPhaseSelect);
	DDX_Check(pDX, IDC_CHECK52, m_bPDUpOutput);
	DDX_Check(pDX, IDC_CHECK53, m_bPDDownOutput);
	DDX_Check(pDX, IDC_CHECK54, m_bForceCPUp);
	DDX_Check(pDX, IDC_CHECK55, m_bForceCPDown);
	DDX_Check(pDX, IDC_CHECK56, m_bForceCPMidRail);
	DDX_Text(pDX, IDC_EDIT104, m_strReg0x0BRead);
	DDX_Text(pDX, IDC_EDIT94, m_iChannelsPerFPD);
	DDX_Control(pDX, IDC_COMBO44, m_GPOSel);
	DDX_Check(pDX, IDC_CHECK57, m_bGPOTestData);
	DDX_Check(pDX, IDC_CHECK58, m_bPreventAutomuxSDO);
	DDX_Check(pDX, IDC_CHECK59, m_bDisablePFET);
	DDX_Check(pDX, IDC_CHECK60, m_bDisableNFET);
	DDX_Text(pDX, IDC_EDIT105, m_strReg0x0FRead);
	DDX_Text(pDX, IDC_EDIT68, m_iVCOTuneCurve);
	DDX_Check(pDX, IDC_CHECK61, m_bVCOTuningBusy);
	DDX_Text(pDX, IDC_EDIT106, m_strReg0x10Read);
	DDX_Text(pDX, IDC_EDIT95, m_iSARErrorMagnitude);
	DDX_Check(pDX, IDC_CHECK62, m_bSARError);
	DDX_Check(pDX, IDC_CHECK63, m_bGPOOut);
	DDX_Check(pDX, IDC_CHECK64, m_bLockDetectOut);
	DDX_Text(pDX, IDC_EDIT107, m_strReg0x13Read);
	DDX_Control(pDX, IDC_COMBO45, m_AuxSPIMode);
	DDX_Text(pDX, IDC_EDIT96, m_sAuxGPOValues);
	DDX_Control(pDX, IDC_COMBO46, m_AuxGPOOutputLevel);
	DDX_Check(pDX, IDC_CHECK65, m_bPhaseSync);
	DDX_Control(pDX, IDC_COMBO47, m_AuxSPIGPOOutput);
	DDX_Control(pDX, IDC_COMBO48, m_AuxSPIOutputs);
	DDX_Text(pDX, IDC_EDIT108, m_strReg0x14Read);
	DDX_Check(pDX, IDC_CHECK66, m_bVCOManualCal);
	DDX_Text(pDX, IDC_EDIT97, m_iCapacitorSwitchSet);
	DDX_Control(pDX, IDC_COMBO49, m_VCOBand);
	DDX_Check(pDX, IDC_CHECK67, m_bManualVCOTune);
	DDX_Check(pDX, IDC_CHECK68, m_bAutoScaleCPCurrent);
	DDX_Text(pDX, IDC_EDIT109, m_strReg0x15Read);
	DDX_Control(pDX, IDC_COMBO50, m_RFDividerRatio);
	DDX_Control(pDX, IDC_COMBO51, m_LO1OutputGain);
	DDX_Control(pDX, IDC_COMBO52, m_LO2OutputGain);
	DDX_Control(pDX, IDC_COMBO53, m_DividerOutputGain);
	DDX_Text(pDX, IDC_EDIT110, m_strReg0x16Read);
	DDX_Check(pDX, IDC_CHECK69, m_bVCOSubsysMasterEnable);
	DDX_Check(pDX, IDC_CHECK70, m_bVCOEnable);
	DDX_Check(pDX, IDC_CHECK71, m_bExternalVCOBufferEnable);
	DDX_Check(pDX, IDC_CHECK72, m_bPLLBufferEnable);
	DDX_Check(pDX, IDC_CHECK73, m_bLO1OutputBufferEnable);
	DDX_Check(pDX, IDC_CHECK74, m_bLO2OutputBufferEnable);
	DDX_Check(pDX, IDC_CHECK75, m_bExternalVCOInputEnable);
	DDX_Check(pDX, IDC_CHECK76, m_bPreLockMute);
	DDX_Control(pDX, IDC_COMBO54, m_LO1OutputMode);
	DDX_Control(pDX, IDC_COMBO55, m_LO2OutputMode);
	DDX_Control(pDX, IDC_COMBO56, m_CPOutputSelect);
	DDX_Text(pDX, IDC_EDIT111, m_strReg0x17Read);
	DDX_Control(pDX, IDC_COMBO57, m_ExternalVCOBias);
	DDX_Text(pDX, IDC_EDIT112, m_iReg0x19Write);
	DDX_Text(pDX, IDC_EDIT98, m_iReg0x1AWrite);
	DDX_Control(pDX, IDC_COMBO58, m_HarmonicLPFBand);
	DDX_Control(pDX, IDC_COMBO66, m_Mode);
	DDX_Text(pDX, IDC_EDIT137, m_strFreqRF);
	DDX_Text(pDX, IDC_EDIT138, m_strFreqVCO);
	DDX_Text(pDX, IDC_EDIT139, m_dFreqError);
	DDX_Text(pDX, IDC_EDIT120, m_strReg0x02Read);
	DDX_Text(pDX, IDC_EDIT128, m_strReg0x03Read);
	DDX_Text(pDX, IDC_EDIT129, m_strReg0x04Read);
	DDX_Text(pDX, IDC_EDIT130, m_strReg0x05Read);
	DDX_Text(pDX, IDC_EDIT131, m_strReg0x0CRead);
	DDX_Text(pDX, IDC_EDIT132, m_strReg0x11Read);
	DDX_Text(pDX, IDC_EDIT133, m_strReg0x12Read);
	DDX_Text(pDX, IDC_EDIT134, m_strReg0x18Read);
	DDX_Text(pDX, IDC_EDIT135, m_strReg0x19Read);
	DDX_Text(pDX, IDC_EDIT136, m_strReg0x1ARead);
	DDX_Text(pDX, IDC_EDIT155, m_strReg0x00Write);
	DDX_Text(pDX, IDC_EDIT140, m_strReg0x01Write);
	DDX_Text(pDX, IDC_EDIT141, m_strReg0x02Write);
	DDX_Text(pDX, IDC_EDIT142, m_strReg0x03Write);
	DDX_Text(pDX, IDC_EDIT143, m_strReg0x04Write);
	DDX_Text(pDX, IDC_EDIT144, m_strReg0x05Write);
	DDX_Text(pDX, IDC_EDIT145, m_strReg0x06Write);
	DDX_Text(pDX, IDC_EDIT146, m_strReg0x07Write);
	DDX_Text(pDX, IDC_EDIT147, m_strReg0x08Write);
	DDX_Text(pDX, IDC_EDIT148, m_strReg0x09Write);
	DDX_Text(pDX, IDC_EDIT149, m_strReg0x0aWrite);
	DDX_Text(pDX, IDC_EDIT150, m_strReg0x0bWrite);
	DDX_Text(pDX, IDC_EDIT151, m_strReg0x0cWrite);
	DDX_Text(pDX, IDC_EDIT152, m_strReg0x0dWrite);
	DDX_Text(pDX, IDC_EDIT153, m_strReg0x0eWrite);
	DDX_Text(pDX, IDC_EDIT154, m_strReg0x0fWrite);
	DDX_Text(pDX, IDC_EDIT171, m_strReg0x10Write);
	DDX_Text(pDX, IDC_EDIT156, m_strReg0x11Write);
	DDX_Text(pDX, IDC_EDIT157, m_strReg0x12Write);
	DDX_Text(pDX, IDC_EDIT158, m_strReg0x13Write);
	DDX_Text(pDX, IDC_EDIT159, m_strReg0x14Write);
	DDX_Text(pDX, IDC_EDIT160, m_strReg0x15Write);
	DDX_Text(pDX, IDC_EDIT161, m_strReg0x16Write);
	DDX_Text(pDX, IDC_EDIT162, m_strReg0x17Write);
	DDX_Text(pDX, IDC_EDIT163, m_strReg0x18Write);
	DDX_Text(pDX, IDC_EDIT164, m_strReg0x19Write);
	DDX_Text(pDX, IDC_EDIT165, m_strReg0x1aWrite);
	DDX_Text(pDX, IDC_EDIT166, m_strReg0x1bWrite);
	DDX_Text(pDX, IDC_EDIT167, m_strReg0x1cWrite);
	DDX_Text(pDX, IDC_EDIT168, m_strReg0x1dWrite);
	DDX_Text(pDX, IDC_EDIT169, m_strReg0x1eWrite);
	DDX_Text(pDX, IDC_EDIT170, m_strReg0x1fWrite);
}


BEGIN_MESSAGE_MAP(CHMC1197Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON61, &CHMC1197Dlg::OnBnClickedReadReg0x00)
	ON_BN_CLICKED(IDC_BUTTON66, &CHMC1197Dlg::OnBnClickedWriteReg0x00)
	ON_BN_CLICKED(IDC_BUTTON67, &CHMC1197Dlg::OnBnClickedWriteReg0x01)
	ON_BN_CLICKED(IDC_BUTTON2, &CHMC1197Dlg::OnBnClickedWriteReg0x02)
	ON_BN_CLICKED(IDC_BUTTON52, &CHMC1197Dlg::OnBnClickedWriteReg0x03)
	ON_BN_CLICKED(IDC_BUTTON84, &CHMC1197Dlg::OnBnClickedWriteReg0x04)
	ON_BN_CLICKED(IDC_BUTTON86, &CHMC1197Dlg::OnBnClickedWriteReg0x05)
	ON_BN_CLICKED(IDC_BUTTON74, &CHMC1197Dlg::OnBnClickedWriteReg0x06)
	ON_BN_CLICKED(IDC_BUTTON75, &CHMC1197Dlg::OnBnClickedWriteReg0x07)
	ON_BN_CLICKED(IDC_BUTTON88, &CHMC1197Dlg::OnBnClickedWriteReg0x08)
	ON_BN_CLICKED(IDC_BUTTON90, &CHMC1197Dlg::OnBnClickedWriteReg0x09)
	ON_BN_CLICKED(IDC_BUTTON92, &CHMC1197Dlg::OnBnClickedWriteReg0x0a)
	ON_BN_CLICKED(IDC_BUTTON77, &CHMC1197Dlg::OnBnClickedWriteReg0x0b)
	ON_BN_CLICKED(IDC_BUTTON94, &CHMC1197Dlg::OnBnClickedWriteReg0x0c)
	ON_BN_CLICKED(IDC_BUTTON96, &CHMC1197Dlg::OnBnClickedWriteReg0x0f)
	ON_BN_CLICKED(IDC_BUTTON103, &CHMC1197Dlg::OnBnClickedWriteReg0x14)
	ON_BN_CLICKED(IDC_BUTTON105, &CHMC1197Dlg::OnBnClickedWriteReg0x15)
	ON_BN_CLICKED(IDC_BUTTON107, &CHMC1197Dlg::OnBnClickedWriteReg0x16)
	ON_BN_CLICKED(IDC_BUTTON109, &CHMC1197Dlg::OnBnClickedWriteReg0x17)
	ON_BN_CLICKED(IDC_BUTTON113, &CHMC1197Dlg::OnBnClickedWriteReg0x18)
	ON_BN_CLICKED(IDC_BUTTON115, &CHMC1197Dlg::OnBnClickedWriteReg0x19)
	ON_BN_CLICKED(IDC_BUTTON111, &CHMC1197Dlg::OnBnClickedWriteReg0x1a)
	ON_BN_CLICKED(IDC_BUTTON117, &CHMC1197Dlg::OnBnClickedWriteReg0x21)
	ON_BN_CLICKED(IDC_BUTTON68, &CHMC1197Dlg::OnBnClickedReadReg0x01)
	ON_BN_CLICKED(IDC_BUTTON51, &CHMC1197Dlg::OnBnClickedReadReg0x02)
	ON_BN_CLICKED(IDC_BUTTON83, &CHMC1197Dlg::OnBnClickedReadReg0x03)
	ON_BN_CLICKED(IDC_BUTTON85, &CHMC1197Dlg::OnBnClickedReadReg0x04)
	ON_BN_CLICKED(IDC_BUTTON87, &CHMC1197Dlg::OnBnClickedReadReg0x05)
	ON_BN_CLICKED(IDC_BUTTON69, &CHMC1197Dlg::OnBnClickedReadReg0x06)
	ON_BN_CLICKED(IDC_BUTTON70, &CHMC1197Dlg::OnBnClickedReadReg0x07)
	ON_BN_CLICKED(IDC_BUTTON89, &CHMC1197Dlg::OnBnClickedReadReg0x08)
	ON_BN_CLICKED(IDC_BUTTON91, &CHMC1197Dlg::OnBnClickedReadReg0x09)
	ON_BN_CLICKED(IDC_BUTTON93, &CHMC1197Dlg::OnBnClickedReadReg0x0a)
	ON_BN_CLICKED(IDC_BUTTON78, &CHMC1197Dlg::OnBnClickedReadReg0x0b)
	ON_BN_CLICKED(IDC_BUTTON95, &CHMC1197Dlg::OnBnClickedReadReg0x0c)
	ON_BN_CLICKED(IDC_BUTTON97, &CHMC1197Dlg::OnBnClickedReadReg0x0f)
	ON_BN_CLICKED(IDC_BUTTON99, &CHMC1197Dlg::OnBnClickedReadReg0x10)
	ON_BN_CLICKED(IDC_BUTTON100, &CHMC1197Dlg::OnBnClickedReadReg0x11)
	ON_BN_CLICKED(IDC_BUTTON101, &CHMC1197Dlg::OnBnClickedReadReg0x12)
	ON_BN_CLICKED(IDC_BUTTON102, &CHMC1197Dlg::OnBnClickedReadReg0x13)
	ON_BN_CLICKED(IDC_BUTTON104, &CHMC1197Dlg::OnBnClickedReadReg0x14)
	ON_BN_CLICKED(IDC_BUTTON106, &CHMC1197Dlg::OnBnClickedReadReg0x15)
	ON_BN_CLICKED(IDC_BUTTON108, &CHMC1197Dlg::OnBnClickedReadReg0x16)
	ON_BN_CLICKED(IDC_BUTTON110, &CHMC1197Dlg::OnBnClickedReadReg0x17)
	ON_BN_CLICKED(IDC_BUTTON114, &CHMC1197Dlg::OnBnClickedReadReg0x18)
	ON_BN_CLICKED(IDC_BUTTON116, &CHMC1197Dlg::OnBnClickedReadReg0x19)
	ON_BN_CLICKED(IDC_BUTTON112, &CHMC1197Dlg::OnBnClickedReadReg0x1a)
	ON_BN_CLICKED(IDC_BUTTON138, &CHMC1197Dlg::OnBnClickedApply)
	ON_CBN_SELCHANGE(IDC_COMBO66, &CHMC1197Dlg::OnCbnSelchangeMode)
	ON_EN_CHANGE(IDC_EDIT137, &CHMC1197Dlg::OnEnChangeFreqRF)
	ON_BN_CLICKED(IDC_BUTTON139, &CHMC1197Dlg::OnBnClickedWriteAll)
	ON_BN_CLICKED(IDC_BUTTON140, &CHMC1197Dlg::OnBnClickedReadAll)
	ON_BN_CLICKED(IDC_BUTTON156, &CHMC1197Dlg::OnBnClickedWReg0x00)
	ON_BN_CLICKED(IDC_BUTTON141, &CHMC1197Dlg::OnBnClickedWReg0x01)
	ON_BN_CLICKED(IDC_BUTTON142, &CHMC1197Dlg::OnBnClickedWReg0x02)
	ON_BN_CLICKED(IDC_BUTTON143, &CHMC1197Dlg::OnBnClickedWReg0x03)
	ON_BN_CLICKED(IDC_BUTTON144, &CHMC1197Dlg::OnBnClickedWReg0x04)
	ON_BN_CLICKED(IDC_BUTTON145, &CHMC1197Dlg::OnBnClickedWReg0x05)
	ON_BN_CLICKED(IDC_BUTTON146, &CHMC1197Dlg::OnBnClickedWReg0x06)
	ON_BN_CLICKED(IDC_BUTTON147, &CHMC1197Dlg::OnBnClickedWReg0x07)
	ON_BN_CLICKED(IDC_BUTTON148, &CHMC1197Dlg::OnBnClickedWReg0x08)
	ON_BN_CLICKED(IDC_BUTTON149, &CHMC1197Dlg::OnBnClickedWReg0x09)
	ON_BN_CLICKED(IDC_BUTTON150, &CHMC1197Dlg::OnBnClickedWReg0x0a)
	ON_BN_CLICKED(IDC_BUTTON151, &CHMC1197Dlg::OnBnClickedWReg0x0b)
	ON_BN_CLICKED(IDC_BUTTON152, &CHMC1197Dlg::OnBnClickedWReg0x0c)
	ON_BN_CLICKED(IDC_BUTTON153, &CHMC1197Dlg::OnBnClickedWReg0x0d)
	ON_BN_CLICKED(IDC_BUTTON154, &CHMC1197Dlg::OnBnClickedWReg0x0e)
	ON_BN_CLICKED(IDC_BUTTON155, &CHMC1197Dlg::OnBnClickedWReg0x0f)
	ON_BN_CLICKED(IDC_BUTTON172, &CHMC1197Dlg::OnBnClickedWReg0x10)
	ON_BN_CLICKED(IDC_BUTTON157, &CHMC1197Dlg::OnBnClickedWReg0x11)
	ON_BN_CLICKED(IDC_BUTTON158, &CHMC1197Dlg::OnBnClickedWReg0x12)
	ON_BN_CLICKED(IDC_BUTTON159, &CHMC1197Dlg::OnBnClickedWReg0x13)
	ON_BN_CLICKED(IDC_BUTTON160, &CHMC1197Dlg::OnBnClickedWReg0x14)
	ON_BN_CLICKED(IDC_BUTTON161, &CHMC1197Dlg::OnBnClickedWReg0x15)
	ON_BN_CLICKED(IDC_BUTTON162, &CHMC1197Dlg::OnBnClickedWReg0x16)
	ON_BN_CLICKED(IDC_BUTTON163, &CHMC1197Dlg::OnBnClickedWReg0x17)
	ON_BN_CLICKED(IDC_BUTTON164, &CHMC1197Dlg::OnBnClickedWReg0x18)
	ON_BN_CLICKED(IDC_BUTTON165, &CHMC1197Dlg::OnBnClickedWReg0x19)
	ON_BN_CLICKED(IDC_BUTTON166, &CHMC1197Dlg::OnBnClickedWReg0x1a)
	ON_BN_CLICKED(IDC_BUTTON167, &CHMC1197Dlg::OnBnClickedWReg0x1b)
	ON_BN_CLICKED(IDC_BUTTON168, &CHMC1197Dlg::OnBnClickedWReg0x1c)
	ON_BN_CLICKED(IDC_BUTTON169, &CHMC1197Dlg::OnBnClickedWReg0x1d)
	ON_BN_CLICKED(IDC_BUTTON170, &CHMC1197Dlg::OnBnClickedWReg0x1e)
	ON_BN_CLICKED(IDC_BUTTON171, &CHMC1197Dlg::OnBnClickedWReg0x1f)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CHMC1197Dlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_CHECK7, &CHMC1197Dlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CHMC1197Dlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CHMC1197Dlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, &CHMC1197Dlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK34, &CHMC1197Dlg::OnBnClickedCheck34)
	ON_BN_CLICKED(IDC_CHECK35, &CHMC1197Dlg::OnBnClickedCheck35)
	ON_BN_CLICKED(IDC_CHECK36, &CHMC1197Dlg::OnBnClickedCheck36)
	ON_EN_CHANGE(IDC_EDIT12, &CHMC1197Dlg::OnEnChangeEdit12)
	ON_EN_CHANGE(IDC_EDIT90, &CHMC1197Dlg::OnEnChangeEdit90)
	ON_EN_CHANGE(IDC_EDIT91, &CHMC1197Dlg::OnEnChangeEdit91)
	ON_EN_CHANGE(IDC_EDIT92, &CHMC1197Dlg::OnEnChangeEdit92)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CHMC1197Dlg::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_CHECK37, &CHMC1197Dlg::OnBnClickedCheck37)
	ON_BN_CLICKED(IDC_CHECK38, &CHMC1197Dlg::OnBnClickedCheck38)
	ON_BN_CLICKED(IDC_CHECK39, &CHMC1197Dlg::OnBnClickedCheck39)
	ON_BN_CLICKED(IDC_CHECK40, &CHMC1197Dlg::OnBnClickedCheck40)
	ON_BN_CLICKED(IDC_CHECK41, &CHMC1197Dlg::OnBnClickedCheck41)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CHMC1197Dlg::OnCbnSelchangeCombo6)
	ON_BN_CLICKED(IDC_CHECK42, &CHMC1197Dlg::OnBnClickedCheck42)
	ON_EN_CHANGE(IDC_EDIT93, &CHMC1197Dlg::OnEnChangeEdit93)
	ON_BN_CLICKED(IDC_CHECK43, &CHMC1197Dlg::OnBnClickedCheck43)
	ON_BN_CLICKED(IDC_CHECK44, &CHMC1197Dlg::OnBnClickedCheck44)
	ON_BN_CLICKED(IDC_CHECK45, &CHMC1197Dlg::OnBnClickedCheck45)
	ON_BN_CLICKED(IDC_CHECK46, &CHMC1197Dlg::OnBnClickedCheck46)
	ON_CBN_SELCHANGE(IDC_COMBO31, &CHMC1197Dlg::OnCbnSelchangeCombo31)
	ON_CBN_SELCHANGE(IDC_COMBO32, &CHMC1197Dlg::OnCbnSelchangeCombo32)
	ON_CBN_SELCHANGE(IDC_COMBO39, &CHMC1197Dlg::OnCbnSelchangeCombo39)
	ON_CBN_SELCHANGE(IDC_COMBO40, &CHMC1197Dlg::OnCbnSelchangeCombo40)
	ON_BN_CLICKED(IDC_CHECK47, &CHMC1197Dlg::OnBnClickedCheck47)
	ON_BN_CLICKED(IDC_CHECK48, &CHMC1197Dlg::OnBnClickedCheck48)
	ON_BN_CLICKED(IDC_CHECK49, &CHMC1197Dlg::OnBnClickedCheck49)
	ON_CBN_SELCHANGE(IDC_COMBO41, &CHMC1197Dlg::OnCbnSelchangeCombo41)
	ON_BN_CLICKED(IDC_CHECK50, &CHMC1197Dlg::OnBnClickedCheck50)
	ON_CBN_SELCHANGE(IDC_COMBO42, &CHMC1197Dlg::OnCbnSelchangeCombo42)
	ON_BN_CLICKED(IDC_CHECK51, &CHMC1197Dlg::OnBnClickedCheck51)
	ON_CBN_SELCHANGE(IDC_COMBO43, &CHMC1197Dlg::OnCbnSelchangeCombo43)
	ON_BN_CLICKED(IDC_CHECK52, &CHMC1197Dlg::OnBnClickedCheck52)
	ON_BN_CLICKED(IDC_CHECK53, &CHMC1197Dlg::OnBnClickedCheck53)
	ON_BN_CLICKED(IDC_CHECK54, &CHMC1197Dlg::OnBnClickedCheck54)
	ON_BN_CLICKED(IDC_CHECK55, &CHMC1197Dlg::OnBnClickedCheck55)
	ON_BN_CLICKED(IDC_CHECK56, &CHMC1197Dlg::OnBnClickedCheck56)
	ON_EN_CHANGE(IDC_EDIT94, &CHMC1197Dlg::OnEnChangeEdit94)
	ON_CBN_SELCHANGE(IDC_COMBO44, &CHMC1197Dlg::OnCbnSelchangeCombo44)
	ON_BN_CLICKED(IDC_CHECK57, &CHMC1197Dlg::OnBnClickedCheck57)
	ON_BN_CLICKED(IDC_CHECK58, &CHMC1197Dlg::OnBnClickedCheck58)
	ON_BN_CLICKED(IDC_CHECK59, &CHMC1197Dlg::OnBnClickedCheck59)
	ON_BN_CLICKED(IDC_CHECK60, &CHMC1197Dlg::OnBnClickedCheck60)
	ON_CBN_SELCHANGE(IDC_COMBO45, &CHMC1197Dlg::OnCbnSelchangeCombo45)
	ON_EN_CHANGE(IDC_EDIT96, &CHMC1197Dlg::OnEnChangeEdit96)
	ON_CBN_SELCHANGE(IDC_COMBO46, &CHMC1197Dlg::OnCbnSelchangeCombo46)
	ON_BN_CLICKED(IDC_CHECK65, &CHMC1197Dlg::OnBnClickedCheck65)
	ON_CBN_SELCHANGE(IDC_COMBO47, &CHMC1197Dlg::OnCbnSelchangeCombo47)
	ON_CBN_SELCHANGE(IDC_COMBO48, &CHMC1197Dlg::OnCbnSelchangeCombo48)
	ON_BN_CLICKED(IDC_CHECK66, &CHMC1197Dlg::OnBnClickedCheck66)
	ON_EN_CHANGE(IDC_EDIT97, &CHMC1197Dlg::OnEnChangeEdit97)
	ON_CBN_SELCHANGE(IDC_COMBO49, &CHMC1197Dlg::OnCbnSelchangeCombo49)
	ON_BN_CLICKED(IDC_CHECK67, &CHMC1197Dlg::OnBnClickedCheck67)
	ON_BN_CLICKED(IDC_CHECK68, &CHMC1197Dlg::OnBnClickedCheck68)
	ON_CBN_SELCHANGE(IDC_COMBO50, &CHMC1197Dlg::OnCbnSelchangeCombo50)
	ON_CBN_SELCHANGE(IDC_COMBO51, &CHMC1197Dlg::OnCbnSelchangeCombo51)
	ON_CBN_SELCHANGE(IDC_COMBO52, &CHMC1197Dlg::OnCbnSelchangeCombo52)
	ON_CBN_SELCHANGE(IDC_COMBO53, &CHMC1197Dlg::OnCbnSelchangeCombo53)
	ON_BN_CLICKED(IDC_CHECK69, &CHMC1197Dlg::OnBnClickedCheck69)
	ON_BN_CLICKED(IDC_CHECK70, &CHMC1197Dlg::OnBnClickedCheck70)
	ON_BN_CLICKED(IDC_CHECK71, &CHMC1197Dlg::OnBnClickedCheck71)
	ON_BN_CLICKED(IDC_CHECK72, &CHMC1197Dlg::OnBnClickedCheck72)
	ON_BN_CLICKED(IDC_CHECK73, &CHMC1197Dlg::OnBnClickedCheck73)
	ON_BN_CLICKED(IDC_CHECK74, &CHMC1197Dlg::OnBnClickedCheck74)
	ON_BN_CLICKED(IDC_CHECK75, &CHMC1197Dlg::OnBnClickedCheck75)
	ON_BN_CLICKED(IDC_CHECK76, &CHMC1197Dlg::OnBnClickedCheck76)
	ON_CBN_SELCHANGE(IDC_COMBO54, &CHMC1197Dlg::OnCbnSelchangeCombo54)
	ON_CBN_SELCHANGE(IDC_COMBO55, &CHMC1197Dlg::OnCbnSelchangeCombo55)
	ON_CBN_SELCHANGE(IDC_COMBO56, &CHMC1197Dlg::OnCbnSelchangeCombo56)
	ON_CBN_SELCHANGE(IDC_COMBO57, &CHMC1197Dlg::OnCbnSelchangeCombo57)
	ON_EN_CHANGE(IDC_EDIT112, &CHMC1197Dlg::OnEnChangeEdit112)
	ON_EN_CHANGE(IDC_EDIT98, &CHMC1197Dlg::OnEnChangeEdit98)
	ON_BN_CLICKED(IDC_BUTTON173, &CHMC1197Dlg::OnBnClickedWReg)
END_MESSAGE_MAP()


BOOL CHMC1197Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ChipEnablePin.SetCurSel(0);
	m_DSMType.SetCurSel(2);
	m_LDCounts.SetCurSel(4);
	m_SDOOutputLevel.SetCurSel(1);
	m_CPDownGain.SetCurSel(100);
	m_CPUpGain.SetCurSel(100);
	m_CPOffset.SetCurSel(81);
	m_VtuneResolution.SetCurSel(6);
	m_FSMVSPIClock.SetCurSel(1);
	m_PDPhaseSelect.SetCurSel(0);
	m_GPOSel.SetCurSel(1);
	m_AuxSPIMode.SetCurSel(0);
	m_AuxGPOOutputLevel.SetCurSel(0);
	m_AuxSPIGPOOutput.SetCurSel(0);
	m_AuxSPIOutputs.SetCurSel(0);
	m_VCOBand.SetCurSel(2);
	m_RFDividerRatio.SetCurSel(4);
	m_LO1OutputGain.SetCurSel(3);
	m_LO2OutputGain.SetCurSel(2);
	m_DividerOutputGain.SetCurSel(1);
	m_LO1OutputMode.SetCurSel(1);
	m_LO2OutputMode.SetCurSel(0);
	m_CPOutputSelect.SetCurSel(0);
	m_ExternalVCOBias.SetCurSel(0);
	m_HarmonicLPFBand.SetCurSel(15);
	m_Mode.SetCurSel(2);

	m_XGroupBoxReg0x00.SetTextColor(BLACK);
	m_XGroupBoxReg0x01.SetTextColor(BLACK);
	m_XGroupBoxReg0x02.SetTextColor(BLACK);
	m_XGroupBoxReg0x03.SetTextColor(BLACK);
	m_XGroupBoxReg0x04.SetTextColor(BLACK);
	m_XGroupBoxReg0x05.SetTextColor(BLACK);
	m_XGroupBoxReg0x06.SetTextColor(BLACK);
	m_XGroupBoxReg0x07.SetTextColor(BLACK);
	m_XGroupBoxReg0x08.SetTextColor(BLACK);
	m_XGroupBoxReg0x09.SetTextColor(BLACK);
	m_XGroupBoxReg0x0A.SetTextColor(BLACK);
	m_XGroupBoxReg0x0B.SetTextColor(BLACK);
	m_XGroupBoxReg0x0C.SetTextColor(BLACK);
	m_XGroupBoxReg0x0F.SetTextColor(BLACK);
	m_XGroupBoxReg0x10.SetTextColor(BLACK);
	m_XGroupBoxReg0x11.SetTextColor(BLACK);
	m_XGroupBoxReg0x12.SetTextColor(BLACK);
	m_XGroupBoxReg0x13.SetTextColor(BLACK);
	m_XGroupBoxReg0x14.SetTextColor(BLACK);
	m_XGroupBoxReg0x15.SetTextColor(BLACK);
	m_XGroupBoxReg0x16.SetTextColor(BLACK);
	m_XGroupBoxReg0x17.SetTextColor(BLACK);
	m_XGroupBoxReg0x18.SetTextColor(BLACK);
	m_XGroupBoxReg0x19.SetTextColor(BLACK);
	m_XGroupBoxReg0x1A.SetTextColor(BLACK);
	m_XGroupBoxReg0x21.SetTextColor(BLACK);
	m_XGroupBoxQuickSet.SetTextColor(BLACK);

	m_XGroupBoxReg0x00.SetBorderColor(BLACK);
	m_XGroupBoxReg0x01.SetBorderColor(BLACK);
	m_XGroupBoxReg0x02.SetBorderColor(BLACK);
	m_XGroupBoxReg0x03.SetBorderColor(BLACK);
	m_XGroupBoxReg0x04.SetBorderColor(BLACK);
	m_XGroupBoxReg0x05.SetBorderColor(BLACK);
	m_XGroupBoxReg0x06.SetBorderColor(BLACK);
	m_XGroupBoxReg0x07.SetBorderColor(BLACK);
	m_XGroupBoxReg0x08.SetBorderColor(BLACK);
	m_XGroupBoxReg0x09.SetBorderColor(BLACK);
	m_XGroupBoxReg0x0A.SetBorderColor(BLACK);
	m_XGroupBoxReg0x0B.SetBorderColor(BLACK);
	m_XGroupBoxReg0x0C.SetBorderColor(BLACK);
	m_XGroupBoxReg0x0F.SetBorderColor(BLACK);
	m_XGroupBoxReg0x10.SetBorderColor(BLACK);
	m_XGroupBoxReg0x11.SetBorderColor(BLACK);
	m_XGroupBoxReg0x12.SetBorderColor(BLACK);
	m_XGroupBoxReg0x13.SetBorderColor(BLACK);
	m_XGroupBoxReg0x14.SetBorderColor(BLACK);
	m_XGroupBoxReg0x15.SetBorderColor(BLACK);
	m_XGroupBoxReg0x16.SetBorderColor(BLACK);
	m_XGroupBoxReg0x17.SetBorderColor(BLACK);
	m_XGroupBoxReg0x18.SetBorderColor(BLACK);
	m_XGroupBoxReg0x19.SetBorderColor(BLACK);
	m_XGroupBoxReg0x1A.SetBorderColor(BLACK);
	m_XGroupBoxReg0x21.SetBorderColor(BLACK);
	m_XGroupBoxQuickSet.SetBorderColor(GREEN);
	Para2Reg();
	return TRUE;
}

void CHMC1197Dlg::Para2Reg()
{
	CString strRegWrite[27];
	strRegWrite[0x00].Format("%#010x",Para2Reg00());
	strRegWrite[0x01].Format("%#010x",Para2Reg01());
	strRegWrite[0x02].Format("%#010x",Para2Reg02());
	strRegWrite[0x03].Format("%#010x",Para2Reg03());
	strRegWrite[0x04].Format("%#010x",Para2Reg04());
	strRegWrite[0x05].Format("%#010x",Para2Reg05());
	strRegWrite[0x06].Format("%#010x",Para2Reg06());
	strRegWrite[0x07].Format("%#010x",Para2Reg07());
	strRegWrite[0x08].Format("%#010x",Para2Reg08());
	strRegWrite[0x09].Format("%#010x",Para2Reg09());
	strRegWrite[0x0a].Format("%#010x",Para2Reg0a());
	strRegWrite[0x0b].Format("%#010x",Para2Reg0b());
	strRegWrite[0x0c].Format("%#010x",Para2Reg0c());
	strRegWrite[0x0f].Format("%#010x",Para2Reg0f());
	strRegWrite[0x14].Format("%#010x",Para2Reg14());
	strRegWrite[0x15].Format("%#010x",Para2Reg15());
	strRegWrite[0x16].Format("%#010x",Para2Reg16());
	strRegWrite[0x17].Format("%#010x",Para2Reg17());
	strRegWrite[0x18].Format("%#010x",Para2Reg18());
	strRegWrite[0x19].Format("%#010x",Para2Reg19());
	strRegWrite[0x1a].Format("%#010x",Para2Reg1a());

	m_strReg0x00Write = strRegWrite[0x00];
	m_strReg0x01Write = strRegWrite[0x01];
	m_strReg0x02Write = strRegWrite[0x02];
	m_strReg0x03Write = strRegWrite[0x03];
	m_strReg0x04Write = strRegWrite[0x04];
	m_strReg0x05Write = strRegWrite[0x05];
	m_strReg0x06Write = strRegWrite[0x06];
	m_strReg0x07Write = strRegWrite[0x07];
	m_strReg0x08Write = strRegWrite[0x08];
	m_strReg0x09Write = strRegWrite[0x09];
	m_strReg0x0aWrite = strRegWrite[0x0a];
	m_strReg0x0bWrite = strRegWrite[0x0b];
	m_strReg0x0cWrite = strRegWrite[0x0c];
	m_strReg0x0fWrite = strRegWrite[0x0f];
	m_strReg0x14Write = strRegWrite[0x14];
	m_strReg0x15Write = strRegWrite[0x15];
	m_strReg0x16Write = strRegWrite[0x16];
	m_strReg0x17Write = strRegWrite[0x17];
	m_strReg0x18Write = strRegWrite[0x18];
	m_strReg0x19Write = strRegWrite[0x19];
	m_strReg0x1aWrite = strRegWrite[0x1a];
	UpdateData(FALSE);
}

uint32_t CHMC1197Dlg::Para2Reg00()
{
	UpdateData(TRUE);
	return hmc1197para2reg00(0,(uint32_t)m_sSoftReset);
}

uint32_t CHMC1197Dlg::Para2Reg01()
{
	UpdateData(TRUE);
	uint32_t uiChipEnPin = (uint32_t)m_ChipEnablePin.GetCurSel();
	uint32_t uiSPIChipEn = m_bSPIChipEnable ? 1 : 0;
	uint32_t uiKeepBiasOn = m_bKeepBiasOn ? 1 : 0;
	uint32_t uiKeepPFDOn = m_bKeepPFDOn ? 1 : 0;
	uint32_t uiKeepCPOn = m_bKeepCPOn ? 1 : 0;
	uint32_t uiKeepRefBufOn = m_bKeepRefBufferOn ? 1 : 0;
	uint32_t uiKeepVCOOn = m_bKeepVCOOn ? 1 : 0;
	uint32_t uiKeepGPODriOn = m_bKeepGPODriverOn ? 1 : 0;
	return hmc1197para2reg01(uiChipEnPin,uiSPIChipEn,uiKeepBiasOn,uiKeepPFDOn,uiKeepCPOn,uiKeepRefBufOn,uiKeepVCOOn,uiKeepGPODriOn);
}

uint32_t CHMC1197Dlg::Para2Reg02()
{
	UpdateData(TRUE);
	return hmc1197para2reg02((uint32_t)m_iRDiv);
}

uint32_t CHMC1197Dlg::Para2Reg03()
{
	UpdateData(TRUE);
	return hmc1197para2reg03((uint32_t)m_iINT);
}

uint32_t CHMC1197Dlg::Para2Reg04()
{
	UpdateData(TRUE);
	return hmc1197para2reg04((uint32_t)m_iFRAC);
}

uint32_t CHMC1197Dlg::Para2Reg05()
{
	return hmc1197para2reg05();
}

uint32_t CHMC1197Dlg::Para2Reg06()
{
	UpdateData(TRUE);
	uint32_t uiDSMOrder = (uint32_t)m_DSMType.GetCurSel();
	uint32_t uiSyncSPIMode = m_bSyncSPIMode ? 1 : 0;
	uint32_t uiExactFreqMode = m_bExactFreqMode ? 1 : 0;
	uint32_t uiFracBypass = m_bBypassDSM ? 1 : 0;
	uint32_t uiAutoSeed = m_bAutoseedDSM ? 1 : 0;
	uint32_t uiDSMEn = m_bDSMEnable ? 1 : 0;
	return hmc1197para2reg06(uiDSMOrder,uiSyncSPIMode,uiExactFreqMode,uiFracBypass,uiAutoSeed,uiDSMEn);
}

uint32_t CHMC1197Dlg::Para2Reg07()
{
	UpdateData(TRUE);
	uint32_t uiLDCnts = (uint32_t)m_LDCounts.GetCurSel();
	uint32_t uiLDEn = m_bLDEnable ? 1 : 0; 
	uint32_t uiLDTraining = (uint32_t)m_sLDTraining;
	uint32_t uiCSPEn = m_bCSPEnable ? 1 : 0;
	return hmc1197para2reg07(uiLDCnts,uiLDEn,uiLDTraining,uiCSPEn);
}

uint32_t CHMC1197Dlg::Para2Reg08()
{
	UpdateData(TRUE);
	uint32_t uiGPOLDOSDOEn = m_bGPOLDOSDOEnable ? 1 : 0;
	uint32_t uiVCOPsBiasEn = m_bVCOPsBiasEnable ? 1 : 0;
	uint32_t uiHighRef = m_bHighRef ? 1 : 0;
	uint32_t uiSDOLv = (uint32_t)m_SDOOutputLevel.GetCurSel();
	return hmc1197para2reg08(uiGPOLDOSDOEn,uiVCOPsBiasEn,uiHighRef,uiSDOLv);
}

uint32_t CHMC1197Dlg::Para2Reg09()
{
	UpdateData(TRUE);
	uint32_t uiCPDownGain = (uint32_t)m_CPDownGain.GetCurSel();
	uint32_t uiCPUpGain = (uint32_t)m_CPUpGain.GetCurSel();
	uint32_t uiCPOffset = (uint32_t)m_CPOffset.GetCurSel();
	uint32_t uiOffsetUpEn = m_bOffsetUpEnable ? 1 : 0;
	uint32_t uiOffsetDownEn = m_bOffsetDownEnable ? 1 : 0;
	uint32_t uiHikCPMode = m_bHikCPMode ? 1 : 0;
	return hmc1197para2reg09(uiCPDownGain,uiCPUpGain,uiCPOffset,uiOffsetUpEn,uiOffsetDownEn,uiHikCPMode);
}

uint32_t CHMC1197Dlg::Para2Reg0a()
{
	UpdateData(TRUE);
	uint32_t uiVtuneResolution = (uint32_t)m_VtuneResolution.GetCurSel();
	uint32_t uiAutoCalDis = m_bAutoCalDisable ? 1 : 0;
	uint32_t uiFSMVSPIClk = (uint32_t)m_FSMVSPIClock.GetCurSel();
	uint32_t uiAutoRelock = m_bAutoRelockOnce ? 1 : 0;
	return hmc1197para2reg0a(uiVtuneResolution,uiAutoCalDis,uiFSMVSPIClk,uiAutoRelock);
}

uint32_t CHMC1197Dlg::Para2Reg0b()
{
	UpdateData(TRUE);
	uint32_t uiPDPhaseSel = (uint32_t)m_PDPhaseSelect.GetCurSel();
	uint32_t uiPDUpOutput = m_bPDUpOutput ? 1 : 0;
	uint32_t uiPDDownOutput = m_bPDDownOutput ? 1 : 0;
	uint32_t uiForceCPUp = m_bForceCPUp ? 1 : 0;
	uint32_t uiForceCPDown = m_bForceCPDown ? 1 : 0;
	uint32_t uiForceCPMid = m_bForceCPMidRail ? 1 : 0;
	return hmc1197para2reg0b(uiPDPhaseSel,uiPDUpOutput,uiPDDownOutput,uiForceCPUp,uiForceCPDown,uiForceCPMid);
}

uint32_t CHMC1197Dlg::Para2Reg0c()
{
	UpdateData(TRUE);
	return hmc1197para2reg0c((uint32_t)m_iChannelsPerFPD);
}

uint32_t CHMC1197Dlg::Para2Reg0f()
{
	UpdateData(TRUE);
	uint32_t uiGPO = (uint32_t)m_GPOSel.GetCurSel();
	uint32_t uiGPOTestData = m_bGPOTestData ? 1 : 0;
	uint32_t uiPreventAutomuxSDO = m_bPreventAutomuxSDO ? 1 : 0;
	uint32_t uiPFETDis = m_bDisablePFET ? 1 : 0;
	uint32_t uiNFETDis = m_bDisableNFET ? 1 : 0;
	return hmc1197para2reg0f(uiGPO,m_bGPOTestData,uiPreventAutomuxSDO,uiPFETDis,uiNFETDis);
}

uint32_t CHMC1197Dlg::Para2Reg14()
{
	UpdateData(TRUE);
	uint32_t uiAuxSPIMode = (uint32_t)m_AuxSPIMode.GetCurSel();
	uint32_t uiAuxGPOValues = (uint32_t)m_sAuxGPOValues;
	uint32_t uiAuxGPOOutputLv = (uint32_t)m_AuxGPOOutputLevel.GetCurSel();
	uint32_t uiPhaseSync = m_bPhaseSync ? 1 : 0;
	uint32_t uiAuxSPIGPOOutput = (uint32_t)m_AuxSPIGPOOutput.GetCurSel();
	uint32_t uiAuxSPIOutputs = (uint32_t)m_AuxSPIOutputs.GetCurSel();
	return hmc1197para2reg14(uiAuxSPIMode,uiAuxGPOValues,uiAuxGPOOutputLv,uiPhaseSync,uiAuxSPIGPOOutput,uiAuxSPIOutputs);
}

uint32_t CHMC1197Dlg::Para2Reg15()
{
	UpdateData(TRUE);
	uint32_t uiVCOManualCal = m_bVCOManualCal ? 1 : 0;
	uint32_t uiCapacitorSwSet = (uint32_t)m_iCapacitorSwitchSet;
	uint32_t uiVCOBand = (uint32_t)m_VCOBand.GetCurSel();
	uint32_t uiManualVCOTune = m_bManualVCOTune ? 1 : 0;
	uint32_t uiAutoScaleCPCurrentEn = m_bAutoScaleCPCurrent ? 1 : 0;
	return hmc1197para2reg15(uiVCOManualCal,uiCapacitorSwSet,uiVCOBand,uiManualVCOTune,uiAutoScaleCPCurrentEn);
}

uint32_t CHMC1197Dlg::Para2Reg16()
{
	UpdateData(TRUE);
	uint32_t uiRFDivRatio = (uint32_t)m_RFDividerRatio.GetCurSel();
	uint32_t uiLO1OutputGain = (uint32_t)m_LO1OutputGain.GetCurSel();
	uint32_t uiLO2OutputGain = (uint32_t)m_LO2OutputGain.GetCurSel();
	uint32_t uiDivOutputGain = (uint32_t)m_DividerOutputGain.GetCurSel();
	return hmc1197para2reg16(uiRFDivRatio,uiLO1OutputGain,uiLO2OutputGain,uiDivOutputGain);
}

uint32_t CHMC1197Dlg::Para2Reg17()
{
	UpdateData(TRUE);
	uint32_t uiVCOSubsysMasterEn = m_bVCOSubsysMasterEnable ? 1 : 0;
	uint32_t uiVCOEn = m_bVCOEnable ? 1 : 0;
	uint32_t uiExtVCOBufferEn = m_bExternalVCOBufferEnable ? 1 : 0;
	uint32_t uiPLLEn = m_bPLLBufferEnable ? 1 : 0;
	uint32_t uiLO1OutputEn = m_bLO1OutputBufferEnable ? 1 : 0;
	uint32_t uiLO2OutputEn = m_bLO2OutputBufferEnable ? 1 : 0;
	uint32_t uiExtVCOInputEn = m_bExternalVCOInputEnable ? 1 : 0;
	uint32_t uiPreLockMute = m_bPreLockMute ? 1 : 0;
	uint32_t uiLO1OutputMode = (uint32_t)m_LO1OutputMode.GetCurSel();
	uint32_t uiLO2OutputMode = (uint32_t)m_LO2OutputMode.GetCurSel();
	uint32_t uiCPOutputSel = (uint32_t)m_CPOutputSelect.GetCurSel();
	return hmc1197para2reg17(uiVCOSubsysMasterEn,uiVCOEn,uiExtVCOBufferEn,uiPLLEn,uiLO1OutputEn,uiLO2OutputEn,uiExtVCOInputEn,uiPreLockMute,uiLO1OutputMode,uiLO2OutputMode,uiCPOutputSel);
}

uint32_t CHMC1197Dlg::Para2Reg18()
{
	UpdateData(TRUE);
	uint32_t uiExtVCOBisa = (uint32_t)m_ExternalVCOBias.GetCurSel();
	return hmc1197para2reg18(uiExtVCOBisa);	
}

uint32_t CHMC1197Dlg::Para2Reg19()
{
	return hmc1197para2reg19();
}

uint32_t CHMC1197Dlg::Para2Reg1a()
{
	UpdateData(TRUE);
	return hmc1197para2reg1a((uint32_t)m_iReg0x1AWrite);
}

uint32_t CHMC1197Dlg::Para2Reg21()
{
	UpdateData(TRUE);
	uint32_t uiHarmonicLPFSel = (uint32_t)m_HarmonicLPFBand.GetCurSel();
	return hmc1197para2reg21(uiHarmonicLPFSel);
}

void CHMC1197Dlg::OnBnClickedWriteReg0x00()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg00());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x01()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg01());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x02()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg02());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x03()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg03());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x04()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg04());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x05()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg05());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x06()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg06());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x07()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg07());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x08()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg08());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x09()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg09());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x0a()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg0a());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x0b()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg0b());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x0c()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg0c());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x0f()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg0f());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x14()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg14());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x15()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg15());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x16()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg16());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x17()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg17());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x18()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg18());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x19()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg19());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x1a()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg1a());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedWriteReg0x21()
{
	m_pSP1401->SetTxLO1Reg(Para2Reg21());
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedReadReg0x00()
{
	uint32_t uiRegData = 0;
	m_pSP1401->GetTxLO1Reg(0x00,uiRegData);
	m_strReg0x00Read.Format("%#08X",uiRegData);
	UpdateData(FALSE);
	SetFocus();
}

void CHMC1197Dlg::OnBnClickedReadReg0x01()
{
	READ_REG(0x01);
}

void CHMC1197Dlg::OnBnClickedReadReg0x02()
{
	READ_REG(0x02);
}

void CHMC1197Dlg::OnBnClickedReadReg0x03()
{
	READ_REG(0x03);
}

void CHMC1197Dlg::OnBnClickedReadReg0x04()
{
	READ_REG(0x04);
}

void CHMC1197Dlg::OnBnClickedReadReg0x05()
{
	READ_REG(0x05);
}

void CHMC1197Dlg::OnBnClickedReadReg0x06()
{
	READ_REG(0x06);
}

void CHMC1197Dlg::OnBnClickedReadReg0x07()
{
	READ_REG(0x07);
}

void CHMC1197Dlg::OnBnClickedReadReg0x08()
{
	READ_REG(0x08);
}

void CHMC1197Dlg::OnBnClickedReadReg0x09()
{
	READ_REG(0x09);
}

void CHMC1197Dlg::OnBnClickedReadReg0x0a()
{
	READ_REG(0x0A);
}

void CHMC1197Dlg::OnBnClickedReadReg0x0b()
{
	READ_REG(0x0B);
}

void CHMC1197Dlg::OnBnClickedReadReg0x0c()
{
	READ_REG(0x0C);
}

void CHMC1197Dlg::OnBnClickedReadReg0x0f()
{
	READ_REG(0x0F);
}

void CHMC1197Dlg::OnBnClickedReadReg0x10()
{
	READ_REG(0x10);
}

void CHMC1197Dlg::OnBnClickedReadReg0x11()
{
	READ_REG(0x11);
}

void CHMC1197Dlg::OnBnClickedReadReg0x12()
{
	READ_REG(0x12);
}

void CHMC1197Dlg::OnBnClickedReadReg0x13()
{
	READ_REG(0x13);
}

void CHMC1197Dlg::OnBnClickedReadReg0x14()
{
	READ_REG(0x14);
}

void CHMC1197Dlg::OnBnClickedReadReg0x15()
{
	READ_REG(0x15);
}

void CHMC1197Dlg::OnBnClickedReadReg0x16()
{
	READ_REG(0x16);
}

void CHMC1197Dlg::OnBnClickedReadReg0x17()
{
	READ_REG(0x17);
}

void CHMC1197Dlg::OnBnClickedReadReg0x18()
{
	READ_REG(0x18);
}

void CHMC1197Dlg::OnBnClickedReadReg0x19()
{
	READ_REG(0x19);
}

void CHMC1197Dlg::OnBnClickedReadReg0x1a()
{
	READ_REG(0x1A);
}

void CHMC1197Dlg::OnBnClickedApply()
{
	UpdateData(TRUE);
	uint64_t uiFreqVCO = 0;
	uint32_t _PD = 100000000;
	uint32_t _INT = 0;
	uint32_t _FRAC = 0;
	uint32_t _CHS = 0;
	FreqMode Mode = (FreqMode)(m_Mode.GetCurSel());

	switch (Mode) {
		case _INT_Mode : {
			OnBnClickedWriteReg0x02();
			OnBnClickedWriteReg0x03();
			OnBnClickedWriteReg0x06();
			OnBnClickedWriteReg0x09();
			return;
					}
		case _FRAC_Mode : {
			m_CPOffset.SetCurSel(0);
			m_bOffsetUpEnable = FALSE;
			m_bOffsetDownEnable = FALSE;
			UpdateData(FALSE);
			OnBnClickedWriteReg0x02();
			OnBnClickedWriteReg0x03();
			OnBnClickedWriteReg0x04();
			OnBnClickedWriteReg0x06();
			OnBnClickedWriteReg0x09();
			m_CPOffset.SetCurSel(81);
			m_bOffsetUpEnable = TRUE;
			m_bOffsetDownEnable = TRUE;
			UpdateData(FALSE);
			OnBnClickedWriteReg0x09();
			return;
						  }
		case _EXACT_Mode : {
			if (m_dFreqError != 0.0) {
				CseMessageBox("not support this freq");
				return;
			}
			m_CPOffset.SetCurSel(0);
			m_bOffsetUpEnable = FALSE;
			m_bOffsetDownEnable = FALSE;
			UpdateData(FALSE);
			OnBnClickedWriteReg0x02();
			OnBnClickedWriteReg0x03();
			OnBnClickedWriteReg0x04();
			OnBnClickedWriteReg0x06();
			OnBnClickedWriteReg0x09();
			OnBnClickedWriteReg0x0c();
			m_CPOffset.SetCurSel(81);
			m_bOffsetUpEnable = TRUE;
			m_bOffsetDownEnable = TRUE;
			UpdateData(FALSE);
			OnBnClickedWriteReg0x09();
			return;
						   }
	}
}

void CHMC1197Dlg::OnCbnSelchangeMode()
{
	OnEnChangeFreqRF();
}

void CHMC1197Dlg::OnEnChangeFreqRF()
{
	UpdateData(TRUE);
	uint64_t uiFreqRF = 0,uiFreqVCO = 0;
	uint32_t _PD = 100000000;
	uint32_t _INT = 0;
	uint32_t _FRAC = 0;
	uint32_t _CHS = 0;
	int32_t iDivIdx = 0;
	str2freq(m_strFreqRF,uiFreqRF);
	FreqMode Mode = (FreqMode)(m_Mode.GetCurSel());

	if (uiFreqRF > 4100000000) {
		uiFreqRF = 4100000000;
		freq2str(uiFreqRF,m_strFreqRF);
		UpdateData(FALSE);
	}
	uiFreqVCO = uiFreqRF;
	if (uiFreqRF < 2050000000) {
		int32_t iDiv[63] = {0,1,2,2,4,4,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,60,62};
		for (iDivIdx = 0;iDivIdx < ARRAY_SIZE(iDiv);iDivIdx ++) {
			uiFreqVCO = uiFreqRF * iDiv[iDivIdx];
			if (uiFreqVCO >= 2050000000 && uiFreqVCO <= 4100000000) {
				m_RFDividerRatio.SetCurSel(iDivIdx);
				break;
			}
		}
	}
	freq2str(uiFreqVCO,m_strFreqVCO);

	switch (Mode) {
		case _INT_Mode : {
			hmc1197_freq_formula_intmode(uiFreqVCO,_PD,1,_INT,m_dFreqError);
			m_iINT = (int)_INT;
			m_iFRAC = 0;
			m_iChannelsPerFPD = 0;
			m_bExactFreqMode = FALSE;
			m_bBypassDSM = TRUE;
			m_bDSMEnable = FALSE;
			m_CPOffset.SetCurSel(0);
			m_bOffsetUpEnable = FALSE;
			m_bOffsetDownEnable = FALSE;
			if (m_dFreqError) {
				m_dFreqError /= (double)iDivIdx;
				uiFreqVCO = uiFreqVCO / _PD * _PD;
				freq2str(uiFreqVCO,m_strFreqVCO);
			}
			break;
						 }
		case _FRAC_Mode : {
			hmc1197_freq_formula_fracmode(uiFreqVCO,_PD,1,_INT,_FRAC,m_dFreqError);
			m_iINT = (int)_INT;
			m_iFRAC = (int)_FRAC;
			m_iChannelsPerFPD = 0;
			m_bExactFreqMode = FALSE;
			m_bBypassDSM = FALSE;
			m_bDSMEnable = TRUE;
			m_CPOffset.SetCurSel(81);
			m_bOffsetUpEnable = TRUE;
			m_bOffsetDownEnable = TRUE;
			break;
						  }
		case _EXACT_Mode : {
			if (hmc1197_freq_formula_exactmode(uiFreqVCO,_PD,1,_INT,_FRAC,_CHS) < 0)
				m_dFreqError = (double)uiFreqRF;
			else
				m_dFreqError = 0.0;
			m_iINT = (int)_INT;
			m_iFRAC = (int)_FRAC;
			m_iChannelsPerFPD = (int)_CHS;
			m_bExactFreqMode = TRUE;
			m_bBypassDSM = FALSE;
			m_bDSMEnable = TRUE;
			m_CPOffset.SetCurSel(81);
			m_bOffsetUpEnable = TRUE;
			m_bOffsetDownEnable = TRUE;
			break;
						   }
		default:break;
	}
	UpdateData(FALSE);
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedWriteAll()
{
	OnBnClickedWriteReg0x00();
	OnBnClickedWriteReg0x01();
	OnBnClickedWriteReg0x02();
	OnBnClickedWriteReg0x03();
	OnBnClickedWriteReg0x04();
	OnBnClickedWriteReg0x05();
	OnBnClickedWriteReg0x06();
	OnBnClickedWriteReg0x07();
	OnBnClickedWriteReg0x08();
	OnBnClickedWriteReg0x09();
	OnBnClickedWriteReg0x0a();
	OnBnClickedWriteReg0x0b();
	OnBnClickedWriteReg0x0c();
	OnBnClickedWriteReg0x0f();
	OnBnClickedWriteReg0x14();
	OnBnClickedWriteReg0x15();
	OnBnClickedWriteReg0x16();
	OnBnClickedWriteReg0x17();
	OnBnClickedWriteReg0x18();
	OnBnClickedWriteReg0x19();
	OnBnClickedWriteReg0x1a();
//	OnBnClickedWriteReg0x21();
}

void CHMC1197Dlg::OnBnClickedReadAll()
{
	OnBnClickedReadReg0x00();
	OnBnClickedReadReg0x01();
	OnBnClickedReadReg0x02();
	OnBnClickedReadReg0x03();
	OnBnClickedReadReg0x04();
	OnBnClickedReadReg0x05();
	OnBnClickedReadReg0x06();
	OnBnClickedReadReg0x07();
	OnBnClickedReadReg0x08();
	OnBnClickedReadReg0x09();
	OnBnClickedReadReg0x0a();
	OnBnClickedReadReg0x0b();
	OnBnClickedReadReg0x0c();
	OnBnClickedReadReg0x0f();
	OnBnClickedReadReg0x10();
	OnBnClickedReadReg0x11();
	OnBnClickedReadReg0x12();
	OnBnClickedReadReg0x13();
	OnBnClickedReadReg0x14();
	OnBnClickedReadReg0x15();
	OnBnClickedReadReg0x16();
	OnBnClickedReadReg0x17();
	OnBnClickedReadReg0x18();
	OnBnClickedReadReg0x19();
	OnBnClickedReadReg0x1a();
}

BOOL CHMC1197Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

BOOL CHMC1197Dlg::ShowWindow(int nCmdShow)
{
	SetWindowTextX();
	return CDialog::ShowWindow(nCmdShow);
}

void CHMC1197Dlg::SetWindowTextX()
{
	if (!::IsWindow(m_hWnd))
		return;

	CString strWndText;
	uint32_t uiRfuIdx = 0,uiRfIdx = 0;

	if (m_pSP1401) {
		uiRfIdx = m_pSP1401->GetRfIdx();
		uiRfuIdx = m_pSP1401->GetRfuIdx();
	}
	strWndText.Format("R1C_HMC1197_RFU%d_RF%d_Tx_LO1",uiRfuIdx,uiRfIdx);

	return SetWindowText(strWndText);
}

void CHMC1197Dlg::OnBnClickedWReg0x00()
{
	WRITE_REG(0x00);
}

void CHMC1197Dlg::OnBnClickedWReg0x01()
{
	WRITE_REG(0x01);
}

void CHMC1197Dlg::OnBnClickedWReg0x02()
{
	WRITE_REG(0x02);
}

void CHMC1197Dlg::OnBnClickedWReg0x03()
{
	WRITE_REG(0x03);
}

void CHMC1197Dlg::OnBnClickedWReg0x04()
{
	WRITE_REG(0x04);
}

void CHMC1197Dlg::OnBnClickedWReg0x05()
{
	WRITE_REG(0x05);
}

void CHMC1197Dlg::OnBnClickedWReg0x06()
{
	WRITE_REG(0x06);
}

void CHMC1197Dlg::OnBnClickedWReg0x07()
{
	WRITE_REG(0x07);
}

void CHMC1197Dlg::OnBnClickedWReg0x08()
{
	WRITE_REG(0x08);
}

void CHMC1197Dlg::OnBnClickedWReg0x09()
{
	WRITE_REG(0x09);
}

void CHMC1197Dlg::OnBnClickedWReg0x0a()
{
	WRITE_REG(0x0a);
}

void CHMC1197Dlg::OnBnClickedWReg0x0b()
{
	WRITE_REG(0x0b);
}

void CHMC1197Dlg::OnBnClickedWReg0x0c()
{
	WRITE_REG(0x0c);
}

void CHMC1197Dlg::OnBnClickedWReg0x0d()
{
	WRITE_REG(0x0d);
}

void CHMC1197Dlg::OnBnClickedWReg0x0e()
{
	WRITE_REG(0x0e);
}

void CHMC1197Dlg::OnBnClickedWReg0x0f()
{
	WRITE_REG(0x0f);
}

void CHMC1197Dlg::OnBnClickedWReg0x10()
{
	WRITE_REG(0x10);
}

void CHMC1197Dlg::OnBnClickedWReg0x11()
{
	WRITE_REG(0x11);
}

void CHMC1197Dlg::OnBnClickedWReg0x12()
{
	WRITE_REG(0x12);
}

void CHMC1197Dlg::OnBnClickedWReg0x13()
{
	WRITE_REG(0x13);
}

void CHMC1197Dlg::OnBnClickedWReg0x14()
{
	WRITE_REG(0x14);
}

void CHMC1197Dlg::OnBnClickedWReg0x15()
{
	WRITE_REG(0x15);
}

void CHMC1197Dlg::OnBnClickedWReg0x16()
{
	WRITE_REG(0x16);
}

void CHMC1197Dlg::OnBnClickedWReg0x17()
{
	WRITE_REG(0x17);
}

void CHMC1197Dlg::OnBnClickedWReg0x18()
{
	WRITE_REG(0x18);
}

void CHMC1197Dlg::OnBnClickedWReg0x19()
{
	WRITE_REG(0x19);
}

void CHMC1197Dlg::OnBnClickedWReg0x1a()
{
	WRITE_REG(0x1a);
}

void CHMC1197Dlg::OnBnClickedWReg0x1b()
{
	WRITE_REG(0x1b);
}

void CHMC1197Dlg::OnBnClickedWReg0x1c()
{
	WRITE_REG(0x1c);
}

void CHMC1197Dlg::OnBnClickedWReg0x1d()
{
	WRITE_REG(0x1d);
}

void CHMC1197Dlg::OnBnClickedWReg0x1e()
{
	WRITE_REG(0x1e);
}

void CHMC1197Dlg::OnBnClickedWReg0x1f()
{
	WRITE_REG(0x1f);
}

void CHMC1197Dlg::OnCbnSelchangeCombo2()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck7()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck8()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck9()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck10()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck34()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck35()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck36()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit12()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit90()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit91()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit92()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo3()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck37()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck38()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck39()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck40()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck41()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo6()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck42()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit93()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck43()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck44()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck45()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck46()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo31()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo32()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo39()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo40()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck47()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck48()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck49()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo41()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck50()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo42()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck51()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo43()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck52()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck53()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck54()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck55()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck56()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit94()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo44()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck57()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck58()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck59()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck60()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo45()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit96()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo46()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck65()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo47()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo48()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck66()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit97()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo49()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck67()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck68()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo50()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo51()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo52()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo53()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck69()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck70()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck71()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck72()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck73()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck74()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck75()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedCheck76()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo54()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo55()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo56()
{
	Para2Reg();
}

void CHMC1197Dlg::OnCbnSelchangeCombo57()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit112()
{
	Para2Reg();
}

void CHMC1197Dlg::OnEnChangeEdit98()
{
	Para2Reg();
}

void CHMC1197Dlg::OnBnClickedWReg()
{
	OnBnClickedWReg0x00();
	OnBnClickedWReg0x01();
	OnBnClickedWReg0x02();
	OnBnClickedWReg0x03();
	OnBnClickedWReg0x04();
	OnBnClickedWReg0x05();
	OnBnClickedWReg0x06();
	OnBnClickedWReg0x07();
	OnBnClickedWReg0x08();
	OnBnClickedWReg0x09();
	OnBnClickedWReg0x0a();
	OnBnClickedWReg0x0b();
	OnBnClickedWReg0x0c();
	OnBnClickedWReg0x0d();
	OnBnClickedWReg0x0e();
	OnBnClickedWReg0x0f();
	OnBnClickedWReg0x10();
	OnBnClickedWReg0x11();
	OnBnClickedWReg0x12();
	OnBnClickedWReg0x13();
	OnBnClickedWReg0x14();
	OnBnClickedWReg0x15();
	OnBnClickedWReg0x16();
	OnBnClickedWReg0x17();
	OnBnClickedWReg0x18();
	OnBnClickedWReg0x19();
	OnBnClickedWReg0x1a();
	OnBnClickedWReg0x1b();
	OnBnClickedWReg0x1c();
	OnBnClickedWReg0x1d();
	OnBnClickedWReg0x1e();
	OnBnClickedWReg0x1f();
}