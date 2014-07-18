#ifndef INCLUDED_ALGO_RANGE
#define INCLUDED_ALGO_RANGE

#ifndef INCLUDED_ALGO_PROPERTY
#include "algo_property.h"
#endif

#ifndef INCLUDED_ALGO_ITERATOR
#include "algo_iterator.h"
#endif

#ifndef INCLUDED_ALGORITHM
#include <algorithm>
#define INCLUDED_ALGORITHM
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
    
    struct ReversedRange {} ;
    
    namespace detail {
        
    struct EmptyValue {
        typedef EmptyValue type ;
        
        friend
        ALGO_INLINE
        bool operator == ( EmptyValue x, EmptyValue y ) { return true ; }
        
        friend
        ALGO_INLINE
        bool operator != ( EmptyValue x, EmptyValue y ) { return false ; }
        
        friend
        ALGO_INLINE
        bool operator < ( EmptyValue x, EmptyValue y ) { return false ; }
        
        friend
        ALGO_INLINE
        bool operator <= ( EmptyValue x, EmptyValue y ) { return true ; }
        
        friend
        ALGO_INLINE
        bool operator > ( EmptyValue x, EmptyValue y ) { return false ; }
        
        friend
        ALGO_INLINE
        bool operator >= ( EmptyValue x, EmptyValue y ) { return true ; }
    } ;
        
    } // namespace detail
    
    
    // Leave predicate to a later problem ... the predicate states whether the current item is the last in the range
    // which hints at more state being required.
    // struct EndPredicate {} ;
    
    
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsARange
        : ALGO_CALL::HasProperty < ALGO_CALL::StartIterator, Range >::type
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
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsARange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        : ALGO_CALL::IteratorTraits < typename StartIteratorType < Range >::type >
    {} ;
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct BasicBoundedRange
    {
        typedef ALGO_CALL::Compound < ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter >
            , ALGO_CALL::ValueAndProperty < ALGO_CALL::EndIterator, Iter > > type ;
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
    
    template < typename Iter, typename DifferenceType = typename ALGO_CALL::IteratorTraits < Iter >::difference_type ALGO_COMMA_ENABLE_IF_PARAM >
    struct BasicBoundedCountedRange
    {
        typedef ALGO_CALL::Compound <
            ALGO_CALL::ValueAndProperty < ALGO_CALL::StartIterator, Iter >
            , ALGO_CALL::Compound < ALGO_CALL::ValueAndProperty < ALGO_CALL::Count, DifferenceType >
                , ALGO_CALL::ValueAndProperty < ALGO_CALL::EndIterator, Iter > > > type ;
    } ;
    
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsABoundedRange : ALGO_LOGIC_CALL::and_ <
        ALGO_CALL::IsARange < Range >
        , ALGO_CALL::HasProperty < ALGO_CALL::EndIterator, Range > >
    {} ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsACountedRange : ALGO_LOGIC_CALL::and_ <
        ALGO_CALL::IsARange < Range >
        , ALGO_CALL::HasProperty < ALGO_CALL::Count, Range > >
    {} ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsAFiniteRange : ALGO_LOGIC_CALL::or_ <
        ALGO_CALL::IsABoundedRange < Range >
        , ALGO_CALL::IsACountedRange < Range > >
    {} ;
    
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsANonFiniteRange : ALGO_LOGIC_CALL::and_ <
        ALGO_CALL::IsARange < Range >
        , ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsAFiniteRange < Range > > >
    {} ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsAReversedRange : ALGO_LOGIC_CALL::and_ <
        ALGO_CALL::IsARange < Range >
        , ALGO_CALL::HasProperty < ALGO_CALL::ReversedRange, Range > >
    {} ;
    
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct BasicReversedRange ;
    
    template < typename Range >
    struct BasicReversedRange < Range
        , typename ALGO_LOGIC_CALL::disable_if_pred < IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < Range, std::bidirectional_iterator_tag >::type::value), "Must be a bidirectional range " ) ;
        
        // Put the EmptyValue at the RHS of the Compound value.
        typedef ALGO_CALL::Compound < Range, ALGO_CALL::ValueAndProperty < ALGO_CALL::ReversedRange, ALGO_DETAIL_CALL::EmptyValue > > type ;
    } ;
    
    // Reverse ( Reverse ( X ) ) = X
    template < typename Range >
    struct BasicReversedRange < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < Range, std::bidirectional_iterator_tag >::type::value), "Must be a bidirectional range " ) ;
        
        typedef typename ALGO_CALL::RemoveProperty < ReversedRange, Range >::type type ;
    } ;
    
    
    template < typename BoundedRange ALGO_COMMA_ENABLE_IF_PARAM >
    struct EndIteratorType
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::IsABoundedRange < BoundedRange >::type::value), "Must be a bounded range " ) ;
        
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::EndIterator, BoundedRange >::type type ;
    } ;
    
    template < typename CountedRange ALGO_COMMA_ENABLE_IF_PARAM >
    struct CountType
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::IsACountedRange < CountedRange >::type::value), "Must be a counted range " ) ;
        
        typedef typename ALGO_CALL::ValueType < ALGO_CALL::Count, CountedRange >::type type ;
    } ;
    
    namespace detail {
        
        template < typename CountedRange, typename N >
        ALGO_INLINE
        typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsACountedRange < CountedRange >, void >::type
        subtractFromCount ( CountedRange& x, N n, ALGO_CALL::InPlace )
        {
            ALGO_STATIC_ASSERT ( (ALGO_CALL::IsACountedRange < CountedRange >::type::value), "Must be a counted range " ) ;

            ALGO_CALL::GetValue < ALGO_CALL::Count, ALGO_CALL::ByReference, CountedRange >::apply ( x ) -= typename ALGO_CALL::CountType < CountedRange >::type ( n ) ;
        }
        
        template < typename NotACountedRange, typename N >
        ALGO_NO_OP_FUNCTION
        ALGO_INLINE
        typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::IsACountedRange < NotACountedRange >, void >::type
        subtractFromCount ( NotACountedRange& x, N n, ALGO_CALL::InPlace )
            ALGO_NOEXCEPT_DECL ( true )
        {
            ALGO_STATIC_ASSERT_IS_RANGE ( NotACountedRange );
            // No count to modify
        }
        
        template < typename Range >
        struct IsANonReversedRange
            : ALGO_LOGIC_CALL::and_ <
                ALGO_CALL::IsARange < Range >
                , ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsAReversedRange < Range > > >
        {} ;
        
        template < typename Range >
        struct IsARangeWithStdIteratorStartIterator
            : ALGO_LOGIC_CALL::and_ <
                ALGO_CALL::IsARange < Range >
                , ALGO_CALL::IsRealStdIterator < typename ALGO_CALL::ValueReturnType< ALGO_CALL::StartIterator, ALGO_CALL::ByValue, Range >::type > >
        {};
        
    } // namespace detail
    
    
    
    template < typename Range >
    struct Predecessor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_DETAIL_CALL::IsANonReversedRange < Range >
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
            , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >
#endif
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Predecessor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ALGO_CALL::Predecessor < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type >::apply ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, -1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
    template < typename Range >
    struct Predecessor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_DETAIL_CALL::IsANonReversedRange < Range >
            , ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range >
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Predecessor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            --( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, -1, ALGO_CALL::InPlace () ) ;
        }
    } ;
#endif
    
    template < typename Range >
    struct Predecessor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsAReversedRange < Range >
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
            , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >
#endif
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Predecessor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ALGO_CALL::Successor < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type >::apply ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, -1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
    template < typename Range >
    struct Predecessor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsAReversedRange < Range >
            , ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range >
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Predecessor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ++( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, -1, ALGO_CALL::InPlace () ) ;
        }
    } ;
#endif
    
    
    
    template < typename Range >
    struct Successor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_DETAIL_CALL::IsANonReversedRange < Range >
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
            , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >
#endif
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Successor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ALGO_CALL::Successor < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type >::apply ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, 1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
    template < typename Range >
    struct Successor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_DETAIL_CALL::IsANonReversedRange < Range >
            , ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Successor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ++( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, 1, ALGO_CALL::InPlace () ) ;
        }
    } ;
#endif
    
    template < typename Range >
    struct Successor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsAReversedRange < Range >
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
            , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >
#endif
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Successor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            ALGO_CALL::Predecessor < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type >::apply ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, 1, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
    template < typename Range >
    struct Successor < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsAReversedRange < Range >
            , ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Successor type ;
        
        ALGO_INLINE
        static void apply ( Range& x )
        {
            --( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ) ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, 1, ALGO_CALL::InPlace () ) ;
        }
    } ;
#endif
    
    
    
    template < typename Range1, typename Range2 >
    struct Distance < Range1, Range2
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsARange < Range1 >
            , ALGO_CALL::IsARange < Range2 > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Distance type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < Range1, std::forward_iterator_tag >::type::value), "Must be a forward range " ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < Range2, std::forward_iterator_tag >::type::value), "Must be a forward range " ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Range1 >::difference_type apply ( Range1 const& x, Range2 const& y )
        {
            return ALGO_CALL::distance ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range1 const >::apply ( x )
                                        , ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range2 const >::apply ( y ) ) ;
        }
    } ;
    
    
    
    template < typename Range >
    struct Advance < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_DETAIL_CALL::IsANonReversedRange < Range >
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
            , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >
#endif
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Advance type ;
        
        ALGO_INLINE
        static void apply ( Range& x, typename ALGO_CALL::IteratorTraits < Range >::difference_type n )
        {
            ALGO_CALL::Advance < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type >::apply
                ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ), n ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, n, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
    template < typename Range >
    struct Advance < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_DETAIL_CALL::IsANonReversedRange < Range >
            , ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range >
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Advance type ;
        
        ALGO_INLINE
        static void apply ( Range& x, typename ALGO_CALL::IteratorTraits < Range >::difference_type n )
        {
            std::advance ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ), n ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, n, ALGO_CALL::InPlace () ) ;
        }
    } ;
#endif
    
    template < typename Range >
    struct Advance < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsAReversedRange < Range >
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
            , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >
#endif
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Advance type ;
        
        ALGO_INLINE
        static void apply ( Range& x, typename ALGO_CALL::IteratorTraits < Range >::difference_type n )
        {
            ALGO_CALL::Advance < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type >::apply
                ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ), -n ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, n, ALGO_CALL::InPlace () ) ;
        }
    } ;
    
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
    template < typename Range >
    struct Advance < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsAReversedRange < Range >
            , ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range >
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Advance type ;
        
        ALGO_INLINE
        static void apply ( Range& x, typename ALGO_CALL::IteratorTraits < Range >::difference_type n )
        {
            std::advance ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByReference, Range >::apply ( x ), -n ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, n, ALGO_CALL::InPlace () ) ;
        }
    } ;
#endif
    
    
    
    template < typename Range >
    struct Deref < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsARange < Range >
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
            , ALGO_LOGIC_CALL::not_ < ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >
#endif
            >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Deref type ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Range >::reference apply ( Range const& x )
        {
            return ALGO_CALL::Deref < typename ALGO_CALL::ValueReturnType < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, Range const >::type >::apply
                ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, Range const >::apply ( x ) ) ;
        }
    } ;
    
#ifndef ALGO_NO_RANGE_STD_SPECIALIZATIONS
    template < typename Range >
    struct Deref < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsARange < Range >
            , ALGO_DETAIL_CALL::IsARangeWithStdIteratorStartIterator < Range > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Deref type ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Range >::reference apply ( Range const& x )
        {
            return *( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, Range const >::apply ( x ) ) ;
        }
    } ;
#endif
    
    
    
    template < typename CountedRange >
    struct IsEmpty < CountedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsACountedRange < CountedRange >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef IsEmpty type ;
        
        ALGO_INLINE
        static bool apply ( CountedRange const& x )
        {
            return typename CountType < CountedRange >::type ( 0 ) == ALGO_CALL::GetValue < ALGO_CALL::Count, ALGO_CALL::ByValue, CountedRange const >::apply ( x ) ;
        }
    } ;
    
    // Note use the counted method of determining whether a range is empty in preference to the start & end iterator
    // as it is one piece of unknown data being obtained, rather than two.
    template < typename BoundedRange >
    struct IsEmpty < BoundedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsACountedRange < BoundedRange > >
            , ALGO_CALL::IsABoundedRange < BoundedRange > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef IsEmpty type ;
        
        ALGO_INLINE
        static bool apply ( BoundedRange const& x )
        {
            return ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, BoundedRange const >::apply ( x )
                == ALGO_CALL::GetValue < ALGO_CALL::EndIterator, ALGO_CALL::ByValue, BoundedRange const >::apply ( x ) ;
        }
    } ;
    
    template < typename NonFiniteRange >
    struct IsEmpty < NonFiniteRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsANonFiniteRange < NonFiniteRange >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef IsEmpty type ;
        
        ALGO_NO_OP_FUNCTION
        ALGO_INLINE
        static bool apply ( NonFiniteRange const& )
            ALGO_NOEXCEPT_DECL ( true )
        {
            return false ;
        }
    } ;
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct HasCountO1Time
        : ALGO_LOGIC_CALL::or_ <
            ALGO_CALL::IsACountedRange < Range >
            , ALGO_LOGIC_CALL::and_ <
                ALGO_CALL::IsABoundedRange < Range >
                , std::is_convertible < typename ALGO_CALL::IteratorTraits < Range >::iterator_category
                    , std::random_access_iterator_tag > > >
    {} ;

    namespace detail {
    
        const ptrdiff_t UNABLE_TO_DETERMINE_SIZE = -1 ;
        
    }
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct CountO1Time ;
    
    template < typename Range >
    struct CountO1Time < Range, typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::HasCountO1Time < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef CountO1Time type ;
        
        ALGO_STATIC_ASSERT_IS_RANGE ( Range ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Range >::difference_type apply ( Range x )
        {
            return ALGO_DETAIL_CALL::UNABLE_TO_DETERMINE_SIZE ;
        }
    };
    
    template < typename CountedRange >
    struct CountO1Time < CountedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsACountedRange < CountedRange >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef CountO1Time type ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < CountedRange >::difference_type apply ( CountedRange x )
        {
            return ALGO_CALL::getValue < ALGO_CALL::Count > ( x ) ;
        }
    } ;
    
    template < typename RandomAccessBoundedRange >
    struct CountO1Time < RandomAccessBoundedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsACountedRange < RandomAccessBoundedRange > >
            , ALGO_CALL::HasCountO1Time < RandomAccessBoundedRange >
            , ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsAReversedRange < RandomAccessBoundedRange > > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef CountO1Time type ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < RandomAccessBoundedRange >::difference_type apply ( RandomAccessBoundedRange x )
        {
            return ALGO_CALL::distance ( ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( x )
                                        , ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( x ) ) ;
        }
    } ;
    
    template < typename RandomAccessBoundedRange >
    struct CountO1Time < RandomAccessBoundedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsACountedRange < RandomAccessBoundedRange > >
            , ALGO_CALL::HasCountO1Time < RandomAccessBoundedRange >
            , ALGO_CALL::IsAReversedRange < RandomAccessBoundedRange > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef CountO1Time type ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < RandomAccessBoundedRange >::difference_type apply ( RandomAccessBoundedRange x )
        {
            return ALGO_CALL::distance ( ALGO_CALL::getValue < ALGO_CALL::EndIterator > ( x )
                                        , ALGO_CALL::getValue < ALGO_CALL::StartIterator > ( x ) ) ;
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
    
    struct NoArgument {} ;
    
    template < typename FirstArgument, typename SecondArgument = NoArgument ALGO_COMMA_ENABLE_IF_PARAM >
    struct DeduceRangeType ;
    
    template < typename FirstArgument >
    struct DeduceRangeType <
        FirstArgument
        , ALGO_CALL::NoArgument
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsARange < FirstArgument >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef DeduceRangeType type ;
        
        typedef FirstArgument returnType ;
        
        ALGO_INLINE
        static returnType apply ( FirstArgument x )
        {
            return x ;
        }
    } ;
    
    template < typename FirstArgument >
    struct DeduceRangeType <
        FirstArgument
        , ALGO_CALL::NoArgument
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsARange < FirstArgument > >
            , ALGO_CALL::HasIteratorTraits < FirstArgument > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef DeduceRangeType type ;
        
        typedef typename ALGO_CALL::BasicUnboundedRange < FirstArgument >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( FirstArgument x )
        {
            returnType returnValue = { x } ;
            return returnValue ;
        }
    } ;
    
    // Should not have received a range for 2-argument deduction.
    template < typename FirstArgument, typename SecondArgument >
    struct DeduceRangeType <
        FirstArgument
        , SecondArgument
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_LOGIC_CALL::is_same < SecondArgument, NoArgument > >
            , ALGO_LOGIC_CALL::or_ < ALGO_CALL::IsARange < FirstArgument >
                , ALGO_CALL::IsARange < SecondArgument > > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type > ;
    
    
    template < typename FirstArgument, typename SecondArgument >
    struct DeduceRangeType <
        FirstArgument
        , SecondArgument
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_LOGIC_CALL::is_same < SecondArgument, NoArgument > >
            , ALGO_CALL::HasIteratorTraits < FirstArgument >
            , ALGO_CALL::HasIteratorTraits < SecondArgument > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef DeduceRangeType type ;
        
        typedef typename ALGO_CALL::BasicBoundedRange < FirstArgument, SecondArgument >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( FirstArgument x, SecondArgument y )
        {
            returnType returnValue = { { x }, { y } } ;
            return returnValue ;
        }
    } ;
    
    template < typename FirstArgument, typename SecondArgument >
    struct DeduceRangeType <
        FirstArgument
        , SecondArgument
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_LOGIC_CALL::is_same < SecondArgument, NoArgument > >
            , ALGO_CALL::HasIteratorTraits < FirstArgument >
            , ALGO_LOGIC_CALL::not_ < ALGO_CALL::HasIteratorTraits < SecondArgument > > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef DeduceRangeType type ;
        
        typedef typename BasicCountedRange < FirstArgument >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( FirstArgument x, SecondArgument y )
        {
            returnType returnValue = { { x }, { y } } ;
            return returnValue ;
        }
    } ;
    
    template < typename T, ptrdiff_t N >
    struct DeduceRangeType <
        T [ N ]
        , NoArgument
        , ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef DeduceRangeType type ;
        
        typedef typename ALGO_CALL::BasicBoundedRange < T* >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( T (&x)[ N ] )
        {
            return ALGO_CALL::DeduceRangeType < T*, T* >::apply ( x, x + N ) ;
        }
    } ;
    
    namespace detail {
        
        template < typename T >
        struct GetConstModifiedIterator
        {
            typedef typename T::iterator type ;
        } ;
        
        template < typename T >
        struct GetConstModifiedIterator < T const >
        {
            typedef typename T::const_iterator type ;
        } ;
        
    } // namespace detail
    
    template < typename T >
    struct DeduceRangeType < T&
        , NoArgument
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_TRAITS_TEST_CALL::HasContainerTraits < typename std::remove_cv < T >::type >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef DeduceRangeType type ;
        
        typedef typename ALGO_CALL::BasicBoundedCountedRange < typename ALGO_DETAIL_CALL::GetConstModifiedIterator < T >::type >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( T& x )
        {
            typename ALGO_CALL::IteratorTraits < typename ALGO_DETAIL_CALL::GetConstModifiedIterator < T >::type >::difference_type size = x.size () ;
            returnType returnType = { { x.begin () }, { { size }, { x.end () } } } ;
            return returnType ;
        }
    } ;
    
    template < typename T >
    ALGO_INLINE
    typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_TRAITS_TEST_CALL::HasContainerTraits < typename std::remove_cv < T >::type >
        , typename ALGO_CALL::DeduceRangeType < T& >::returnType >::type
    deduceRange ( T& x )
    {
        return ALGO_CALL::DeduceRangeType < T& >::apply ( x ) ;
    }
    
    template < typename FirstArgument >
    ALGO_INLINE
    typename ALGO_CALL::DeduceRangeType < FirstArgument >::returnType deduceRange ( FirstArgument x )
    {
        return ALGO_CALL::DeduceRangeType < FirstArgument >::apply ( x ) ;
    }
    
    template < typename T, ptrdiff_t N >
    ALGO_INLINE
    typename ALGO_CALL::DeduceRangeType < T [ N ] >::returnType deduceRange ( T (&x) [ N ] )
    {
        return ALGO_CALL::DeduceRangeType < T [ N ] >::apply ( x ) ;
    }
    
    template < typename FirstArgument, typename SecondArgument >
    ALGO_INLINE
    typename ALGO_CALL::DeduceRangeType < FirstArgument, SecondArgument >::returnType deduceRange ( FirstArgument x, SecondArgument y )
    {
        return ALGO_CALL::DeduceRangeType < FirstArgument, SecondArgument >::apply ( x, y ) ;
    }
    
    namespace detail {
        
    template < typename Range >
    ALGO_INLINE
    typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsABoundedRange < Range >, Range >::type
    reverseRangeIterators ( Range x )
    {
        std::swap ( ALGO_CALL::getValueByReference < ALGO_CALL::StartIterator > ( x )
                   , ALGO_CALL::getValueByReference < ALGO_CALL::EndIterator > ( x ) ) ;
        
        return x ;
    }
    
    template < typename Range >
    ALGO_INLINE
    typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsACountedRange < Range >, Range >::type
    reverseRangeIterators ( Range x )
    {
        ALGO_CALL::advance ( ALGO_CALL::getValueByReference < ALGO_CALL::StartIterator > ( x )
                            , ALGO_CALL::getValue < ALGO_CALL::Count > ( x )
                            , ALGO_CALL::InPlace () ) ;
        
        return x ;
    }
        
    } // namespace detail
    
    template < typename Range >
    ALGO_INLINE
    typename ALGO_CALL::BasicReversedRange < Range >::type
    reverseRange ( Range x )
    {
        ALGO_STATIC_ASSERT  ( (ALGO_CALL::IsAFiniteRange < Range >::type::value ), "Must be a finite range" ) ;
        typedef typename ALGO_CALL::BasicReversedRange < Range >::type ReturnType ;
        
        return ALGO_DETAIL_CALL::reverseRangeIterators ( ALGO_CALL::convertPropertySet < ReturnType > ( x ) ) ;
    }
    
    template < typename Iter >
    struct DeduceReversedUnboundedRangeType
        : ALGO_CALL::BasicReversedRange < typename ALGO_CALL::BasicUnboundedRange < Iter >::type >
    {} ;
    
    // NOTE made this a different function name in order to have the caller expicitly specify that the
    // given iterator is one past the right-hand end of the range.
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::DeduceReversedUnboundedRangeType < Iter >::type
    deduceReversedUnboundedRange ( Iter x )
    {
        ALGO_STATIC_ASSERT  ( (ALGO_CALL::IsRealStdIterator < Iter >::type::value ), "Must be an iterator" ) ;
        
        typename ALGO_CALL::DeduceReversedUnboundedRangeType < Iter >::type returnValue = {} ;
        ALGO_CALL::setValue < ALGO_CALL::StartIterator > ( returnValue, x, ALGO_CALL::InPlace () ) ;
        return returnValue ;
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
            ALGO_CALL::assignImpl ( op ( ALGO_CALL::deref ( x ) ), y ) ;
            
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
            ALGO_CALL::assignImpl ( op ( ALGO_CALL::deref ( x ), ALGO_CALL::deref ( y ) ), z ) ;
            
            ALGO_CALL::successor ( x, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( y, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( z, ALGO_CALL::InPlace () ) ;
        }
        return std::make_pair ( z, std::make_pair ( x, y ) ) ;
    }
    
    namespace detail {
        
    template < typename OutputRange1, typename OutputRange2, typename T >
    void split ( OutputRange1& x, OutputRange2& y, T z )
    {
        ALGO_CALL::moveAssign ( &z.first, x ),
        ALGO_CALL::moveAssign ( &z.second, y ) ;
    }
        
    } // namespace detail

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
            ALGO_DETAIL_CALL::split ( y, z, op ( ALGO_CALL::deref ( x ) ) ) ;

            ALGO_CALL::successor ( x, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( y, ALGO_CALL::InPlace () )
            , ALGO_CALL::successor ( z, ALGO_CALL::InPlace () ) ;
        }
        return std::make_pair ( std::make_pair ( y, z ), x ) ;
    }
    
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
            , DefaultDeduceStepOperationTag
            , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
        template < typename Range >
        struct DeduceStepOperation < Range
            , ALGO_DETAIL_CALL::pre_op_o_tag
            , DefaultDeduceStepOperationTag
            , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
        template < typename Range >
        struct DeduceStepOperation < Range
            , ALGO_DETAIL_CALL::post_op_i_tag
            , DefaultDeduceStepOperationTag
            , typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
        template < typename Range >
        struct DeduceStepOperation < Range
            , ALGO_DETAIL_CALL::post_op_o_tag
            , DefaultDeduceStepOperationTag
            , typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::IsAReversedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
        {
            typedef ALGO_CALL::Successor < Range > type ;
        } ;
        
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
            
            ptrdiff_t const size1 = ALGO_CALL::CountO1Time < Range1 >::apply ( x ) ;
            ptrdiff_t const size2 = ALGO_CALL::CountO1Time < Range2 >::apply ( y ) ;
            
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
        , typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_DETAIL_CALL::IsSuitableForStepOverCounted < InputRange, OutputRange >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef StepOverRange type ;
        
        typedef std::pair < OutputRange, InputRange > ReturnType ;
        
        ALGO_INLINE
        static ReturnType apply ( InputRange from, OutputRange to, StepOperation op )
        {
            return StepOverRangeCheckingIsEmpty < InputRange, OutputRange, StepOperation, DeduceStepOperationTag >::apply ( from, to, op ) ;
        }
    } ;
    
    template < typename InputRange, typename OutputRange, typename StepOperation, typename DeduceStepOperationTag >
    struct StepOverRange < InputRange, OutputRange, StepOperation, DeduceStepOperationTag
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_DETAIL_CALL::IsSuitableForStepOverCounted < InputRange, OutputRange >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef StepOverRange type ;
        
        typedef std::pair < OutputRange, InputRange > ReturnType ;
        
        ALGO_INLINE
        static ReturnType apply ( InputRange from, OutputRange to, StepOperation op )
        {
            return StepOverCounted < InputRange, OutputRange, StepOperation, DeduceStepOperationTag >::apply ( from, to, ALGO_DETAIL_CALL::getMinRangeLength ( from, to ), op ) ;
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
        return ALGO_CALL::getValue < ALGO_CALL::StartIterator >
        ( ALGO_CALL::stepOverDeduced ( ALGO_CALL::deduceRange ( from, to ), ALGO_CALL::deduceRange ( o ), op  ) ) ;
    }
    
    template < typename I, typename N, typename O, typename StepOperation >
    ALGO_INLINE
    O stepCounted ( I from, N times, O o, StepOperation op )
    {
        return ALGO_CALL::getValue < ALGO_CALL::StartIterator >
        ( ALGO_CALL::stepOverDeduced ( ALGO_CALL::deduceRange ( from, times ), ALGO_CALL::deduceRange ( o ), op ) ) ;
    }
} // namespace algo

#endif
