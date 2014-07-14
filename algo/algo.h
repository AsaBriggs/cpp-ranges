#ifndef INCLUDED_ALGO
#define INCLUDED_ALGO

#ifndef INCLUDED_ALGO_BASICS
#include "algo_basics.h"
#endif

#ifndef INCLUDED_ALGO_BUFFER
#include "algo_buffer.h"
#endif

#ifndef INCLUDED_ALGO_ITERATOR
#include "algo_iterator.h"
#endif

#ifndef INCLUDED_ALGO_RANGE
#include "algo_range.h"
#endif

#ifndef INCLUDED_ALGO_TEMPLATE_PARAMS
#include "algo_template_parms.h"
#endif

#ifndef INCLUDED_ALGO_TRAITS
#include "algo_traits.h"
#endif


namespace algo
{
// Define a series of tags to allow composition of stepping.
    
struct Operation_tag {} ;

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
    
    
    
template < typename Op ALGO_COMMA_ENABLE_IF_PARAM >
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
template < typename Op ALGO_COMMA_ENABLE_IF_PARAM >
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
template < typename Op ALGO_COMMA_ENABLE_IF_PARAM >
struct Backwards
{
    template < typename Iter >
    ALGO_INLINE
    void operator () ( Op, Iter& x ) const
    {
        ALGO_CALL::predecessor ( x, ALGO_CALL::InPlace () ) ;
    }
} ;


template < typename Tag, template < typename Tag2 ALGO_COMMA_ENABLE_IF_PARAM > class ToBind >
struct BoundParam
{
    typedef ALGO_CALL::Param < Tag, ToBind < Tag > > type ;
} ;

    
typedef ALGO_CALL::Parameters <
    ALGO_CALL::BoundParam < ALGO_CALL::pre_op_i_tag, ALGO_CALL::DefaultOperation >::type
    , ALGO_CALL::BoundParam < ALGO_CALL::post_op_i_tag, ALGO_CALL::DefaultOperation >::type
    , ALGO_CALL::BoundParam < ALGO_CALL::pre_op_o_tag, ALGO_CALL::DefaultOperation >::type
    , ALGO_CALL::BoundParam < ALGO_CALL::post_op_o_tag, ALGO_CALL::DefaultOperation >::type
    , ALGO_CALL::BoundParam < ALGO_CALL::pre_op_ad_tag, ALGO_CALL::DefaultOperation >::type
    , ALGO_CALL::BoundParam < ALGO_CALL::post_op_ad_tag, ALGO_CALL::DefaultOperation >::type
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

    
    
template < typename InputRange, typename OutputRange, typename StepOperation ALGO_COMMA_ENABLE_IF_PARAM >
struct StepOverRange
{
    typedef std::pair < OutputRange, InputRange > ReturnType ;
    
    ALGO_INLINE
    ReturnType operator () ( InputRange from, OutputRange to, StepOperation op ) const
    {
        while ( !ALGO_CALL::isEmpty ( from )
               && !ALGO_CALL::isEmpty ( to ) )
        {
            ALGO_CALL::step ( from
                            , to
                            , op ) ;
        }
        return std::make_pair ( to, from ) ;
    }
} ;

template < typename I, typename O, typename StepOperation >
ALGO_INLINE
O stepOver ( I from, I to, O o, StepOperation op )
{
    typedef typename ALGO_CALL::BasicBoundedRange < I >::type InputRange ;
    typedef typename ALGO_CALL::BasicUnboundedRange < O >::type OutputRange ;
    
    InputRange input = { from, to } ;
    OutputRange output = { o } ;
    std::pair < OutputRange, InputRange > result
        = ALGO_CALL::StepOverRange < InputRange, OutputRange, StepOperation > () ( input, output, op ) ;
    return ALGO_CALL::getValueByReference < StartIterator > ( result.first ) ;
}

template < typename I, typename N, typename O, typename StepOperation >
ALGO_INLINE
O stepCounted ( I from, N times, O o, StepOperation op )
{
    typedef typename ALGO_CALL::BasicCountedRange < I, N >::type InputRange ;
    typedef typename ALGO_CALL::BasicUnboundedRange < O >::type OutputRange ;
        
    InputRange input = { from, times } ;
    OutputRange output = { o } ;
    std::pair < OutputRange, InputRange > result
        = ALGO_CALL::StepOverRange < InputRange, OutputRange, StepOperation > () ( input, output, op ) ;
    return ALGO_CALL::getValueByReference < StartIterator > ( result.first ) ;
}
    
    
    
template < typename I, typename O, typename StepOperation ALGO_COMMA_ENABLE_IF_PARAM >
struct StepOver
{
    ALGO_INLINE
    O operator () ( I from, I to, O o, StepOperation op ) const
    {
        while ( from != to )
        {
            ALGO_CALL::step ( from
                                , o
                                , op ) ;
        }
        return o ;
    }
} ;
    
template < typename I, typename O, typename StepOperation >
ALGO_INLINE
O stepOverIter ( I from, I to, O o, StepOperation op )
{
    return ALGO_CALL::StepOver < I, O, StepOperation > () ( from, to, o, op ) ;
}
    
    
    
template < typename I, typename N, typename O, typename StepOperation ALGO_COMMA_ENABLE_IF_PARAM >
struct StepCounted
{
    ALGO_INLINE
    O operator () ( I from, N times, O o, StepOperation op ) const
    {
        while ( times )
        {
            --times,
            ALGO_CALL::step ( from
                                , o
                                , op ) ;
        }
        return o ;
    }
} ;
    
template < typename I, typename N, typename O, typename StepOperation >
ALGO_INLINE
O stepCountedIter ( I from, N times, O o, StepOperation op )
{
    return ALGO_CALL::StepCounted < I, N, O, StepOperation > () ( from, times, o, op ) ;
}
    
    
    
typedef OverallOperation <
    ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::AllaryOperatorToBinaryOperator < ALGO_CALL::Assign > >
    , ALGO_CALL::BoundParam < ALGO_CALL::post_op_i_tag, ALGO_CALL::Forwards >::type
    , ALGO_CALL::BoundParam < ALGO_CALL::post_op_o_tag, ALGO_CALL::Forwards >::type
> CopyForwardOperation ;
    
typedef OverallOperation <
    ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::AllaryOperatorToBinaryOperator < ALGO_CALL::MoveAssign > >
    , ALGO_CALL::BoundParam < ALGO_CALL::post_op_i_tag, ALGO_CALL::Forwards >::type
    , ALGO_CALL::BoundParam < ALGO_CALL::post_op_o_tag, ALGO_CALL::Forwards >::type
> MoveForwardOperation ;
    
template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct Copy
{
    ALGO_INLINE
    O operator () ( I f, I l, O o ) const
    {
        return ALGO_CALL::stepOver ( f
                                    , l
                                    , o
                                    , CopyForwardOperation::type () ) ;
    }
} ;

template < typename I, typename O >
struct Copy <
    I*
    , O*
    , typename std::enable_if < std::is_same < typename std::remove_cv<I>::type, O >::value &&
        ALGO_CALL::IsBitwiseCopyable < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    O* operator () ( I* f, I* l, O* o ) const
    {
        ptrdiff_t const diff = ALGO_CALL::distance ( f, l ) ;
        ALGO_ASSERT ( diff >= 0 ) ;
        
        if ( ALGO_LIKELIHOOD ( !ALGO_CALL::equalUnderlyingAddress ( o, f ), true ) )
        {
            ALGO_CALL::copyBytesOverlapped ( o
                                            , f
                                            , diff ) ;
        }
        
        return ALGO_CALL::advance ( o, diff ) ;
    }
} ;
    
template < typename I, typename O >
ALGO_INLINE
O copy ( I f, I l, O o )
{
    if ( f == l ) return o ;
    
    return ALGO_CALL::unstripIter < O > ( ALGO_CALL::Copy < I, O > () ( ALGO_CALL::stripIter ( f )
                                                                        , ALGO_CALL::stripIter ( l )
                                                                        , ALGO_CALL::stripIter ( o ) ) ) ;
}
    
template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct CopyBackward
{
    typedef OverallOperation <
          ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::AllaryOperatorToBinaryOperator < ALGO_CALL::Assign > >
        , ALGO_CALL::BoundParam < ALGO_CALL::pre_op_i_tag, ALGO_CALL::Backwards >::type
        , ALGO_CALL::BoundParam < ALGO_CALL::pre_op_o_tag, ALGO_CALL::Backwards >::type
    > CopyBackwardOperation ;
    
    ALGO_INLINE
    O operator () ( I f, I l, O o ) const
    {
        // Note passes last as the from iterator
        return ALGO_CALL::stepOver ( l
                                    , f
                                    , o
                                    , CopyBackwardOperation::type () ) ;
    }
} ;

template < typename I, typename O >
struct CopyBackward <
    I*
    , O*
    , typename std::enable_if < std::is_same < typename std::remove_cv<I>::type, O >::value &&
        ALGO_CALL::IsBitwiseCopyable < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    O* operator () ( I* f, I* l, O* o ) const
    {
        ptrdiff_t const diff = ALGO_CALL::distance ( f, l ) ;
        ALGO_ASSERT ( diff >= 0 ) ;
        
        O* const tmp = ALGO_CALL::advance ( o, -diff ) ;
        
        copy ( f, l, tmp ) ;
        
        return tmp ;
    }
} ;
    
template < typename I, typename O >
ALGO_INLINE
O copy_backward ( I f, I l, O o )
{
    if ( f == l ) return o ;
    
    return ALGO_CALL::unstripIter < O > ( ALGO_CALL::CopyBackward < I, O > () ( ALGO_CALL::stripIter ( f )
                                                                               , ALGO_CALL::stripIter ( l )
                                                                               , ALGO_CALL::stripIter ( o ) ) ) ;
}

    
    
template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct Fill
{
    typedef OverallOperation <
          ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::AllaryOperatorToBinaryOperator < ALGO_CALL::Assign > >
        , ALGO_CALL::BoundParam < ALGO_CALL::post_op_o_tag, ALGO_CALL::Forwards >::type
    > CopyNothingIForwardsO ;
    
    ALGO_INLINE
    void operator () ( Iter f, Iter l, typename ALGO_CALL::IteratorTraits < Iter >::value_type const& value ) const
    {
        typename ALGO_CALL::IteratorTraits < Iter >::value_type const* const valuePtr = &value ;
        
        while ( f != l )
        {
            ALGO_CALL::step ( valuePtr
                             , f
                             , CopyNothingIForwardsO::type () ) ;
        }
    }
} ;
    
    
    
template < typename T >
struct Fill <
    T*
    , typename std::enable_if < ALGO_CALL::IsBitwiseCopyable < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    void operator () ( T* f, T* l, T const value ) const
    {
        ptrdiff_t const toCopy = ALGO_CALL::distance ( f, l ) ;
        ALGO_ASSERT ( toCopy > 0 ) ;
        
        ALGO_CALL::copyBytesNotOverlapped ( f
                                        , &value
                                        , 1 ) ;
        ptrdiff_t copied = 1 ;
        
        while ( copied * 2 < toCopy )
        {
            ALGO_CALL::copyBytesNotOverlapped ( ALGO_CALL::advance ( f, copied )
                                               , f
                                               , copied ) ;
            copied *= 2 ;
        }
        
        if ( copied != toCopy )
        {
            ALGO_ASSERT ( toCopy > copied ) ;
            
            ALGO_CALL::copyBytesNotOverlapped ( ALGO_CALL::advance ( f, copied )
                                               , f
                                               , ( toCopy - copied ) ) ;
        }
    }
} ;
    
template < typename Iter >
ALGO_INLINE
void fill ( Iter f, Iter l, typename ALGO_CALL::IteratorTraits < Iter >::value_type const& value )
{
    if ( f == l ) return ;
    
    ALGO_CALL::Fill < Iter > () ( ALGO_CALL::stripIter ( f )
                                 , ALGO_CALL::stripIter ( l )
                                 , value ) ;
}
    
    

struct RotateNoChoice {} ;
struct RotateBlocks : RotateNoChoice {} ;
    
    
    
template < typename Iter, typename Choice ALGO_COMMA_ENABLE_IF_PARAM >
struct RotateRightByOne
{
    ALGO_INLINE
    void operator () ( Iter f, Iter m ) const
    {
        typename ALGO_CALL::IteratorTraits < Iter >::value_type tmp ( ALGO_CALL::derefMove ( m ) ) ;
        // m = location of the hole
            
        while ( m != f )
        {
            Iter pred = ALGO_CALL::predecessor ( m ) ;
            ALGO_CALL::moveAssign ( pred, m ) ;
            m = pred ;
        }
        ALGO_CALL::moveAssign ( &tmp, m ) ; // f or m, either could have used.
    }
} ;
    
    
    
template < typename T >
struct RotateRightByOne <
    T*
    , ALGO_CALL::RotateBlocks
    , typename std::enable_if < ALGO_CALL::IsBitwiseCopyable < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    void operator () ( T* f, T* m ) const
    {
        // No alignment requirements as this is just a temporary buffer where the value is not "used".
        char buffer [ sizeof ( T ) ] ;
            
        ALGO_CALL::copyBytesNotOverlapped ( reinterpret_cast < T* > ( buffer ), m, 1 ) ;
            
        ALGO_CALL::copyBytesOverlapped ( f + 1, f, m - f ) ;
            
        ALGO_CALL::copyBytesNotOverlapped ( f, reinterpret_cast < T* > ( buffer ), 1 ) ;
    }
} ;
    
    
    
template < typename Iter, typename Choice >
ALGO_INLINE
void rotateRightByOne ( Iter f, Iter m, Choice )
{
    if ( f == m ) return ;
        
    return ALGO_CALL::RotateRightByOne < Iter, Choice > () ( f, m ) ;
}
  
    
    
template < typename Iter, typename Choice ALGO_COMMA_ENABLE_IF_PARAM >
struct RotateLeftByOne
{
    ALGO_INLINE
    void operator () ( Iter f, Iter m ) const
    {
        typename ALGO_CALL::IteratorTraits < Iter >::value_type tmp ( ALGO_CALL::derefMove ( f ) ) ;
        // f = location of the hole
            
        while ( m != f )
        {
            Iter succ = ALGO_CALL::successor ( f ) ;
            ALGO_CALL::moveAssign ( succ, f ) ;
            f = succ ;
        }
            
        ALGO_CALL::moveAssign ( &tmp, m ) ; // f or m, either could have used.
    }
} ;
    
    
    
template < typename T >
struct RotateLeftByOne <
    T*
    , ALGO_CALL::RotateBlocks
    , typename std::enable_if < ALGO_CALL::IsBitwiseCopyable < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    void operator () ( T* f, T* m ) const
    {
        // No alignment requirements as this is just a temporary buffer where the value is not "used".
        char buffer [ sizeof ( T ) ] ;
            
        ALGO_CALL::copyBytesNotOverlapped ( reinterpret_cast < T* > ( buffer ), f, 1 ) ;
            
        ALGO_CALL::copyBytesOverlapped ( f, f + 1, m - f ) ;
            
        ALGO_CALL::copyBytesNotOverlapped ( m, reinterpret_cast < T* > ( buffer ), 1 ) ;
    }
} ;
   
    
    
template < typename Iter, typename Choice >
ALGO_INLINE
void rotateLeftByOne ( Iter f, Iter m, Choice )
{
    if ( f == m ) return ;
        
    return ALGO_CALL::RotateLeftByOne < Iter, Choice > () ( f, m ) ;
}
    
    
    
template < typename Iter, typename N, typename Cmp >
ALGO_INLINE
Iter minIter ( Iter f, N n, Cmp cmp )
{
    if ( n <= N ( 1 ) ) return f ;
        
    Iter min = f ;
    ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ) ;
    --n ;
        
    while ( n )
    {
        if ( cmp ( ALGO_CALL::deref ( f ), ALGO_CALL::deref ( min )  ) )
        {
            min = f ;
        }
        ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ),
        --n ;
    }
        
    return min ;
}
   
    
    
template < typename Iter, typename Cmp >
ALGO_INLINE
Iter minIter ( Iter f, Iter l, Cmp cmp )
{
    if ( f == l ) return f ;
        
    Iter min = f ;
    ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ) ;
        
    while ( f != l )
    {
        if ( cmp ( ALGO_CALL::deref ( f ), ALGO_CALL::deref ( min )  ) )
        {
            min = f ;
        }
        ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ) ;
    }
        
    return min ;
}
    
    
    
template < typename Iter, typename N, typename Cmp >
ALGO_INLINE
Iter maxIter ( Iter f, N n, Cmp cmp )
{
    if ( n <= N ( 1 ) ) return f ;
        
    Iter max = f ;
    ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ) ;
    --n ;
        
    while ( n )
    {
        if ( !cmp ( ALGO_CALL::deref ( f ), ALGO_CALL::deref ( max )  ) )
        {
            max = f ;
        }
        ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ),
        --n ;
    }
        
    return max ;
}
    
    
    
template < typename Iter, typename Cmp >
ALGO_INLINE
Iter maxIter ( Iter f, Iter l, Cmp cmp )
{
    if ( f == l ) return f ;
        
    Iter max = f ;
    ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ) ;
        
    while ( f != l )
    {
        if ( !cmp ( ALGO_CALL::deref ( f ), ALGO_CALL::deref ( max )  ) )
        {
            max = f ;
        }
        ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ) ;
    }
        
    return max ;
}
    
    
    
struct ZeroedNewDeleteProtocol : NewDeleteProtocol
{
    ALGO_INLINE
    static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
    {
        const ALGO_CALL::PointerAndSize returnValue = NewDeleteProtocol::allocate ( size ) ;
        // Fill does not throw
        fill ( returnValue.ptr
              , ALGO_CALL::advance ( returnValue.ptr, returnValue.size )
              , 0 ) ;
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
        fill ( returnValue.ptr
              , ALGO_CALL::advance ( returnValue.ptr, returnValue.size )
              , 0 ) ;
        return returnValue ;
    }
} ;
    
    
    
} // namespace algo


#endif
