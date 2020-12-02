#ifndef RD_COMMON_CAL_TABLE_HPP
#define RD_COMMON_CAL_TABLE_HPP

#define DECL_CAL_TABLE(project,...) \
    namespace ns_##project { BETTER_ENUM(cal_table_t,int32_t,__VA_ARGS__) }

#include "rd.h"
#include "enum.h"
#include <boost/cstdint.hpp>
#include <boost/format.hpp>
#include <vector>

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
    };

    virtual ~cal_table() {}
    virtual char *data_f(uint32_t idx) = 0;

    virtual uint32_t size_data_f() = 0;
    virtual uint32_t size_of_data_f() = 0;
    virtual uint32_t size_of_data_m() = 0;
    virtual void prepare_cal(void *data_f_before,uint32_t data_f_before_size) = 0;
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

    void prepare_cal(void *data_f_before,uint32_t data_f_before_size)
    {
        SAFE_NEW(_data_f,std::vector<data_f_t>);
        SAFE_NEW(_data_calibrating,std::vector<data_f_t>);

        _data_f->clear();
        _data_f->resize(data_f_before_size / size_of_data_f());
        memcpy(data_f(0),data_f_before,data_f_before_size);

        _data_calibrating->clear();
    }

    void add(void *data)
    { _data_calibrating->push_back(*static_cast<data_f_t *>(data)); }

    void combine()
    {
        size_t last_idx = 0;
        bool is_new_element = true;

        if (_data_calibrating == nullptr || _data_f == nullptr) {
            return;
        }
        for (size_t i = 0;i < _data_calibrating->size();++i) {
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

} // namespace rd

#endif
