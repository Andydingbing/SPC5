#ifndef FTP_H
#define FTP_H

#include <string>

namespace sp_rd {

class ftp
{
public:
    ftp(const std::string &host,const std::string &usr,const std::string &pwd) :
        _host(host), _usr(usr), _pwd(pwd) {}
    bool is_remote_available();
    bool put(const std::string &local,const std::string &remote,const std::string &name);
    bool get(const std::string &local,const std::string &remote,const std::string &name);

private:
    std::string _host;
    std::string _usr;
    std::string _pwd;
};

} // namespace sp_rd

#endif // FTP_H
