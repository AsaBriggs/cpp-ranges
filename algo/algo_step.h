#ifndef INCLUDED_ALGO_STEP
#define INCLUDED_ALGO_STEP

#include "algo_range.h"

namespace algo {
    
    template < template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM > class Op ALGO_COMMA_ENABLE_IF_PARAM >
    struct TransferOperatorWrapper
    {
        typedef TransferOperatorWrapper type ;
        
        // Must pass by reference to be transparent to Op
        template < typename I, typename O >
        ALGO_INLINE
        static void apply ( I& i, O& o )
        ALGO_NOEXCEPT_DECL ( noexcept ( Op < I, O >::apply ( i, o ) ) )
        {
            Op < I, O >::apply ( i, o ) ;
        }
    } ;
    
    namespace detail {
        
        // Define a series of tags to allow composition of stepping.
        
        struct Operation_tag {} ;
        
        struct pre_op_i_tag {} ;
        struct pre_op_o_tag {} ;
        
        struct post_op_i_tag {} ;
        struct post_op_o_tag {} ;
        
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
        
        struct DefaultDeduceStepOperationTag {} ;
        
        template < typename Range, typename Tag, typename DeductionTag ALGO_COMMA_ENABLE_IF_PARAM >
        struct DeduceStepOperation
        : ALGO_DETAIL_CALL::NoOp
        {} ;
        
        template < typename Range>
        struct DeduceStepOperation < Range
        , ALGO_DETAIL_CALL::pre_op_i_tag
        , ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
        template < typename Range >
        struct DeduceStepOperation < Range
        , ALGO_DETAIL_CALL::pre_op_o_tag
        , ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
        template < typename Range >
        struct DeduceStepOperation < Range
        , ALGO_DETAIL_CALL::post_op_i_tag
        , ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag
        , typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
        template < typename Range >
        struct DeduceStepOperation < Range
        , ALGO_DETAIL_CALL::post_op_o_tag
        , ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag
        , typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
        // Set this to true when the stepping operation decreases the length of each range by one.
        template < typename DeductionTag ALGO_COMMA_ENABLE_IF_PARAM >
        struct RegularDeduceStepOperation
        : ALGO_LOGIC_CALL::false_type
        {} ;
        
        template <>
        struct RegularDeduceStepOperation < ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag, ALGO_ENABLE_IF_PARAM_DEFAULT >
        : ALGO_LOGIC_CALL::true_type
        {} ;
        
    } // namespace detail
    
    // Unary operations (i.e successor/predecessor) are stateless
    template < typename DeduceStepOperationTag, typename I, typename O, typename Op >
    ALGO_INLINE
    void step ( I& i, O& o, Op op )
    {
        typedef typename ALGO_DETAIL_CALL::DeduceStepOperation < I, ALGO_DETAIL_CALL::pre_op_i_tag, DeduceStepOperationTag >::type pre_op_i_type ;
        
        typedef typename ALGO_DETAIL_CALL::DeduceStepOperation < O, ALGO_DETAIL_CALL::pre_op_o_tag, DeduceStepOperationTag >::type pre_op_o_type ;
        
        typedef typename ALGO_DETAIL_CALL::DeduceStepOperation < I, ALGO_DETAIL_CALL::post_op_i_tag, DeduceStepOperationTag >::type post_op_i_type ;
        
        typedef typename ALGO_DETAIL_CALL::DeduceStepOperation < O, ALGO_DETAIL_CALL::post_op_o_tag, DeduceStepOperationTag >::type post_op_o_type ;
        
        // ALGO_ASSERT ( !ALGO_CALL::isEmpty ( i ) ) ;
        // ALGO_ASSERT ( !ALGO_CALL::isEmpty ( o ) ) ;
        
        pre_op_i_type::apply ( i )
        , pre_op_o_type::apply ( o ) ;
        
        op.apply ( i, o ) ;
        
        post_op_i_type::apply ( i )
        , post_op_o_type::apply ( o ) ;
    }
    
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag ALGO_COMMA_ENABLE_IF_PARAM >
    struct StepOverRangeCheckingIsEmpty
    {
        typedef StepOverRangeCheckingIsEmpty type ;
        
        typedef std::pair < OutputRange, InputRange > ReturnType ;
        
        ALGO_STATIC_ASSERT_IS_RANGE ( InputRange ) ;
        ALGO_STATIC_ASSERT_IS_RANGE ( OutputRange ) ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < InputRange >::type::value
                              || ALGO_CALL::IsAFiniteRange < OutputRange >::type::value), "Infinite loop!" ) ;
        
        ALGO_INLINE
        static ReturnType apply ( InputRange from, OutputRange to, StepOperation op )
        {
            while ( !ALGO_CALL::IsEmpty < InputRange >::apply ( from )
                   && !ALGO_CALL::IsEmpty < OutputRange >::apply ( to ) )
            {
                ALGO_CALL::step < DeduceStepOperationTag > ( from, to, op ) ;
            }
            return std::make_pair ( to, from ) ;
        }
    } ;
    
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag ALGO_COMMA_ENABLE_IF_PARAM >
    struct StepOverCounted
    {
        typedef StepOverCounted type ;
        
        typedef std::pair < OutputRange, InputRange > ReturnType ;
        
        template < typename N >
        ALGO_INLINE
        static ReturnType apply ( InputRange from, OutputRange to, N n, StepOperation op )
        {
            while ( n-- )
            {
                ALGO_CALL::step < DeduceStepOperationTag > ( from, to, op ) ;
            }
            
            return std::make_pair ( to, from ) ;
        }
    } ;
    
    namespace detail {
        
        template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
        struct IsSuitableForStepOverCounted
        : ALGO_LOGIC_CALL::or_ <
        ALGO_CALL::HasCountO1Time < Range >
        , ALGO_CALL::IsANonFiniteRange < Range >
        >
        {} ;
        
        template < typename Range1, typename Range2 >
        ptrdiff_t getMinRangeLength ( Range1 const& x, Range2 const& y )
        {
            ALGO_STATIC_ASSERT ( (ALGO_DETAIL_CALL::IsSuitableForStepOverCounted < Range1 >::type::value), "Must be a suitable range" ) ;
            ALGO_STATIC_ASSERT ( (ALGO_DETAIL_CALL::IsSuitableForStepOverCounted < Range2 >::type::value), "Must be a suitable range" ) ;
            
            ALGO_STATIC_ASSERT ( (ALGO_LOGIC_CALL::or_ <
                                  ALGO_CALL::IsAFiniteRange < Range1 >
                                  , ALGO_CALL::IsAFiniteRange < Range2 > >::type::value), "Infinite loop!" ) ;
            
            ptrdiff_t const size1 = ALGO_CALL::countO1Time ( x ) ;
            ptrdiff_t const size2 = ALGO_CALL::countO1Time ( y ) ;
            
            if ( ALGO_DETAIL_CALL::UNABLE_TO_DETERMINE_SIZE == size1 )
            {
                ALGO_ASSERT ( ALGO_DETAIL_CALL::UNABLE_TO_DETERMINE_SIZE != size2 ) ;
                ALGO_ASSERT ( size2 >= 0 ) ;
                return size2 ;
            }
            else if ( ALGO_DETAIL_CALL::UNABLE_TO_DETERMINE_SIZE == size2 )
            {
                ALGO_ASSERT ( size1 >= 0 ) ;
                return size1 ;
            }
            else
            {
                ALGO_ASSERT ( size1 >= 0 ) ;
                ALGO_ASSERT ( size2 >= 0 ) ;
                return std::min ( size1, size2 ) ;
            }
        }
        
    } // namespace detail
    
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag ALGO_COMMA_ENABLE_IF_PARAM >
    struct StepOverRange ;
    
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag >
    struct StepOverRange < InputRange, OutputRange, StepOperation, DeduceStepOperationTag
    , typename ALGO_LOGIC_CALL::enable_if_pred <
    ALGO_LOGIC_CALL::and_ <
    ALGO_DETAIL_CALL::RegularDeduceStepOperation < DeduceStepOperationTag >
    , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsSuitableForStepOverCounted < InputRange, OutputRange > > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef StepOverRange type ;
        
        typedef std::pair < OutputRange, InputRange > ReturnType ;
        
        ALGO_INLINE
        static ReturnType apply ( InputRange from, OutputRange to, StepOperation op )
        {
            return ALGO_CALL::StepOverRangeCheckingIsEmpty < InputRange, OutputRange, StepOperation, DeduceStepOperationTag >::apply ( from, to, op ) ;
        }
    } ;
    
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag >
    struct StepOverRange < InputRange, OutputRange, StepOperation, DeduceStepOperationTag
    , typename ALGO_LOGIC_CALL::enable_if_pred <
    ALGO_LOGIC_CALL::and_ <
    ALGO_DETAIL_CALL::RegularDeduceStepOperation < DeduceStepOperationTag >
    , ALGO_DETAIL_CALL::IsSuitableForStepOverCounted < InputRange, OutputRange > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef StepOverRange type ;
        
        typedef std::pair < OutputRange, InputRange > ReturnType ;
        
        ALGO_INLINE
        static ReturnType apply ( InputRange from, OutputRange to, StepOperation op )
        {
            return ALGO_CALL::StepOverCounted < InputRange, OutputRange, StepOperation, DeduceStepOperationTag >::apply ( from, to, ALGO_DETAIL_CALL::getMinRangeLength ( from, to ), op ) ;
        }
    } ;
    
    // Note that in the case of DeduceStepOperationTag not being ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag
    // we can't assume that the stepping actually "uses up" one element per range per step.
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag >
    struct StepOverRange < InputRange, OutputRange, StepOperation, DeduceStepOperationTag
    , typename ALGO_LOGIC_CALL::enable_if_pred <
    ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::RegularDeduceStepOperation < DeduceStepOperationTag > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef StepOverRange type ;
        
        typedef std::pair < OutputRange, InputRange > ReturnType ;
        
        ALGO_INLINE
        static ReturnType apply ( InputRange from, OutputRange to, StepOperation op )
        {
            return ALGO_CALL::StepOverRangeCheckingIsEmpty < InputRange, OutputRange, StepOperation, DeduceStepOperationTag >::apply ( from, to, op ) ;
        }
    } ;
    
    template < typename InputRange, typename OutputRange, typename StepOperation >
    ALGO_INLINE
    OutputRange stepOverDeduced ( InputRange x, OutputRange y, StepOperation op )
    {
        return ALGO_CALL::StepOverRange < InputRange, OutputRange, StepOperation, ALGO_DETAIL_CALL::DefaultDeduceStepOperationTag >::apply ( x, y, op ).first ;
    }
    
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag >
    ALGO_INLINE
    OutputRange stepOverDeduced ( InputRange x, OutputRange y, StepOperation op, DeduceStepOperationTag )
    {
        return ALGO_CALL::StepOverRange < InputRange, OutputRange, StepOperation, DeduceStepOperationTag >::apply ( x, y, op ).first ;
    }
    
    template < typename I, typename O, typename StepOperation >
    ALGO_INLINE
    O stepOver ( I from, I to, O o, StepOperation op )
    {
        return ALGO_CALL::convertRangeToIterator < O >
        ( ALGO_CALL::stepOverDeduced ( ALGO_CALL::deduceRange ( from, to ), ALGO_CALL::deduceRange ( o ), op  ) ) ;
    }
    
    template < typename I, typename N, typename O, typename StepOperation >
    ALGO_INLINE
    O stepCounted ( I from, N times, O o, StepOperation op )
    {
        return ALGO_CALL::convertRangeToIterator < O >
        ( ALGO_CALL::stepOverDeduced ( ALGO_CALL::deduceRange ( from, times ), ALGO_CALL::deduceRange ( o ), op ) ) ;
    }
    
} // namespace algo

#endif
