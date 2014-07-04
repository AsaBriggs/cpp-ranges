#ifndef INCLUDED_ALGO_H
#define INCLUDED_ALGO_H

#include <type_traits>
#include <iterator>
#include <utility>
#include <memory>
#include <cstdlib>
#include <cassert>

#ifndef ALGO_INLINE
#define ALGO_INLINE __attribute__((always_inline)) inline
#endif

#ifndef ALGO_ASSERT
#define ALGO_ASSERT(x) assert ( ( x ) )
#endif

#ifndef ALGO_SAFE_MODE
#define ALGO_SAFE_MODE(x) x
#endif

#ifndef ALGO_LIKELIHOOD
#define ALGO_LIKELIHOOD(test, likelihood) __builtin_expect ( !!( test ), ( likelihood ) )
#endif

#define ALGO_CALL ::algo
#define ALGO_ENABLE_IF_PARAM_DEFAULT ALGO_CALL::EnableIfDeductionType
#define ALGO_COMMA_ENABLE_IF_PARAM , typename enable = ALGO_ENABLE_IF_PARAM_DEFAULT
#define ALGO_NULLPTR nullptr

namespace algo
{
struct EnableIfDeductionType {} ;
    
    
    
struct InPlace {} ;
    
    
    
struct ByReturnValue {} ;
    
    
    
template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
struct IsBitwiseCopyable : std::is_trivially_copy_assignable < T >
{} ;
    
template < typename T, size_t N >
struct IsBitwiseCopyable < T [ N ], ALGO_ENABLE_IF_PARAM_DEFAULT > : ALGO_CALL::IsBitwiseCopyable < T >
{} ;
    
template < typename T >
struct IsBitwiseCopyable < T&, ALGO_ENABLE_IF_PARAM_DEFAULT > : std::false_type
{} ;
    
template < typename T, typename U >
struct IsBitwiseCopyable < std::pair < T, U >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    : std::integral_constant < bool, ALGO_CALL::IsBitwiseCopyable < T >::value && ALGO_CALL::IsBitwiseCopyable < U >::value >
{} ;
    
    
    
template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
struct IsBitwiseMoveable : ALGO_CALL::IsBitwiseCopyable < T >
{} ;
    
// shared pointer, managed pointer, containers that have no back-pointers from remote parts.

    
template < typename Iter >
struct IsNotProxiedIterator
    : std::is_same< typename std::remove_cv < typename std::iterator_traits < Iter >::reference >::type,
                    typename std::remove_cv < typename std::iterator_traits < Iter >::value_type >::type& >
{} ;


    
template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
struct IsTriviallyDestructible : std::is_trivially_destructible < T >
{} ;

    
    
template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
struct SizeOf : std::integral_constant < size_t, sizeof ( T ) >
{} ;
    
    
    
// Must be a power of 2, must be <= Sizeof ( T )
template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
struct AlignmentOf : std::alignment_of < T >
{} ;

    
template < size_t X >
struct PowerOfTwo ;

// 2 ^ ( - infinity ) ?
template <>
struct PowerOfTwo < 0u > : std::false_type
{};

template <>
struct PowerOfTwo < 1u > : std::true_type
{};
    
template <>
struct PowerOfTwo < 2u > : std::true_type
{};
    
template < size_t X >
struct PowerOfTwo : std::integral_constant < bool, ( 0u == X % 2 ) && PowerOfTwo < X / 2u >::value >
{};

    
    
template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
struct GetAlignmentOf : ALGO_CALL::AlignmentOf < T >
{
    typedef ALGO_CALL::AlignmentOf < T > ParentType ;
    
    static_assert ( ALGO_CALL::PowerOfTwo < ParentType::value >::value, "Alignment must be a power of 2" ) ;
    static_assert ( ALGO_CALL::SizeOf < T >::value >= ParentType::value, "" ) ;
    static_assert ( ParentType::value < size_t ( std::numeric_limits < ptrdiff_t >::max () ), "Can not have an alignment greater than ptrdiff_t max value" ) ;
} ;
    
    
    
template < typename BidirectionalIterator ALGO_COMMA_ENABLE_IF_PARAM >
struct Predecessor
{
    ALGO_INLINE
    BidirectionalIterator operator () ( BidirectionalIterator x, ALGO_CALL::ByReturnValue ) const
    {
        return --x ;
    }
    
    ALGO_INLINE
    void operator () ( BidirectionalIterator& x, ALGO_CALL::InPlace ) const
    {
        --x ;
    }
} ;

template  < typename BidirectionalIterator >
ALGO_INLINE
BidirectionalIterator predecessor ( BidirectionalIterator x, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
{
    return ALGO_CALL::Predecessor < BidirectionalIterator > () ( x, ALGO_CALL::ByReturnValue () ) ;
}

template < typename BidirectionalIterator >
ALGO_INLINE
void predecessor ( BidirectionalIterator& x, ALGO_CALL::InPlace )
{
    ALGO_CALL::Predecessor < BidirectionalIterator > () ( x, ALGO_CALL::InPlace () ) ;
}



template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
struct Successor
{
    ALGO_INLINE
    ForwardIterator operator () ( ForwardIterator x, ALGO_CALL::ByReturnValue ) const
    {
        return ++x ;
    }
    
    ALGO_INLINE
    void operator () ( ForwardIterator& x, ALGO_CALL::InPlace ) const
    {
        ++x ;
    }
} ;

template < typename ForwardIterator >
ALGO_INLINE
ForwardIterator successor ( ForwardIterator x, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
{
    return ALGO_CALL::Successor < ForwardIterator > () ( x, ALGO_CALL::ByReturnValue () ) ;
}

template < typename ForwardIterator >
ALGO_INLINE
void successor ( ForwardIterator& x, ALGO_CALL::InPlace )
{
    ALGO_CALL::Successor < ForwardIterator > () ( x, ALGO_CALL::InPlace () ) ;
}



template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
struct Distance
{
    ALGO_INLINE
    typename std::iterator_traits < ForwardIterator >::difference_type operator () ( ForwardIterator x, ForwardIterator y ) const
    {
        return std::distance ( x, y ) ;
    }
} ;

template < typename ForwardIterator >
ALGO_INLINE
typename std::iterator_traits < ForwardIterator >::difference_type distance ( ForwardIterator x, ForwardIterator y )
{
    return ALGO_CALL::Distance < ForwardIterator > () ( x, y ) ;
}



template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
struct Advance
{
    ALGO_INLINE
    ForwardIterator operator () ( ForwardIterator x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::ByReturnValue ) const
    {
        std::advance ( x, n ) ;
        return x ;
    }
    
    ALGO_INLINE
    void operator () ( ForwardIterator& x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::InPlace ) const
    {
        std::advance ( x, n ) ;
    }
} ;

template < typename ForwardIterator >
ALGO_INLINE
ForwardIterator advance ( ForwardIterator x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
{
    return ALGO_CALL::Advance < ForwardIterator > () ( x, n, ByReturnValue () ) ;
}

template < typename ForwardIterator >
ALGO_INLINE
void advance ( ForwardIterator& x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::InPlace )
{
    ALGO_CALL::Advance < ForwardIterator > () ( x, n, ALGO_CALL::InPlace () ) ;
}



template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct Deref
{
    ALGO_INLINE
    typename std::iterator_traits < Iter >::reference operator () ( Iter x ) const
    {
        return *x ;
    }
} ;

template < typename Iter >
ALGO_INLINE
typename std::iterator_traits < Iter >::reference deref ( Iter x )
{
    return ALGO_CALL::Deref < Iter > () ( x ) ;
}

    

template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct DerefMove
{
    ALGO_INLINE
    typename std::iterator_traits < Iter >::value_type&& operator () ( Iter x ) const
    {
        return std::move ( *x ) ;
    }
} ;

template < typename Iter >
ALGO_INLINE
typename std::iterator_traits < Iter >::value_type&& derefMove ( Iter x )
{
    return std::forward < typename std::iterator_traits < Iter >::value_type > ( ALGO_CALL::DerefMove < Iter > () ( x ) ) ;
}
    
    
    
template < typename X ALGO_COMMA_ENABLE_IF_PARAM >
struct AddressOf
{
    ALGO_INLINE
    X* operator () ( X& x ) const
    {
        return &x ;
    }
} ;

template < typename X >
ALGO_INLINE
X* addressOf ( X& x )
{
    return ALGO_CALL::AddressOf < X > () ( x ) ;
}
    
    
    
template < typename Iter1, typename Iter2 ALGO_COMMA_ENABLE_IF_PARAM >
struct EqualUnderlyingAddress
{
    ALGO_INLINE
    bool operator () ( Iter1 x, Iter2 y ) const
    {
        return reinterpret_cast < const volatile void* > ( ALGO_CALL::addressOf ( ALGO_CALL::deref ( x ) ) )
                == reinterpret_cast < const volatile void* > ( ALGO_CALL::addressOf ( ALGO_CALL::deref ( y ) ) ) ;
    }
} ;

template < typename Iter1, typename Iter2 >
ALGO_INLINE
bool equalUnderlyingAddress ( Iter1 x, Iter2 y )
{
    // Requires x & y are dereferencable
    return ALGO_CALL::EqualUnderlyingAddress < Iter1, Iter2 > () ( x, y ) ;
}
    
    
    
template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
struct DestroyReferenced
{
    ALGO_INLINE
    void operator () ( T& x ) const
    {
        x.~T () ;
    }
} ;

template < typename T >
struct DestroyReferenced < T, typename std::enable_if < ALGO_CALL::IsTriviallyDestructible < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    void operator () ( T& x ) const
    {}
} ;

template < typename T >
ALGO_INLINE
void destroyReferenced ( T& x )
{
    return ALGO_CALL::DestroyReferenced < T > () ( x ) ;
}

    

template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct DestroyPointed ;

template < typename Iter >
struct DestroyPointed < Iter,
    typename std::enable_if < ALGO_CALL::IsNotProxiedIterator < Iter >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () ( Iter x ) const
    {
        destroyReferenced ( ALGO_CALL::deref ( x ) ) ;
    }
} ;

template < typename Iter >
ALGO_INLINE
void destroyPointed ( Iter x )
{
    return ALGO_CALL::DestroyPointed < Iter > () ( x ) ;
}


    
template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct AssignImpl
{
    template < typename T >
    ALGO_INLINE
    void operator () ( Iter x, T&& y ) const
    {
        ALGO_CALL::deref ( x ) = std::forward < T > ( y ) ;
    }
} ;
    
template < typename Iter, typename T >
ALGO_INLINE
void assignImpl ( Iter x, T&& y )
{
    ALGO_CALL::AssignImpl < Iter > () ( x, std::forward < T > ( y ) ) ;
}

template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct Assign
{
    ALGO_INLINE
    void operator () ( I i, O o ) const
    {
        ALGO_CALL::assignImpl ( o, ALGO_CALL::deref ( i ) ) ;
    }
} ;

template < typename I, typename O >
ALGO_INLINE
void assign ( I i, O o )
{
    ALGO_CALL::Assign < I, O > () ( i , o ) ;
}



template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct MoveAssign
{
    ALGO_INLINE
    void operator () ( I i, O o ) const
    {
        // Disable if i is a proxy?
        ALGO_CALL::assignImpl ( o, std::forward < typename std::iterator_traits< I >::value_type > ( ALGO_CALL::derefMove ( i ) ) ) ;
    }
} ;

template < typename I, typename O >
ALGO_INLINE
void moveAssign ( I i, O o )
{
    ALGO_CALL::MoveAssign < I, O > () ( i , o ) ;
}

    
    
template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct ConstructImpl
{
    template < typename T >
    ALGO_INLINE
    void operator () ( Iter x, T&& y ) const
    {
        typedef typename std::iterator_traits < Iter >::value_type IterValue ;
        
        new ( ALGO_CALL::addressOf ( ALGO_CALL::deref ( x ) ) ) IterValue ( std::forward < T > ( y ) );
    }
} ;

template < typename Iter, typename T >
ALGO_INLINE
void constructImpl ( Iter x, T&& y )
{
    ALGO_CALL::ConstructImpl < Iter > () ( x, std::forward < T > ( y ) ) ;
}

    
    
template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct CopyConstruct
{
    // Default for proxied iterators is to assign
    ALGO_INLINE
    void operator () ( I i, O o ) const
    {
        ALGO_CALL::assign ( i, o ) ;
    }
} ;
    
template <typename I, typename O >
struct CopyConstruct < I, O,
    typename std::enable_if < ALGO_CALL::IsNotProxiedIterator < I >::value && ALGO_CALL::IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () ( I i, O o ) const
    {
        ALGO_CALL::constructImpl ( o, ALGO_CALL::deref ( i ) ) ;
    }
} ;
    
template < typename I, typename O >
ALGO_INLINE
void copyConstruct ( I i, O o )
{
    ALGO_CALL::CopyConstruct < I, O > () ( i , o ) ;
}

    
    
template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct MoveConstruct 
{
    // Default for proxied iterators is to assign
    ALGO_INLINE
    void operator () ( I i, O o ) const
    {
        ALGO_CALL::assign ( i, o ) ;
    }
} ;

template < typename I, typename O >
struct MoveConstruct < I, O,
    typename std::enable_if< IsNotProxiedIterator < I >::value && IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () ( I i, O o ) const
    {
        ALGO_CALL::constructImpl ( o, std::forward < typename std::iterator_traits< I >::value_type > ( ALGO_CALL::derefMove ( i ) ) ) ;
    }
} ;

template < typename I, typename O >
ALGO_INLINE
void moveConstruct ( I i, O o )
{
    ALGO_CALL::MoveConstruct < I, O > () ( i , o ) ;
}
    
    
template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct IterSwap
{
    ALGO_INLINE
    void operator () ( I i, O o ) const
    {
        std::iter_swap ( i, o ) ;
    }
} ;

template < typename I, typename O >
ALGO_INLINE
void iterSwap ( I i, O o )
{
    ALGO_CALL::IterSwap < I, O > () ( i , o ) ;
}

    
    
template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct StripIter
{
    typedef Iter type ;
    
    ALGO_INLINE
    type operator () ( Iter x ) const
    {
        return x ;
    }
} ;
    
template < typename Iter >
ALGO_INLINE
typename ALGO_CALL::StripIter < Iter >::type stripIter ( Iter x )
{
    return ALGO_CALL::StripIter < Iter > () ( x ) ;
}

    
    
template < typename ReturnIter, typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct UnstripIter
{
    ALGO_INLINE
    ReturnIter operator () ( Iter x ) const
    {
        return x ;
    }
} ;
    
template < typename ReturnIter, typename Iter >
ALGO_INLINE
ReturnIter unstripIter ( Iter x )
{
    return ALGO_CALL::UnstripIter < ReturnIter, Iter > () ( x ) ;
}
    
    
    
// Define a series of tags to allow composition of stepping.
    
struct Operation_tag {} ; // op(Operation_tag(), i, o);

// These are called before and after the Operation_tag
struct pre_op_ad_tag {} ;
struct pre_op_i_tag {} ;
struct pre_op_o_tag {} ;

struct post_op_i_tag {} ;
struct post_op_o_tag {} ;
struct post_op_ad_tag {} ;

struct EmptyAuxilliaryData {} ;
    
template < typename I, typename O, typename AuxilliaryData, typename Op ALGO_COMMA_ENABLE_IF_PARAM >
struct Step
{
    ALGO_INLINE
    void operator () ( I& i, O& o, AuxilliaryData& ad, Op op ) const
    {
        op ( ALGO_CALL::pre_op_ad_tag (), ad ) ;
        
        op ( ALGO_CALL::pre_op_i_tag (), i ),
        op ( ALGO_CALL::pre_op_o_tag (), o ) ;
        
        op ( ALGO_CALL::Operation_tag (), i, o, ad ) ;
        
        op ( ALGO_CALL::post_op_i_tag (), i ),
        op ( ALGO_CALL::post_op_o_tag (), o ) ;
        
        op ( ALGO_CALL::post_op_ad_tag (), ad ) ;
    }
} ;

template < typename I, typename O, typename Op >
ALGO_INLINE
void step ( I& i, O& o, Op op )
{
    EmptyAuxilliaryData ad ;
    ALGO_CALL::Step < I, O, EmptyAuxilliaryData, Op > () ( i, o, ad, op ) ;
}

template < typename I, typename O, typename AuxilliaryData, typename Op >
ALGO_INLINE
void stepWithAuxilliaryData ( I& i, O& o, AuxilliaryData& ad, Op op )
{
    ALGO_CALL::Step < I, O, AuxilliaryData, Op > () ( i, o, ad, op ) ;
}

    

// Wrapper
template < template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM > class Op ALGO_COMMA_ENABLE_IF_PARAM >
struct AllaryOperatorToBinaryOperator
{
    // Must pass by reference to be transparent to Op
    template < typename I, typename O, typename AuxilliaryData >
    ALGO_INLINE
    void operator () ( ALGO_CALL::Operation_tag, I& i, O& o, AuxilliaryData& ad ) const
    {
        Op < I, O > () ( i, o ) ;
    }
} ;

    
    
template < typename Op
    , template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM > class First
    , template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM > class Second ALGO_COMMA_ENABLE_IF_PARAM >
struct AndUnaryOperator
{
    // Must pass by reference to be transparent to First and Second
    template < typename Iter >
    ALGO_INLINE
    void operator () ( Op, Iter& x ) const
    {
        First < Iter > () ( Op (), x ) ;
        Second < Iter > () ( Op (), x ) ;
    }
} ;
    
    
    
template <
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM > class Op1
    , template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM > class Op2 ALGO_COMMA_ENABLE_IF_PARAM >
struct AndAllaryOperator
{
    // Must pass by reference to be transparent to Op1 and Op2
    template < typename I, typename O, typename AuxilliaryData >
    ALGO_INLINE
    void operator () ( Operation_tag, I& i, O& o, AuxilliaryData& ad ) const
    {
        Op1 < I, O, AuxilliaryData > () ( i, o, ad ) ;
        Op1 < I, O, AuxilliaryData > () ( i, o, ad ) ;
    }
} ;
    
    
    
template < typename Op >
struct DefaultOperation
{
    template < typename Iter >
    ALGO_INLINE
    void operator () ( Op, Iter& ) const
    {}
} ;
    
    

struct DefaultOperations
{
    template < typename Iter, typename Op >
    ALGO_INLINE
    void operator () ( Op, Iter& ) const
    {}
} ;

    
    
// binds successor to the given Op
template <typename Op>
struct Forwards
{
    template < typename Iter >
    ALGO_INLINE
    void operator () ( Op, Iter& x ) const
    {
        ALGO_CALL::successor ( x, ALGO_CALL::InPlace () ) ;
    }
} ;

    
    
// binds predecessor to the given Op
template < typename Op >
struct Backwards
{
    template < typename Iter >
    ALGO_INLINE
    void operator () ( Op, Iter& x ) const
    {
        ALGO_CALL::predecessor ( x, ALGO_CALL::InPlace () ) ;
    }
} ;
    
    

// I forwards, O forwards
// I forwards, O backwards
// I backwards, O forwards
// I backwards, O backwards

struct IForwardsOForwards : Forwards < post_op_i_tag >, Forwards < post_op_o_tag >, DefaultOperations {} ;
struct IForwardsOBackwards : Forwards < post_op_i_tag >, Backwards < pre_op_o_tag >, DefaultOperations {} ;
struct IBackwardsOForwards : Backwards < pre_op_i_tag >, Forwards < post_op_o_tag >, DefaultOperations {} ;
struct IBackwardsOBackwards : Backwards < pre_op_i_tag >, Backwards < pre_op_o_tag >, DefaultOperations {} ;

struct CopyForwards : AllaryOperatorToBinaryOperator < Assign >, IForwardsOForwards {} ;

template < typename I, typename O >
ALGO_INLINE
O copyImpl ( I f, I l, O o )
{
    while ( f != l )
    {
        ALGO_CALL::step ( f
                         , o
                         , ALGO_CALL::CopyForwards () ) ;
    }
    return o ;
}

template < typename I, typename O >
ALGO_INLINE
typename std::enable_if < std::is_same < typename std::remove_cv<I>::type, O >::value &&
                        ALGO_CALL::IsBitwiseCopyable < O >::value, O* >::type
copyImpl ( I* f, I* l, O* o )
{
    const ptrdiff_t diff = ALGO_CALL::distance ( f, l ) ;
    ALGO_ASSERT ( diff >= 0 ) ;
    
    if ( ALGO_LIKELIHOOD ( !ALGO_CALL::equalUnderlyingAddress ( o, f ), true ) )
    {
        std::memmove ( o, f, ALGO_CALL::SizeOf < I >::value * diff ) ;
    }
    return ALGO_CALL::advance ( o, diff ) ;
}
    
template < typename I, typename O >
ALGO_INLINE
O copy ( I f, I l, O o )
{
    if ( f == l ) return o ;
    
    return ALGO_CALL::unstripIter < O > ( ALGO_CALL::copyImpl ( ALGO_CALL::stripIter ( f )
                                                               , ALGO_CALL::stripIter ( l )
                                                               , ALGO_CALL::stripIter ( o ) ) ) ;
}

    
struct CopyBackward : AllaryOperatorToBinaryOperator < Assign >, IBackwardsOBackwards {} ;

template < typename I, typename O >
ALGO_INLINE
typename std::enable_if < std::is_same < typename std::remove_cv < I >::type, O >::value &&
                        ALGO_CALL::IsBitwiseCopyable < O >::value, O* >::type
copyBackwardImpl ( I* f, I* l, O* o )
{
    const ptrdiff_t diff = ALGO_CALL::distance ( f, l ) ;
    ALGO_ASSERT ( diff >= 0 ) ;
    
    O* tmp = ALGO_CALL::advance ( o, -diff ) ;
    
    copyImpl ( f, l, tmp ) ;

    return tmp ;
}
    
template < typename I, typename O >
ALGO_INLINE
O copyBackwardImpl ( I f, I l, O o )
{
    while ( f != l )
    {
        ALGO_CALL::step ( l
                         , o
                         , ALGO_CALL::CopyBackward () ) ;
    }
    return o ;
}
    
template < typename I, typename O >
ALGO_INLINE
O copy_backward ( I f, I l, O o )
{
    if ( f == l ) return o ;
    
    return ALGO_CALL::unstripIter < O > ( ALGO_CALL::copyBackwardImpl ( ALGO_CALL::stripIter ( f )
                                                                       , ALGO_CALL::stripIter ( l )
                                                                       , ALGO_CALL::stripIter ( o ) ) ) ;
}

struct CopyNothingIForwardsO : AllaryOperatorToBinaryOperator < Assign >, Forwards < post_op_o_tag >, DefaultOperations {} ;
    
template < typename Iter >
void fillImpl ( Iter f, Iter l, typename std::iterator_traits < Iter >::value_type const& value )
{
    while ( f != l )
    {
        typename std::iterator_traits < Iter >::value_type const* valuePtr = &value ;
        ALGO_CALL::step ( valuePtr, f, ALGO_CALL::CopyNothingIForwardsO () ) ;
        ALGO_ASSERT ( &value == valuePtr ) ;
    }
}
    
template < typename T >
ALGO_INLINE
typename std::enable_if < ALGO_CALL::IsBitwiseCopyable < T >::value, void >::type
fillImpl ( T* f, T* l, T const& value )
{
    ptrdiff_t toCopy = ALGO_CALL::distance ( f, l ) ;
    ALGO_ASSERT ( toCopy > 0 ) ;
    
    std::memmove ( f, &value, ALGO_CALL::SizeOf < T >::value ) ;
    ptrdiff_t copied = 1 ;
    
    while ( copied * 2 < toCopy )
    {
        std::memcpy ( ALGO_CALL::advance ( f, copied ), f, ALGO_CALL::SizeOf < T >::value * copied ) ;
        copied *= 2 ;
    }
    
    if ( copied != toCopy )
    {
        std::memcpy ( ALGO_CALL::advance ( f, copied ), f, ALGO_CALL::SizeOf < T >::value * ( toCopy - copied ) ) ;
    }
}
    
template < typename Iter >
void fill ( Iter f, Iter l, typename std::iterator_traits < Iter >::value_type const& value )
{
    if ( f == l ) return ;
    
    ALGO_CALL::fillImpl ( ALGO_CALL::stripIter ( f ),
                         ALGO_CALL::stripIter ( l ),
                         value ) ;
}
    
    
    
struct PointerAndSize
{
    char* ptr ;
    ptrdiff_t size ;
} ;
    
    
    
struct BufferCalculation
{
    template < typename T >
    static
    ALGO_INLINE
    T* calculateBegin ( PointerAndSize data )
    {
        ALGO_ASSERT ( ALGO_NULLPTR != data.ptr ) ;
        ALGO_ASSERT ( data.size > 0u ) ;
        
        const uintptr_t offset = reinterpret_cast < uintptr_t > ( data.ptr ) % uintptr_t ( ALGO_CALL::GetAlignmentOf < T >::value ) ;
        
        // Overflow check
        ALGO_ASSERT ( std::numeric_limits < uintptr_t >::max () - offset >= ALGO_CALL::SizeOf < T >::value ) ;
        
        if ( ALGO_LIKELIHOOD ( uintptr_t ( data.size ) >= ( ALGO_CALL::SizeOf < T >::value + offset ), true ) )
        {
            // Overflow check
            ALGO_ASSERT ( std::numeric_limits < ptrdiff_t >::max () >= offset ) ;
            
            return reinterpret_cast < T* > ( ALGO_CALL::advance ( data.ptr, ptrdiff_t ( offset ) ) ) ;
        }
        else
        {
            // Return null if it is not possible to fit in a single instance of type T into the buffer.
            return ALGO_NULLPTR ;
        }
    }
    
    template < typename T >
    static
    ALGO_INLINE
    T* calculateEnd ( T* const begin, PointerAndSize data )
    {
        ALGO_ASSERT ( ALGO_NULLPTR != data.ptr ) ;
        ALGO_ASSERT ( data.size > 0u ) ;
        
        if ( ALGO_LIKELIHOOD ( ALGO_NULLPTR == begin, false ) )
        {
            return ALGO_NULLPTR ;
        }
        else
        {
            ptrdiff_t const offset = ALGO_CALL::distance ( data.ptr, reinterpret_cast < char* >( begin ) ) ;
            ALGO_ASSERT ( offset >= 0 ) ;
            
            // asertion is equivalent to ( data.size - offset ) / ALGO_CALL::SizeOf < T >::value but without the divide
            // and the dangerous subtract
            ALGO_ASSERT ( uintptr_t ( data.size ) >= ( ALGO_CALL::SizeOf < T >::value + offset ) ) ;
            
            // Overflow check
            ALGO_ASSERT ( ( data.size - offset ) <= uintptr_t ( std::numeric_limits < ptrdiff_t >::max () ) ) ;
            
            return ALGO_CALL::advance ( begin, ptrdiff_t ( data.size - offset ) / ALGO_CALL::SizeOf < T >::value ) ;
        }
    }
} ;
    
// Provides storage, does not manage object lifetimes, nor is it coupled to the underlying object type
template < ptrdiff_t Size >
struct StackBuffer
{
    // No need to make it private and remove its pod-ness
    char d_buff [ Size ] ;
    
    template < typename T >
    ALGO_INLINE
    T* begin ()
    {
        return ALGO_CALL::BufferCalculation::template calculateBegin < T > ( { &this->d_buff[0], Size } ) ;
    }
    
    template < typename T >
    ALGO_INLINE
    T* end ()
    {
        return ALGO_CALL::BufferCalculation::calculateEnd ( this->template begin < T > (), { &this->d_buff[0], Size } ) ;
    }
} ;
    
    
    
struct MallocFreeProtocol
{
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        void* const returnValue = malloc ( size ) ;
        
        if ( ALGO_LIKELIHOOD ( returnValue, true ) )
        {
            return { reinterpret_cast < char* > ( returnValue ), size } ;
        }
        else
        {
            throw std::bad_alloc () ;
        }
    }
    
    ALGO_INLINE
    static void deallocate ( ALGO_CALL::PointerAndSize data )
    {
        if ( ALGO_LIKELIHOOD ( data.ptr, true ) )
        {
            free ( data.ptr ) ;
        }
    }
} ;

    
    
struct CallocFreeProtocol : MallocFreeProtocol
{
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        void* const returnValue = calloc ( size, 1u ) ;
        
        if ( ALGO_LIKELIHOOD ( returnValue, true ) )
        {
            return { reinterpret_cast < char* > ( returnValue ), size } ;
        }
        else
        {
            throw std::bad_alloc () ;
        }
    }
} ;

    
    
struct NewDeleteProtocol
{
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        return { new char [ size ], size } ;
    }

    ALGO_INLINE
    static void deallocate ( ALGO_CALL::PointerAndSize data )
    {
        if ( ALGO_LIKELIHOOD ( data.ptr, true ) )
        {
            delete [] ( data.ptr ) ;
        }
    }
} ;

    
    
struct ZeroedNewDeleteProtocol : NewDeleteProtocol
{
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        const ALGO_CALL::PointerAndSize returnValue = NewDeleteProtocol::allocate ( size ) ;
        // Fill does not throw
        fill ( returnValue.ptr, ALGO_CALL::advance ( returnValue.ptr, returnValue.size ), 0 ) ;
        return returnValue ;
    }
} ;
    
    
struct StlTemporaryBufferProtocol
{
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        const std::pair< char*, std::ptrdiff_t > returnValue = std::get_temporary_buffer < char > ( size ) ;
        if ( ALGO_LIKELIHOOD ( returnValue.first, true ) )
        {
            return { returnValue.first, returnValue.second } ;
        }
        else
        {
            throw std::bad_alloc () ;
        }
    }
    
    ALGO_INLINE
    static void deallocate ( ALGO_CALL::PointerAndSize data )
    {
        if ( ALGO_LIKELIHOOD ( data.ptr, true ) )
        {
            std::return_temporary_buffer ( data.ptr ) ;
        }
    }
} ;

struct ZeroedStlTemporaryBufferProtocol : StlTemporaryBufferProtocol
{
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        const ALGO_CALL::PointerAndSize returnValue = StlTemporaryBufferProtocol::allocate ( size ) ;
        // Fill does not throw
        fill ( returnValue.ptr, ALGO_CALL::advance ( returnValue.ptr, returnValue.size ), 0 ) ;
        return returnValue ;
    }
} ;
    
template < typename AllocatorProtocol >
class AllocatingBuffer
{
public:
    ALGO_INLINE
    explicit AllocatingBuffer ( const ptrdiff_t size )
        : d_data ( this->allocate ( size ) )
    {}
    
    ALGO_INLINE
    ~AllocatingBuffer ()
    {
        this->deallocate ( this->d_data ) ;
        ALGO_SAFE_MODE ( this->d_data.ptr = ALGO_NULLPTR ) ;
        ALGO_SAFE_MODE ( this->d_data.size = 0u ) ;
    }
    
    template < typename T >
    ALGO_INLINE
    T* begin ()
    {
        return ALGO_CALL::BufferCalculation::template calculateBegin < T > ( this->d_data ) ;
    }
    
    template < typename T >
    ALGO_INLINE
    T* end ()
    {
        return ALGO_CALL::BufferCalculation::calculateEnd ( this->template begin < T > (), this->d_data ) ;
    }
    
private:
    AllocatingBuffer ( AllocatingBuffer const& ) ;
    AllocatingBuffer& operator= ( AllocatingBuffer const& ) ;
    
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        ALGO_ASSERT ( size > 0 ) ;
        return AllocatorProtocol::allocate ( size ) ;
    }
    
    ALGO_INLINE
    static void deallocate ( ALGO_CALL::PointerAndSize data )
    {
        AllocatorProtocol::deallocate ( data ) ;
    }
    
    PointerAndSize d_data ;
} ;

    
    
template < typename T, bool destructionLikely = false ALGO_COMMA_ENABLE_IF_PARAM >
struct ObjectProctor
{
    ALGO_INLINE
    explicit ObjectProctor ( T* obj )
        : d_obj ( obj )
    {}
    
    ALGO_INLINE
    explicit ObjectProctor ( T& obj )
        : d_obj ( &obj )
    {}
    
    ALGO_INLINE
    ~ObjectProctor ()
    {
        if ( ALGO_LIKELIHOOD ( this->d_obj, destructionLikely ) )
        {
            this->destroyObject () ;
        }
    }
    
    ALGO_INLINE
    void disarm ()
    {
        this->d_obj = ALGO_NULLPTR ;
    }
private:
    
    ObjectProctor ( ObjectProctor const& x ) ;
    ObjectProctor& operator= ( ObjectProctor const& ) ;
    
    // non-inline, for unusual case of having to do something
    void destroyObject ()
    {
        T* const obj = this->d_obj ;
        ALGO_SAFE_MODE ( this->d_obj = ALGO_NULLPTR ) ;
        ALGO_CALL::destroyPointed ( obj ) ;
    }
    
    T* d_obj ;
} ;
    
    
// Optimisation for when destruction is not required
template < typename T, bool destructionLikely >
struct ObjectProctor < T, destructionLikely,
    typename std::enable_if < ALGO_CALL::IsTriviallyDestructible < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    explicit ObjectProctor ( T* )
    {}
    
    ALGO_INLINE
    explicit ObjectProctor ( T& )
    {}
    
    // No need for a destructor
    
    ALGO_INLINE
    void disarm ()
    {}
private:
    
    ObjectProctor ( ObjectProctor const& x ) ;
    ObjectProctor& operator= ( ObjectProctor const& ) ;
} ;



template < typename Iter, bool destructionLikely = false ALGO_COMMA_ENABLE_IF_PARAM >
struct IterProctor
{
    ALGO_INLINE
    explicit IterProctor ( Iter x )
    : d_start ( x )
    , d_end   ( x )
    {}
    
    ALGO_INLINE
    ~IterProctor ()
    {
        if ( ALGO_LIKELIHOOD ( this->d_start != this->d_end, destructionLikely ) )
        {
            this->destroyExtent () ;
        }
    }
    
    ALGO_INLINE
    void incrementStart ()
    {
        // Can't increment start past end
        ALGO_ASSERT ( this->d_start != this->d_end ) ;
        ALGO_CALL::successor ( this->d_start, ALGO_CALL::InPlace () ) ;
    }
    
    ALGO_INLINE
    void incrementEnd ()
    {
        ALGO_CALL::successor ( this->d_end, ALGO_CALL::InPlace () ) ;
    }
    
private:
    IterProctor ( IterProctor const& ) ;
    IterProctor& operator= ( IterProctor const& ) ;
    
    // Keep non-inline ... code is unlikely to be executed
    void destroyExtent ()
    {
        while ( this->d_start != this->d_end )
        {
            ALGO_CALL::destroyPointed ( this->d_start ) ; // Throwing on destruction is undefined behaviour ...
            ALGO_CALL::successor ( this->d_start, ALGO_CALL::InPlace () ) ;
        }
    }
    
    Iter d_start ;
    Iter d_end ;
} ;
    
    
    
// Optimisation for when destruction is not required
template < typename Iter, bool destructionLikely >
struct IterProctor < Iter, destructionLikely,
    typename std::enable_if < ALGO_CALL::IsTriviallyDestructible < typename std::iterator_traits < Iter >::value_type >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    explicit IterProctor ( Iter )
    {}
    
    // No need for a destructor
    
    ALGO_INLINE
    void incrementStart ()
    {}
    
    ALGO_INLINE
    void incrementEnd ()
    {}
    
private:
    IterProctor ( IterProctor const& ) ;
    IterProctor& operator= ( IterProctor const& ) ;
} ;

    
// Acts as a simple wrapper to the more general IterProctor, to save clients having to type T* rather than T.
template < typename T, bool destructionLikely = false ALGO_COMMA_ENABLE_IF_PARAM >
struct BufferProctor : ALGO_CALL::IterProctor < T*, destructionLikely >
{
    typedef ALGO_CALL::IterProctor < T*, destructionLikely > ParentType ;
    
    ALGO_INLINE
    explicit BufferProctor ( T* x )
        : ParentType ( x )
    {}
} ;
    
    
    
} // namespace algo


#endif
