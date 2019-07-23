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
        int32_t  result;
        uint32_t addr;
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
        RD_LOG_ALL_OFF      = 0x00000000,
        RD_LOG_PROMPT       = 0x00000001,
        RD_LOG_MESSAGE      = 0x00000004,
        RD_LOG_MESSAGE_F    = 0x00000008,
        RD_LOG_REG          = 0x00000010,
        RD_LOG_REG_F        = 0x00000020,
        RD_LOG_TRACE        = 0x00000040,
        RD_LOG_ALL_ON       = 0x7FFFFFFF
    };

public:
    log_t();
    virtual ~log_t();
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
    msg_log_vector *msgs();
    reg_log_vector *regs();
    bool is_en(log_sw_t);
    void en(log_sw_t,bool);
    void set_default();
    void set_last_err(const char *format,...);
    void set_last_err(int, const char *format,...);
    char *last_err();

    virtual void stdprintf(const char *fmt,...);

    virtual int add_msg(const char *fmt,...);
    virtual int add_msg(int32_t result,const char *fmt,...);
    virtual int add_reg(int32_t result,const std::string &fpga,uint32_t addr,uint32_t w = 0xFFFFFFFF,uint32_t r = 0xFFFFFFFF);
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
