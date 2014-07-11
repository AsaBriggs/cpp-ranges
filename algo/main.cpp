//
//  main.cpp
//  algo
//
//  Created by Asa Briggs on 01/07/2014.
//  Copyright (c) 2014 Asa Briggs. All rights reserved.
//

#include "algo.h"
#include "algo_sort.h"
#include "algo_property.h"
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
static_assert ( std::is_same < algo::FindWrapper< algo::Param < Tag < 10 >, Value < 10 > >, TestParams >::type, Value < 10 > >::value , "unexpected" ) ;
static_assert ( std::is_same < algo::FindWrapper< algo::Param < Tag < 10 >, Value < 10 > >, algo::Parameters <> >::type, Value < 10 > >::value , "unexpected" ) ;

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

static_assert ( std::is_same < TestDeducedParams::Param0, Value < 0 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param1, Value < 1 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param2, Value < 2 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param3, Value < 3 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param4, Value < 4 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param5, Value < 5 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param6, Value < 6 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param7, Value < 7 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param8, Value < 8 > >::value , "unexpected" ) ;
static_assert ( std::is_same < TestDeducedParams::Param9, Value < 9 > >::value , "unexpected" ) ;


void testPredecessor ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    TEST_ASSERT ( algo::predecessor(next) == begin ) ;
    
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
    
    TEST_ASSERT ( algo::successor(begin) == next ) ;
    
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
    
    TEST_ASSERT ( end == algo::advance ( begin, 2 ) ) ;
    TEST_ASSERT ( begin == algo::advance ( end, -2 ) ) ;
    
    int* tmp = begin ;
    algo::advance(tmp, 2, algo::InPlace () ) ;
    TEST_ASSERT ( end == tmp ) ;
    
    int* tmp2 = end ;
    algo::advance(tmp2, -2, algo::InPlace () ) ;
    TEST_ASSERT ( begin == tmp2 ) ;
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
    
    algo::copyBytesNotOverlapped ( arr2.begin (), arr.begin (), arr.size () ) ;
    TEST_ASSERT ( arr == arr2 ) ;
    
    std::array < int, 5 > arr3 = {} ;
    std::array < int, 5 > const arr4 = {} ;
    algo::copyBytesNotOverlapped ( arr3.begin (), arr.begin (), 0 ) ;
    TEST_ASSERT ( arr4 == arr3 ) ;
}

void testCopyBytesOverlapped ()
{
    std::array < int, 5 > const arr = { 1, 2, 3, 4, 5 } ;
    std::array < int, 5 > arr2 ;
    
    algo::copyBytesOverlapped ( arr2.begin (), arr.begin (), arr.size () ) ;
    TEST_ASSERT ( arr == arr2 ) ;
    
    // Test overlapping input/output
    algo::copyBytesOverlapped ( arr2.begin () + 1, arr2.begin (), arr.size () - 1 ) ;
    
    std::array < int, 5 > const expected = { 1, 1, 2, 3, 4 } ;
    TEST_ASSERT ( expected == arr2 ) ;
    
    algo::copyBytesOverlapped ( arr2.begin (), arr2.begin () + 2, arr.size () - 2 ) ;
    
    std::array < int, 5 > const expected2 = { 2, 3, 4, 3, 4 } ;
    TEST_ASSERT ( expected2 == arr2 ) ;
    
    std::array < int, 5 > arr3 = {} ;
    std::array < int, 5 > const arr4 = {} ;
    algo::copyBytesOverlapped ( arr3.begin (), arr.begin (), 0 ) ;
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

void assertADTypeValue ( ADType& x, ADType* address, int index )
{
    TEST_ASSERT ( &x == address ) ;
    
    switch ( index )
    {
        case 0:
            TEST_ASSERT ( ad0 == x ) ;
            x = ad1 ;
            break ;
        case 1:
            TEST_ASSERT ( ad1 == x ) ;
            x = ad2 ;
            break ;
        case 2:
            TEST_ASSERT ( ad2 == x ) ;
            x = ad3;
            break ;
        default:
            TEST_ASSERT ( false ) ;
            break ;
    }
}

void assertADTypeValue ( algo::EmptyAuxilliaryData& x, algo::EmptyAuxilliaryData*& address, int index )
{
    switch ( index )
    {
        case 0:
            TEST_ASSERT ( 0 == address ) ;
            address = &x ;
            break ;
        case 1:
            TEST_ASSERT ( &x == address ) ;
            break ;
        case 2:
            TEST_ASSERT ( &x == address ) ;
            break ;
        default:
            TEST_ASSERT ( false ) ;
            break ;
    }
}

template < class AuxilliaryDataType >
struct TestStep
{
    mutable bool pre_op_ad_tag_called ;
    mutable int post_op_i_o_called ;
    mutable AuxilliaryDataType* adAddress ;
    IOType* iAddress ;
    IOType* oAddress ;
    
    TestStep ( AuxilliaryDataType* adAddress, IOType* iAddress, IOType* oAddress )
        : pre_op_ad_tag_called ( false )
        , post_op_i_o_called ( 0 )
        , adAddress ( adAddress )
        , iAddress ( iAddress )
        , oAddress ( oAddress )
    {}
    
    void operator () ( algo::pre_op_ad_tag, AuxilliaryDataType& ad) const
    {
        TEST_ASSERT ( !pre_op_ad_tag_called ) ;
        pre_op_ad_tag_called = true ;
        assertADTypeValue ( ad, adAddress, 0 ) ;
    }
    
    void operator () ( algo::pre_op_i_tag, IOType& i) const
    {
        TEST_ASSERT ( pre_op_ad_tag_called ) ;
        TEST_ASSERT ( &i == iAddress ) ;
        TEST_ASSERT ( io0 == i ) ;
        i = io1 ;
    }
    
    void operator () ( algo::pre_op_o_tag, IOType& o) const
    {
        TEST_ASSERT ( pre_op_ad_tag_called ) ;
        TEST_ASSERT ( &o == oAddress ) ;
        TEST_ASSERT ( io0 == o ) ;
        o = io1 ;
    }
    
    void operator () ( algo::Operation_tag, IOType& i, IOType& o, AuxilliaryDataType& ad) const
    {
        TEST_ASSERT ( &i == iAddress ) ;
        TEST_ASSERT ( io1 == i ) ;
        
        TEST_ASSERT ( &o == oAddress ) ;
        TEST_ASSERT ( io1 == o ) ;
        
        i = io2 ;
        o = io2 ;
        
        assertADTypeValue ( ad, adAddress, 1 ) ;
    }
    
    void operator () ( algo::post_op_i_tag, IOType& i) const
    {
        ++post_op_i_o_called ;
        TEST_ASSERT ( &i == iAddress ) ;
        TEST_ASSERT ( io2 == i ) ;
        i = io3 ;
    }
    
    void operator () ( algo::post_op_o_tag, IOType& o) const
    {
        ++post_op_i_o_called ;
        TEST_ASSERT ( &o == oAddress ) ;
        TEST_ASSERT ( io2 == o ) ;
        o = io3 ;
    }
    
    void operator () ( algo::post_op_ad_tag, AuxilliaryDataType& ad) const
    {
        TEST_ASSERT ( 2 == post_op_i_o_called ) ;
        assertADTypeValue ( ad, adAddress, 2 ) ;
    }
};

void testStep ()
{
    IOType i = io0 ;
    IOType o = io0 ;
    
    algo::step ( i, o, TestStep < algo::EmptyAuxilliaryData > ( 0, &i, &o ) ) ;
    
    TEST_ASSERT ( io3 == i );
    TEST_ASSERT ( io3 == o ) ;
}

void testStepWithAuxilliaryData ()
{
    ADType ad = ad0 ;
    IOType i = io0 ;
    IOType o = io0 ;
    
    algo::stepWithAuxilliaryData ( i, o, ad, TestStep < ADType > ( &ad, &i, &o ) ) ;
    
    TEST_ASSERT ( io3 == i );
    TEST_ASSERT ( io3 == o ) ;
    TEST_ASSERT ( ad3 == ad ) ;
}

struct ATag {} ;

void testForwards ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    int* tmp = begin ;
    
    algo::Forwards < ATag > () ( ATag (), tmp ) ;
    TEST_ASSERT ( next == tmp ) ;
}

void testBackwards ()
{
    static const int ARR_LENGTH = 1 ;
    int arr [ARR_LENGTH] = { 1 } ;
    int* const begin = arr ;
    int* const next = begin + 1 ;
    
    int* tmp = next ;
    
    algo::Backwards < ATag > () ( ATag (), tmp ) ;
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

static_assert ( !algo::IsBitwiseCopyable < NotABitwiseCopyableType >::value, "Needs to be non-bitwise copyable for the tests" );


namespace algo
{
    template <>
    struct IsBitwiseCopyable < IsABitwiseCopyableType > : std::true_type
    {} ;
}

static_assert ( algo::IsBitwiseCopyable < IsABitwiseCopyableType >::value, "Needs to be non-bitwise copyable for the tests" );

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

static_assert ( 256 == algo::AlignmentOf < AlignOn256ByteBoundary >::value, "Not 256 byte aligned" ) ;

void testBufferCalculation ()
{
    // Trivial
    static const ptrdiff_t ARR_LENGTH = 1024 ;
    char arr [ ARR_LENGTH ] ;
    
    {
        char* const  begin = algo::BufferCalculation::calculateBegin < char > ( { arr, 1 } ) ;
        TEST_ASSERT ( begin == arr ) ;
        
        char* const end = algo::BufferCalculation::calculateEnd < char > ( begin, { arr, 1 } ) ;
        
        TEST_ASSERT ( arr + 1 == end ) ;
        TEST_ASSERT ( 1 == ( end - begin ) ) ;
    }
    
    {
        char* const  begin = algo::BufferCalculation::calculateBegin < char > ( { arr, ARR_LENGTH } ) ;
        TEST_ASSERT ( begin == arr ) ;

        char* const end = algo::BufferCalculation::calculateEnd < char > ( begin, { arr, ARR_LENGTH } ) ;

        TEST_ASSERT ( arr + ARR_LENGTH == end ) ;
        TEST_ASSERT ( ARR_LENGTH == ( end - begin ) ) ;
    }
    
    {
        // Now for something slightly more difficult
        algo::BufferRange <double> const range = algo::BufferCalculation::calculateRange < double > ( { arr, ARR_LENGTH } ) ;
        TEST_ASSERT ( reinterpret_cast < void* > ( range.begin ) == reinterpret_cast < void* > ( arr ) ) ;
    
        static_assert ( 0 == ( ARR_LENGTH % sizeof ( double ) ), "Test setup failure; array must be exactly divisible by sizeof ( double )" );
        TEST_ASSERT ( reinterpret_cast < void* > ( range.end ) == reinterpret_cast < void* > ( arr + ARR_LENGTH ) ) ;
        TEST_ASSERT ( ARR_LENGTH / sizeof ( double ) == ( range.end - range.begin ) ) ;
    }
}

void testBufferCalculationUnaligned ()
{
    const ptrdiff_t ARR_LENGTH = 1024 ;
    char arr [ ARR_LENGTH ] ;
    
    char* const bufferStart = &arr[1] ; // Deliberately pick an unaligned start address
    
    AlignOn256ByteBoundary* const begin = algo::BufferCalculation::calculateBegin < AlignOn256ByteBoundary > ( { bufferStart, ARR_LENGTH } ) ;
    TEST_ASSERT ( begin > reinterpret_cast < AlignOn256ByteBoundary* > ( bufferStart ) ) ;
    
    AlignOn256ByteBoundary* const nullBegin = algo::BufferCalculation::calculateBegin < AlignOn256ByteBoundary > ( { bufferStart, 1 } ) ;
    TEST_ASSERT ( ALGO_NULLPTR == nullBegin ) ;
    
    // Rather trick the calculateEnd function by passing in a null begin
    AlignOn256ByteBoundary* const nullEnd = algo::BufferCalculation::calculateEnd < AlignOn256ByteBoundary > ( nullBegin, { bufferStart, ARR_LENGTH } ) ;
    
    TEST_ASSERT ( ALGO_NULLPTR == nullEnd ) ;
    
    AlignOn256ByteBoundary* const end2 = algo::BufferCalculation::calculateEnd < AlignOn256ByteBoundary > ( begin, { bufferStart, ARR_LENGTH - 1 } ) ;
    
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
    bool operator()(const T x, const T y) const noexcept(noexcept(x < y))
    {return x < y;}
};

struct StdSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        std::sort(f, l, myLess< typename std::iterator_traits < Iter >::value_type >());
    }
};

struct StableStdSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        std::stable_sort(f, l, myLess< typename std::iterator_traits < Iter >::value_type >());
    }
};

template < typename Tag, typename SwapIfKind, typename IndexType >
struct NetworkSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort(f, myLess< typename std::iterator_traits < Iter >::value_type >(), Tag (), algo::Int<N>(), SwapIfKind (), IndexType () );
    }
};

struct BoundedInsertionSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion(f, l, myLess< typename std::iterator_traits < Iter >::value_type >() );
    }
} ;

struct CountedInsertionSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion(f, N, myLess< typename std::iterator_traits < Iter >::value_type >() );
    }
} ;

struct BoundedInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel(f, l, myLess< typename std::iterator_traits < Iter >::value_type >() );
    }
} ;

struct CountededInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel(f, N, myLess< typename std::iterator_traits < Iter >::value_type >() );
    }
} ;

struct BoundedUnstableInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel_unstable(f, l, myLess< typename std::iterator_traits < Iter >::value_type >() );
    }
} ;

struct CountededUnstableInsertionUnguardedSorter
{
    template < int N, class Iter >
    void sort ( Iter f, Iter l ) const
    {
        algo::sort_insertion_sentinel_unstable(f, N, myLess< typename std::iterator_traits < Iter >::value_type >() );
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
{}

void testStepCounted ()
{}

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
    
    PropertySet updated = algo::setValue < PropertyName > ( set, value ) ;
    PropertySet updated2 = algo::setValue < PropertyName > ( constSet, value ) ;
    
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
    static_assert ( algo::HasProperty<Tag1, V1>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag1, V2>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag1, V3>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag1, V4>::type (), "unexpected" ) ;
    
    static_assert ( !algo::HasProperty<Tag2, V1>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag2, V2>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag2, V3>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag2, V4>::type (), "unexpected" ) ;
    
    static_assert ( !algo::HasProperty<Tag3, V1>::type (), "unexpected" ) ;
    static_assert ( !algo::HasProperty<Tag3, V2>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag3, V3>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag3, V4>::type (), "unexpected" ) ;
    
    static_assert ( !algo::HasProperty<Tag4, V1>::type (), "unexpected" ) ;
    static_assert ( !algo::HasProperty<Tag4, V2>::type (), "unexpected" ) ;
    static_assert ( !algo::HasProperty<Tag4, V3>::type (), "unexpected" ) ;
    static_assert ( algo::HasProperty<Tag4, V4>::type (), "unexpected" ) ;
    
    
    // test ValueType
    static_assert ( std::is_same < algo::ValueType < Tag1, V1 >::type, Value1 >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag1, V2 >::type, Value1 >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag1, V3 >::type, Value1 >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag1, V4 >::type, Value1 >::type (), "unexpected" ) ;
    
    static_assert ( std::is_same < algo::ValueType < Tag2, V2 >::type, Value2 >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag2, V3 >::type, Value2 >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag2, V4 >::type, Value2 >::type (), "unexpected" ) ;
    
    static_assert ( std::is_same < algo::ValueType < Tag3, V3 >::type, Value3 >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag3, V4 >::type, Value3 >::type (), "unexpected" ) ;
    
    static_assert ( std::is_same < algo::ValueType < Tag4, V4 >::type, Value4 >::type (), "unexpected" ) ;
    
    // Test the const version too
    static_assert ( std::is_same < algo::ValueType < Tag1, V1 const >::type, Value1 const >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag1, V2 const >::type, Value1 const >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag1, V3 const >::type, Value1 const >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag1, V4 const >::type, Value1 const >::type (), "unexpected" ) ;
    
    static_assert ( std::is_same < algo::ValueType < Tag2, V2 const >::type, Value2 const >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag2, V3 const >::type, Value2 const >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag2, V4 const >::type, Value2 const >::type (), "unexpected" ) ;
    
    static_assert ( std::is_same < algo::ValueType < Tag3, V3 const >::type, Value3 const >::type (), "unexpected" ) ;
    static_assert ( std::is_same < algo::ValueType < Tag3, V4 const >::type, Value3 const >::type (), "unexpected" ) ;
    
    static_assert ( std::is_same < algo::ValueType < Tag4, V4 const >::type, Value4 const >::type (), "unexpected" ) ;
    
    
    
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
    
        V2 b2 = { updatedValue2, a2 } ;
        V2 b3 = { updatedValue2, updatedValue1 } ;
        TEST_ASSERT ( b2 == b ) ;
        TEST_ASSERT ( b3 == b ) ;
    
        V3 c2 = { updatedValue3, b2 } ;
        V3 c3 = { updatedValue3, updatedValue2, a2 } ;
        V3 c4 = { updatedValue3, updatedValue2, updatedValue1 } ;
        TEST_ASSERT ( c2 == c ) ;
        TEST_ASSERT ( c3 == c ) ;
        TEST_ASSERT ( c4 == c ) ;
    
        V4 d2 = { updatedValue4, c2 } ;
        V4 d3 = { updatedValue4, updatedValue3, b2 } ;
        V4 d4 = { updatedValue4, updatedValue3, updatedValue2, a2 } ;
        V4 d5 = { updatedValue4, updatedValue3, updatedValue2, updatedValue1 } ;
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
        V4 tmp = { value4, updatedValue3, updatedValue2, updatedValue1 } ;
        V4 tmp2 = algo::addOrUpdateValue < Tag4 > ( tmp, updatedValue4 ) ;
        
        TEST_ASSERT ( value4 == algo::getValueByReference < Tag4 > ( tmp ) ) ;
        TEST_ASSERT ( updatedValue4 == algo::getValueByReference < Tag4 > ( tmp2 ) ) ;
    }
}

int main(int argc, const char * argv[] )
{
    // algo_basics.h
    test_swap ( false ) ;
    test_swap ( true ) ;
    
    test_select_if ( false ) ;
    test_select_if ( true ) ;
    
    // algo_property.h
    testProperty () ;
    testValueAndPropertyRelationalOperations () ;
    testCompoundRelationalOperations () ;
    
    // algo_iterator.h
    testPredecessor () ;
    testSuccessor () ;
    testDistance () ;
    testAdvance () ;
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
    
    // algo.h
    testStep () ;
    testStepWithAuxilliaryData () ;
    
    testForwards () ;
    testBackwards () ;
    
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
    
    //TOD implement tests for these
    testStepOver () ;
    testStepCounted () ;
    
    return 0;
}

