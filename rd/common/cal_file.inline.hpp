#ifndef RD_COMMON_CAL_FILE_INLINE_HPP
#define RD_COMMON_CAL_FILE_INLINE_HPP

#include "rd.h"
#include "liblog.h"
#include "cal_file.hpp"
#include <fstream>
#include <boost/current_function.hpp>

using namespace std;
using namespace rd;

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
    std::fstream stream(_path,ios::in | ios::out | ios::binary); \
    assert_fs(stream);

#define stream_r(dst) \
    assert_fs(stream.read(reinterpret_cast<char *>(&dst),sizeof(dst)));

#define stream_w(src) \
    assert_fs(stream.write(reinterpret_cast<const char *>(&src),sizeof(src)));

class convert_buf_t
{
public:
    void *_buf;
    char sn[64];
    int32_t  table;
    uint32_t size;

    convert_buf_t() : _buf(nullptr),table(-1),size(0) { ZERO_ARRAY(sn); }
    ~convert_buf_t() { if (size > 0) { delete []buf(); } }

    char *buf() { return static_cast<char *>(_buf); }
    void allocate(uint32_t new_size)
    {
        if (size == 0) {
            _buf = new char[new_size / sizeof(char)];
        } else if (size < new_size) {
            delete []buf();
            _buf = new char[new_size / sizeof(char)];
        }
    }
};

static const int32_t size_granularity = 256;
static convert_buf_t convert_buf;

template<typename cal_table_t>
uint32_t basic_cal_file<cal_table_t>::ver_current = 0;

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::open()
{
    if (!pre_check()) {
        if (create() < 0) { return -1; }
        if (!pre_check()) { return -1; }
    }
    load();
    return 0;
}

template<typename cal_table_t>
bool basic_cal_file<cal_table_t>::pre_check()
{
    uint32_t ver = get_version();

    if (ver == 0) {
        return false;
    } else if (ver < ver_current) {
//        cfv_ctrl ctrl;
//        ctrl.target(this);
//        ctrl.for_c(ver);
    }
    return true;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::create()
{
    cal_file_header_t header;
    header.ver = 1;

    open_to_w(stream);
    stream_w(header);
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::load()
{
    cal_file_header_t header;
    cal_table_header_t table_header;

    uint32_t max_size = 0;
    uint32_t cur_table_size = 0;
    list<cal_table_header_t> table_headers;
    typename list<cal_table_header_t>::iterator iter_th;

    map<int32_t,cal_table *>::iterator iter_tables;

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
        assert_fs(stream.read(convert_buf.buf(),(*iter_th).pts * (*iter_th).per_size));

        if ((iter_tables = _tables.find(iter_th->id)) != _tables.end()) {
            if (iter_tables->second->size_of_data_f() != iter_th->per_size) {
                continue;
            }
            iter_tables->second->map_from(convert_buf.buf(),(*iter_th).pts);
        }
    }
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::get_header(cal_file_header_t &header) const
{
    open_to_r(stream);
    stream_r(header);

    return int32_t(stream.tellg());
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::get_header(cal_table_header_t &table_header) const
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

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::set_header(const cal_table_header_t &table_header) const
{
    cal_file_header_t header;
    cal_table_header_t each_table_header;
    const int32_t table_header_size = sizeof(cal_table_header_t);

    open_to_wr(stream);
    stream_r(header);

    for (uint32_t i = 0;i < header.tables;++i) {
        stream_r(each_table_header);

        if (each_table_header.id == table_header.id) {
            assert_fs(stream.seekp(table_header_size * -1,ios::cur));
            stream_w(table_header);
            return 0;
        }
    }
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::get_header(
        cal_file_header_t &header,
        std::list<cal_table_header_t> &table_headers) const
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

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::set_header(
        const cal_file_header_t &header,
        const std::list<cal_table_header_t> &table_headers) const
{
    typename list<cal_table_header_t>::const_iterator iter_th = table_headers.cbegin();

    open_to_wr(stream);
    stream_w(header);

    for (;iter_th != table_headers.cend();++iter_th) {
        stream_w(*iter_th);
    }
    return 0;
}

template<typename cal_table_t>
uint32_t basic_cal_file<cal_table_t>::get_version() const
{
    cal_file_header_t header;
    get_header(header);
    return header.ver;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::w_from_pos(uint32_t pos,uint32_t size,void *data) const
{
    open_to_wr(stream);
    assert_fs(stream.seekp(pos,ios::beg));
    assert_fs(stream.write(static_cast<char *>(data),size));
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::r_from_pos(uint32_t pos,uint32_t size,void *data) const
{
    open_to_wr(stream);
    assert_fs(stream.seekg(pos,ios::beg));
    assert_fs(stream.read(static_cast<char *>(data),size));
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::table_pos_size(const cal_table_t table,uint32_t &pos,uint32_t &size) const
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

    pos += stream.tellg();
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::make_sure_has(const cal_table_t table,cal_table *t)
{
    cal_table_header_t table_header;
    table_header.id = table._to_integral();
    table_header.per_size = t->size_of_data_f();

    int32_t data_start_pos = 0;
    cal_file_header_t header;
    list<cal_table_header_t> table_headers;
    typename list<cal_table_header_t>::const_iterator iter_th;

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

    map<int32_t,cal_table *>::const_iterator iter_tables;

//    if ((iter_tables = _tables.find(table._to_integral())) != _tables.end()) {
//        if (iter_tables->second != nullptr) {
//            delete iter_tables->second;
//        }
//    }

    _tables[table._to_integral()] = t;
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::add(const cal_table_t table,void *data)
{
    map<int32_t,cal_table *>::const_iterator iter;

    if ((iter = _tables.find(table._to_integral())) != _tables.end()) {
        iter->second->add(data);
    }
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::persistence(const cal_table_t table) const
{
    uint32_t pos = 0;
    uint32_t cur_size = 0;
    uint32_t new_size = 0;
    uint32_t per_size = 0;
    cal_table_header_t table_header;
    cal_table *t = nullptr;

    map<int32_t,cal_table *>::const_iterator iter;

    if ((iter = _tables.find(table._to_integral())) == _tables.end()) {
        return 0;
    }

    INT_CHECK(table_pos_size(table,pos,cur_size));

    t = iter->second;
    t->combine();
    per_size = t->size_of_data_f();
    new_size = t->size_data_f() * per_size;
    table_header.id = table._to_integral();

    if (new_size > cur_size) {
        INT_CHECK(get_header(table_header));
        table_header.pts = t->size_data_f();
        table_header.per_size = t->size_of_data_f();
        INT_CHECK(set_header(table_header));
        INT_CHECK(slice_and_move(pos + cur_size,new_size - cur_size));
    }
    for (uint32_t i = 0;i < t->size_data_f();++i) {
        INT_CHECK(w_from_pos(pos,per_size,t->data_f(i)));
        pos += per_size;
    }
    return 0;
}

template<typename cal_table_t>
int32_t basic_cal_file<cal_table_t>::slice_and_move(uint32_t pos,uint32_t offset) const
{
    uint32_t size = 0;
    int32_t left = 0;
    int32_t move_block_size = 0;
    char buf[size_granularity] = {0};

    open_to_wr(stream);
    stream.seekp(0,ios::end);
    stream.seekg(0,ios::end);
    size = stream.tellp();

    if (pos > size) {
        return 0;
    }

    if (pos == size) {
        left = offset;
        while (left > size_granularity) {
            assert_fs(stream.write(buf,size_granularity));
            left -= size_granularity;
        }
        assert_fs(stream.write(buf,left));
        return 0;
    }

    left = size - pos;

    while(1) {
        if (left <= 0) {
            break;
        }

        move_block_size = left < size_granularity ? left : size_granularity;
        move_block_size = offset < move_block_size ? offset : move_block_size;

        assert_fs(stream.seekg(move_block_size * -1,ios::cur));
        assert_fs(stream.read(buf,move_block_size));
        assert_fs(stream.seekg(move_block_size * -1,ios::cur));
        assert_fs(stream.seekg(offset,ios::cur));
        assert_fs(stream.write(buf,move_block_size));

        left -= move_block_size;

        assert_fs(stream.seekg(pos + left,ios::beg));
    }
    return 0;
}

template<typename cal_table_t>
void basic_cal_file<cal_table_t>::show_header(const cal_file_header_t &head)
{

}

#endif
