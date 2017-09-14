#include "Log.h"
#include <time.h>
#include "pthread.h"

#ifdef _WIN64
#include <wincon.h>
#endif

#define MSG_LOG_PATH "C://logmsg.log"
#define REG_LOG_PATH "C://logreg.log"

#ifdef _MSC_VER
#include <afx.h>
#pragma comment(lib,"pthreadVC2.lib")
#pragma data_seg("LogShareData")
    CLog *g_pLog = new CLog();
#pragma data_seg()
#pragma comment(linker,"/section:LogShareData,rws")
#elif defined __GNUC__
#define TRACE(arg)
    CLog *g_pLog __attribute__((section("LogShareData"))) = new CLog();
#endif

pthread_mutex_t g_LogLock = PTHREAD_MUTEX_INITIALIZER;

LARGE_INTEGER g_TSFreq;
LARGE_INTEGER g_TSFirstCount;

MsgLog::MsgLog()
{
	m_iResult = 0;
	memset(m_szTime,0,sizeof(m_szTime));
	memset(m_szMsg,0,sizeof(m_szMsg));
}

RegLog::RegLog()
{
    m_iAddr = 0xffff;
	m_iResult = 0;
	memset(m_szTime,0,sizeof(m_szTime));
	memset(m_szFpga,0,sizeof(m_szFpga));
	m_uiWValue = 0xffffffff;
	m_uiRValue = 0xffffffff;
}

CLog::CLog()
{
	m_phWnd = NULL;
	m_pMsgLog = NULL;
	m_pRegLog = NULL;
    m_pConsole = NULL;
	m_fpMsg = fopen(MSG_LOG_PATH,"a");
	m_fpReg = fopen(REG_LOG_PATH,"a");
	m_bOwnData = false;
	QueryPerformanceFrequency(&g_TSFreq);
	QueryPerformanceCounter(&g_TSFirstCount);
#ifdef _DEBUG
	m_uiSw = L_MSG | L_PROMPT | L_TRACE;
#else
	m_uiSw = L_MSG;
#endif
}

CLog::~CLog()
{
	if (m_fpMsg) {
		fclose(m_fpMsg);
		m_fpMsg = NULL;
	}
	if (m_fpReg) {
		fclose(m_fpReg);
		m_fpReg = NULL;
	}
	if (m_phWnd) {
		m_phWnd->clear();
		delete m_phWnd;
		m_phWnd = NULL;
	}
	if (m_bOwnData) {
		if (m_pMsgLog) {
			m_pMsgLog->clear();
			delete m_pMsgLog;
			m_pMsgLog = NULL;
		}
		if (m_pRegLog) {
			m_pRegLog->clear();
			delete m_pRegLog;
			m_pRegLog = NULL;
		}
	}
	if (m_pConsole)
		FreeConsole();
}

CLog *CLog::Instance()
{
    return g_pLog;
}

void CLog::Init()
{
	m_bOwnData = true;
	if (m_pMsgLog) {
		m_pMsgLog->clear();
		delete m_pMsgLog;
		m_pMsgLog = NULL;
	}
	if (m_pRegLog) {
		m_pRegLog->clear();
		delete m_pRegLog;
		m_pRegLog = NULL;
	}
	m_pMsgLog = new vector<MsgLog>;
	m_pRegLog = new vector<RegLog>;
}

void CLog::Init(HWND hWnd)
{
	if (m_phWnd) {
		m_phWnd->clear();
		delete m_phWnd;
		m_phWnd = NULL;
	}
	m_phWnd = new vector<HWND>;
	m_phWnd->push_back(hWnd);
	Init();
}

void CLog::Init(vector<HWND> hWnd)
{
	vector<HWND>::iterator iter;

	if (m_phWnd) {
		m_phWnd->clear();
		delete m_phWnd;
		m_phWnd = NULL;
	}
	m_phWnd = new vector<HWND>;
	for (iter = hWnd.begin();iter != hWnd.end();iter ++)
		m_phWnd->push_back(*iter);
	Init();
}

void CLog::Init(vector<MsgLog> *pMsgLog,vector<RegLog> *pRegLog)
{
	m_bOwnData = false;
	m_pMsgLog = pMsgLog;
	m_pRegLog = pRegLog;
}

void CLog::Init(HWND hWnd,vector<MsgLog> *pMsgLog,vector<RegLog> *pRegLog)
{
	if (m_phWnd) {
		m_phWnd->clear();
		delete m_phWnd;
		m_phWnd = NULL;
	}
	m_phWnd = new vector<HWND>;
	m_phWnd->push_back(hWnd);
	Init(pMsgLog,pRegLog);
}

void CLog::Init(vector<HWND> hWnd,vector<MsgLog> *pMsgLog,vector<RegLog> *pRegLog)
{
	vector<HWND>::iterator iter;
	if (m_phWnd) {
		m_phWnd->clear();
		delete m_phWnd;
		m_phWnd = NULL;
	}
	m_phWnd = new vector<HWND>;
	for (iter = hWnd.begin();iter != hWnd.end();iter ++)
		m_phWnd->push_back(*iter);
	Init(pMsgLog,pRegLog);
}

vector<MsgLog> *CLog::GetMsgLog()
{
	return m_pMsgLog;
}

vector<RegLog> *CLog::GetRegLog()
{
	return m_pRegLog;
}

void CLog::Set(uint64_t uiLogSw)
{
	m_uiSw = uiLogSw;
}

uint64_t CLog::Get()
{
	return m_uiSw;
}

bool CLog::IsEnabled(LogSw Sw)
{
	if (L_ALL_ON == Sw)
		return L_ALL_ON == m_uiSw ? true : false;
	if (L_ALL_OFF == Sw)
		return L_ALL_OFF == m_uiSw ? true : false;
	return (m_uiSw & Sw) ? true : false;
}

void CLog::SetEnable(LogSw Sw,bool bEn)
{
	if (bEn)
		m_uiSw |= (uint64_t)Sw;
	else
		m_uiSw &= (~(uint64_t)Sw);
}

void CLog::SetDefault()
{
#ifdef _DEBUG
	m_uiSw = L_MSG | L_PROMPT | L_TRACE;
#else
	m_uiSw = L_MSG;
#endif
}

double CLog::GetTimeStamp() 
{
	LARGE_INTEGER ll;
	QueryPerformanceCounter(&ll);
	return ((double)ll.QuadPart - (double)g_TSFirstCount.QuadPart) / (double)g_TSFreq.QuadPart;
}

void CLog::stdprintf(const char *format,...)
{	
	if (0 == (m_uiSw & L_PROMPT))
		return;

	char szBuf[512];
	double dTime = GetTimeStamp();

	memset(szBuf,0,512);
    sprintf(szBuf,"%10.3f s  ",dTime);

	va_list ap;
    va_start(ap,format);
    vsprintf(&szBuf[strlen(szBuf)],format,ap);
	va_end(ap);

	if (!m_pConsole) {
		AllocConsole();
		m_pConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	WriteConsole(m_pConsole,szBuf,(DWORD)strlen(szBuf),NULL,NULL);
}

void CLog::printf(const char *format,...)
{
	char szBuf[512];
	memset(szBuf,0,512);

	va_list ap;
    va_start(ap,format);
    vsprintf(szBuf,format,ap);
	va_end(ap);

	stdprintf(szBuf);
}

void CLog::trace(const char *format,...)
{
#ifdef _DEBUG
	if (0 == (m_uiSw & L_TRACE))
		return;

	char szBuf[512];
	memset(szBuf,0,512);

	va_list ap;
    va_start(ap,format);
    vsprintf(szBuf,format,ap);
	va_end(ap);

	TRACE(szBuf);
#endif
}

int CLog::AddMsgList(const char *pFormat,...)
{
	if ((0 == (m_uiSw & L_MSG)) && (0 == (m_uiSw & L_MSG_F)))
		return 0;

	pthread_mutex_lock(&g_LogLock);

	MsgLog MsgL;
    time_t Time;
    tm *LT;
    time(&Time);
    LT = localtime(&Time);
    sprintf(MsgL.m_szTime,"%4d/%02d/%02d %02d:%02d:%02d",LT->tm_year + 1900,LT->tm_mon,LT->tm_mday,LT->tm_hour,LT->tm_min,LT->tm_sec);
	MsgL.m_iResult = 0;

	va_list va_msg;
    va_start(va_msg,pFormat);
    vsprintf(MsgL.m_szMsg,pFormat,va_msg);
	va_end(va_msg);

	if (m_uiSw & L_MSG_F) {
        fprintf(m_fpMsg,"%s %50s %#010x\n",MsgL.m_szTime,MsgL.m_szMsg,MsgL.m_iResult);
		fflush(m_fpMsg);
	}

	if ((m_uiSw & L_MSG) && (NULL != m_pMsgLog)) {
		m_pMsgLog->push_back(MsgL);
		if (m_phWnd) {
			vector<HWND>::iterator iter;
			for (iter = m_phWnd->begin();iter != m_phWnd->end();iter ++)
                ::PostMessage(*iter,WM_CSE_MSG_LOG,0,0);
		}
	}
	pthread_mutex_unlock(&g_LogLock);
	return 0;
}

int CLog::AddMsgList(int iResult,const char *pFormat,...)
{
	if ((0 == (m_uiSw & L_MSG)) && (0 == (m_uiSw & L_MSG_F)))
		return 0;

	pthread_mutex_lock(&g_LogLock);

	MsgLog MsgL;
    time_t Time;
    tm *LT;
    time(&Time);
    LT = localtime(&Time);
    sprintf(MsgL.m_szTime,"%4d/%02d/%02d %02d:%02d:%02d",LT->tm_year + 1900,LT->tm_mon,LT->tm_mday,LT->tm_hour,LT->tm_min,LT->tm_sec);
    MsgL.m_iResult = iResult;

	va_list va_msg;
    va_start(va_msg,pFormat);
    vsprintf(MsgL.m_szMsg,pFormat,va_msg);
	va_end(va_msg);

	if (m_uiSw & L_MSG_F) {
        fprintf(m_fpMsg,"%s %50s %#010x\n",MsgL.m_szTime,MsgL.m_szMsg,MsgL.m_iResult);
		fflush(m_fpMsg);
	}

	if ((m_uiSw & L_MSG) && (NULL != m_pMsgLog)) {
		m_pMsgLog->push_back(MsgL);
		if (m_phWnd) {
			vector<HWND>::iterator iter;
			for (iter = m_phWnd->begin();iter != m_phWnd->end();iter ++)
                ::PostMessage(*iter,WM_CSE_MSG_LOG,0,0);
		}
	}
	pthread_mutex_unlock(&g_LogLock);
    return iResult;
}

int CLog::AddRegList(int iResult,char *pFpga,int iAddr,unsigned uiWVvalue,unsigned uiRValue)
{
	if ((0 == (m_uiSw & L_REG)) && (0 == (m_uiSw & L_REG_F)))
		return 0;

	pthread_mutex_lock(&g_LogLock);

	RegLog RegL;
    time_t Time;
    tm *LT;
    time(&Time);
    LT = localtime(&Time);
    sprintf(RegL.m_szTime,"%02d:%02d:%02d",LT->tm_hour,LT->tm_min,LT->tm_sec);
    RegL.m_iResult = iResult;
    RegL.m_iAddr = iAddr;
    RegL.m_uiWValue = uiWVvalue;
    RegL.m_uiRValue = uiRValue;
    strcpy(RegL.m_szFpga,pFpga);

    if (uiWVvalue != 0xffffffff) {
		if (m_uiSw & L_REG_F) {
            fprintf(m_fpReg,"%s	%4s  %#06x  %#010x  %16s  %d\n",RegL.m_szTime,RegL.m_szFpga,RegL.m_iAddr,RegL.m_uiWValue,"",RegL.m_iResult);
			fflush(m_fpReg);
		}
	}
    if (uiRValue != 0xffffffff) {
		if (m_uiSw & L_REG_F) {
            fprintf(m_fpReg,"%s	%4s  %#06x  %16s  %#010x  %d\n",RegL.m_szTime,RegL.m_szFpga,RegL.m_iAddr,"",RegL.m_uiRValue,RegL.m_iResult);
			fflush(m_fpReg);
		}
	}

	if ((m_uiSw & L_REG) && (NULL != m_pRegLog)) {
		m_pRegLog->push_back(RegL);
		if (m_phWnd) {
			vector<HWND>::iterator iter;
			for (iter = m_phWnd->begin();iter != m_phWnd->end();iter ++)
                ::PostMessage(*iter,WM_CSE_REG_LOG,0,0);
		}
	}
	pthread_mutex_unlock(&g_LogLock);
    return iResult;
}

void CLog::SetLastError(const char *pFormat,...)
{
	va_list ap;

    memset(m_szLastError,0,512);

    va_start(ap,pFormat);
    vsprintf(m_szLastError,pFormat,ap);
	va_end(ap);
	AddMsgList(-1,m_szLastError);
}

void CLog::SetLastError(int iResult,const char *pFormat,...)
{
	va_list ap;

    memset(m_szLastError,0,512);

    va_start(ap,pFormat);
    vsprintf(m_szLastError,pFormat,ap);
	va_end(ap);
    AddMsgList(iResult,m_szLastError);
}

char *CLog::GetLastError()
{
	return m_szLastError;
}
