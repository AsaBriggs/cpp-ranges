#ifndef INCLUDED_ALGO_TRAITS
#define INCLUDED_ALGO_TRAITS

#ifndef INCLUDED_ALGO_BASICS
#include "algo_basics.h"
#endif

#ifndef INCLUDED_TYPE_TRAITS
#include <type_traits>
#define INCLUDED_TYPE_TRAITS
#endif

#ifndef INCLUDED_UTILITY
#include <utility>
#define INCLUDED_UTILITY
#endif

#ifndef INCLUDED_ITERATOR
#include <iterator>
#define INCLUDED_ITERATOR
#endif

#ifndef INCLUDED_LIMITS
#include <limits>
#define INCLUDED_LIMITS
#endif

namespace algo
{
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct IteratorTraits : std::iterator_traits < Iter >
    {} ;
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsBitwiseCopyable : std::is_trivially_copy_assignable < T >
    {} ;
    
    template < typename T, size_t N >
    struct IsBitwiseCopyable < T [ N ], ALGO_ENABLE_IF_PARAM_DEFAULT > : ALGO_CALL::IsBitwiseCopyable < T >
    {} ;
    
    template < typename T >
    struct IsBitwiseCopyable < T&, ALGO_ENABLE_IF_PARAM_DEFAULT > : std::false_type
    {} ;
    
    template < typename T, typename U >
    struct IsBitwiseCopyable < std::pair < T, U >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    : std::integral_constant < bool, ALGO_CALL::IsBitwiseCopyable < T >::value && ALGO_CALL::IsBitwiseCopyable < U >::value >
    {} ;
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsBitwiseMoveable : ALGO_CALL::IsBitwiseCopyable < T >
    {} ;
    
    // shared pointer, managed pointer, containers that have no back-pointers from remote parts.
    
    
    template < typename Iter >
    struct IsNotProxiedIterator
    : std::is_same< typename std::remove_cv < typename ALGO_CALL::IteratorTraits < Iter >::reference >::type,
    typename std::remove_cv < typename ALGO_CALL::IteratorTraits < Iter >::value_type >::type& >
    {} ;
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsTriviallyDestructible : std::is_trivially_destructible < T >
    {} ;
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct SizeOf : std::integral_constant < size_t, sizeof ( T ) >
    {} ;
    
    
    
    // Must be a power of 2, must be <= Sizeof ( T )
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct AlignmentOf : std::alignment_of < T >
    {} ;
    
    
    template < size_t X >
    struct PowerOfTwo ;
    
    // 2 ^ ( - infinity ) ?
    template <>
    struct PowerOfTwo < 0u > : std::false_type
    {};
    
    template <>
    struct PowerOfTwo < 1u > : std::true_type
    {};
    
    template <>
    struct PowerOfTwo < 2u > : std::true_type
    {};
    
    template < size_t X >
    struct PowerOfTwo : std::integral_constant < bool, ( 0u == X % 2 ) && PowerOfTwo < X / 2u >::value >
    {};
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct GetAlignmentOf : ALGO_CALL::AlignmentOf < T >
    {
        typedef ALGO_CALL::AlignmentOf < T > ParentType ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::PowerOfTwo < ParentType::value >::value), "Alignment must be a power of 2" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::SizeOf < T >::value >= ParentType::value), "" ) ;
        ALGO_STATIC_ASSERT ( (ParentType::value < size_t ( std::numeric_limits < ptrdiff_t >::max () )), "Can not have an alignment greater than ptrdiff_t max value" ) ;
    } ;

} // namespace algo

#endif
