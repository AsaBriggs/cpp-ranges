//
//  main.cpp
//  algo
//
//  Created by Asa Briggs on 01/07/2014.
//  Copyright (c) 2014 Asa Briggs. All rights reserved.
//

#include "algo.h"
#include "algo_sort.h"
#include "algo_range.h"
#include "algo_template_params.h"
#include "timer.h"

#include <array>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <map>

namespace test_common {
    
    template < typename Tag >
    struct RecordDestruction
    {
        bool* toMark ;
        
        explicit RecordDestruction ( bool& toMark )
        : toMark ( &toMark )
        {}
        
        ~RecordDestruction ()
        {
            *toMark = true ;
        }
    };
    
    typedef RecordDestruction < long > DestructionSkipped ;
    
    struct test_exception : std::exception
    {};
    
#ifdef NDEBUG
#define TEST_ASSERT(x) do { if ( (x) ) throw test_common::test_exception () ; } while ( 0 )
#else
#define TEST_ASSERT(x) assert(x)
#endif

}

namespace algo {
    
    template <>
    struct IsTriviallyDestructible < test_common::DestructionSkipped, ALGO_ENABLE_IF_PARAM_DEFAULT > : ALGO_LOGIC_CALL::true_type
    {} ;
    
}

namespace algo_basic_h {

// and_
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::and_< ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type>::type >::type::value ), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::and_< ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::false_type>::type >::type::value ), "unexpected" ) ;
    
// or_
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::or_< ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type>::type >::type::value ), "unexpected" ) ;
    
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::or_< ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::true_type>::type >::type::value ), "unexpected" ) ;

// if_
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::if_< ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::false_type>::type >::type::value ), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::if_< ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::false_type>::type >::type::value ), "unexpected" ) ;

// eval_if ... note makes use of the self-evaluating nature of the boolean integral constants.
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::eval_if< ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::false_type>::type >::type::value ), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::eval_if< ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::false_type>::type >::type::value ), "unexpected" ) ;
    
ALGO_STATIC_ASSERT ( (!ALGO_LOGIC_CALL::not_ < ALGO_LOGIC_CALL::true_type >::type::value ), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_LOGIC_CALL::not_ < ALGO_LOGIC_CALL::false_type >::type::value ), "" ) ;
    
const int expected_value = 1 ;
template < typename T > typename ALGO_LOGIC_CALL::enable_if < std::is_same < T, int >::type::value, int >::type enable ( T ) { return expected_value ; }
template < typename T > typename ALGO_LOGIC_CALL::enable_if < !std::is_same < T, int >::type::value, int >::type enable ( T ) { return expected_value + 1 ; }
    
template < typename T > typename ALGO_LOGIC_CALL::disable_if < std::is_same < T, int >::type::value, int >::type disable ( T ) { return expected_value + 1 ; }
template < typename T > typename ALGO_LOGIC_CALL::disable_if < !std::is_same < T, int >::type::value, int >::type disable ( T ) { return expected_value ; }
    
template < typename T > typename ALGO_LOGIC_CALL::enable_if_pred < std::is_same < T, int >, int >::type enable2 ( T ) { return expected_value ; }
template < typename T > typename ALGO_LOGIC_CALL::enable_if_pred < std::is_same < T, float >, int >::type enable2 ( T ) { return expected_value + 1 ; }
    
template < typename T > typename ALGO_LOGIC_CALL::disable_if_pred < std::is_same < T, int >, int >::type disable2 ( T ) { return expected_value + 1 ; }
template < typename T > typename ALGO_LOGIC_CALL::disable_if_pred < std::is_same < T, float >, int >::type disable2 ( T ) { return expected_value ; }
    
void test_enable_disable ()
{
    TEST_ASSERT ( expected_value == enable (1) ) ;
    TEST_ASSERT ( expected_value == disable (1) ) ;
    TEST_ASSERT ( expected_value == enable2 (1) ) ;
    TEST_ASSERT ( expected_value == disable2 (1) ) ;
}
    
void test_swap(bool swapValue)
{
    const int aOrig = 1 ;
    const int bOrig = 2 ;
    const int aExpected = swapValue ? bOrig : aOrig ;
    const int bExpected = swapValue ? aOrig : bOrig ;
    
    int a = aOrig ;
    int b = bOrig ;
    
    ALGO_CALL::swap_if ( swapValue, a, b, ALGO_CALL::Unpredictable () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    ALGO_CALL::swap_if ( swapValue, a, b, ALGO_CALL::Ternary () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    ALGO_CALL::swap_if ( swapValue, a, b, ALGO_CALL::Consistent () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    ALGO_CALL::swap_if ( swapValue, a, b, ALGO_CALL::PredictableTrue () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    ALGO_CALL::swap_if ( swapValue, a, b, ALGO_CALL::PredictableFalse () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    ALGO_CALL::swap_if ( swapValue, a, b, ALGO_CALL::NotInline < ALGO_CALL::PredictableFalse > () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
}

void test_select_if(bool x)
{
    const int ifFalse = 1 ;
    const int ifTrue = 2 ;
    
    const int selectedValue = ( x ? ifTrue : ifFalse ) ;
    
    TEST_ASSERT( selectedValue == ALGO_CALL::select_if(x, ifFalse, ifTrue, ALGO_CALL::Unpredictable () ) ) ;
    TEST_ASSERT( selectedValue == ALGO_CALL::select_if(x, ifFalse, ifTrue, ALGO_CALL::Ternary () ) ) ;
    TEST_ASSERT( selectedValue == ALGO_CALL::select_if(x, ifFalse, ifTrue, ALGO_CALL::Consistent () ) ) ;
    TEST_ASSERT( selectedValue == ALGO_CALL::select_if(x, ifFalse, ifTrue, ALGO_CALL::PredictableFalse () ) ) ;
    TEST_ASSERT( selectedValue == ALGO_CALL::select_if(x, ifFalse, ifTrue, ALGO_CALL::PredictableTrue () ) ) ;
    TEST_ASSERT( selectedValue == ALGO_CALL::select_if(x, ifFalse, ifTrue, ALGO_CALL::NotInline < ALGO_CALL::PredictableTrue > () ) ) ;
}

} // namespace algo_basic_h

namespace algo_traits_h {
    
struct trivial {};

ALGO_STATIC_ASSERT ( !ALGO_CALL::IsAPointer < int >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsAPointer < int* >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < int*, std::input_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < int*, std::forward_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < int*, std::bidirectional_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < int*, std::random_access_iterator_tag >::type::value), "unexpected" ) ;

typedef std::map < int, int >::iterator MapIterator ;

ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < MapIterator, std::input_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < MapIterator, std::forward_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < MapIterator, std::bidirectional_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (!ALGO_CALL::CheckIteratorCategory < MapIterator, std::random_access_iterator_tag >::type::value), "unexpected" ) ;


ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::DifferenceTypeTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::ValueTypeTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::PointerTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::ReferenceTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::IteratorCategoryTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::DifferenceTypeTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::ValueTypeTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::PointerTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::ReferenceTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::traits_test::IteratorCategoryTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::DifferenceTypeTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::ValueTypeTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::PointerTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::ReferenceTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::IteratorCategoryTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;

// Note need to go via ALGO_CALL::IteratorTraits for the T* const traits to be obtained
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::DifferenceTypeTest < ALGO_CALL::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::ValueTypeTest < ALGO_CALL::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::PointerTest < ALGO_CALL::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::ReferenceTest < ALGO_CALL::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::traits_test::IteratorCategoryTest < ALGO_CALL::IteratorTraits < char* const > >::type::value, "value not found!" ) ;

ALGO_STATIC_ASSERT( !ALGO_CALL::HasIteratorTraits < trivial >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !ALGO_CALL::HasIteratorTraits < char >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( ALGO_CALL::HasIteratorTraits < char const* >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT( ALGO_CALL::HasIteratorTraits < char const* const >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < int >::type::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < float >::type::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < bool >::type::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < bool* >::type::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < const bool* >::type::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < trivial >::type::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::IsBitwiseCopyable < std::pair < int, int > >::type::value), "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::IsBitwiseCopyable < std::array < int, 5 > >::type::value), "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < int [ 5 ] >::type::value, "unexpectedly not trivially copyable" ) ;


ALGO_STATIC_ASSERT ( ALGO_CALL::IsNotProxiedIterator <int*>::type::value, "unexpected" );
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsNotProxiedIterator <std::vector<bool>::iterator>::type::value, "unexpected" );

struct MyTestIterator {} ;
    
} // namespace algo_traits_h

namespace algo {

    template <>
    struct IteratorTraits < algo_traits_h::MyTestIterator > : std::iterator_traits < int* >
    {} ;
    
} // namespace algo

namespace algo_traits_h {
    
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < MyTestIterator >::type::value), "" ) ;
    ALGO_STATIC_ASSERT ( (!ALGO_CALL::IsRealStdIterator < MyTestIterator >::type::value), "" ) ;
    
    ALGO_STATIC_ASSERT ( (ALGO_CALL::IsRealStdIterator < int* >::type::value), "" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::IsRealStdIterator < int* const >::type::value), "" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::IsRealStdIterator < std::vector<bool>::iterator >::type::value), "" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::IsRealStdIterator < MapIterator >::type::value), "" ) ;
    
} // namespace algo_traits_h

namespace algo_template_params_h {
    
template < int N >
struct Tag {};

template < int N >
struct Value {} ;

// Template Params tests
// Exercise through the FindWrapper, to kill two birds in one stone.
typedef ALGO_CALL::Parameters <
    ALGO_CALL::Param < Tag < 0 >, Value < 0 > >
    , ALGO_CALL::Param < Tag < 1 >, Value < 1 > >
    , ALGO_CALL::Param < Tag < 2 >, Value < 2 > >
    , ALGO_CALL::Param < Tag < 3 >, Value < 3 > >
    , ALGO_CALL::Param < Tag < 4 >, Value < 4 > >
    , ALGO_CALL::Param < Tag < 5 >, Value < 5 > >
    , ALGO_CALL::Param < Tag < 6 >, Value < 6 > >
    , ALGO_CALL::Param < Tag < 7 >, Value < 7 > >
    , ALGO_CALL::Param < Tag < 8 >, Value < 8 > >
    , ALGO_CALL::Param < Tag < 9 >, Value < 9 > >
> TestParams ;

// Tag not in TestParams
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::FindWrapper< ALGO_CALL::Param < Tag < 10 >, Value < 10 > >, TestParams >::type, Value < 10 > >::type::value ), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::FindWrapper< ALGO_CALL::Param < Tag < 10 >, Value < 10 > >, ALGO_CALL::Parameters <> >::type, Value < 10 > >::type::value) , "unexpected" ) ;

typedef ALGO_CALL::Parameters <
    ALGO_CALL::Param < Tag < 0 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 1 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 2 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 3 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 4 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 5 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 6 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 7 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 8 >, Value < 10 > >
    , ALGO_CALL::Param < Tag < 9 >, Value < 10 > >
> DefaultParams ;

typedef ALGO_CALL::DeduceTypes < TestParams, DefaultParams >::type TestDeducedParams ;

ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param0, Value < 0 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param1, Value < 1 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param2, Value < 2 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param3, Value < 3 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param4, Value < 4 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param5, Value < 5 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param6, Value < 6 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param7, Value < 7 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param8, Value < 8 > >::type::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param9, Value < 9 > >::type::value) , "unexpected" ) ;

} // namespace algo_template_params_h

namespace algo_iterator_h {

void testPredecessor ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    TEST_ASSERT ( ALGO_CALL::predecessor(next, ALGO_CALL::ByReturnValue () ) == begin ) ;
    
    int* nextInPlace = begin + 1 ;
    ALGO_CALL::predecessor(nextInPlace, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( begin == nextInPlace ) ;
}

void testSuccessor ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    TEST_ASSERT ( ALGO_CALL::successor(begin, ALGO_CALL::ByReturnValue ()) == next ) ;
    
    int* beginInPlace = begin ;
    ALGO_CALL::successor(beginInPlace, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( next == beginInPlace ) ;
}

void testDistance ()
{
    static const int ARR_LENGTH = 2 ;
    int arr [ARR_LENGTH] = { 1, 2 } ;
    
    int* const begin = arr ;
    int* const end = begin + 2 ;
    
    TEST_ASSERT ( 2 == ALGO_CALL::distance ( begin, end ) ) ;
    TEST_ASSERT ( -2 == ALGO_CALL::distance ( end, begin ) ) ;
}

void testAdvance ()
{
    static const int ARR_LENGTH = 2 ;
    int arr [ARR_LENGTH] = { 1, 2 } ;
    
    int* const begin = arr ;
    int* const end = begin + 2 ;
    
    TEST_ASSERT ( end == ALGO_CALL::advance ( begin, 2, ALGO_CALL::ByReturnValue () ) ) ;
    TEST_ASSERT ( begin == ALGO_CALL::advance ( end, -2, ALGO_CALL::ByReturnValue () ) ) ;
    
    int* tmp = begin ;
    ALGO_CALL::advance(tmp, 2, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( end == tmp ) ;
    
    int* tmp2 = end ;
    ALGO_CALL::advance(tmp2, -2, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( begin == tmp2 ) ;
}

void testIsEmpty ()
{
    int arr [ 1 ] = { 1 } ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( arr ) ) ;
}

void testDeref ()
{
    static const int ARR_LENGTH = 2 ;
    int arr [ARR_LENGTH] = { 1, 2 } ;
    
    TEST_ASSERT ( ALGO_CALL::deref ( arr ) == arr [ 0 ] ) ;
}

void testAddressOf ()
{
    int a = 0 ;
    TEST_ASSERT ( &a == ALGO_CALL::addressOf ( a ) ) ;
}

void testUnderlyingAddressOf ()
{
    int arr [ 1 ] ;
    TEST_ASSERT ( arr == ALGO_CALL::underlyingAddressOf ( arr ) ) ;
    
    std::pair < const int, int > toInsert ( 1, 2 ) ;
    
    std::map < int, int > aMap ;
    aMap.insert ( toInsert ) ;
    
    auto iter = aMap.find ( toInsert.first ) ;
    TEST_ASSERT ( toInsert == *( ALGO_CALL::underlyingAddressOf ( iter ) ) ) ;
}

void testEqualUnderlyingAddress ()
{
    char arr [ sizeof ( unsigned long ) * 2 ] = {} ;
    
    char* begin = arr ;
    char* another = begin + sizeof ( unsigned long ) ;
    
    unsigned long* uBegin = reinterpret_cast < unsigned long* > ( begin ) ;
    unsigned long* uAnother = uBegin + 1 ;
    
    TEST_ASSERT ( ALGO_CALL::equalUnderlyingAddress ( begin, begin ) ) ;
    TEST_ASSERT ( !ALGO_CALL::equalUnderlyingAddress ( begin, another ) ) ;
    
    TEST_ASSERT ( ALGO_CALL::equalUnderlyingAddress ( uBegin, uBegin ) ) ;
    TEST_ASSERT ( !ALGO_CALL::equalUnderlyingAddress ( uBegin, uAnother ) ) ;
    
    TEST_ASSERT ( ALGO_CALL::equalUnderlyingAddress ( begin, uBegin ) ) ;
    TEST_ASSERT ( ALGO_CALL::equalUnderlyingAddress ( another, uAnother ) ) ;
}

void testDestroyPointed ()
{
    typedef test_common::RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    ALGO_CALL::destroyPointed ( toDestroy ) ;
    TEST_ASSERT ( destroyed ) ;
}

} // namespace algo_iterator_h

namespace algo_iterator_h {
    
void testDestroyPointed2 ()
{
    char arr[16];
    bool destroyed = false ;
    new (&arr) test_common::DestructionSkipped ( destroyed ) ;
    
    test_common::DestructionSkipped* const toDestroy = reinterpret_cast<test_common::DestructionSkipped*>(&arr);
    ALGO_CALL::destroyPointed ( toDestroy ) ;
    TEST_ASSERT ( !destroyed ) ;
    typedef test_common::DestructionSkipped DS ;
    
    toDestroy->~DS() ;
}

void testDestroyReferenced ()
{
    typedef test_common::RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    ALGO_CALL::destroyReferenced ( *toDestroy ) ;
    TEST_ASSERT ( destroyed ) ;
}

void testDestroyReferenced2 ()
{
    char arr[16];
    bool destroyed = false ;
    new (&arr) test_common::DestructionSkipped ( destroyed ) ;
    
    test_common::DestructionSkipped* const toDestroy = reinterpret_cast<test_common::DestructionSkipped*>(&arr);
    ALGO_CALL::destroyReferenced ( *toDestroy ) ;
    TEST_ASSERT ( !destroyed ) ;
    typedef test_common::DestructionSkipped DS ;
    toDestroy->~DS() ;
}


namespace operatorCounts
{

int moveConstructorCalled ;
int copyConstructorCalled ;
int moveAssignmentCalled ;
int copyAssignmentCalled ;

void resetCounts ()
{
    moveConstructorCalled = 0 ;
    copyConstructorCalled = 0 ;
    moveAssignmentCalled = 0 ;
    copyAssignmentCalled = 0 ;
}

struct TestType
{
    bool assignmentExpected ;
    
    TestType ( bool assignmentExpected )
        : assignmentExpected ( assignmentExpected )
    {}

    TestType ( TestType const& )
    {
        ++copyConstructorCalled ;
    }
    
    TestType ( TestType&& )
    {
        ++moveConstructorCalled ;
    }
    
    void operator= ( TestType const& )
    {
        TEST_ASSERT ( assignmentExpected ) ;
        ++copyAssignmentCalled ;
    }
    
    void operator= ( TestType const&& )
    {
        TEST_ASSERT ( assignmentExpected ) ;
        ++moveAssignmentCalled ;
    }
} ;
    
}

void testAssign ()
{
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    operatorCounts::TestType output ( true ) ;
    ALGO_CALL::assign ( &input, &output ) ;
    TEST_ASSERT ( 1 == operatorCounts::copyAssignmentCalled ) ;
}

void testMoveAssign ()
{
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    operatorCounts::TestType output ( true ) ;
    ALGO_CALL::moveAssign ( &input, &output ) ;
    TEST_ASSERT ( 1 == operatorCounts::moveAssignmentCalled ) ;
}

void testCopyConstruct ()
{
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    char buffer [ 16 ] ;
    operatorCounts::TestType* outputAddress = reinterpret_cast < operatorCounts::TestType* >( buffer ) ;
    
    ALGO_CALL::copyConstruct ( &input, outputAddress ) ;
    TEST_ASSERT ( 1 == operatorCounts::copyConstructorCalled ) ;
    outputAddress->~TestType() ;
}

void testMoveConstruct ()
{
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    char buffer [ 16 ] ;
    operatorCounts::TestType* outputAddress = reinterpret_cast < operatorCounts::TestType* >( buffer ) ;
    
    ALGO_CALL::moveConstruct ( &input, outputAddress ) ;
    TEST_ASSERT ( 1 == operatorCounts::moveConstructorCalled ) ;
    outputAddress->~TestType() ;
}

void testDerefMove ()
{
    // Located here to take advantage of the operatorCounts mechanism
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    operatorCounts::TestType output ( true ) ;
    output = ALGO_CALL::derefMove ( &input ) ;

    TEST_ASSERT ( 1 == operatorCounts::moveAssignmentCalled ) ;
    
    operatorCounts::resetCounts () ;
    char buffer [ 16 ] ;
    operatorCounts::TestType* outputAddress = reinterpret_cast < operatorCounts::TestType* >( buffer ) ;
    
    new ( outputAddress ) operatorCounts::TestType ( ALGO_CALL::derefMove ( &input ) ) ;
    TEST_ASSERT ( 1 == operatorCounts::moveConstructorCalled ) ;
    outputAddress->~TestType() ;
}

const int UNSWAPPED = 0 ;
const int SWAPPED_LHS = 2 ;
const int SWAPPED_RHS = 3 ;

namespace ANamespace
{
// Tests use of function swap as swapImpl will not be found
struct Swapable
{
    int* swapped ;
    
    void swapImpl ( Swapable& x )
    {
        TEST_ASSERT ( UNSWAPPED == *( this->swapped ) ) ;
        TEST_ASSERT ( UNSWAPPED == *( x.swapped ) ) ;
        *( this->swapped ) = SWAPPED_LHS ;
        *( x.swapped ) = SWAPPED_RHS ;
    }
};

void swap ( Swapable& lhs, Swapable& rhs )
{
    lhs.swapImpl ( rhs ) ;
}

// Tests discovery of member function swap using ADL
struct Swapable2
{
    int* swapped ;
    
    void swap ( Swapable& x )
    {
        TEST_ASSERT ( UNSWAPPED == *( this->swapped ) ) ;
        TEST_ASSERT ( UNSWAPPED == *( x.swapped ) ) ;
        *( this->swapped ) = SWAPPED_LHS ;
        *( x.swapped ) = SWAPPED_RHS ;
    }
};
    
}

void testSwapped ()
{
    int swap1 = UNSWAPPED ;
    int swap2 = UNSWAPPED ;
    
    ANamespace::Swapable tmp1 = { &swap1 } ;
    ANamespace::Swapable tmp2 = { &swap2 } ;
    
    ALGO_CALL::iterSwap ( &tmp1, &tmp2 ) ;
    
    TEST_ASSERT ( SWAPPED_LHS == swap1 ) ;
    TEST_ASSERT ( SWAPPED_RHS == swap2 ) ;
}

void testCopyBytesNotOverlapped ()
{
    std::array < int, 5 > const arr = { 1, 2, 3, 4, 5 } ;
    std::array < int, 5 > arr2 ;
    
    ALGO_CALL::copyBytesNotOverlapped ( arr.begin (), arr2.begin (), arr.size () ) ;
    TEST_ASSERT ( arr == arr2 ) ;
    
    std::array < int, 5 > arr3 = {} ;
    std::array < int, 5 > const arr4 = {} ;
    ALGO_CALL::copyBytesNotOverlapped ( arr.begin (),arr3.begin (), 0 ) ;
    TEST_ASSERT ( arr4 == arr3 ) ;
}

void testCopyBytesOverlapped ()
{
    std::array < int, 5 > const arr = { 1, 2, 3, 4, 5 } ;
    std::array < int, 5 > arr2 ;
    
    ALGO_CALL::copyBytesOverlapped ( arr.begin (), arr2.begin (), arr.size () ) ;
    TEST_ASSERT ( arr == arr2 ) ;
    
    // Test overlapping input/output
    ALGO_CALL::copyBytesOverlapped ( arr2.begin (), arr2.begin () + 1, arr.size () - 1 ) ;
    
    std::array < int, 5 > const expected = { 1, 1, 2, 3, 4 } ;
    TEST_ASSERT ( expected == arr2 ) ;
    
    ALGO_CALL::copyBytesOverlapped ( arr2.begin () + 2, arr2.begin (), arr.size () - 2 ) ;
    
    std::array < int, 5 > const expected2 = { 2, 3, 4, 3, 4 } ;
    TEST_ASSERT ( expected2 == arr2 ) ;
    
    std::array < int, 5 > arr3 = {} ;
    std::array < int, 5 > const arr4 = {} ;
    ALGO_CALL::copyBytesOverlapped ( arr.begin (), arr3.begin (), 0 ) ;
    TEST_ASSERT ( arr4 == arr3 ) ;
}

void testStripIter ()
{
    int arr [2] = {1,2};
    TEST_ASSERT ( &arr[0] == ALGO_CALL::stripIter ( &arr[0] ) ) ;
}

void testUnstripIter ()
{
    int arr [2] = {1,2};
    TEST_ASSERT ( &arr[0] == ALGO_CALL::unstripIter < int* > ( &arr[0] ) ) ;
}

} // namespace algo_iterator_h

namespace algo_h {

    struct TestStepTag {} ;
    
    typedef ALGO_CALL::BasicBoundedRange < char* >::type TestStepInputRange ;
    
    typedef ALGO_CALL::BasicUnboundedRange < char* >::type TestStepOutputRange ;
    
    struct StepDetails
    {
        static bool PreOpICalled ;
        static bool PostOpICalled ;
        static bool OperationCalled ;
        static bool PreOpOCalled ;
        static bool PostOpOCalled ;
        
        static TestStepInputRange* expectedInput ;
        static TestStepOutputRange* expectedOutput ;
        
        static void reset ( TestStepInputRange* expectedInputRange, TestStepOutputRange* expectedOutputRange )
        {
            PreOpICalled = false ;
            PostOpICalled = false ;
            OperationCalled = false ;
            PreOpOCalled = false ;
            PostOpOCalled = false ;
            expectedInput = expectedInputRange ;
            expectedOutput = expectedOutputRange ;
        }
        
        static void validate ()
        {
            TEST_ASSERT ( StepDetails::PreOpICalled ) ;
            TEST_ASSERT ( StepDetails::PreOpOCalled ) ;
            TEST_ASSERT ( StepDetails::OperationCalled ) ;
            TEST_ASSERT ( StepDetails::PostOpICalled ) ;
            TEST_ASSERT ( StepDetails::PostOpOCalled ) ;
        }
    };
    bool StepDetails::PreOpICalled ;
    bool StepDetails::PostOpICalled ;
    bool StepDetails::OperationCalled ;
    bool StepDetails::PreOpOCalled ;
    bool StepDetails::PostOpOCalled ;
    TestStepInputRange* StepDetails::expectedInput ;
    TestStepOutputRange* StepDetails::expectedOutput ;
    
    struct PreOpI
    {
        static void apply ( TestStepInputRange const& i )
        {
            TEST_ASSERT ( !StepDetails::PreOpICalled ) ;
            TEST_ASSERT ( !StepDetails::OperationCalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpICalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpOCalled ) ;
            TEST_ASSERT ( &i == StepDetails::expectedInput ) ;
            StepDetails::PreOpICalled = true ;
        }
    } ;
    
    struct PostOpI
    {
        static void apply ( TestStepInputRange const& i )
        {
            TEST_ASSERT ( StepDetails::PreOpICalled ) ;
            TEST_ASSERT ( StepDetails::PreOpOCalled ) ;
            TEST_ASSERT ( StepDetails::OperationCalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpICalled ) ;
            TEST_ASSERT ( &i == StepDetails::expectedInput ) ;
            StepDetails::PostOpICalled = true ;
        }
    } ;
    
    struct PreOpO
    {
        static void apply ( TestStepOutputRange const& o )
        {
            TEST_ASSERT ( !StepDetails::PreOpOCalled ) ;
            TEST_ASSERT ( !StepDetails::OperationCalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpICalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpOCalled ) ;
            TEST_ASSERT ( &o == StepDetails::expectedOutput ) ;
            StepDetails::PreOpOCalled = true ;
        }
    } ;
    
    struct PostOpO
    {
        static void apply ( TestStepOutputRange const& o )
        {
            TEST_ASSERT ( StepDetails::PreOpICalled ) ;
            TEST_ASSERT ( StepDetails::PreOpOCalled ) ;
            TEST_ASSERT ( StepDetails::OperationCalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpOCalled ) ;
            TEST_ASSERT ( &o == StepDetails::expectedOutput ) ;
            StepDetails::PostOpOCalled = true ;
        }
    } ;
    
    struct Operation
    {
        static void apply ( TestStepInputRange const& i, TestStepOutputRange const& o )
        {
            TEST_ASSERT ( StepDetails::PreOpICalled ) ;
            TEST_ASSERT ( StepDetails::PreOpOCalled ) ;
            TEST_ASSERT ( !StepDetails::OperationCalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpICalled ) ;
            TEST_ASSERT ( !StepDetails::PostOpOCalled ) ;
            TEST_ASSERT ( &i == StepDetails::expectedInput ) ;
            TEST_ASSERT ( &o == StepDetails::expectedOutput ) ;
            StepDetails::OperationCalled = true ;
        }
    } ;
    
} // namespace algo_h

namespace algo {
    
    namespace detail {
        
        template <>
        struct DeduceStepOperation < algo_h::TestStepInputRange, ALGO_DETAIL_CALL::pre_op_i_tag, algo_h::TestStepTag >
        {
            typedef algo_h::PreOpI type ;
        } ;
        
        template <>
        struct DeduceStepOperation < algo_h::TestStepInputRange, ALGO_DETAIL_CALL::post_op_i_tag, algo_h::TestStepTag >
        {
            typedef algo_h::PostOpI type ;
        } ;
        
        template <>
        struct DeduceStepOperation < algo_h::TestStepOutputRange, ALGO_DETAIL_CALL::pre_op_o_tag, algo_h::TestStepTag >
        {
            typedef algo_h::PreOpO type ;
        } ;
        
        template <>
        struct DeduceStepOperation < algo_h::TestStepOutputRange, ALGO_DETAIL_CALL::post_op_o_tag, algo_h::TestStepTag >
        {
            typedef algo_h::PostOpO type ;
        } ;
        
    } // namespace detail
    
} // namespace algo

namespace algo_h {
    
void teststep ()
{
    // Mainly tests DeduceStepOperation deduction, but also the order in which things are called.
    std::array < char, 10 > arr ;
    std::array < char, 10 > arr2 ;
    
    TestStepInputRange i = ALGO_CALL::deduceRange ( arr.begin (), arr.end () ) ;
    TestStepOutputRange o = ALGO_CALL::deduceRange ( arr2.begin () ) ;
    
    StepDetails::reset ( &i, &o ) ;
    
    ALGO_CALL::step < TestStepTag > ( i, o, Operation () ) ;
    
    StepDetails::validate () ;
}

static const int ARR_SIZE = 10000 ;

template < typename Int >
struct MyInt
{
    Int val ;
    
    MyInt ()
    : val ()
    {}
    
    MyInt ( Int const& x )
        : val ( x )
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
    
    MyInt& operator= (Int x )
    {
        this->val = x ;
        return *this ;
    }
    
    friend
    bool operator== ( MyInt const& x, MyInt const& y )
    {
        return x.val == y.val ;
    }
    
    friend
    bool operator!= ( MyInt const& x, MyInt const& y )
    {
        return !( x == y ) ;
    }

    friend
    MyInt operator+ ( MyInt const& x, MyInt const& y )
    {
        return x.val + y.val ;
    }
    
    friend
    std::ostream& operator<< ( std::ostream& o, MyInt const& x )
    {
        o << x.val ;
        return o ;
    }
};

typedef MyInt < int > NotABitwiseCopyableType ;
typedef MyInt < long > IsABitwiseCopyableType ;

ALGO_STATIC_ASSERT ( !ALGO_CALL::IsBitwiseCopyable < NotABitwiseCopyableType >::type::value, "Needs to be non-bitwise copyable for the tests" );

} // namespace algo_h

namespace algo {
    
    template <>
    struct IsBitwiseCopyable < algo_h::IsABitwiseCopyableType > : ALGO_LOGIC_CALL::true_type
    {} ;
    
} // namespace algo

namespace algo_h {
    
ALGO_STATIC_ASSERT ( ALGO_CALL::IsBitwiseCopyable < IsABitwiseCopyableType >::type::value, "Needs to be non-bitwise copyable for the tests" );

template < typename T >
void testCopy ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    auto endIter = ALGO_CALL::copy ( c.begin (), c.end (), output.begin () ) ;
    TEST_ASSERT ( endIter == output.end () ) ;
    TEST_ASSERT ( c == output ) ;
    
    auto endIter2 = ALGO_CALL::copy ( c.begin (), c.begin (), output.begin () ) ;
    TEST_ASSERT ( endIter2 == output.begin () ) ;
    
    auto endIter3 = ALGO_CALL::copy ( c.begin (), c.end (), c.begin () ) ;
    TEST_ASSERT ( endIter3 == c.end () ) ;
}

template < typename T >
void testCopyBackwards ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    auto endIter = ALGO_CALL::copy_backward ( c.begin (), c.end (), output.end () ) ;
    
    TEST_ASSERT ( endIter == output.begin () ) ;
    TEST_ASSERT ( c == output ) ;
    
    auto endIter2 = ALGO_CALL::copy_backward ( c.begin (), c.begin (), output.begin () ) ;
    TEST_ASSERT ( endIter2 == output.begin () ) ;
    
    auto endIter3 = ALGO_CALL::copy_backward ( c.begin (), c.end (), c.end () ) ;
    TEST_ASSERT ( endIter3 == c.begin () ) ;
}

template < typename T, size_t ARR_LENGTH >
void testFill ()
{
    const T FILL_VALUE = T ( 234 ) ;
    
    std::array < T, ARR_LENGTH > arr ;
    
    ALGO_CALL::fill ( arr.begin (), arr.end (), FILL_VALUE ) ;
    
    for ( auto i : arr )
    {
        TEST_ASSERT ( FILL_VALUE == i ) ;
    }
}




template < typename T >
void testCopyTimed ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    timer t ;
    t.start () ;
    
    for ( int i = 0 ; i < 10000 ; ++i )
    {
        ALGO_CALL::copy ( c.begin (), c.end (), output.begin () ) ;
    }
    
    std::cout << "testCopyTimed " << typeid ( T ).name () << ' ' << t.stop () << '\n' ;
}

template < typename T >
void testCopyBackwardsTimed ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    timer t ;
    t.start () ;
    
    for ( int i = 0 ; i < 10000 ; ++i )
    {
        ALGO_CALL::copy_backward ( c.begin (), c.end (), output.end () ) ;
    }
    
    std::cout << "testCopyBackwardsTimed " << typeid ( T ).name () << ' ' << t.stop () << '\n' ;
}



struct MinIterBounded
{
    template < typename Iter, typename Cmp >
    Iter operator () ( Iter f, Iter l, Cmp cmp ) const
    {
        return ALGO_CALL::minIter ( f, l, cmp ) ;
    }
} ;

struct MinIterCounted
{
    template < typename Iter, typename Cmp >
    Iter operator () ( Iter f, Iter l, Cmp cmp ) const
    {
        return ALGO_CALL::minIter ( f, l - f, cmp ) ;
    }
} ;

template < typename ToCall >
void testMinIter ()
{
    std::array < int, 10 > original = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 } ;
    
    std::array < int, 10 > arr = original ;
    auto i = ToCall () ( arr.begin (), arr.end (), std::less < int > () ) ;
    TEST_ASSERT ( i == &arr [ 9 ] ) ;
    
    auto i2 = ToCall () ( arr.begin (), arr.begin (), std::less < int > () ) ;
    TEST_ASSERT ( i2 == arr.begin () ) ;
    
    std::array < int, 10 > arr2 = {} ;
    auto i3 = ToCall () ( arr2.begin (), arr2.end (), std::less < int > () ) ;
    TEST_ASSERT ( i3 == arr2.begin () ) ;
}


struct MaxIterBounded
{
    template < typename Iter, typename Cmp >
    Iter operator () ( Iter f, Iter l, Cmp cmp ) const
    {
        return ALGO_CALL::maxIter ( f, l, cmp ) ;
    }
} ;

struct MaxIterCounted
{
    template < typename Iter, typename Cmp >
    Iter operator () ( Iter f, Iter l, Cmp cmp ) const
    {
        return ALGO_CALL::maxIter ( f, l - f, cmp ) ;
    }
} ;

template < typename ToCall >
void testMaxIter ()
{
    std::array < int, 10 > original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
    
    std::array < int, 10 > arr = original ;
    auto i = ToCall () ( arr.begin (), arr.end (), std::less < int > () ) ;
    TEST_ASSERT ( i == &arr [ 9 ] ) ;
    
    auto i2 = ToCall () ( arr.begin (), arr.begin (), std::less < int > () ) ;
    TEST_ASSERT ( i2 == arr.begin () ) ;
    
    std::array < int, 10 > arr2 = {} ;
    auto i3 = ToCall () ( arr2.begin (), arr2.end (), std::less < int > () ) ;
    TEST_ASSERT ( i3 == arr2.begin () + 9 ) ;
}

template < typename RotateChoice >
void testRotateRightByOne ()
{
    std::array < int, 2 > shortArray = { 9, 8 } ;
    ALGO_CALL::rotateRightByOne ( shortArray.begin (), shortArray.begin () + 1, RotateChoice () ) ;
    
    std::array < int, 2 > shortArrayResult = { 8, 9 } ;
    
    TEST_ASSERT ( shortArray == shortArrayResult ) ;
    
    std::array < int, 10 > const original = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 } ;
    std::array < int, 10 > arr = original ;
    
    // Rotate the 9th element to be the first.
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 } ;
    TEST_ASSERT ( res == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res2 = { 1, 0, 9, 8, 7, 6, 5, 4, 3, 2 } ;
    TEST_ASSERT ( res2 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res3 = { 2, 1, 0, 9, 8, 7, 6, 5, 4, 3 } ;
    TEST_ASSERT ( res3 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res4 = { 3, 2, 1, 0, 9, 8, 7, 6, 5, 4 } ;
    TEST_ASSERT ( res4 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res5 = { 4, 3, 2, 1, 0, 9, 8, 7, 6, 5 } ;
    TEST_ASSERT ( res5 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res6 = { 5, 4, 3, 2, 1, 0, 9, 8, 7, 6 } ;
    TEST_ASSERT ( res6 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res7 = { 6, 5, 4, 3, 2, 1, 0, 9, 8, 7  } ;
    TEST_ASSERT ( res7 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res8 = { 7, 6, 5, 4, 3, 2, 1, 0, 9, 8  } ;
    TEST_ASSERT ( res8 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res9 = { 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 } ;
    TEST_ASSERT ( res9 == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
    
    ALGO_CALL::rotateRightByOne (arr.begin (), arr.begin (), RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
}


template < typename RotateChoice >
void testRotateLeftByOne ()
{
    std::array < int, 2 > shortArray = { 9, 8 } ;
    ALGO_CALL::rotateLeftByOne ( shortArray.begin (), shortArray.begin () + 1, RotateChoice () ) ;
    
    std::array < int, 2 > shortArrayResult = { 8, 9 } ;
    
    TEST_ASSERT ( shortArray == shortArrayResult ) ;
    
    std::array < int, 10 > const original = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 } ;
    std::array < int, 10 > arr = original ;
    
    // Rotate the 9th element to be the first.
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res = { 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 } ;
    TEST_ASSERT ( res == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res2 = { 7, 6, 5, 4, 3, 2, 1, 0, 9, 8  } ;
    TEST_ASSERT ( res2 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res3 = { 6, 5, 4, 3, 2, 1, 0, 9, 8, 7  } ;
    TEST_ASSERT ( res3 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res4 = { 5, 4, 3, 2, 1, 0, 9, 8, 7, 6 } ;
    TEST_ASSERT ( res4 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res5 = { 4, 3, 2, 1, 0, 9, 8, 7, 6, 5 } ;
    TEST_ASSERT ( res5 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res6 = { 3, 2, 1, 0, 9, 8, 7, 6, 5, 4 } ;
    TEST_ASSERT ( res6 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res7 = { 2, 1, 0, 9, 8, 7, 6, 5, 4, 3 } ;
    TEST_ASSERT ( res7 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res8 = { 1, 0, 9, 8, 7, 6, 5, 4, 3, 2 } ;
    TEST_ASSERT ( res8 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res9 = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 } ;
    TEST_ASSERT ( res9 == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
    
    ALGO_CALL::rotateLeftByOne (arr.begin (), arr.begin (), RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
}


struct StdCopy
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        std::copy ( copyStart
                   , copyEnd
                   , copyToStart ) ;
    }
} ;
    
struct StdCopyReversed
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        typedef std::reverse_iterator < InputIter > RInputIter ;
        typedef std::reverse_iterator < OutputIter > ROuptutIter ;
        
        std::copy ( RInputIter ( copyEnd )
                   , RInputIter ( copyStart )
                   , ROuptutIter ( copyToEnd ) ) ;
    }
} ;
   
struct StepOverDeduced
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        typedef typename ALGO_CALL::BasicBoundedRange < InputIter >::type InputRange ;
        InputRange x = ALGO_CALL::deduceRange ( copyStart, copyEnd ) ;
        
        typedef typename ALGO_CALL::BasicBoundedRange < OutputIter >::type OutputRange ;
        OutputRange y = ALGO_CALL::deduceRange ( copyToStart, copyToEnd ) ;
        
        ALGO_CALL::stepOverDeduced ( x, y, ALGO_CALL::Assign < InputRange, OutputRange > () ) ;
    }
} ;
    
struct StepOverDeducedUnbounded
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        typedef typename ALGO_CALL::BasicBoundedRange < InputIter >::type InputRange ;
        InputRange x = ALGO_CALL::deduceRange ( copyStart, copyEnd ) ;
        
        typedef typename ALGO_CALL::BasicUnboundedRange < OutputIter >::type OutputRange ;
        OutputRange y = ALGO_CALL::deduceRange ( copyToStart ) ;
        
        ALGO_CALL::stepOverDeduced ( x, y, ALGO_CALL::Assign < InputRange, OutputRange > () ) ;
    }
} ;

    
struct StepOverDeducedInputCountedOutputUnbounded
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        typedef typename ALGO_CALL::BasicCountedRange < InputIter >::type InputRange ;
        InputRange x = ALGO_CALL::deduceRange ( copyStart, copyEnd - copyStart ) ;
        
        typedef typename ALGO_CALL::BasicUnboundedRange < OutputIter >::type OutputRange ;
        OutputRange y = ALGO_CALL::deduceRange ( copyToStart ) ;
        
        ALGO_CALL::stepOverDeduced ( x, y, ALGO_CALL::Assign < InputRange, OutputRange > () ) ;
    }
} ;
     
struct StepOverCounted
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        
        typedef typename ALGO_CALL::BasicCountedRange < InputIter >::type InputRange ;
        InputRange x = ALGO_CALL::deduceRange ( copyStart, copyEnd - copyStart ) ;
        
        typedef typename ALGO_CALL::BasicUnboundedRange < OutputIter >::type OutputRange ;
        OutputRange y = ALGO_CALL::deduceRange ( copyToStart ) ;
        
        typedef ALGO_CALL::Assign < InputRange, OutputRange > Op ;
        
        ALGO_CALL::StepOverCounted < InputRange, OutputRange, Op, ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag >::apply ( x, y, copyEnd - copyStart, Op () ) ;
    }
} ;
    
struct StepOverCountedUnbounded
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        
        typedef typename ALGO_CALL::BasicUnboundedRange < InputIter >::type InputRange ;
        InputRange x = ALGO_CALL::deduceRange ( copyStart ) ;
        
        typedef typename ALGO_CALL::BasicUnboundedRange < OutputIter >::type OutputRange ;
        OutputRange y = ALGO_CALL::deduceRange ( copyToStart ) ;
        
        typedef ALGO_CALL::Assign < InputRange, OutputRange > Op ;
        
        ALGO_CALL::StepOverCounted < InputRange, OutputRange, Op, ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag >::apply ( x, y, copyEnd - copyStart, Op () ) ;
    }
} ;

    
struct StepOverCountedIters
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        typedef ALGO_CALL::Assign < InputIter, OutputIter > Op ;
        
        ALGO_CALL::StepOverCounted < InputIter, OutputIter, Op, ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag >::apply ( copyStart, copyToStart, copyEnd - copyStart, Op () ) ;
    }
} ;
    
struct StepOverDeducedUnboundedReversed
{
    template < typename InputIter, typename OutputIter >
    ALGO_INLINE
    static void apply ( InputIter copyStart, InputIter copyEnd, OutputIter copyToStart, OutputIter copyToEnd )
    {
        typedef typename ALGO_CALL::BasicReversedRange < typename ALGO_CALL::BasicBoundedRange < InputIter >::type >::type InputRange ;
        InputRange x = ALGO_CALL::reverseRange ( ALGO_CALL::deduceRange ( copyStart, copyEnd ) ) ;
        
        typedef typename ALGO_CALL::BasicReversedRange < typename ALGO_CALL::BasicBoundedRange < OutputIter >::type >::type OutputRange ;
        OutputRange y = ALGO_CALL::reverseRange ( ALGO_CALL::deduceRange ( copyToStart, copyToEnd ) ) ;
        
        ALGO_CALL::stepOverDeduced ( x, y, ALGO_CALL::Assign < InputRange, OutputRange > () ) ;
    }
} ;
    
template < typename Operation, typename ContainedType >
void testStepPerformance ( const char* testName, const char* containedName )
{
    const ptrdiff_t BUFFER_SIZE = 1024 * 1024 * 1024 ;
    
    ALGO_CALL::AllocatingBuffer < ALGO_CALL::NewDeleteProtocol > buffer1 ( BUFFER_SIZE ) ;
    
    ALGO_CALL::AllocatingBuffer < ALGO_CALL::NewDeleteProtocol > buffer2 ( BUFFER_SIZE ) ;
    
    std::iota ( buffer1.template begin < ContainedType > (), buffer1.template end < ContainedType > (), 0 ) ;
    
    timer t ;
    
    t.start () ;
    for ( int i = 0 ; i < 10 ; ++i )
    {
        Operation::apply ( buffer1.template begin < ContainedType > ()
                          , buffer1.template end < ContainedType > ()
                          , buffer2.template begin < ContainedType > ()
                          ,  buffer2.template end < ContainedType > () ) ;
    }
    double const time = t.stop () ;
    
    ContainedType res = std::accumulate( buffer2.template begin < ContainedType > (), buffer2.template end < ContainedType > (), ContainedType ( 0 ) ) ;
    
    std::cout << " testStepPerformance " << testName << ' ' << containedName << ' ' << time << ' ' << res << '\n' ;
}
    
template < typename Operation >
void testStepPerformance ( const char* testName )
{
    testStepPerformance < Operation, unsigned int > ( testName, "unsigned int" ) ;
        
    ALGO_STATIC_ASSERT ( !std::is_trivially_copy_assignable < MyInt < unsigned int > >::type::value, "unexpected" ) ;
        
    testStepPerformance < Operation, MyInt < unsigned int > > ( testName, "MyInt < unsigned int >" ) ;
}
    
} // namespace algo_h

namespace algo_buffer_h {
    
    struct AlignOn256ByteBoundary
{
    char ar [ 256 ] ;
};
    
} // namespace algo_buffer_h

namespace algo {
    
    template <>
    struct AlignmentOf < algo_buffer_h::AlignOn256ByteBoundary, ALGO_ENABLE_IF_PARAM_DEFAULT > : ALGO_LOGIC_CALL::integral_constant < int, 256 >
    {} ;
    
} // namespace algo

namespace algo_buffer_h {
    
ALGO_STATIC_ASSERT ( 256 == ALGO_CALL::AlignmentOf < AlignOn256ByteBoundary >::value, "Not 256 byte aligned" ) ;

void testBufferCalculation ()
{
    // Trivial
    static const ptrdiff_t ARR_LENGTH = 1024 ;
    char arr [ ARR_LENGTH ] ;
    
    {
        ALGO_CALL::PointerAndSize data = { arr, 1 } ;
        char* const  begin = ALGO_CALL::BufferCalculation::calculateBegin < char > ( data ) ;
        TEST_ASSERT ( begin == arr ) ;
        
        char* const end = ALGO_CALL::BufferCalculation::calculateEnd < char > ( begin, data ) ;
        
        TEST_ASSERT ( arr + 1 == end ) ;
        TEST_ASSERT ( 1 == ( end - begin ) ) ;
    }
    
    {
        ALGO_CALL::PointerAndSize data = { arr, ARR_LENGTH } ;
        char* const  begin = ALGO_CALL::BufferCalculation::calculateBegin < char > ( data ) ;
        TEST_ASSERT ( begin == arr ) ;

        char* const end = ALGO_CALL::BufferCalculation::calculateEnd < char > ( begin, data ) ;

        TEST_ASSERT ( arr + ARR_LENGTH == end ) ;
        TEST_ASSERT ( ARR_LENGTH == ( end - begin ) ) ;
    }
    
    {
        // Now for something slightly more difficult
        ALGO_CALL::PointerAndSize data = { arr, ARR_LENGTH } ;
        
        ALGO_CALL::BufferRange <double> const range = ALGO_CALL::BufferCalculation::calculateRange < double > ( data ) ;
        TEST_ASSERT ( reinterpret_cast < void* > ( range.begin ) == reinterpret_cast < void* > ( arr ) ) ;
    
        ALGO_STATIC_ASSERT ( 0 == ( ARR_LENGTH % sizeof ( double ) ), "Test setup failure; array must be exactly divisible by sizeof ( double )" );
        TEST_ASSERT ( reinterpret_cast < void* > ( range.end ) == reinterpret_cast < void* > ( arr + ARR_LENGTH ) ) ;
        TEST_ASSERT ( ARR_LENGTH / sizeof ( double ) == ( range.end - range.begin ) ) ;
    }
}

void testBufferCalculationUnaligned ()
{
    const ptrdiff_t ARR_LENGTH = 1024 ;
    char arr [ ARR_LENGTH ] ;
    
    char* const bufferStart = &arr[1] ; // Deliberately pick an unaligned start address
    
    ALGO_CALL::PointerAndSize data = { bufferStart, ARR_LENGTH } ;
    
    AlignOn256ByteBoundary* const begin = ALGO_CALL::BufferCalculation::calculateBegin < AlignOn256ByteBoundary > ( data ) ;
    TEST_ASSERT ( begin > reinterpret_cast < AlignOn256ByteBoundary* > ( bufferStart ) ) ;
    
    ALGO_CALL::PointerAndSize data2 = { bufferStart, 1 } ;
    
    AlignOn256ByteBoundary* const nullBegin = ALGO_CALL::BufferCalculation::calculateBegin < AlignOn256ByteBoundary > ( data2 ) ;
    TEST_ASSERT ( ALGO_NULLPTR == nullBegin ) ;
    
    // Rather trick the calculateEnd function by passing in a null begin
    AlignOn256ByteBoundary* const nullEnd = ALGO_CALL::BufferCalculation::calculateEnd < AlignOn256ByteBoundary > ( nullBegin, data ) ;
    
    TEST_ASSERT ( ALGO_NULLPTR == nullEnd ) ;
    
    ALGO_CALL::PointerAndSize data3 = { bufferStart, ARR_LENGTH - 1 } ;
    
    AlignOn256ByteBoundary* const end2 = ALGO_CALL::BufferCalculation::calculateEnd < AlignOn256ByteBoundary > ( begin, data3 ) ;
    
    TEST_ASSERT ( 3 == ( end2 - begin ) ) ;
}

void testStackBuffer ()
{
    const ptrdiff_t ARR_LENGTH = 1024 ;
    ALGO_CALL::StackBuffer<ARR_LENGTH> buffer ;
    ALGO_CALL::BufferRange < char > const range = buffer.getRange<char>() ;
    
    TEST_ASSERT ( ARR_LENGTH == ( buffer.end < char > () - buffer.begin < char > () ) ) ;
    TEST_ASSERT ( buffer.begin < char > () == range.begin ) ;
    TEST_ASSERT ( buffer.end < char > () == range.end ) ;
    TEST_ASSERT ( ARR_LENGTH == ( buffer.end < char > () - buffer.begin < char > () ) ) ;
    TEST_ASSERT ( ( ARR_LENGTH / sizeof ( double ) ) == ( buffer.end < double > () - buffer.begin < double > () ) ) ;
    
    char* bufferAddr = buffer.begin < char > () ;
    AlignOn256ByteBoundary* const begin = buffer.begin < AlignOn256ByteBoundary > () ;
    AlignOn256ByteBoundary* const end = buffer.end < AlignOn256ByteBoundary > () ;
    const ptrdiff_t len = end - begin ;
    
    if ( 0 == ( uintptr_t ( bufferAddr ) % ALGO_CALL::AlignmentOf < AlignOn256ByteBoundary >::value ) )
    {
        TEST_ASSERT ( ( ARR_LENGTH / ALGO_CALL::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
    }
    else
    {
        TEST_ASSERT ( ( ( ARR_LENGTH - 1 ) / ALGO_CALL::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
    }
}

template < class Protocol >
void testBufferAllocationProtocol ( bool checkIfZeroed )
{
    const ptrdiff_t sizeToAllocate = 1024 * 1024 * 8 ;

    const ALGO_CALL::PointerAndSize data = Protocol::allocate ( sizeToAllocate ) ;
    TEST_ASSERT ( data.ptr ) ;
    TEST_ASSERT ( sizeToAllocate == data.size ) ;
    
    if ( checkIfZeroed )
    {
        for ( ptrdiff_t i = 0 ; i < sizeToAllocate ; ++i )
        {
            TEST_ASSERT ( 0 == data.ptr [ i ] ) ;
        }
    }
    
    // Write to the array, as a test of whether we have permission.
    for ( ptrdiff_t i = 0 ; i < sizeToAllocate ; ++i )
    {
        data.ptr [ i ] = 1 ;
    }
    
    Protocol::deallocate ( data ) ;
}


struct TestAllocatorProtocol
{
    static const ptrdiff_t TEST_BUFFER_SIZE = 1024 ;
    static char TEST_BUFFER [ TEST_BUFFER_SIZE ] ;
    
    static bool allocate_called ;
    static bool deallocate_called ;
    
    static void reset ()
    {
        allocate_called = false ;
        deallocate_called = false ;
    }
    
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        allocate_called = true ;
        return { TEST_BUFFER, TEST_BUFFER_SIZE } ;
    }
    
    static void deallocate ( ALGO_CALL::PointerAndSize data )
    {
        TEST_ASSERT ( TEST_BUFFER == data.ptr ) ;
        TEST_ASSERT ( TEST_BUFFER_SIZE == data.size ) ;
        deallocate_called = true ;
    }
};

char TestAllocatorProtocol::TEST_BUFFER [ TEST_BUFFER_SIZE ] ;

bool TestAllocatorProtocol::allocate_called ;
bool TestAllocatorProtocol::deallocate_called ;

void testAllocatingBuffer ()
{
    // Request one sized array, but get a smaller one (TestAllocatorProtocol::TEST_BUFFER_SIZE) back
    const ptrdiff_t ARR_LENGTH = TestAllocatorProtocol::TEST_BUFFER_SIZE * 2 ;
    
    TestAllocatorProtocol::reset () ;
    {
        ALGO_CALL::AllocatingBuffer < TestAllocatorProtocol > buffer ( ARR_LENGTH ) ;
        
        TEST_ASSERT ( TestAllocatorProtocol::allocate_called ) ;
        TEST_ASSERT ( !TestAllocatorProtocol::deallocate_called ) ;
        TestAllocatorProtocol::reset () ;
        
        
        TEST_ASSERT ( TestAllocatorProtocol::TEST_BUFFER_SIZE == ( buffer.end < char > () - buffer.begin < char > () ) ) ;
        TEST_ASSERT ( ( TestAllocatorProtocol::TEST_BUFFER_SIZE / sizeof ( double ) ) == ( buffer.end < double > () - buffer.begin < double > () ) ) ;
    
        char* bufferAddr = buffer.begin < char > () ;
        AlignOn256ByteBoundary* const begin = buffer.begin < AlignOn256ByteBoundary > () ;
        AlignOn256ByteBoundary* const end = buffer.end < AlignOn256ByteBoundary > () ;
        const ptrdiff_t len = end - begin ;
        
        ALGO_CALL::BufferRange<AlignOn256ByteBoundary> const range = buffer.getRange < AlignOn256ByteBoundary > () ;
        
        TEST_ASSERT ( range.begin == begin ) ;
        TEST_ASSERT ( range.end == end ) ;
    
        if ( 0 == ( uintptr_t ( bufferAddr ) % ALGO_CALL::AlignmentOf < AlignOn256ByteBoundary >::value ) )
        {
            TEST_ASSERT ( ( TestAllocatorProtocol::TEST_BUFFER_SIZE / ALGO_CALL::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
        }
        else
        {
            TEST_ASSERT ( ( ( TestAllocatorProtocol::TEST_BUFFER_SIZE - 1 ) / ALGO_CALL::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
        }
        
        TEST_ASSERT ( !TestAllocatorProtocol::allocate_called ) ;
        TEST_ASSERT ( !TestAllocatorProtocol::deallocate_called ) ;
    }
    // check that destruction of the buffer resulted in deallocation
    TEST_ASSERT ( !TestAllocatorProtocol::allocate_called ) ;
    TEST_ASSERT ( TestAllocatorProtocol::deallocate_called ) ;
}

void testObjectProctor ()
{
    typedef test_common::RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    
    {
        ALGO_CALL::ObjectProctor < DestructionType > proctor ( toDestroy ) ;
        proctor.disarm () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::ObjectProctor < DestructionType > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( destroyed ) ;
    
    destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    {
        ALGO_CALL::ObjectProctor < DestructionType > proctor ( *toDestroy ) ;
    }
    TEST_ASSERT ( destroyed ) ;
}

void testTrivialObjectProctor ()
{
    {
        int obj = 0 ;
        ALGO_CALL::ObjectProctor < int > proctor ( &obj ) ;
    }
    
    {
        int obj = 0 ;
        ALGO_CALL::ObjectProctor < int > proctor ( &obj ) ;
        proctor.disarm() ;
    }
    
    char arr[16];
    bool destroyed = false ;
    new (&arr) test_common::DestructionSkipped ( destroyed ) ;
    
    test_common::DestructionSkipped* const toDestroy = reinterpret_cast < test_common::DestructionSkipped* >( &arr ) ;

    {
        ALGO_CALL::ObjectProctor < test_common::DestructionSkipped > proctor ( toDestroy ) ;
        proctor.disarm () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::ObjectProctor < test_common::DestructionSkipped > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::ObjectProctor < test_common::DestructionSkipped > proctor ( *toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;

    typedef test_common::DestructionSkipped DS ;
    
    toDestroy->~DS () ;
}

void testIterProctorLengthOne ()
{
    typedef test_common::RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( destroyed ) ;
}

void testIterProctorLengthTwo ()
{
    typedef test_common::RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    bool destroyed2 = false ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    DestructionType* const toDestroy2 = toDestroy + 1 ;
    
    new (toDestroy) DestructionType ( destroyed ) ;
    new (toDestroy2) DestructionType ( destroyed2 ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    TEST_ASSERT ( !destroyed2 ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    TEST_ASSERT ( !destroyed2 ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( destroyed ) ;
    TEST_ASSERT ( !destroyed2 ) ;
    
    destroyed = false ;
    new (toDestroy) DestructionType ( destroyed ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    TEST_ASSERT ( destroyed2 ) ;
    
    destroyed2 = false ;
    new (toDestroy2) DestructionType ( destroyed2 ) ;
    
    {
        ALGO_CALL::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( destroyed ) ;
    TEST_ASSERT ( destroyed2 ) ;
}

void testTrivialIterProctor ()
{
    // Test the trivial (stateless) proctor
    
    {
        int obj = 0 ;
        ALGO_CALL::IterProctor < int* > proctor ( &obj ) ;
    }
    
    {
        int obj = 0 ;
        ALGO_CALL::IterProctor < int* > proctor ( &obj ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    
    char arr[16];
    bool destroyed = false ;
    new (&arr) test_common::DestructionSkipped ( destroyed ) ;
    
    test_common::DestructionSkipped* const toDestroy = reinterpret_cast < test_common::DestructionSkipped* >( &arr ) ;
    
    {
        ALGO_CALL::IterProctor < test_common::DestructionSkipped* > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::IterProctor < test_common::DestructionSkipped* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::IterProctor < test_common::DestructionSkipped* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    typedef test_common::DestructionSkipped DS ;
    
    toDestroy->~DS () ;
}

void testBufferProctorLengthOne ()
{
    typedef test_common::RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    
    {
        ALGO_CALL::BufferProctor < DestructionType > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::BufferProctor < DestructionType > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::BufferProctor < DestructionType > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( destroyed ) ;
}

void testTrivialBufferProctor ()
{
    // Test the trivial (stateless) proctor
    
    {
        int obj = 0 ;
        ALGO_CALL::BufferProctor < int > proctor ( &obj ) ;
    }
    
    {
        int obj = 0 ;
        ALGO_CALL::BufferProctor < int > proctor ( &obj ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    
    char arr[16];
    bool destroyed = false ;
    new (&arr) test_common::DestructionSkipped ( destroyed ) ;
    
    test_common::DestructionSkipped* const toDestroy = reinterpret_cast < test_common::DestructionSkipped* >( &arr ) ;
    
    {
        ALGO_CALL::BufferProctor < test_common::DestructionSkipped > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::BufferProctor < test_common::DestructionSkipped > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        ALGO_CALL::BufferProctor < test_common::DestructionSkipped > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    typedef test_common::DestructionSkipped DS ;
    
    toDestroy->~DS () ;
}

} // namespace algo_buffer_h

namespace algo_property_h {
    
struct Tag1 {} ;
typedef short Value1 ;

struct Tag2 {} ;
typedef double Value2 ;

struct Tag3 {} ;
typedef long Value3 ;

struct Tag4 {} ;
typedef std::string Value4 ;

typedef ALGO_CALL::ValueAndProperty < Tag1, Value1 > V1 ;
typedef ALGO_CALL::AddPropertyType < Tag2, Value2, V1 >::type V2 ;
typedef ALGO_CALL::AddPropertyType < Tag3, Value3, V2 >::type V3 ;
typedef ALGO_CALL::AddPropertyType < Tag4, Value4, V3 >::type V4 ;

template < typename Value >
void testRelationalOperations ( Value const& lesser, Value const& greater )
{
    TEST_ASSERT ( lesser == lesser ) ;
    TEST_ASSERT ( greater == greater ) ;
    TEST_ASSERT ( !( lesser == greater ) ) ;
    TEST_ASSERT ( !( greater == lesser ) ) ;

    TEST_ASSERT ( !( lesser != lesser ) ) ;
    TEST_ASSERT ( !( greater != greater ) ) ;
    TEST_ASSERT ( lesser != greater ) ;
    TEST_ASSERT ( greater != lesser ) ;
    
    TEST_ASSERT ( !( lesser < lesser ) ) ;
    TEST_ASSERT ( lesser <= lesser ) ;
    TEST_ASSERT ( !( lesser > lesser ) ) ;
    TEST_ASSERT ( lesser >= lesser ) ;
    
    TEST_ASSERT ( !( greater < greater ) ) ;
    TEST_ASSERT ( greater <= greater ) ;
    TEST_ASSERT ( !( greater > greater ) ) ;
    TEST_ASSERT ( greater >= greater ) ;
    
    TEST_ASSERT ( lesser < greater ) ;
    TEST_ASSERT ( !( greater < lesser ) ) ;
    TEST_ASSERT ( lesser <= greater ) ;
    TEST_ASSERT ( !( greater <= lesser ) ) ;
    TEST_ASSERT ( !( lesser > greater ) ) ;
    TEST_ASSERT ( greater > lesser ) ;
    TEST_ASSERT ( !( lesser >= greater ) ) ;
    TEST_ASSERT ( greater >= lesser ) ;
    
    // Now check that some address trick is not used for equality
    Value const lesser2 = lesser ;
    Value const greater2 = greater ;
    
    TEST_ASSERT ( lesser == lesser2 ) ;
    TEST_ASSERT ( lesser2 == lesser ) ;
    TEST_ASSERT ( greater == greater2 ) ;
    TEST_ASSERT ( greater2 == greater ) ;
    
    TEST_ASSERT ( !( lesser != lesser2 ) ) ;
    TEST_ASSERT ( !( lesser2 != lesser ) ) ;
    TEST_ASSERT ( !( greater != greater2 ) ) ;
    TEST_ASSERT ( !( greater2 != greater ) ) ;
    
    TEST_ASSERT ( !( lesser < lesser2 ) ) ;
    TEST_ASSERT ( !( lesser2 < lesser ) ) ;
    TEST_ASSERT ( lesser <= lesser2 ) ;
    TEST_ASSERT ( lesser2 <= lesser ) ;
    
    TEST_ASSERT ( !( lesser > lesser2 ) ) ;
    TEST_ASSERT ( !( lesser2 > lesser ) ) ;
    TEST_ASSERT ( lesser >= lesser2 ) ;
    TEST_ASSERT ( lesser2 >= lesser ) ;
    
    TEST_ASSERT ( !( greater < greater2 ) ) ;
    TEST_ASSERT ( !( greater2 < greater ) ) ;
    TEST_ASSERT ( greater <= greater2 ) ;
    TEST_ASSERT ( greater2 <= greater ) ;
    
    TEST_ASSERT ( !( greater > greater2 ) ) ;
    TEST_ASSERT ( !( greater2 > greater ) ) ;
    TEST_ASSERT ( greater >= greater2 ) ;
    TEST_ASSERT ( greater2 >= greater ) ;
}

void testValueAndPropertyRelationalOperations ()
{
    ALGO_CALL::ValueAndProperty < Tag1, Value1 > lesser = { 1 } ;
    ALGO_CALL::ValueAndProperty < Tag1, Value1 > greater = { 2 } ;
    
    testRelationalOperations ( lesser, greater ) ;
}

void testCompoundRelationalOperations ()
{
    typedef ALGO_CALL::Compound < long , double > TestType ;
    
    TestType v1 = { 123, 456.0 } ;
    TestType v2 = { 123, 457.0 } ;
    
    TestType v3 = { 124, 455.0 } ;
    TestType v4 = { 124, 456.0 } ;
    TestType v5 = { 124, 457.0 } ;
    
    
    testRelationalOperations ( v1, v2 ) ;
    testRelationalOperations ( v1, v3 ) ;
    testRelationalOperations ( v1, v4 ) ;
    testRelationalOperations ( v1, v5 ) ;
    
    testRelationalOperations ( v2, v3 ) ;
    testRelationalOperations ( v2, v4 ) ;
    testRelationalOperations ( v2, v5 ) ;
    
    testRelationalOperations ( v3, v4 ) ;
    testRelationalOperations ( v3, v5 ) ;
    
    testRelationalOperations ( v4, v5 ) ;
}

template < typename PropertyName, typename PropertyValue, typename PropertySet >
void testUpdate ( PropertyValue const& value, PropertySet& set, PropertySet const& constSet )
{
    PropertyValue originalValue = ALGO_CALL::getValue < PropertyName > ( set ) ;
    
    PropertySet updated = ALGO_CALL::setValue < PropertyName > ( set, value, ALGO_CALL::ByReturnValue () ) ;
    PropertySet updated2 = ALGO_CALL::setValue < PropertyName > ( constSet, value, ALGO_CALL::ByReturnValue () ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < PropertyName > ( set ) == originalValue ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < PropertyName > ( constSet ) == originalValue ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < PropertyName > ( updated ) == value ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < PropertyName > ( updated2 ) == value ) ;
    
    ALGO_CALL::setValue < PropertyName > ( set, value, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < PropertyName > ( set ) == value ) ;
}

void testProperty ()
{
    const Value1 value1 = 123 ;
    const Value2 value2 = 3.0 ;
    const Value3 value3 = 234923432 ;
    const Value4 value4 = "abcde" ;
    
    V1 a = { value1 } ;
    V1 const aConst = a ;
    
    V2 b = ALGO_CALL::addProperty < Tag2 > ( a, value2 ) ;
    V2 const bConst = b ;
    
    V3 c = ALGO_CALL::addProperty < Tag3 >( b, value3 ) ;
    V3 const cConst = c ;
    
    V4 d = ALGO_CALL::addProperty < Tag4 >( c, value4 ) ;
    V4 const dConst = d ;
    
    
    // Test HasProperty
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag1, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag1, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag1, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag1, V4>::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<Tag2, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag2, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag2, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag2, V4>::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<Tag3, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<Tag3, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag3, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag3, V4>::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<Tag4, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<Tag4, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<Tag4, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<Tag4, V4>::type ()), "unexpected" ) ;
    
    
    // test ValueType
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V1 >::type, Value1 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V2 >::type, Value1 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V3 >::type, Value1 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V4 >::type, Value1 >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag2, V2 >::type, Value2 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag2, V3 >::type, Value2 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag2, V4 >::type, Value2 >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag3, V3 >::type, Value3 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag3, V4 >::type, Value3 >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag4, V4 >::type, Value4 >::type ()), "unexpected" ) ;
    
    // Test the const version too
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V1 const >::type, Value1 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V2 const >::type, Value1 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V3 const >::type, Value1 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag1, V4 const >::type, Value1 const >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag2, V2 const >::type, Value2 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag2, V3 const >::type, Value2 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag2, V4 const >::type, Value2 const >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag3, V3 const >::type, Value3 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag3, V4 const >::type, Value3 const >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < ALGO_CALL::ValueType < Tag4, V4 const >::type, Value4 const >::type ()), "unexpected" ) ;
    
    
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( a ) == value1 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( b ) == value1 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( c ) == value1 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( d ) == value1 ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag2 > ( b ) == value2 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag2 > ( c ) == value2 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag2 > ( d ) == value2 ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag3 > ( c ) == value3 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag3 > ( d ) == value3 ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag4 > ( d ) == value4 ) ;
    
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( aConst ) == value1 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( bConst ) == value1 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( cConst ) == value1 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag1 > ( dConst ) == value1 ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag2 > ( bConst ) == value2 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag2 > ( cConst ) == value2 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag2 > ( dConst ) == value2 ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag3 > ( cConst ) == value3 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < Tag3 > ( dConst ) == value3 ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < Tag4 > ( dConst ) == value4 ) ;
    
    // By Reference
    TEST_ASSERT ( ALGO_CALL::getValueByReference < Tag4 > ( d ) == value4 ) ;
    TEST_ASSERT ( ALGO_CALL::getValueByReference < Tag4 > ( dConst ) == value4 ) ;
    
    
    const Value1 updatedValue1 = 128 ;
    const Value2 updatedValue2 =  0.1 ;
    const Value3 updatedValue3 = 12345 ;
    const Value4 updatedValue4 = "ghijkl" ;
    
    testUpdate < Tag1 > ( updatedValue1, a, aConst ) ;
    testUpdate < Tag1 > ( updatedValue1, b, bConst ) ;
    testUpdate < Tag1 > ( updatedValue1, c, cConst ) ;
    testUpdate < Tag1 > ( updatedValue1, d, dConst ) ;
    
    testUpdate < Tag2 > ( updatedValue2, b, bConst ) ;
    testUpdate < Tag2 > ( updatedValue2, c, cConst ) ;
    testUpdate < Tag2 > ( updatedValue2, d, dConst ) ;
    
    testUpdate < Tag3 > ( updatedValue3, c, cConst ) ;
    testUpdate < Tag3 > ( updatedValue3, d, dConst ) ;
    
    testUpdate < Tag4 > ( updatedValue4, d, dConst ) ;
    
    // Test that the equality syntax works.
    
    V1 aExpected = { updatedValue1 } ;
    V2 bExpected = ALGO_CALL::addProperty < Tag2 > ( aExpected, updatedValue2 ) ;
    V3 cExpected = ALGO_CALL::addProperty < Tag3 >( bExpected, updatedValue3 ) ;
    V4 dExpected = ALGO_CALL::addProperty < Tag4 >( cExpected, updatedValue4 ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    TEST_ASSERT ( cExpected == c ) ;
    TEST_ASSERT ( dExpected == d ) ;
    
    {
        // Test the aggregate construction syntax.
        V1 a2 = { updatedValue1 } ;
        TEST_ASSERT ( a2 == a ) ;
    
        V2 b2 = { a2, updatedValue2 } ;
        V2 b3 = { updatedValue1, updatedValue2 } ;
        TEST_ASSERT ( b2 == b ) ;
        TEST_ASSERT ( b3 == b ) ;
    
        V3 c2 = { b2, updatedValue3 } ;
        V3 c3 = { a2, updatedValue2, updatedValue3 } ;
        V3 c4 = { updatedValue1, updatedValue2, updatedValue3 } ;
        TEST_ASSERT ( c2 == c ) ;
        TEST_ASSERT ( c3 == c ) ;
        TEST_ASSERT ( c4 == c ) ;
    
        V4 d2 = { c2, updatedValue4 } ;
        V4 d3 = { b2, updatedValue3, updatedValue4 } ;
        V4 d4 = { a2, updatedValue2, updatedValue3, updatedValue4 } ;
        V4 d5 = { updatedValue1, updatedValue2, updatedValue3, updatedValue4 } ;
        TEST_ASSERT ( d2 == d ) ;
        TEST_ASSERT ( d3 == d ) ;
        TEST_ASSERT ( d4 == d ) ;
        TEST_ASSERT ( d5 == d ) ;
    }
    
    
    // Test addOrUpdateValue
    {
        // Test the AddValue
        V2 val2 = ALGO_CALL::addOrUpdateValue < Tag2 > ( aConst, value2 ) ;
        TEST_ASSERT ( val2 == bConst ) ;
        
        Value2 value2ToMove = value2 ;
        V2 val2a = ALGO_CALL::addOrUpdateValue < Tag2 > ( aConst, ALGO_CALL::move ( value2ToMove ) ) ;
        TEST_ASSERT ( val2a == bConst ) ;
        
        // OK, try this on a moveable type ... Value4 is a string
        Value4 value4a = value4 ;
        
        TEST_ASSERT ( dConst == ALGO_CALL::addOrUpdateValue < Tag4 > ( cConst, value4 ) ) ;
        TEST_ASSERT ( dConst == ALGO_CALL::addOrUpdateValue < Tag4 > ( cConst, value4a ) ) ;
        TEST_ASSERT ( dConst == ALGO_CALL::addOrUpdateValue < Tag4 > ( cConst, ALGO_CALL::move ( value4a ) ) ) ;
        
        // Now test the UpdateValue paths
        V4 tmp = { updatedValue1, updatedValue2, updatedValue3, value4 } ;
        V4 tmp2 = ALGO_CALL::addOrUpdateValue < Tag4 > ( tmp, updatedValue4 ) ;
        
        TEST_ASSERT ( value4 == ALGO_CALL::getValueByReference < Tag4 > ( tmp ) ) ;
        TEST_ASSERT ( updatedValue4 == ALGO_CALL::getValueByReference < Tag4 > ( tmp2 ) ) ;
    }
    
    // test mergePropertySets
    {
        typedef ALGO_CALL::ValueAndProperty < Tag1, Value1 > VP1 ;
        typedef ALGO_CALL::ValueAndProperty < Tag2, Value2 > VP2 ;
        typedef ALGO_CALL::ValueAndProperty < Tag3, Value3 > VP3 ;
        typedef ALGO_CALL::ValueAndProperty < Tag4, Value4 > VP4 ;
        typedef ALGO_CALL::Compound < VP1, VP2 > C1 ;
        typedef ALGO_CALL::Compound < VP3, VP4 > C2 ;
        
        C1 compound1 = { value1, value2 } ;
        C2 compound2 = { value3, value4 } ;
        
        ALGO_CALL::MergePropertySetsType < C1, C2 >::type tmp = ALGO_CALL::mergePropertySets ( compound1, compound2 ) ;
        
        TEST_ASSERT ( value1 == ALGO_CALL::getValueByReference < Tag1 > ( tmp ) ) ;
        TEST_ASSERT ( value2 == ALGO_CALL::getValueByReference < Tag2 > ( tmp ) ) ;
        TEST_ASSERT ( value3 == ALGO_CALL::getValueByReference < Tag3 > ( tmp ) ) ;
        TEST_ASSERT ( value4 == ALGO_CALL::getValueByReference < Tag4 > ( tmp ) ) ;
        
    }
}

const Value1 expectedValue1 = 123 ;
const Value2 expectedValue2 = 3.0 ;
const Value3 expectedValue3 = 234923432 ;
const Value4 expectedValue4 = "abcde" ;

struct TestVisitor
{
    bool visitedTag1 ;
    bool visitedTag2 ;
    bool visitedTag3 ;
    bool visitedTag4 ;
    
    void visit ( Value1 const& x, Tag1 )
    {
        visitedTag1 = true ;
        TEST_ASSERT ( x == expectedValue1 ) ;
    }
    
    void visit ( Value2 const& x, Tag2 )
    {
        visitedTag2 = true ;
        TEST_ASSERT ( x == expectedValue2 ) ;
    }
    
    void visit ( Value3 const& x, Tag3 )
    {
        visitedTag3 = true ;
        TEST_ASSERT ( x == expectedValue3 ) ;
    }
    
    void visit ( Value4 const& x, Tag4 )
    {
        visitedTag4 = true ;
        TEST_ASSERT ( x == expectedValue4 ) ;
    }
    
    
    template < typename PropertyName, typename ValueType >
    void visit ( ValueType const& v )
    {
        this->visit ( v, PropertyName () ) ;
    }
} ;

void testVisitProperty ()
{
    V4 a = { expectedValue1, expectedValue2, expectedValue3, expectedValue4 } ;
    TestVisitor tmp = { false, false, false, false } ;
    TestVisitor x = ALGO_CALL::visit(a, tmp ) ;
    
    TEST_ASSERT ( x.visitedTag1 ) ;
    TEST_ASSERT ( x.visitedTag2 ) ;
    TEST_ASSERT ( x.visitedTag3 ) ;
    TEST_ASSERT ( x.visitedTag4 ) ;
}
    
template < int N >
struct TagN {} ;

typedef unsigned long long TaggedValueType ;

typedef ALGO_CALL::ValueAndProperty < TagN < 0 >, TaggedValueType > TV0 ;
typedef ALGO_CALL::AddPropertyType < TagN < 1 >, TaggedValueType, TV0 >::type TV1 ;
typedef ALGO_CALL::AddPropertyType < TagN < 2 >, TaggedValueType, TV1 >::type TV2 ;
typedef ALGO_CALL::AddPropertyType < TagN < 3 >, TaggedValueType, TV2 >::type TV3 ;
typedef ALGO_CALL::AddPropertyType < TagN < 4 >, TaggedValueType, TV3 >::type TV4 ;
typedef ALGO_CALL::AddPropertyType < TagN < 5 >, TaggedValueType, TV4 >::type TV5 ;
typedef ALGO_CALL::AddPropertyType < TagN < 6 >, TaggedValueType, TV5 >::type TV6 ;
typedef ALGO_CALL::AddPropertyType < TagN < 7 >, TaggedValueType, TV6 >::type TV7 ;

struct EightValues
{
    TaggedValueType m0 ;
    TaggedValueType m1 ;
    TaggedValueType m2 ;
    TaggedValueType m3 ;
    TaggedValueType m4 ;
    TaggedValueType m5 ;
    TaggedValueType m6 ;
    TaggedValueType m7 ;
};


typedef ALGO_CALL::ValueAndProperty < TagN < 0 >, TaggedValueType > TAV0 ;
typedef ALGO_CALL::ValueAndProperty < TagN < 1 >, TaggedValueType > TAV1 ;
typedef ALGO_CALL::ValueAndProperty < TagN < 2 >, TaggedValueType > TAV2 ;
typedef ALGO_CALL::ValueAndProperty < TagN < 3 >, TaggedValueType > TAV3 ;
typedef ALGO_CALL::ValueAndProperty < TagN < 4 >, TaggedValueType > TAV4 ;
typedef ALGO_CALL::ValueAndProperty < TagN < 5 >, TaggedValueType > TAV5 ;
typedef ALGO_CALL::ValueAndProperty < TagN < 6 >, TaggedValueType > TAV6 ;
typedef ALGO_CALL::ValueAndProperty < TagN < 7 >, TaggedValueType > TAV7 ;

typedef ALGO_CALL::Compound < TAV0, TAV1 > TAV01 ;
typedef ALGO_CALL::Compound < TAV2, TAV3 > TAV23 ;
typedef ALGO_CALL::Compound < TAV4, TAV5 > TAV45 ;
typedef ALGO_CALL::Compound < TAV6, TAV7 > TAV67 ;


typedef ALGO_CALL::Compound < TAV01, TAV23 > TAV0123 ;
typedef ALGO_CALL::Compound < TAV45, TAV67 > TAV4567 ;

typedef ALGO_CALL::Compound < TAV0123, TAV4567 > TAV01234567 ;

inline TaggedValueType selectRandom ( EightValues const& x, ptrdiff_t index )
{
    switch ( index )
    {
        case 0 :
            return x.m0 ;
        case 1 :
            return x.m1 ;
        case 2 :
            return x.m2 ;
        case 3 :
            return x.m3 ;
        case 4 :
            return x.m4 ;
        case 5 :
            return x.m5 ;
        case 6 :
            return x.m6 ;
        case 7 :
            return x.m7 ;
        default :
            return 0 ;
    }
}

inline void writeRandom ( EightValues& x, TaggedValueType y, ptrdiff_t index )
{
    switch ( index )
    {
        case 0 :
            x.m0 = y ;
            break ;
        case 1 :
            x.m1 = y ;
            break ;
        case 2 :
            x.m2 = y ;
            break ;
        case 3 :
            x.m3 = y ;
            break ;
        case 4 :
            x.m4 = y ;
        case 5 :
            x.m5 = y ;
        case 6 :
            x.m6 = y ;
        case 7 :
            x.m7 = y ;
        default:
            break ;
    }
}

template < class T >
inline TaggedValueType selectRandom ( T const& x, ptrdiff_t index )
{
    switch ( index )
    {
        case 0 :
            return ALGO_CALL::GetValue < TagN < 0 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        case 1 :
            return ALGO_CALL::GetValue < TagN < 1 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        case 2 :
            return ALGO_CALL::GetValue < TagN < 2 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        case 3 :
            return ALGO_CALL::GetValue < TagN < 3 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        case 4 :
            return ALGO_CALL::GetValue < TagN < 4 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        case 5 :
            return ALGO_CALL::GetValue < TagN < 5 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        case 6 :
            return ALGO_CALL::GetValue < TagN < 6 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        case 7 :
            return ALGO_CALL::GetValue < TagN < 7 >, ALGO_CALL::ByValue, T const >::apply ( x ) ;
        default :
            return 0 ;
    }
}

template < class T >
inline void writeRandom ( T& x, TaggedValueType y, ptrdiff_t index )
{
    switch ( index )
    {
        case 0 :
            ALGO_CALL::SetValue < TagN < 0 >, T >::apply ( x, y ) ;
            break ;
        case 1 :
            ALGO_CALL::SetValue < TagN < 1 >, T >::apply ( x, y ) ;
            break ;
        case 2 :
            ALGO_CALL::SetValue < TagN < 2 >, T >::apply ( x, y ) ;
            break ;
        case 3 :
            ALGO_CALL::SetValue < TagN < 3 >, T >::apply ( x, y ) ;
            break ;
        case 4 :
            ALGO_CALL::SetValue < TagN < 4 >, T >::apply ( x, y ) ;
            break ;
        case 5 :
            ALGO_CALL::SetValue < TagN < 5 >, T >::apply ( x, y ) ;
            break ;
        case 6 :
            ALGO_CALL::SetValue < TagN < 6 >, T >::apply ( x, y ) ;
            break ;
        case 7 :
            ALGO_CALL::SetValue < TagN < 7 >, T >::apply ( x, y ) ;
            break ;
        default:
            break ;
    }
}

void propertiesPerformanceTest ()
{
    EightValues a = { 0, 1, 2, 3, 4, 5, 6, 7 } ;
    TV7 b = { 0, 1, 2, 3, 4, 5, 6, 7 } ;
    TAV01234567 c = { 0, 1, 2, 3, 4, 5, 6, 7 } ;
    
    TaggedValueType aAccumulated = 0 ;
    TaggedValueType bAccumulated = 0 ;
    TaggedValueType cAccumulated = 0 ;
    const size_t NUMBER_OF_RUNS = 100000000 ;
    
    double totalTimeA = 0.0;
    double totalTimeB = 0.0;
    double totalTimeC = 0.0;
    timer t;
    
    std::vector < ptrdiff_t > randomArray ;
    randomArray.resize ( NUMBER_OF_RUNS ) ;
    ptrdiff_t* iter = &( randomArray [ 0 ] ) ;
    
    for ( int i = 0 ; i < NUMBER_OF_RUNS ; ++i )
    {
        *iter++ = std::rand () % 8 ;
    }
    
    t.start () ;
    for ( auto i : randomArray )
    {
        aAccumulated += selectRandom ( a, i ) ;
        writeRandom ( a, aAccumulated, i ) ;
    }
    totalTimeA = t.stop () ;
    
    t.start () ;
    for ( auto i : randomArray )
    {
        bAccumulated += selectRandom ( b, i ) ;
        writeRandom ( b, bAccumulated, i ) ;
    }
    totalTimeB = t.stop () ;
    
    t.start () ;
    for ( auto i : randomArray )
    {
        cAccumulated += selectRandom ( c, i ) ;
        writeRandom ( c, cAccumulated, i ) ;
    }
    totalTimeC = t.stop () ;

    std::cout << "propertiesPerformanceTest struct " << totalTimeA << ' ' << aAccumulated
                << ", list " << totalTimeB << ' ' << bAccumulated
                << ", balanced tree " << totalTimeC << ' ' << cAccumulated << '\n' ;
}
typedef ALGO_CALL::RemoveProperty < TagN < 1 >, TV0 >::type TV0A ;
ALGO_STATIC_ASSERT( (std::is_same < TV0A, TV0 >::type::value), "" ) ;
    
typedef ALGO_CALL::RemoveProperty < TagN < 2 >, TAV01 >::type TAV01A ;
ALGO_STATIC_ASSERT( (std::is_same < TAV01A, TAV01 >::type::value), "" ) ;
    
// Removed from M0 of a Compound
typedef ALGO_CALL::RemoveProperty < TagN < 0 >, TAV01234567 >::type TAV01234567NoTag0 ;
    
// Removed from M0 of a Compound
typedef ALGO_CALL::RemoveProperty < TagN < 7 >, TAV01234567NoTag0 >::type TAV01234567NoTag0NoTag7 ;
    
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 1 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 2 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 3 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 4 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 5 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 6 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 7 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<TagN < 0 >, TAV01234567NoTag0 >::type::value), "" ) ;
    
    
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 1 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 2 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 3 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 4 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 5 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty<TagN < 6 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<TagN < 0 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty<TagN < 7 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;

void testConvertPropertySet ()
{
    TV7 val = {} ;
    ALGO_CALL::setValue < TagN < 0 > > ( val, 1, ALGO_CALL::InPlace () ) ;
    ALGO_CALL::setValue < TagN < 1 > > ( val, 2, ALGO_CALL::InPlace () ) ;
    ALGO_CALL::setValue < TagN < 2 > > ( val, 3, ALGO_CALL::InPlace () ) ;
    ALGO_CALL::setValue < TagN < 3 > > ( val, 4, ALGO_CALL::InPlace () ) ;
    ALGO_CALL::setValue < TagN < 4 > > ( val, 5, ALGO_CALL::InPlace () ) ;
    ALGO_CALL::setValue < TagN < 5 > > ( val, 6, ALGO_CALL::InPlace () ) ;
    ALGO_CALL::setValue < TagN < 6 > > ( val, 7, ALGO_CALL::InPlace () ) ;
    ALGO_CALL::setValue < TagN < 7 > > ( val, 8, ALGO_CALL::InPlace () ) ;
    
    TV7 const val2 = ALGO_CALL::convertPropertySet < TV7 > ( val ) ;
    TEST_ASSERT ( val2 == val ) ;
    
    TV6 const val3 = ALGO_CALL::convertPropertySet < TV6 > ( val ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 0 > > ( val3 ) == ALGO_CALL::getValue < TagN < 0 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 1 > > ( val3 ) == ALGO_CALL::getValue < TagN < 1 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 2 > > ( val3 ) == ALGO_CALL::getValue < TagN < 2 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 3 > > ( val3 ) == ALGO_CALL::getValue < TagN < 3 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 4 > > ( val3 ) == ALGO_CALL::getValue < TagN < 4 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 5 > > ( val3 ) == ALGO_CALL::getValue < TagN < 5 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 6 > > ( val3 ) == ALGO_CALL::getValue < TagN < 6 > > ( val ) ) ;
    
    TV7 const val4 = ALGO_CALL::convertPropertySet < TV7 > ( val3 ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 0 > > ( val4 ) == ALGO_CALL::getValue < TagN < 0 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 1 > > ( val4 ) == ALGO_CALL::getValue < TagN < 1 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 2 > > ( val4 ) == ALGO_CALL::getValue < TagN < 2 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 3 > > ( val4 ) == ALGO_CALL::getValue < TagN < 3 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 4 > > ( val4 ) == ALGO_CALL::getValue < TagN < 4 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 5 > > ( val4 ) == ALGO_CALL::getValue < TagN < 5 > > ( val ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 6 > > ( val4 ) == ALGO_CALL::getValue < TagN < 6 > > ( val ) ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < TagN < 7 > > ( val4 ) == 0 ) ;
}

} // namespace algo_property_h

namespace algo_range_h {

typedef ALGO_CALL::BasicUnboundedRange < int* >::type AnUnboundedRange ;

ALGO_STATIC_ASSERT ( ALGO_CALL::IsARange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsABoundedRange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsACountedRange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsAFiniteRange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsANonFiniteRange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::RepeatableIterator < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, ALGO_CALL::StartIteratorType < AnUnboundedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::HasCountO1Time < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsAReversedRange < AnUnboundedRange >::type::value, "unexpected" ) ;

typedef ALGO_CALL::BasicCountedRange < int* >::type ACountedRange ;

ALGO_STATIC_ASSERT ( ALGO_CALL::IsARange < ACountedRange>::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsABoundedRange < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsACountedRange < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsAFiniteRange < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsANonFiniteRange < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::RepeatableIterator < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, ALGO_CALL::StartIteratorType < ACountedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < ptrdiff_t, ALGO_CALL::CountType < ACountedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::HasCountO1Time < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsAReversedRange < ACountedRange >::type::value, "unexpected" ) ;

typedef ALGO_CALL::BasicBoundedRange < int* >::type ABoundedRange ;

ALGO_STATIC_ASSERT ( (ALGO_CALL::IsARange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::IsABoundedRange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (!ALGO_CALL::IsACountedRange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsANonFiniteRange < ABoundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::RepeatableIterator < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, ALGO_CALL::StartIteratorType < ABoundedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, ALGO_CALL::EndIteratorType < ABoundedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasCountO1Time < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsAReversedRange < ABoundedRange >::type::value, "unexpected" ) ;


ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasCountO1Time < ALGO_CALL::BasicBoundedRange < std::map < int, int >::iterator, std::map < int, int >::iterator >::type >::type::value), "unexpected" ) ;

typedef ALGO_CALL::BasicReversedRange < ABoundedRange >::type AReversedRange ;

ALGO_STATIC_ASSERT ( (ALGO_CALL::IsARange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::IsABoundedRange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (!ALGO_CALL::IsACountedRange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( !ALGO_CALL::IsANonFiniteRange < AReversedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::RepeatableIterator < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, ALGO_CALL::StartIteratorType < AReversedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, ALGO_CALL::EndIteratorType < AReversedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (ALGO_CALL::HasCountO1Time < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( ALGO_CALL::IsAReversedRange < AReversedRange >::type::value, "unexpected" ) ;

template < typename Range >
void testRangeNavigation ( Range& a, bool isEmptyAfterTwoAdvances )
{
    Range copy = a ;
    TEST_ASSERT ( 0 == ALGO_CALL::distance ( copy, a ) ) ;
    TEST_ASSERT ( 0 == ALGO_CALL::distance ( a, copy ) ) ;
    
    TEST_ASSERT ( 0 == ALGO_CALL::deref ( a ) ) ;
    ALGO_CALL::advance ( a, 1, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( a ) ) ;
    
    TEST_ASSERT ( 1 == ALGO_CALL::distance ( copy, a ) ) ;
    TEST_ASSERT ( -1 == ALGO_CALL::distance ( a, copy ) ) ;
    
    ALGO_CALL::advance ( a, 1, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( isEmptyAfterTwoAdvances == ALGO_CALL::isEmpty ( a ) ) ;
    ALGO_CALL::advance ( a, -2, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    
    TEST_ASSERT ( 0 == ALGO_CALL::deref ( a ) ) ;
    ALGO_CALL::successor ( a, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( a ) ) ;
    ALGO_CALL::successor ( a, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( isEmptyAfterTwoAdvances == ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( 2 == ALGO_CALL::distance ( copy, a ) ) ;
    TEST_ASSERT ( -2 == ALGO_CALL::distance ( a, copy ) ) ;
    ALGO_CALL::predecessor ( a, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( a ) ) ;
    ALGO_CALL::predecessor ( a, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( 0 == ALGO_CALL::deref ( a ) ) ;
    
    TEST_ASSERT ( 0 == ALGO_CALL::distance ( copy, a ) ) ;
    TEST_ASSERT ( 0 == ALGO_CALL::distance ( a, copy ) ) ;
}

void testBasicRanges ()
{
    typedef ALGO_CALL::BasicBoundedRange < int* >::type BoundedIntPtr ;
    typedef ALGO_CALL::BasicCountedRange < int* >::type CountedIntPtr ;
    typedef ALGO_CALL::BasicUnboundedRange < int* >::type UnboundedIntPtr ;
    // Bounded range and a count.
    typedef ALGO_CALL::Compound < BoundedIntPtr, ALGO_CALL::ValueAndProperty < ALGO_CALL::Count, ptrdiff_t > > BoundedCountedIntPtr ;
    
    int arr [] = { 0, 1 } ;
    
    BoundedIntPtr a = { arr, arr + 2 } ;
    CountedIntPtr b = { arr, 2 } ;
    BoundedCountedIntPtr c = { arr, arr + 2, 2 } ;
    UnboundedIntPtr d = { arr } ;
    
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( b ) ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( c ) ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
    
    testRangeNavigation ( a, true ) ;
    testRangeNavigation ( b, true ) ;
    testRangeNavigation ( c, true ) ;
    testRangeNavigation ( d, false ) ;
}

void testDeduceRange1 ()
{
    int arr [] = { 1 } ;
    ALGO_CALL::BasicUnboundedRange < int* >::type a = ALGO_CALL::deduceRange ( &arr [ 0 ] ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( a ) ) ;
    
    ALGO_CALL::BasicUnboundedRange < int* >::type b = ALGO_CALL::deduceRange ( a ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( b ) ) ;
    TEST_ASSERT ( b == a ) ;
    
    ALGO_CALL::BasicBoundedRange < int* >::type c = ALGO_CALL::deduceRange ( arr ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( c ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( c ) ) ;
}

void testDeduceRange2 ()
{
    int arr [] = { 1 } ;
    ALGO_CALL::BasicBoundedRange < int* >::type a = ALGO_CALL::deduceRange ( arr, arr + 1 ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( a ) ) ;
    
    ALGO_CALL::BasicCountedRange < int*, ptrdiff_t >::type b = ALGO_CALL::deduceRange ( arr, ptrdiff_t ( 1 ) ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( b ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( b ) ) ;
    
    ALGO_CALL::BasicCountedRange < int*, ptrdiff_t >::type c = ALGO_CALL::deduceRange ( arr, short ( 1 ) ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( c ) ) ;
    TEST_ASSERT ( 1 == ALGO_CALL::deref ( c ) ) ;
}

template < typename ReversedRange, typename ArrayStartIter >
void testIteration ( ReversedRange const x, ArrayStartIter y )
{
    ReversedRange d = x ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
    ALGO_CALL::successor ( d, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( ALGO_CALL::deref ( d ) == 2 ) ;
    
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
    ALGO_CALL::successor ( d, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( ALGO_CALL::deref ( d ) == 1 ) ;
    
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
    ALGO_CALL::successor ( d, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( ALGO_CALL::deref ( d ) == 0 ) ;
    
    TEST_ASSERT ( ALGO_CALL::isEmpty ( d ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( d ) == y ) ;
    
    ALGO_CALL::predecessor ( d, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
    TEST_ASSERT ( ALGO_CALL::deref ( d ) == 1 ) ;
    
    ALGO_CALL::predecessor ( d, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
    TEST_ASSERT ( ALGO_CALL::deref ( d ) == 2 ) ;
    
    
    ALGO_CALL::predecessor ( d, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
    
    TEST_ASSERT ( d == x ) ;
    
    ALGO_CALL::advance ( d, 3, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( d ) ) ;
    
    TEST_ASSERT ( -3 == ALGO_CALL::distance ( x, d ) ) ;
    TEST_ASSERT ( 3 == ALGO_CALL::distance ( d, x ) ) ;
    
    ALGO_CALL::advance ( d, -3, ALGO_CALL::InPlace () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( d ) ) ;
}

void testReverseRange ()
{
    typedef std::array < int, 3 > ArrayType ;
    ArrayType arr = { 0, 1, 2 } ;
    typedef ArrayType::iterator iter ;
    
    typedef ALGO_CALL::BasicBoundedRange < iter >::type iterRange ;
    iterRange arrayRange = ALGO_CALL::deduceRange ( arr.begin (), arr.end () ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( arrayRange ) == arr.begin () ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( arrayRange ) == arr.end () ) ;
    
    typedef typename ALGO_CALL::BasicReversedRange < iterRange >::type ReversedIterRange ;
    
    ReversedIterRange reservedBoundedRange = ALGO_CALL::reverseRange ( arrayRange ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( reservedBoundedRange ) == arr.end () ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( reservedBoundedRange ) == arr.begin () ) ;
    
    iterRange doubleReversedRange = ALGO_CALL::reverseRange ( reservedBoundedRange ) ;
    
    TEST_ASSERT ( arrayRange == doubleReversedRange ) ;
    
    // Now test reverse iteration
    testIteration ( reservedBoundedRange, arr.begin () ) ;
    
    
    // Now test reversed counted
    typedef ALGO_CALL::BasicCountedRange < iter >::type CountedIterRange ;
    
    CountedIterRange countedRange = ALGO_CALL::deduceRange ( arr.begin (), 3 ) ;
    typedef typename ALGO_CALL::BasicReversedRange < CountedIterRange >::type ReversedCountedIterRange ;
    ReversedCountedIterRange reverseCountedRange = ALGO_CALL::reverseRange ( countedRange ) ;
    testIteration ( reverseCountedRange, arr.begin () ) ;
    
    TEST_ASSERT ( 3 == ALGO_CALL::distance ( arrayRange, reservedBoundedRange ) ) ;
    TEST_ASSERT ( 3 == ALGO_CALL::distance ( arrayRange, reverseCountedRange ) ) ;
    
    // Works because underneath this are random access iterators.
    TEST_ASSERT ( -3 == ALGO_CALL::distance ( reservedBoundedRange, arrayRange ) ) ;
    TEST_ASSERT ( -3 == ALGO_CALL::distance ( reverseCountedRange, arrayRange ) ) ;
}

struct EqualToFour
{
    bool operator () ( int x ) const { return x == 4 ; }
} ;

struct LessThanFour
{
    bool operator () ( int x ) const { return x < 4 ; }
} ;

struct Sum
{
    int sum ;
    void operator () ( int x ) { sum += x ; }
};

struct AddOne
{
    int operator () ( int x ) const { return x + 1 ; }
};

void testFindIf ()
{
    int arr [] = { 0, 1, 2, 3, 4, 5 } ;
    auto i = ALGO_CALL::find_if ( ALGO_CALL::deduceRange ( &arr [ 0 ] ), EqualToFour () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( i ) ) ;
    TEST_ASSERT ( 4 == ALGO_CALL::deref ( i ) ) ;
    
    
    auto i2 = ALGO_CALL::find_if ( ALGO_CALL::deduceRange ( arr, arr + 3 ), EqualToFour () ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i2 ) ) ;
    TEST_ASSERT ( 3 == ALGO_CALL::deref ( i2 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    auto i3 = ALGO_CALL::find_if ( ALGO_CALL::deduceRange ( arr, 2 ), EqualToFour () ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i3 ) ) ;
    TEST_ASSERT ( 2 == ALGO_CALL::deref ( i3 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    // Empty
    auto i4 = ALGO_CALL::find_if ( ALGO_CALL::deduceRange ( arr, 0 ), EqualToFour () ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i4 ) ) ;
    TEST_ASSERT ( 0 == ALGO_CALL::deref ( i4 ) ) ; // Naughty ! But we know this iterator is actually valid.
}

void testFindIfNot ()
{
    int arr [] = { 0, 1, 2, 3, 4, 5 } ;
    auto i = ALGO_CALL::find_if_not ( ALGO_CALL::deduceRange ( &arr [ 0 ] ), LessThanFour () ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( i ) ) ;
    TEST_ASSERT ( 4 == ALGO_CALL::deref ( i ) ) ;
    
    
    auto i2 = ALGO_CALL::find_if_not ( ALGO_CALL::deduceRange ( arr, arr + 3 ), LessThanFour () ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i2 ) ) ;
    TEST_ASSERT ( 3 == ALGO_CALL::deref ( i2 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    auto i3 = ALGO_CALL::find_if_not ( ALGO_CALL::deduceRange ( arr, 2 ), LessThanFour () ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i3 ) ) ;
    TEST_ASSERT ( 2 == ALGO_CALL::deref ( i3 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    // Empty
    auto i4 = ALGO_CALL::find_if_not ( ALGO_CALL::deduceRange ( arr, 0 ), LessThanFour () ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i4 ) ) ;
    TEST_ASSERT ( 0 == ALGO_CALL::deref ( i4 ) ) ; // Naughty ! But we know this iterator is actually valid.
}

void testForEach ()
{
    int arr [] = { 0, 1, 2, 3, 4, 5 } ;
    
    Sum tmp = { 0 } ;
    Sum result = ALGO_CALL::for_each ( ALGO_CALL::deduceRange ( arr, 6 ), tmp ) ;
    
    TEST_ASSERT ( 15 == result.sum ) ;
    
    
    // Empty
    Sum result2 = ALGO_CALL::for_each ( ALGO_CALL::deduceRange ( arr, 0 ), tmp ) ;
    TEST_ASSERT ( 0 == result2.sum ) ;
}

void testTransform ()
{
    const ptrdiff_t ARR_LEN = 10 ;
    
    // Output and input range of same length
    int arr [ ARR_LEN ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
    int res [ ARR_LEN ] = {} ;
    
    auto i = ALGO_CALL::transform ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( res ), AddOne () ) ;
    
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i.first ) ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i.second ) ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( i.first ) == &res [ 0 ] + ARR_LEN ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( i.second ) == &arr [ 0 ] + ARR_LEN ) ;
    
    for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
    {
        TEST_ASSERT ( ( arr [ i ] + 1 ) == res [ i ] ) ;
    }
    
    // Output range longer than input range
    int res2 [ ARR_LEN + 1 ] = {} ;
    
    auto i2 = ALGO_CALL::transform ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( res2 ), AddOne () ) ;
    
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( i2.first ) ) ;
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second ) ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( i2.first ) == &res2 [ 0 ] + ARR_LEN ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( i2.second ) == &arr [ 0 ] + ARR_LEN ) ;
    
    for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
    {
        TEST_ASSERT ( ( arr [ i ] + 1 ) == res2 [ i ] ) ;
    }
    
    // Output range shorter than input range
    int res3 [ ARR_LEN - 1 ] = {} ;
    
    auto i3 = ALGO_CALL::transform ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( res3 ), AddOne () ) ;
    
    TEST_ASSERT ( ALGO_CALL::isEmpty ( i3.first ) ) ;
    TEST_ASSERT ( !ALGO_CALL::isEmpty ( i3.second ) ) ;
    
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( i3.first ) == &res3 [ 0 ] + ( ARR_LEN - 1 ) ) ;
    TEST_ASSERT ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( i3.second ) == &arr [ 0 ] + ( ARR_LEN - 1 ) ) ;
    
    for ( ptrdiff_t i = 0 ; i < ( ARR_LEN - 1 ) ; ++i )
    {
        TEST_ASSERT ( ( arr [ i ] + 1 ) == res3 [ i ] ) ;
    }
}

void testZip ()
{
    const ptrdiff_t ARR_LEN = 10 ;
    int arr [ ARR_LEN ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
    
    {
        int res [ ARR_LEN ] = {} ;
    
        auto i = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( res ), std::plus < int > () ) ;
    
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i.second.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i.second.second ) ) ;
    
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 2 * i == res [ i ] ) ;
        }
    }
    
    // Result array longer than inputs
    {
        int res [ ARR_LEN + 1 ] = {} ;
        
        auto i2 = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( res ), std::plus < int > () ) ;
        
        
        TEST_ASSERT ( !ALGO_CALL::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( 10 == ALGO_CALL::distance ( ALGO_CALL::deduceRange ( res ), i2.first ) ) ;
        
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.second ) ) ;
        
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 2 * i == res [ i ] ) ;
        }
        
        TEST_ASSERT ( 0 == res [ ARR_LEN ] ) ;
    }
    
    // Result array shorter than inputs
    {
        int res [ ARR_LEN - 1 ] = {} ;
        
        auto i2 = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( res ), std::plus < int > () ) ;
        
        
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.first ) ) ;
        
        TEST_ASSERT ( !ALGO_CALL::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( 9 == ALGO_CALL::deref ( i2.second.first ) ) ;
        
        TEST_ASSERT ( !ALGO_CALL::isEmpty ( i2.second.second ) ) ;
        TEST_ASSERT ( 9 == ALGO_CALL::deref ( i2.second.second ) ) ;
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN - 1 ; ++i )
        {
            TEST_ASSERT ( 2 * i == res [ i ] ) ;
        }
    }
    
    // input arrays differing
    {
        int input2 [ ARR_LEN ] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 } ;
        int res [ ARR_LEN ] = {} ;
        int res2 [ ARR_LEN ] = {} ;
        auto i2 = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( input2 ), ALGO_CALL::deduceRange ( res ), std::plus < int > () ) ;
        
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.second ) ) ;
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 3 * i == res [ i ] ) ;
        }
        
        auto i3 = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( input2 ), ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( res2 ), std::plus < int > () ) ;
        
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i3.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i3.second.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i3.second.second ) ) ;
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 3 * i == res2 [ i ] ) ;
        }
    }
    
    
    // input arrays differing in length (2nd shorter than the 1st)
    {
        int input2 [ ARR_LEN - 1 ] = { 0, 2, 4, 6, 8, 10, 12, 14, 16 } ;
        int res [ ARR_LEN ] = {} ;
        auto i2 = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( input2 ), ALGO_CALL::deduceRange ( res ), std::plus < int > () ) ;
        
        TEST_ASSERT ( !ALGO_CALL::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( 9 == ALGO_CALL::distance ( ALGO_CALL::deduceRange ( res ), i2.first ) ) ;
        
        TEST_ASSERT ( !ALGO_CALL::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( 9 == ALGO_CALL::distance ( ALGO_CALL::deduceRange ( arr ), i2.second.first ) ) ;
        
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.second ) ) ;
        
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN - 1 ; ++i )
        {
            TEST_ASSERT ( 3 * i == res [ i ] ) ;
        }
    }
    
    // input arrays differing in length (2nd longer than the 1st)
    {
        int input2 [ ARR_LEN + 1 ] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 } ;
        int res [ ARR_LEN ] = {} ;
        auto i2 = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( input2 ), ALGO_CALL::deduceRange ( res ), std::plus < int > () ) ;
        
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( !ALGO_CALL::isEmpty ( i2.second.second ) ) ;
        TEST_ASSERT ( 10 == ALGO_CALL::distance ( ALGO_CALL::deduceRange ( input2 ), i2.second.second ) ) ;
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 3 * i == res [ i ] ) ;
        }
    }
    
    {
        // Inputs and outputs are the same
        auto i2 = ALGO_CALL::zip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( arr ), std::plus < int > () ) ;
    
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i2.second.second ) ) ;
    
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 2 * i == arr [ i ] ) ;
        }
    }
}

struct SplitInts
{
    std::pair < int, int > operator () ( int x ) const
    {
        return std::make_pair ( x / 2, x % 2 ) ;
    }
} ;

void testUnzip ()
{
    const ptrdiff_t ARR_LEN = 10 ;
    int arr [ ARR_LEN ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
    {
        int out1 [ ARR_LEN ] = {};
        int out2 [ ARR_LEN ] = {};
        auto i = ALGO_CALL::unzip ( ALGO_CALL::deduceRange ( arr ), ALGO_CALL::deduceRange ( out1 ), ALGO_CALL::deduceRange ( out2 ), SplitInts () ) ;
        
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i.first.first ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i.first.second ) ) ;
        TEST_ASSERT ( ALGO_CALL::isEmpty ( i.second ) ) ;
        
        for ( int i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( i / 2 == out1 [ i ] ) ;
            TEST_ASSERT ( i % 2 == out2 [ i ] ) ;
        }
    }
}

template < typename T >
struct ArrayDeleter
{
    ArrayDeleter ( T* x ) : x ( x ) {}
    ~ArrayDeleter () { delete [] x ; }
private:
    ArrayDeleter ( ArrayDeleter const& ) ;
    ArrayDeleter& operator= ( ArrayDeleter const& ) ;
    
    T* x ;
} ;

struct InlineLoopCalls {} ;
struct NotInlineLoopCalls {} ;
    
const char* print ( InlineLoopCalls ) { return "InlineLoopCalls" ; }
const char* print ( NotInlineLoopCalls ) { return "NotInlineLoopCalls" ; }
    
// Isolating the loop into a function gives the optimiser the best chance of recognising a loop!
template < ptrdiff_t ARRAY_SIZE, typename Range >
ALGO_INLINE
typename ALGO_CALL::IteratorTraits < Range >::value_type loopOver ( Range x, InlineLoopCalls )
{
    typename ALGO_CALL::IteratorTraits < Range >::value_type returnValue = 0 ;
        
    for ( ptrdiff_t i = 0 ; i < ARRAY_SIZE ; ++i )
    {
        returnValue += ALGO_CALL::Deref < Range >::apply ( x ) ;
        ALGO_CALL::Successor < Range >::apply ( x ) ;
    }
        
    return returnValue ;
}
    
// Isolating the loop into a function gives the optimiser the best chance of recognising a loop!
template < ptrdiff_t ARRAY_SIZE, typename Range >
ALGO_INLINE
typename ALGO_CALL::IteratorTraits < Range >::value_type loopOver ( Range x, NotInlineLoopCalls )
{
    typename ALGO_CALL::IteratorTraits < Range >::value_type returnValue = 0 ;
        
    for ( ptrdiff_t i = 0 ; i < ARRAY_SIZE ; ++i )
    {
        returnValue += ALGO_CALL::deref ( x ) ;
        ALGO_CALL::successor ( x, ALGO_CALL::InPlace () ) ;
    }
        
    return returnValue ;
}
    
template < typename InlinedDerefSuccessorType >
void testRangePerformance ()
{
    typedef unsigned char ContainedType ;
    const ptrdiff_t ARRAY_SIZE = 1024 * 1024 * 1024 ;
    ContainedType* arr = new ContainedType [ ARRAY_SIZE ] ;
        
    ArrayDeleter < ContainedType > deleter ( arr ) ;
        
    std::iota ( arr, arr + ARRAY_SIZE, 0u ) ;
        
    typedef ALGO_CALL::BasicBoundedRange < ContainedType* >::type Bounded ;
    Bounded bounded = ALGO_CALL::deduceRange ( arr, arr + ARRAY_SIZE ) ;
        
    typedef ALGO_CALL::BasicCountedRange < ContainedType* >::type Counted ;
    Counted counted = ALGO_CALL::deduceRange ( arr, ARRAY_SIZE ) ;
        
    typedef ALGO_CALL::BasicUnboundedRange < ContainedType* >::type Unbounded ;
    Unbounded unbounded = ALGO_CALL::deduceRange ( arr ) ;
        
    timer t ;
        
    t.start() ;
    ContainedType const acc1 = loopOver < ARRAY_SIZE > ( bounded, InlinedDerefSuccessorType () ) ;
    double time1 = t.stop () ;
        
    t.start() ;
    ContainedType const acc2 = loopOver < ARRAY_SIZE > ( counted, InlinedDerefSuccessorType () ) ;
    double time2 = t.stop () ;
        
    t.start() ;
    ContainedType const acc3 = loopOver < ARRAY_SIZE > ( unbounded, InlinedDerefSuccessorType () ) ;
    double time3 = t.stop () ;
        
    t.start() ;
    ContainedType const acc4 = loopOver < ARRAY_SIZE > ( arr, InlinedDerefSuccessorType () ) ;
    double time4 = t.stop () ;
        
    t.start() ;
    ContainedType const acc5 = std::accumulate ( arr, arr + ARRAY_SIZE, ContainedType ( 0 ) ) ;
    double time5 = t.stop () ;
        
    TEST_ASSERT ( acc1 == acc2 ) ;
    TEST_ASSERT ( acc1 == acc3 ) ;
    TEST_ASSERT ( acc1 == acc4 ) ;
    TEST_ASSERT ( acc1 == acc5 ) ;

    std::cout << "testRangePerformance " << print ( InlinedDerefSuccessorType () )
        << " BasicBoundedRange " << time1
        << " BasicCountedRange " << time2
        << " BasicUnboundedRange " << time3
        << " raw pointer " << time4
        << " std::accumulate " << time5 << '\n' ;
}
    
} // namespace algo_range_h

namespace algo_sort_h {
    
template <typename T>
struct myLess
{
    __attribute__((const))
    bool operator()(const T x, const T y) const ALGO_NOEXCEPT_DECL(noexcept(x < y))
    {return x < y;}
};

struct StdSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        std::sort(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >());
    }
};

struct StableStdSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        std::stable_sort(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >());
    }
};

template < typename Tag, typename SwapIfKind, typename IndexType >
struct NetworkSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        ALGO_CALL::sort(f, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >(), Tag (), ALGO_CALL::Int<N>(), SwapIfKind (), IndexType () );
    }
};

struct BoundedInsertionSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        ALGO_CALL::sort_insertion(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct CountedInsertionSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        ALGO_CALL::sort_insertion(f, N, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct BoundedInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        ALGO_CALL::sort_insertion_sentinel(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct CountededInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        ALGO_CALL::sort_insertion_sentinel(f, N, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct BoundedUnstableInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        ALGO_CALL::sort_insertion_sentinel_unstable(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct CountededUnstableInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        ALGO_CALL::sort_insertion_sentinel_unstable(f, N, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

template < int N, typename Contained >
std::vector < std::array < Contained, N > > const& getTestSet ()
{
    typedef std::array<Contained, N> Container ;
    
    static bool initialised = false ;
    static std::vector < Container > testSet ;
    if ( !initialised )
    {
        initialised = true ;
        const int NUMBER_OF_RUNS = 1000000 ;
        testSet.reserve ( NUMBER_OF_RUNS ) ;
        
        Container arr;
        std::iota ( arr.begin (), arr.end (), Contained ( 32 ) ) ;
        
        Container toSort (arr);
        
        for (int i = 0 ; i < NUMBER_OF_RUNS ; ++i )
        {
            std::random_shuffle ( toSort.begin (), toSort.end () ) ;
            testSet.push_back ( toSort ) ;
        }

    }
    return testSet ;
}

template <int N, class Sorter>
void test_sort()
{
    typedef int Contained;
    typedef std::array<Contained, N> Container ;
    
    Container arr;
    std::iota(arr.begin(), arr.end(), 32);
    
    int count = 0;
    double totalTime = 0.0;
    timer t;
    
    for ( auto const& toSort : getTestSet < N, Contained > () )
    {
        Container sorted = toSort ;
        
        ++count;
        t.start();
        
        Sorter ().template sort < N > ( sorted.begin (), sorted.end () ) ;
        
        totalTime += t.stop();
        
        if ( sorted != arr )
        {
            std::cout << " test_sort Error " ;
            for ( auto j : sorted )
            {
                std::cout << j <<',' ;
            }
            
            std::cout << " state " ;
            
            for ( auto j : toSort )
            {
                std::cout << j <<',' ;
            }
            std::cout <<'\n';
        }
    }
    
    std::cout << ", " << totalTime ;
}


template <int N, class Sorter>
void test_zero_one ()
{
    typedef unsigned long CounterType ;
    const CounterType upperLimit = ( 1u << N ) ; // not inclusive
    // sort the binary representation of i, which by zero-one principle tests whether a sort is valid
    // Unable to test the stability of sort here ... but trust that comparing adjacent elements will not result in unstable behaviour.
    
    std::array < bool, N > toSort ;
    
    std::bitset<N> bs ;
    
    for ( CounterType i = 0u ; i < upperLimit ; ++i )
    {
        CounterType zeroCount = 0u ;
        CounterType zeroSum = 0u ;
        CounterType oneSum = 0u ;
        
        bs = i ;
        
        for ( CounterType j = 0u ; j < N ; ++j )
        {
            toSort [ j ] = bs.test(j) ;
            if ( !toSort [ j ] )
            {
                ++zeroCount ;
            }
        }
        
        Sorter ().template sort < N > ( toSort.begin (), toSort.end () ) ;
        
        // check all the zeroes are to the left of toSort, and the ones to the right
        for ( CounterType j = 0u ; j < zeroCount ; ++j )
        {
            zeroSum += CounterType ( toSort [ j ] ) ;
        }
        
        for ( CounterType j =  zeroCount ; j < N ; ++j )
        {
            oneSum += CounterType ( toSort [ j ] ) ;
        }
        
        if ( ( 0u != zeroSum ) || ( ( N - zeroCount ) != oneSum ) )
        {
            std::cout << "test_zero_one Incorrect sorting on " << bs << '\n' ;
        }
    }
}

template < class Sorter >
void testSorting ()
{
#ifdef ALGO_FULL_SORT_TESTS
    test_zero_one<1, Sorter>();
    test_zero_one<2, Sorter>();
    test_zero_one<3, Sorter>();
    test_zero_one<4, Sorter>();
    test_zero_one<5, Sorter>();
    test_zero_one<6, Sorter>();
    test_zero_one<7, Sorter>();
    //test_zero_one<8, Sorter>();
    test_zero_one<9, Sorter>();
    test_zero_one<10, Sorter>();
    test_zero_one<11, Sorter>();
    test_zero_one<12, Sorter>();
    test_zero_one<13, Sorter>();
    test_zero_one<14, Sorter>();
    test_zero_one<15, Sorter>();
#endif
    
    test_zero_one<8, Sorter>();
    test_zero_one<16, Sorter>();
    
#ifdef ALGO_TEST_PERFORMANCE
    
#ifdef ALGO_FULL_SORT_TESTS
    test_sort<1, Sorter>();
    test_sort<2, Sorter>();
    test_sort<3, Sorter>();
    test_sort<4, Sorter>();
    test_sort<5, Sorter>();
    test_sort<6, Sorter>();
    test_sort<7, Sorter>();
    //test_sort<8, Sorter>();
    test_sort<9, Sorter>();
    test_sort<10, Sorter>();
    test_sort<11, Sorter>();
    test_sort<12, Sorter>();
    test_sort<13, Sorter>();
    test_sort<14, Sorter>();
    test_sort<15, Sorter>();
#endif
    test_sort<8, Sorter>();
    test_sort<16, Sorter>();
    
#endif
}

template < typename SortingTag, typename SwapIfTag, typename IndexType >
void testSortingCombinations ( const char* indexTypeName )
{
    std::cout << typeid(SortingTag).name () << ", "
    << typeid(SwapIfTag).name () << ", "
    << indexTypeName << " " ;
    testSorting < NetworkSorter < SortingTag, SwapIfTag, IndexType > > () ;
    std::cout << '\n' ;
}

template < typename SortingTag, typename SwapIfTag >
void testSortingCombinationsSwapIfTagInlineNotInline ()
{
    testSortingCombinations < SortingTag, SwapIfTag, unsigned char > ( "unsigned char" ) ;
    testSortingCombinations < SortingTag, SwapIfTag, signed char > ("signed char") ;
    testSortingCombinations < SortingTag, SwapIfTag, unsigned short > ("unsigned short") ;
    testSortingCombinations < SortingTag, SwapIfTag, signed short > ("signed short") ;
    testSortingCombinations < SortingTag, SwapIfTag, unsigned int > ("unsigned int") ;
    testSortingCombinations < SortingTag, SwapIfTag, signed int > ("signed int") ;
    testSortingCombinations < SortingTag, SwapIfTag, unsigned long > ("unsigned long") ;
    testSortingCombinations < SortingTag, SwapIfTag, signed long > ("signed long") ;
    testSortingCombinations < SortingTag, SwapIfTag, unsigned long long > ("unsigned long long") ;
    testSortingCombinations < SortingTag, SwapIfTag, signed long long > ("signed long long") ;
}

template < typename SortingTag, typename SwapIfTag >
void testSortingCombinationsSwapIfTag ()
{
    testSortingCombinationsSwapIfTagInlineNotInline < SortingTag, SwapIfTag > () ;
    testSortingCombinationsSwapIfTagInlineNotInline < SortingTag, ALGO_CALL::NotInline < SwapIfTag > > () ;
}

template < typename SortingTag >
void testSortingCombinationsSortingTag ()
{
    testSortingCombinationsSwapIfTag <SortingTag, ALGO_CALL::Unpredictable > () ;
    testSortingCombinationsSwapIfTag <SortingTag, ALGO_CALL::Ternary > () ;
    testSortingCombinationsSwapIfTag <SortingTag, ALGO_CALL::Consistent > () ;
    testSortingCombinationsSwapIfTag <SortingTag, ALGO_CALL::PredictableFalse > () ;
    testSortingCombinationsSwapIfTag <SortingTag, ALGO_CALL::PredictableTrue > () ;
}

} // namespace algo_sort_h

int main(int argc, const char * argv[] )
{
    algo_basic_h::test_enable_disable () ;
    algo_basic_h::test_swap ( false ) ;
    algo_basic_h::test_swap ( true ) ;
    
    algo_basic_h::test_select_if ( false ) ;
    algo_basic_h::test_select_if ( true ) ;
    
    algo_property_h::testProperty () ;
    algo_property_h::testValueAndPropertyRelationalOperations () ;
    algo_property_h::testCompoundRelationalOperations () ;
#ifdef ALGO_TEST_PERFORMANCE
    algo_property_h::propertiesPerformanceTest () ;
#endif
    algo_property_h::testVisitProperty () ;
    algo_property_h::testConvertPropertySet () ;
    
    algo_iterator_h::testPredecessor () ;
    algo_iterator_h::testSuccessor () ;
    algo_iterator_h::testDistance () ;
    algo_iterator_h::testAdvance () ;
    algo_iterator_h::testIsEmpty () ;
    algo_iterator_h::testDeref () ;
    algo_iterator_h::testDerefMove () ;
    algo_iterator_h::testAddressOf () ;
    algo_iterator_h::testUnderlyingAddressOf () ;
    algo_iterator_h::testEqualUnderlyingAddress () ;
    algo_iterator_h::testDestroyPointed () ;
    algo_iterator_h::testDestroyPointed2 () ;
    algo_iterator_h::testDestroyReferenced () ;
    algo_iterator_h::testDestroyReferenced2 () ;
    algo_iterator_h::testAssign () ;
    algo_iterator_h::testMoveAssign () ;
    algo_iterator_h::testCopyConstruct () ;
    algo_iterator_h::testMoveConstruct () ;
    algo_iterator_h::testSwapped () ;
    algo_iterator_h::testCopyBytesNotOverlapped () ;
    algo_iterator_h::testCopyBytesOverlapped () ;
    algo_iterator_h::testStripIter () ;
    algo_iterator_h::testUnstripIter () ;
    
    algo_range_h::testBasicRanges () ;
    algo_range_h::testDeduceRange1 () ;
    algo_range_h::testDeduceRange2 () ;
    algo_range_h::testReverseRange () ;
    algo_range_h::testFindIf () ;
    algo_range_h::testFindIfNot () ;
    algo_range_h::testForEach () ;
    algo_range_h::testTransform () ;
    algo_range_h::testZip () ;
    algo_range_h::testUnzip () ;
    
#ifdef ALGO_TEST_PERFORMANCE
    algo_range_h::testRangePerformance < algo_range_h::InlineLoopCalls > () ;
    algo_range_h::testRangePerformance < algo_range_h::NotInlineLoopCalls > () ;
#endif
    
    algo_h::teststep () ;
    
    algo_h::testCopy < int > () ;
    algo_h::testCopyBackwards < int > () ;
    algo_h::testFill < int, 1024 > () ;
    algo_h::testFill < int, 1000 > () ;
    
    // These cases test for when the type is not bitwise copyable, so the step-by-step algorithm is chosen
    algo_h::testCopy < algo_h::NotABitwiseCopyableType > () ;
    algo_h::testCopyBackwards < algo_h::NotABitwiseCopyableType > () ;
    algo_h::testFill < algo_h::NotABitwiseCopyableType, 1024 > () ;
    algo_h::testFill < algo_h::NotABitwiseCopyableType, 1000 > () ;
    
    // These cases test for when the type is bitwise copyable through specialisation of IsBitwiseCopyable
    algo_h::testCopy < algo_h::IsABitwiseCopyableType > () ;
    algo_h::testCopyBackwards < algo_h::IsABitwiseCopyableType > () ;
    algo_h::testFill < algo_h::IsABitwiseCopyableType, 1024 > () ;
    algo_h::testFill < algo_h::IsABitwiseCopyableType, 1000 > () ;
    
    algo_h::testRotateRightByOne < ALGO_CALL::RotateNoChoice > () ;
    algo_h::testRotateRightByOne < ALGO_CALL::RotateBlocks > () ;
    algo_h::testRotateLeftByOne < ALGO_CALL::RotateNoChoice > () ;
    algo_h::testRotateLeftByOne < ALGO_CALL::RotateBlocks > () ;
    
    algo_h::testMinIter < algo_h::MinIterBounded > () ;
    algo_h::testMinIter < algo_h::MinIterCounted > () ;
    algo_h::testMaxIter < algo_h::MaxIterBounded > () ;
    algo_h::testMaxIter < algo_h::MaxIterCounted > () ;
    
#ifdef ALGO_TEST_PERFORMANCE
    algo_h::testStepPerformance < algo_h::StdCopy > ( "std::copy" ) ;
    algo_h::testStepPerformance < algo_h::StdCopyReversed > ( "reversed std::copy" ) ;
    
    algo_h::testStepPerformance < algo_h::StepOverDeduced > ( "step over, both bounded ranges" ) ;
    algo_h::testStepPerformance < algo_h::StepOverDeducedUnbounded > ( "step over, output range unbounded" ) ;
    algo_h::testStepPerformance < algo_h::StepOverDeducedInputCountedOutputUnbounded > ( "step over, input range counted, output range unbounded" ) ;
     algo_h::testStepPerformance < algo_h::StepOverDeducedUnboundedReversed > ( "step over, output range unbounded, both reversed" ) ;
     algo_h::testStepPerformance < algo_h::StepOverCounted > ( "step over counted, output range unbounded" ) ;
     algo_h::testStepPerformance < algo_h::StepOverCountedUnbounded > ( "step over counted, both ranges unbounded" ) ;
     algo_h::testStepPerformance < algo_h::StepOverCountedIters > ( "step over counted, both ranges are iterators" ) ;
    

    algo_h::testCopyTimed < int > () ;
    algo_h::testCopyBackwardsTimed < int > () ;
    
    algo_h::testCopyTimed < algo_h::NotABitwiseCopyableType > () ;
    algo_h::testCopyBackwardsTimed < algo_h::NotABitwiseCopyableType > () ;
    
    algo_h::testCopyTimed < algo_h::IsABitwiseCopyableType > () ;
    algo_h::testCopyBackwardsTimed < algo_h::IsABitwiseCopyableType > () ;
#endif
    
    algo_buffer_h::testBufferCalculation () ;
    algo_buffer_h::testBufferCalculationUnaligned () ;
    
    algo_buffer_h::testStackBuffer () ;
    
    algo_buffer_h::testBufferAllocationProtocol < ALGO_CALL::MallocFreeProtocol > ( false ) ;
    algo_buffer_h::testBufferAllocationProtocol < ALGO_CALL::CallocFreeProtocol > ( true ) ;
    algo_buffer_h::testBufferAllocationProtocol < ALGO_CALL::NewDeleteProtocol > ( false ) ;
    algo_buffer_h::testBufferAllocationProtocol < ALGO_CALL::ZeroedNewDeleteProtocol > ( true ) ;
    algo_buffer_h::testBufferAllocationProtocol < ALGO_CALL::StlTemporaryBufferProtocol > ( false ) ;
    algo_buffer_h::testBufferAllocationProtocol < ALGO_CALL::ZeroedStlTemporaryBufferProtocol > ( true ) ;
    
    algo_buffer_h::testAllocatingBuffer () ;
    
    algo_buffer_h::testObjectProctor () ;
    algo_buffer_h::testTrivialObjectProctor () ;
    
    algo_buffer_h::testIterProctorLengthOne () ;
    algo_buffer_h::testIterProctorLengthTwo () ;
    algo_buffer_h::testTrivialIterProctor () ;
    
    algo_buffer_h::testBufferProctorLengthOne () ;
    algo_buffer_h::testTrivialBufferProctor () ;

    algo_sort_h::testSortingCombinationsSortingTag < ALGO_CALL::UnstableExchange > () ;
    
    algo_sort_h::testSortingCombinationsSortingTag < ALGO_CALL::StableExchange > () ;
    
    algo_sort_h::testSortingCombinationsSortingTag < ALGO_CALL::UnstableExchangeIndices > () ;
    
    algo_sort_h::testSortingCombinationsSortingTag < ALGO_CALL::StableExchangeIndices > () ;
    
    std::cout << "BoundedInsertionSorter " ;
    
    algo_sort_h::testSorting < algo_sort_h::BoundedInsertionSorter > () ;
    
    std::cout << "CountedInsertionSorter " ;
    
    algo_sort_h::testSorting < algo_sort_h::CountedInsertionSorter > () ;
    
    std::cout << "BoundedInsertionUnguardedSorter " ;
    
    algo_sort_h::testSorting < algo_sort_h::BoundedInsertionUnguardedSorter > () ;
    
    std::cout << "CountededInsertionUnguardedSorter " ;
    
    algo_sort_h::testSorting < algo_sort_h::CountededInsertionUnguardedSorter > () ;
    
    std::cout << "BoundedUnstableInsertionUnguardedSorter " ;
    
    algo_sort_h::testSorting < algo_sort_h::BoundedUnstableInsertionUnguardedSorter > () ;
    
    std::cout << "CountededUnstableInsertionUnguardedSorter " ;
    
    algo_sort_h::testSorting < algo_sort_h::CountededUnstableInsertionUnguardedSorter > () ;
    
#ifdef ALGO_TEST_PERFORMANCE
    std::cout << "std::sort " ;
    
    algo_sort_h::testSorting < algo_sort_h::StdSorter > () ;
    
    std::cout << "std::stable_sort " ;
    
    algo_sort_h::testSorting < algo_sort_h::StableStdSorter > () ;
#endif

    return 0;
}

