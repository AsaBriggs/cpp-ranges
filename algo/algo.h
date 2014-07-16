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

struct pre_op_i_tag {} ;
struct pre_op_o_tag {} ;

struct post_op_i_tag {} ;
struct post_op_o_tag {} ;
    
template < typename I, typename O, typename Op ALGO_COMMA_ENABLE_IF_PARAM >
struct Step
{
    ALGO_INLINE
    static void apply ( I& i, O& o, Op op )
    {
        op.apply ( ALGO_CALL::pre_op_i_tag (), i ),
        op.apply ( ALGO_CALL::pre_op_o_tag (), o ) ;
        
        op.apply ( ALGO_CALL::Operation_tag (), i, o ) ;
        
        op.apply ( ALGO_CALL::post_op_i_tag (), i ),
        op.apply ( ALGO_CALL::post_op_o_tag (), o ) ;
    }
} ;

template < typename I, typename O, typename Op >
ALGO_INLINE
void step ( I& i, O& o, Op op )
{
    ALGO_CALL::Step < I, O, Op >::apply ( i, o, op ) ;
}

    

// Wrapper
template < template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM > class Op ALGO_COMMA_ENABLE_IF_PARAM >
struct AllaryOperatorToBinaryOperator
{
    // Must pass by reference to be transparent to Op
    template < typename I, typename O >
    ALGO_INLINE
    static void apply ( ALGO_CALL::Operation_tag, I& i, O& o )
    {
        Op < I, O >::apply ( i, o ) ;
    }
} ;

    
    
template < typename Op
    , template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM > class Op1
    , template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM > class Op2 ALGO_COMMA_ENABLE_IF_PARAM >
struct AndUnaryOperator
{
    // Must pass by reference to be transparent to First and Second
    template < typename Iter >
    ALGO_INLINE
    static void apply ( Op, Iter& x )
    {
        Op1 < Iter >::apply ( Op (), x ) ;
        Op2 < Iter >::apply ( Op (), x ) ;
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
    static void apply ( Operation_tag, I& i, O& o, AuxilliaryData& ad )
    {
        Op1 < I, O, AuxilliaryData >::apply ( i, o, ad ) ;
        Op2 < I, O, AuxilliaryData >::apply ( i, o, ad ) ;
    }
} ;
    
    
    
template < typename Op ALGO_COMMA_ENABLE_IF_PARAM >
struct DefaultOperation
{
    template < typename Iter >
    ALGO_INLINE
    static void apply ( Op, Iter& )
    {}
} ;

    
    
struct DefaultAllaryOperator
{
    // Must pass by reference to be transparent to Op1 and Op2
    template < typename I, typename O >
    ALGO_INLINE
    static void apply ( Operation_tag, I& i, O& o )
    {}
};

    
    
// binds successor to the given Op
template < typename Op ALGO_COMMA_ENABLE_IF_PARAM >
struct Forwards
{
    template < typename Iter >
    ALGO_INLINE
    static void apply ( Op, Iter& x )
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
    static void apply ( Op, Iter& x )
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
    , ALGO_CALL::Param < ALGO_CALL::Operation_tag, ALGO_CALL::DefaultAllaryOperator >
> DefaultStepOperationParams ;
    
template <
    typename T0 = ALGO_CALL::DefaultParam
    , typename T1 = ALGO_CALL::DefaultParam
    , typename T2 = ALGO_CALL::DefaultParam
    , typename T3 = ALGO_CALL::DefaultParam
    , typename T4 = ALGO_CALL::DefaultParam >
struct OverallOperation
{
    typedef typename ALGO_CALL::DeduceTypes <
        ALGO_CALL::Parameters < T0, T1, T2, T3, T4 >
        , DefaultStepOperationParams
    >::type DeducedTypes ;
    
    struct type
        : DeducedTypes::Param0
        , DeducedTypes::Param1
        , DeducedTypes::Param2
        , DeducedTypes::Param3
        , DeducedTypes::Param4
    {
        using DeducedTypes::Param0::apply ;
        using DeducedTypes::Param1::apply ;
        using DeducedTypes::Param2::apply ;
        using DeducedTypes::Param3::apply ;
        using DeducedTypes::Param4::apply ;    
    };
} ;

    
    
template < typename InputRange, typename OutputRange, typename StepOperation ALGO_COMMA_ENABLE_IF_PARAM >
struct StepOverRange
{
    typedef std::pair < OutputRange, InputRange > ReturnType ;
    
    //ALGO_INLINE
    static ReturnType apply ( InputRange from, OutputRange to, StepOperation op )
    {
        while ( !ALGO_CALL::isEmpty ( from )
               && !ALGO_CALL::isEmpty ( to ) )
        {
            ALGO_CALL::Step < InputRange, OutputRange, StepOperation >::apply (
                            from
                            , to
                            , op ) ;
        }
        return std::make_pair ( to, from ) ;
    }
} ;

    
template < typename InputRange, typename OutputRange, typename StepOperation >
ALGO_INLINE
OutputRange stepOverDeduced ( InputRange x, OutputRange y, StepOperation op )
{
    ALGO_STATIC_ASSERT_IS_RANGE ( InputRange ) ;
    ALGO_STATIC_ASSERT_IS_RANGE ( OutputRange ) ;
    
    ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < InputRange >::type::value
                        || ALGO_CALL::IsAFiniteRange < OutputRange >::type::value), "Infinite loop!" ) ;
        
    return ALGO_CALL::StepOverRange < InputRange, OutputRange, StepOperation >::apply ( x, y, op ).first ;
}
    
template < typename I, typename O, typename StepOperation >
ALGO_INLINE
O stepOver ( I f, I l, O o, StepOperation op )
{
    return ALGO_CALL::getValueByReference < StartIterator > ( ALGO_CALL::stepOverDeduced ( ALGO_CALL::deduceRange ( f, l ), ALGO_CALL::deduceRange ( o ), op ) ) ;
}

template < typename I, typename N, typename O, typename StepOperation >
ALGO_INLINE
O stepCounted ( I f, N times, O o, StepOperation op )
{
    return ALGO_CALL::getValueByReference < StartIterator > ( ALGO_CALL::stepOverDeduced ( ALGO_CALL::deduceRange ( f, times ), ALGO_CALL::deduceRange ( o ), op ) ) ;
}
    
    
template < typename I, typename O, typename StepOperation ALGO_COMMA_ENABLE_IF_PARAM >
struct StepOver
{
    ALGO_INLINE
    static O apply ( I from, I to, O o, StepOperation op )
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
    return ALGO_CALL::StepOver < I, O, StepOperation >::apply ( from, to, o, op ) ;
}
    
    
    
template < typename I, typename N, typename O, typename StepOperation ALGO_COMMA_ENABLE_IF_PARAM >
struct StepCounted
{
    ALGO_INLINE
    O apply ( I from, N times, O o, StepOperation op ) const
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
    return ALGO_CALL::StepCounted < I, N, O, StepOperation >::apply ( from, times, o, op ) ;
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
    static O apply ( I f, I l, O o )
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
    static O* apply ( I* f, I* l, O* o )
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
    
    return ALGO_CALL::unstripIter < O > ( ALGO_CALL::Copy < I, O >::apply ( ALGO_CALL::stripIter ( f )
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
    static O apply ( I f, I l, O o )
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
    static O* apply ( I* f, I* l, O* o )
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
    
    return ALGO_CALL::unstripIter < O > ( ALGO_CALL::CopyBackward < I, O >::apply ( ALGO_CALL::stripIter ( f )
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
    static void apply ( Iter f, Iter l, typename ALGO_CALL::IteratorTraits < Iter >::value_type const& value )
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
    static void apply ( T* f, T* l, T const value )
    {
        ptrdiff_t const toCopy = ALGO_CALL::distance ( f, l ) ;
        ALGO_ASSERT ( toCopy > 0 ) ;
        
        ALGO_CALL::copyBytesNotOverlapped ( f
                                        , &value
                                        , 1 ) ;
        ptrdiff_t copied = 1 ;
        
        while ( copied * 2 < toCopy )
        {
            ALGO_CALL::copyBytesNotOverlapped ( ALGO_CALL::advance ( f, copied, ALGO_CALL::ByReturnValue () )
                                               , f
                                               , copied ) ;
            copied *= 2 ;
        }
        
        if ( copied != toCopy )
        {
            ALGO_ASSERT ( toCopy > copied ) ;
            
            ALGO_CALL::copyBytesNotOverlapped ( ALGO_CALL::advance ( f, copied, ALGO_CALL::ByReturnValue () )
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
    
    ALGO_CALL::Fill < Iter >::apply ( ALGO_CALL::stripIter ( f )
                                 , ALGO_CALL::stripIter ( l )
                                 , value ) ;
}
    
    

struct RotateNoChoice {} ;
struct RotateBlocks : RotateNoChoice {} ;
    
    
    
template < typename Iter, typename Choice ALGO_COMMA_ENABLE_IF_PARAM >
struct RotateRightByOne
{
    ALGO_INLINE
    static void apply ( Iter f, Iter m )
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
    static void apply ( T* f, T* m )
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
        
    return ALGO_CALL::RotateRightByOne < Iter, Choice >::apply ( f, m ) ;
}
  
    
    
template < typename Iter, typename Choice ALGO_COMMA_ENABLE_IF_PARAM >
struct RotateLeftByOne
{
    ALGO_INLINE
    static void apply ( Iter f, Iter m )
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
    static void apply ( T* f, T* m )
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
        
    return ALGO_CALL::RotateLeftByOne < Iter, Choice >::apply ( f, m ) ;
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
              , ALGO_CALL::advance ( returnValue.ptr, returnValue.size, ALGO_CALL::ByReturnValue () )
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
              , ALGO_CALL::advance ( returnValue.ptr, returnValue.size, ALGO_CALL::ByReturnValue () )
              , 0 ) ;
        return returnValue ;
    }
} ;
    
    
    
} // namespace algo


#endif
