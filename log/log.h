#ifndef LOG_H
#define LOG_H

#include "rd.h"
#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>

#if defined(RD_PLATFORM_WIN32) && defined (GUI_MFC)
    #include <WinUser.h>

    #define WM_MSG_LOG (WM_USER + 1000)
    #define WM_REG_LOG (WM_USER + 1001)
#endif

#define Log log_t::instance()

namespace sp_rd {

class RD_API log_t : boost::noncopyable
{
public:
    struct RD_API msg_log_t {
        int32_t result;
        std::string msg;
        std::string time;
    public:
        msg_log_t();
    };

    struct RD_API reg_log_t {
        int32_t  addr;
        int16_t  result;
        uint32_t w;
        uint32_t r;
        std::string fpga;
        std::string time;
    public:
        reg_log_t();
    };

public:
    typedef std::vector<msg_log_t> msg_log_vector;
    typedef std::vector<reg_log_t> reg_log_vector;

    typedef boost::posix_time::ptime ptime;
    typedef boost::shared_ptr<ptime> ptime_sptr;
    typedef boost::shared_ptr<msg_log_vector> msg_log_vector_sptr;
    typedef boost::shared_ptr<reg_log_vector> reg_log_vector_sptr;

    enum log_sw_t {
        l_all_off	= 0x0000000000000000,
        l_prompt	= 0x0000000000000001,
        l_msg		= 0x0000000000000004,
        l_msg_f     = 0x0000000000000008,
        l_reg		= 0x0000000000000010,
        l_reg_f     = 0x0000000000000020,
        l_trace     = 0x0000000000000040,
        l_all_on	= 0x00000000ffffffff
    };

public:
    log_t();
    ~log_t();
    static log_t &instance();
    void init();

#ifdef GUI_MFC
    void init(HWND);
    void init(std::vector<HWND>);
private:
    std::vector<HWND> *m_wnd;
#else
    void init(void (*msg_callback)(),void (*reg_callback)());
private:
    void (*m_msg_callback)();
    void (*m_reg_callback)();
#endif

public:
    msg_log_vector *get_msg_log();
    reg_log_vector *get_reg_log();
    void set(uint64_t);
    uint64_t get();
    bool is_enabled(log_sw_t);
    void set_enalbe(log_sw_t,bool);
    void set_default();
    void set_last_err(const char *format,...);
    void set_last_err(int, const char *format,...);
    char *get_last_err();

    virtual void stdprintf(const char *fmt,...);

    virtual int add_msg_list(const char *fmt,...);
//    virtual int add_msg_list(const string &str);
    virtual int add_msg_list(int result,const char *fmt,...);
    virtual int add_reg_list(int result,char *fpga,int addr,unsigned w = 0xffffffff,unsigned r = 0xffffffff);
public:
    boost::posix_time::time_duration time_elapsed();
private:
    void set_log_time(msg_log_t *lg);
    void set_log_time(reg_log_t *lg);
private:
    msg_log_vector_sptr m_msg_log;
    reg_log_vector_sptr m_reg_log;
    ptime_sptr m_tstar;
    ptime_sptr m_tcurr;
    FILE *m_fp_msg;
    FILE *m_fp_reg;
    char m_last_err[512];
    uint64_t m_sw;
#ifdef RD_PLATFORM_WIN32
    void *m_console;
#endif
};

} //namespace sp_rd

#endif // LOG_H
