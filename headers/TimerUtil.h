#include <chrono>
#ifndef TIMER_UTIL_H
#define TIMER_UTIL_H

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

class TimerUtil {
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;   
    TimerUtil() {
        this->start = high_resolution_clock::now();
    }
    void startTimer() { 
        this->start = high_resolution_clock::now();
    }
    double stopTimer() { 
        auto end = high_resolution_clock::now();
        duration<double, std::milli> ms = end - start;
        return ms.count();
    }
};

#endif 