//
//  main.cpp
//  algo
//
//  Created by Asa Briggs on 01/07/2014.
//  Copyright (c) 2014 Asa Briggs. All rights reserved.
//

#include "algo.h"
#include "timer.h"

#include <array>
#include <cassert>
#include <iostream>
#include <numeric>

static const int ARR_SIZE = 10000 ;

void copyTest ()
{
    std::array < int, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < int, ARR_SIZE > output ;
    
    auto endIter = algo::copy ( c.begin (), c.end (), output.begin () ) ;
    assert ( endIter == output.end () ) ;
    assert ( c == output ) ;
}

void copyTestTimed ()
{
    std::array < int, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < int, ARR_SIZE > output ;

    timer t ;
    t.start () ;
    
    for ( int i = 0 ; i < 1000000 ; ++i )
    {
        algo::copy ( c.begin (), c.end (), output.begin () ) ;
    }
    
    std::cout << t.stop () << '\n' ;
}

void copyBackwardsTestTimed ()
{
    std::array < int, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < int, ARR_SIZE > output ;
    
    timer t ;
    t.start () ;
    
    for ( int i = 0 ; i < 1000000 ; ++i )
    {
        algo::copy_backward ( c.begin (), c.end (), output.end () ) ;
    }
    std::cout << t.stop () << '\n' ;
}

void copyBackwardsTest ()
{
    std::array < int, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < int, ARR_SIZE > output ;
    
    auto endIter = algo::copy_backward ( c.begin (), c.end (), output.end () ) ;
    
    assert ( endIter == output.begin () ) ;
    assert ( c == output ) ;
}

struct trivial {};

static_assert ( algo::IsBitwiseCopyable < int >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < float >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < bool >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < bool* >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < const bool* >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < trivial >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < std::pair < int, int > >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < std::array < int, 5 > >::value, "unexpectedly not trivially copyable" ) ;
static_assert ( algo::IsBitwiseCopyable < int [ 5 ] >::value, "unexpectedly not trivially copyable" ) ;


int main(int argc, const char * argv[])
{
    copyTest () ;
    copyBackwardsTest () ;
    
    /*
     algo
     1.5945e+09
     1.60146e+09
     Program ended with exit code: 0
     
     std
     1.62063e+09
     1.62001e+09
     Program ended with exit code: 0
     */
    copyTestTimed () ;
    copyBackwardsTestTimed () ;
    
    return 0;
}

