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

#ifndef UTILITIES_SPEC_HPP
#define UTILITIES_SPEC_HPP

#include <freq_string.hpp>

namespace sp_rd {

class RD_API spec
{
public:
    static double test_rf_tx_freq_res_max();
    static double test_rf_tx_freq_res_min();
    static double test_rf_tx_freq_res();

    static double test_if_tx_freq_res_max();
    static double test_if_tx_freq_res_min();
    static double test_if_tx_freq_res();

    static double test_rf_rx_freq_res_max();
    static double test_rf_rx_freq_res_min();
    static double test_rf_rx_freq_res();

    static double test_if_rx_freq_res_max();
    static double test_if_rx_freq_res_min();
    static double test_if_rx_freq_res();

    static void test_tx_phase_noise_freq(range_freq_string &freq);
    static void test_tx_phase_noise_offset(std::string &offset);
    static void test_tx_phase_noise_span(std::string &span);
    static void test_tx_phase_noise_rbw(std::string &rbw);
    static double test_tx_phase_noise(uint64_t freq,double offset);

public:
    static double cal_tx_lol();
    static double cal_tx_sb();
    static void cal_tx_filter_freq(range_freq_string &freq);
    static double cal_tx_filter_flatness();
    static double cal_tx_filter_ripple();

private:
    static std::string path();
};

} // namespace sp_rd

#endif // UTILITIES_TEST_DATA_HPP

