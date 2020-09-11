#include <iostream>
#include <list>
#include "list.hpp"
#include <boost/timer/timer.hpp>

int main()
{
    const int n = 100000;
    int *a = nullptr;

    long long time_ctd = 0;
    long long time_std = 0;
    std::list<int *> std_list;

    boost::timer::cpu_timer time;

    while (1) {
        time.start();
        for (int i = 0;i < n;++i) {
            std_list.push_back(a);
        }
        time_std = time.elapsed().wall;
        std_list.clear();

        ctd::dl_list<int *> ctd_list;
        time.start();
        for (int i = 0;i < n;++i) {
            a = new int;
//            *a = i;
            ctd_list.push_back(a);
        }
        time_ctd = time.elapsed().wall;

        std::cout << time_std << std::endl;
        std::cout << time_ctd << std::endl;

        std::cout << (time_std > time_ctd ? "ctd" : "std") << " is faster!" << std::endl;
    }
}
