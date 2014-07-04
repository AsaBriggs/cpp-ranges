#ifndef INCLUDED_ALGO_ITERATOR
#define INCLUDED_ALGO_ITERATOR

#ifndef INCLUDED_ALGO_BASICS
#include "algo_basics.h"
#endif

#ifndef INCLUDED_ALGO_TRAITS
#include "algo_traits.h"
#endif

namespace algo
{
    template < typename BidirectionalIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Predecessor
    {
        ALGO_INLINE
        BidirectionalIterator operator () ( BidirectionalIterator x, ALGO_CALL::ByReturnValue ) const
        {
            return --x ;
        }
        
        ALGO_INLINE
        void operator () ( BidirectionalIterator& x, ALGO_CALL::InPlace ) const
        {
            --x ;
        }
    } ;
    
    template  < typename BidirectionalIterator >
    ALGO_INLINE
    BidirectionalIterator predecessor ( BidirectionalIterator x, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
    {
        return ALGO_CALL::Predecessor < BidirectionalIterator > () ( x, ALGO_CALL::ByReturnValue () ) ;
    }
    
    template < typename BidirectionalIterator >
    ALGO_INLINE
    void predecessor ( BidirectionalIterator& x, ALGO_CALL::InPlace )
    {
        ALGO_CALL::Predecessor < BidirectionalIterator > () ( x, ALGO_CALL::InPlace () ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Successor
    {
        ALGO_INLINE
        ForwardIterator operator () ( ForwardIterator x, ALGO_CALL::ByReturnValue ) const
        {
            return ++x ;
        }
        
        ALGO_INLINE
        void operator () ( ForwardIterator& x, ALGO_CALL::InPlace ) const
        {
            ++x ;
        }
    } ;
    
    template < typename ForwardIterator >
    ALGO_INLINE
    ForwardIterator successor ( ForwardIterator x, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
    {
        return ALGO_CALL::Successor < ForwardIterator > () ( x, ALGO_CALL::ByReturnValue () ) ;
    }
    
    template < typename ForwardIterator >
    ALGO_INLINE
    void successor ( ForwardIterator& x, ALGO_CALL::InPlace )
    {
        ALGO_CALL::Successor < ForwardIterator > () ( x, ALGO_CALL::InPlace () ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Distance
    {
        ALGO_INLINE
        typename std::iterator_traits < ForwardIterator >::difference_type operator () ( ForwardIterator x, ForwardIterator y ) const
        {
            return std::distance ( x, y ) ;
        }
    } ;
    
    template < typename ForwardIterator >
    ALGO_INLINE
    typename std::iterator_traits < ForwardIterator >::difference_type distance ( ForwardIterator x, ForwardIterator y )
    {
        return ALGO_CALL::Distance < ForwardIterator > () ( x, y ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Advance
    {
        ALGO_INLINE
        ForwardIterator operator () ( ForwardIterator x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::ByReturnValue ) const
        {
            std::advance ( x, n ) ;
            return x ;
        }
        
        ALGO_INLINE
        void operator () ( ForwardIterator& x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::InPlace ) const
        {
            std::advance ( x, n ) ;
        }
    } ;
    
    template < typename ForwardIterator >
    ALGO_INLINE
    ForwardIterator advance ( ForwardIterator x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::ByReturnValue = ALGO_CALL::ByReturnValue () )
    {
        return ALGO_CALL::Advance < ForwardIterator > () ( x, n, ByReturnValue () ) ;
    }
    
    template < typename ForwardIterator >
    ALGO_INLINE
    void advance ( ForwardIterator& x, typename std::iterator_traits < ForwardIterator >::difference_type n, ALGO_CALL::InPlace )
    {
        ALGO_CALL::Advance < ForwardIterator > () ( x, n, ALGO_CALL::InPlace () ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct Deref
    {
        ALGO_INLINE
        typename std::iterator_traits < Iter >::reference operator () ( Iter x ) const
        {
            return *x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename std::iterator_traits < Iter >::reference deref ( Iter x )
    {
        return ALGO_CALL::Deref < Iter > () ( x ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct DerefMove
    {
        ALGO_INLINE
        typename std::iterator_traits < Iter >::value_type&& operator () ( Iter x ) const
        {
            return std::move ( *x ) ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename std::iterator_traits < Iter >::value_type&& derefMove ( Iter x )
    {
        return std::forward < typename std::iterator_traits < Iter >::value_type > ( ALGO_CALL::DerefMove < Iter > () ( x ) ) ;
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
    
    
    
    template < typename Iter1, typename Iter2 ALGO_COMMA_ENABLE_IF_PARAM >
    struct EqualUnderlyingAddress
    {
        ALGO_INLINE
        bool operator () ( Iter1 x, Iter2 y ) const
        {
            return reinterpret_cast < const volatile void* > ( ALGO_CALL::addressOf ( ALGO_CALL::deref ( x ) ) )
            == reinterpret_cast < const volatile void* > ( ALGO_CALL::addressOf ( ALGO_CALL::deref ( y ) ) ) ;
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
            ALGO_CALL::assignImpl ( o, std::forward < typename std::iterator_traits< I >::value_type > ( ALGO_CALL::derefMove ( i ) ) ) ;
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
            typedef typename std::iterator_traits < Iter >::value_type IterValue ;
            
            new ( ALGO_CALL::addressOf ( ALGO_CALL::deref ( x ) ) ) IterValue ( std::forward < T > ( y ) );
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
            ALGO_CALL::constructImpl ( o, std::forward < typename std::iterator_traits< I >::value_type > ( ALGO_CALL::derefMove ( i ) ) ) ;
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
        ALGO_CALL::IterSwap < I, O > () ( i , o ) ;
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
