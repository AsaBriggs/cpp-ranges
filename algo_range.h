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
    
    
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsARange : ALGO_CALL::HasProperty < ALGO_CALL::StartIterator, PropertySet >::type
    {} ;
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct StartIteratorType
    {
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, PropertySet >::type type ;
    } ;
    
    // Moved IsARange & StartIteratorType up the top to allow definition of IteratorTraits partial specialisation
    // that can be used by other metafunctions/functions.
    template < typename PropertySet >
    struct IteratorTraits < PropertySet
        , typename std::enable_if < ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    : ALGO_CALL::IteratorTraits < typename StartIteratorType < PropertySet >::type >
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
    
    
    
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct BoundedRange : ALGO_CALL::logic::or_ <
        ALGO_CALL::HasProperty < ALGO_CALL::EndIterator, PropertySet > >
    {} ;
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct CountedRange : ALGO_CALL::logic::or_ <
        ALGO_CALL::HasProperty < ALGO_CALL::Count, PropertySet > >
    {} ;
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct FiniteRange : ALGO_CALL::logic::or_ <
        ALGO_CALL::BoundedRange < PropertySet >
        , ALGO_CALL::CountedRange < PropertySet > >
    {} ;
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct EndIteratorType
    {
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::EndIterator, PropertySet >::type type ;
    } ;
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct CountType
    {
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::Count, PropertySet >::type type ;
    } ;
    
    
    template < typename PropertySet, typename N >
    ALGO_INLINE
    typename std::enable_if < ALGO_CALL::CountedRange < PropertySet >::type::value, void >::type
    modifyCount ( PropertySet& x, N n, ALGO_CALL::InPlace )
    {
        ALGO_STATIC_ASSERT ( (typename ALGO_CALL::IsARange < PropertySet >::type ()), "" ) ;
        
        ALGO_CALL::getValueByReference < Count > ( x ) += typename CountType < PropertySet >::type ( n ) ;
    }
    
    template < typename PropertySet, typename N >
    ALGO_INLINE
    typename std::enable_if < !ALGO_CALL::CountedRange < PropertySet >::type::value, void >::type
    modifyCount ( PropertySet& x, N n, ALGO_CALL::InPlace )
    {
        ALGO_STATIC_ASSERT ( (typename ALGO_CALL::IsARange < PropertySet >::type ()), "" ) ;
        // No count to modify
    }
    
    
    
    template < typename PropertySet >
    struct Predecessor < PropertySet
        , typename std::enable_if < ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        void operator () ( PropertySet& x ) const
        {
            ALGO_CALL::predecessor ( ALGO_CALL::getValueByReference < StartIterator > ( x ), ALGO_CALL::InPlace () ) ;
            ALGO_CALL::modifyCount ( x, 1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
    
    
    template < typename PropertySet >
    struct Successor < PropertySet
        , typename std::enable_if < ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        void operator () ( PropertySet& x ) const
        {
            ALGO_CALL::successor ( ALGO_CALL::getValueByReference < StartIterator > ( x ), ALGO_CALL::InPlace () ) ;
            ALGO_CALL::modifyCount ( x, -1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
    
    
    template < typename PropertySet >
    struct Distance < PropertySet
        , typename std::enable_if < ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < PropertySet >::difference_type operator () ( PropertySet x, PropertySet y ) const
        {
            return ALGO_CALL::distance ( ALGO_CALL::getValueByReference < StartIterator > ( x )
                                        , ALGO_CALL::getValueByReference < StartIterator > ( y ) ) ;
        }
    } ;
    
    
    
    template < typename PropertySet >
    struct Advance < PropertySet
        , typename std::enable_if < ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        void operator () ( PropertySet& x, typename ALGO_CALL::IteratorTraits < PropertySet >::difference_type n ) const
        {
            ALGO_CALL::advance ( ALGO_CALL::getValueByReference < StartIterator > ( x ), n, ALGO_CALL::InPlace () ) ;
            ALGO_CALL::modifyCount ( x, -n, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
    
    
    template < typename PropertySet >
    struct Deref < PropertySet
        , typename std::enable_if < ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < PropertySet >::reference operator () ( PropertySet x ) const
        {
            return ALGO_CALL::deref ( ALGO_CALL::getValueByReference < ALGO_CALL::StartIterator > ( x ) ) ;
        }
    } ;
    
    
    template < typename PropertySet >
    struct IsEmpty < PropertySet
        , typename std::enable_if < ALGO_CALL::CountedRange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        bool operator () ( PropertySet x ) const
        {
            return typename CountType < PropertySet >::type ( 0 ) == ALGO_CALL::getValue < ALGO_CALL::Count > ( x ) ;
        }
    } ;
    
    template < typename PropertySet >
    struct IsEmpty < PropertySet
        , typename std::enable_if < !ALGO_CALL::CountedRange < PropertySet >::type::value
            && ALGO_CALL::BoundedRange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        bool operator () ( PropertySet x ) const
        {
            return ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( x ) == ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( x ) ;
        }
    } ;
    
    template < typename PropertySet >
    struct IsEmpty < PropertySet
        , typename std::enable_if < !ALGO_CALL::FiniteRange < PropertySet >::type::value
            && ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        bool operator () ( PropertySet x ) const
        {
            return false ;
        }
    } ;
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct HasCountO1Time : std::integral_constant < bool
        , ALGO_CALL::CountedRange < PropertySet >::type::value
            || ( ALGO_CALL::BoundedRange < PropertySet >::type::value
                && std::is_convertible < typename ALGO_CALL::IteratorTraits < PropertySet >::iterator_category
                    , std::random_access_iterator_tag >::type::value ) >
    {} ;
    
    template < typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct CountO1Time
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < PropertySet >::difference_type operator () ( PropertySet x ) const
        {
            return -1 ;
        }
    };
    
    template < typename PropertySet >
    struct CountO1Time < PropertySet, std::enable_if < ALGO_CALL::CountedRange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT > >
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < PropertySet >::difference_type operator () ( PropertySet x ) const
        {
            return ALGO_CALL::getValue< ALGO_CALL::Count > ( x ) ;
        }
    } ;
    
    template < typename PropertySet >
    struct CountO1Time < PropertySet, std::enable_if < !ALGO_CALL::CountedRange < PropertySet >::type::value
            && ALGO_CALL::HasCountO1Time < PropertySet >::type::value
        , ALGO_ENABLE_IF_PARAM_DEFAULT > >
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < PropertySet >::difference_type operator () ( PropertySet x ) const
        {
            return ALGO_CALL::distance ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( x )
                                        , ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( x ) ) ;
        }
    } ;
    
    template < typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < PropertySet >::difference_type
    countO1Time ( PropertySet x )
    {
        return CountO1Time < PropertySet > () ( x ) ;
    }
    
    
    
    template < typename FirstArgument ALGO_COMMA_ENABLE_IF_PARAM >
    struct DeduceRange1 ;
    
    template < typename FirstArgument >
    struct DeduceRange1 < FirstArgument, typename std::enable_if < ALGO_CALL::IsARange < FirstArgument >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef FirstArgument type ;
        
        ALGO_INLINE
        type operator () ( FirstArgument x ) const
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
        type operator () ( FirstArgument x ) const
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
        type operator () ( T (&x)[ N ] ) const
        {
            type returnValue = { &x [ 0 ] , &x [ 0 ] + N } ;
            return returnValue ;
        }
    } ;
    
    template < typename FirstArgument >
    ALGO_INLINE
    typename DeduceRange1 < FirstArgument >::type deduceRange ( FirstArgument x )
    {
        return DeduceRange1 < FirstArgument > () ( x ) ;
    }
    
    template < typename T, ptrdiff_t N >
    ALGO_INLINE
    typename DeduceRange1 < T [ N ] >::type deduceRange ( T (&x) [ N ] )
    {
        return DeduceRange1 < T [ N ] > () ( x ) ;
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
        type operator () ( FirstArgument x, SecondArgument y ) const
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
        type operator () ( FirstArgument x, SecondArgument y ) const
        {
            type returnValue = { x, y } ;
            return returnValue ;
        }
    } ;
    
    template < typename FirstArgument, typename SecondArgument >
    ALGO_INLINE
    typename DeduceRange2 < FirstArgument, SecondArgument >::type deduceRange ( FirstArgument x, SecondArgument y )
    {
        return DeduceRange2 < FirstArgument, SecondArgument > () ( x, y ) ;
    }
    
    
    
    template < typename PropertySet, typename Pred >
    PropertySet find_if ( PropertySet x, Pred p )
    {
        while ( !ALGO_CALL::isEmpty ( x )
               && !p ( ALGO_CALL::deref ( x ) ) )
        {
            successor ( x, ALGO_CALL::InPlace () ) ;
        }
        return x ;
    }
    
    
    
    template < typename PropertySet, typename Pred >
    PropertySet find_if_not ( PropertySet x, Pred p )
    {
        while ( !ALGO_CALL::isEmpty ( x )
               && p ( ALGO_CALL::deref ( x ) ) )
        {
            successor ( x, ALGO_CALL::InPlace () ) ;
        }
        return x ;
    }
    
    
    
    template < typename PropertySet, typename Op >
    Op for_each ( PropertySet x, Op op )
    {
        while ( !ALGO_CALL::isEmpty ( x ) )
        {
            op ( ALGO_CALL::deref ( x ) ) ;
            successor ( x, ALGO_CALL::InPlace () ) ;
        }
        return op ;
    }
    
    template < typename InputPropertySet, typename OutputPropertySet, typename Op >
    std::pair < OutputPropertySet, InputPropertySet >
    transform ( InputPropertySet x, OutputPropertySet y, Op op )
    {
        ALGO_STATIC_ASSERT ( (typename ALGO_CALL::FiniteRange < InputPropertySet >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet >::type ()), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x )
                && !ALGO_CALL::isEmpty ( y ) )
        {
            ALGO_CALL::assignImpl ( y, op ( ALGO_CALL::deref ( x ) ) ) ;
            
            ALGO_CALL::successor ( x, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( y, ALGO_CALL::InPlace () ) ;
        }
        return std::make_pair( y, x ) ;
    }
    
    template < typename InputPropertySet1, typename InputPropertySet2, typename OutputPropertySet, typename Op >
    std::pair < OutputPropertySet, std::pair < InputPropertySet1, InputPropertySet2 > >
    zip ( InputPropertySet1 x, InputPropertySet2 y, OutputPropertySet z, Op op )
    {
        ALGO_STATIC_ASSERT ( (typename ALGO_CALL::FiniteRange < InputPropertySet1 >::type ()
                       || typename ALGO_CALL::FiniteRange < InputPropertySet2 >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet >::type ()), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x ) && !ALGO_CALL::isEmpty ( y ) && !ALGO_CALL::isEmpty ( z ) )
        {
            ALGO_CALL::assignImpl ( z, op ( ALGO_CALL::deref ( x ), ALGO_CALL::deref ( y ) ) ) ;
            
            ALGO_CALL::successor ( x, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( y, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( z, ALGO_CALL::InPlace () ) ;
        }
        return std::make_pair ( z, std::make_pair ( x, y ) ) ;
    }
    
    template < typename OutputPropertySet1, typename OutputPropertySet2, typename T >
    void split ( OutputPropertySet1& x, OutputPropertySet2& y, T z )
    {
        ALGO_CALL::moveAssign ( &z.first, x ),
        ALGO_CALL::moveAssign ( &z.second, y ) ;
    }

    template < typename InputPropertySet, typename OutputPropertySet1, typename OutputPropertySet2, typename Op >
    std::pair < std::pair < OutputPropertySet1, OutputPropertySet2 >, InputPropertySet >
    unzip ( InputPropertySet x, OutputPropertySet1 y, OutputPropertySet2 z, Op op )
    {
        ALGO_STATIC_ASSERT ( (typename ALGO_CALL::FiniteRange < InputPropertySet >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet1 >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet2 >::type ()), "Infinite loop!" ) ;
        
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
