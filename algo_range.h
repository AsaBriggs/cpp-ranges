#ifndef INCLUDED_ALGO_RANGE
#define INCLUDED_ALGO_RANGE

#ifndef INCLUDED_ALGO_PROPERTY
#include "algo_property.h"
#endif

#ifndef INCLUDED_ALGO_ITERATOR
#include "algo_iterator.h"
#endif


namespace algo
{
    // Compile-time Predicates Range -> bool
    // Readable
    // Writeable
    // Repeatable
    // Finite
    
    // Run-time Predicates Range -> bool
    // IsEmpty () when not Finite then always false, otherwise query the range.
    
    
    // SplitRange () Range -> Range x Range
    // Start Range () -> Iterator
    // End (only if constant time) Range -> Iterator
    // Advance start () Range -> Range
    
    
    
    // Bounded range
    // counted
    // predicated
    // unbounded
    
    struct StartIterator {} ;
    struct EndIterator {} ;
    struct Count {} ;
    
    // Leave predicate to a later problem ... the predicate states whether the current item is the last in the range
    // which hints at more state being required.
    // struct EndPredicate {} ;
    
    
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsARange : ALGO_CALL::HasProperty < ALGO_CALL::StartIterator, Range >::type
    {} ;
    
#define ALGO_STATIC_ASSERT_IS_RANGE( x ) ALGO_STATIC_ASSERT ( (ALGO_CALL::IsARange < x >::type::value), "Must be a Range" )
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct StartIteratorType
    {
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type type ;
    } ;
    
    // Moved IsARange & StartIteratorType up the top to allow definition of IteratorTraits partial specialisation
    // that can be used by other metafunctions/functions.
    template < typename Range >
    struct IteratorTraits < Range
        , typename std::enable_if < ALGO_CALL::IsARange < Range >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    : ALGO_CALL::IteratorTraits < typename StartIteratorType < Range >::type >
    {} ;
    
    
    
    template < typename Iter, typename EndIter = Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct BasicBoundedRange
    {
        typedef ALGO_CALL::Compound < ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter >
            , ALGO_CALL::ValueAndProperty < ALGO_CALL::EndIterator, EndIter > > type ;
    } ;
    
    template < typename Iter, typename DifferenceType = typename ALGO_CALL::IteratorTraits < Iter >::difference_type ALGO_COMMA_ENABLE_IF_PARAM >
    struct BasicCountedRange
    {
        typedef ALGO_CALL::Compound <
            ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter >
            , ALGO_CALL::ValueAndProperty < ALGO_CALL::Count, DifferenceType > > type ;
    } ;
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct BasicUnboundedRange
    {
        typedef ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter > type ;
    } ;
    
    
    
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsABoundedRange : ALGO_CALL::logic::and_ <
        ALGO_CALL::IsARange < Range >
        , ALGO_CALL::HasProperty < ALGO_CALL::EndIterator, Range > >
    {} ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsACountedRange : ALGO_CALL::logic::and_ <
        ALGO_CALL::IsARange < Range >
        ,ALGO_CALL::HasProperty < ALGO_CALL::Count, Range > >
    {} ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsAFiniteRange : ALGO_CALL::logic::or_ <
        ALGO_CALL::IsABoundedRange < Range >
        , ALGO_CALL::IsACountedRange < Range > >
    {} ;
    
    template < typename BoundedRange ALGO_COMMA_ENABLE_IF_PARAM >
    struct EndIteratorType
    {
        ALGO_STATIC_ASSERT ( (IsABoundedRange < BoundedRange >::type::value), "Must be a bounded range " ) ;
        
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::EndIterator, BoundedRange >::type type ;
    } ;
    
    template < typename CountedRange ALGO_COMMA_ENABLE_IF_PARAM >
    struct CountType
    {
        ALGO_STATIC_ASSERT ( (IsACountedRange < CountedRange >::type::value), "Must be a counted range " ) ;
        
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::Count, CountedRange >::type type ;
    } ;
    
    
    template < typename CountedRange, typename N >
    ALGO_INLINE
    typename std::enable_if < ALGO_CALL::IsACountedRange < CountedRange >::type::value, void >::type
    modifyCount ( CountedRange& x, N n, ALGO_CALL::InPlace )
    {
        ALGO_STATIC_ASSERT ( (IsACountedRange < CountedRange >::type::value), "Must be a counted range " ) ;
        
        ALGO_CALL::getValueByReference < Count > ( x ) += typename CountType < CountedRange >::type ( n ) ;
    }
    
    template < typename NotACountedRange, typename N >
    ALGO_INLINE
    typename std::enable_if < !ALGO_CALL::IsACountedRange < NotACountedRange >::type::value, void >::type
    modifyCount ( NotACountedRange& x, N n, ALGO_CALL::InPlace )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( NotACountedRange );
        // No count to modify
    }
    
    
    
    template < typename Range >
    struct Predecessor < Range
        , typename std::enable_if < ALGO_CALL::IsARange < Range >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ALGO_CALL::predecessor ( ALGO_CALL::getValueByReference < StartIterator > ( x ), ALGO_CALL::InPlace () ) ;
            ALGO_CALL::modifyCount ( x, 1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
    
    
    template < typename Range >
    struct Successor < Range
        , typename std::enable_if < ALGO_CALL::IsARange < Range >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ALGO_CALL::successor ( ALGO_CALL::getValueByReference < StartIterator > ( x ), ALGO_CALL::InPlace () ) ;
            ALGO_CALL::modifyCount ( x, -1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
    
    
    template < typename Range >
    struct Distance < Range
        , typename std::enable_if < ALGO_CALL::IsARange < Range >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Range >::difference_type apply ( Range x, Range y )
        {
            return ALGO_CALL::distance ( ALGO_CALL::getValueByReference < StartIterator > ( x )
                                        , ALGO_CALL::getValueByReference < StartIterator > ( y ) ) ;
        }
    } ;
    
    
    
    template < typename Range >
    struct Advance < Range
        , typename std::enable_if < ALGO_CALL::IsARange < Range >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static void apply ( Range& x, typename ALGO_CALL::IteratorTraits < Range >::difference_type n )
        {
            ALGO_CALL::advance ( ALGO_CALL::getValueByReference < StartIterator > ( x ), n, ALGO_CALL::InPlace () ) ;
            ALGO_CALL::modifyCount ( x, -n, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
    
    
    template < typename Range >
    struct Deref < Range
        , typename std::enable_if < ALGO_CALL::IsARange < Range >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Range >::reference apply ( Range x )
        {
            return ALGO_CALL::deref ( ALGO_CALL::getValueByReference < ALGO_CALL::StartIterator > ( x ) ) ;
        }
    } ;
    
    
    template < typename CountedRange >
    struct IsEmpty < CountedRange
        , typename std::enable_if < ALGO_CALL::IsACountedRange < CountedRange >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static bool apply ( CountedRange x )
        {
            return typename CountType < CountedRange >::type ( 0 ) == ALGO_CALL::getValue < ALGO_CALL::Count > ( x ) ;
        }
    } ;
    
    template < typename BoundedRange >
    struct IsEmpty < BoundedRange
        , typename std::enable_if < !ALGO_CALL::IsACountedRange < BoundedRange >::type::value
            && ALGO_CALL::IsABoundedRange < BoundedRange >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static bool apply ( BoundedRange x )
        {
            return ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( x ) == ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( x ) ;
        }
    } ;
    
    template < typename NonFiniteRange >
    struct IsEmpty < NonFiniteRange
        , typename std::enable_if < !ALGO_CALL::IsAFiniteRange < NonFiniteRange >::type::value
            && ALGO_CALL::IsARange < NonFiniteRange >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static bool apply ( NonFiniteRange )
        {
            return false ;
        }
    } ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct HasCountO1Time : std::integral_constant < bool
        , ALGO_CALL::IsACountedRange < Range >::type::value
            || ( ALGO_CALL::IsABoundedRange < Range >::type::value
                && std::is_convertible < typename ALGO_CALL::IteratorTraits < Range >::iterator_category
                    , std::random_access_iterator_tag >::type::value ) >
    {} ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct CountO1Time
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( Range ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Range >::difference_type apply ( Range x )
        {
            return -1 ;
        }
    };
    
    template < typename CountedRange >
    struct CountO1Time < CountedRange, std::enable_if < ALGO_CALL::IsACountedRange < CountedRange >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT > >
    {
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < CountedRange >::difference_type apply ( CountedRange x )
        {
            return ALGO_CALL::getValue< ALGO_CALL::Count > ( x ) ;
        }
    } ;
    
    template < typename RandomAccessBoundedRange >
    struct CountO1Time < RandomAccessBoundedRange, std::enable_if < !ALGO_CALL::IsACountedRange < RandomAccessBoundedRange >::type::value
            && ALGO_CALL::HasCountO1Time < RandomAccessBoundedRange >::type::value
        , ALGO_ENABLE_IF_PARAM_DEFAULT > >
    {
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < RandomAccessBoundedRange >::difference_type apply ( RandomAccessBoundedRange x )
        {
            return ALGO_CALL::distance ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( x )
                                        , ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( x ) ) ;
        }
    } ;
    
    template < typename Range >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < Range >::difference_type
    countO1Time ( Range x )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( Range ) ;
        
        return CountO1Time < Range >::apply ( x ) ;
    }
    
    
    
    template < typename FirstArgument ALGO_COMMA_ENABLE_IF_PARAM >
    struct DeduceRange1 ;
    
    template < typename FirstArgument >
    struct DeduceRange1 < FirstArgument, typename std::enable_if < ALGO_CALL::IsARange < FirstArgument >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef FirstArgument type ;
        
        ALGO_INLINE
        static type apply ( FirstArgument x )
        {
            return x ;
        }
    } ;
    
    template < typename FirstArgument >
    struct DeduceRange1 < FirstArgument, typename std::enable_if <
        !ALGO_CALL::IsARange < FirstArgument >::type::value
        && ALGO_CALL::HasIteratorTraits < FirstArgument >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef typename BasicUnboundedRange < FirstArgument >::type type ;
        
        ALGO_INLINE
        static type apply ( FirstArgument x )
        {
            type returnValue = { x } ;
            return returnValue ;
        }
    } ;
    
    template < typename T, ptrdiff_t N >
    struct DeduceRange1 < T [ N ], ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef typename BasicBoundedRange < T* >::type type ;
        
        ALGO_INLINE
        static type apply ( T (&x)[ N ] )
        {
            type returnValue = { &x [ 0 ] , &x [ 0 ] + N } ;
            return returnValue ;
        }
    } ;
    
    template < typename FirstArgument >
    ALGO_INLINE
    typename DeduceRange1 < FirstArgument >::type deduceRange ( FirstArgument x )
    {
        return DeduceRange1 < FirstArgument >::apply ( x ) ;
    }
    
    template < typename T, ptrdiff_t N >
    ALGO_INLINE
    typename DeduceRange1 < T [ N ] >::type deduceRange ( T (&x) [ N ] )
    {
        return DeduceRange1 < T [ N ] >::apply ( x ) ;
    }
    
    
    
    template < typename FirstArgument, typename SecondArgument ALGO_COMMA_ENABLE_IF_PARAM >
    struct DeduceRange2 ;
    
    // Should not have received a range. Declared to make an ambiguity at compile-time.
    template < typename FirstArgument, typename SecondArgument >
    struct DeduceRange2 < FirstArgument
        , SecondArgument
        , typename std::enable_if <
            ALGO_CALL::IsARange < FirstArgument >::type::value
            || ALGO_CALL::IsARange < SecondArgument >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type > ;
    
    
    template < typename FirstArgument, typename SecondArgument >
    struct DeduceRange2 < FirstArgument
        , SecondArgument
        , typename std::enable_if <
            ALGO_CALL::HasIteratorTraits < FirstArgument >::type::value
            && ALGO_CALL::HasIteratorTraits < SecondArgument >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef typename BasicBoundedRange < FirstArgument, SecondArgument >::type type ;
        
        ALGO_INLINE
        static type apply ( FirstArgument x, SecondArgument y )
        {
            type returnValue = { x, y } ;
            return returnValue ;
        }
    } ;
    
    template < typename FirstArgument, typename SecondArgument >
    struct DeduceRange2 < FirstArgument
        , SecondArgument
        , typename std::enable_if <
            ALGO_CALL::HasIteratorTraits < FirstArgument >::type::value
            && !ALGO_CALL::HasIteratorTraits < SecondArgument >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef typename BasicCountedRange < FirstArgument >::type type ;
        
        ALGO_INLINE
        static type apply ( FirstArgument x, SecondArgument y )
        {
            type returnValue = { x, y } ;
            return returnValue ;
        }
    } ;
    
    template < typename FirstArgument, typename SecondArgument >
    ALGO_INLINE
    typename DeduceRange2 < FirstArgument, SecondArgument >::type deduceRange ( FirstArgument x, SecondArgument y )
    {
        return DeduceRange2 < FirstArgument, SecondArgument >::apply ( x, y ) ;
    }
    
    
    
    template < typename Range, typename Pred >
    Range find_if ( Range x, Pred p )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( Range ) ;
        
        while ( !ALGO_CALL::isEmpty ( x )
               && !p ( ALGO_CALL::deref ( x ) ) )
        {
            successor ( x, ALGO_CALL::InPlace () ) ;
        }
        return x ;
    }
    
    /*
    // Dangerous! Need to take by reference to prevent array decay
    template < typename RangeArgument, typename Pred >
    typename DeduceRange1 < RangeArgument >::type find_if2 ( RangeArgument& x, Pred p )
    {
        return ALGO_CALL::find_if ( ALGO_CALL::deduceRange ( x ), p ) ;
    }
    */
    
    
    template < typename InputRange, typename Pred >
    InputRange find_if_not ( InputRange x, Pred p )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( InputRange ) ;
        
        while ( !ALGO_CALL::isEmpty ( x )
               && p ( ALGO_CALL::deref ( x ) ) )
        {
            successor ( x, ALGO_CALL::InPlace () ) ;
        }
        return x ;
    }
    
    
    
    template < typename InputRange, typename Op >
    Op for_each ( InputRange x, Op op )
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < InputRange >::type::value), "Infinite loop!" );
        
        while ( !ALGO_CALL::isEmpty ( x ) )
        {
            op ( ALGO_CALL::deref ( x ) ) ;
            successor ( x, ALGO_CALL::InPlace () ) ;
        }
        return op ;
    }
    
    template < typename InputRange, typename OutputRange, typename Op >
    std::pair < OutputRange, InputRange >
    transform ( InputRange x, OutputRange y, Op op )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( InputRange ) ;
        ALGO_STATIC_ASSERT_IS_RANGE ( OutputRange ) ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < InputRange >::type::value
                              || ALGO_CALL::IsAFiniteRange < OutputRange >::type::value), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x )
                && !ALGO_CALL::isEmpty ( y ) )
        {
            ALGO_CALL::assignImpl ( y, op ( ALGO_CALL::deref ( x ) ) ) ;
            
            ALGO_CALL::successor ( x, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( y, ALGO_CALL::InPlace () ) ;
        }
        return std::make_pair( y, x ) ;
    }
    
    template < typename InputRange1, typename InputRange2, typename OutputRange, typename Op >
    std::pair < OutputRange, std::pair < InputRange1, InputRange2 > >
    zip ( InputRange1 x, InputRange2 y, OutputRange z, Op op )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( InputRange1 ) ;
        ALGO_STATIC_ASSERT_IS_RANGE ( InputRange2 ) ;
        ALGO_STATIC_ASSERT_IS_RANGE ( OutputRange ) ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < InputRange1 >::type::value
                              || ALGO_CALL::IsAFiniteRange < InputRange2 >::type::value
                              || ALGO_CALL::IsAFiniteRange < OutputRange >::type::value), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x ) && !ALGO_CALL::isEmpty ( y ) && !ALGO_CALL::isEmpty ( z ) )
        {
            ALGO_CALL::assignImpl ( z, op ( ALGO_CALL::deref ( x ), ALGO_CALL::deref ( y ) ) ) ;
            
            ALGO_CALL::successor ( x, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( y, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( z, ALGO_CALL::InPlace () ) ;
        }
        return std::make_pair ( z, std::make_pair ( x, y ) ) ;
    }
    
    template < typename OutputRange1, typename OutputRange2, typename T >
    void split ( OutputRange1& x, OutputRange2& y, T z )
    {
        ALGO_CALL::moveAssign ( &z.first, x ),
        ALGO_CALL::moveAssign ( &z.second, y ) ;
    }

    template < typename InputRange, typename OutputRange1, typename OutputRange2, typename Op >
    std::pair < std::pair < OutputRange1, OutputRange2 >, InputRange >
    unzip ( InputRange x, OutputRange1 y, OutputRange2 z, Op op )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( InputRange ) ;
        ALGO_STATIC_ASSERT_IS_RANGE ( OutputRange1 ) ;
        ALGO_STATIC_ASSERT_IS_RANGE ( OutputRange2 ) ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::IsAFiniteRange < InputRange >::type::value
                              || ALGO_CALL::IsAFiniteRange < OutputRange1 >::type::value
                              || ALGO_CALL::IsAFiniteRange < OutputRange2 >::type::value), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x ) && !ALGO_CALL::isEmpty ( y ) && !ALGO_CALL::isEmpty ( z ) )
        {
            split ( y, z, op ( ALGO_CALL::deref ( x ) ) ) ;

            ALGO_CALL::successor ( x, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( y, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( z, ALGO_CALL::InPlace () ) ;
        }
        return std::make_pair ( std::make_pair ( y, z ), x ) ;
    }
} // namespace algo

#endif
