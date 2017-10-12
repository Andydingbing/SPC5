#pragma once

class CTestCMPBThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestCMPBThread)
public:
	typedef struct Info {
		typedef struct Rfu {
			uint32_t m_uiIdx;
			CString m_strS6Rsrc;
			CString m_strPath[2];
			BOOL m_bProgram[2];
			BOOL m_bConnect[2];		//k7_0/1
			Rfu();
		}Rfu;
		typedef struct Ceu {
			uint32_t m_uiIdx;
			CString m_strS6Rsrc;
			CString m_strPath[4];
			BOOL m_bProgram[4];
			BOOL m_bConnect[4];		//k7_0/1/2/3
			Ceu();
		}Ceu;
		typedef struct Cxu {
			CString m_strS6Rsrc;
			CString m_strPath;
			BOOL m_bProgram;
			BOOL m_bConnect;		//v7
			Cxu();
		}Cxu;
		vector<Rfu> m_Rfu;
		vector<Ceu> m_Ceu;
		vector<Cxu> m_Cxu;
		uint32_t m_uiDelayms;
		uint32_t m_uiOneBlockSize;
		uint32_t m_uiTimes;
	public:
		Info() {
			m_Rfu.clear();
			m_Ceu.clear();
			m_Cxu.clear();
		}
	}Info;

	typedef struct Result {
		typedef struct Rfu {
			uint32_t m_uiTimes[2];
			uint32_t m_uiFails[2];
			uint32_t m_uiVer[2];
			BOOL m_bConnect[2];
			Rfu();
		}Rfu;
		typedef struct Ceu {
			uint32_t m_uiTimes[4];
			uint32_t m_uiFails[4];
			uint32_t m_uiVer[4];
			BOOL m_bConnect[4];
			Ceu();
		}Ceu;
		typedef struct Cxu {
			uint32_t m_uiTimes;
			uint32_t m_uiFails;
			uint32_t m_uiVer;
			BOOL m_bConnect;
			Cxu();
		}Cxu;
		vector<Rfu> m_Rfu;
		vector<Ceu> m_Ceu;
		vector<Cxu> m_Cxu;
	public:
		Result() {
			m_Rfu.clear();
			m_Ceu.clear();
			m_Cxu.clear();
		}
	}Result;
protected:
	CTestCMPBThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestCMPBThread();
public:
	CTestCMPBThread(CWnd *pParent,Info *pInfo);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
private:
	int32_t ProgramRfuK7(int32_t iK7Index,CString strS6Rsrc,CString strPath);
	int32_t ProgramCeuK7(int32_t iK7Index,CString strS6Rsrc,CString strPath);
	int32_t ProgramCxuV7(CString strS6Rsrc,CString strPath);
private:
	Info m_Info;
};


