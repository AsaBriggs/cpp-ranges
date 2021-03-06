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
    struct IsAPointer
        : ALGO_LOGIC_CALL::false_type
    {} ;
    
    template < typename T >
    struct IsAPointer < T* >
        : ALGO_LOGIC_CALL::true_type
    {} ;
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct IteratorTraits
        : std::iterator_traits < Iter >
    {} ;
    
    template < typename T >
    struct IteratorTraits < T* const, ALGO_ENABLE_IF_PARAM_DEFAULT >
        : std::iterator_traits < T* >
    {} ;
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct RepeatableIterator
        : std::is_convertible < typename ALGO_CALL::IteratorTraits < Iter >::iterator_category
            , std::forward_iterator_tag >
    {} ;
    
    template < typename Iter, typename IteratorCategory ALGO_COMMA_ENABLE_IF_PARAM >
    struct CheckIteratorCategory
        : std::is_convertible < typename ALGO_CALL::IteratorTraits < Iter >::iterator_category
            , IteratorCategory >
    {} ;
    
    namespace traits_test
    {
#define ALGO_TRAITS_TEST_CALL ALGO_CALL::traits_test
        
#define ALGO_TEST_FOR_NESTED_TYPE( TestName, TestForNestedType ) \
template < typename T > \
struct TestName \
{ \
private: \
    struct NotOne {char tmp [2] ; } ; \
    template < typename U > static NotOne test ( ... ) ; \
    template < typename U > static char test ( typename U::TestForNestedType * = 0 ) ; \
public: \
    typedef ALGO_LOGIC_CALL::integral_constant < bool, sizeof ( test < T > ( 0 ) ) == 1 > type ;\
} ;
        
#define ALGO_TEST_FOR_NESTED_REFERENCE_TYPE( TestName, TestForNestedType ) \
template < typename T > \
struct TestName \
{ \
private: \
    struct NotOne {char tmp [2] ; } ; \
    template < typename U > static NotOne test ( ... ) ; \
    template < typename U > static char test ( typename std::remove_reference < typename U::TestForNestedType >::type * = 0 ) ; \
public: \
    typedef ALGO_LOGIC_CALL::integral_constant < bool, sizeof ( test < T > ( 0 ) ) == 1 > type ;\
} ;
        
        ALGO_TEST_FOR_NESTED_TYPE( DifferenceTypeTest, difference_type )
        ALGO_TEST_FOR_NESTED_TYPE( ValueTypeTest, value_type )
        ALGO_TEST_FOR_NESTED_TYPE( PointerTest, pointer )
        ALGO_TEST_FOR_NESTED_TYPE( IteratorCategoryTest, iterator_category )
        ALGO_TEST_FOR_NESTED_REFERENCE_TYPE( ReferenceTest, reference )
        ALGO_TEST_FOR_NESTED_REFERENCE_TYPE( ConstReferenceTest, const_reference )
        
        ALGO_TEST_FOR_NESTED_TYPE( IteratorTest, iterator )
        ALGO_TEST_FOR_NESTED_TYPE( ConstIteratorTest, const_iterator )
        ALGO_TEST_FOR_NESTED_TYPE( SizeTypeTest, size_type )

        
#undef ALGO_TEST_FOR_NESTED_REFERENCE_TYPE
#undef ALGO_TEST_FOR_NESTED_TYPE
        
        template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
        struct HasIteratorTraits
            : ALGO_LOGIC_CALL::and_ <
                ALGO_TRAITS_TEST_CALL::IteratorCategoryTest   < T >
                , ALGO_TRAITS_TEST_CALL::DifferenceTypeTest   < T >
                , ALGO_TRAITS_TEST_CALL::ValueTypeTest     < T >
                , ALGO_TRAITS_TEST_CALL::PointerTest       < T >
                , ALGO_TRAITS_TEST_CALL::ReferenceTest     < T > >
        {} ;
        
        template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
        struct HasNestedConstIterator
            : ALGO_LOGIC_CALL::false_type
        {} ;
        
        template < typename T >
        struct HasNestedConstIterator < T
            , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_TRAITS_TEST_CALL::ConstIteratorTest < T >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
            : ALGO_TRAITS_TEST_CALL::HasIteratorTraits < typename T::const_iterator >
        {} ;
        
        template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
        struct HasNestedIterator
            : ALGO_LOGIC_CALL::false_type
        {} ;
        
        template < typename T >
        struct HasNestedIterator < T
            , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_TRAITS_TEST_CALL::IteratorTest < T >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
            : ALGO_TRAITS_TEST_CALL::HasIteratorTraits < typename T::iterator >
        {} ;
        
        template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
        struct HasContainerTraits
            : ALGO_LOGIC_CALL::and_ <
                ALGO_TRAITS_TEST_CALL::HasNestedConstIterator < T >
                , ALGO_TRAITS_TEST_CALL::HasNestedIterator < T >
                , ALGO_TRAITS_TEST_CALL::ValueTypeTest < T >
                , ALGO_TRAITS_TEST_CALL::ReferenceTest < T >
                , ALGO_LOGIC_CALL::and_ <
                    ALGO_TRAITS_TEST_CALL::ConstReferenceTest < T >
                    , ALGO_TRAITS_TEST_CALL::DifferenceTypeTest < T >
                    , ALGO_TRAITS_TEST_CALL::SizeTypeTest < T > > >
        {} ;
    }
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct HasIteratorTraits
        : ALGO_TRAITS_TEST_CALL::HasIteratorTraits < ALGO_CALL::IteratorTraits < T > >
    {} ;
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct HasContainerTraits
        : ALGO_TRAITS_TEST_CALL::HasContainerTraits < T >
    {} ;
    
    // Might want to distinguish between an iterator and a proxy to an iterator
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsRealStdIterator
        : ALGO_TRAITS_TEST_CALL::HasIteratorTraits < std::iterator_traits < T > >
    {} ;
    
    template < typename T >
    struct IsRealStdIterator < T * const, ALGO_ENABLE_IF_PARAM_DEFAULT >
        : ALGO_LOGIC_CALL::true_type
    {} ;
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsBitwiseCopyable
        : std::is_trivially_copy_assignable < T >
    {} ;
    
    template < typename T, size_t N >
    struct IsBitwiseCopyable < T [ N ], ALGO_ENABLE_IF_PARAM_DEFAULT >
        : ALGO_CALL::IsBitwiseCopyable < T >
    {} ;
    
    template < typename T >
    struct IsBitwiseCopyable < T&, ALGO_ENABLE_IF_PARAM_DEFAULT >
        : ALGO_LOGIC_CALL::false_type
    {} ;
    
    template < typename T, typename U >
    struct IsBitwiseCopyable < std::pair < T, U >, ALGO_ENABLE_IF_PARAM_DEFAULT >
        : ALGO_LOGIC_CALL::and_ < ALGO_CALL::IsBitwiseCopyable < T >, ALGO_CALL::IsBitwiseCopyable < U > >
    {} ;
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsBitwiseMoveable
        : ALGO_CALL::IsBitwiseCopyable < T >
    {} ;
    
    // shared pointer, managed pointer, containers that have no back-pointers from remote parts.
    
    
    template < typename Iter >
    struct IsNotProxiedIterator
        : ALGO_LOGIC_CALL::is_same< typename std::remove_cv < typename ALGO_CALL::IteratorTraits < Iter >::reference >::type,
            typename std::remove_cv < typename ALGO_CALL::IteratorTraits < Iter >::value_type >::type& >
    {} ;
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsTriviallyDestructible
        : std::is_trivially_destructible < T >
    {} ;
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct SizeOf
        : ALGO_LOGIC_CALL::integral_constant < size_t, sizeof ( T ) >
    {} ;
    
    
    
    // Must be a power of 2, must be <= Sizeof ( T )
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct AlignmentOf
        : std::alignment_of < T >
    {} ;
    
    
    template < size_t X >
    struct PowerOfTwo ;
    
    // 2 ^ ( - infinity ) ?
    template <>
    struct PowerOfTwo < 0u >
        : ALGO_LOGIC_CALL::false_type
    {};
    
    template <>
    struct PowerOfTwo < 1u >
        : ALGO_LOGIC_CALL::true_type
    {};
    
    template <>
    struct PowerOfTwo < 2u >
        : ALGO_LOGIC_CALL::true_type
    {};
    
    template < size_t X >
    struct PowerOfTwo
        : ALGO_LOGIC_CALL::integral_constant < bool, ( 0u == X % 2 ) && PowerOfTwo < X / 2u >::value >
    {};
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct GetAlignmentOf
        : ALGO_CALL::AlignmentOf < T >
    {
        typedef ALGO_CALL::AlignmentOf < T > ParentType ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::PowerOfTwo < ParentType::value >::value), "Alignment must be a power of 2" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::SizeOf < T >::value >= ParentType::value), "" ) ;
        ALGO_STATIC_ASSERT ( (ParentType::value < size_t ( std::numeric_limits < ptrdiff_t >::max () )), "Can not have an alignment greater than ptrdiff_t max value" ) ;
    } ;

} // namespace algo

#endif
