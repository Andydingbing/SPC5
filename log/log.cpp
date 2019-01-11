#include "log.h"
#include <iostream>
#include <cstdarg> // Bring in va
#include <boost/format.hpp>

using namespace sp_rd;

#ifdef RD_PLATFORM_WIN32
    #define MSG_LOG_PATH "C://msg.log"
    #define REG_LOG_PATH "C://reg.log"

	#include "pthread.h"
	#include <WinDef.h>
	#include <WinBase.h>
    #include <WinCon.h>
#elif defined RD_PLATFORM_LINUX
    #define MSG_LOG_PATH "/var/log/spc5/msg.log"
    #define REG_LOG_PATH "/var/log/spc5/reg.log"
    #include <sys/time.h>
    #include <pthread.h>
#endif

#ifdef RD_C_MSC
    #pragma comment(lib,"pthreadVC2.lib")
    #pragma data_seg("LogShareData")
        boost::shared_ptr<log_t> g_log(new log_t);
//        static log_t g_log /*= log_t()*/;
    #pragma data_seg()
    #pragma comment(linker,"/section:LogShareData,rws")
#elif defined RD_C_GNUC
    #define TRACE(arg)
    static log_t *g_log __attribute__((section("LogShareData"))) /*= &log_t()*/;
#endif

pthread_mutex_t g_log_lock = PTHREAD_MUTEX_INITIALIZER;

log_t::msg_log_t::msg_log_t() :
    result(0),
    msg("\0"),
    time("\0")
{
}

log_t::reg_log_t::reg_log_t() :
    addr(0xffff),
    result(0),
    w(0xffffffff),
    r(0xffffffff),
    fpga("\0"),
    time("\0")
{
}

log_t::log_t() :
    m_msg_log(NULL),
    m_reg_log(NULL)
{
    m_tstar = boost::make_shared<log_t::ptime>(boost::posix_time::microsec_clock::local_time());
    m_tcurr = boost::make_shared<log_t::ptime>(*m_tstar);

#ifdef RD_PLATFORM_WIN32
    m_console = NULL;
#endif

#ifdef GUI_MFC
    m_wnd = NULL;
#else
    m_msg_callback = NULL;
    m_reg_callback = NULL;
#endif

    m_fp_msg = fopen(MSG_LOG_PATH,"a");
    m_fp_reg = fopen(REG_LOG_PATH,"a");

#ifdef _DEBUG
    m_sw = l_msg | l_prompt | l_trace;
#else
    m_sw = l_msg;
#endif
}

log_t::~log_t()
{
    SAFE_CLOSE_FP(m_fp_msg);
    SAFE_CLOSE_FP(m_fp_reg);

#ifdef GUI_MFC
    SAFE_DELETE_VECTOR(m_wnd);
#endif

#ifdef RD_PLATFORM_WIN32
    if (m_console)
		FreeConsole();
#endif
}

log_t &log_t::instance()
{
    return *(g_log.get());
}

void log_t::init()
{
    if (!m_msg_log)
        m_msg_log = boost::make_shared<msg_log_vector>();
    if (!m_reg_log)
        m_reg_log = boost::make_shared<reg_log_vector>();
}

#ifdef GUI_MFC
void log_t::init(HWND wnd)
{
    SAFE_DELETE_VECTOR(m_wnd);
    m_wnd = new std::vector<HWND>;
    m_wnd->push_back(wnd);
    init();
}

void log_t::init(std::vector<HWND> wnd)
{
    std::vector<HWND>::iterator iter;

    SAFE_DELETE_VECTOR(m_wnd)
    m_wnd = new std::vector<HWND>;
    for (iter = wnd.begin();iter != wnd.end();iter ++)
        m_wnd->push_back(*iter);
    init();
}
#else
void log_t::init(void (*msg_callback)(),void (*reg_callback)())
{
    m_msg_callback = msg_callback;
    m_reg_callback = reg_callback;
    init();
}
#endif

log_t::msg_log_vector *log_t::get_msg_log()
{
    return m_msg_log.get();
}

log_t::reg_log_vector *log_t::get_reg_log()
{
    return m_reg_log.get();
}

void log_t::set(uint64_t sw)
{
    m_sw = sw;
}

uint64_t log_t::get()
{
    return m_sw;
}

bool log_t::is_enabled(log_t::log_sw_t sw)
{
    if (l_all_on == sw)
        return l_all_on == m_sw ? true : false;
    if (l_all_off == sw)
        return l_all_off == m_sw ? true : false;
    return (m_sw & sw) ? true : false;
}

void log_t::set_enalbe(log_t::log_sw_t sw,bool en)
{
    if (en)
        m_sw |= (uint64_t)sw;
	else
        m_sw &= (~(uint64_t)sw);
}

void log_t::set_default()
{
#ifdef _DEBUG
    m_sw = l_msg | l_prompt | l_trace;
#else
    m_sw = l_msg;
#endif
}

boost::posix_time::time_duration log_t::time_elapsed()
{
    *m_tcurr = boost::posix_time::microsec_clock::local_time();
    return *m_tcurr - *m_tstar;
}

void log_t::stdprintf(const char *fmt,...)
{	
    if (!(m_sw & l_prompt))
		return;

    std::string buf = to_simple_string(time_elapsed());
    buf += " ";
    char fmt_buf[512] = "";
    buf.copy(fmt_buf,buf.length(),0);

	va_list ap;
    va_start(ap,fmt);
    vsprintf(&fmt_buf[strlen(fmt_buf)],fmt,ap);
	va_end(ap);
    buf = fmt_buf;

#ifdef RD_PLATFORM_WIN32
    if (!m_console) {
		AllocConsole();
        m_console = GetStdHandle(STD_OUTPUT_HANDLE);
	}
    WriteConsoleA(m_console,buf.c_str(),DWORD(buf.length()),NULL,NULL);
#elif defined RD_PLATFORM_LINUX
    printf("%s",buf);
#endif
}

void log_t::set_log_time(log_t::msg_log_t *lg)
{
    *m_tcurr = boost::posix_time::microsec_clock::local_time();
    std::tm tm_time = to_tm(*m_tcurr);

    boost::format tf("%4d/%02d/%02d %02d:%02d:%02d");
    tf % (tm_time.tm_year + 1900);
    tf % tm_time.tm_mon;
    tf % tm_time.tm_mday;
    tf % tm_time.tm_hour;
    tf % tm_time.tm_min;
    tf % tm_time.tm_sec;

    lg->time = tf.str();
}

void log_t::set_log_time(log_t::reg_log_t *lg)
{
    *m_tcurr = boost::posix_time::microsec_clock::local_time();
    std::tm tm_time = to_tm(*m_tcurr);

    boost::format tf("%02d:%02d:%02d");
    tf % tm_time.tm_hour;
    tf % tm_time.tm_min;
    tf % tm_time.tm_sec;

    lg->time = tf.str();
}

int log_t::add_msg_list(const char *fmt,...)
{
    if ((!(m_sw & l_msg)) && (!(m_sw & l_msg_f)))
		return 0;

    pthread_mutex_lock(&g_log_lock);

    msg_log_t msg;

    set_log_time(&msg);

    char msg_buf[512] = "";
    va_list ap;
    va_start(ap,fmt);
    vsprintf(msg_buf,fmt,ap);
    va_end(ap);
    msg.msg = msg_buf;

    if (m_sw & l_msg_f) {
        fprintf(m_fp_msg,"%s %50s %#010x\n",msg.time,msg.msg,msg.result);
        fflush(m_fp_msg);
	}

    if ((m_sw & l_msg) && (NULL != m_msg_log)) {
        m_msg_log->push_back(msg);

#if defined GUI_MFC
    if (m_wnd) {
        vector<HWND>::iterator iter;
        for (iter = m_wnd->begin();iter != m_wnd->end();iter ++)
            ::PostMessage(*iter,WM_MSG_LOG,0,0);
    }
#else
    if (m_msg_callback)
        m_msg_callback();
#endif

	}
    pthread_mutex_unlock(&g_log_lock);
	return 0;
}

int log_t::add_msg_list(int result, const char *fmt,...)
{
    if ((!(m_sw & l_msg)) && (!(m_sw & l_msg_f)))
		return 0;

    pthread_mutex_lock(&g_log_lock);

    msg_log_t msg;

    set_log_time(&msg);

    char msg_buf[512] = "";
    va_list ap;
    va_start(ap,fmt);
    vsprintf(msg_buf,fmt,ap);
    va_end(ap);
    msg.msg = msg_buf;
    msg.result = result;

    if (m_sw & l_msg_f) {
        fprintf(m_fp_msg,"%s %50s %#010x\n",msg.time,msg.msg,msg.result);
        fflush(m_fp_msg);
	}

    if ((m_sw & l_msg) && (NULL != m_msg_log)) {
        m_msg_log->push_back(msg);

#if defined GUI_MFC
    if (m_wnd) {
        vector<HWND>::iterator iter;
        for (iter = m_wnd->begin();iter != m_wnd->end();iter ++)
            ::PostMessage(*iter,WM_MSG_LOG,0,0);
    }
#else
    if (m_msg_callback)
        m_msg_callback();
#endif

	}
    pthread_mutex_unlock(&g_log_lock);
    return result;
}

int log_t::add_reg_list(int result,char *fpga,int addr,unsigned w,unsigned r)
{
    if ((!(m_sw & l_reg)) && (!(m_sw & l_reg_f)))
		return 0;

    pthread_mutex_lock(&g_log_lock);

    reg_log_t reg;

    set_log_time(&reg);
    reg.result = result;
    reg.addr = addr;
    reg.w = w;
    reg.r = r;
    reg.fpga = fpga;

    if (w != 0xffffffff) {
        if (m_sw & l_reg_f) {
            fprintf(m_fp_reg,"%s	%4s  %#06x  %#010x  %16s  %d\n",
                    reg.time,
                    reg.fpga,
                    reg.addr,
                    reg.w,
                    "",
                    reg.result);
            fflush(m_fp_reg);
		}
	}
    if (r != 0xffffffff) {
        if (m_sw & l_reg_f) {
            fprintf(m_fp_reg,"%s	%4s  %#06x  %16s  %#010x  %d\n",
                    reg.time,
                    reg.fpga,
                    reg.addr,
                    "",
                    reg.r,
                    reg.result);
            fflush(m_fp_reg);
		}
	}

    if ((m_sw & l_reg) && (NULL != m_reg_log)) {
        m_reg_log->push_back(reg);

#if defined GUI_MFC
        if (m_wnd) {
			vector<HWND>::iterator iter;
            for (iter = m_wnd->begin();iter != m_wnd->end();iter ++)
                ::PostMessage(*iter,WM_REG_LOG,0,0);
		}
#else
        if (m_reg_callback)
            m_reg_callback();
#endif

	}
    pthread_mutex_unlock(&g_log_lock);
    return result;
}

void log_t::set_last_err(const char *format,...)
{
    memset(m_last_err,0,sizeof(m_last_err));

    va_list ap;
    va_start(ap,format);
    vsprintf(m_last_err,format,ap);
	va_end(ap);
    add_msg_list(-1,m_last_err);
}

void log_t::set_last_err(int result,const char *format,...)
{
    memset(m_last_err,0,sizeof(m_last_err));

    va_list ap;
    va_start(ap,format);
    vsprintf(m_last_err,format,ap);
	va_end(ap);
    add_msg_list(result,m_last_err);
}

char *log_t::get_last_err()
{
    return m_last_err;
}
