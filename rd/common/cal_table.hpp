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
#include "sequence.hpp"
#include "set_helper.hpp"
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
    TX0_Pwr_Output,
    TX0_Pwr_IO,
    TX1_Pwr,
    TX0_Att_Output,
    TX0_Att_IO,
    TX1_Att,
    TX_RF_IF_FR_0000_3000,
    TX_RF_FR_0000_3000,
    TX_RF_FR_3000_4800,
    TX_RF_FR_4800_6000,
    TX_RF_FR_6000_7500,
    TX_IF_FR_3000_7500,
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
    typedef sequence::arithmetic_sequence_t<int64_t> sequence_t;

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
        int64_t  freq;

        int64_t  key() const { return freq; }

        void set_key(const int64_t key)  { freq = key; }
        void set_key(const uint64_t key) { freq = int64_t(key); }

        // can not be virtual
        RD_INLINE int64_t key_lower_bound() { return INT64_MIN; }
        RD_INLINE int64_t key_upper_bound() { return INT64_MAX; }

#define DATA_F_KEY_MIN(min) RD_INLINE int64_t key_lower_bound() { return int64_t(min); }
#define DATA_F_KEY_MAX(max) RD_INLINE int64_t key_upper_bound() { return int64_t(max); }
    };

    virtual ~cal_table() {}
    std::vector<sequence_t> *freq_sequence() { return &_freq_sequence; }
    std::string freq_sequence_string() { return sequence::parse(_freq_sequence); }

    virtual char *data_f(uint32_t idx) = 0;

    virtual uint32_t size_data_f() = 0;
    virtual uint32_t size_of_data_f() = 0;
    virtual uint32_t size_of_data_m() = 0;
    virtual void prepare_cal(void *data_f_before,uint32_t data_f_before_size,
                             const std::set<uint64_t> *keys = nullptr,
                             const bool update_data_calibrating = false) = 0;
    virtual void map_from(void *data,uint32_t pts) = 0;
    virtual void add(void *data) = 0;
    virtual void combine(std::string &freq_str) = 0;

protected:
    std::vector<sequence_t> _freq_sequence;
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

    virtual int32_t get(const uint64_t &key,data_f_t &data) const
    {
        typename std::vector<data_f_t>::iterator iter = _data_f->begin();

        for (;iter != _data_f->end();++iter) {
            if (iter->key() >= int64_t(key)) {
                data = *iter;
                return 0;
            }
        }
        return -1;
    }

    virtual int32_t get(const uint64_t &key,data_m_t &data) const
    {
        typename std::vector<data_m_t>::const_iterator iter = _data_m.cbegin();

        for (;iter != _data_m.cend();++iter) {
//            if (iter->key() >= int64_t(key)) {
//                data = *iter;
//                return 0;
//            }
        }
        return 0;
    }

    void update_data_calibrating_from_data_f(const std::set<uint64_t> *keys)
    {
        if (_data_calibrating->size() == 0) {
            return;
        }

        size_t i = 0;
        data_f_t data;
        std::set<uint64_t>::iterator iter_key = keys->begin();
        typename std::vector<data_f_t>::iterator iter_data = _data_calibrating->begin();

        for (;iter_key != keys->end();++iter_key,++iter_data) {
            INT_CHECKV(get(*iter_key,data));

            *iter_data = data;
            iter_data->set_key(*iter_key);

            if (++i == _data_calibrating->size()) {
                return;
            }
        }
    }

    void prepare_cal(void *data_f_before,uint32_t data_f_before_size,
                     const std::set<uint64_t> *keys = nullptr,
                     const bool update_data_calibrating = false)
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
           if (is_between(int64_t(*iter_keys),data.key_lower_bound(),data.key_upper_bound())) {
                data.set_key(*iter_keys);
                _data_calibrating->push_back(data);
           }
        }

        if (update_data_calibrating) {
            update_data_calibrating_from_data_f(keys);
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

    void combine(std::string &freq_str)
    {
        size_t last_idx = 0;
        bool is_new_element = true;
        std::set<int64_t> freq;
        typename std::vector<data_f_t>::const_iterator iter_data_f;

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

        for (iter_data_f = _data_f->cbegin();iter_data_f != _data_f->cend();++iter_data_f) {
            freq.insert(freq.begin(),iter_data_f->freq);
        }
        sequence_string_of<int64_t>(freq,freq_str);
    }

protected:
    std::vector<data_f_t> *_data_calibrating;
    std::vector<data_f_t> *_data_f;
    std::vector<data_m_t> _data_m;
//    std::array<data_m_t,5> _cache;
};


typedef point_2d<int64_t,double> fr_point;

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

//    int32_t get(const uint64_t &freq,data_f_t &data) const
//    {

//    }

//    int32_t get(const uint64_t &freq,data_m_t &data) const
//    {

//    }

    int32_t save_as(const std::string &path)
    { return rd::save_as<data_f_t>(this->_data_calibrating,path); }
};

} // namespace rd

#endif
