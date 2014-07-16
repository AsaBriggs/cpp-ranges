#ifndef INCLUDED_ALGO_ITERATOR
#define INCLUDED_ALGO_ITERATOR

#ifndef INCLUDED_ALGO_BASICS
#include "algo_basics.h"
#endif

#ifndef INCLUDED_ALGO_TRAITS
#include "algo_traits.h"
#endif

#ifndef INCLUDED_CSTRING
#include <cstring>
#define INCLUDED_CSTRING
#endif

namespace algo
{
    template < typename BidirectionalIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Predecessor
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < BidirectionalIterator >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < BidirectionalIterator, std::bidirectional_iterator_tag >::type::value ), "Must be bidirectional iterator" ) ;
        
        ALGO_INLINE
        static void apply ( BidirectionalIterator& x )
        {
            --x ;
        }
    } ;
    
    template  < typename BidirectionalIterator >
    ALGO_INLINE
    BidirectionalIterator predecessor ( BidirectionalIterator x, ALGO_CALL::ByReturnValue )
    {
        ALGO_CALL::Predecessor < BidirectionalIterator >::apply ( x ) ;
        return x ;
    }
    
    template < typename BidirectionalIterator >
    ALGO_INLINE
    void predecessor ( BidirectionalIterator& x, ALGO_CALL::InPlace )
    {
        ALGO_CALL::Predecessor < BidirectionalIterator >::apply ( x ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Successor
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( ForwardIterator& x )
        {
            ++x ;
        }
    } ;
    
    template < typename ForwardIterator >
    ALGO_INLINE
    ForwardIterator successor ( ForwardIterator x, ALGO_CALL::ByReturnValue )
    {
        ALGO_CALL::Successor < ForwardIterator >::apply ( x ) ;
        return x ;
    }
    
    template < typename ForwardIterator >
    ALGO_INLINE
    void successor ( ForwardIterator& x, ALGO_CALL::InPlace )
    {
        ALGO_CALL::Successor < ForwardIterator >::apply ( x ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Distance
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type apply ( ForwardIterator x, ForwardIterator y )
        {
            return std::distance ( x, y ) ;
        }
    } ;
    
    template < typename ForwardIterator >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type distance ( ForwardIterator x, ForwardIterator y )
    {
        return ALGO_CALL::Distance < ForwardIterator >::apply ( x, y ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Advance
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( ForwardIterator& x, typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type n )
        {
            std::advance ( x, n ) ;
        }
    } ;
    
    template < typename ForwardIterator, typename N >
    ALGO_INLINE
    ForwardIterator advance ( ForwardIterator x, N n, ALGO_CALL::ByReturnValue )
    {
        typedef typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type difference_type ;
        
        ALGO_ASSERT ( std::numeric_limits < difference_type >::max () >= n ) ;
        ALGO_ASSERT ( std::numeric_limits < difference_type >::min () <= n ) ;
        
        ALGO_CALL::Advance < ForwardIterator >::apply ( x, difference_type ( n ) ) ;
        return x ;
    }
    
    template < typename ForwardIterator, typename N >
    ALGO_INLINE
    void advance ( ForwardIterator& x, N n, ALGO_CALL::InPlace )
    {
        typedef typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type difference_type ;
        
        ALGO_ASSERT ( std::numeric_limits < difference_type >::max () >= n ) ;
        ALGO_ASSERT ( std::numeric_limits < difference_type >::min () <= n ) ;
        
        ALGO_CALL::Advance < ForwardIterator >::apply ( x, difference_type ( n ) ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsEmpty
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static bool apply ( ForwardIterator )
        {
            // TODO figure out if there should be a null-check here ...
            return false ;
        }
    } ;
    
    template < typename Iterator >
    ALGO_INLINE
    bool isEmpty ( Iterator x )
    {
        return ALGO_CALL::IsEmpty < Iterator >::apply ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct Deref
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Iter >::reference apply ( Iter x )
        {
            return *x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < Iter >::reference deref ( Iter x )
    {
        return ALGO_CALL::Deref < Iter >::apply ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct DerefMove
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Iter >::value_type&& apply ( Iter x )
        {
            return std::move ( ALGO_CALL::Deref < Iter >::apply ( x ) ) ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < Iter >::value_type&& derefMove ( Iter x )
    {
        return ALGO_CALL::DerefMove < Iter >::apply ( x ) ;
    }
    
    
    
    template < typename X ALGO_COMMA_ENABLE_IF_PARAM >
    struct AddressOf
    {
        ALGO_INLINE
        static X* apply ( X& x )
        {
            return &x ;
        }
    } ;
    
    template < typename X >
    ALGO_INLINE
    X* addressOf ( X& x )
    {
        return ALGO_CALL::AddressOf < X >::apply ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct UnderlyingAddressOf
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Iter >::pointer apply ( Iter x )
        {
            return ALGO_CALL::AddressOf < typename ALGO_CALL::IteratorTraits < Iter >::value_type >::apply ( ALGO_CALL::Deref < Iter >::apply ( x ) ) ;
        }
    } ;
    
    template < typename X >
    struct UnderlyingAddressOf < X*, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        ALGO_INLINE
        static X* apply ( X* x )
        {
            return x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < Iter >::pointer underlyingAddressOf ( Iter x )
    {
        return ALGO_CALL::UnderlyingAddressOf < Iter >::apply ( x ) ;
    }
    
    
    
    template < typename Iter1, typename Iter2 ALGO_COMMA_ENABLE_IF_PARAM >
    struct EqualUnderlyingAddress
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter1 >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter2 >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static bool apply ( Iter1 x, Iter2 y )
        {
            return reinterpret_cast < const volatile void* > ( ALGO_CALL::UnderlyingAddressOf < Iter1 >::apply ( x ) )
                == reinterpret_cast < const volatile void* > ( ALGO_CALL::UnderlyingAddressOf < Iter2 >::apply ( y ) ) ;
        }
    } ;
    
    template < typename Iter1, typename Iter2 >
    ALGO_INLINE
    bool equalUnderlyingAddress ( Iter1 x, Iter2 y )
    {
        // Requires x & y are dereferencable
        return ALGO_CALL::EqualUnderlyingAddress < Iter1, Iter2 >::apply ( x, y ) ;
    }
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct DestroyReferenced
    {
        ALGO_INLINE
        static void apply ( T& x )
        {
            x.~T () ;
        }
    } ;
    
    template < typename T >
    struct DestroyReferenced < T, typename std::enable_if < ALGO_CALL::IsTriviallyDestructible < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        static void apply ( T& x )
        {}
    } ;
    
    template < typename T >
    ALGO_INLINE
    void destroyReferenced ( T& x )
    {
        return ALGO_CALL::DestroyReferenced < T >::apply ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct DestroyPointed ;
    
    template < typename Iter >
    struct DestroyPointed < Iter
        , typename std::enable_if < ALGO_CALL::IsNotProxiedIterator < Iter >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( Iter x )
        {
            ALGO_CALL::DestroyReferenced < typename ALGO_CALL::IteratorTraits < Iter >::value_type >::apply ( ALGO_CALL::Deref < Iter >::apply ( x ) ) ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    void destroyPointed ( Iter x )
    {
        return ALGO_CALL::DestroyPointed < Iter >::apply ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct AssignImpl
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        template < typename T >
        ALGO_INLINE
        static void apply ( Iter x, T&& y )
        {
            ALGO_CALL::Deref < Iter >::apply ( x ) = std::forward < T > ( y ) ;
        }
    } ;
    
    template < typename Iter, typename T >
    ALGO_INLINE
    void assignImpl ( Iter x, T&& y )
    {
        ALGO_CALL::AssignImpl < Iter >::apply ( x, std::forward < T > ( y ) ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct Assign
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::AssignImpl < O >::apply ( o, ALGO_CALL::Deref < I >::apply ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void assign ( I i, O o )
    {
        ALGO_CALL::Assign < I, O >::apply ( i , o ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct MoveAssign
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            // Disable if i is a proxy?
            ALGO_CALL::AssignImpl < O >::apply ( o, ALGO_CALL::DerefMove < I >::apply ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void moveAssign ( I i, O o )
    {
        ALGO_CALL::MoveAssign < I, O >::apply ( i , o ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct ConstructImpl
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        template < typename T >
        ALGO_INLINE
        static void apply ( Iter x, T&& y )
        {
            typedef typename ALGO_CALL::IteratorTraits < Iter >::value_type IterValue ;
            
            new ( ALGO_CALL::UnderlyingAddressOf < Iter >::apply ( x ) ) IterValue ( std::forward < T > ( y ) );
        }
    } ;
    
    template < typename Iter, typename T >
    ALGO_INLINE
    void constructImpl ( Iter x, T&& y )
    {
        ALGO_CALL::ConstructImpl < Iter >::apply ( x, std::forward < T > ( y ) ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct CopyConstruct
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        // Default for proxied iterators is to assign
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::Assign < I, O >::apply ( i, o ) ;
        }
    } ;
    
    template <typename I, typename O >
    struct CopyConstruct < I, O
        , typename std::enable_if < ALGO_CALL::IsNotProxiedIterator < I >::value
            && ALGO_CALL::IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::ConstructImpl < O >::apply ( o, ALGO_CALL::Deref < I >::apply ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void copyConstruct ( I i, O o )
    {
        ALGO_CALL::CopyConstruct < I, O >::apply ( i , o ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct MoveConstruct
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        // Default for proxied iterators is to assign
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::Assign < I, O >::apply ( i, o ) ;
        }
    } ;
    
    template < typename I, typename O >
    struct MoveConstruct < I, O
        , typename std::enable_if< IsNotProxiedIterator < I >::value && IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::ConstructImpl < O >::apply ( o, ALGO_CALL::DerefMove < I >::apply ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void moveConstruct ( I i, O o )
    {
        ALGO_CALL::MoveConstruct < I, O >::apply ( i , o ) ;
    }
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct IterSwap
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            std::iter_swap ( i, o ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void iterSwap ( I i, O o )
    {
        ALGO_STATIC_ASSERT ( (std::is_same < typename ALGO_CALL::IteratorTraits < O >::value_type, typename ALGO_CALL::IteratorTraits < I >::value_type > ()), "Input and Output value_types need to be the same" ) ;
        
        ALGO_CALL::IterSwap < I, O >::apply ( i , o ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct CopyBytesNotOverlapped
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( I i, O o, size_t n )
        {
            ALGO_ASSERT ( std::numeric_limits < size_t >::max () / n > sizeof ( typename ALGO_CALL::IteratorTraits < I >::value_type ) ) ;
            
            size_t const len = sizeof ( typename ALGO_CALL::IteratorTraits < I >::value_type ) * n ;
            
            std::memcpy ( ALGO_CALL::underlyingAddressOf ( o )
                         , ALGO_CALL::underlyingAddressOf ( i )
                         , len ) ;
        }
    } ;
    
    // [o, o + n) and [i, i + n) must not overlap
    template < typename I, typename O, typename N >
    ALGO_INLINE
    void copyBytesNotOverlapped ( I i, O o, N n )
    {
        ALGO_STATIC_ASSERT ( (std::is_same < typename ALGO_CALL::IteratorTraits < O >::value_type, typename ALGO_CALL::IteratorTraits < I >::value_type > ()), "Input and Output value_types need to be the same" ) ;
        
        if ( !n ) return ;
        
        ALGO_ASSERT ( n > 0 ) ;
        ALGO_ASSERT ( std::numeric_limits < size_t >::max () >= n ) ;
        
        ALGO_CALL::CopyBytesNotOverlapped < I, O >::apply ( i, o, size_t ( n ) ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct CopyBytesOverlapped
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( O o, I i, size_t n )
        {
            ALGO_ASSERT ( std::numeric_limits < size_t >::max () / n > sizeof ( typename ALGO_CALL::IteratorTraits < I >::value_type ) ) ;
            
            size_t const len = sizeof ( typename ALGO_CALL::IteratorTraits < I >::value_type ) * n ;
            
            std::memmove ( ALGO_CALL::underlyingAddressOf ( o )
                          , ALGO_CALL::underlyingAddressOf ( i )
                         , len ) ;
        }
    } ;
    
    // [o, o + n) and [i, i + n) may overlap
    template < typename I, typename O, typename N >
    ALGO_INLINE
    void copyBytesOverlapped ( O o, I i, N n )
    {
        ALGO_STATIC_ASSERT ( (std::is_same < typename ALGO_CALL::IteratorTraits < O >::value_type, typename ALGO_CALL::IteratorTraits < I >::value_type > ()), "Input and Output value_types need to be the same" ) ;
        
        if ( !n ) return ;
        
        ALGO_ASSERT ( n > 0 ) ;
        ALGO_ASSERT ( std::numeric_limits < size_t >::max () >= n ) ;
        
        ALGO_CALL::CopyBytesOverlapped < I, O >::apply ( o, i, size_t ( n ) ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct StripIter
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        typedef Iter type ;
        
        ALGO_INLINE
        static type apply ( Iter x )
        {
            return x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::StripIter < Iter >::type stripIter ( Iter x )
    {
        return ALGO_CALL::StripIter < Iter >::apply ( x ) ;
    }
    
    
    
    template < typename ReturnIter, typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct UnstripIter
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ReturnIter >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static ReturnIter apply ( Iter x )
        {
            return x ;
        }
    } ;
    
    template < typename ReturnIter, typename Iter >
    ALGO_INLINE
    ReturnIter unstripIter ( Iter x )
    {
        return ALGO_CALL::UnstripIter < ReturnIter, Iter >::apply ( x ) ;
    }

} // namespace algo

#endif
