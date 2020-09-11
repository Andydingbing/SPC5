#ifndef RD_COMMON_CAL_FILE_HPP
#define RD_COMMON_CAL_FILE_HPP

#include "cal_table.hpp"
#include <list>
#include <map>
#include <boost/noncopyable.hpp>

namespace rd {

struct cal_file_header_t {
    // Encryption
    uint64_t encryption;
    uint32_t hash[256];

    uint32_t ver;        // Calibration file version
    uint32_t ver_driver; // Driver version
    uint32_t ver_tool;   // Calibration tool version
    uint32_t rsv0;
    char     sn[64];     // Hardware SN
    uint32_t tables;
    uint32_t rsv1;
    uint32_t rsv2[4];

    cal_file_header_t() { memset(this,0,sizeof(cal_file_header_t)); }
};

struct cal_table_header_t {
    int32_t  id;         // <cal_table_t>
    uint32_t pts;        // Total points
    uint32_t per_size;   // Size(bype) of each <data_f_t> type point
    uint32_t rsv0;

    cal_table_header_t() : id(-1),pts(0),per_size(0),rsv0(0) {}
};

template<typename cal_table_t>
class basic_cal_file : boost::noncopyable
{
public:
    basic_cal_file() {}
    basic_cal_file(const std::string &path) { _path = path; }

    uint32_t version_current() const { return ver_current; }
    void set_version_current(const uint32_t ver) { ver_current = ver; }

    friend class cfv_ctrl;

public:
    int32_t make_sure_has(const cal_table_t table,cal_table *t);

    int32_t add(const cal_table_t table,void *data);
    int32_t persistence(const cal_table_t table) const;

protected:
    virtual int32_t open();

    int32_t slice_and_move(uint32_t pos,uint32_t offset) const;
    void show_header(const cal_file_header_t &head);

private:
    bool pre_check();

    int32_t create();
    int32_t load();
    int32_t table_pos_size(const cal_table_t table,uint32_t &pos,uint32_t &size) const;

    int32_t get_header(cal_file_header_t &header) const;
    int32_t get_header(cal_table_header_t &table_header) const;
    int32_t get_header(cal_file_header_t &header,std::list<cal_table_header_t> &table_headers) const;

    int32_t set_header(const cal_file_header_t &header) const
    { return w_from_pos(0,sizeof(cal_file_header_t),&header); }
    int32_t set_header(const cal_table_header_t &table_header) const;
    int32_t set_header(const cal_file_header_t &header,const std::list<cal_table_header_t> &table_headers) const;

    uint32_t get_version() const;

    int32_t w_from_pos(uint32_t pos,uint32_t size,void *data) const;
    int32_t r_from_pos(uint32_t pos,uint32_t size,void *data) const;

private:
    std::string _path;
    std::map<int32_t,cal_table *> _tables;

protected:
    static uint32_t ver_current;
};

} // namespace rd

#endif // CAL_FILE_H
