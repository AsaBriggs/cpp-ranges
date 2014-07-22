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
    struct ElementCount
    {
        typedef typename ALGO_CALL::IteratorTraits < Range >::difference_type returnType ;
    } ;
    
    template < typename CountedRange >
    struct ElementCount < CountedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsACountedRange < CountedRange >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef ElementCount type ;
        
        typedef typename ALGO_CALL::IteratorTraits < CountedRange >::difference_type returnType ;
        
        ALGO_INLINE
        static returnType apply ( CountedRange x )
        {
            return ALGO_CALL::GetValue < ALGO_CALL::Count, ALGO_CALL::ByReference, CountedRange >::apply ( x ) ;
        }
    } ;
    
    template < typename BoundedRange >
    struct ElementCount < BoundedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsACountedRange < BoundedRange > >
            , ALGO_CALL::IsABoundedRange < BoundedRange >
            , ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsAReversedRange < BoundedRange > > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef ElementCount type ;
        
        typedef typename ALGO_CALL::IteratorTraits < BoundedRange >::difference_type returnType ;
        
        ALGO_INLINE
        static returnType apply ( BoundedRange x )
        {
            return ALGO_CALL::distance ( ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, BoundedRange >::apply ( x )
                                        , ALGO_CALL::GetValue < ALGO_CALL::EndIterator, ALGO_CALL::ByValue, BoundedRange >::apply ( x ) ) ;
        }
    } ;
    
    template < typename BoundedRange >
    struct ElementCount < BoundedRange
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsACountedRange < BoundedRange > >
            , ALGO_CALL::IsABoundedRange < BoundedRange >
            , ALGO_CALL::IsAReversedRange < BoundedRange > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef ElementCount type ;
        
        typedef typename ALGO_CALL::IteratorTraits < BoundedRange >::difference_type returnType ;
        
        ALGO_INLINE
        static returnType apply ( BoundedRange x )
        {
            return ALGO_CALL::distance ( ALGO_CALL::GetValue < ALGO_CALL::EndIterator, ALGO_CALL::ByValue, BoundedRange >::apply ( x )
                                        , ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, BoundedRange >::apply ( x ) ) ;
        }
    } ;
    
    template < typename Range >
    ALGO_INLINE
    typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::HasCountO1Time < Range >, typename ALGO_CALL::ElementCount < Range >::returnType >::type
    countO1Time ( Range x )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( Range ) ;
        
        return ALGO_CALL::ElementCount < Range >::apply ( x ) ;
    }
    
    template < typename Range >
    ALGO_INLINE
    typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::HasCountO1Time < Range >, typename ALGO_CALL::ElementCount < Range >::returnType >::type
    countO1Time ( Range x )
    {
        return ALGO_DETAIL_CALL::UNABLE_TO_DETERMINE_SIZE ;
    }
    
    template < typename Range >
    ALGO_INLINE
    typename ALGO_CALL::ElementCount < Range >::returnType
    elementCount ( Range x )
    {
        ALGO_STATIC_ASSERT_IS_RANGE ( Range ) ;
        
        return ALGO_CALL::ElementCount < Range >::apply ( x ) ;
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
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::HasContainerTraits < typename std::remove_cv < T >::type >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
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
    typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::HasContainerTraits < typename std::remove_cv < T >::type >
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
    
    template < typename ConvertTo, typename ConvertFrom ALGO_COMMA_ENABLE_IF_PARAM >
    struct ConvertRangeToIterator ;
    
    template < typename Convert >
    struct ConvertRangeToIterator < Convert, Convert, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef ConvertRangeToIterator type ;
        
        ALGO_INLINE
        static Convert apply ( Convert const& x )
        {
            return x ;
        }
    } ;
    
    template < typename ConvertTo, typename ConvertFrom >
    struct ConvertRangeToIterator < ConvertTo, ConvertFrom
        , typename ALGO_LOGIC_CALL::enable_if_pred <
            ALGO_LOGIC_CALL::and_ <
                ALGO_CALL::IsRealStdIterator < ConvertTo >
                , ALGO_CALL::IsARange < ConvertFrom >
                , ALGO_LOGIC_CALL::is_same < ConvertTo, typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, ConvertFrom >::type > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef ConvertRangeToIterator type ;

        ALGO_INLINE
        static ConvertTo apply ( ConvertFrom const& x )
        {
            return ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, ConvertFrom const >::apply ( x ) ;
        }
    } ;
    
    template < typename ConvertTo, typename ConvertFrom >
    ConvertTo convertRangeToIterator ( ConvertFrom const& x )
    {
        return ALGO_CALL::ConvertRangeToIterator < ConvertTo, ConvertFrom >::apply ( x ) ;
    }
    
    
    
    namespace detail {
        
        template < typename Range, typename Iter >
        typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::HasProperty< ALGO_CALL::EndIterator, Range >, void >::type
        setEndIter ( Range& x, Iter i, ALGO_CALL::InPlace )
        {
            ALGO_CALL::SetValue < ALGO_CALL::EndIterator, Range >::apply ( x, i ) ;
        }
        
        template < typename Range, typename Iter >
        typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::HasProperty< ALGO_CALL::EndIterator, Range >, void >::type
        setEndIter ( Range& x, Iter i, ALGO_CALL::InPlace )
        {}
        
    } // namespace detail
    
    template < typename Range ALGO_COMMA_ENABLE_IF_PARAM >
    struct HalveRange ;
    
    template < typename Range >
    struct HalveRange < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsACountedRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef std::pair < Range, Range > returnType ;
        
        ALGO_INLINE
        static returnType apply ( Range x )
        {
            typedef typename ALGO_CALL::CountType < Range >::type CountType ;
            CountType count = ALGO_CALL::GetValue < ALGO_CALL::Count, ALGO_CALL::ByValue, Range >::apply ( x ) ;
            CountType firstHalf = count / 2 ;
            CountType secondHalf = count - firstHalf ;

            Range secondRange = x ;
            ALGO_CALL::Advance < Range >::apply ( secondRange, firstHalf ) ;
            
            ALGO_DETAIL_CALL::setEndIter ( x, ALGO_CALL::GetValue < ALGO_CALL::StartIterator, ALGO_CALL::ByValue, Range >::apply ( secondRange ), ALGO_CALL::InPlace () ) ;
            ALGO_DETAIL_CALL::subtractFromCount ( x, secondHalf, ALGO_CALL::InPlace () ) ;

            
            return std::make_pair ( x, secondRange ) ;
        }
    } ;
    
    template < typename Range >
    struct HalveRange < Range
    , typename ALGO_LOGIC_CALL::enable_if_pred <
        ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsABoundedRange < Range >
            , ALGO_LOGIC_CALL::not_ < ALGO_CALL::IsACountedRange < Range > > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef typename ALGO_CALL::IteratorTraits < typename ALGO_CALL::ValueType < ALGO_CALL::StartIterator, Range >::type >::difference_type CountType ;
        typedef typename ALGO_CALL::AddPropertyType < ALGO_CALL::Count, CountType, Range >::type CountAugmentedRange ;
        typedef typename ALGO_CALL::HalveRange < CountAugmentedRange >::returnType returnType ;
        
        ALGO_INLINE
        static returnType apply ( Range x )
        {
            CountAugmentedRange const y = ALGO_CALL::addProperty < ALGO_CALL::Count > ( x, ALGO_CALL::ElementCount < Range >::apply ( x ) ) ;
            
            return ALGO_CALL::HalveRange < CountAugmentedRange >::apply ( y ) ;
        }
    } ;
    
    template < typename Range >
    struct HalveRange < Range
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsANonFiniteRange < Range >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type > ;
    
    template < typename Range >
    typename ALGO_CALL::HalveRange < Range >::returnType halveRange ( Range x )
    {
        return ALGO_CALL::HalveRange < Range >::apply ( x ) ;
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
} // namespace algo

#endif
