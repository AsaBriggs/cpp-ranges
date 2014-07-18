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

// Unary operations (i.e successor/predecessor) are stateless
template < typename Pre_op_i, typename Pre_op_o, typename Op, typename Post_op_i, typename Post_op_o, typename I, typename O >
ALGO_INLINE
void stepStatelessOperations ( I& i, O& o )
{
    Pre_op_i::apply ( i )
    , Pre_op_o::apply ( o ) ;
    
    Op::apply ( i, o ) ;
    
    Post_op_i::apply ( i )
    , Post_op_o::apply ( o ) ;
}
    
template < template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM > class Op ALGO_COMMA_ENABLE_IF_PARAM >
struct TransferOperatorWrapper
{
    // Must pass by reference to be transparent to Op
    template < typename I, typename O >
    ALGO_INLINE
    static void apply ( I& i, O& o )
        ALGO_NOEXCEPT_DECL ( noexcept ( Op < I, O >::apply ( i, o ) ) )
    {
        Op < I, O >::apply ( i, o ) ;
    }
} ;

struct NoOp
{
    typedef NoOp type ;
        
    template < typename T >
    ALGO_NO_OP_FUNCTION
    ALGO_INLINE
    static void apply ( T const& )
        ALGO_NOEXCEPT_DECL ( true )
    {}
};

namespace detail {
    
struct DefaultDeduceStepOperationTag {} ;
    
template < typename Range, typename Tag, typename DeductionTag ALGO_COMMA_ENABLE_IF_PARAM >
struct DeduceStepOperation : ALGO_CALL::NoOp
{} ;
    
template < typename Range, typename DeductionTag >
struct DeduceStepOperation < Range
    , ALGO_CALL::pre_op_i_tag
    , DeductionTag
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    typedef ALGO_CALL::Successor < Range > type ;
} ;
    
template < typename Range, typename DeductionTag >
struct DeduceStepOperation < Range
    , ALGO_CALL::pre_op_o_tag
    , DeductionTag
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    typedef ALGO_CALL::Successor < Range > type ;
} ;
    
template < typename Range, typename DeductionTag >
struct DeduceStepOperation < Range
    , ALGO_CALL::post_op_i_tag
    , DeductionTag
    , typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    typedef ALGO_CALL::Successor < Range > type ;
} ;
    
template < typename Range, typename DeductionTag >
struct DeduceStepOperation < Range
    , ALGO_CALL::post_op_o_tag
    , DeductionTag
    , typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    typedef ALGO_CALL::Successor < Range > type ;
} ;

} // namespace detail

template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag ALGO_COMMA_ENABLE_IF_PARAM >
struct StepOverRange
{
    typedef std::pair < OutputRange, InputRange > ReturnType ;
    
    ALGO_STATIC_ASSERT_IS_RANGE ( InputRange ) ;
    ALGO_STATIC_ASSERT_IS_RANGE ( OutputRange ) ;
    
    ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < InputRange >::type::value
                          || ALGO_CALL::IsAFiniteRange < OutputRange >::type::value), "Infinite loop!" ) ;
    
    ALGO_INLINE
    static ReturnType apply ( InputRange from, OutputRange to )
    {
        typedef typename ALGO_CALL::detail::DeduceStepOperation < InputRange, ALGO_CALL::pre_op_i_tag, DeduceStepOperationTag >::type pre_op_i_type ;
        
        typedef typename ALGO_CALL::detail::DeduceStepOperation < OutputRange, ALGO_CALL::pre_op_o_tag, DeduceStepOperationTag >::type pre_op_o_type ;
        
        typedef typename ALGO_CALL::detail::DeduceStepOperation < InputRange, ALGO_CALL::post_op_i_tag, DeduceStepOperationTag >::type post_op_i_type ;
        
        typedef typename ALGO_CALL::detail::DeduceStepOperation < OutputRange, ALGO_CALL::post_op_o_tag, DeduceStepOperationTag >::type post_op_o_type ;

        while ( !ALGO_CALL::IsEmpty < InputRange >::apply ( from )
               && !ALGO_CALL::IsEmpty < OutputRange >::apply ( to ) )
        {
            ALGO_CALL::stepStatelessOperations < pre_op_i_type, pre_op_o_type, StepOperation, post_op_i_type, post_op_o_type > ( from, to ) ;
        }
        return std::make_pair ( to, from ) ;
    }
} ;
    
template < typename StepOperation, typename InputRange, typename OutputRange >
ALGO_INLINE
OutputRange stepOverDeduced ( InputRange x, OutputRange y )
{
    return ALGO_CALL::StepOverRange < InputRange, OutputRange, StepOperation, ALGO_CALL::detail::DefaultDeduceStepOperationTag >::apply ( x, y ).first ;
}

template < typename StepOperation, typename InputRange, typename OutputRange, typename DeduceStepOperationTag >
ALGO_INLINE
OutputRange stepOverDeduced ( InputRange x, OutputRange y, DeduceStepOperationTag )
{
    return ALGO_CALL::StepOverRange < InputRange, OutputRange, StepOperation, DeduceStepOperationTag >::apply ( x, y ).first ;
}
    
template < typename StepOperation, typename I, typename O >
ALGO_INLINE
O stepOver ( I from, I to, O o )
{
    return ALGO_CALL::getValue < ALGO_CALL::StartIterator >
        ( ALGO_CALL::stepOverDeduced < StepOperation > ( ALGO_CALL::deduceRange ( from, to ), ALGO_CALL::deduceRange ( o )  ) ) ;
}
    
template < typename StepOperation, typename I, typename N, typename O >
ALGO_INLINE
O stepCounted ( I from, N times, O o )
{
    return ALGO_CALL::getValue < ALGO_CALL::StartIterator >
        ( ALGO_CALL::stepOverDeduced < StepOperation > ( ALGO_CALL::deduceRange ( from, times ), ALGO_CALL::deduceRange ( o )  ) ) ;
}
    
    
    
template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
struct Copy
{
    ALGO_INLINE
    static O apply ( I f, I l, O o )
    {
        return ALGO_CALL::getValue < ALGO_CALL::StartIterator >
            ( ALGO_CALL::stepOverDeduced < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::Assign > >
                ( ALGO_CALL::deduceRange ( f, l ), ALGO_CALL::deduceRange ( o ) ) ) ;
    }
} ;

template < typename I, typename O >
struct Copy <
    I*
    , O*
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
        std::is_same < typename std::remove_cv<I>::type, O >,
        ALGO_CALL::IsBitwiseCopyable < O > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    static O* apply ( I* f, I* l, O* o )
    {
        ptrdiff_t const diff = ALGO_CALL::distance ( f, l ) ;
        ALGO_ASSERT ( diff >= 0 ) ;
        
        if ( ALGO_LIKELIHOOD ( !ALGO_CALL::equalUnderlyingAddress ( o, f ), true ) )
        {
            ALGO_CALL::copyBytesOverlapped ( f
                                            , o
                                            , diff ) ;
        }
        
        return ALGO_CALL::advance ( o, diff, ALGO_CALL::ByReturnValue () ) ;
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
    ALGO_INLINE
    static O apply ( I f, I l, O o )
    {
        return ALGO_CALL::getValue < ALGO_CALL::StartIterator >
            ( ALGO_CALL::stepOverDeduced < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::Assign > >
                ( ALGO_CALL::reverseRange ( ALGO_CALL::deduceRange ( f, l ) ), ALGO_CALL::deduceReversedUnboundedRange ( o ) ) ) ;
    }
} ;

template < typename I, typename O >
struct CopyBackward <
    I*
    , O*
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
        std::is_same < typename std::remove_cv<I>::type, O >,
        ALGO_CALL::IsBitwiseCopyable < O > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    static O* apply ( I* f, I* l, O* o )
    {
        ptrdiff_t const diff = ALGO_CALL::distance ( f, l ) ;
        ALGO_ASSERT ( diff >= 0 ) ;
        
        O* const tmp = ALGO_CALL::advance ( o, -diff, ALGO_CALL::ByReturnValue () ) ;
        
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

namespace detail {
        
struct FillInputOperationDeductionTag {} ;
    
template < typename Range >
struct DeduceStepOperation < Range, ALGO_CALL::pre_op_i_tag, ALGO_CALL::detail::FillInputOperationDeductionTag, ALGO_ENABLE_IF_PARAM_DEFAULT >
    : ALGO_CALL::NoOp
{} ;

} // namespace detail
    
    
template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
struct Fill
{
    // Taken a copy of value in passing by value, to ensure no aliasing issues occur.
    ALGO_INLINE
    static void apply ( Iter f, Iter l, typename ALGO_CALL::IteratorTraits < Iter >::value_type value )
    {
        ALGO_CALL::stepOverDeduced < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::Assign > >
             ( ALGO_CALL::deduceRange ( &value ), ALGO_CALL::deduceRange ( f, l ), ALGO_CALL::detail::FillInputOperationDeductionTag () ) ;
    }
} ;
    
    
    
template < typename T >
struct Fill <
    T*
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsBitwiseCopyable < T >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    static void apply ( T* f, T* l, T const value )
    {
        ptrdiff_t const toCopy = ALGO_CALL::distance ( f, l ) ;
        ALGO_ASSERT ( toCopy > 0 ) ;
        
        ALGO_CALL::copyBytesNotOverlapped ( &value
                                        , f
                                        , 1 ) ;
        ptrdiff_t copied = 1 ;
        
        while ( copied * 2 < toCopy )
        {
            ALGO_CALL::copyBytesNotOverlapped ( f
                                               , ALGO_CALL::advance ( f, copied, ALGO_CALL::ByReturnValue () )
                                               , copied ) ;
            copied *= 2 ;
        }
        
        if ( copied != toCopy )
        {
            ALGO_ASSERT ( toCopy > copied ) ;
            
            ALGO_CALL::copyBytesNotOverlapped ( f
                                               , ALGO_CALL::advance ( f, copied, ALGO_CALL::ByReturnValue () )
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
            Iter pred = ALGO_CALL::predecessor ( m, ALGO_CALL::ByReturnValue () ) ;
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
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsBitwiseCopyable < T >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    static void apply ( T* f, T* m )
    {
        // No alignment requirements as this is just a temporary buffer where the value is not "used".
        char buffer [ sizeof ( T ) ] ;
            
        ALGO_CALL::copyBytesNotOverlapped ( m, reinterpret_cast < T* > ( buffer ), 1 ) ;
            
        ALGO_CALL::copyBytesOverlapped ( f, f + 1, m - f ) ;
            
        ALGO_CALL::copyBytesNotOverlapped ( reinterpret_cast < T* > ( buffer ), f, 1 ) ;
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
            Iter succ = ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ) ;
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
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsBitwiseCopyable < T >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
{
    ALGO_INLINE
    static void apply ( T* f, T* m )
    {
        // No alignment requirements as this is just a temporary buffer where the value is not "used".
        char buffer [ sizeof ( T ) ] ;
            
        ALGO_CALL::copyBytesNotOverlapped ( f, reinterpret_cast < T* > ( buffer ), 1 ) ;
            
        ALGO_CALL::copyBytesOverlapped ( f + 1, f, m - f ) ;
            
        ALGO_CALL::copyBytesNotOverlapped ( reinterpret_cast < T* > ( buffer ), m, 1 ) ;
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
