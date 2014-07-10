#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

// Taken from https://github.com/psoberoi/stepanov-conversations-course/blob/master/tape/timer.h

#if __cplusplus <= 199711L

#ifndef INCLUDED_TIME
#include <time.h>
#define INCLUDED_TIME
#endif

class timer {
private:
    clock_t start_time;
public:
    typedef double result_type;
    
    void start() {
        start_time = clock();
    }
    
    result_type stop() {
        return 1000000000. * ((clock() - start_time) / double(CLOCKS_PER_SEC));
    }
};

#else

#ifdef USE_CLANG_BUILTIN_TIMER

class timer {
private:
    unsigned long long start_time ;
public:
    void start() {
        start_time = __builtin_readcyclecounter () ;
    }
    
    double stop() {
        unsigned long long end = __builtin_readcyclecounter () ;
        return double ( end - start_time ) ;
    }
};

#else

#ifndef INCLUDED_CHRONO
#include <chrono>
#define INCLUDED_CHRONO
#endif

class timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    double stop() {
        auto stop_time = std::chrono::high_resolution_clock::now();
        return double(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count());
    }
};

#endif

#endif

#endif