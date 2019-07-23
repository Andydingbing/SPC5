#include "ftp.h"
#include "rd.h"
#include "ftplib.h"

using namespace std;
using namespace sp_rd;

#define STATUS_CHK(func) \
    if ( (ret = func) != 1) return false;

bool ftp::is_remote_available()
{
    int ret = 0;
    netbuf *ctrl = nullptr;

    FtpInit();
    STATUS_CHK(FtpConnect(_host.c_str(),&ctrl));
    STATUS_CHK(FtpClose(ctrl));
    return true;
}

bool ftp::put(const string &local,const string &remote,const string &name)
{
    int ret = 0;
    netbuf *ctrl = nullptr;

    FtpInit();
    STATUS_CHK(FtpConnect(_host.c_str(),&ctrl));
    STATUS_CHK(FtpOptions(FTPLIB_CONNMODE,FTPLIB_PASSIVE,ctrl));
    STATUS_CHK(FtpLogin(_usr.c_str(),_pwd.c_str(),ctrl));
    STATUS_CHK(FtpChdir(remote.c_str(),ctrl));
    STATUS_CHK(FtpPut(local.c_str(),name.c_str(),FTPLIB_IMAGE,ctrl));
    STATUS_CHK(FtpClose(ctrl));
    return true;
}

bool ftp::get(const string &local,const string &remote,const string &name)
{
    int ret = 0;
    netbuf *ctrl = nullptr;

    FtpInit();
    STATUS_CHK(FtpConnect(_host.c_str(),&ctrl));
    STATUS_CHK(FtpClose(ctrl));
    return true;
}
