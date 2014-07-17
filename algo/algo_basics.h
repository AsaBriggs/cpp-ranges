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

#ifndef ALGO_STATIC_ASSERT
#define ALGO_STATIC_ASSERT( test, reason ) static_assert ( test, reason )
#endif

#define ALGO_CALL ::algo
#define ALGO_LOGIC_CALL ALGO_CALL::logic
#define ALGO_ENABLE_IF_PARAM_DEFAULT ALGO_CALL::EnableIfDeductionType
#define ALGO_COMMA_ENABLE_IF_PARAM , typename enable = ALGO_ENABLE_IF_PARAM_DEFAULT
#define ALGO_NULLPTR nullptr
#define ALGO_CONSTEXPR_VARIABLE constexpr
#define ALGO_CONSTEXPR_FUNCTION constexpr

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
    
    
    template < typename T >
    ALGO_INLINE
    ALGO_CONSTEXPR_FUNCTION
    T&&
    forward ( typename std::remove_reference < T >::type& x )
    ALGO_NOEXCEPT_DECL ( true )
    {
        return static_cast < T&& > ( x ) ;
    }
    
    template < typename T >
    ALGO_INLINE
    ALGO_CONSTEXPR_FUNCTION
    T&&
    forward ( typename std::remove_reference < T >::type&& x )
    ALGO_NOEXCEPT_DECL ( true )
    {
        static_assert( !std::is_lvalue_reference < T >::type::value,
                      "Can not forward an rvalue as an lvalue." ) ;
        return static_cast < T&& > ( x ) ;
    }
    
    template < typename T >
    ALGO_INLINE
    ALGO_CONSTEXPR_FUNCTION
    typename std::remove_reference < T >::type&&
    move ( T&& x )
    ALGO_NOEXCEPT_DECL ( true )
    {
        typedef typename std::remove_reference < T >::type U ;
        return static_cast < U&& >( x ) ;
    }
    
    
    template < class T >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Unpredictable )
    ALGO_NOEXCEPT_DECL ( noexcept ( std::is_nothrow_move_constructible < T >::type::value
                                   && std::is_nothrow_move_assignable < T >::type::value
                                   && std::is_nothrow_destructible < T >::type::value ) )
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
        T arr [] = { ALGO_CALL::move ( x ), ALGO_CALL::move ( y ) } ;
        // Use parallel assignment to remove instruction ordering requirement
        x = ALGO_CALL::move ( arr [ swapNeeded ] ), y = ALGO_CALL::move ( arr [ !swapNeeded ] ) ;
    }
        
    template < class T >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Ternary )
        ALGO_NOEXCEPT_DECL ( noexcept ( swapNeeded ? std::iter_swap ( &x, &y ) : ( void ) 0 ) )
    {
        swapNeeded ? std::iter_swap ( &x, &y ) : ( void ) 0 ;
    }

    // need this here to enable noexcept to be expressed.
    using std::swap ;
    
    template < class T >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Consistent )
        ALGO_NOEXCEPT_DECL ( noexcept ( swap ( x, y ) ) )
    {
        // Leave branch prediction algorithm to pick up on the consistent direction.
        if ( swapNeeded ) swap ( x, y ) ;
    }
    
    template < class T, class Prediction >
    ALGO_INLINE
    void swap_if ( bool swapNeeded, T& x, T& y, Prediction )
        ALGO_NOEXCEPT_DECL ( noexcept ( swap ( x, y ) ) )
    {
        // Issue a hint to the compiler about the predicted direction.
        if ( ALGO_LIKELIHOOD ( swapNeeded, Prediction::value ) ) swap ( x, y ) ;
    }
        
    // Allows non-inlining of conditionals.
    template < class T, class Prediction >
    ALGO_NOINLINE
    void swap_if ( bool swapNeeded, T& x, T& y, NotInline < Prediction > )
        ALGO_NOEXCEPT_DECL ( noexcept ( swap_if ( swapNeeded, x, y, Prediction () ) ) )
    {
        // Hopefully the swap_if gets inlined into this function, but the "__attribute__(( noinline ))" prevents it from being inlined into the caller.
        ALGO_CALL::swap_if ( swapNeeded, x, y, Prediction () ) ;
    }
    
    
        
    template < class T >
    ALGO_INLINE
    T select_if ( bool x, T ifFalse, T ifTrue, Unpredictable )
        ALGO_NOEXCEPT_DECL ( noexcept ( std::is_nothrow_move_constructible < T >::type::value
                                       && std::is_nothrow_move_assignable < T >::type::value
                                       && std::is_nothrow_destructible < T >::type::value ) )
    {
        T arr [] = { ALGO_CALL::move ( ifFalse ), ALGO_CALL::move ( ifTrue ) } ;
        return ALGO_CALL::move ( arr [ x ] ) ;
    }
     
    template < class T >
    ALGO_INLINE
    T select_if ( bool x, T ifFalse, T ifTrue, Ternary )
        ALGO_NOEXCEPT_DECL ( noexcept ( x ? ifTrue : ifFalse ) )
    {
        return x ? ifTrue : ifFalse ;
    }
     
    template < class T >
    ALGO_INLINE
    T select_if ( bool x, T ifFalse, T ifTrue, Consistent )
    {
        if ( x ) return ifTrue ; else return ifFalse ;
    }
     
    template < class T, class Prediction >
    ALGO_INLINE
    T select_if ( bool x, T ifFalse, T ifTrue, Prediction )
    {
        if ( ALGO_LIKELIHOOD ( x, Prediction::value ) ) return ifTrue ; else return ifFalse ;
    }
        
    template < class T, class Prediction >
    ALGO_NOINLINE
    T select_if ( bool x, T ifFalse, T ifTrue, NotInline < Prediction > )
        ALGO_NOEXCEPT_DECL ( noexcept ( select_if ( x, ifFalse, ifTrue, Prediction () ) ) )
    {
        // Hopefully the select_if gets inlined into this function, but the "__attribute__(( noinline ))" prevents it from being inlined into the caller.
        return ALGO_CALL::select_if ( x, ALGO_CALL::move ( ifFalse ), ALGO_CALL::move ( ifTrue ), Prediction () ) ;
    }
    
    namespace logic
    {
        template < typename Type, Type val >
        struct integral_constant
        {
            typedef integral_constant type ;
            typedef Type value_type ;

            static ALGO_CONSTEXPR_VARIABLE value_type value = val ;
            ALGO_INLINE ALGO_CONSTEXPR_FUNCTION operator value_type () const { return val ; }
            ALGO_INLINE ALGO_CONSTEXPR_FUNCTION value_type operator () () const { return val ; }
        } ;
        
        typedef integral_constant < bool, true > true_type ;
        typedef integral_constant < bool, false > false_type ;
        
        namespace detail {
        
        template < bool value >
        struct not_impl ;
            
        template <>
        struct not_impl < false >
            : ALGO_LOGIC_CALL::true_type
        {} ;
            
        template <>
        struct not_impl < true >
            : ALGO_LOGIC_CALL::false_type
        {} ;
            
        template < bool P0, typename P1, typename P2, typename P3, typename P4 >
        struct or_impl ;
        
        template < typename P1, typename P2, typename P3, typename P4 >
        struct or_impl < true, P1, P2, P3, P4 >
            : ALGO_LOGIC_CALL::true_type
        {} ;
        
        template <>
        struct or_impl < false, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type, ALGO_LOGIC_CALL::false_type >
            : ALGO_LOGIC_CALL::false_type
        {} ;
        
        template < typename P1, typename P2, typename P3, typename P4 >
        struct or_impl < false, P1, P2, P3, P4 >
            : ALGO_LOGIC_CALL::detail::or_impl < P1::type::value, P2, P3, P4, ALGO_LOGIC_CALL::false_type >
        {} ;
        
        
        template < bool value, typename P1, typename P2, typename P3, typename P4 >
        struct and_impl ;
        
        template < typename P1, typename P2, typename P3, typename P4 >
        struct and_impl < false, P1, P2, P3, P4 >
            : ALGO_LOGIC_CALL::false_type
        {} ;
        
        template <>
        struct and_impl < true, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type, ALGO_LOGIC_CALL::true_type >
            : ALGO_LOGIC_CALL::true_type
        {} ;
        
        template < typename P1, typename P2, typename P3, typename P4 >
        struct and_impl < true, P1, P2, P3, P4 >
            : ALGO_LOGIC_CALL::detail::and_impl < P1::type::value, P2, P3, P4, ALGO_LOGIC_CALL::true_type >
        {} ;
        
        
        template < bool condition, typename IfTrue, typename IfFalse >
        struct eval_if_impl ;
        
        template < typename IfTrue, typename IfFalse >
        struct eval_if_impl < false , IfTrue, IfFalse >
        {
            typedef typename IfFalse::type type ;
        } ;
        
        template < typename IfTrue, typename IfFalse >
        struct eval_if_impl < true, IfTrue, IfFalse >
        {
            typedef typename IfTrue::type type ;
        } ;
        
        
        template < bool condition, typename IfTrue, typename IfFalse >
        struct if_impl ;
        
        template < typename IfTrue, typename IfFalse >
        struct if_impl < false, IfTrue, IfFalse >
        {
            typedef IfFalse type ;
        } ;
        
        template < typename IfTrue, typename IfFalse >
        struct if_impl < true, IfTrue, IfFalse >
        {
            typedef IfTrue type ;
        } ;
        
        template < bool enable, typename T >
        struct enable_if_impl ;
        
        template < typename T >
        struct enable_if_impl < false, T >
        {} ;
            
        template < typename T >
        struct enable_if_impl < true, T >
        {
            typedef T type ;
        } ;
            
        template < bool disable, typename T >
        struct disable_if_impl ;
            
        template < typename T >
        struct disable_if_impl < false, T >
        {
            typedef T type ;
        } ;
            
        template < typename T >
        struct disable_if_impl < true, T >
        {} ;

        } // namespace detail
        
        template < typename T >
        struct not_
            : ALGO_LOGIC_CALL::detail::not_impl < T::type::value >
        {} ;
        
        template < typename P0 = ALGO_LOGIC_CALL::false_type, typename P1 = ALGO_LOGIC_CALL::false_type, typename P2 = ALGO_LOGIC_CALL::false_type, typename P3 = ALGO_LOGIC_CALL::false_type, typename P4 = ALGO_LOGIC_CALL::false_type >
        struct or_
            : ALGO_LOGIC_CALL::detail::or_impl < P0::type::value, P1, P2, P3, P4 >
        {} ;
        
        template < typename P0 = ALGO_LOGIC_CALL::true_type, typename P1 = ALGO_LOGIC_CALL::true_type, typename P2 = ALGO_LOGIC_CALL::true_type, typename P3 = ALGO_LOGIC_CALL::true_type, typename P4 = ALGO_LOGIC_CALL::true_type >
        struct and_
            : ALGO_LOGIC_CALL::detail::and_impl < P0::type::value, P1, P2, P3, P4 >
        {};
        
        template < typename Condition, typename IfTrue, typename IfFalse >
        struct eval_if
            : ALGO_LOGIC_CALL::detail::eval_if_impl < Condition::type::value, IfTrue, IfFalse >
        {} ;
        
        template < typename Condition, typename IfTrue, typename IfFalse >
        struct if_
            : ALGO_LOGIC_CALL::detail::if_impl < Condition::type::value, IfTrue, IfFalse >
        {} ;
        
        template < bool enable, typename T = void >
        struct enable_if
            : ALGO_LOGIC_CALL::detail::enable_if_impl < enable, T >
        {} ;
        
        template < typename Predicate, typename T = void >
        struct enable_if_pred
            : ALGO_LOGIC_CALL::detail::enable_if_impl < Predicate::type::value, T >
        {} ;
        
        template < bool disable, typename T = void >
        struct disable_if
            : ALGO_LOGIC_CALL::detail::disable_if_impl < disable, T >
        {} ;
        
        template < typename Predicate, typename T = void >
        struct disable_if_pred
            : ALGO_LOGIC_CALL::detail::disable_if_impl < Predicate::type::value, T >
        {} ;
        
    } // namespace logic

} // namespace algo

#endif
