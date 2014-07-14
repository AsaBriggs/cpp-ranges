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
        ALGO_INLINE
        void operator () ( BidirectionalIterator& x ) const
        {
            --x ;
        }
    } ;
    
    template  < typename BidirectionalIterator >
    ALGO_INLINE
    BidirectionalIterator predecessor ( BidirectionalIterator x, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
    {
        ALGO_CALL::Predecessor < BidirectionalIterator > () ( x ) ;
        return x ;
    }
    
    template < typename BidirectionalIterator >
    ALGO_INLINE
    void predecessor ( BidirectionalIterator& x, ALGO_CALL::InPlace )
    {
        ALGO_CALL::Predecessor < BidirectionalIterator > () ( x ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Successor
    {
        ALGO_INLINE
        void operator () ( ForwardIterator& x ) const
        {
            ++x ;
        }
    } ;
    
    template < typename ForwardIterator >
    ALGO_INLINE
    ForwardIterator successor ( ForwardIterator x, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
    {
        ALGO_CALL::Successor < ForwardIterator > () ( x ) ;
        return x ;
    }
    
    template < typename ForwardIterator >
    ALGO_INLINE
    void successor ( ForwardIterator& x, ALGO_CALL::InPlace )
    {
        ALGO_CALL::Successor < ForwardIterator > () ( x ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Distance
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type operator () ( ForwardIterator x, ForwardIterator y ) const
        {
            return std::distance ( x, y ) ;
        }
    } ;
    
    template < typename ForwardIterator >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type distance ( ForwardIterator x, ForwardIterator y )
    {
        return ALGO_CALL::Distance < ForwardIterator > () ( x, y ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Advance
    {
        ALGO_INLINE
        void operator () ( ForwardIterator& x, typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type n ) const
        {
            std::advance ( x, n ) ;
        }
    } ;
    
    template < typename ForwardIterator, typename N >
    ALGO_INLINE
    ForwardIterator advance ( ForwardIterator x, N n, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
    {
        typedef typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type difference_type ;
        
        ALGO_ASSERT ( std::numeric_limits < difference_type >::max () >= n ) ;
        ALGO_ASSERT ( std::numeric_limits < difference_type >::min () <= n ) ;
        
        ALGO_CALL::Advance < ForwardIterator > () ( x, difference_type ( n ) ) ;
        return x ;
    }
    
    template < typename ForwardIterator, typename N >
    ALGO_INLINE
    void advance ( ForwardIterator& x, N n, ALGO_CALL::InPlace )
    {
        typedef typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type difference_type ;
        
        ALGO_ASSERT ( std::numeric_limits < difference_type >::max () >= n ) ;
        ALGO_ASSERT ( std::numeric_limits < difference_type >::min () <= n ) ;
        
        ALGO_CALL::Advance < ForwardIterator > () ( x, difference_type ( n ) ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct IsEmpty
    {
        ALGO_INLINE
        bool operator () ( ForwardIterator& x ) const
        {
            // TODO figure out if there should be a null-check here ...
            return false ;
        }
    } ;
    
    template < typename Iterator >
    ALGO_INLINE
    bool isEmpty ( Iterator x )
    {
        return IsEmpty < Iterator > () ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct Deref
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < Iter >::reference operator () ( Iter x ) const
        {
            return *x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < Iter >::reference deref ( Iter x )
    {
        return ALGO_CALL::Deref < Iter > () ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct DerefMove
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < Iter >::value_type&& operator () ( Iter x ) const
        {
            return std::move ( ALGO_CALL::deref ( x ) ) ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < Iter >::value_type&& derefMove ( Iter x )
    {
        return ALGO_CALL::DerefMove < Iter > () ( x ) ;
    }
    
    
    
    template < typename X ALGO_COMMA_ENABLE_IF_PARAM >
    struct AddressOf
    {
        ALGO_INLINE
        X* operator () ( X& x ) const
        {
            return &x ;
        }
    } ;
    
    template < typename X >
    ALGO_INLINE
    X* addressOf ( X& x )
    {
        return ALGO_CALL::AddressOf < X > () ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct UnderlyingAddressOf
    {
        ALGO_INLINE
        typename ALGO_CALL::IteratorTraits < Iter >::pointer operator () ( Iter x ) const
        {
            return ALGO_CALL::addressOf ( ALGO_CALL::deref ( x ) ) ;
        }
    } ;
    
    template < typename X >
    struct UnderlyingAddressOf < X*, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        ALGO_INLINE
        X* operator () ( X* x ) const
        {
            return x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < Iter >::pointer underlyingAddressOf ( Iter x )
    {
        return ALGO_CALL::UnderlyingAddressOf < Iter > () ( x ) ;
    }
    
    
    
    template < typename Iter1, typename Iter2 ALGO_COMMA_ENABLE_IF_PARAM >
    struct EqualUnderlyingAddress
    {
        ALGO_INLINE
        bool operator () ( Iter1 x, Iter2 y ) const
        {
            return reinterpret_cast < const volatile void* > ( ALGO_CALL::underlyingAddressOf ( x ) )
            == reinterpret_cast < const volatile void* > ( ALGO_CALL::underlyingAddressOf ( y ) ) ;
        }
    } ;
    
    template < typename Iter1, typename Iter2 >
    ALGO_INLINE
    bool equalUnderlyingAddress ( Iter1 x, Iter2 y )
    {
        // Requires x & y are dereferencable
        return ALGO_CALL::EqualUnderlyingAddress < Iter1, Iter2 > () ( x, y ) ;
    }
    
    
    
    template < typename T ALGO_COMMA_ENABLE_IF_PARAM >
    struct DestroyReferenced
    {
        ALGO_INLINE
        void operator () ( T& x ) const
        {
            x.~T () ;
        }
    } ;
    
    template < typename T >
    struct DestroyReferenced < T, typename std::enable_if < ALGO_CALL::IsTriviallyDestructible < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        ALGO_INLINE
        void operator () ( T& x ) const
        {}
    } ;
    
    template < typename T >
    ALGO_INLINE
    void destroyReferenced ( T& x )
    {
        return ALGO_CALL::DestroyReferenced < T > () ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct DestroyPointed ;
    
    template < typename Iter >
    struct DestroyPointed < Iter,
    typename std::enable_if < ALGO_CALL::IsNotProxiedIterator < Iter >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_INLINE
        void operator () ( Iter x ) const
        {
            destroyReferenced ( ALGO_CALL::deref ( x ) ) ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    void destroyPointed ( Iter x )
    {
        return ALGO_CALL::DestroyPointed < Iter > () ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct AssignImpl
    {
        template < typename T >
        ALGO_INLINE
        void operator () ( Iter x, T&& y ) const
        {
            ALGO_CALL::deref ( x ) = std::forward < T > ( y ) ;
        }
    } ;
    
    template < typename Iter, typename T >
    ALGO_INLINE
    void assignImpl ( Iter x, T&& y )
    {
        ALGO_CALL::AssignImpl < Iter > () ( x, std::forward < T > ( y ) ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct Assign
    {
        ALGO_INLINE
        void operator () ( I i, O o ) const
        {
            ALGO_CALL::assignImpl ( o, ALGO_CALL::deref ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void assign ( I i, O o )
    {
        ALGO_CALL::Assign < I, O > () ( i , o ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct MoveAssign
    {
        ALGO_INLINE
        void operator () ( I i, O o ) const
        {
            // Disable if i is a proxy?
            ALGO_CALL::assignImpl ( o, ALGO_CALL::derefMove ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void moveAssign ( I i, O o )
    {
        ALGO_CALL::MoveAssign < I, O > () ( i , o ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct ConstructImpl
    {
        template < typename T >
        ALGO_INLINE
        void operator () ( Iter x, T&& y ) const
        {
            typedef typename ALGO_CALL::IteratorTraits < Iter >::value_type IterValue ;
            
            new ( ALGO_CALL::underlyingAddressOf ( x ) ) IterValue ( std::forward < T > ( y ) );
        }
    } ;
    
    template < typename Iter, typename T >
    ALGO_INLINE
    void constructImpl ( Iter x, T&& y )
    {
        ALGO_CALL::ConstructImpl < Iter > () ( x, std::forward < T > ( y ) ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct CopyConstruct
    {
        // Default for proxied iterators is to assign
        ALGO_INLINE
        void operator () ( I i, O o ) const
        {
            ALGO_CALL::assign ( i, o ) ;
        }
    } ;
    
    template <typename I, typename O >
    struct CopyConstruct < I, O,
    typename std::enable_if < ALGO_CALL::IsNotProxiedIterator < I >::value && ALGO_CALL::IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_INLINE
        void operator () ( I i, O o ) const
        {
            ALGO_CALL::constructImpl ( o, ALGO_CALL::deref ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void copyConstruct ( I i, O o )
    {
        ALGO_CALL::CopyConstruct < I, O > () ( i , o ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct MoveConstruct
    {
        // Default for proxied iterators is to assign
        ALGO_INLINE
        void operator () ( I i, O o ) const
        {
            ALGO_CALL::assign ( i, o ) ;
        }
    } ;
    
    template < typename I, typename O >
    struct MoveConstruct < I, O,
    typename std::enable_if< IsNotProxiedIterator < I >::value && IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_INLINE
        void operator () ( I i, O o ) const
        {
            ALGO_CALL::constructImpl ( o, ALGO_CALL::derefMove ( i ) ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void moveConstruct ( I i, O o )
    {
        ALGO_CALL::MoveConstruct < I, O > () ( i , o ) ;
    }
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct IterSwap
    {
        ALGO_INLINE
        void operator () ( I i, O o ) const
        {
            std::iter_swap ( i, o ) ;
        }
    } ;
    
    template < typename I, typename O >
    ALGO_INLINE
    void iterSwap ( I i, O o )
    {
        static_assert ( std::is_same < typename ALGO_CALL::IteratorTraits < O >::value_type, typename ALGO_CALL::IteratorTraits < I >::value_type > (), "Input and Output value_types need to be the same" ) ;
        
        ALGO_CALL::IterSwap < I, O > () ( i , o ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct CopyBytesNotOverlapped
    {
        ALGO_INLINE
        void operator () ( O o, I i, size_t n ) const
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
    void copyBytesNotOverlapped ( O o, I i, N n )
    {
        static_assert ( std::is_same < typename ALGO_CALL::IteratorTraits < O >::value_type, typename ALGO_CALL::IteratorTraits < I >::value_type > (), "Input and Output value_types need to be the same" ) ;
        
        if ( !n ) return ;
        
        ALGO_ASSERT ( n > 0 ) ;
        ALGO_ASSERT ( std::numeric_limits < size_t >::max () >= n ) ;
        
        ALGO_CALL::CopyBytesNotOverlapped < I, O > () ( o, i, size_t ( n ) ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct CopyBytesOverlapped
    {
        ALGO_INLINE
        void operator () ( O o, I i, size_t n ) const
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
        static_assert ( std::is_same < typename ALGO_CALL::IteratorTraits < O >::value_type, typename ALGO_CALL::IteratorTraits < I >::value_type > (), "Input and Output value_types need to be the same" ) ;
        
        if ( !n ) return ;
        
        ALGO_ASSERT ( n > 0 ) ;
        ALGO_ASSERT ( std::numeric_limits < size_t >::max () >= n ) ;
        
        ALGO_CALL::CopyBytesOverlapped < I, O > () ( o, i, size_t ( n ) ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct StripIter
    {
        typedef Iter type ;
        
        ALGO_INLINE
        type operator () ( Iter x ) const
        {
            return x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::StripIter < Iter >::type stripIter ( Iter x )
    {
        return ALGO_CALL::StripIter < Iter > () ( x ) ;
    }
    
    
    
    template < typename ReturnIter, typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct UnstripIter
    {
        ALGO_INLINE
        ReturnIter operator () ( Iter x ) const
        {
            return x ;
        }
    } ;
    
    template < typename ReturnIter, typename Iter >
    ALGO_INLINE
    ReturnIter unstripIter ( Iter x )
    {
        return ALGO_CALL::UnstripIter < ReturnIter, Iter > () ( x ) ;
    }

} // namespace algo

#endif
