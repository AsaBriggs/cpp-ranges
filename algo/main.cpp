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
#include <deque>
#include <vector>

static const int ARR_SIZE = 10000 ;

template < typename Int >
struct MyInt
{
    Int val ;
    
    MyInt ()
        : val ()
    {}
    
    MyInt ( MyInt const& x )
        : val ( x.val )
    {}
    
    MyInt ( MyInt&& x )
        : val ( x.val )
    {}
    
    ~MyInt ()
    {}
    
    MyInt& operator= ( MyInt const& x )
    {
        this->val = x.val ;
        return *this ;
    }
    
    MyInt& operator= ( MyInt&& x )
    {
        this->val = x.val ;
        return *this ;
    }
};

template < typename T >
void copyTest ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    auto endIter = std::copy ( c.begin (), c.end (), output.begin () ) ;
    assert ( endIter == output.end () ) ;
    assert ( c == output ) ;
}

template < typename T >
void copyTestTimed ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;

    timer t ;
    t.start () ;
    
    for ( int i = 0 ; i < 1000000 ; ++i )
    {
        std::copy ( c.begin (), c.end (), output.begin () ) ;
    }
    
    std::cout << t.stop () << '\n' ;
}

template < typename T >
void copyBackwardsTestTimed ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    timer t ;
    t.start () ;
    
    for ( int i = 0 ; i < 1000000 ; ++i )
    {
        std::copy_backward ( c.begin (), c.end (), output.end () ) ;
    }
    std::cout << t.stop () << '\n' ;
}

template < typename T >
void copyBackwardsTest ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    auto endIter = std::copy_backward ( c.begin (), c.end (), output.end () ) ;
    
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


static_assert ( algo::IsNotProxiedIterator <int*>::value, "unexpected" );
static_assert ( !algo::IsNotProxiedIterator <std::vector<bool>::iterator>::value, "unexpected" );

typedef long ADType ;
typedef int IOType ;

const ADType ad0 = 10 ;
const ADType ad1 = 11 ;
const ADType ad2 = 12 ;
const ADType ad3 = 13 ;

const IOType io0 = 0 ;
const IOType io1 = 1 ;
const IOType io2 = 2 ;
const IOType io3 = 3 ;

struct TestStep
{
    mutable bool pre_op_ad_tag_called ;
    mutable int post_op_i_o_called ;
    
    TestStep ()
        : pre_op_ad_tag_called ( false )
        , post_op_i_o_called ( 0 )
    {}
    
    void operator () ( algo::pre_op_ad_tag, ADType* ad) const
    {
        assert ( !pre_op_ad_tag_called ) ;
        assert ( ad0 == *ad ) ;
        pre_op_ad_tag_called = true ;
        *ad = ad1 ;
    }
    
    void operator () ( algo::pre_op_i_tag, IOType* i) const
    {
        assert ( pre_op_ad_tag_called ) ;
        assert ( io0 == *i ) ;
        *i = io1 ;
    }
    
    void operator () ( algo::pre_op_o_tag, IOType* o) const
    {
        assert ( pre_op_ad_tag_called ) ;
        assert ( io0 == *o ) ;
        *o = io1 ;    }
    
    void operator () ( algo::Operation_tag, IOType* i, IOType* o, ADType* ad) const
    {
        assert ( ad1 == *ad ) ;
        assert ( io1 == *i ) ;
        assert ( io1 == *o ) ;

        *ad = ad2 ;
        *i = io2 ;
        *o = io2 ;
    }
    
    void operator () ( algo::post_op_i_tag, IOType* i) const
    {
        ++post_op_i_o_called ;
        assert ( io2 == *i ) ;
        *i = io3 ;
    }
    
    void operator () ( algo::post_op_o_tag, IOType* o) const
    {
        ++post_op_i_o_called ;
        assert ( io2 == *o ) ;
        *o = io3 ;
    }
    
    void operator () ( algo::post_op_ad_tag, ADType* ad) const
    {
        assert ( ad2 == *ad ) ;
        assert ( 2 == post_op_i_o_called ) ;
        *ad = ad3 ;
    }
};

void testStep ()
{
    ADType ad = ad0 ;
    IOType i = io0 ;
    IOType o = io0 ;
    
    ADType* adPtr = &ad ;
    IOType* iPtr = &i ;
    IOType* oPtr = &o ;
    
    algo::stepWithAuxilliaryData ( iPtr, oPtr, adPtr, TestStep () ) ;
    
    assert ( &ad == adPtr ) ;
    assert ( &i == iPtr ) ;
    assert ( &o == oPtr ) ;
    assert ( io3 == i );
    assert ( io3 == o ) ;
    assert ( ad3 == ad ) ;
}

int main(int argc, const char * argv[])
{
    copyTest < int > () ;
    copyBackwardsTest < int > () ;
    testStep () ;
    
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
    copyTestTimed < int > () ;
    copyBackwardsTestTimed < int > () ;
    
    return 0;
}

