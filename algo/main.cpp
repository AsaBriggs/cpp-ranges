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
#include "timer.h"

#include <array>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <deque>
#include <vector>
#include <map>

struct test_exception : std::exception
{};

#ifdef NDEBUG
#define TEST_ASSERT(x) do { if ( (x) ) throw test_exception () ; } while ( 0 )
#else
#define TEST_ASSERT(x) assert(x)
#endif

struct trivial {};

ALGO_STATIC_ASSERT ( !algo::IsAPointer < int >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( algo::IsAPointer < int* >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT ( (algo::CheckIteratorCategory < int*, std::input_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::CheckIteratorCategory < int*, std::forward_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::CheckIteratorCategory < int*, std::bidirectional_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::CheckIteratorCategory < int*, std::random_access_iterator_tag >::type::value), "unexpected" ) ;

typedef std::map < int, int >::iterator MapIterator ;

ALGO_STATIC_ASSERT ( (algo::CheckIteratorCategory < MapIterator, std::input_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::CheckIteratorCategory < MapIterator, std::forward_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::CheckIteratorCategory < MapIterator, std::bidirectional_iterator_tag >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (!algo::CheckIteratorCategory < MapIterator, std::random_access_iterator_tag >::type::value), "unexpected" ) ;


ALGO_STATIC_ASSERT( !algo::traits_test::DifferenceTypeTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::ValueTypeTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::PointerTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::ReferenceTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::IteratorCategoryTest < std::iterator_traits < char > >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT( !algo::traits_test::DifferenceTypeTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::ValueTypeTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::PointerTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::ReferenceTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::traits_test::IteratorCategoryTest < std::iterator_traits < trivial > >::type::value, "unexpected" ) ;

ALGO_STATIC_ASSERT( algo::traits_test::DifferenceTypeTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::ValueTypeTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::PointerTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::ReferenceTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::IteratorCategoryTest < std::iterator_traits < char* > >::type::value, "value not found!" ) ;

// Note need to go via algo::IteratorTraits for the T* const traits to be obtained
ALGO_STATIC_ASSERT( algo::traits_test::DifferenceTypeTest < algo::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::ValueTypeTest < algo::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::PointerTest < algo::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::ReferenceTest < algo::IteratorTraits < char* const > >::type::value, "value not found!" ) ;
ALGO_STATIC_ASSERT( algo::traits_test::IteratorCategoryTest < algo::IteratorTraits < char* const > >::type::value, "value not found!" ) ;

ALGO_STATIC_ASSERT( !algo::HasIteratorTraits < trivial >::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( !algo::HasIteratorTraits < char >::value, "unexpected" ) ;
ALGO_STATIC_ASSERT( algo::HasIteratorTraits < char const* >::value, "unexpected" ) ;

ALGO_STATIC_ASSERT( algo::HasIteratorTraits < char const* const >::value, "unexpected" ) ;

ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < int >::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < float >::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < bool >::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < bool* >::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < const bool* >::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < trivial >::value, "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( (algo::IsBitwiseCopyable < std::pair < int, int > >::value), "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( (algo::IsBitwiseCopyable < std::array < int, 5 > >::value), "unexpectedly not trivially copyable" ) ;
ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < int [ 5 ] >::value, "unexpectedly not trivially copyable" ) ;


ALGO_STATIC_ASSERT ( algo::IsNotProxiedIterator <int*>::value, "unexpected" );
ALGO_STATIC_ASSERT ( !algo::IsNotProxiedIterator <std::vector<bool>::iterator>::value, "unexpected" );

template < int N >
struct Tag {};

template < int N >
struct Value {} ;

// Template Params tests
// Exercise through the FindWrapper, to kill two birds in one stone.
typedef algo::Parameters <
    algo::Param < Tag < 0 >, Value < 0 > >
    , algo::Param < Tag < 1 >, Value < 1 > >
    , algo::Param < Tag < 2 >, Value < 2 > >
    , algo::Param < Tag < 3 >, Value < 3 > >
    , algo::Param < Tag < 4 >, Value < 4 > >
    , algo::Param < Tag < 5 >, Value < 5 > >
    , algo::Param < Tag < 6 >, Value < 6 > >
    , algo::Param < Tag < 7 >, Value < 7 > >
    , algo::Param < Tag < 8 >, Value < 8 > >
    , algo::Param < Tag < 9 >, Value < 9 > >
> TestParams ;

// Tag not in TestParams
ALGO_STATIC_ASSERT ( (std::is_same < algo::FindWrapper< algo::Param < Tag < 10 >, Value < 10 > >, TestParams >::type, Value < 10 > >::value ), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < algo::FindWrapper< algo::Param < Tag < 10 >, Value < 10 > >, algo::Parameters <> >::type, Value < 10 > >::value) , "unexpected" ) ;

typedef algo::Parameters <
    algo::Param < Tag < 0 >, Value < 10 > >
    , algo::Param < Tag < 1 >, Value < 10 > >
    , algo::Param < Tag < 2 >, Value < 10 > >
    , algo::Param < Tag < 3 >, Value < 10 > >
    , algo::Param < Tag < 4 >, Value < 10 > >
    , algo::Param < Tag < 5 >, Value < 10 > >
    , algo::Param < Tag < 6 >, Value < 10 > >
    , algo::Param < Tag < 7 >, Value < 10 > >
    , algo::Param < Tag < 8 >, Value < 10 > >
    , algo::Param < Tag < 9 >, Value < 10 > >
> DefaultParams ;

typedef algo::DeduceTypes < TestParams, DefaultParams >::type TestDeducedParams ;

ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param0, Value < 0 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param1, Value < 1 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param2, Value < 2 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param3, Value < 3 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param4, Value < 4 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param5, Value < 5 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param6, Value < 6 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param7, Value < 7 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param8, Value < 8 > >::value) , "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < TestDeducedParams::Param9, Value < 9 > >::value) , "unexpected" ) ;


void testPredecessor ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    TEST_ASSERT ( algo::predecessor(next, ALGO_CALL::ByReturnValue () ) == begin ) ;
    
    int* nextInPlace = begin + 1 ;
    algo::predecessor(nextInPlace, algo::InPlace () ) ;
    TEST_ASSERT ( begin == nextInPlace ) ;
}

void testSuccessor ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    TEST_ASSERT ( algo::successor(begin, ALGO_CALL::ByReturnValue ()) == next ) ;
    
    int* beginInPlace = begin ;
    algo::successor(beginInPlace, algo::InPlace () ) ;
    TEST_ASSERT ( next == beginInPlace ) ;
}

void testDistance ()
{
    static const int ARR_LENGTH = 2 ;
    int arr [ARR_LENGTH] = { 1, 2 } ;
    
    int* const begin = arr ;
    int* const end = begin + 2 ;
    
    TEST_ASSERT ( 2 == algo::distance ( begin, end ) ) ;
    TEST_ASSERT ( -2 == algo::distance ( end, begin ) ) ;
}

void testAdvance ()
{
    static const int ARR_LENGTH = 2 ;
    int arr [ARR_LENGTH] = { 1, 2 } ;
    
    int* const begin = arr ;
    int* const end = begin + 2 ;
    
    TEST_ASSERT ( end == algo::advance ( begin, 2, ALGO_CALL::ByReturnValue () ) ) ;
    TEST_ASSERT ( begin == algo::advance ( end, -2, ALGO_CALL::ByReturnValue () ) ) ;
    
    int* tmp = begin ;
    algo::advance(tmp, 2, algo::InPlace () ) ;
    TEST_ASSERT ( end == tmp ) ;
    
    int* tmp2 = end ;
    algo::advance(tmp2, -2, algo::InPlace () ) ;
    TEST_ASSERT ( begin == tmp2 ) ;
}

void testIsEmpty ()
{
    int arr [ 1 ] = { 1 } ;
    TEST_ASSERT ( !algo::isEmpty ( arr ) ) ;
}

void testDeref ()
{
    static const int ARR_LENGTH = 2 ;
    int arr [ARR_LENGTH] = { 1, 2 } ;
    
    TEST_ASSERT ( algo::deref ( arr ) == arr [ 0 ] ) ;
}

void testAddressOf ()
{
    int a = 0 ;
    TEST_ASSERT ( &a == algo::addressOf ( a ) ) ;
}

void testUnderlyingAddressOf ()
{
    int arr [ 1 ] ;
    TEST_ASSERT ( arr == algo::underlyingAddressOf ( arr ) ) ;
    
    std::pair < const int, int > toInsert ( 1, 2 ) ;
    
    std::map < int, int > aMap ;
    aMap.insert ( toInsert ) ;
    
    auto iter = aMap.find ( toInsert.first ) ;
    TEST_ASSERT ( toInsert == *( algo::underlyingAddressOf ( iter ) ) ) ;
}

void testEqualUnderlyingAddress ()
{
    char arr [ sizeof ( unsigned long ) * 2 ] = {} ;
    
    char* begin = arr ;
    char* another = begin + sizeof ( unsigned long ) ;
    
    unsigned long* uBegin = reinterpret_cast < unsigned long* > ( begin ) ;
    unsigned long* uAnother = uBegin + 1 ;
    
    TEST_ASSERT ( algo::equalUnderlyingAddress ( begin, begin ) ) ;
    TEST_ASSERT ( !algo::equalUnderlyingAddress ( begin, another ) ) ;
    
    TEST_ASSERT ( algo::equalUnderlyingAddress ( uBegin, uBegin ) ) ;
    TEST_ASSERT ( !algo::equalUnderlyingAddress ( uBegin, uAnother ) ) ;
    
    TEST_ASSERT ( algo::equalUnderlyingAddress ( begin, uBegin ) ) ;
    TEST_ASSERT ( algo::equalUnderlyingAddress ( another, uAnother ) ) ;
}

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

void testDestroyPointed ()
{
    typedef RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    algo::destroyPointed ( toDestroy ) ;
    TEST_ASSERT ( destroyed ) ;
}

typedef RecordDestruction < long > DestructionSkipped ;

namespace algo
{
    template <>
    struct IsTriviallyDestructible < DestructionSkipped, ALGO_ENABLE_IF_PARAM_DEFAULT > : std::true_type
    {} ;
}

void testDestroyPointed2 ()
{
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionSkipped ( destroyed ) ;
    
    DestructionSkipped* const toDestroy = reinterpret_cast<DestructionSkipped*>(&arr);
    algo::destroyPointed ( toDestroy ) ;
    TEST_ASSERT ( !destroyed ) ;
    
    toDestroy->~DestructionSkipped() ;
}

void testDestroyReferenced ()
{
    typedef RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    algo::destroyReferenced ( *toDestroy ) ;
    TEST_ASSERT ( destroyed ) ;
}

void testDestroyReferenced2 ()
{
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionSkipped ( destroyed ) ;
    
    DestructionSkipped* const toDestroy = reinterpret_cast<DestructionSkipped*>(&arr);
    algo::destroyReferenced ( *toDestroy ) ;
    TEST_ASSERT ( !destroyed ) ;
    
    toDestroy->~DestructionSkipped() ;
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
    algo::assign ( &input, &output ) ;
    TEST_ASSERT ( 1 == operatorCounts::copyAssignmentCalled ) ;
}

void testMoveAssign ()
{
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    operatorCounts::TestType output ( true ) ;
    algo::moveAssign ( &input, &output ) ;
    TEST_ASSERT ( 1 == operatorCounts::moveAssignmentCalled ) ;
}

void testCopyConstruct ()
{
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    char buffer [ 16 ] ;
    operatorCounts::TestType* outputAddress = reinterpret_cast < operatorCounts::TestType* >( buffer ) ;
    
    algo::copyConstruct ( &input, outputAddress ) ;
    TEST_ASSERT ( 1 == operatorCounts::copyConstructorCalled ) ;
    outputAddress->~TestType() ;
}

void testMoveConstruct ()
{
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    char buffer [ 16 ] ;
    operatorCounts::TestType* outputAddress = reinterpret_cast < operatorCounts::TestType* >( buffer ) ;
    
    algo::moveConstruct ( &input, outputAddress ) ;
    TEST_ASSERT ( 1 == operatorCounts::moveConstructorCalled ) ;
    outputAddress->~TestType() ;
}

void testDerefMove ()
{
    // Located here to take advantage of the operatorCounts mechanism
    operatorCounts::resetCounts () ;
    operatorCounts::TestType input  ( false ) ;
    operatorCounts::TestType output ( true ) ;
    output = algo::derefMove ( &input ) ;

    TEST_ASSERT ( 1 == operatorCounts::moveAssignmentCalled ) ;
    
    operatorCounts::resetCounts () ;
    char buffer [ 16 ] ;
    operatorCounts::TestType* outputAddress = reinterpret_cast < operatorCounts::TestType* >( buffer ) ;
    
    new ( outputAddress ) operatorCounts::TestType ( algo::derefMove ( &input ) ) ;
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
    
    algo::iterSwap ( &tmp1, &tmp2 ) ;
    
    TEST_ASSERT ( SWAPPED_LHS == swap1 ) ;
    TEST_ASSERT ( SWAPPED_RHS == swap2 ) ;
}

void testCopyBytesNotOverlapped ()
{
    std::array < int, 5 > const arr = { 1, 2, 3, 4, 5 } ;
    std::array < int, 5 > arr2 ;
    
    algo::copyBytesNotOverlapped ( arr.begin (), arr2.begin (), arr.size () ) ;
    TEST_ASSERT ( arr == arr2 ) ;
    
    std::array < int, 5 > arr3 = {} ;
    std::array < int, 5 > const arr4 = {} ;
    algo::copyBytesNotOverlapped ( arr.begin (),arr3.begin (), 0 ) ;
    TEST_ASSERT ( arr4 == arr3 ) ;
}

void testCopyBytesOverlapped ()
{
    std::array < int, 5 > const arr = { 1, 2, 3, 4, 5 } ;
    std::array < int, 5 > arr2 ;
    
    algo::copyBytesOverlapped ( arr.begin (), arr2.begin (), arr.size () ) ;
    TEST_ASSERT ( arr == arr2 ) ;
    
    // Test overlapping input/output
    algo::copyBytesOverlapped ( arr2.begin (), arr2.begin () + 1, arr.size () - 1 ) ;
    
    std::array < int, 5 > const expected = { 1, 1, 2, 3, 4 } ;
    TEST_ASSERT ( expected == arr2 ) ;
    
    algo::copyBytesOverlapped ( arr2.begin () + 2, arr2.begin (), arr.size () - 2 ) ;
    
    std::array < int, 5 > const expected2 = { 2, 3, 4, 3, 4 } ;
    TEST_ASSERT ( expected2 == arr2 ) ;
    
    std::array < int, 5 > arr3 = {} ;
    std::array < int, 5 > const arr4 = {} ;
    algo::copyBytesOverlapped ( arr.begin (), arr3.begin (), 0 ) ;
    TEST_ASSERT ( arr4 == arr3 ) ;
}

void testStripIter ()
{
    int arr [2] = {1,2};
    TEST_ASSERT ( &arr[0] == algo::stripIter ( &arr[0] ) ) ;
}

void testUnstripIter ()
{
    int arr [2] = {1,2};
    TEST_ASSERT ( &arr[0] == algo::unstripIter < int* > ( &arr[0] ) ) ;
}




typedef int IOType ;

const IOType io0 = 0 ;
const IOType io1 = 1 ;
const IOType io2 = 2 ;
const IOType io3 = 3 ;


struct TestStep
{
    mutable bool pre_op_ad_tag_called ;
    static int post_op_i_o_called ;
    IOType* iAddress ;
    IOType* oAddress ;
    
    TestStep ( IOType* iAddress, IOType* oAddress )
        : pre_op_ad_tag_called ( false )
        , iAddress ( iAddress )
        , oAddress ( oAddress )
    {}
    
    ~TestStep ()
    {}
    
    void apply ( algo::pre_op_i_tag, IOType& i) const
    {
        TEST_ASSERT ( &i == iAddress ) ;
        TEST_ASSERT ( io0 == i ) ;
        i = io1 ;
    }
    
    void apply ( algo::pre_op_o_tag, IOType& o) const
    {
        TEST_ASSERT ( &o == oAddress ) ;
        TEST_ASSERT ( io0 == o ) ;
        o = io1 ;
    }
    
    void apply ( algo::Operation_tag, IOType& i, IOType& o ) const
    {
        TEST_ASSERT ( &i == iAddress ) ;
        TEST_ASSERT ( io1 == i ) ;
        
        TEST_ASSERT ( &o == oAddress ) ;
        TEST_ASSERT ( io1 == o ) ;
        
        i = io2 ;
        o = io2 ;
    }
    
    void apply ( algo::post_op_i_tag, IOType& i) const
    {
        ++post_op_i_o_called ;
        TEST_ASSERT ( &i == iAddress ) ;
        TEST_ASSERT ( io2 == i ) ;
        i = io3 ;
    }
    
    void apply ( algo::post_op_o_tag, IOType& o) const
    {
        ++post_op_i_o_called ;
        TEST_ASSERT ( &o == oAddress ) ;
        TEST_ASSERT ( io2 == o ) ;
        o = io3 ;
    }
};

int TestStep::post_op_i_o_called ;

void testStep ()
{
    TestStep::post_op_i_o_called = 0 ;
    IOType i = io0 ;
    IOType o = io0 ;
    
    algo::step ( i, o, TestStep ( &i, &o ) ) ;
    
    TEST_ASSERT ( 2 == TestStep::post_op_i_o_called ) ;
    TestStep::post_op_i_o_called = 0 ;
    
    TEST_ASSERT ( io3 == i );
    TEST_ASSERT ( io3 == o ) ;
}

struct ATag {} ;

void testForwards ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    int* tmp = begin ;
    
    algo::Forwards < ATag >::apply ( ATag (), tmp ) ;
    TEST_ASSERT ( next == tmp ) ;
}

void testBackwards ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    int* tmp = next ;
    
    algo::Backwards < ATag >::apply ( ATag (), tmp ) ;
    TEST_ASSERT ( begin == tmp ) ;
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
};

typedef MyInt < int > NotABitwiseCopyableType ;
typedef MyInt < long > IsABitwiseCopyableType ;

ALGO_STATIC_ASSERT ( !algo::IsBitwiseCopyable < NotABitwiseCopyableType >::value, "Needs to be non-bitwise copyable for the tests" );


namespace algo
{
    template <>
    struct IsBitwiseCopyable < IsABitwiseCopyableType > : std::true_type
    {} ;
}

ALGO_STATIC_ASSERT ( algo::IsBitwiseCopyable < IsABitwiseCopyableType >::value, "Needs to be non-bitwise copyable for the tests" );

template < typename T >
void testCopy ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    auto endIter = algo::copy ( c.begin (), c.end (), output.begin () ) ;
    TEST_ASSERT ( endIter == output.end () ) ;
    TEST_ASSERT ( c == output ) ;
    
    auto endIter2 = algo::copy ( c.begin (), c.begin (), output.begin () ) ;
    TEST_ASSERT ( endIter2 == output.begin () ) ;
    
    auto endIter3 = algo::copy ( c.begin (), c.end (), c.begin () ) ;
    TEST_ASSERT ( endIter3 == c.end () ) ;
}

template < typename T >
void testCopyBackwards ()
{
    std::array < T, ARR_SIZE > c ;
    std::iota ( c.begin (), c.end (), ARR_SIZE ) ;
    
    std::array < T, ARR_SIZE > output ;
    
    auto endIter = algo::copy_backward ( c.begin (), c.end (), output.end () ) ;
    
    TEST_ASSERT ( endIter == output.begin () ) ;
    TEST_ASSERT ( c == output ) ;
    
    auto endIter2 = algo::copy_backward ( c.begin (), c.begin (), output.begin () ) ;
    TEST_ASSERT ( endIter2 == output.begin () ) ;
    
    auto endIter3 = algo::copy_backward ( c.begin (), c.end (), c.end () ) ;
    TEST_ASSERT ( endIter3 == c.begin () ) ;
}

template < typename T, size_t ARR_LENGTH >
void testFill ()
{
    const T FILL_VALUE = T ( 234 ) ;
    
    std::array < T, ARR_LENGTH > arr ;
    
    algo::fill ( arr.begin (), arr.end (), FILL_VALUE ) ;
    
    for ( auto i : arr )
    {
        TEST_ASSERT ( FILL_VALUE == i ) ;
    }
}

struct AlignOn256ByteBoundary
{
    char ar [ 256 ] ;
};

namespace algo
{
    template <>
    struct AlignmentOf < AlignOn256ByteBoundary, ALGO_ENABLE_IF_PARAM_DEFAULT > : std::integral_constant < int, 256 >
    {} ;
}

ALGO_STATIC_ASSERT ( 256 == algo::AlignmentOf < AlignOn256ByteBoundary >::value, "Not 256 byte aligned" ) ;

void testBufferCalculation ()
{
    // Trivial
    static const ptrdiff_t ARR_LENGTH = 1024 ;
    char arr [ ARR_LENGTH ] ;
    
    {
        algo::PointerAndSize data = { arr, 1 } ;
        char* const  begin = algo::BufferCalculation::calculateBegin < char > ( data ) ;
        TEST_ASSERT ( begin == arr ) ;
        
        char* const end = algo::BufferCalculation::calculateEnd < char > ( begin, data ) ;
        
        TEST_ASSERT ( arr + 1 == end ) ;
        TEST_ASSERT ( 1 == ( end - begin ) ) ;
    }
    
    {
        algo::PointerAndSize data = { arr, ARR_LENGTH } ;
        char* const  begin = algo::BufferCalculation::calculateBegin < char > ( data ) ;
        TEST_ASSERT ( begin == arr ) ;

        char* const end = algo::BufferCalculation::calculateEnd < char > ( begin, data ) ;

        TEST_ASSERT ( arr + ARR_LENGTH == end ) ;
        TEST_ASSERT ( ARR_LENGTH == ( end - begin ) ) ;
    }
    
    {
        // Now for something slightly more difficult
        algo::PointerAndSize data = { arr, ARR_LENGTH } ;
        
        algo::BufferRange <double> const range = algo::BufferCalculation::calculateRange < double > ( data ) ;
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
    
    algo::PointerAndSize data = { bufferStart, ARR_LENGTH } ;
    
    AlignOn256ByteBoundary* const begin = algo::BufferCalculation::calculateBegin < AlignOn256ByteBoundary > ( data ) ;
    TEST_ASSERT ( begin > reinterpret_cast < AlignOn256ByteBoundary* > ( bufferStart ) ) ;
    
    algo::PointerAndSize data2 = { bufferStart, 1 } ;
    
    AlignOn256ByteBoundary* const nullBegin = algo::BufferCalculation::calculateBegin < AlignOn256ByteBoundary > ( data2 ) ;
    TEST_ASSERT ( ALGO_NULLPTR == nullBegin ) ;
    
    // Rather trick the calculateEnd function by passing in a null begin
    AlignOn256ByteBoundary* const nullEnd = algo::BufferCalculation::calculateEnd < AlignOn256ByteBoundary > ( nullBegin, data ) ;
    
    TEST_ASSERT ( ALGO_NULLPTR == nullEnd ) ;
    
    algo::PointerAndSize data3 = { bufferStart, ARR_LENGTH - 1 } ;
    
    AlignOn256ByteBoundary* const end2 = algo::BufferCalculation::calculateEnd < AlignOn256ByteBoundary > ( begin, data3 ) ;
    
    TEST_ASSERT ( 3 == ( end2 - begin ) ) ;
}

void testStackBuffer ()
{
    const ptrdiff_t ARR_LENGTH = 1024 ;
    algo::StackBuffer<ARR_LENGTH> buffer ;
    algo::BufferRange < char > const range = buffer.getRange<char>() ;
    
    TEST_ASSERT ( ARR_LENGTH == ( buffer.end < char > () - buffer.begin < char > () ) ) ;
    TEST_ASSERT ( buffer.begin < char > () == range.begin ) ;
    TEST_ASSERT ( buffer.end < char > () == range.end ) ;
    TEST_ASSERT ( ARR_LENGTH == ( buffer.end < char > () - buffer.begin < char > () ) ) ;
    TEST_ASSERT ( ( ARR_LENGTH / sizeof ( double ) ) == ( buffer.end < double > () - buffer.begin < double > () ) ) ;
    
    char* bufferAddr = buffer.begin < char > () ;
    AlignOn256ByteBoundary* const begin = buffer.begin < AlignOn256ByteBoundary > () ;
    AlignOn256ByteBoundary* const end = buffer.end < AlignOn256ByteBoundary > () ;
    const ptrdiff_t len = end - begin ;
    
    if ( 0 == ( uintptr_t ( bufferAddr ) % algo::AlignmentOf < AlignOn256ByteBoundary >::value ) )
    {
        TEST_ASSERT ( ( ARR_LENGTH / algo::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
    }
    else
    {
        TEST_ASSERT ( ( ( ARR_LENGTH - 1 ) / algo::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
    }
}

template < class Protocol >
void testBufferAllocationProtocol ( bool checkIfZeroed )
{
    const ptrdiff_t sizeToAllocate = 1024 * 1024 * 8 ;

    const algo::PointerAndSize data = Protocol::allocate ( sizeToAllocate ) ;
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
    
    static algo::PointerAndSize allocate ( const ptrdiff_t size )
    {
        allocate_called = true ;
        return { TEST_BUFFER, TEST_BUFFER_SIZE } ;
    }
    
    static void deallocate ( algo::PointerAndSize data )
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
        algo::AllocatingBuffer < TestAllocatorProtocol > buffer ( ARR_LENGTH ) ;
        
        TEST_ASSERT ( TestAllocatorProtocol::allocate_called ) ;
        TEST_ASSERT ( !TestAllocatorProtocol::deallocate_called ) ;
        TestAllocatorProtocol::reset () ;
        
        
        TEST_ASSERT ( TestAllocatorProtocol::TEST_BUFFER_SIZE == ( buffer.end < char > () - buffer.begin < char > () ) ) ;
        TEST_ASSERT ( ( TestAllocatorProtocol::TEST_BUFFER_SIZE / sizeof ( double ) ) == ( buffer.end < double > () - buffer.begin < double > () ) ) ;
    
        char* bufferAddr = buffer.begin < char > () ;
        AlignOn256ByteBoundary* const begin = buffer.begin < AlignOn256ByteBoundary > () ;
        AlignOn256ByteBoundary* const end = buffer.end < AlignOn256ByteBoundary > () ;
        const ptrdiff_t len = end - begin ;
        
        algo::BufferRange<AlignOn256ByteBoundary> const range = buffer.getRange < AlignOn256ByteBoundary > () ;
        
        TEST_ASSERT ( range.begin == begin ) ;
        TEST_ASSERT ( range.end == end ) ;
    
        if ( 0 == ( uintptr_t ( bufferAddr ) % algo::AlignmentOf < AlignOn256ByteBoundary >::value ) )
        {
            TEST_ASSERT ( ( TestAllocatorProtocol::TEST_BUFFER_SIZE / algo::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
        }
        else
        {
            TEST_ASSERT ( ( ( TestAllocatorProtocol::TEST_BUFFER_SIZE - 1 ) / algo::AlignmentOf < AlignOn256ByteBoundary >::value ) == len ) ;
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
    typedef RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    
    {
        algo::ObjectProctor < DestructionType > proctor ( toDestroy ) ;
        proctor.disarm () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::ObjectProctor < DestructionType > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( destroyed ) ;
    
    destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    {
        algo::ObjectProctor < DestructionType > proctor ( *toDestroy ) ;
    }
    TEST_ASSERT ( destroyed ) ;
}

void testTrivialObjectProctor ()
{
    {
        int obj = 0 ;
        algo::ObjectProctor < int > proctor ( &obj ) ;
    }
    
    {
        int obj = 0 ;
        algo::ObjectProctor < int > proctor ( &obj ) ;
        proctor.disarm() ;
    }
    
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionSkipped ( destroyed ) ;
    
    DestructionSkipped* const toDestroy = reinterpret_cast < DestructionSkipped* >( &arr ) ;

    {
        algo::ObjectProctor < DestructionSkipped > proctor ( toDestroy ) ;
        proctor.disarm () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::ObjectProctor < DestructionSkipped > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::ObjectProctor < DestructionSkipped > proctor ( *toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    toDestroy->~DestructionSkipped () ;
}

void testIterProctorLengthOne ()
{
    typedef RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( destroyed ) ;
}

void testIterProctorLengthTwo ()
{
    typedef RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    bool destroyed2 = false ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    DestructionType* const toDestroy2 = toDestroy + 1 ;
    
    new (toDestroy) DestructionType ( destroyed ) ;
    new (toDestroy2) DestructionType ( destroyed2 ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    TEST_ASSERT ( !destroyed2 ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    TEST_ASSERT ( !destroyed2 ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( destroyed ) ;
    TEST_ASSERT ( !destroyed2 ) ;
    
    destroyed = false ;
    new (toDestroy) DestructionType ( destroyed ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    TEST_ASSERT ( destroyed2 ) ;
    
    destroyed2 = false ;
    new (toDestroy2) DestructionType ( destroyed2 ) ;
    
    {
        algo::IterProctor < DestructionType* > proctor ( toDestroy ) ;
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
        algo::IterProctor < int* > proctor ( &obj ) ;
    }
    
    {
        int obj = 0 ;
        algo::IterProctor < int* > proctor ( &obj ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionSkipped ( destroyed ) ;
    
    DestructionSkipped* const toDestroy = reinterpret_cast < DestructionSkipped* >( &arr ) ;
    
    {
        algo::IterProctor < DestructionSkipped* > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::IterProctor < DestructionSkipped* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::IterProctor < DestructionSkipped* > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    toDestroy->~DestructionSkipped () ;
}

void testBufferProctorLengthOne ()
{
    typedef RecordDestruction < int > DestructionType ;
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionType ( destroyed ) ;
    
    DestructionType* const toDestroy = reinterpret_cast < DestructionType* >( &arr ) ;
    
    {
        algo::BufferProctor < DestructionType > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::BufferProctor < DestructionType > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::BufferProctor < DestructionType > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( destroyed ) ;
}

void testTrivialBufferProctor ()
{
    // Test the trivial (stateless) proctor
    
    {
        int obj = 0 ;
        algo::BufferProctor < int > proctor ( &obj ) ;
    }
    
    {
        int obj = 0 ;
        algo::BufferProctor < int > proctor ( &obj ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    
    char arr[16];
    bool destroyed = false ;
    new (&arr) DestructionSkipped ( destroyed ) ;
    
    DestructionSkipped* const toDestroy = reinterpret_cast < DestructionSkipped* >( &arr ) ;
    
    {
        algo::BufferProctor < DestructionSkipped > proctor ( toDestroy ) ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::BufferProctor < DestructionSkipped > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    {
        algo::BufferProctor < DestructionSkipped > proctor ( toDestroy ) ;
        proctor.incrementEnd () ;
        proctor.incrementStart () ;
    }
    TEST_ASSERT ( !destroyed ) ;
    
    toDestroy->~DestructionSkipped () ;
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
        algo::copy ( c.begin (), c.end (), output.begin () ) ;
    }
    
    std::cout << t.stop () << '\n' ;
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
        algo::copy_backward ( c.begin (), c.end (), output.end () ) ;
    }
    std::cout << t.stop () << '\n' ;
}




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
        algo::sort(f, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >(), Tag (), algo::Int<N>(), SwapIfKind (), IndexType () );
    }
};

struct BoundedInsertionSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct CountedInsertionSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion(f, N, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct BoundedInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct CountededInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel(f, N, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct BoundedUnstableInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel_unstable(f, l, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
    }
} ;

struct CountededUnstableInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel_unstable(f, N, myLess< typename ALGO_CALL::IteratorTraits < Iter >::value_type >() );
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
            std::cout << "Error " ;
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
            std::cout << "Incorrect sorting on " << bs << '\n' ;
        }
    }
}

void test_swap(bool swapValue)
{
    const int aOrig = 1 ;
    const int bOrig = 2 ;
    const int aExpected = swapValue ? bOrig : aOrig ;
    const int bExpected = swapValue ? aOrig : bOrig ;
    
    int a = aOrig ;
    int b = bOrig ;
    
    algo::swap_if ( swapValue, a, b, algo::Unpredictable () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    algo::swap_if ( swapValue, a, b, algo::Ternary () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    algo::swap_if ( swapValue, a, b, algo::Consistent () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;

    a = aOrig ;
    b = bOrig ;
    
    algo::swap_if ( swapValue, a, b, algo::PredictableTrue () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    algo::swap_if ( swapValue, a, b, algo::PredictableFalse () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
    
    a = aOrig ;
    b = bOrig ;
    
    algo::swap_if ( swapValue, a, b, algo::NotInline < algo::PredictableFalse > () ) ;
    
    TEST_ASSERT ( aExpected == a ) ;
    TEST_ASSERT ( bExpected == b ) ;
}

void test_select_if(bool x)
{
    const int ifFalse = 1 ;
    const int ifTrue = 2 ;
    
    const int selectedValue = ( x ? ifTrue : ifFalse ) ;
    
    TEST_ASSERT( selectedValue == algo::select_if(x, ifFalse, ifTrue, algo::Unpredictable () ) ) ;
    TEST_ASSERT( selectedValue == algo::select_if(x, ifFalse, ifTrue, algo::Ternary () ) ) ;
    TEST_ASSERT( selectedValue == algo::select_if(x, ifFalse, ifTrue, algo::Consistent () ) ) ;
    TEST_ASSERT( selectedValue == algo::select_if(x, ifFalse, ifTrue, algo::PredictableFalse () ) ) ;
    TEST_ASSERT( selectedValue == algo::select_if(x, ifFalse, ifTrue, algo::PredictableTrue () ) ) ;
    TEST_ASSERT( selectedValue == algo::select_if(x, ifFalse, ifTrue, algo::NotInline < algo::PredictableTrue > () ) ) ;
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

struct MinIterBounded
{
    template < typename Iter, typename Cmp >
    Iter operator () ( Iter f, Iter l, Cmp cmp ) const
    {
        return algo::minIter ( f, l, cmp ) ;
    }
} ;

struct MinIterCounted
{
    template < typename Iter, typename Cmp >
    Iter operator () ( Iter f, Iter l, Cmp cmp ) const
    {
        return algo::minIter ( f, l - f, cmp ) ;
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
        return algo::maxIter ( f, l, cmp ) ;
    }
} ;

struct MaxIterCounted
{
    template < typename Iter, typename Cmp >
    Iter operator () ( Iter f, Iter l, Cmp cmp ) const
    {
        return algo::maxIter ( f, l - f, cmp ) ;
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
    algo::rotateRightByOne ( shortArray.begin (), shortArray.begin () + 1, RotateChoice () ) ;
    
    std::array < int, 2 > shortArrayResult = { 8, 9 } ;
    
    TEST_ASSERT ( shortArray == shortArrayResult ) ;
    
    std::array < int, 10 > const original = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 } ;
    std::array < int, 10 > arr = original ;
    
    // Rotate the 9th element to be the first.
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 } ;
    TEST_ASSERT ( res == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res2 = { 1, 0, 9, 8, 7, 6, 5, 4, 3, 2 } ;
    TEST_ASSERT ( res2 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res3 = { 2, 1, 0, 9, 8, 7, 6, 5, 4, 3 } ;
    TEST_ASSERT ( res3 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res4 = { 3, 2, 1, 0, 9, 8, 7, 6, 5, 4 } ;
    TEST_ASSERT ( res4 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res5 = { 4, 3, 2, 1, 0, 9, 8, 7, 6, 5 } ;
    TEST_ASSERT ( res5 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res6 = { 5, 4, 3, 2, 1, 0, 9, 8, 7, 6 } ;
    TEST_ASSERT ( res6 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res7 = { 6, 5, 4, 3, 2, 1, 0, 9, 8, 7  } ;
    TEST_ASSERT ( res7 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res8 = { 7, 6, 5, 4, 3, 2, 1, 0, 9, 8  } ;
    TEST_ASSERT ( res8 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res9 = { 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 } ;
    TEST_ASSERT ( res9 == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), &arr[9], RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
    
    algo::rotateRightByOne (arr.begin (), arr.begin (), RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
}


template < typename RotateChoice >
void testRotateLeftByOne ()
{
    std::array < int, 2 > shortArray = { 9, 8 } ;
    algo::rotateLeftByOne ( shortArray.begin (), shortArray.begin () + 1, RotateChoice () ) ;
    
    std::array < int, 2 > shortArrayResult = { 8, 9 } ;
    
    TEST_ASSERT ( shortArray == shortArrayResult ) ;
    
    std::array < int, 10 > const original = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 } ;
    std::array < int, 10 > arr = original ;
    
    // Rotate the 9th element to be the first.
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res = { 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 } ;
    TEST_ASSERT ( res == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res2 = { 7, 6, 5, 4, 3, 2, 1, 0, 9, 8  } ;
    TEST_ASSERT ( res2 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res3 = { 6, 5, 4, 3, 2, 1, 0, 9, 8, 7  } ;
    TEST_ASSERT ( res3 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res4 = { 5, 4, 3, 2, 1, 0, 9, 8, 7, 6 } ;
    TEST_ASSERT ( res4 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res5 = { 4, 3, 2, 1, 0, 9, 8, 7, 6, 5 } ;
    TEST_ASSERT ( res5 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res6 = { 3, 2, 1, 0, 9, 8, 7, 6, 5, 4 } ;
    TEST_ASSERT ( res6 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res7 = { 2, 1, 0, 9, 8, 7, 6, 5, 4, 3 } ;
    TEST_ASSERT ( res7 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res8 = { 1, 0, 9, 8, 7, 6, 5, 4, 3, 2 } ;
    TEST_ASSERT ( res8 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    std::array < int, 10 > const res9 = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 } ;
    TEST_ASSERT ( res9 == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), &arr[9], RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
    
    algo::rotateLeftByOne (arr.begin (), arr.begin (), RotateChoice ());
    TEST_ASSERT ( original == arr ) ;
}



void testStepOver ()
{
    std::array < int, 2 > shortArray = { 9, 8 } ;
    std::array < int, 2 > shortArray2 = {} ;

    algo::stepOver ( shortArray.begin (), shortArray.end (), shortArray2.begin (), algo::CopyForwardOperation::type () ) ;
    TEST_ASSERT ( shortArray2[0] == shortArray[0] ) ;
    TEST_ASSERT ( shortArray2[1] == shortArray[1] ) ;
}

void testStepCounted ()
{
    std::array < int, 2 > shortArray = { 9, 8 } ;
    std::array < int, 2 > shortArray2 = {} ;
    
    algo::stepCounted ( shortArray.begin (), 2, shortArray2.begin (), algo::CopyForwardOperation::type () ) ;
    TEST_ASSERT ( shortArray2[0] == shortArray[0] ) ;
    TEST_ASSERT ( shortArray2[1] == shortArray[1] ) ;
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
    testSortingCombinationsSwapIfTagInlineNotInline < SortingTag, algo::NotInline < SwapIfTag > > () ;
}

template < typename SortingTag >
void testSortingCombinationsSortingTag ()
{
    testSortingCombinationsSwapIfTag <SortingTag, algo::Unpredictable > () ;
    testSortingCombinationsSwapIfTag <SortingTag, algo::Ternary > () ;
    testSortingCombinationsSwapIfTag <SortingTag, algo::Consistent > () ;
    testSortingCombinationsSwapIfTag <SortingTag, algo::PredictableFalse > () ;
    testSortingCombinationsSwapIfTag <SortingTag, algo::PredictableTrue > () ;
}

namespace property
{


struct Tag1 {} ;
typedef short Value1 ;

struct Tag2 {} ;
typedef double Value2 ;

struct Tag3 {} ;
typedef long Value3 ;

struct Tag4 {} ;
typedef std::string Value4 ;

typedef algo::ValueAndProperty < Tag1, Value1 > V1 ;
typedef algo::AddPropertyType < Tag2, Value2, V1 >::type V2 ;
typedef algo::AddPropertyType < Tag3, Value3, V2 >::type V3 ;
typedef algo::AddPropertyType < Tag4, Value4, V3 >::type V4 ;

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
    algo::ValueAndProperty < Tag1, Value1 > lesser = { 1 } ;
    algo::ValueAndProperty < Tag1, Value1 > greater = { 2 } ;
    
    testRelationalOperations ( lesser, greater ) ;
}

void testCompoundRelationalOperations ()
{
    typedef algo::Compound < long , double > TestType ;
    
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
    PropertyValue originalValue = algo::getValue < PropertyName > ( set ) ;
    
    PropertySet updated = algo::setValue < PropertyName > ( set, value, algo::ByReturnValue () ) ;
    PropertySet updated2 = algo::setValue < PropertyName > ( constSet, value, algo::ByReturnValue () ) ;
    
    TEST_ASSERT ( algo::getValue < PropertyName > ( set ) == originalValue ) ;
    TEST_ASSERT ( algo::getValue < PropertyName > ( constSet ) == originalValue ) ;
    TEST_ASSERT ( algo::getValue < PropertyName > ( updated ) == value ) ;
    TEST_ASSERT ( algo::getValue < PropertyName > ( updated2 ) == value ) ;
    
    algo::setValue < PropertyName > ( set, value, algo::InPlace () ) ;
    TEST_ASSERT ( algo::getValue < PropertyName > ( set ) == value ) ;
}

void testProperty ()
{
    const Value1 value1 = 123 ;
    const Value2 value2 = 3.0 ;
    const Value3 value3 = 234923432 ;
    const Value4 value4 = "abcde" ;
    
    V1 a = { value1 } ;
    V1 const aConst = a ;
    
    V2 b = algo::addProperty < Tag2 > ( a, value2 ) ;
    V2 const bConst = b ;
    
    V3 c = algo::addProperty < Tag3 >( b, value3 ) ;
    V3 const cConst = c ;
    
    V4 d = algo::addProperty < Tag4 >( c, value4 ) ;
    V4 const dConst = d ;
    
    
    // Test HasProperty
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag1, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag1, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag1, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag1, V4>::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (!algo::HasProperty<Tag2, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag2, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag2, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag2, V4>::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (!algo::HasProperty<Tag3, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (!algo::HasProperty<Tag3, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag3, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag3, V4>::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (!algo::HasProperty<Tag4, V1>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (!algo::HasProperty<Tag4, V2>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (!algo::HasProperty<Tag4, V3>::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (algo::HasProperty<Tag4, V4>::type ()), "unexpected" ) ;
    
    
    // test ValueType
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V1 >::type, Value1 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V2 >::type, Value1 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V3 >::type, Value1 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V4 >::type, Value1 >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag2, V2 >::type, Value2 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag2, V3 >::type, Value2 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag2, V4 >::type, Value2 >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag3, V3 >::type, Value3 >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag3, V4 >::type, Value3 >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag4, V4 >::type, Value4 >::type ()), "unexpected" ) ;
    
    // Test the const version too
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V1 const >::type, Value1 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V2 const >::type, Value1 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V3 const >::type, Value1 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag1, V4 const >::type, Value1 const >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag2, V2 const >::type, Value2 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag2, V3 const >::type, Value2 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag2, V4 const >::type, Value2 const >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag3, V3 const >::type, Value3 const >::type ()), "unexpected" ) ;
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag3, V4 const >::type, Value3 const >::type ()), "unexpected" ) ;
    
    ALGO_STATIC_ASSERT ( (std::is_same < algo::ValueType < Tag4, V4 const >::type, Value4 const >::type ()), "unexpected" ) ;
    
    
    
    TEST_ASSERT ( algo::getValue < Tag1 > ( a ) == value1 ) ;
    TEST_ASSERT ( algo::getValue < Tag1 > ( b ) == value1 ) ;
    TEST_ASSERT ( algo::getValue < Tag1 > ( c ) == value1 ) ;
    TEST_ASSERT ( algo::getValue < Tag1 > ( d ) == value1 ) ;
    
    TEST_ASSERT ( algo::getValue < Tag2 > ( b ) == value2 ) ;
    TEST_ASSERT ( algo::getValue < Tag2 > ( c ) == value2 ) ;
    TEST_ASSERT ( algo::getValue < Tag2 > ( d ) == value2 ) ;
    
    TEST_ASSERT ( algo::getValue < Tag3 > ( c ) == value3 ) ;
    TEST_ASSERT ( algo::getValue < Tag3 > ( d ) == value3 ) ;
    
    TEST_ASSERT ( algo::getValue < Tag4 > ( d ) == value4 ) ;
    
    
    TEST_ASSERT ( algo::getValue < Tag1 > ( aConst ) == value1 ) ;
    TEST_ASSERT ( algo::getValue < Tag1 > ( bConst ) == value1 ) ;
    TEST_ASSERT ( algo::getValue < Tag1 > ( cConst ) == value1 ) ;
    TEST_ASSERT ( algo::getValue < Tag1 > ( dConst ) == value1 ) ;
    
    TEST_ASSERT ( algo::getValue < Tag2 > ( bConst ) == value2 ) ;
    TEST_ASSERT ( algo::getValue < Tag2 > ( cConst ) == value2 ) ;
    TEST_ASSERT ( algo::getValue < Tag2 > ( dConst ) == value2 ) ;
    
    TEST_ASSERT ( algo::getValue < Tag3 > ( cConst ) == value3 ) ;
    TEST_ASSERT ( algo::getValue < Tag3 > ( dConst ) == value3 ) ;
    
    TEST_ASSERT ( algo::getValue < Tag4 > ( dConst ) == value4 ) ;
    
    // By Reference
    TEST_ASSERT ( algo::getValueByReference < Tag4 > ( d ) == value4 ) ;
    TEST_ASSERT ( algo::getValueByReference < Tag4 > ( dConst ) == value4 ) ;
    
    
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
    V2 bExpected = algo::addProperty < Tag2 > ( aExpected, updatedValue2 ) ;
    V3 cExpected = algo::addProperty < Tag3 >( bExpected, updatedValue3 ) ;
    V4 dExpected = algo::addProperty < Tag4 >( cExpected, updatedValue4 ) ;
    
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
        V2 val2 = algo::addOrUpdateValue < Tag2 > ( aConst, value2 ) ;
        TEST_ASSERT ( val2 == bConst ) ;
        
        Value2 value2ToMove = value2 ;
        V2 val2a = algo::addOrUpdateValue < Tag2 > ( aConst, std::move ( value2ToMove ) ) ;
        TEST_ASSERT ( val2a == bConst ) ;
        
        // OK, try this on a moveable type ... Value4 is a string
        Value4 value4a = value4 ;
        
        TEST_ASSERT ( dConst == algo::addOrUpdateValue < Tag4 > ( cConst, value4 ) ) ;
        TEST_ASSERT ( dConst == algo::addOrUpdateValue < Tag4 > ( cConst, value4a ) ) ;
        TEST_ASSERT ( dConst == algo::addOrUpdateValue < Tag4 > ( cConst, std::move ( value4a ) ) ) ;
        
        // Now test the UpdateValue paths
        V4 tmp = { updatedValue1, updatedValue2, updatedValue3, value4 } ;
        V4 tmp2 = algo::addOrUpdateValue < Tag4 > ( tmp, updatedValue4 ) ;
        
        TEST_ASSERT ( value4 == algo::getValueByReference < Tag4 > ( tmp ) ) ;
        TEST_ASSERT ( updatedValue4 == algo::getValueByReference < Tag4 > ( tmp2 ) ) ;
    }
    
    // test mergePropertySets
    {
        typedef algo::ValueAndProperty < Tag1, Value1 > VP1 ;
        typedef algo::ValueAndProperty < Tag2, Value2 > VP2 ;
        typedef algo::ValueAndProperty < Tag3, Value3 > VP3 ;
        typedef algo::ValueAndProperty < Tag4, Value4 > VP4 ;
        typedef algo::Compound < VP1, VP2 > C1 ;
        typedef algo::Compound < VP3, VP4 > C2 ;
        
        C1 compound1 = { value1, value2 } ;
        C2 compound2 = { value3, value4 } ;
        
        algo::MergePropertySetsType < C1, C2 >::type tmp = algo::mergePropertySets ( compound1, compound2 ) ;
        
        TEST_ASSERT ( value1 == algo::getValueByReference < Tag1 > ( tmp ) ) ;
        TEST_ASSERT ( value2 == algo::getValueByReference < Tag2 > ( tmp ) ) ;
        TEST_ASSERT ( value3 == algo::getValueByReference < Tag3 > ( tmp ) ) ;
        TEST_ASSERT ( value4 == algo::getValueByReference < Tag4 > ( tmp ) ) ;
        
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
    TestVisitor x = algo::visit(a, tmp ) ;
    
    TEST_ASSERT ( x.visitedTag1 ) ;
    TEST_ASSERT ( x.visitedTag2 ) ;
    TEST_ASSERT ( x.visitedTag3 ) ;
    TEST_ASSERT ( x.visitedTag4 ) ;
}
    
template < int N >
struct TagN {} ;

typedef unsigned long long TaggedValueType ;

typedef algo::ValueAndProperty < TagN < 0 >, TaggedValueType > TV0 ;
typedef algo::AddPropertyType < TagN < 1 >, TaggedValueType, TV0 >::type TV1 ;
typedef algo::AddPropertyType < TagN < 2 >, TaggedValueType, TV1 >::type TV2 ;
typedef algo::AddPropertyType < TagN < 3 >, TaggedValueType, TV2 >::type TV3 ;
typedef algo::AddPropertyType < TagN < 4 >, TaggedValueType, TV3 >::type TV4 ;
typedef algo::AddPropertyType < TagN < 5 >, TaggedValueType, TV4 >::type TV5 ;
typedef algo::AddPropertyType < TagN < 6 >, TaggedValueType, TV5 >::type TV6 ;
typedef algo::AddPropertyType < TagN < 7 >, TaggedValueType, TV6 >::type TV7 ;

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


typedef algo::ValueAndProperty < TagN < 0 >, TaggedValueType > TAV0 ;
typedef algo::ValueAndProperty < TagN < 1 >, TaggedValueType > TAV1 ;
typedef algo::ValueAndProperty < TagN < 2 >, TaggedValueType > TAV2 ;
typedef algo::ValueAndProperty < TagN < 3 >, TaggedValueType > TAV3 ;
typedef algo::ValueAndProperty < TagN < 4 >, TaggedValueType > TAV4 ;
typedef algo::ValueAndProperty < TagN < 5 >, TaggedValueType > TAV5 ;
typedef algo::ValueAndProperty < TagN < 6 >, TaggedValueType > TAV6 ;
typedef algo::ValueAndProperty < TagN < 7 >, TaggedValueType > TAV7 ;

typedef algo::Compound < TAV0, TAV1 > TAV01 ;
typedef algo::Compound < TAV2, TAV3 > TAV23 ;
typedef algo::Compound < TAV4, TAV5 > TAV45 ;
typedef algo::Compound < TAV6, TAV7 > TAV67 ;


typedef algo::Compound < TAV01, TAV23 > TAV0123 ;
typedef algo::Compound < TAV45, TAV67 > TAV4567 ;

typedef algo::Compound < TAV0123, TAV4567 > TAV01234567 ;

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
            return algo::getValueByReference < TagN < 0 > > ( x ) ;
        case 1 :
            return algo::getValueByReference < TagN < 1 > > ( x ) ;
        case 2 :
            return algo::getValueByReference < TagN < 2 > > ( x ) ;
        case 3 :
            return algo::getValueByReference < TagN < 3 > > ( x ) ;
        case 4 :
            return algo::getValueByReference < TagN < 4 > > ( x ) ;
        case 5 :
            return algo::getValueByReference < TagN < 5 > > ( x ) ;
        case 6 :
            return algo::getValueByReference < TagN < 6 > > ( x ) ;
        case 7 :
            return algo::getValueByReference < TagN < 7 > > ( x ) ;
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
            algo::setValue < TagN < 0 > > ( x, y, algo::InPlace () ) ;
            break ;
        case 1 :
            algo::setValue < TagN < 1 > > ( x, y, algo::InPlace () ) ;
            break ;
        case 2 :
            algo::setValue < TagN < 2 > > ( x, y, algo::InPlace () ) ;
            break ;
        case 3 :
            algo::setValue < TagN < 3 > > ( x, y, algo::InPlace () ) ;
            break ;
        case 4 :
            algo::setValue < TagN < 4 > > ( x, y, algo::InPlace () ) ;
        case 5 :
            algo::setValue < TagN < 5 > > ( x, y, algo::InPlace () ) ;
        case 6 :
            algo::setValue < TagN < 6 > > ( x, y, algo::InPlace () ) ;
        case 7 :
            algo::setValue < TagN < 7 > > ( x, y, algo::InPlace () ) ;
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

    std::cout << "A " << totalTimeA << ' ' << aAccumulated
                << ", B " << totalTimeB << ' ' << bAccumulated
                << ", C " << totalTimeC << ' ' << cAccumulated << '\n' ;
}
typedef algo::RemoveProperty < TagN < 1 >, TV0 >::type TV0A ;
ALGO_STATIC_ASSERT( (std::is_same < TV0A, TV0 >::type::value), "" ) ;
    
typedef algo::RemoveProperty < TagN < 2 >, TAV01 >::type TAV01A ;
ALGO_STATIC_ASSERT( (std::is_same < TAV01A, TAV01 >::type::value), "" ) ;
    
// Removed from M0 of a Compound
typedef algo::RemoveProperty < TagN < 0 >, TAV01234567 >::type TAV01234567NoTag0 ;
    
// Removed from M0 of a Compound
typedef algo::RemoveProperty < TagN < 7 >, TAV01234567NoTag0 >::type TAV01234567NoTag0NoTag7 ;
    
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 1 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 2 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 3 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 4 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 5 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 6 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 7 >, TAV01234567NoTag0 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (!algo::HasProperty<TagN < 0 >, TAV01234567NoTag0 >::type::value), "" ) ;
    
    
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 1 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 2 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 3 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 4 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 5 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (algo::HasProperty<TagN < 6 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (!algo::HasProperty<TagN < 0 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;
ALGO_STATIC_ASSERT ( (!algo::HasProperty<TagN < 7 >, TAV01234567NoTag0NoTag7 >::type::value), "" ) ;

void testConvertPropertySet ()
{
    TV7 val = {} ;
    algo::setValue < TagN < 0 > > ( val, 1, algo::InPlace () ) ;
    algo::setValue < TagN < 1 > > ( val, 2, algo::InPlace () ) ;
    algo::setValue < TagN < 2 > > ( val, 3, algo::InPlace () ) ;
    algo::setValue < TagN < 3 > > ( val, 4, algo::InPlace () ) ;
    algo::setValue < TagN < 4 > > ( val, 5, algo::InPlace () ) ;
    algo::setValue < TagN < 5 > > ( val, 6, algo::InPlace () ) ;
    algo::setValue < TagN < 6 > > ( val, 7, algo::InPlace () ) ;
    algo::setValue < TagN < 7 > > ( val, 8, algo::InPlace () ) ;
    
    TV7 const val2 = algo::convertPropertySet < TV7 > ( val ) ;
    TEST_ASSERT ( val2 == val ) ;
    
    TV6 const val3 = algo::convertPropertySet < TV6 > ( val ) ;
    
    TEST_ASSERT ( algo::getValue < TagN < 0 > > ( val3 ) == algo::getValue < TagN < 0 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 1 > > ( val3 ) == algo::getValue < TagN < 1 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 2 > > ( val3 ) == algo::getValue < TagN < 2 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 3 > > ( val3 ) == algo::getValue < TagN < 3 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 4 > > ( val3 ) == algo::getValue < TagN < 4 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 5 > > ( val3 ) == algo::getValue < TagN < 5 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 6 > > ( val3 ) == algo::getValue < TagN < 6 > > ( val ) ) ;
    
    TV7 const val4 = algo::convertPropertySet < TV7 > ( val3 ) ;
    TEST_ASSERT ( algo::getValue < TagN < 0 > > ( val4 ) == algo::getValue < TagN < 0 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 1 > > ( val4 ) == algo::getValue < TagN < 1 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 2 > > ( val4 ) == algo::getValue < TagN < 2 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 3 > > ( val4 ) == algo::getValue < TagN < 3 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 4 > > ( val4 ) == algo::getValue < TagN < 4 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 5 > > ( val4 ) == algo::getValue < TagN < 5 > > ( val ) ) ;
    TEST_ASSERT ( algo::getValue < TagN < 6 > > ( val4 ) == algo::getValue < TagN < 6 > > ( val ) ) ;
    
    TEST_ASSERT ( algo::getValue < TagN < 7 > > ( val4 ) == 0 ) ;
}

} // namespace property



typedef algo::BasicUnboundedRange < int* >::type AnUnboundedRange ;

ALGO_STATIC_ASSERT ( algo::IsARange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::IsABoundedRange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::IsACountedRange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::IsAFiniteRange < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( algo::RepeatableIterator < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, algo::StartIteratorType < AnUnboundedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::HasCountO1Time < AnUnboundedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::IsAReversedRange < AnUnboundedRange >::type::value, "unexpected" ) ;

typedef algo::BasicCountedRange < int* >::type ACountedRange ;

ALGO_STATIC_ASSERT ( algo::IsARange < ACountedRange>::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::IsABoundedRange < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( algo::IsACountedRange < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( algo::IsAFiniteRange < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( algo::RepeatableIterator < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, algo::StartIteratorType < ACountedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < ptrdiff_t, algo::CountType < ACountedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( algo::HasCountO1Time < ACountedRange >::type::value, "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::IsAReversedRange < ACountedRange >::type::value, "unexpected" ) ;

typedef algo::BasicBoundedRange < int* >::type ABoundedRange ;

ALGO_STATIC_ASSERT ( (algo::IsARange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::IsABoundedRange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (!algo::IsACountedRange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::IsAFiniteRange < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::RepeatableIterator < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, algo::StartIteratorType < ABoundedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, algo::EndIteratorType < ABoundedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::HasCountO1Time < ABoundedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( !algo::IsAReversedRange < ABoundedRange >::type::value, "unexpected" ) ;


ALGO_STATIC_ASSERT ( (!algo::HasCountO1Time < algo::BasicBoundedRange < std::map < int, int >::iterator, std::map < int, int >::iterator >::type >::type::value), "unexpected" ) ;

typedef algo::BasicReversedRange < ABoundedRange >::type AReversedRange ;

ALGO_STATIC_ASSERT ( (algo::IsARange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::IsABoundedRange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (!algo::IsACountedRange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::IsAFiniteRange < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::RepeatableIterator < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, algo::StartIteratorType < AReversedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (std::is_same < int*, algo::EndIteratorType < AReversedRange >::type >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( (algo::HasCountO1Time < AReversedRange >::type::value), "unexpected" ) ;
ALGO_STATIC_ASSERT ( algo::IsAReversedRange < AReversedRange >::type::value, "unexpected" ) ;

template < typename Range >
void testRangeNavigation ( Range& a, bool isEmptyAfterTwoAdvances )
{
    Range copy = a ;
    TEST_ASSERT ( 0 == algo::distance ( copy, a ) ) ;
    TEST_ASSERT ( 0 == algo::distance ( a, copy ) ) ;
    
    TEST_ASSERT ( 0 == algo::deref ( a ) ) ;
    algo::advance ( a, 1, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( a ) ) ;
    
    TEST_ASSERT ( 1 == algo::distance ( copy, a ) ) ;
    TEST_ASSERT ( -1 == algo::distance ( a, copy ) ) ;
    
    algo::advance ( a, 1, algo::InPlace () ) ;
    TEST_ASSERT ( isEmptyAfterTwoAdvances == algo::isEmpty ( a ) ) ;
    algo::advance ( a, -2, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    
    TEST_ASSERT ( 0 == algo::deref ( a ) ) ;
    algo::successor ( a, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( a ) ) ;
    algo::successor ( a, algo::InPlace () ) ;
    TEST_ASSERT ( isEmptyAfterTwoAdvances == algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( 2 == algo::distance ( copy, a ) ) ;
    TEST_ASSERT ( -2 == algo::distance ( a, copy ) ) ;
    algo::predecessor ( a, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( a ) ) ;
    algo::predecessor ( a, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( 0 == algo::deref ( a ) ) ;
    
    TEST_ASSERT ( 0 == algo::distance ( copy, a ) ) ;
    TEST_ASSERT ( 0 == algo::distance ( a, copy ) ) ;
}

void testBasicRanges ()
{
    typedef algo::BasicBoundedRange < int* >::type BoundedIntPtr ;
    typedef algo::BasicCountedRange < int* >::type CountedIntPtr ;
    typedef algo::BasicUnboundedRange < int* >::type UnboundedIntPtr ;
    // Bounded range and a count.
    typedef algo::Compound < BoundedIntPtr, algo::ValueAndProperty < algo::Count, ptrdiff_t > > BoundedCountedIntPtr ;
    
    int arr [] = { 0, 1 } ;
    
    BoundedIntPtr a = { arr, arr + 2 } ;
    CountedIntPtr b = { arr, 2 } ;
    BoundedCountedIntPtr c = { arr, arr + 2, 2 } ;
    UnboundedIntPtr d = { arr } ;
    
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( !algo::isEmpty ( b ) ) ;
    TEST_ASSERT ( !algo::isEmpty ( c ) ) ;
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
    
    testRangeNavigation ( a, true ) ;
    testRangeNavigation ( b, true ) ;
    testRangeNavigation ( c, true ) ;
    testRangeNavigation ( d, false ) ;
}

void testDeduceRange1 ()
{
    int arr [] = { 1 } ;
    algo::BasicUnboundedRange < int* >::type a = algo::deduceRange ( &arr [ 0 ] ) ;
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( a ) ) ;
    
    algo::BasicUnboundedRange < int* >::type b = algo::deduceRange ( a ) ;
    TEST_ASSERT ( !algo::isEmpty ( b ) ) ;
    TEST_ASSERT ( b == a ) ;
    
    algo::BasicBoundedRange < int* >::type c = algo::deduceRange ( arr ) ;
    TEST_ASSERT ( !algo::isEmpty ( c ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( c ) ) ;
}

void testDeduceRange2 ()
{
    int arr [] = { 1 } ;
    algo::BasicBoundedRange < int* >::type a = algo::deduceRange ( arr, arr + 1 ) ;
    TEST_ASSERT ( !algo::isEmpty ( a ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( a ) ) ;
    
    algo::BasicCountedRange < int*, ptrdiff_t >::type b = algo::deduceRange ( arr, ptrdiff_t ( 1 ) ) ;
    TEST_ASSERT ( !algo::isEmpty ( b ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( b ) ) ;
    
    algo::BasicCountedRange < int*, ptrdiff_t >::type c = algo::deduceRange ( arr, short ( 1 ) ) ;
    TEST_ASSERT ( !algo::isEmpty ( c ) ) ;
    TEST_ASSERT ( 1 == algo::deref ( c ) ) ;
}

template < typename ReversedRange, typename ArrayStartIter >
void testIteration ( ReversedRange const x, ArrayStartIter y )
{
    ReversedRange d = x ;
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
    algo::successor ( d, algo::InPlace () ) ;
    TEST_ASSERT ( algo::deref ( d ) == 2 ) ;
    
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
    algo::successor ( d, algo::InPlace () ) ;
    TEST_ASSERT ( algo::deref ( d ) == 1 ) ;
    
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
    algo::successor ( d, algo::InPlace () ) ;
    TEST_ASSERT ( algo::deref ( d ) == 0 ) ;
    
    TEST_ASSERT ( algo::isEmpty ( d ) ) ;
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( d ) == y ) ;
    
    algo::predecessor ( d, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
    TEST_ASSERT ( algo::deref ( d ) == 1 ) ;
    
    algo::predecessor ( d, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
    TEST_ASSERT ( algo::deref ( d ) == 2 ) ;
    
    
    algo::predecessor ( d, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
    
    TEST_ASSERT ( d == x ) ;
    
    algo::advance ( d, 3, algo::InPlace () ) ;
    TEST_ASSERT ( algo::isEmpty ( d ) ) ;
    
    TEST_ASSERT ( -3 == algo::distance ( x, d ) ) ;
    TEST_ASSERT ( 3 == algo::distance ( d, x ) ) ;
    
    algo::advance ( d, -3, algo::InPlace () ) ;
    TEST_ASSERT ( !algo::isEmpty ( d ) ) ;
}

void testReverseRange ()
{
    typedef std::array < int, 3 > ArrayType ;
    ArrayType arr = { 0, 1, 2 } ;
    typedef ArrayType::iterator iter ;
    
    typedef algo::BasicBoundedRange < iter >::type iterRange ;
    iterRange a = algo::deduceRange ( arr.begin (), arr.end () ) ;
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( a ) == arr.begin () ) ;
    TEST_ASSERT ( algo::getValue < algo::EndIterator > ( a ) == arr.end () ) ;
    
    typedef typename algo::BasicReversedRange < iterRange >::type ReversedIterRange ;
    
    ReversedIterRange b = algo::reverseRange ( a ) ;
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( b ) == arr.end () ) ;
    TEST_ASSERT ( algo::getValue < algo::EndIterator > ( b ) == arr.begin () ) ;
    
    iterRange c = algo::reverseRange ( b ) ;
    
    TEST_ASSERT ( a == c ) ;
    
    // Now test reverse iteration
    testIteration ( b, arr.begin () ) ;
    
    
    // Now test reversed counted
    typedef algo::BasicCountedRange < iter >::type CountedIterRange ;
    
    CountedIterRange d = algo::deduceRange ( arr.begin (), 3 ) ;
    typedef typename algo::BasicReversedRange < CountedIterRange >::type ReversedCountedIterRange ;
    ReversedCountedIterRange e = algo::reverseRange ( d ) ;
    testIteration ( e, arr.begin () ) ;
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
    auto i = algo::find_if ( algo::deduceRange ( &arr [ 0 ] ), EqualToFour () ) ;
    TEST_ASSERT ( !algo::isEmpty ( i ) ) ;
    TEST_ASSERT ( 4 == algo::deref ( i ) ) ;
    
    
    auto i2 = algo::find_if ( algo::deduceRange ( arr, arr + 3 ), EqualToFour () ) ;
    TEST_ASSERT ( algo::isEmpty ( i2 ) ) ;
    TEST_ASSERT ( 3 == algo::deref ( i2 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    auto i3 = algo::find_if ( algo::deduceRange ( arr, 2 ), EqualToFour () ) ;
    TEST_ASSERT ( algo::isEmpty ( i3 ) ) ;
    TEST_ASSERT ( 2 == algo::deref ( i3 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    // Empty
    auto i4 = algo::find_if ( algo::deduceRange ( arr, 0 ), EqualToFour () ) ;
    TEST_ASSERT ( algo::isEmpty ( i4 ) ) ;
    TEST_ASSERT ( 0 == algo::deref ( i4 ) ) ; // Naughty ! But we know this iterator is actually valid.
}

void testFindIfNot ()
{
    int arr [] = { 0, 1, 2, 3, 4, 5 } ;
    auto i = algo::find_if_not ( algo::deduceRange ( &arr [ 0 ] ), LessThanFour () ) ;
    TEST_ASSERT ( !algo::isEmpty ( i ) ) ;
    TEST_ASSERT ( 4 == algo::deref ( i ) ) ;
    
    
    auto i2 = algo::find_if_not ( algo::deduceRange ( arr, arr + 3 ), LessThanFour () ) ;
    TEST_ASSERT ( algo::isEmpty ( i2 ) ) ;
    TEST_ASSERT ( 3 == algo::deref ( i2 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    auto i3 = algo::find_if_not ( algo::deduceRange ( arr, 2 ), LessThanFour () ) ;
    TEST_ASSERT ( algo::isEmpty ( i3 ) ) ;
    TEST_ASSERT ( 2 == algo::deref ( i3 ) ) ; // Naughty ! But we know this iterator is actually valid.
    
    
    // Empty
    auto i4 = algo::find_if_not ( algo::deduceRange ( arr, 0 ), LessThanFour () ) ;
    TEST_ASSERT ( algo::isEmpty ( i4 ) ) ;
    TEST_ASSERT ( 0 == algo::deref ( i4 ) ) ; // Naughty ! But we know this iterator is actually valid.
}

void testForEach ()
{
    int arr [] = { 0, 1, 2, 3, 4, 5 } ;
    
    Sum tmp = { 0 } ;
    Sum result = algo::for_each ( algo::deduceRange ( arr, 6 ), tmp ) ;
    
    TEST_ASSERT ( 15 == result.sum ) ;
    
    
    // Empty
    Sum result2 = algo::for_each ( algo::deduceRange ( arr, 0 ), tmp ) ;
    TEST_ASSERT ( 0 == result2.sum ) ;
}

void testTransform ()
{
    const ptrdiff_t ARR_LEN = 10 ;
    
    // Output and input range of same length
    int arr [ ARR_LEN ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
    int res [ ARR_LEN ] = {} ;
    
    auto i = algo::transform ( algo::deduceRange ( arr ), algo::deduceRange ( res ), AddOne () ) ;
    
    TEST_ASSERT ( algo::isEmpty ( i.first ) ) ;
    TEST_ASSERT ( algo::isEmpty ( i.second ) ) ;
    
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( i.first ) == &res [ 0 ] + ARR_LEN ) ;
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( i.second ) == &arr [ 0 ] + ARR_LEN ) ;
    
    for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
    {
        TEST_ASSERT ( ( arr [ i ] + 1 ) == res [ i ] ) ;
    }
    
    // Output range longer than input range
    int res2 [ ARR_LEN + 1 ] = {} ;
    
    auto i2 = algo::transform ( algo::deduceRange ( arr ), algo::deduceRange ( res2 ), AddOne () ) ;
    
    TEST_ASSERT ( !algo::isEmpty ( i2.first ) ) ;
    TEST_ASSERT ( algo::isEmpty ( i2.second ) ) ;
    
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( i2.first ) == &res2 [ 0 ] + ARR_LEN ) ;
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( i2.second ) == &arr [ 0 ] + ARR_LEN ) ;
    
    for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
    {
        TEST_ASSERT ( ( arr [ i ] + 1 ) == res2 [ i ] ) ;
    }
    
    // Output range shorter than input range
    int res3 [ ARR_LEN - 1 ] = {} ;
    
    auto i3 = algo::transform ( algo::deduceRange ( arr ), algo::deduceRange ( res3 ), AddOne () ) ;
    
    TEST_ASSERT ( algo::isEmpty ( i3.first ) ) ;
    TEST_ASSERT ( !algo::isEmpty ( i3.second ) ) ;
    
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( i3.first ) == &res3 [ 0 ] + ( ARR_LEN - 1 ) ) ;
    TEST_ASSERT ( algo::getValue < algo::StartIterator > ( i3.second ) == &arr [ 0 ] + ( ARR_LEN - 1 ) ) ;
    
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
    
        auto i = algo::zip ( algo::deduceRange ( arr ), algo::deduceRange ( arr ), algo::deduceRange ( res ), std::plus < int > () ) ;
    
        TEST_ASSERT ( algo::isEmpty ( i.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i.second.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i.second.second ) ) ;
    
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 2 * i == res [ i ] ) ;
        }
    }
    
    // Result array longer than inputs
    {
        int res [ ARR_LEN + 1 ] = {} ;
        
        auto i2 = algo::zip ( algo::deduceRange ( arr ), algo::deduceRange ( arr ), algo::deduceRange ( res ), std::plus < int > () ) ;
        
        
        TEST_ASSERT ( !algo::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( 10 == algo::distance ( algo::deduceRange ( res ), i2.first ) ) ;
        
        TEST_ASSERT ( algo::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i2.second.second ) ) ;
        
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 2 * i == res [ i ] ) ;
        }
        
        TEST_ASSERT ( 0 == res [ ARR_LEN ] ) ;
    }
    
    // Result array shorter than inputs
    {
        int res [ ARR_LEN - 1 ] = {} ;
        
        auto i2 = algo::zip ( algo::deduceRange ( arr ), algo::deduceRange ( arr ), algo::deduceRange ( res ), std::plus < int > () ) ;
        
        
        TEST_ASSERT ( algo::isEmpty ( i2.first ) ) ;
        
        TEST_ASSERT ( !algo::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( 9 == algo::deref ( i2.second.first ) ) ;
        
        TEST_ASSERT ( !algo::isEmpty ( i2.second.second ) ) ;
        TEST_ASSERT ( 9 == algo::deref ( i2.second.second ) ) ;
        
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
        auto i2 = algo::zip ( algo::deduceRange ( arr ), algo::deduceRange ( input2 ), algo::deduceRange ( res ), std::plus < int > () ) ;
        
        TEST_ASSERT ( algo::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i2.second.second ) ) ;
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 3 * i == res [ i ] ) ;
        }
        
        auto i3 = algo::zip ( algo::deduceRange ( input2 ), algo::deduceRange ( arr ), algo::deduceRange ( res2 ), std::plus < int > () ) ;
        
        TEST_ASSERT ( algo::isEmpty ( i3.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i3.second.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i3.second.second ) ) ;
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 3 * i == res2 [ i ] ) ;
        }
    }
    
    
    // input arrays differing in length (2nd shorter than the 1st)
    {
        int input2 [ ARR_LEN - 1 ] = { 0, 2, 4, 6, 8, 10, 12, 14, 16 } ;
        int res [ ARR_LEN ] = {} ;
        auto i2 = algo::zip ( algo::deduceRange ( arr ), algo::deduceRange ( input2 ), algo::deduceRange ( res ), std::plus < int > () ) ;
        
        TEST_ASSERT ( !algo::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( 9 == algo::distance ( algo::deduceRange ( res ), i2.first ) ) ;
        
        TEST_ASSERT ( !algo::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( 9 == algo::distance ( algo::deduceRange ( arr ), i2.second.first ) ) ;
        
        TEST_ASSERT ( algo::isEmpty ( i2.second.second ) ) ;
        
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN - 1 ; ++i )
        {
            TEST_ASSERT ( 3 * i == res [ i ] ) ;
        }
    }
    
    // input arrays differing in length (2nd longer than the 1st)
    {
        int input2 [ ARR_LEN + 1 ] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 } ;
        int res [ ARR_LEN ] = {} ;
        auto i2 = algo::zip ( algo::deduceRange ( arr ), algo::deduceRange ( input2 ), algo::deduceRange ( res ), std::plus < int > () ) ;
        
        TEST_ASSERT ( algo::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( !algo::isEmpty ( i2.second.second ) ) ;
        TEST_ASSERT ( 10 == algo::distance ( algo::deduceRange ( input2 ), i2.second.second ) ) ;
        
        for ( ptrdiff_t i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( 3 * i == res [ i ] ) ;
        }
    }
    
    {
        // Inputs and outputs are the same
        auto i2 = algo::zip ( algo::deduceRange ( arr ), algo::deduceRange ( arr ), algo::deduceRange ( arr ), std::plus < int > () ) ;
    
        TEST_ASSERT ( algo::isEmpty ( i2.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i2.second.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i2.second.second ) ) ;
    
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
        auto i = algo::unzip ( algo::deduceRange ( arr ), algo::deduceRange ( out1 ), algo::deduceRange ( out2 ), SplitInts () ) ;
        
        TEST_ASSERT ( algo::isEmpty ( i.first.first ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i.first.second ) ) ;
        TEST_ASSERT ( algo::isEmpty ( i.second ) ) ;
        
        for ( int i = 0 ; i < ARR_LEN ; ++i )
        {
            TEST_ASSERT ( i / 2 == out1 [ i ] ) ;
            TEST_ASSERT ( i % 2 == out2 [ i ] ) ;
        }
    }
}

template < typename ContainedType >
struct SetAndIncrement
{
    ContainedType val ;
    
    ALGO_INLINE
    void operator () ( ContainedType& x )
    {
        x = this->val ;
        this->val += 1 ;
    }
} ;

template < typename ContainedType >
struct SumValues
{
    ContainedType val ;
    
    ALGO_INLINE
    void operator () ( ContainedType x )
    {
        this->val += x ;
    }
} ;

void testStepPerformance ()
{
    const ptrdiff_t BUFFER_SIZE = 1024 * 1024 * 1024 ;
    typedef unsigned int ContainedType ;
    
    algo::AllocatingBuffer < algo::NewDeleteProtocol > buffer1 ( BUFFER_SIZE ) ;
    
    algo::AllocatingBuffer < algo::NewDeleteProtocol > buffer2 ( BUFFER_SIZE ) ;
    
    std::iota ( buffer1.template begin < ContainedType > (), buffer1.template end < ContainedType > (), 0 ) ;
    
    timer t ;
    
    t.start () ;
    std::copy ( buffer1.template begin < ContainedType > (), buffer1.template end < ContainedType > (), buffer2.template begin < ContainedType > () ) ;
    double const time = t.stop () ;
    
    ContainedType res = std::accumulate( buffer2.template begin < ContainedType > (), buffer2.template end < ContainedType > (), 0 ) ;
    
    std::cout << " testStepPerformance stepOverIter " << time << ' ' << res << '\n' ;

}

void testStepPerformance2 ()
{
    const ptrdiff_t BUFFER_SIZE = 1024 * 1024 * 1024 ;
    typedef unsigned int ContainedType ;
    
    algo::AllocatingBuffer < algo::NewDeleteProtocol > buffer1 ( BUFFER_SIZE ) ;
    
    algo::BasicBoundedRange < ContainedType* >::type buffer1Range = algo::deduceRange ( buffer1.template begin < ContainedType > (), buffer1.template end < ContainedType > () ) ;
    
    algo::AllocatingBuffer < algo::NewDeleteProtocol > buffer2 ( BUFFER_SIZE ) ;
    algo::BasicBoundedRange < ContainedType* >::type buffer2Range = algo::deduceRange ( buffer2.template begin < ContainedType > (), buffer2.template end < ContainedType > () ) ;
    
    std::iota ( buffer1.template begin < ContainedType > (), buffer1.template end < ContainedType > (), 0 ) ;
    
    timer t ;
    
    t.start () ;
    algo::stepOverDeduced ( buffer1Range, buffer2Range, algo::CopyForwardOperation::type () ) ;
    double const time2 = t.stop () ;
    
    ContainedType res = std::accumulate( buffer2.template begin < ContainedType > (), buffer2.template end < ContainedType > (), 0 ) ;
    
    std::cout << " testStepPerformance2  stepOverDeduced " << time2 << ' ' << res << '\n' ;
}

void testStepPerformance3 ()
{
    const ptrdiff_t BUFFER_SIZE = 1024 * 1024 * 1024 ;
    typedef unsigned int ContainedType ;
    
    algo::AllocatingBuffer < algo::NewDeleteProtocol > buffer1 ( BUFFER_SIZE ) ;
    
    algo::BasicBoundedRange < ContainedType* >::type buffer1Range = algo::deduceRange ( buffer1.template begin < ContainedType > (), buffer1.template end < ContainedType > () ) ;
    
    algo::AllocatingBuffer < algo::NewDeleteProtocol > buffer2 ( BUFFER_SIZE ) ;
    
    std::iota ( buffer1.template begin < ContainedType > (), buffer1.template end < ContainedType > (), 0 ) ;
    
    timer t ;
    
    t.start () ;
    algo::stepOverDeduced ( buffer1Range, algo::deduceRange ( buffer2.template begin < ContainedType > () ), algo::CopyForwardOperation::type () ) ;
    double const time2 = t.stop () ;
    
    ContainedType res = std::accumulate( buffer2.template begin < ContainedType > (), buffer2.template end < ContainedType > (), 0 ) ;
    
    std::cout << " testStepPerformance3 " << time2 << ' ' << res << '\n' ;
}

int main(int argc, const char * argv[] )
{
    // algo_basics.h
    test_swap ( false ) ;
    test_swap ( true ) ;
    
    test_select_if ( false ) ;
    test_select_if ( true ) ;
    
    // algo_property.h
    property::testProperty () ;
    property::testValueAndPropertyRelationalOperations () ;
    property::testCompoundRelationalOperations () ;
#ifdef ALGO_TEST_PERFORMANCE
    property::propertiesPerformanceTest () ;
#endif
    property::testVisitProperty () ;
    property::testConvertPropertySet () ;
    
    // algo_iterator.h
    testPredecessor () ;
    testSuccessor () ;
    testDistance () ;
    testAdvance () ;
    testIsEmpty () ;
    testDeref () ;
    testDerefMove () ;
    testAddressOf () ;
    testUnderlyingAddressOf () ;
    testEqualUnderlyingAddress () ;
    testDestroyPointed () ;
    testDestroyPointed2 () ;
    testDestroyReferenced () ;
    testDestroyReferenced2 () ;
    testAssign () ;
    testMoveAssign () ;
    testCopyConstruct () ;
    testMoveConstruct () ;
    testSwapped () ;
    testCopyBytesNotOverlapped () ;
    testCopyBytesOverlapped () ;
    testStripIter () ;
    testUnstripIter () ;
    
    // algo_range.h
    testBasicRanges () ;
    testDeduceRange1 () ;
    testDeduceRange2 () ;
    testReverseRange () ;
    testFindIf () ;
    testFindIfNot () ;
    testForEach () ;
    testTransform () ;
    testZip () ;
    testUnzip () ;
    
    // algo.h
    testStep () ;
    
    testForwards () ;
    testBackwards () ;
    
    testStepOver () ;
    testStepCounted () ;
    
    testCopy < int > () ;
    testCopyBackwards < int > () ;
    testFill < int, 1024 > () ;
    testFill < int, 1000 > () ;
    
    // These cases test for when the type is not bitwise copyable, so the step-by-step algorithm is chosen
    testCopy < NotABitwiseCopyableType > () ;
    testCopyBackwards < NotABitwiseCopyableType > () ;
    testFill < NotABitwiseCopyableType, 1024 > () ;
    testFill < NotABitwiseCopyableType, 1000 > () ;
    
    // These cases test for when the type is bitwise copyable through specialisation of IsBitwiseCopyable
    testCopy < IsABitwiseCopyableType > () ;
    testCopyBackwards < IsABitwiseCopyableType > () ;
    testFill < IsABitwiseCopyableType, 1024 > () ;
    testFill < IsABitwiseCopyableType, 1000 > () ;
    
    testRotateRightByOne < algo::RotateNoChoice > () ;
    testRotateRightByOne < algo::RotateBlocks > () ;
    testRotateLeftByOne < algo::RotateNoChoice > () ;
    testRotateLeftByOne < algo::RotateBlocks > () ;
    
    testMinIter < MinIterBounded > () ;
    testMinIter < MinIterCounted > () ;
    testMaxIter < MaxIterBounded > () ;
    testMaxIter < MaxIterCounted > () ;
    
#ifdef ALGO_TEST_PERFORMANCE
    testStepPerformance () ;
    testStepPerformance2 () ;
    testStepPerformance3 () ;
#endif
    
    // algo_buffer.h
    testBufferCalculation () ;
    testBufferCalculationUnaligned () ;
    
    testStackBuffer () ;
    
    testBufferAllocationProtocol < algo::MallocFreeProtocol > ( false ) ;
    testBufferAllocationProtocol < algo::CallocFreeProtocol > ( true ) ;
    testBufferAllocationProtocol < algo::NewDeleteProtocol > ( false ) ;
    testBufferAllocationProtocol < algo::ZeroedNewDeleteProtocol > ( true ) ;
    testBufferAllocationProtocol < algo::StlTemporaryBufferProtocol > ( false ) ;
    testBufferAllocationProtocol < algo::ZeroedStlTemporaryBufferProtocol > ( true ) ;
    
    testAllocatingBuffer () ;
    
    testObjectProctor () ;
    testTrivialObjectProctor () ;
    
    testIterProctorLengthOne () ;
    testIterProctorLengthTwo () ;
    testTrivialIterProctor () ;
    
    testBufferProctorLengthOne () ;
    testTrivialBufferProctor () ;
    
    // algo_sort.h
#ifdef ALGO_TEST_PERFORMANCE
    testCopyTimed < int > () ;
    testCopyBackwardsTimed < int > () ;
    
    testCopyTimed < NotABitwiseCopyableType > () ;
    testCopyBackwardsTimed < NotABitwiseCopyableType > () ;
    
    testCopyTimed < IsABitwiseCopyableType > () ;
    testCopyBackwardsTimed < IsABitwiseCopyableType > () ;
#endif
    
    testSortingCombinationsSortingTag < algo::UnstableExchange > () ;
    
    testSortingCombinationsSortingTag < algo::StableExchange > () ;
    
    testSortingCombinationsSortingTag < algo::UnstableExchangeIndices > () ;
    
    testSortingCombinationsSortingTag < algo::StableExchangeIndices > () ;
    
    std::cout << "BoundedInsertionSorter " ;
    
    testSorting < BoundedInsertionSorter > () ;
    
    std::cout << "CountedInsertionSorter " ;
    
    testSorting < CountedInsertionSorter > () ;
    
    std::cout << "BoundedInsertionUnguardedSorter " ;
    
    testSorting < BoundedInsertionUnguardedSorter > () ;
    
    std::cout << "CountededInsertionUnguardedSorter " ;
    
    testSorting < CountededInsertionUnguardedSorter > () ;
    
    std::cout << "BoundedUnstableInsertionUnguardedSorter " ;
    
    testSorting < BoundedUnstableInsertionUnguardedSorter > () ;
    
    std::cout << "CountededUnstableInsertionUnguardedSorter " ;
    
    testSorting < CountededUnstableInsertionUnguardedSorter > () ;
    
#ifdef ALGO_TEST_PERFORMANCE
    std::cout << "std::sort " ;
    
    testSorting < StdSorter > () ;
    
    std::cout << "std::stable_sort " ;
    
    testSorting < StableStdSorter > () ;
#endif

    return 0;
}

