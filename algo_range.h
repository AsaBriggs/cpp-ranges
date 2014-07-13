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
    
    template < typename Iter >
    struct BasicBoundedRange
    {
        typedef ALGO_CALL::Compound < ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter >
            , ALGO_CALL::ValueAndProperty < ALGO_CALL::EndIterator, Iter > > type ;
    } ;
    
    template < typename Iter >
    struct BasicCountedRange
    {
        typedef ALGO_CALL::Compound <
            ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter >
            , ALGO_CALL::ValueAndProperty < ALGO_CALL::Count, typename ALGO_CALL::IteratorTraits < Iter >::difference_type > > type ;
    } ;
    
    template < typename Iter >
    struct BasicUnboundedRange
    {
        typedef ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter > type ;
    } ;
    
    
    
    template < typename PropertySet >
    struct IsARange : ALGO_CALL::HasProperty < ALGO_CALL::StartIterator, PropertySet >::type
    {} ;
    
    template < typename PropertySet >
    struct BoundedRange : ALGO_IMPL_CALL::or_ <
        ALGO_CALL::HasProperty < ALGO_CALL::EndIterator, PropertySet > >
    {} ;
    
    template < typename PropertySet >
    struct CountedRange : ALGO_IMPL_CALL::or_ <
        ALGO_CALL::HasProperty < ALGO_CALL::Count, PropertySet > >
    {} ;
    
    template < typename PropertySet >
    struct FiniteRange : ALGO_IMPL_CALL::or_ <
        ALGO_CALL::BoundedRange < PropertySet >
        , ALGO_CALL::CountedRange < PropertySet > >
    {} ;
    
    
    template < typename PropertySet >
    struct StartIteratorType
    {
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, PropertySet >::type type ;
    } ;
    
    template < typename PropertySet >
    struct EndIteratorType
    {
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::EndIterator, PropertySet >::type type ;
    } ;
    
    template < typename PropertySet >
    struct CountType
    {
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::Count, PropertySet >::type type ;
    } ;
    
    
    template < typename PropertySet >
    struct IteratorTraits < PropertySet
            , typename std::enable_if < ALGO_CALL::IsARange < PropertySet >::type::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        : ALGO_CALL::IteratorTraits < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, PropertySet >::type >
    {} ;
    
    
    template < typename PropertySet, typename N >
    ALGO_INLINE
    typename std::enable_if < ALGO_CALL::CountedRange < PropertySet >::type::value, void >::type
    modifyCount ( PropertySet& x, N n )
    {
        static_assert ( typename ALGO_CALL::IsARange < PropertySet >::type (), "" ) ;
        
        ALGO_CALL::getValueByReference < Count > ( x ) += typename CountType < PropertySet >::type ( n ) ;
    }
    
    template < typename PropertySet, typename N >
    ALGO_INLINE
    typename std::enable_if < !ALGO_CALL::CountedRange < PropertySet >::type::value, void >::type
    modifyCount ( PropertySet& x, N n )
    {
        static_assert ( typename ALGO_CALL::IsARange < PropertySet >::type (), "" ) ;
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
            ALGO_CALL::modifyCount ( x, 1 ) ;
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
            ALGO_CALL::modifyCount ( x, -1 ) ;
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
            ALGO_CALL::modifyCount ( x, -n ) ;
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
    ALGO_INLINE
    typename std::enable_if < ALGO_CALL::CountedRange < PropertySet >::type::value, bool >::type
    isEmpty ( PropertySet& x )
    {
        static_assert ( typename ALGO_CALL::IsARange < PropertySet >::type (), "" ) ;
        
        return typename CountType < PropertySet >::type ( 0 ) == ALGO_CALL::getValue < ALGO_CALL::Count > ( x ) ;
    }
    
    template < typename PropertySet >
    ALGO_INLINE
    typename std::enable_if < !ALGO_CALL::CountedRange < PropertySet >::type::value
                            && ALGO_CALL::BoundedRange < PropertySet >::type::value
        , bool >::type
    isEmpty ( PropertySet& x )
    {
        static_assert ( typename ALGO_CALL::IsARange < PropertySet >::type (), "" ) ;
        
        return ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( x ) == ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( x ) ;
    }
    
    
    template < typename PropertySet >
    constexpr
    ALGO_INLINE
    typename std::enable_if < !ALGO_CALL::FiniteRange < PropertySet >::type::value, bool >::type
    isEmpty ( PropertySet& x )
    {
        static_assert ( typename ALGO_CALL::IsARange < PropertySet >::type (), "" ) ;
        
        return false ;
    }
    
    
    template < typename PropertySet, typename Op >
    Op for_each ( PropertySet x, Op op )
    {
        while ( !ALGO_CALL::isEmpty ( x ) )
        {
            op ( ALGO_CALL::deref ( x ) ) ;
            successorRange ( x ) ;
        }
        return op ;
    }
    
    template < typename InputPropertySet, typename OutputPropertySet >
    std::pair < OutputPropertySet, InputPropertySet >
    copy ( InputPropertySet x, OutputPropertySet y )
    {
        static_assert ( typename ALGO_CALL::FiniteRange < InputPropertySet >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet >::type (), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x ) && !ALGO_CALL::isEmpty ( y ) )
        {
            ALGO_CALL::assign ( x, y ) ;
            
            ALGO_CALL::successor ( x )
            , ALGO_CALL::successor ( y ) ;
        }
        return { y, x } ;
    }
    
    template < typename InputPropertySet1, typename InputPropertySet2, typename OutputPropertySet, typename Op >
    std::pair < OutputPropertySet, std::pair < InputPropertySet1, InputPropertySet2 > >
    zip ( InputPropertySet1 x, InputPropertySet2 y, OutputPropertySet z, Op op )
    {
        static_assert ( typename ALGO_CALL::FiniteRange < InputPropertySet1 >::type ()
                       || typename ALGO_CALL::FiniteRange < InputPropertySet2 >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet >::type (), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x ) && !ALGO_CALL::isEmpty ( y ) && !ALGO_CALL::isEmpty ( z ) )
        {
            ALGO_CALL::deref ( z ) = op ( ALGO_CALL::deref ( x ), ALGO_CALL::deref ( y ) ) ;
            
            ALGO_CALL::successor ( x )
            , ALGO_CALL::successor ( y )
            , ALGO_CALL::successor ( z ) ;
        }
        return { z, { x, y } } ;
    }
    
    template < typename OutputPropertySet1, typename OutputPropertySet2, typename T >
    void split ( OutputPropertySet1& x, OutputPropertySet2& y, T z )
    {
        ALGO_CALL::moveAssign ( &z.first, x ),
        ALGO_CALL::moveAssign ( &z.second, x ) ;
    }

    template < typename InputPropertySet, typename OutputPropertySet1, typename OutputPropertySet2, typename Op >
    std::pair < std::pair < OutputPropertySet1, OutputPropertySet2 >, InputPropertySet >
    unzip ( InputPropertySet x, OutputPropertySet1 y, OutputPropertySet2 z )
    {
        static_assert ( typename ALGO_CALL::FiniteRange < InputPropertySet >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet1 >::type ()
                       || typename ALGO_CALL::FiniteRange < OutputPropertySet2 >::type (), "Infinite loop!" ) ;
        
        while ( !ALGO_CALL::isEmpty ( x ) && !ALGO_CALL::isEmpty ( y ) && !ALGO_CALL::isEmpty ( z ) )
        {
            split ( y, z, op ( ALGO_CALL::deref ( x ) ) ) ;

            ALGO_CALL::successor ( x )
            , ALGO_CALL::successor ( y )
            , ALGO_CALL::successor ( z ) ;
        }
        return { { y, z }, x } ;
    }
} // namespace algo

#endif
