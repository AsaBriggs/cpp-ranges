#ifndef INCLUDED_ALGO
#define INCLUDED_ALGO

#include "algo_template_parms.h"
#include "algo_basics.h"
#include "algo_traits.h"
#include "algo_buffer.h"

namespace algo
{
    
    
    
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

    
    
struct DefaultAllaryOperator
{
    // Must pass by reference to be transparent to Op1 and Op2
    template < typename I, typename O, typename AuxilliaryData >
    ALGO_INLINE
    void operator () ( Operation_tag, I& i, O& o, AuxilliaryData& ad ) const
    {}
};

    
    
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



    
typedef ALGO_CALL::Parameters <
      ALGO_CALL::Param < ALGO_CALL::pre_op_i_tag, ALGO_CALL::DefaultOperation < ALGO_CALL::pre_op_i_tag > >
    , ALGO_CALL::Param < ALGO_CALL::post_op_i_tag, ALGO_CALL::DefaultOperation < ALGO_CALL::post_op_i_tag > >
    , ALGO_CALL::Param < ALGO_CALL::pre_op_o_tag, ALGO_CALL::DefaultOperation < ALGO_CALL::pre_op_o_tag > >
    , ALGO_CALL::Param < ALGO_CALL::post_op_o_tag, ALGO_CALL::DefaultOperation < ALGO_CALL::post_op_o_tag > >
    , ALGO_CALL::Param < ALGO_CALL::pre_op_ad_tag, ALGO_CALL::DefaultOperation < ALGO_CALL::pre_op_ad_tag > >
    , ALGO_CALL::Param < ALGO_CALL::post_op_ad_tag, ALGO_CALL::DefaultOperation < ALGO_CALL::post_op_ad_tag > >
    , ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::DefaultAllaryOperator >
> DefaultStepOperationParams ;
    
template <
    typename T0 = ALGO_CALL::DefaultParam
    , typename T1 = ALGO_CALL::DefaultParam
    , typename T2 = ALGO_CALL::DefaultParam
    , typename T3 = ALGO_CALL::DefaultParam
    , typename T4 = ALGO_CALL::DefaultParam
    , typename T5 = ALGO_CALL::DefaultParam
    , typename T6 = ALGO_CALL::DefaultParam >
struct OverallOperation
{
    typedef typename ALGO_CALL::DeduceTypes <
        ALGO_CALL::Parameters < T0, T1, T2, T3, T4, T5, T6 >
        , DefaultStepOperationParams
    >::type DeducedTypes ;
    
    struct type
        : DeducedTypes::Param0
        , DeducedTypes::Param1
        , DeducedTypes::Param2
        , DeducedTypes::Param3
        , DeducedTypes::Param4
        , DeducedTypes::Param5
        , DeducedTypes::Param6
    {};
} ;


typedef OverallOperation <
    ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::AllaryOperatorToBinaryOperator < ALGO_CALL::Assign > >
    , ALGO_CALL::Param < ALGO_CALL::post_op_i_tag, ALGO_CALL::Forwards < ALGO_CALL::post_op_i_tag > >
    , ALGO_CALL::Param < ALGO_CALL::post_op_o_tag, ALGO_CALL::Forwards < ALGO_CALL::post_op_o_tag > >
>::type CopyForwards ;
    
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
    
    
    
typedef OverallOperation <
    ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::AllaryOperatorToBinaryOperator < ALGO_CALL::Assign > >
    , ALGO_CALL::Param < ALGO_CALL::pre_op_i_tag, ALGO_CALL::Backwards < ALGO_CALL::pre_op_i_tag > >
    , ALGO_CALL::Param < ALGO_CALL::pre_op_o_tag, ALGO_CALL::Backwards < ALGO_CALL::pre_op_o_tag > >
>::type CopyBackward ;
    
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

    
    
    
typedef OverallOperation <
    ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::AllaryOperatorToBinaryOperator < ALGO_CALL::Assign > >
    , ALGO_CALL::Param < ALGO_CALL::post_op_o_tag, ALGO_CALL::Forwards < ALGO_CALL::post_op_o_tag > >
    >::type CopyNothingIForwardsO ;
    
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
    
    
    
} // namespace algo


#endif
