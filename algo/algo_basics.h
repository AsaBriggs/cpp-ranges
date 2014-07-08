#ifndef INCLUDED_ALGO_BASICS
#define INCLUDED_ALGO_BASICS

#ifndef INCLUDED_CASSERT
#include <cassert>
#define INCLUDED_CASSERT
#endif

#ifndef INCLUDED_TYPE_TRAITS
#include <type_traits>
#define INCLUDED_TYPE_TRAITS
#endif


#ifndef ALGO_INLINE
#define ALGO_INLINE __attribute__((always_inline)) inline
#endif

#ifndef ALGO_NOINLINE
#define ALGO_NOINLINE __attribute__((noinline))
#endif

#ifndef ALGO_ASSERT
#define ALGO_ASSERT( x ) assert ( ( x ) )
#endif

#ifndef ALGO_SAFE_MODE
#define ALGO_SAFE_MODE( x ) x
#endif

#ifndef ALGO_LIKELIHOOD

#ifdef _MSC_VER
#define ALGO_LIKELIHOOD( test, likelihood )
#else
#define ALGO_LIKELIHOOD( test, likelihood ) __builtin_expect ( !!( test ), ( likelihood ) )
#endif

#endif

#ifndef ALGO_NOEXCEPT_DECL
#define ALGO_NOEXCEPT_DECL( x ) noexcept ( x )
#endif

#define ALGO_CALL ::algo
#define ALGO_ENABLE_IF_PARAM_DEFAULT ALGO_CALL::EnableIfDeductionType
#define ALGO_COMMA_ENABLE_IF_PARAM , typename enable = ALGO_ENABLE_IF_PARAM_DEFAULT
#define ALGO_NULLPTR nullptr

namespace algo
{
    struct EnableIfDeductionType {} ;
    struct InPlace {} ;
    struct ByReturnValue {} ;
    
    
    
    
    // Uses an array to swap values if needed.
    struct Unpredictable {};
    
    // Uses the ternary operator to swap values if needed.
    struct Ternary {} ;
    
    // Not predictable what value this takes, but at run-time it will tend to take value true or false
    struct Consistent {};
    
    
    struct PredictableFalse { enum { value = 0 } ; };
    struct PredictableTrue { enum { value = 1 } ; };
    
    // Have tagged predictions to uniquify the location of each conditional swap
    template < class Tag >
    struct PredictableFalseWithTag : PredictableFalse {};
    template < class Tag >
    struct PredictableTrueWithTag : PredictableTrue {};
    
    template < class T >
    struct NotInline {};
    
    ALGO_INLINE
    void doNothing () ALGO_NOEXCEPT_DECL ( true )
    {}
    
    template < class T >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Unpredictable )
    ALGO_NOEXCEPT_DECL ( noexcept ( std::is_nothrow_move_constructible < T >::value
                                   && std::is_nothrow_move_assignable < T >::value
                                   && std::is_nothrow_destructible < T >::value ) )
    {
        //typedef unsigned int IndexType ;
        //const IndexType tmp = ( std::numeric_limits < IndexType >::max () + IndexType ( !needed ) ) & ( xi - yi ) ;
        //xi -= tmp ;
        //yi += tmp ;
        
        //
        // OK, so constructing an array and moving between arrays is not cheap
        // but in a tight loop an unpredictable conditional is very expensive, so this may end up being quicker.
        // Also writes are less expensive than one might think as the write buffer comes into play.
        //
        T arr [] = { std::move ( x ), std::move ( y ) } ;
        // Use parallel assignment to remove instruction ordering requirement
        x = std::move ( arr [ swapNeeded ] ), y = std::move ( arr [ !swapNeeded ] ) ;
    }
        
    template < class T >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Ternary )
    ALGO_NOEXCEPT_DECL ( noexcept ( swapNeeded ? std::iter_swap ( &x, &y ) : doNothing () ) )
    {
        swapNeeded ? std::iter_swap ( &x, &y ) : doNothing () ;
    }

    // need this here to enable noexcept to be expressed.
    using std::swap ;
    
    template < class T, class Prediction >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Consistent ) ALGO_NOEXCEPT_DECL ( noexcept ( swap ( x, y ) ) )
    {
        // Leave branch prediction algorithm to pick up on the consistent direction.
        if ( swapNeeded )
        {
            swap ( x, y ) ;
        }
    }
    
    template < class T, class Prediction >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Prediction ) ALGO_NOEXCEPT_DECL ( noexcept ( swap ( x, y ) ) )
    {
        // Issue a hint to the compiler about the predicted direction.
        if ( ALGO_LIKELIHOOD ( swapNeeded, Prediction::value ) )
        {
            swap ( x, y ) ;
        }
    }
    
    
    // Allows non-inlining of conditionals.
    template < class T, class Prediction >
    ALGO_NOINLINE
    void swap_if ( bool swapNeeded, T& x, T& y, NotInline < Prediction > ) ALGO_NOEXCEPT_DECL ( noexcept ( swap_if ( swapNeeded, x, y, Prediction () ) ) )
    {
        // Hopefully the swap_if gets inlined into this function, but the "__attribute__(( noinline ))" prevents it from being inlined into the caller.
        swap_if ( swapNeeded, x, y, Prediction () ) ;
    }
    
} // namespace algo

#endif
