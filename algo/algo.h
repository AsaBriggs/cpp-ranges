#ifndef INCLUDED_ALGO_H
#define INCLUDED_ALGO_H

#include <type_traits>
#include <iterator>
#include <utility>
#include <memory>

#define ALGO_INLINE __attribute__((always_inline)) inline
#define ALGO_ASSERT(x)
#define ALGO_ENABLE_IF_PARAM , typename enable = void*


namespace algo
{

struct InPlace{};
struct ByReturnValue{};
    
    
template < typename T ALGO_ENABLE_IF_PARAM >
struct IsBitwiseCopyable : std::is_trivially_copy_assignable<T>
{};
    
template < typename T, int N >
struct IsBitwiseCopyable < T [ N ], void* > : IsBitwiseCopyable < T >
{};
    
template < typename T, typename U >
struct IsBitwiseCopyable < std::pair < T, U >, void* >
    : std::integral_constant<bool, IsBitwiseCopyable < T >::value && IsBitwiseCopyable < U >::value >
{};
    
template < class T ALGO_ENABLE_IF_PARAM >
struct IsBitwiseMoveable : IsBitwiseCopyable<T>
{};
    
    // shared pointer, managed pointer ... things containing these.
    
    
template < typename BidirectionalIterator ALGO_ENABLE_IF_PARAM >
struct Predecessor
{
    ALGO_INLINE
    BidirectionalIterator operator()(BidirectionalIterator x, ByReturnValue) const
    {
        return --x;
    }
    
    ALGO_INLINE
    void operator()(BidirectionalIterator& x, InPlace) const
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
    ForwardIterator operator()(ForwardIterator x, ByReturnValue) const
    {
        return ++x ;
    }
    
    ALGO_INLINE
    void operator()(ForwardIterator& x, InPlace) const
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
    typename std::iterator_traits<Iter>::reference operator()(Iter x) const
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
    X* operator()(X& x) const
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
struct DestroyPointed
{
    // enable if not proxy
    ALGO_INLINE
    void operator()( Iter x ) const
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



template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct CopyConstruct
{
    ALGO_INLINE
    void operator()(I i, O o) const
    {
        typedef typename std::iterator_traits<O>::type T ;
        // enable if not a proxy o
        new (addressOf(deref(o))) T (deref(i));
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
    ALGO_INLINE
    void operator()(I i, O o) const
    {
        typedef typename std::iterator_traits<O>::type T ;
        // enable if not a proxy o
        new (addressOf(deref(o))) T (std::move(deref(i)));
    }
};

template < typename I, typename O >
ALGO_INLINE
void moveConstruct (I i, O o)
{
    MoveConstruct < I, O > () ( i , o ) ;
}



template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct Assign
{
    ALGO_INLINE
    void operator()(I i, O o) const
    {
        deref(o) = deref(i);
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
    void operator()(I i, O o) const
    {
        deref(o) = std::move(deref(i));
    }
};

template < typename I, typename O >
ALGO_INLINE
void moveAssign (I i, O o)
{
    MoveAssign < I, O > () ( i , o ) ;
}



template <typename I, typename O ALGO_ENABLE_IF_PARAM >
struct Swap
{
    ALGO_INLINE
    void operator()(I i, O o) const
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

struct pre_op_i_tag{};
struct pre_op_o_tag{};
struct Operation_tag{};
struct post_op_i_tag{};
struct post_op_o_tag{};

template <typename O, typename I, typename Op ALGO_ENABLE_IF_PARAM >
struct Step
{
    ALGO_INLINE
    void operator () ( I& i, O& o, Op op ) const
    {
        op(pre_op_i_tag(), i),
        op(pre_op_o_tag(), o);
        
        op(Operation_tag(), i, o);
        
        op(post_op_i_tag(), i),
        op(post_op_o_tag(), o);
    }
};

template <typename O, typename I, typename Op>
ALGO_INLINE
void step(I& i, O& o, Op op)
{
    Step < I, O, Op > () ( i, o, op ) ;
}



template < template < typename I, typename O ALGO_ENABLE_IF_PARAM > class Op ALGO_ENABLE_IF_PARAM >
struct Operator
{
    template <typename I, typename O>
    ALGO_INLINE
    void operator () ( Operation_tag, I i, O o ) const
    {
        Op < I, O >() ( i, o ) ;
    }
};



struct DefaultOperations
{
    template < typename I, typename Op >
    ALGO_INLINE
    void operator () ( Op, I ) const
    {}
    
    // Do not provide an Operation_tag overload
};


// binds successor to the given Op
template <typename Op>
struct Forwards
{
    template <typename Iter>
    ALGO_INLINE
    void operator()(Op, Iter& x) const
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
    void operator()(Op, Iter& x) const
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
template < typename T, typename U >
ALGO_INLINE
typename std::enable_if < std::is_same<typename std::remove_cv<T>::type, U>::value &&
                        IsBitwiseCopyable <U>::value, U* >::type
copyImpl ( T* f, T* l, U* o )
{
    size_t diff = l - f ;
    ALGO_ASSERT ( diff >= 0 ) ;
    if ( o != f )
    {
        std::memmove ( o, f, sizeof ( T ) * diff ) ;
    }
    return o + diff ;
}
    
template < typename I, typename O>
O copy ( I f, I l, O o )
{
    if ( f == l ) return o ;
    
    return O ( copyImpl ( stripIter ( f )
                         , stripIter ( l )
                         , stripIter ( o ) ) ) ;
}

    
struct CopyBackward : Operator < Assign >, IBackwardsOBackwards {} ;

template < typename T, typename U >
ALGO_INLINE
typename std::enable_if < std::is_same<typename std::remove_cv<T>::type, U>::value &&
                        IsBitwiseCopyable <U>::value, U* >::type
copyBackwardImpl ( T* f, T* l, U* o )
{
    size_t diff = l - f ;
    ALGO_ASSERT ( diff >= 0 ) ;
    if ( l != o )
    {
        std::memmove ( o - diff, f, sizeof ( T ) * diff ) ;
    }
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




/*









template <typename I, typename P>
I find_if(I f, I l, P p)
{
    while (f!=l && !p(*f)) successor(f, InPlace());
    return f;
}

template <typename I, typename P>
I find_if_not(I f, I l, P p)
{
    while (f!=l && p(*f)) successor(f, InPlace());
    return f;
}

template <typename I, typename P>
I find_reverse_if(I f, I l, P p)
{
    while (f!=l && !p(*predecessor(l))) predecessor(l, InPlace());
    return l;
}

template <typename I, typename P>
I find_reverse_if_not(I f, I l, P p)
{
    while (f!=l && p(*predecessor(l))) predecessor(l, InPlace());
    return l;
}


template <typename I, typename P>
I find_reverse_if2(I f, I l, P p)
{
    if(f==l) return l;
    I pred = predecessor(l);
    while (f!=l && !p(*pred)) {
        l = pred;
        predecessor(pred, InPlace());
    }
    return l;
}

template <typename I, typename P>
I find_reverse_if_not2(I f, I l, P p)
{
    if(f==l) return l;
    I pred = predecessor(l);
    while (f!=l && p(*pred)) {
        l = pred;
        predecessor(pred, InPlace());
    }
    return l;
}

template <typename I, typename P>
I partition_bidirectional(I f, I l, P p)
{
    while (true)
    {
        f = find_if(f, l, p);
        l = find_reverse_if_not(f, l, p);
        if (f==l) return f;
        std::iter_swap(f, l);
        ++f, --l;
    }
}
 */
    
} // namespace algo


#endif
