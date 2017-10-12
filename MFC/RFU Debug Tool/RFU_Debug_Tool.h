#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"
#include "MainDlg.h"
#include "ADF5355Dlg.h"
#include "HMC1197Dlg.h"

#include "LogDlg.h"
#include "LogViewerDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

#include "TestCMPBDlg.h"
#include "TestAlgo5355Dlg.h"
#include "TestFittingInterpDlg.h"

#define APPNAME	_T("SPC5 RFU Debug Tool")

#define PROTECT_EYE (RGB(199,237,204))
#define RED			(RGB(255,0,0))
#define GREEN		(RGB(0,255,0))
#define BLUE		(RGB(0,0,255))
#define YELLOW		(RGB(255,255,0))
#define WHITE		(RGB(255,255,255))
#define BLACK		(RGB(0,0,0))
#define BROWN		(RGB(128,64,0))
#define NAVY		(RGB(0,255,255))
#define PURPLE		(RGB(255,0,255))
#define GREY		(RGB(192,192,192))
#define ORANGE		(RGB(250,128,10))
#define PERU		(RGB(205,133,63))
#define SA_BCKGRD	(RGB(63,49,44))

#define SLATEBLUE	(RGB(106,90,205))
#define KHAKI3		(RGB(205,198,115))
#define SIENNA		(RGB(160,82,45))
#define INDIANRED1	(RGB(255,106,106))
#define PINK		(RGB(255,192,203))
#define TOMATO		(RGB(255,99,71))
#define PEACHPUFF1	(RGB(255,218,185))
#define CYAN2		(RGB(0,238,238))
#define AQUAMARINE1	(RGB(127,255,212))
#define DIMGREY		(RGB(105,105,105))
#define OLIVEDRAB1	(RGB(192,255,62))
#define GREENYELLOW	(RGB(173,255,47))
#define ROSYBROWN	(RGB(188,143,143))
#define INDIANRED3	(RGB(205,85,85))
#define VIOLET		(RGB(238,130,238))
#define WHEAT1		(RGB(255,231,186))
#define CHOCOLATE1	(RGB(255,127,36))
#define BISQUE3		(RGB(205,183,158))
#define CORAl1		(RGB(255,114,86))
#define IVORY1		(RGB(255,255,240))
#define ROYALBLUE4	(RGB(39,64,139))
#define PLUM3		(RGB(205,150,201))
#define PURPLE2		(RGB(145,44,238))
#define DARKCYAN	(RGB(0,139,139))

#define WM_CSE_SINGLE_THREAD_DONE			(WM_USER + 1)	//single calibration or test thread done
#define WM_CSE_SINGLE_THREAD_CANCEL			(WM_USER + 2)	//single calibration or test thread cancel
#define WM_CSE_SP3301_INIT					(WM_USER + 10)	//rfu init
#define WM_CSE_SP3301_CHANGE				(WM_USER + 11)	//rfu has changed
#define WM_CSE_SWHW_VERSION					(WM_USER + 12)	//show hardware and software version
#define WM_CSE_ALL_IQ_CAP_SHOW				(WM_USER + 14)	//show data when running continuous all ch iq capture
#define WM_CSE_UPDATE_DATA_FALSE			(WM_USER + 15)	//update data false
#define WM_CSE_IQ_CHART						(WM_USER + 16)	//show arb file iq chart dialog
#define WM_CSE_UPDATE_DATA_TRUE				(WM_USER + 17)	//update data true

#define WM_CSE_SYNCRFANDRFD_DLG_TXFREQ		(WM_USER + 21)		//sync parameters between rf overview dialog and rf advance dialog
#define WM_CSE_SYNCRFANDRFD_DLG_TXATT0		(WM_USER + 22)		//r1c
#define WM_CSE_SYNCRFANDRFD_DLG_TXATT1		(WM_USER + 23)
#define WM_CSE_SYNCRFANDRFD_DLG_TXATT2		(WM_USER + 24)
#define WM_CSE_SYNCRFANDRFD_DLG_TXPAATT		(WM_USER + 25)
#define WM_CSE_SYNCRFANDRFD_DLG_TXATT3		(WM_USER + 26)
#define WM_CSE_SYNCRFANDRFD_DLG_TXDET		(WM_USER + 27)
#define WM_CSE_SYNCRFANDRFD_DLG_TXTEMP		(WM_USER + 28)
#define WM_CSE_SYNCRFANDRFD_DLG_RXFREQ		(WM_USER + 29)
#define WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT	(WM_USER + 30)
#define WM_CSE_SYNCRFANDRFD_DLG_RXATT019	(WM_USER + 31)		//r1c
#define WM_CSE_SYNCRFANDRFD_DLG_RXATT1		(WM_USER + 32)
#define WM_CSE_SYNCRFANDRFD_DLG_RXATT2		(WM_USER + 33)
#define WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER	(WM_USER + 34)
#define WM_CSE_SYNCRFANDRFD_DLG_RXATT3		(WM_USER + 35)		//r1c
#define WM_CSE_SYNCRFANDRFD_DLG_RXDET		(WM_USER + 36)
#define WM_CSE_SYNCRFANDRFD_DLG_RXTEMP		(WM_USER + 37)
#define WM_CSE_SYNCRFANDRFD_DLG_IOMODE		(WM_USER + 38)

#define WM_CSE_RF_R1A_TEMP_GUARD_SHOW		(WM_USER + 50)		//show data when guarding r1a/b rf0,1,2,3's temperature
#define WM_CSE_RF_R1C_TEMP_GUARD_SHOW		(WM_USER + 51)		//show data when guarding r1c's temp0,1,2,3,4,5,6,7
#define WM_CSE_SP1401_CHANGE				(WM_USER + 52)		//rf has changed

#define WM_CSE_CAL_SHOW_DATA					(WM_USER + 60)
#define WM_CSE_CAL_TX_LOLEAKAGE_SHOW			(WM_USER + 61)		//show data when calibrating tx loleakage
#define WM_CSE_CAL_TX_SIDEBAND_SHOW_CALLING		(WM_USER + 63)		//show data when calibrating tx sideband
#define WM_CSE_CAL_TX_FILTER_RFFR_0_SHOW		(WM_USER + 62)		//show rf frequency response when calibrating tx filter
#define WM_CSE_CAL_TX_FILTER_RFFR_1_SHOW		(WM_USER + 77)		//show rf frequency response when calibrating tx filter
#define WM_CSE_CAL_TX_FILTER_IFFR_SHOW		(WM_USER + 64)		//show if frequency response when calibrating tx filter
#define WM_CSE_CAL_TX_FILTER_80_SHOW		(WM_USER + 71)		//show bw 80MHz coef when calibrating tx filter
#define WM_CSE_CAL_TX_FILTER_160_SHOW		(WM_USER + 73)		//show bw 160Mhz coef when calibrating tx filter
#define WM_CSE_CAL_TX_ATT_SHOW				(WM_USER + 65)		//show data when calibrating tx att
#define WM_CSE_CAL_TX_POWER_SHOW			(WM_USER + 66)		//show data when calibrating tx power
#define WM_CSE_CAL_TX_FILTER_OFF_80_SHOW	(WM_USER + 78)		//show data when calibrating tx filter 80M offset
#define WM_CSE_CAL_RX_SGPMM_SHOW			(WM_USER + 68)		//show data when calibrating rx sgpmm file
#define WM_CSE_CAL_RX_REF_SHOW				(WM_USER + 69)		//show data when calibrating rx reference
#define WM_CSE_CAL_RX_ATT_SHOW				(WM_USER + 72)		//show data when calibrating rx att
#define WM_CSE_CAL_RX_FILTER_RFFR_SHOW		(WM_USER + 77)		//show rf frequency response when calibrating tx filter
#define WM_CSE_CAL_RX_FILTER_IFFR_SHOW		(WM_USER + 74)		//show if frequency response when calibrating tx filter
#define WM_CSE_CAL_RX_FILTER_80_SHOW		(WM_USER + 75)		//show bw 80MHz coef when calibrating tx filter
#define WM_CSE_CAL_RX_FILTER_160_SHOW		(WM_USER + 76)		//show bw 160Mhz coef when calibrating tx filter
#define WM_CSE_CAL_RX_FILTER_OFF_80_SHOW	(WM_USER + 79)		//show data when calibrating rx filter 80M offset
#define WM_CSE_TEST_SB_STABILITY_SN_SHOW	(WM_USER + 100)		//show single board test : read sn data
#define WM_CSE_TEST_SB_STABILITY_LO_SHOW	(WM_USER + 101)		//show single board test : lo lock data
#define WM_CSE_TEST_CMPB_SHOW				(WM_USER + 102)		//show complete machine program bit test data
#define WM_CSE_TEST_X9119_SHOW				(WM_USER + 103)		//show x9119 test output frequency
#define WM_CSE_TEST_TX_FR_SHOW				(WM_USER + 104)		//show tx frequency response test data
#define WM_CSE_TEST_RX_FR_SHOW				(WM_USER + 105)		//show rx frequency response test data
#define WM_CSE_TEST_TS_SHOW					(WM_USER + 106)		//show t/rx temp stability test data
#define WM_CSE_TEST_ALGORITHM_VCO5355_SHOW	(WM_USER + 108)		//show vco 5355 frequency algorithm test data
#define WM_CSE_TEST_BTR_SHOW				(WM_USER + 109)		//show blower vs rf temperature relation test data
#define WM_CSE_TEST_TTB_SHOW				(WM_USER + 110)		//show base power vs rf temperature relation test data

#define IQ_MAX 10000.0
#define IQ_MIN -10000.0
#define POWER_MIN -180.0
#define POWER_MAX 10.0

#define MAIN_LOG_DLG_HEIGHT_RATIO	5
#define MAIN_DLG_INITIAL_HEIGHT		450
#define MAIN_DLG_INITIAL_WIDTH		1050
#define PROGRESS_HEIGHT				23
#define REG_LIST_WIDTH				425

#define IQ_CAPTURE_CHART_RECT	CRect(240,5,810,384)	//iq capture dialog's chart size
#define CAL_CHART_RECT			CRect(159,67,804,386)	//calibration dialog's chart size
#define TEST_CHART_RECT			CRect(159,67,804,386)	//test dialog's chart size

#define SAFE_DESTROY_DLG(ptr_dlg)	\
	if (ptr_dlg) {					\
		ptr_dlg->DestroyWindow();	\
		delete ptr_dlg;				\
		ptr_dlg = NULL;				\
	}

int32_t TimeDomain(ISP1401 *pSP1401,uint32_t uiSamples,double *pX,int16_t *pI,int16_t *pQ);						//supported in another thread
int32_t TimeDomain(ISP1401 *pSP1401,uint32_t uiSamples,CChartXYSerie *pILineSerie,CChartXYSerie *pQLineSerie);	//not supported in another thread
int32_t TimeDomain(uint32_t uiSamples,const ArbReader::Data_t *pData,CChartXYSerie *pILineSerie,CChartXYSerie *pQLineSerie);
int32_t DFT(ISP1401 *pSP1401,int32_t iSampleRate,uint32_t uiSamples,int32_t *pPts,double *pX,double *pOut);		//supported in another thread
int32_t DFT(ISP1401 *pSP1401,int32_t iSampleRate,uint32_t uiSamples,CChartXYSerie *pLineSerie);					//not supported in another thread
int32_t DFT(int32_t iSampleRate,uint32_t uiSamples,const ArbReader::Data_t *pData,CChartXYSerie *pLineSerie);

int32_t ExeFirProcess(char *pPath);

int32_t freq2str(double	  freq,CString &strFreq,int32_t digit = 0);
int32_t freq2str(float	  freq,CString &strFreq,int32_t digit = 0);
int32_t freq2str(uint64_t freq,CString &strFreq,int32_t digit = 0);
int32_t freq2str(int32_t  freq,CString &strFreq,int32_t digit = 0);
int32_t freq2str(int64_t  freq,CString &strFreq,int32_t digit = 0);

int32_t coef2str(int16_t *pCoef,int32_t iOrder,CString &strBuf);

int32_t freq2array(CString pin1,CString pin2,CString step,int64_t *pout = NULL,int64_t *pfreqstar = NULL,int64_t *pfreqstop = NULL,int64_t *pmax = NULL,int64_t *pmin = NULL);

int32_t tm2str(tm t,CString &strTime);

class CCSEApp : public CWinApp
{
public:
	CCSEApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CCSEApp theApp;
extern CMainDlg *g_pMainDlg;
extern CADF5355Dlg *g_pADF5355Dlg;
extern CHMC1197Dlg *g_pHMC1197Dlg;
extern CLogDlg		 *g_pLogDlg;
extern CLogViewerDlg *g_pLogViewerDlg;

extern CTestCMPBDlg		*g_pTestCMPBDlg;
extern CTestAlgo5355Dlg *g_pTestAlgo5355Dlg;
extern CTestFittingInterpDlg *g_pTestFittingInterpDlg;

#define ALL_DLG g_pMainDlg->m_pTabCtrl