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
    struct Predecessor ;
    
    template < typename BidirectionalIterator >
    struct Predecessor < BidirectionalIterator
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsRealStdIterator < BidirectionalIterator >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Predecessor type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < BidirectionalIterator >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::CheckIteratorCategory < BidirectionalIterator, std::bidirectional_iterator_tag >::type::value ), "Must be bidirectional iterator" ) ;
        
        ALGO_INLINE
        static void apply ( BidirectionalIterator& x )
            ALGO_NOEXCEPT_DECL ( noexcept ( --x ) )
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
    struct Successor ;
    
    template < typename ForwardIterator >
    struct Successor < ForwardIterator
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsRealStdIterator < ForwardIterator >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Successor type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( ForwardIterator& x )
            ALGO_NOEXCEPT_DECL ( noexcept ( ++x ) )
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
    
    
    
    template < typename ForwardIterator1, typename ForwardIterator2 ALGO_COMMA_ENABLE_IF_PARAM >
    struct Distance ;
    
    template < typename ForwardIterator1, typename ForwardIterator2 >
    struct Distance < ForwardIterator1, ForwardIterator2
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsRealStdIterator < ForwardIterator1 >
            , ALGO_CALL::IsRealStdIterator < ForwardIterator2 > >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Distance type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator1 >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator2 >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < ForwardIterator1 >::difference_type apply ( ForwardIterator1 x, ForwardIterator2 y )
            ALGO_NOEXCEPT_DECL ( noexcept ( std::distance ( x, y ) ) )
        {
            return std::distance ( x, y ) ;
        }
    } ;
    
    template < typename ForwardIterator1, typename ForwardIterator2 >
    ALGO_INLINE
    typename ALGO_CALL::IteratorTraits < ForwardIterator1 >::difference_type distance ( ForwardIterator1 x, ForwardIterator2 y )
    {
        return ALGO_CALL::Distance < ForwardIterator1, ForwardIterator2 >::apply ( x, y ) ;
    }
    
    
    
    template < typename ForwardIterator ALGO_COMMA_ENABLE_IF_PARAM >
    struct Advance ;
    
    template < typename ForwardIterator >
    struct Advance < ForwardIterator
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsRealStdIterator < ForwardIterator >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Advance type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( ForwardIterator& x, typename ALGO_CALL::IteratorTraits < ForwardIterator >::difference_type n )
            ALGO_NOEXCEPT_DECL ( noexcept ( std::advance ( x, n ) ) )
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
    struct IsEmpty ;
    
    template < typename ForwardIterator >
    struct IsEmpty < ForwardIterator
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsRealStdIterator < ForwardIterator >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef IsEmpty type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < ForwardIterator >::type::value ), "Must be an iterator" ) ;
        
        ALGO_NO_OP_FUNCTION
        ALGO_INLINE
        static bool apply ( ForwardIterator )
            ALGO_NOEXCEPT_DECL ( true )
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
    struct Deref ;
    
    template < typename Iter >
    struct Deref < Iter
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsRealStdIterator < Iter >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef Deref type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Iter >::reference apply ( Iter x )
            ALGO_NOEXCEPT_DECL ( noexcept ( *x ) )
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
        typedef DerefMove type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static typename ALGO_CALL::IteratorTraits < Iter >::value_type&& apply ( Iter x )
        {
            return ALGO_CALL::move ( ALGO_CALL::Deref < Iter >::apply ( x ) ) ;
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
        typedef AddressOf type ;
        
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
        typedef UnderlyingAddressOf type ;
        
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
        typedef UnderlyingAddressOf type ;
        
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
        typedef EqualUnderlyingAddress type ;
        
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
        typedef DestroyReferenced type ;
        
        ALGO_INLINE
        static void apply ( T& x )
        {
            x.~T () ;
        }
    } ;
    
    template < typename T >
    struct DestroyReferenced < T, typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsTriviallyDestructible < T >, ALGO_ENABLE_IF_PARAM_DEFAULT >::type >
    {
        typedef DestroyReferenced type ;
        
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
        , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::IsNotProxiedIterator < Iter >, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        typedef DestroyPointed type ;
        
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
        typedef AssignImpl type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        template < typename T >
        ALGO_INLINE
        static void apply ( T&& x, Iter y )
        {
            ALGO_CALL::Deref < Iter >::apply ( y ) = ALGO_CALL::forward < T > ( x ) ;
        }
    } ;
    
    template < typename T, typename Iter >
    ALGO_INLINE
    void assignImpl ( T&& x, Iter y )
    {
        ALGO_CALL::AssignImpl < Iter >::apply ( ALGO_CALL::forward < T > ( x ), y ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct Assign
    {
        typedef Assign type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::AssignImpl < O >::apply ( ALGO_CALL::Deref < I >::apply ( i ), o ) ;
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
        typedef MoveAssign type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            // Disable if i is a proxy?
            ALGO_CALL::AssignImpl < O >::apply ( ALGO_CALL::DerefMove < I >::apply ( i ), o ) ;
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
        typedef ConstructImpl type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        template < typename T >
        ALGO_INLINE
        static void apply ( T&& x, Iter y )
        {
            typedef typename ALGO_CALL::IteratorTraits < Iter >::value_type IterValue ;
            
            new ( ALGO_CALL::UnderlyingAddressOf < Iter >::apply ( y ) ) IterValue ( ALGO_CALL::forward < T > ( x ) );
        }
    } ;
    
    template < typename T, typename Iter >
    ALGO_INLINE
    void constructImpl ( T&& x, Iter y )
    {
        ALGO_CALL::ConstructImpl < Iter >::apply ( ALGO_CALL::forward < T > ( x ), y ) ;
    }
    
    
    
    template < typename I, typename O ALGO_COMMA_ENABLE_IF_PARAM >
    struct CopyConstruct
    {
        typedef CopyConstruct type ;
        
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
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ < ALGO_CALL::IsNotProxiedIterator < I >
            , ALGO_CALL::IsNotProxiedIterator < O > >, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        typedef CopyConstruct type ;
        
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::ConstructImpl < O >::apply ( ALGO_CALL::Deref < I >::apply ( i ), o ) ;
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
        typedef MoveConstruct type ;
        
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
    , typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_LOGIC_CALL::and_ <
            ALGO_CALL::IsNotProxiedIterator < I >
            , ALGO_CALL::IsNotProxiedIterator < O > >, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        typedef MoveConstruct type ;
        
        ALGO_INLINE
        static void apply ( I i, O o )
        {
            ALGO_CALL::ConstructImpl < O >::apply ( ALGO_CALL::DerefMove < I >::apply ( i ), o ) ;
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
        typedef IterSwap type ;
        
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
        typedef CopyBytesNotOverlapped type ;
        
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
        typedef CopyBytesOverlapped type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < I >::type::value ), "Must be an iterator" ) ;
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < O >::type::value ), "Must be an iterator" ) ;
        
        ALGO_INLINE
        static void apply ( I i, O o, size_t n )
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
    void copyBytesOverlapped ( I i, O o, N n )
    {
        ALGO_STATIC_ASSERT ( (std::is_same < typename ALGO_CALL::IteratorTraits < O >::value_type, typename ALGO_CALL::IteratorTraits < I >::value_type > ()), "Input and Output value_types need to be the same" ) ;
        
        if ( !n ) return ;
        
        ALGO_ASSERT ( n > 0 ) ;
        ALGO_ASSERT ( std::numeric_limits < size_t >::max () >= n ) ;
        
        ALGO_CALL::CopyBytesOverlapped < I, O >::apply ( i, o, size_t ( n ) ) ;
    }
    
    
    
    template < typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct StripIter
    {
        typedef StripIter type ;
        
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasIteratorTraits < Iter >::type::value ), "Must be an iterator" ) ;
        
        typedef Iter returnType ;
        
        ALGO_INLINE
        static returnType apply ( Iter x )
        {
            return x ;
        }
    } ;
    
    template < typename Iter >
    ALGO_INLINE
    typename ALGO_CALL::StripIter < Iter >::returnType stripIter ( Iter x )
    {
        return ALGO_CALL::StripIter < Iter >::apply ( x ) ;
    }
    
    
    
    template < typename ReturnIter, typename Iter ALGO_COMMA_ENABLE_IF_PARAM >
    struct UnstripIter
    {
        typedef UnstripIter type ;
        
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
