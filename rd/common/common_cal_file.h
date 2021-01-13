#ifndef RD_COMMON_CAL_FILE_H
#define RD_COMMON_CAL_FILE_H

#include "cal_table.hpp"
#include "cal_file_memory.h"
#include "traits.hpp"
#include <list>
#include <map>
#include <fstream>
#include "liblog.h"
#include <boost/current_function.hpp>
#include <boost/noncopyable.hpp>

#define assert_fs(stream) \
    do {if ((stream).bad()) { \
        Log.set_last_err("%s:%s:%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
        return -1; \
        } \
    } while(0)

#define open_to_w(stream) \
    std::ofstream stream(_path,std::ofstream::binary); \
    assert_fs(stream);

#define open_to_r(stream) \
    std::ifstream stream(_path,std::ifstream::binary); \
    assert_fs(stream);

#define open_to_wr(stream) \
    std::fstream stream(_path,std::ios::in | std::ios::out | std::ios::binary); \
    assert_fs(stream);

#define stream_r(dst) \
    assert_fs(stream.read(reinterpret_cast<char *>(&dst),sizeof(dst)));

#define stream_w(src) \
    assert_fs(stream.write(reinterpret_cast<const char *>(&src),sizeof(src)));

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
    int32_t  id;       // <cal_table_t>
    uint32_t pts;      // Total points
    uint32_t per_size; // Size(byte) of each <data_f_t> type point
    uint32_t rsv0;
    char freq_str[512];

    cal_table_header_t() : id(-1),pts(0),per_size(0),rsv0(0) { clr_freq_str(); }
    int32_t set_freq_str(const std::string &str)
    {
        std::size_t length = str.length();

        if (length > ARRAY_SIZE(freq_str) - 1) {
            return -1;
        }

        clr_freq_str();
        str.copy(freq_str,str.length());
        freq_str[length] = '\0';
        return 0;
    }

private:
    void clr_freq_str() { memset(freq_str,0,sizeof(freq_str)); }
};

template<typename cal_table_t>
class basic_cal_file : boost::noncopyable
{
public:
    basic_cal_file() { _ver_current = 0; }
    basic_cal_file(const std::string &path) { _path = path; }

    void set_path(const std::string &path) { _path = path; }
    void set_name(const std::string &name)
    {
        std::string suffix = ".cal";
        size_t suffix_length = suffix.length();

        if(_path.length() > suffix_length &&
           _path.substr(_path.length() - suffix_length,suffix_length) == suffix) {
            return;
        }

        _path += name;
        _path += suffix;
    }

    uint32_t version_current() const { return _ver_current; }
    void set_version_current(const uint32_t ver) { _ver_current = ver; }

    friend class cfv_ctrl;

public:
    int32_t make_sure_has(const cal_table_t table,cal_table *t)
    {
        int32_t data_start_pos = 0;

        cal_file_header_t  header;
        cal_table_header_t table_header;
        std::list<cal_table_header_t> table_headers;
        typename std::list<cal_table_header_t>::const_iterator iter_th;

        table_header.id = table._to_integral();
        table_header.per_size = t->size_of_data_f();

        if ((data_start_pos = get_header(header,table_headers)) < 0) {
            return 0;
        }

        for (iter_th = table_headers.begin();iter_th != table_headers.end();++iter_th) {
            if ((*iter_th).id == table_header.id) {
                break;
            }
        }

        if (iter_th == table_headers.end()) {
            header.tables += 1;
            table_headers.push_back(table_header);

            INT_CHECK(slice_and_move(data_start_pos,sizeof(cal_table_header_t)));
            INT_CHECK(set_header(header,table_headers));
            INT_CHECK(load());
        }

        INT_CHECK(load());
    //    map<int32_t,cal_table *>::const_iterator iter_tables;

    //    if ((iter_tables = _tables.find(table._to_integral())) != _tables.end()) {
    //        if (iter_tables->second != nullptr) {
    //            delete iter_tables->second;
    //        }
    //    }

        _tables[table._to_integral()] = t;
        return 0;
    }

    int32_t prepare_cal(const cal_table_t table)
    {
        std::map<int32_t,cal_table *>::const_iterator iter;

        load_to_buf(table);

        if ((iter = _tables.find(table._to_integral())) != _tables.end()) {
            iter->second->prepare_cal(convert_buf.buf,convert_buf.size_table);
            return 0;
        }
        return -1;
    }

    int32_t prepare_cal(const cal_table_t table,const std::set<uint64_t> &freqs,const bool exp = false)
    {
        std::map<int32_t,cal_table *>::const_iterator iter;

        load_to_buf(table);

        if ((iter = _tables.find(table._to_integral())) != _tables.end()) {
            iter->second->prepare_cal(convert_buf.buf,convert_buf.size_table,&freqs,exp);
            return 0;
        }
        return -1;
    }

    int32_t add(const cal_table_t table,void *data)
    {
        std::map<int32_t,cal_table *>::const_iterator iter;

        if ((iter = _tables.find(table._to_integral())) != _tables.end()) {
            iter->second->add(data);
            return 0;
        }
        return -1;
    }

    template<typename int_t,typename traits_t = traits_int<int_t>>
    int32_t add(const int_t table,void *data)
    { return add(cal_table_t::_from_integral(table),data); }

    int32_t persistence(const cal_table_t table) const
    {
        uint32_t pos = 0;
        uint32_t cur_size = 0;
        uint32_t new_size = 0;
        uint32_t per_size = 0;
        cal_table_header_t table_header;
        cal_table *t = nullptr;
        std::string freq_str;

        set_helper::set_helper_t<int64_t,std::vector<int64_t>> freq;

        std::map<int32_t,cal_table *>::const_iterator iter;

        if ((iter = _tables.find(table._to_integral())) == _tables.end()) {
            return 0;
        }

        INT_CHECK(table_pos_size(table,pos,cur_size));

        t = iter->second;
        t->combine(freq_str);
        per_size = t->size_of_data_f();
        new_size = t->size_data_f() * per_size;
        table_header.id = table._to_integral();

        if (new_size > cur_size) {
            INT_CHECK(get_header(table_header));
            table_header.pts = t->size_data_f();
            table_header.per_size = t->size_of_data_f();
            table_header.set_freq_str(freq_str);
            INT_CHECK(set_header(table_header));
            INT_CHECK(slice_and_move(pos + cur_size,new_size - cur_size));
        }
        for (uint32_t i = 0;i < t->size_data_f();++i) {
            INT_CHECK(w_from_pos(pos,per_size,t->data_f(i)));
            pos += per_size;
        }
        return 0;
    }

    template<typename int_t,typename traits_t = traits_int<int_t>>
    int32_t persistence(const int_t table) const
    { return persistence(cal_table_t::_from_integral(table)); }

protected:
    virtual int32_t open()
    {
        if (!pre_check()) {
            if (create() < 0) { return -1; }
            if (!pre_check()) { return -1; }
        }
        load();
        return 0;
    }

    int32_t slice_and_move(uint32_t pos,uint32_t offset) const
    {
        uint32_t size = 0;
        int32_t left = 0;
        int32_t move_block_size = 0;
        char buf[size_granularity] = {0};

        open_to_wr(stream);
        stream.seekp(0,std::ios::end);
        stream.seekg(0,std::ios::end);
        size = uint32_t(stream.tellp());

        if (pos > size) {
            return 0;
        }

        if (pos == size) {
            left = int32_t(offset);
            while (left > size_granularity) {
                assert_fs(stream.write(buf,size_granularity));
                left -= size_granularity;
            }
            assert_fs(stream.write(buf,left));
            return 0;
        }

        left = int32_t(size - pos);

        while(1) {
            if (left <= 0) {
                break;
            }

            move_block_size = left < size_granularity ? left : size_granularity;
            move_block_size = int32_t(offset) < move_block_size ? int32_t(offset) : move_block_size;

            assert_fs(stream.seekg(move_block_size * -1,std::ios::cur));
            assert_fs(stream.read(buf,move_block_size));
            assert_fs(stream.seekg(move_block_size * -1,std::ios::cur));
            assert_fs(stream.seekg(offset,std::ios::cur));
            assert_fs(stream.write(buf,move_block_size));

            left -= move_block_size;

            assert_fs(stream.seekg(pos + uint32_t(left),std::ios::beg));
        }
        return 0;
    }

    void show_header(const cal_file_header_t &head) {}

private:
    bool pre_check()
    {
        uint32_t ver = get_version();

        if (ver == 0) {
            return false;
        } else if (ver < _ver_current) {
    //        cfv_ctrl ctrl;
    //        ctrl.target(this);
    //        ctrl.for_c(ver);
        }
        return true;
    }

    int32_t create()
    {
        cal_file_header_t header;
        header.ver = 1;

        open_to_w(stream);
        stream_w(header);
        return 0;
    }

    int32_t load()
    {
        cal_file_header_t header;
        cal_table_header_t table_header;

        uint32_t max_size = 0;
        uint32_t cur_table_size = 0;
        std::list<cal_table_header_t> table_headers;
        typename std::list<cal_table_header_t>::iterator iter_th;

        std::map<int32_t,cal_table *>::iterator iter_tables;

        open_to_r(stream);
        stream_r(header);

        for (uint32_t i = 0;i < header.tables;++i) {
            stream_r(table_header);

            cur_table_size = table_header.pts * table_header.per_size;
            if (cur_table_size > max_size) {
                max_size = cur_table_size;
            }
            table_headers.push_back(table_header);
        }

        convert_buf.allocate(max_size);

        for (iter_th = table_headers.begin();iter_th != table_headers.end();++iter_th) {
            cur_table_size = (*iter_th).pts * (*iter_th).per_size;
            assert_fs(stream.read(static_cast<char *>(convert_buf.buf),cur_table_size));
            convert_buf.size_table = cur_table_size;

            if ((iter_tables = _tables.find(iter_th->id)) != _tables.end()) {
                if (iter_tables->second->size_of_data_f() != iter_th->per_size) {
                    continue;
                }

                iter_tables->second->freq_sequence()->clear();
                iter_tables->second->map_from(convert_buf.buf,(*iter_th).pts);
                sequence::parse(iter_th->freq_str,iter_tables->second->freq_sequence());

                Log.stdprintf("freq string %s : \n",iter_th->freq_str);
            }
        }
        return 0;
    }

    int32_t load_to_buf(const cal_table_t table)
    {
        std::map<int32_t,cal_table *>::iterator iter_tables;

        if (convert_buf.table == table) {
            return 0;
        }

        if ((iter_tables = _tables.find(table._to_integral())) == _tables.end()) {
            return -1;
        }

        uint32_t pos = 0;
        uint32_t size = 0;

        table_pos_size(table,pos,size);

        r_from_pos(pos,size,convert_buf.buf);

        convert_buf.table = table;
        convert_buf.size_table = size;
        return 0;
    }

    int32_t table_pos_size(const cal_table_t table,uint32_t &pos,uint32_t &size) const
    {
        pos = 0;
        size = 0;
        bool ignore = false;
        uint32_t each_table_size = 0;
        cal_file_header_t header;
        cal_table_header_t table_header;

        open_to_r(stream);
        stream_r(header);

        for (uint32_t i = 0;i < header.tables;i ++) {
            stream_r(table_header);

            each_table_size = table_header.pts * table_header.per_size;
            if (table_header.id != table._to_integral()) {
                pos += ignore ? 0 : each_table_size;
            } else {
                size = each_table_size;
                ignore = true;
            }
        }

        pos += uint32_t(stream.tellg());
        return 0;
    }

    int32_t get_header(cal_file_header_t &header) const
    {
        open_to_r(stream);
        stream_r(header);
        return int32_t(stream.tellg());
    }

    int32_t get_header(cal_table_header_t &table_header) const
    {
        cal_file_header_t header;
        cal_table_header_t each_table_header;

        open_to_wr(stream);
        stream_r(header);

        for (uint32_t i = 0;i < header.tables;++i) {
            stream_r(each_table_header);

            if (each_table_header.id == table_header.id) {
                table_header = each_table_header;
            }
        }
        return 0;
    }

    int32_t get_header(cal_file_header_t &header,std::list<cal_table_header_t> &table_headers) const
    {
        cal_table_header_t table_header;

        table_headers.clear();

        open_to_r(stream);
        stream_r(header);

        for (uint32_t i = 0;i < header.tables;i ++) {
            stream_r(table_header);
            table_headers.push_back(table_header);
        }
        return int32_t(stream.tellg());
    }

    int32_t set_header(const cal_file_header_t &header) const
    { return w_from_pos(0,sizeof(cal_file_header_t),&header); }

    int32_t set_header(const cal_table_header_t &table_header) const
    {
        cal_file_header_t header;
        cal_table_header_t each_table_header;
        const int32_t table_header_size = sizeof(cal_table_header_t);

        open_to_wr(stream);
        stream_r(header);

        for (uint32_t i = 0;i < header.tables;++i) {
            stream_r(each_table_header);

            if (each_table_header.id == table_header.id) {
                assert_fs(stream.seekp(table_header_size * -1,std::ios::cur));
                stream_w(table_header);
                return 0;
            }
        }
        return 0;
    }

    int32_t set_header(const cal_file_header_t &header,const std::list<cal_table_header_t> &table_headers) const
    {
        typename std::list<cal_table_header_t>::const_iterator iter_th = table_headers.cbegin();

        open_to_wr(stream);
        stream_w(header);

        for (;iter_th != table_headers.cend();++iter_th) {
            stream_w(*iter_th);
        }
        return 0;
    }

    uint32_t get_version() const
    {
        cal_file_header_t header;
        get_header(header);
        return header.ver;
    }

    int32_t w_from_pos(uint32_t pos,uint32_t size,const void *data) const
    {
        open_to_wr(stream);
        assert_fs(stream.seekp(pos,std::ios::beg));
        assert_fs(stream.write(static_cast<const char *>(data),size));
        return 0;
    }

    int32_t r_from_pos(uint32_t pos,uint32_t size,void *data) const
    {
        open_to_wr(stream);
        assert_fs(stream.seekg(pos,std::ios::beg));
        assert_fs(stream.read(static_cast<char *>(data),size));
        return 0;
    }

private:
    std::string _path;
    std::map<int32_t,cal_table *> _tables;

protected:
    uint32_t _ver_current;
    static const int32_t size_granularity = 256;
};

} // namespace rd

#endif // RD_COMMON_CAL_FILE_H
