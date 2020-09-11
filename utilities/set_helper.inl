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

#include "set_helper.hpp"
#include "freq_string.hpp"
#include "integers.hpp"

template<typename T,typename container_t>
int32_t set_helper::set_helper_t<T,container_t>::section(const T &element)
{
    RD_ASSERT_THROW(star.size() == stop.size());

    if (element < min || element > max) {
        return -1;
    }

    typename container_t::iterator iter_star = star.begin();
    typename container_t::iterator iter_stop = stop.begin();

    for (size_t i = 0;i < star.size();i ++) {
        if (element >= *iter_star && element <= *iter_stop) {
            return int32_t(i);
        }
        iter_star ++;
        iter_stop ++;
    }

    return -1;
}

template<typename T,typename container_t>
uint32_t set_helper::set_helper_t<T,container_t>::sections(const std::string &str)
{
    size_t length = str.length();

    if (length == 0) {
        return 0;
    }

    size_t pos = 0;
    char next = str.at(0);
    uint32_t sections = 0;

    while ((pos = str.find(';',pos + 1)) != std::string::npos) {
        if (pos < length - 1) {
            next = str.at(pos + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \"" + str + "\",with continuously separator.");
            }
        }
        sections ++;
    }

    while ((pos = str.find(',',pos + 1)) != std::string::npos) {
        if (pos < length - 1) {
            next = str.at(pos + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \"" + str + "\",with continuously separator.");
            }
        }
        sections ++;
    }

    if ((str.back() != ';') && (str.back() != ',')) {
        sections ++;
    }

    return sections;
}

template<typename T,typename container_t>
uint32_t set_helper::set_helper_t<T,container_t>::sections(const char *str)
{
    size_t length = strlen(str);

    if (length == 0) {
        return 0;
    }

    const char *str_temp = str;
    const char back = *(str_temp + length - 1);
    char next = '\0';
    uint32_t sections = 0;

    while ((str_temp = strpbrk(str_temp,";"))) { // Find all ";"
        if (size_t(str_temp - str) < length - 1) {
            next = *(str_temp + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \""
                                          + std::string(str)
                                          + "\",with continuously separator.");
            }
        }
        sections ++;
        str_temp ++;
    }

    str_temp = str;
    while ((str_temp = strpbrk(str_temp,","))) { // Find all ","
        if (size_t(str_temp - str) < length - 1) {
            next = *(str_temp + 1);
            if ((next == ',') || (next == ';')) {
                RD_THROW rd::syntax_error("Invalid string : \""
                                          + std::string(str)
                                          + "\",with continuously separator.");
            }
        }
        sections ++;
        str_temp ++;
    }

    if ((back != ';') && (back != ',')) {
        sections ++;
    }

    return sections;
}

template<typename T,typename container_t>
std::string set_helper::set_helper_t<T,container_t>::add_as_freq_string(const std::string &str1,const std::string &str2)
{
    T t_star = 0;
    T t_stop = 0;
    T t_step = 0;
    T t_step_before = 0;
    uint32_t sec_pts = 1;
    std::string str;

    set_helper_t<T,container_t> set1;
    set_helper_t<T,container_t> set2;
    typename container_t::iterator iter;
    typename container_t::iterator iter_next;

    parse<T>(str1,set1);
    parse<T>(str2,set2);

    for (iter = set2.data.begin();iter != set2.data.end(); ++iter) {
        set1.data.insert(*iter);
    }

    if (set1.data.size() == 0) {
        return std::string();
    }

    iter = set1.data.begin();
    t_star = *iter;

    if (set1.data.size() == 1) {
        return freq_string_from<T>(t_star);
    }

    iter ++;
    t_stop = *iter;

    if (set1.data.size() == 2) {
        return freq_string_from<T>(t_star) + ";" + freq_string_from<T>(t_stop);
    }

    t_step_before = t_stop - t_star;

    iter = set1.data.begin();
    iter ++;
    iter_next = set1.data.begin();
    iter_next ++;
    iter_next ++;
    for (;iter_next != set1.data.end(); ++iter,++iter_next) {
        t_step = *iter_next - *iter;
        if (t_step == t_step_before) {
            ++sec_pts;
            continue;
        } else {
            t_stop = *iter;
            if (sec_pts == 1) {
                str += freq_string_from<T>(t_star) + ";";
                t_star = *iter;
                t_stop = *iter_next;
                t_step_before = t_stop - t_star;
            } else {
                str += freq_string_from<T>(t_star) + ":";
                str += freq_string_from<T>(t_step_before) + ":";
                str += freq_string_from<T>(t_stop) + ";";
                ++iter;
                ++iter_next;

                t_star = *iter;
                if (iter_next != set1.data.end()) {
                    t_stop = *iter_next;
                    t_step_before = t_stop - t_star;
                } else {
                    str += freq_string_from<T>(t_star) + ";";
                    return str;
                }
            }

            sec_pts = 1;
        }
    }

    t_stop = *iter;
    if (sec_pts == 1) {
        str += freq_string_from<T>(t_star) + ";";
        if (t_stop != t_star) {
            str += freq_string_from<T>(t_stop) + ";";
        }
    } else {
        str += freq_string_from<T>(t_star) + ":";
        str += freq_string_from<T>(t_step_before) + ":";
        str += freq_string_from<T>(t_stop) + ";";
    }

    return str;
}

template<typename T,typename container_t>
template<typename specialized_container_t>
void set_helper::set_helper_t<T,container_t>::split(const std::string &str,uint32_t sections,specialized_container_t &out)
{
    size_t length = str.length();
    size_t pos[2] = {0,0};
    size_t pos_start = 0;

    RD_ASSERT_THROW(length > 0);
    out.clear();

    for (uint32_t i = 0;i < sections;i ++) {
        pos[0] = str.find(';',pos_start);
        pos[1] = str.find(',',pos_start);
        pos[0] = (pos[0] != std::string::npos ?
                 (pos[1] != std::string::npos ? std::min(pos[0],pos[1]) : pos[0]) :
                 (pos[1] != std::string::npos ? pos[1] : length));

        out.push_back(str.substr(pos_start,pos[0] - pos_start));
        pos_start = pos[0] + 1;
    }
}

template<typename T,typename container_t>
void set_helper::set_helper_t<T,container_t>::split(const char *str,uint32_t sections,char (*out)[32])
{
    size_t length = strlen(str);
    const char *str_in_temp = str;
    uint32_t pos[2] = {0,0};

    RD_ASSERT_THROW(length > 0);

    for (uint32_t i = 0;i < sections;i ++) {
        while (*str == ' ' || *str == '\t') {
            str ++;
        }

        str_in_temp = strchr(str,';');
        pos[0] = (str_in_temp != nullptr) ? uint32_t(str_in_temp - str) : 0;

        str_in_temp = strchr(str,',');
        pos[1] = (str_in_temp != nullptr) ? uint32_t(str_in_temp - str) : 0;

        if (pos[0] > 0) {
            pos[0] = pos[1] > 0 ? std::min(pos[0],pos[1]) : pos[0];
        } else {
            pos[0] = pos[1] > 0 ? pos[1] : uint32_t(strlen(str));
        }

        for (uint32_t j = 0;j < pos[0];j ++) {
            out[i][j] = str[j];
        }
        out[i][pos[0]] = '\0';

        str += pos[0] + 1;
    }
}

template<typename T,typename container_t>
void set_helper::set_helper_t<T,container_t>::split(const std::string &str,range_string &out)
{
    size_t length = str.length();
    size_t pos_star = 0;
    size_t pos_step = 0;

    RD_ASSERT_THROW(length > 0);

    if ((pos_star = str.find(':',0)) == std::string::npos) {
        out.star = str;
        out.stop = str;
        out.step = str;
        return;
    }

    if ((pos_step = str.find(':',pos_star + 1)) == std::string::npos) {
        RD_THROW rd::syntax_error("Invalid string : \"" + str + "\".");
    }

    out.star = str.substr(0,pos_star);
    out.step = str.substr(pos_star + 1,pos_step - pos_star - 1);
    out.stop = str.substr(pos_step + 1);

    if ((pos_step = str.find(':',pos_step + 1)) != std::string::npos) {
        RD_THROW rd::syntax_error("Invalid string : \"" + str + "\".");
    }
}

template<typename T,typename container_t>
uint32_t set_helper::set_helper_t<T,container_t>::parse_from(const std::string &str)
{
    uint32_t sec = sections(str);
    range_string each_range;

    data.clear();
    star.clear();
    stop.clear();

    if (sec == 0) {
        return 0;
    }

    T T_freq_star = 0;
    T T_freq_stop = 0;
    T T_freq_step = 0;
    std::vector<std::string> splitted_str;

    split(str,sec,splitted_str);

    for (uint32_t i = 0;i < sec;i ++) {
        split(splitted_str[i],each_range);

        floating_point_numbers::auto_notation<T>(each_range.star,T_freq_star);
        floating_point_numbers::auto_notation<T>(each_range.step,T_freq_step);
        floating_point_numbers::auto_notation<T>(each_range.stop,T_freq_stop);

        star.insert(star.end(),T_freq_star);
        stop.insert(stop.end(),T_freq_stop);

        for (uint32_t j = 0;j < SERIE_SIZE(T_freq_star,T_freq_stop,T_freq_step);j ++) {
            data.insert(T_freq_star + T(j) * T_freq_step);
        }
    }

    max = *(data.rbegin());
    min = *(data.begin());

    return uint32_t(data.size());
}

template<typename T,typename container_t>
uint32_t set_helper::set_helper_t<T,container_t>::pts(const std::string &str)
{
    uint32_t pts = 0;
    uint32_t sec = sections(str);
    range_string each_range;

    if (sec == 0) {
        return 0;
    }

    T T_freq_star = 0;
    T T_freq_stop = 0;
    T T_freq_step = 0;
    std::vector<std::string> splitted_str;

    split(str,sec,splitted_str);

    for (uint32_t i = 0;i < sec;i ++) {
        split(splitted_str[i],each_range);

        floating_point_numbers::auto_notation<T>(each_range.star,T_freq_star);
        floating_point_numbers::auto_notation<T>(each_range.step,T_freq_step);
        floating_point_numbers::auto_notation<T>(each_range.stop,T_freq_stop);

        pts += SERIE_SIZE(T_freq_star,T_freq_stop,T_freq_step);
    }

    return pts;
}
