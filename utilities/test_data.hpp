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

#ifndef UTILITIES_TEST_DATA_HPP
#define UTILITIES_TEST_DATA_HPP

#include "test_report.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace sp_rd {

struct test_common_data
{
    enum result_t {
        FAIL = 0,
        PASS
    };

    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
    char result[8]; // "Pass" or "Fail"

    test_common_data() :
        year(2000),month(1),day(1),
        hour(0),minute(0),second(0),
        result("Fail") { }

    std::string time() const
    {
        boost::format fmt("%5d/%02d/%02d-%02d:%02d:%02d");
        fmt % year % month % day;
        fmt % hour % minute % second;
        return fmt.str();
    }

    void parse_time_string(const std::string &time)
    {
        RD_ASSERT_THROW(time.length() == 19);

        year = normal_notation_string_to_uint16_t(time.substr(0,4));
        month = normal_notation_string_to_uint16_t(time.substr(5,7));
        day = normal_notation_string_to_uint16_t(time.substr(8,10));
        hour = normal_notation_string_to_uint16_t(time.substr(11,13));
        minute = normal_notation_string_to_uint16_t(time.substr(14,16));
        second = normal_notation_string_to_uint16_t(time.substr(17,19));
    }

    void set_time()
    {
        boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration time_day = time.time_of_day();
        boost::gregorian::date date = time.date();
        year = date.year();
        month = date.month();
        day = date.day();
        hour = uint16_t(time_day.hours());
        minute = uint16_t(time_day.minutes());
        second = uint16_t(time_day.seconds());
    }

    void set_result(const result_t res)
    {
        ZERO_ARRAY(result);

        if (res == FAIL) set_result("Fail");
        if (res == PASS) set_result("Pass");
    }

    void set_result(const std::string &res)
    {
        RD_ASSERT_THROW(res.length() < 8);
        ZERO_ARRAY(result);

        uint8_t i = 0;
        for (i = 0;i < res.length();i ++) {
            result[i] = res.at(i);
        }
        result[i] = '\0';
    }
};


// Test/Cal report : RF/IF TX/RX frequency response.
typedef struct freq_res_data : test_common_data
{
    double pwr;
    freq_res_data() : pwr(-200.0) {}
    std::string format_it() const
    {
        std::string str = "Power: ";
        boost::format fmt("%-10.2f%10s%25s");
        fmt % pwr % result % time();
        return str + fmt.str();
    }
}
rf_tx_freq_res_test_data,
if_tx_freq_res_test_data,
rf_rx_freq_res_test_data,
if_rx_freq_res_test_data,
rf_tx_freq_res_cal_data,
rf_rx_freq_res_cal_data;

DECL_TEST_REPORT_S(rf_tx_freq_res_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    rf_tx_freq_res_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "Power: "
        stream >> each_data.pwr;
        stream >> each_data.result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(rf_tx_freq_res_test)

typedef rf_tx_freq_res_test
        rf_rx_freq_res_test,
        if_tx_freq_res_test,
        if_rx_freq_res_test,
        rf_tx_freq_res_cal,
        rf_rx_freq_res_cal;


// Test report : TX phase noise.
struct tx_phase_noise_test_data : test_common_data
{
    double phase_noise[3];
    tx_phase_noise_test_data() { ZERO_ARRAY(phase_noise); }
    std::string format_it() const
    {
        std::string str;
        boost::format fmt("%-10.2f");
        fmt % phase_noise[0]; str += "@1K: " + fmt.str();
        fmt % phase_noise[1]; str += "@10K: " + fmt.str();
        fmt % phase_noise[2]; str += "@100K: " + fmt.str();
        str += (boost::format("%10s%25s") % result % time()).str();
        return str;
    }
};

DECL_TEST_REPORT_S(tx_phase_noise_test)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    tx_phase_noise_test_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "@1K: "
        stream >> each_data.phase_noise[0];
        stream >> str_tags; // "@10K: "
        stream >> each_data.phase_noise[1];
        stream >> str_tags; // "@100K: "
        stream >> each_data.phase_noise[2];
        stream >> each_data.result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());
        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(tx_phase_noise_test)


// Cal report : passband frequency response.
typedef struct passband_freq_res_cal_data : test_common_data {
    double flatness;
    double ripple;
    double pwr[256]; // Min : 245.76M / 1M
    passband_freq_res_cal_data() :
        flatness(0.0),
        ripple(0.0) { ZERO_ARRAY(pwr); }
    std::string format_it() const
    {
        std::string str;
        boost::format fmt_pwr("%10.2f");

        str += "Flatness: " + (boost::format("%-10.2f") % flatness).str();
        str += "Ripple: "   + (boost::format("%-10.2f") % ripple).str();

        str += "Powers:";
        for (uint32_t i = 0;i < ARRAY_SIZE(pwr);i ++) {
            fmt_pwr % pwr[i];
            str += fmt_pwr.str();
        }
        str += (boost::format("%10s%25s") % result % time()).str();
        return str;
    }
}
tx_passband_freq_res_160_cal_data,
rx_passband_freq_res_160_cal_data;

DECL_TEST_REPORT_S(passband_freq_res_cal)
void map_from(std::ifstream &stream, const tr_header_t &header)
{
    int64_t freq = 0;
    std::string str_freq;
    std::string str_time;
    std::string str_tags;
    passband_freq_res_cal_data each_data;

    for (uint32_t i = 0;i < header.pt;i ++) {
        stream >> str_freq;
        stream >> str_tags; // "Flatness: "
        stream >> each_data.flatness;
        stream >> str_tags; // "Ripple: "
        stream >> each_data.ripple;

        stream >> str_tags; // "Powers:"
        for (uint32_t i = 0;i < ARRAY_SIZE(each_data.pwr);i ++) {
            stream >> each_data.pwr[i];
        }
        stream >> each_data.result;
        stream >> str_time;
        stream.seekg(1,std::ios::cur);
        RD_ASSERT_THROW(!stream.bad());

        freq = freq_string_to_int64_t(str_freq);
        each_data.parse_time_string(str_time);
        _data.insert(std::map<int64_t,data_type>::value_type(freq,each_data));
    }
}
DECL_TEST_REPORT_E(passband_freq_res_cal)

typedef passband_freq_res_cal
        tx_passband_freq_res_160_cal,
        rx_passband_freq_res_160_cal;

} // namespace sp_rd

#endif // UTILITIES_TEST_DATA_HPP

