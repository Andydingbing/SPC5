#ifndef RD_COMMON_CAL_TABLE_HPP
#define RD_COMMON_CAL_TABLE_HPP

#define DECL_CAL_TABLE(project,...) \
    namespace ns_##project { BETTER_ENUM(cal_table_t,int32_t,__VA_ARGS__) }

#include "rd.h"
#include "enum.h"
#include <boost/cstdint.hpp>
#include <boost/format.hpp>
#include <vector>
#include <set>
#include <fstream>
#include "algo_math.hpp"

#include "liblog.h"

namespace rd {

DECL_CAL_TABLE(sp9500,
    Begin = 0,
    OCXO = 0)

DECL_CAL_TABLE(sp9500x,
    Begin = 0,
    OCXO = 0,
    TX_Sideband,
    TX_LO_Leakage,
    TX_Pwr_Output,
    TX_Pwr_IO,
    TX_Att_Output,
    TX_Att_IO,
    TX_RF_IF_FR_0000_3000,
    TX_RF_FR_0000_3000,
    TX_RF_FR_3000_4800,
    TX_RF_FR_4800_6000,
    TX_RF_FR_6000_7500,
    TX_IF_FR_0000_7500,
    TX_Filter,
    RX_Ref_Output,
    RX_Ref_IO,
    RX_Att_Output,
    RX_Att_IO,
    RX_RF_Response,
    RX_IF_Response,
    RX_Filter,
    Total)

class cal_table
{
public:
    struct time_t {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t time_rsv;
        time_t() : year(0),month(0),day(0),
                   hour(0),minute(0),second(0),time_rsv(0) {}
    };

    struct basic_data_f_t {
        time_t   time;
        uint8_t  is_called;
        uint8_t  method;
        uint16_t station;
        uint32_t staff;
        uint64_t freq;

        uint64_t key() const { return freq; }
        void set_key(const uint64_t key) { freq = key; }

        // can not be virtual
        RD_INLINE uint64_t key_lower_bound() { return 0; }
        RD_INLINE uint64_t key_upper_bound() { return UINT64_MAX; }

#define DATA_F_KEY_MIN(min) RD_INLINE uint64_t key_lower_bound() { return uint64_t(min); }
#define DATA_F_KEY_MAX(max) RD_INLINE uint64_t key_upper_bound() { return uint64_t(max); }
    };

    virtual ~cal_table() {}
    virtual char *data_f(uint32_t idx) = 0;

    virtual uint32_t size_data_f() = 0;
    virtual uint32_t size_of_data_f() = 0;
    virtual uint32_t size_of_data_m() = 0;
    virtual void prepare_cal(void *data_f_before,uint32_t data_f_before_size,const std::set<uint64_t> *keys = nullptr) = 0;
    virtual void map_from(void *data,uint32_t pts) = 0;
    virtual void add(void *data) = 0;
    virtual void combine() = 0;
};

template<typename str_t>
str_t string_of(const cal_table::time_t &t)
{
    str_t str;
    boost::format fmt("%d");

    str += (fmt % (t.year + 1900)).str().c_str();
    str += (fmt % (t.month)).str().c_str();
    str += (fmt % (t.day)).str().c_str();
    str += (fmt % (t.hour)).str().c_str();
    str += (fmt % (t.minute)).str().c_str();
    str += (fmt % (t.second)).str().c_str();
    return str;
}

template<typename data_f_t,typename data_m_t>
class cal_table_data : public cal_table
{
public:
    cal_table_data() : _data_calibrating(nullptr),_data_f(nullptr) {}

    std::vector<data_f_t> *data_calibrating() { return _data_calibrating; }
    std::vector<data_f_t> *data_f() { return _data_f; }
    std::vector<data_m_t> *data_m() { return &_data_m; }

    char *data_f(uint32_t idx)
    { return reinterpret_cast<char *>(&(_data_f->at(idx))); }

    uint32_t size_data_calibrating()
    { return uint32_t(_data_calibrating->size()); }

    uint32_t size_data_f()    { return uint32_t(_data_f->size()); }
    uint32_t size_data_m()    { return uint32_t(_data_m.size()); }
    uint32_t size_of_data_f() { return sizeof(data_f_t); }
    uint32_t size_of_data_m() { return sizeof(data_m_t); }

    void prepare_cal(void *data_f_before,uint32_t data_f_before_size,const std::set<uint64_t> *keys = nullptr)
    {
        SAFE_NEW(_data_f,std::vector<data_f_t>);
        SAFE_NEW(_data_calibrating,std::vector<data_f_t>);

        _data_f->clear();
        _data_f->assign(data_f_before_size / size_of_data_f(),data_f_t());
        // _data_f->resize(data_f_before_size / size_of_data_f());

        if (data_f_before != nullptr && data_f_before_size != 0) {
            memcpy(data_f(0),data_f_before,data_f_before_size);
        }

        _data_calibrating->clear();

        if (keys == nullptr) {
            return;
        }

        std::set<uint64_t>::iterator iter_keys;
        data_f_t data;

        for (iter_keys = keys->begin();iter_keys != keys->end();++iter_keys) {
           if (is_between(*iter_keys, data.key_lower_bound(),data.key_upper_bound())) {
                data.set_key(*iter_keys);
                _data_calibrating->push_back(data);
           }
        }
    }

    void add(void *data)
    {
        data_f_t *d = static_cast<data_f_t *>(data);

        if (_data_calibrating->empty()) {
            _data_calibrating->push_back(*d);
            return;
        }

        // Speed up
        if (_data_calibrating->back().key() == d->key()) {
            (*_data_calibrating)[size_data_calibrating() - 1] = *d;
            return;
        }

        typename std::vector<data_f_t>::iterator iter;

        for (iter = _data_calibrating->begin();iter != _data_calibrating->end();++iter) {
            if (iter->key() == d->key()) {
                *iter = *d;
                return;
            }
        }

        Log.stdprintf("wtf!!!\n");
        _data_calibrating->push_back(*d);
    }

    void combine()
    {
        size_t last_idx = 0;
        bool is_new_element = true;

        if (_data_calibrating == nullptr || _data_f == nullptr) {
            return;
        }
        for (size_t i = 0;i < _data_calibrating->size();++i) {
            is_new_element = true;
            for (size_t j = last_idx;j < _data_f->size();++j) {
                if (_data_f->at(j).key() == _data_calibrating->at(i).key()) {
                    (*_data_f)[j] = (*_data_calibrating)[i];
                    last_idx = j;
                    is_new_element = false;
                    break;
                }
            }
            if (is_new_element) {
                _data_f->push_back(_data_calibrating->at(i));
            }
        }
    }

protected:
    std::vector<data_f_t> *_data_calibrating;
    std::vector<data_f_t> *_data_f;
    std::vector<data_m_t> _data_m;
};


typedef point_2d<uint64_t,double> fr_point;

template<uint32_t n = 1>
struct data_f_fr : cal_table::basic_data_f_t
{
    static BOOST_CONSTEXPR_OR_CONST uint32_t size = n;
    fr_point pts[n];
};

template<typename data_f_t = data_f_fr<1>>
int32_t save_as(const std::vector<data_f_t> *data,const std::string &path)
{
    std::ofstream stream(path);

    if (stream.bad()) {
        Log.set_last_err("%s:%s:%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__);
        return -1;
    }

    if (data == nullptr) {
        return 0;
    }

    boost::format fmt("%.6f,");
    typename std::vector<data_f_t>::const_iterator iter = data->cbegin();

    for (;iter != data->cend();++iter) {
        for (uint32_t i = 0;i < data_f_t::size;++i) {
            stream << ((fmt % iter->pts[i].y).str());
        }
        stream << "\n";
    }
    return 0;
}

template<uint32_t n = 1>
struct data_m_fr
{ double pwr[n]; };

template<typename data_f_fr_t = data_f_fr<1>,typename data_m_fr_t = data_m_fr<data_f_fr_t::size>>
class fr_table_t : public cal_table_data<data_f_fr_t,data_m_fr_t>
{
public:
    using data_f_t = data_f_fr_t;
    using data_m_t = data_m_fr_t;

    void map_from(void *data,uint32_t pts)
    {
        data_f_t *d_f = static_cast<data_f_t *>(data);
        data_m_t d_m;

        this->_data_m.clear();
        for (uint32_t i = 0;i < pts;++i) {
            for (uint32_t j = 0;j < data_f_t::size;++j) {
                d_m.pwr[j] = d_f[i].pts[j].y;
            }
            this->_data_m.push_back(d_m);
        }
    }

    int32_t save_as(const std::string &path)
    { return rd::save_as<data_f_t>(this->_data_calibrating,path); }
};

} // namespace rd

#endif
