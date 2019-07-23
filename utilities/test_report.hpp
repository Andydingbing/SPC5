/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef UTILITIES_TEST_REPORT_HPP
#define UTILITIES_TEST_REPORT_HPP

#include "freq_string.hpp"
#include <map>
#include <fstream>
#include <boost/filesystem.hpp>

namespace sp_rd {

//! List all test items,value only allowed to accumulate.
enum test_item_t : unsigned {
    TI_RF_TX_FREQ_RES,
    TI_IF_TX_FREQ_RES,
    TI_RF_RX_FREQ_RES,
    TI_IF_RX_FREQ_RES,
    TI_TX_PHASE_NOISE,
    TI_TX_NOISE_FLOOR,
    TI_TX_PASSBAND_160,
    TI_RX_PASSBAND_160,

    TEST_TOTAL_ITEMS
};

struct tr_header_t {        // The report header,8 bytes align
    char sn[32];            // RF board SN
    test_item_t item : 32;  // Test item
    uint32_t size;          // Each data size(byte)
    uint32_t pt;            // Points
    char result[64];        // Test result

    tr_header_t() :
        item(test_item_t(TEST_TOTAL_ITEMS)),
        size(0),
        pt(0) {ZERO_ARRAY(sn); ZERO_ARRAY(result);}

    // Just compare sn,item,size.
    bool operator == (const tr_header_t *header) const
    {
        for (uint32_t i = 0;i < ARRAY_SIZE(sn);i ++) {
            if (header->sn[i] != sn[i]) {
                return false;
            }
        }
        if (header->item != item) { return false; }
        if (header->size != size) { return false; }
        return true;
    }
};

template <typename data_t>
class test_report : boost::noncopyable
{
public:
    test_report(const std::string &report_name,
                const tr_header_t *header,
                const bool creat = false)
    {
#ifdef RD_PLATFORM_WIN32
        _path = "C:\\RD";
#elif defined RD_PLATFORM_LINUX
#endif

        if (!boost::filesystem::exists(_path)) {
            if (!boost::filesystem::create_directory(_path)) {
                RD_THROW sp_rd::runtime_error("Can't create" + _path);
            }
        }

        if (header != nullptr) {
            _path += "\\";
            _path += header->sn;
        }

        if (!boost::filesystem::exists(_path)) {
            if (!boost::filesystem::create_directory(_path)) {
                RD_THROW sp_rd::runtime_error("Can't create" + _path);
            }
        }

        if (header != nullptr)
            _path += "\\TestReport";

        if (!boost::filesystem::exists(_path)) {
            if (!boost::filesystem::create_directory(_path)) {
                RD_THROW sp_rd::runtime_error("Can't create" + _path);
            }
        }

        _path += "\\";
        _path += report_name;
        _path += ".log";

        validate(header,creat);
    }

    virtual ~test_report() {}

protected:
    virtual void map_from(std::ifstream &stream,const tr_header_t &head) = 0;

public:
    std::map<int64_t,data_t> *data()
    { return &_data; }

    std::string full_path()
    { return _path; }

    tr_header_t get_header() const
    {
        std::ifstream stream(_path,std::ios::_Nocreate);
        tr_header_t header;
        uint32_t i_item = 0;

        if (!stream.is_open()) {
            return tr_header_t();
        }

        stream.seekg(5,std::ios::beg);
        if (stream.bad()) { return tr_header_t(); }
        stream >> header.sn;
        if (stream.bad()) { return tr_header_t(); }

        stream.seekg(53,std::ios::beg);
        if (stream.bad()) { return tr_header_t(); }
        stream >> i_item;
        if (stream.bad()) { return tr_header_t(); }

        header.item = test_item_t(i_item);

        stream.seekg(71,std::ios::beg);
        if (stream.bad()) { return tr_header_t(); }
        stream >> header.size;
        if (stream.bad()) { return tr_header_t(); }

        stream.seekg(79,std::ios::beg);
        if (stream.bad()) { return tr_header_t(); }
        stream >> header.pt;
        if (stream.bad()) { return tr_header_t(); }

        stream.seekg(112,std::ios::beg);
        if (stream.bad()) { return tr_header_t(); }
        stream >> header.result;
        if (stream.bad()) { return tr_header_t(); }

        stream.close();
        return header;
    }

    void set_header(const tr_header_t &header) const
    {
        std::ofstream stream(_path,std::ios::_Nocreate);

        if (!stream.is_open()) {
            throw sp_rd::runtime_error("Can't read " + _path);
        }

        stream << header_from(header);
        stream.close();
    }

    void set_sn(const std::string &sn)
    {
        RD_ASSERT_THROW(sn.length() > 0);
        RD_ASSERT_THROW(sn.length() < ARRAY_SIZE(tr_header_t::sn));

        uint32_t i = 0;
        tr_header_t header = get_header();

        for (i = 0;i < sn.length();i ++) {
            header.sn[i] = sn.at(i);
        }
        header.sn[i] = '\0';
        set_header(header);
    }

    void set_item(const test_item_t &item)
    {
        RD_ASSERT_THROW(item < TEST_TOTAL_ITEMS);

        tr_header_t header = get_header();
        header.item = item;
        set_header(header);
    }

    void set_result(const std::string &res)
    {
        RD_ASSERT_THROW(res.length() > 0);
        RD_ASSERT_THROW(res.length() < ARRAY_SIZE(tr_header_t::result));

        uint32_t i = 0;
        tr_header_t header = get_header();

        for (i = 0;i < res.length();i ++) {
            header.result[i] = res.at(i);
        }
        header.result[i] = '\0';
        set_header(header);
    }

    void add(const int64_t freq,const data_t &each_data)
    {
        typename std::map<int64_t,data_t>::iterator iter;
        if ((iter = _data.find(freq)) != _data.end()) {
            iter->second = each_data;
        } else {
            _data.insert(std::map<int64_t,data_t>::value_type(freq,each_data));
        }
    }

    void update() const
    {
        tr_header_t header = get_header();
        header.pt = uint32_t(_data.size());

        if (header.pt > 0) {
            header.size = sizeof(data_t);
        }
        set_header(header);

        std::ofstream stream;
        open_for_write(stream);

        typename std::map<int64_t,data_t>::const_iterator iter;

        for (iter = _data.begin();iter != _data.end();iter ++) {
            stream << vertical_freq(iter->first);
            stream << iter->second.format_it();
            stream << std::endl;
        }
        stream.close();
    }

protected:
    void validate(const tr_header_t *header,const bool creat) const
    {
        if (!is_file_valid(header)) {
            if (creat) {
                if (!create(header)) {
                    throw sp_rd::runtime_error("Can't create/open " + _path);
                }
            }
            if (!is_file_valid(header)) {
                throw sp_rd::runtime_error("Invalid file : " + _path);
            }
        }
    }

    bool create(const tr_header_t *header) const
    {
        std::ofstream stream(_path);

        if (!stream.is_open()) {
            stream.open(_path);
            if (!stream.is_open()) {
                return false;
            }
        }

        stream << header_from(*header);
        stream.close();
        return true;
    }

    bool is_file_valid(const tr_header_t *header) const
    {
        tr_header_t header_file = get_header();

    //    if (!basic_sp1401::is_valid_sn(file_info.sn)) {
    //        return false;
    //    }

        if (header_file.size != sizeof(data_t)) {
            return false;
        }

        if (header == nullptr) {
            return true;
        }

        return header_file == header;
    }

    void open_for_read(std::ifstream &stream) const
    {
        stream.open(_path,std::ios::_Nocreate);
        if (!stream.is_open()) {
            throw sp_rd::runtime_error("Can't read " + _path);
        }
        stream.seekg(offset_data,std::ios::beg);
        if (stream.bad()) {
            throw sp_rd::runtime_error("Invalid file : " + _path);
        }
    }

    void open_for_write(std::ofstream &stream) const
    {
        stream.open(_path,std::ios::_Nocreate);
        if (!stream.is_open()) {
            throw sp_rd::runtime_error("Can't Write " + _path);
        }
        stream.seekp(offset_data,std::ios::beg);
        if (stream.bad()) {
            throw sp_rd::runtime_error("Invalid file : " + _path);
        }
    }

    static std::string header_from(const tr_header_t &head)
    {
        std::stringstream stream;

        stream << std::string("SN : ");
        for (uint32_t i = 0;i < ARRAY_SIZE(tr_header_t::sn);i ++) {
            stream << head.sn[i];
        }
        stream << std::endl;

        stream << std::string("Test Item ID : ")
               << boost::format("%-3d") % head.item
               << std::endl;

        stream << std::string("Total Size : ")
               << boost::format("%05d") % head.size
               << " * "
               << boost::format("%05d") % head.pt
               << " = "
               << boost::format(" %010d") % (head.size * head.pt)
               << "Bytes"
               << std::endl;

        stream << std::string("Result : ");
        for (uint32_t i = 0;i < ARRAY_SIZE(tr_header_t::result);i ++) {
            stream << head.result[i];
        }
        stream << std::endl;
        stream << std::endl;

        return stream.str();
    }

    static std::string vertical_freq(const int64_t freq)
    {
        return (boost::format("%-10s") %
                freq_string_from_int64_t(freq)).str();
    }

protected:
    std::string _path;
    std::map<int64_t,data_t> _data;

private:
    const uint8_t offset_data = 178;
};

#define DECL_TEST_REPORT_S(class_name) \
class class_name : public test_report<class_name##_data> { \
public: \
    typedef class_name##_data data_type; \
    class_name(const std::string &report_name, \
               const tr_header_t *header, \
               bool create = false) : \
        test_report(report_name,header,create) {open();} \
private: \
    void open() { \
        tr_header_t header = get_header(); \
        std::ifstream stream; open_for_read(stream); \
        _data.clear(); \
        map_from(stream,header); \
    }

#define DECL_TEST_REPORT_E(class_name) };

} // namespace sp_rd

#endif // UTILITIES_TEST_REPORT_HPP

