#ifndef INCLUDED_ALGO_H
#define INCLUDED_ALGO_H

#include <type_traits>
#include <iterator>
#include <utility>
#include <memory>
#include <cstdlib>
#include <cassert>

struct EnableIfDeductionType {};

#define ALGO_INLINE __attribute__((always_inline)) inline
#define ALGO_ASSERT(x) assert ( (x) )
#define ALGO_SAFE_MODE(x) x
#define ALGO_ENABLE_IF_PARAM_DEFAULT EnableIfDeductionType
#define ALGO_ENABLE_IF_PARAM , typename enable = ALGO_ENABLE_IF_PARAM_DEFAULT
#define ALGO_LIKELIHOOD(test, likelihood) __builtin_expect ( !!(test), (likelihood) )

namespace algo
{

struct InPlace {} ;
struct ByReturnValue {} ;
    
    
    
template < typename T ALGO_ENABLE_IF_PARAM >
struct IsBitwiseCopyable : std::is_trivially_copy_assignable<T>
{};
    
template < typename T, int N >
struct IsBitwiseCopyable < T [ N ], ALGO_ENABLE_IF_PARAM_DEFAULT > : IsBitwiseCopyable < T >
{};
    
template < typename T, typename U >
struct IsBitwiseCopyable < std::pair < T, U >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    : std::integral_constant<bool, IsBitwiseCopyable < T >::value && IsBitwiseCopyable < U >::value >
{};
    
    
    
template < class T ALGO_ENABLE_IF_PARAM >
struct IsBitwiseMoveable : IsBitwiseCopyable<T>
{};
    
// shared pointer, managed pointer, containers that have no back-pointers from remote parts.

    
    
template < typename Iter >
struct IsNotProxiedIterator
    : std::is_same< typename std::remove_cv< typename std::iterator_traits < Iter >::reference >::type,
                    typename std::remove_cv< typename std::iterator_traits < Iter >::value_type >::type& >
{};


    
template < class T ALGO_ENABLE_IF_PARAM >
struct IsTriviallyDestructible : std::is_trivially_destructible < T>
{};

    
    
template < typename BidirectionalIterator ALGO_ENABLE_IF_PARAM >
struct Predecessor
{
    ALGO_INLINE
    BidirectionalIterator operator () (BidirectionalIterator x, ByReturnValue) const
    {
        return --x;
    }
    
    ALGO_INLINE
    void operator () (BidirectionalIterator& x, InPlace) const
    {
        --x;
    }
};

template <typename BidirectionalIterator>
ALGO_INLINE
BidirectionalIterator predecessor(BidirectionalIterator x, ByReturnValue = ByReturnValue ())
{
    return Predecessor <BidirectionalIterator> () ( x, ByReturnValue () );
}

template <typename BidirectionalIterator>
ALGO_INLINE
void predecessor(BidirectionalIterator& x, InPlace)
{
    Predecessor <BidirectionalIterator> () ( x, InPlace () );
}



template < typename ForwardIterator ALGO_ENABLE_IF_PARAM >
struct Successor
{
    ALGO_INLINE
    ForwardIterator operator () (ForwardIterator x, ByReturnValue) const
    {
        return ++x ;
    }
    
    ALGO_INLINE
    void operator () (ForwardIterator& x, InPlace) const
    {
        ++x ;
    }
} ;

template <typename ForwardIterator>
ALGO_INLINE
ForwardIterator successor(ForwardIterator x, ByReturnValue = ByReturnValue () )
{
    return Successor < ForwardIterator > () ( x, ByReturnValue () ) ;
}

template <typename ForwardIterator>
ALGO_INLINE
void successor(ForwardIterator& x, InPlace)
{
    Successor < ForwardIterator > () ( x, InPlace () ) ;
}



template < typename ForwardIterator ALGO_ENABLE_IF_PARAM >
struct Distance
{
    ALGO_INLINE
    typename std::iterator_traits < ForwardIterator >::difference_type operator () ( ForwardIterator x, ForwardIterator y ) const
    {
        return std::distance ( x, y ) ;
    }
};

template <typename ForwardIterator>
ALGO_INLINE
typename std::iterator_traits < ForwardIterator >::difference_type distance ( ForwardIterator x, ForwardIterator y )
{
    return Distance < ForwardIterator > () ( x, y ) ;
}



template < typename ForwardIterator ALGO_ENABLE_IF_PARAM >
struct Advance
{
    ALGO_INLINE
    ForwardIterator operator () ( ForwardIterator x, typename std::iterator_traits < ForwardIterator >::difference_type n ) const
    {
        return std::advance ( x, n ) ;
    }
};

template <typename ForwardIterator>
ALGO_INLINE
ForwardIterator advance ( ForwardIterator x, typename std::iterator_traits < ForwardIterator >::difference_type n )
{
    return Advance < ForwardIterator > () ( x, n ) ;
}



template <typename Iter ALGO_ENABLE_IF_PARAM >
struct Deref
{
    ALGO_INLINE
    typename std::iterator_traits<Iter>::reference operator () (Iter x) const
    {
        return *x ;
    }
};

template < class Iter >
ALGO_INLINE
typename std::iterator_traits<Iter>::reference deref ( Iter x )
{
    return Deref < Iter > () ( x ) ;
}



template <typename X ALGO_ENABLE_IF_PARAM >
struct AddressOf
{
    ALGO_INLINE
    X* operator () (X& x) const
    {
        return &x;
    }
};

template < typename X >
ALGO_INLINE
X* addressOf ( X& x )
{
    return AddressOf < X > ( x ) ;
}



template <typename Iter ALGO_ENABLE_IF_PARAM >
struct DestroyPointed ;
    
template <typename Iter >
struct DestroyPointed < Iter,
    typename std::enable_if< IsNotProxiedIterator < Iter >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () ( Iter x ) const
    {
        deref(x).~T();
    }
};

template < typename Iter >
ALGO_INLINE
void destroyPointed ( Iter x )
{
    return DestroyPointed < Iter > () ( x ) ;
}

    
    
template < typename Iter ALGO_ENABLE_IF_PARAM >
struct AssignImpl
{
    template < typename T >
    ALGO_INLINE
    void operator () ( Iter x, T&& y ) const
    {
        deref ( x ) = std::forward < T > ( y ) ;
    }
};
    
template < typename Iter, typename T >
ALGO_INLINE
void assignImpl ( Iter x, T&& y )
{
    AssignImpl < Iter > () ( x, std::forward < T > ( y ) ) ;
}

template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct Assign
{
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        assignImpl ( o, deref(i) );
    }
};

template < typename I, typename O >
ALGO_INLINE
void assign (I i, O o)
{
    Assign < I, O > () ( i , o ) ;
}



template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct MoveAssign
{
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        // Disable if i is a proxy?
        assignImpl ( o, std::move(deref(i)) );
    }
};

template < typename I, typename O >
ALGO_INLINE
void moveAssign (I i, O o)
{
    MoveAssign < I, O > () ( i , o ) ;
}

    
    
template < typename Iter ALGO_ENABLE_IF_PARAM >
struct ConstructImpl
{
    template < typename T >
    ALGO_INLINE
    void operator () ( Iter x, T&& y ) const
    {
        typedef typename std::iterator_traits<Iter>::type TValue ;
        
        new ( addressOf ( deref ( x ) ) ) TValue ( std::forward < T > ( y ) );
    }
};

template < typename Iter, typename T >
ALGO_INLINE
void constructImpl ( Iter x, T&& y )
{
    ConstructImpl < Iter > () ( x, std::forward < T > ( y ) ) ;
}

    
    
template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct CopyConstruct
{
    // Default for proxied iterators is to assign
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        assign ( i, o ) ;
    }
};
    
template <typename I, typename O >
struct CopyConstruct < I, O,
    typename std::enable_if< IsNotProxiedIterator < I >::value && IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        constructImpl ( o, deref ( i ) ) ;
    }
};
    
template < typename I, typename O >
ALGO_INLINE
void copyConstruct (I i, O o)
{
    CopyConstruct < I, O > () ( i , o ) ;
}

    
    
template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct MoveConstruct 
{
    // Default for proxied iterators is to assign
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        assign ( i, o ) ;
    }
};

template <typename I, typename O >
struct MoveConstruct < I, O,
    typename std::enable_if< IsNotProxiedIterator < I >::value && IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        constructImpl ( o, std::move ( deref ( i ) ) ) ;
    }
};

template < typename I, typename O >
ALGO_INLINE
void moveConstruct (I i, O o)
{
    MoveConstruct < I, O > () ( i , o ) ;
}
    
    
template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct Swap ;
    
template <typename I, typename O >
struct Swap < I, O,
    typename std::enable_if< !IsNotProxiedIterator < I >::value || !IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        typename std::iterator_traits < I >::value_type tmp = *i ;
        *i = std::move ( *o ) ;
        *o = std::move ( tmp ) ;
    }
};
    
template <typename I, typename O >
struct Swap < I, O,
    typename std::enable_if< IsNotProxiedIterator < I >::value && IsNotProxiedIterator < O >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    void operator () (I i, O o) const
    {
        std::iter_swap(o, i);
    }
};

template < typename I, typename O >
ALGO_INLINE
void swap (I i, O o)
{
    Swap < I, O > () ( i , o ) ;
}

    
    
template < typename Iter ALGO_ENABLE_IF_PARAM >
struct StripIter
{
    typedef Iter type ;
    ALGO_INLINE
    type operator () ( Iter x )
    {
        return x ;
    }
};
    
template < typename Iter >
ALGO_INLINE
typename StripIter < Iter >::type stripIter ( Iter x )
{
    return StripIter < Iter >()( x ) ;
}

    
// Define a series of tags to allow composition of stepping.
    
struct Operation_tag{}; // op(Operation_tag(), i, o);

// These are called before and after the Operation_tag
struct pre_op_ad_tag{};
struct pre_op_i_tag{};
struct pre_op_o_tag{};

struct post_op_i_tag{};
struct post_op_o_tag{};
struct post_op_ad_tag{};

struct EmptyAuxilliaryData {};
    
template <typename I, typename O, typename AuxilliaryData, typename Op ALGO_ENABLE_IF_PARAM >
struct Step
{
    ALGO_INLINE
    void operator () ( I& i, O& o, AuxilliaryData& ad, Op op ) const
    {
        op(pre_op_ad_tag(), ad);
        
        op(pre_op_i_tag(), i),
        op(pre_op_o_tag(), o);
        
        op(Operation_tag(), i, o, ad);
        
        op(post_op_i_tag(), i),
        op(post_op_o_tag(), o);
        
        op(post_op_ad_tag(), ad);
    }
};

template <typename I, typename O, typename Op>
ALGO_INLINE
void step(I& i, O& o, Op op)
{
    EmptyAuxilliaryData ad ;
    Step < I, O, EmptyAuxilliaryData, Op > () ( i, o, ad, op ) ;
}

template <typename I, typename O, typename AuxilliaryData, typename Op>
ALGO_INLINE
void stepWithAuxilliaryData(I& i, O& o, AuxilliaryData& ad, Op op)
{
    Step < I, O, AuxilliaryData, Op > () ( i, o, ad, op ) ;
}

    

// Wrapper
template < template < typename I, typename O ALGO_ENABLE_IF_PARAM > class Op ALGO_ENABLE_IF_PARAM >
struct Operator
{
    template <typename I, typename O, typename AuxilliaryData>
    ALGO_INLINE
    void operator () ( Operation_tag, I i, O o, AuxilliaryData& ad ) const
    {
        Op < I, O >() ( i, o ) ;
    }
};

template < typename Op
    , template < typename Iter ALGO_ENABLE_IF_PARAM > class First
    , template < typename Iter ALGO_ENABLE_IF_PARAM > class Second ALGO_ENABLE_IF_PARAM >
struct AndOp
{
    template < typename Iter >
    ALGO_INLINE
    void operator () ( Op, Iter x ) const
    {
        First < Iter > () ( Op (), x ) ;
        Second < Iter > () ( Op (), x ) ;
    }
};
    
template <
    template < typename I, typename O ALGO_ENABLE_IF_PARAM > class Op1
    , template < typename I, typename O ALGO_ENABLE_IF_PARAM > class Op2 ALGO_ENABLE_IF_PARAM >
struct AndOperator
{
    template <typename I, typename O, typename AuxilliaryData>
    ALGO_INLINE
    void operator () ( Operation_tag, I i, O o, AuxilliaryData& ad ) const
    {
        Op1 < I, O, AuxilliaryData >() ( i, o, ad ) ;
        Op1 < I, O, AuxilliaryData >() ( i, o, ad ) ;
    }
};

struct DefaultOperations
{
    template < typename Iter, typename Op >
    ALGO_INLINE
    void operator () ( Op, Iter ) const
    {}

    // Do not provide an Operation_tag overload ... always expect the caller to provide an op (otherwise what is the point of the iteration).
};


// binds successor to the given Op
template <typename Op>
struct Forwards
{
    template <typename Iter>
    ALGO_INLINE
    void operator () (Op, Iter& x) const
    {
        successor(x, InPlace());
    }
};

// binds predecessor to the given Op
template <typename Op>
struct Backwards
{
    template <typename Iter>
    ALGO_INLINE
    void operator () (Op, Iter& x) const
    {
        predecessor(x, InPlace());
    }
};
    
    

// I forwards, O forwards
// I forwards, O backwards
// I backwards, O forwards
// I backwards, O backwards

struct IForwardsOForwards : Forwards<post_op_i_tag>, Forwards<post_op_o_tag>, DefaultOperations {};
struct IForwardsOBackwards : Forwards<post_op_i_tag>, Backwards<pre_op_o_tag>, DefaultOperations {};
struct IBackwardsOForwards : Backwards<pre_op_i_tag>, Forwards<post_op_o_tag>, DefaultOperations {};
struct IBackwardsOBackwards : Backwards<pre_op_i_tag>, Backwards<pre_op_o_tag>, DefaultOperations {};

struct CopyForwards : Operator < Assign >, IForwardsOForwards {};

template < typename I, typename O >
ALGO_INLINE
O copyImpl ( I f, I l, O o )
{
    while ( f != l )
    {
        step ( f
              , o
              , CopyForwards () ) ;
    }
    return o ;
}

    //
template < typename I, typename O >
ALGO_INLINE
typename std::enable_if < std::is_same<typename std::remove_cv<I>::type, O>::value &&
                        IsBitwiseCopyable <O>::value, O* >::type
copyImpl ( I* f, I* l, O* o )
{
    size_t diff = l - f ;
    ALGO_ASSERT ( diff >= 0 ) ;
    if ( o != f )
    {
        std::memmove ( o, f, sizeof ( I ) * diff ) ;
    }
    return o + diff ;
}
    
template < typename I, typename O >
O copy ( I f, I l, O o )
{
    if ( f == l ) return o ;
    
    return copyImpl ( stripIter ( f )
                    , stripIter ( l )
                    , stripIter ( o ) ) ;
}

    
struct CopyBackward : Operator < Assign >, IBackwardsOBackwards {} ;

template < typename I, typename O >
ALGO_INLINE
typename std::enable_if < std::is_same<typename std::remove_cv<I>::type, O>::value &&
                        IsBitwiseCopyable <O>::value, O* >::type
copyBackwardImpl ( I* f, I* l, O* o )
{
    size_t diff = l - f ;
    ALGO_ASSERT ( diff >= 0 ) ;
    copyImpl ( f, l, o - diff ) ;

    return o - diff ;
}
    
template < typename I, typename O >
ALGO_INLINE
O copyBackwardImpl ( I f, I l, O o )
{
    while ( f != l )
    {
        step ( l
                , o
                , CopyBackward () ) ;
    }
    return o ;
}
    
template < typename I, typename O>
O copy_backward ( I f, I l, O o )
{
    if ( f == l ) return o ;
    
    return copyBackwardImpl ( stripIter ( f )
                            , stripIter ( l )
                            , stripIter ( o ) ) ;
}

struct CopyNothingIForwardsO : Operator < Assign >, Forwards<post_op_o_tag>, DefaultOperations {} ;
    
template < typename Iter >
void fillImpl ( Iter f, Iter l, typename std::iterator_traits < Iter >::value_type const& value )
{
    while ( f != l )
    {
        typename std::iterator_traits < Iter >::value_type const* valuePtr = &value ;
        step ( valuePtr, f, CopyNothingIForwardsO () ) ;
    }
}
    
template < typename Iter >
void fill ( Iter f, Iter l, typename std::iterator_traits < Iter >::value_type const& value )
{
    fillImpl ( stripIter ( f ), stripIter ( l ), value ) ;
}
    
//
// SegmentedIterator ... wrapper for deque
//
//
    
// Buffer is a class that provides begin < T > and end < T > member functions, and manages the lifetime of its untyped buffer.
// ALIGNMENT!!!!!
    
// Provides storage, does not manage object lifetimes, nor is it coupled to the underlying object type
template < size_t Size >
struct StackBuffer
{
    // No need to make it private and remove its pod-ness
    char d_buff [ Size ] ;
    
    template < class T >
    ALGO_INLINE
    T* begin ()
    {
        return reinterpret_cast <T*>( &this->d_buff[0] ) ;
    }
    
    template < class T >
    ALGO_INLINE
    T* end ()
    {
        return this->begin < T > () + Size / sizeof ( T ) ;
    }
};

    
    
struct MallocFreeProtocol
{
    static char* allocate ( size_t size )
    {
        void* const returnValue = malloc ( size ) ;
        
        if ( ALGO_LIKELIHOOD ( returnValue, true ) )
        {
            return reinterpret_cast < char* > ( returnValue ) ;
        }
        else
        {
            throw std::bad_alloc ( ) ;
        }
    }
    
    static void deallocate ( char* ptr )
    {
        if ( ALGO_LIKELIHOOD ( ptr, true ) )
        {
            free ( ptr ) ;
        }
    }
};

    
    
struct CallocFreeProtocol : MallocFreeProtocol
{
    static char* allocate ( size_t size )
    {
        void* const returnValue = calloc ( size, 1u ) ;
        
        if ( ALGO_LIKELIHOOD ( returnValue, true ) )
        {
            return reinterpret_cast < char* > ( returnValue ) ;
        }
        else
        {
            throw std::bad_alloc ( ) ;
        }
    }
};

    
    
struct NewDeleteProtocol
{
    static char* allocate ( size_t size )
    {
        // Throwing new []
        return new char [ size ] ;
    }
        
    static void deallocate ( char* ptr )
    {
        if ( ALGO_LIKELIHOOD ( ptr, true ) )
        {
            delete [] ( ptr ) ;
        }
    }
};

    
    
struct ZeroedNewDeleteProtocol : NewDeleteProtocol
{
    static char* allocate ( size_t size )
    {
        char* const buff = NewDeleteProtocol::allocate ( size ) ;
        fill ( buff, buff + size, 0 ) ;
        return buff ;
    }
};
    
template < typename AllocatorProtocol >
class MallocBuffer
{
    char* d_buff ;
    size_t d_size ;
public:
    
    ALGO_INLINE
    explicit MallocBuffer ( size_t size )
    : d_buff ( AllocatorProtocol::allocate ( size ) )
    , d_size ( size )
    {
    }
    
    ALGO_INLINE
    ~MallocBuffer ()
    {
        AllocatorProtocol::deallocate ( this->d_buff ) ;
        ALGO_SAFE_MODE( this->d_buff = 0; )
        ALGO_SAFE_MODE( this->d_size = 0u; )
    }
    
    template < class T >
    ALGO_INLINE
    T* begin ()
    {
        return reinterpret_cast <T*>( this->d_buff ) ;
    }
    
    template < class T >
    ALGO_INLINE
    T* end ()
    {
        return this->begin < T > () + this->d_size / sizeof ( T ) ;
    }
};
    
    
    
template < class T, bool destructionLikely = false ALGO_ENABLE_IF_PARAM >
struct ObjectProctor
{
    ALGO_INLINE
    explicit ObjectProctor ( T* obj )
        : d_obj ( obj )
    {}
    
    ALGO_INLINE
    ~ObjectProctor ()
    {
        if ( ALGO_LIKELIHOOD ( this->d_obj, destructionLikely ) )
        {
            destroyObject () ;
        }
    }
    
    void disarm ()
    {
        this->d_obj = 0 ;
    }
private:
    
    ObjectProctor ( ObjectProctor const& x ) ;
    ObjectProctor& operator= ( ObjectProctor const& ) ;
    
    // non-inline, for unusual case of having to do something
    void destroyObject ()
    {
        T* const obj = this->d_obj ;
        ALGO_SAFE_MODE(this->d_obj = 0 ;)
        obj->~T () ;
    }
    
    T* d_obj ;
};
    
    
    
template < class Iter, bool destructionLikely = false ALGO_ENABLE_IF_PARAM >
struct BufferProctor
{
    ALGO_INLINE
    explicit BufferProctor ( Iter i )
        : d_start ( i )
        , d_end   ( i )
    {}
    
    ALGO_INLINE
    ~BufferProctor ()
    {
        if ( ALGO_LIKELIHOOD ( this->d_start != this->d_end, destructionLikely ) )
        {
            destroyExtent () ;
        }
    }
    
    ALGO_INLINE
    void incrementStart ()
    {
        // Can't increment start past end
        ALGO_ASSERT( this->d_end != this->d_start ) ;
        ++this->d_start ;
    }
    
    ALGO_INLINE
    void incrementEnd ()
    {
        ++this->d_end ;
    }
    
private:
    BufferProctor ( BufferProctor const& ) ;
    BufferProctor& operator= ( BufferProctor const& ) ;
    
    // Keep non-inline ... code is unlikely to be executed
    void destroyExtent ()
    {
        while ( this->d_start != this->d_end )
        {
            this->d_start->~T () ; // Throwing on destruction is undefined behaviour ...
            ++this->d_start;
        }
    }
    
    Iter d_start ;
    Iter d_end ;
};

// No need to keep track of things that are trivially destructible
template < class T, bool destructionLikely >
struct BufferProctor < T, destructionLikely,
    typename std::enable_if< IsTriviallyDestructible < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
{
    ALGO_INLINE
    explicit BufferProctor ( T* )
    {}
    
    ALGO_INLINE
    void incrementStart ()
    {}
    
    ALGO_INLINE
    void incrementEnd ()
    {}
};
    
} // namespace algo


#endif
