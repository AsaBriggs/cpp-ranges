#ifndef INCLUDED_ALGO_PROPERTY
#define INCLUDED_ALGO_PROPERTY

#ifndef INCLUDED_TYPE_TRAITS
#include <type_traits>
#define INCLUDED_TYPE_TRAITS
#endif


namespace algo
{
    struct PropertyInPlace {} ;
    
    template < typename PropertyName, typename PropertySet >
    struct HasProperty : std::false_type
    {} ;
    
    template < typename PropertyName, typename PropertySet >
    struct ValueType ;
    
    template < typename PropertyName, typename PropertySet >
    struct ValueReferenceType
    {
        typedef typename ValueType < PropertyName, PropertySet >::type& type ;
    } ;
    
    
    template < typename PropertyName, typename AssociatedType >
    struct ValueType < PropertyName, const AssociatedType >
    {
        typedef typename ValueType < PropertyName, AssociatedType >::type const type ;
    } ;
    
    
    
    
    
    
    
    // Deliberately kept as an aggregate data-type. No constructors/destructors and public member variable.
    template < typename PropertyName, typename AssociatedType >
    struct ValueAndProperty
    {
        AssociatedType x ;
        
        friend
        bool operator== ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return x.x == y.x ;
        }
        
        friend
        bool operator!= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return !( x == y );
        }
        
        friend
        bool operator< ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return x.x < y.x ;
        }
        
        friend
        bool operator<= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return ! ( y < x ) ;
        }
        
        friend
        bool operator> ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return y < x ;
        }
        
        friend
        bool operator>= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return ! ( x < y ) ;
        }
    } ;
    
    
    
    template < typename PropertyName, typename AssociatedType >
    struct HasProperty < PropertyName, ValueAndProperty < PropertyName, AssociatedType > > : std::true_type
    {} ;
    
    template < typename PropertyName, typename AssociatedType >
    struct ValueType < PropertyName, ValueAndProperty < PropertyName, AssociatedType > >
    {
        typedef AssociatedType type ;
    } ;
    
    
    
    
    // Deliberately kept as an aggregate data-type. No constructors/destructors and all public member variables.
    template < typename M0, typename M1 >
    struct Compound
    {
        M0 m0 ;
        M1 m1 ;
        
        friend
        bool operator== ( Compound const& x, Compound const& y )
        {
            return ( x.m0 == y.m0 ) && ( x.m1 == y.m1 ) ;
        }
        
        friend
        bool operator!= ( Compound const& x, Compound const& y )
        {
            return !( x == y );
        }
        
        friend
        bool operator< ( Compound const& x, Compound const& y )
        {
            return x.m0 < y.m0
                ? true
                : !( y.m0 < x.m0 ) && ( x.m1 < y.m1 ) ;
        }
        
        friend
        bool operator<= ( Compound const& x, Compound const& y )
        {
            return ! ( y < x ) ;
        }
        
        friend
        bool operator> ( Compound const& x, Compound const& y )
        {
            return y < x ;
        }
        
        friend
        bool operator>= ( Compound const& x, Compound const& y )
        {
            return ! ( x < y ) ;
        }
    } ;
    
    
    
    template < bool condition, typename IfTrue, typename IfFalse >
    struct eval_if_impl
    {
        typedef typename IfFalse::type type ;
    } ;
    
    template < typename IfTrue, typename IfFalse >
    struct eval_if_impl < true, IfTrue, IfFalse >
    {
        typedef typename IfTrue::type type ;
    } ;
    
    template < typename Condition, typename IfTrue, typename IfFalse >
    struct eval_if
    {
        typedef typename eval_if_impl < Condition::value, IfTrue, IfFalse >::type type ;
    } ;
    
    
    
    template < bool Condition1, typename Condition2 >
    struct or_impl : std::integral_constant < bool, Condition2::value > {} ;
    
    template < typename Condition2 >
    struct or_impl < true, Condition2 > : std::true_type {} ;
    
    template < typename Condition1, typename Condition2 >
    struct or_ : or_impl < Condition1::value, Condition2 > {} ;
    
    
    
    template < typename PropertyName, typename M0, typename M1 >
    struct HasProperty < PropertyName, Compound < M0, M1 > >
        : or_ < HasProperty < PropertyName, M0 >, HasProperty < PropertyName, M1 > >
    {} ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct ValueType < PropertyName, Compound < M0, M1 > >
    {
        static_assert ( HasProperty < PropertyName, Compound < M0, M1 > >::type::value, "Type must have the property in order to obtain it." ) ;
        
        typedef typename eval_if <
            typename HasProperty < PropertyName, M0 >::type
            , ValueType < PropertyName, M0 >
            , ValueType < PropertyName, M1 >
        >::type type ;
    } ;
    
    
    
    
    
    template < typename PropertyName, typename PropertySet >
    struct GetValue ;
    
    template < typename PropertyName, typename AssociatedType >
    struct GetValue < PropertyName, ValueAndProperty < PropertyName, AssociatedType > > ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct GetValue < PropertyName, Compound < M0, M1 > > ;
    
    
    
    template < typename PropertyName, typename AssociatedType >
    struct GetValue < PropertyName, ValueAndProperty < PropertyName, AssociatedType > >
    {
        typedef typename ValueReferenceType < PropertyName, ValueAndProperty < PropertyName, AssociatedType > >::type returnType ;
        
        returnType operator() ( ValueAndProperty < PropertyName, AssociatedType >& x ) const
        {
            return x.x ;
        }
    } ;
    
    
    template < typename PropertyName, typename M0, typename M1, bool InM0 >
    struct GetValue_Compound
    {
        typedef typename ValueReferenceType < PropertyName, Compound < M0, M1 > >::type returnType ;
        
        returnType operator() ( Compound < M0, M1 >& x ) const
        {
            return GetValue < PropertyName, M0 > () ( x.m0 ) ;
        }
    } ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct GetValue_Compound < PropertyName, M0, M1, false >
    {
        typedef typename ValueReferenceType < PropertyName, Compound < M0, M1 > >::type returnType ;
        
        returnType operator() ( Compound < M0, M1 >& x ) const
        {
            return GetValue < PropertyName, M1 > () ( x.m1 ) ;
        }
    } ;
    
    
    template < typename PropertyName, typename M0, typename M1 >
    struct GetValue < PropertyName, Compound < M0, M1 > >
    {
        typedef typename ValueReferenceType < PropertyName, Compound < M0, M1 > >::type returnType ;
        
        returnType operator () ( Compound < M0, M1 >& x ) const
        {
            return GetValue_Compound < PropertyName, M0, M1, HasProperty < PropertyName, M0 >::type::value > () ( x ) ;
        }
    } ;
    
    
    template < typename PropertyName, typename PropertySet >
    typename ValueReferenceType < PropertyName, PropertySet >::type getValue ( PropertySet& x )
    {
        return GetValue < PropertyName, PropertySet > () ( x ) ;
    }
    
    template < typename PropertyName, typename PropertySet >
    typename ValueReferenceType < PropertyName, const PropertySet >::type getValue ( PropertySet const& x )
    {
        return const_cast< typename ValueReferenceType < PropertyName, const PropertySet >::type >( getValue < PropertyName >( const_cast < PropertySet& > ( x ) ) ) ;
    }
    
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    struct AddPropertyType
    {
        static_assert ( !HasProperty < PropertyName, PropertySet >::type::value, "Cant add a property to a structure which already has it" ) ;
        
        // Right recurse, so that the relational operators bail out quickly
        typedef Compound < ValueAndProperty < PropertyName, AssociatedType >, PropertySet > type ;
    };
    
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    typename AddPropertyType < PropertyName, AssociatedType, PropertySet >::type addProperty ( PropertySet const& x, AssociatedType const& y )
    {
        return { y, x } ;
    }
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    void setValue ( PropertySet& x, AssociatedType const& y, PropertyInPlace )
    {
        getValue < PropertyName, PropertySet > ( x ) = y ;
    }
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    PropertySet setValue ( PropertySet const& x, AssociatedType const& y )
    {
        PropertySet returnValue = x ;
        setValue < PropertyName > ( returnValue, y, PropertyInPlace () ) ;
        return returnValue ;
    }
    
} // namespace algo

/*
 // Define an iterator_range to certainly have First
 // as well as (optionally) Last, Count and Predicate
 // How to handle laziness???? that is when the number of items is unknown, right?
 // Or are there 2 forms of laziness, one where count is unknown and the other when the items themselves are unknown
 // Second form is easy as the iterator can do the calculation on calling dereference
 
 struct First; // Must have this!!!
 struct Last;
 struct Count;
 struct Predicate;
 
 template <class T>
 struct IsBounded : boost::mpl::or_<
 typename HasProperty<Last, T>::type
 , typename HasProperty<Count, T>::type
 , typename HasProperty<Predicate, T>::type
 >
 {};
 
 template<class T>
 struct MakeUnboundedType
 {
 typedef typename ValueAndProperty<First, typename ValueType <First, T>::type > type ;
 };
 
 template <class T>
 typename MakeUnboundedType<T>::type makeUnbounded(T const& x)
 {
 //typename MakeUnboundedType<T>::type tmp = { getValue<First>(x) };
 return { getValue<First>(x) }; //tmp;
 }
 
 // Make bounded
 // Make Counted
 
 template <class T, bool hasLast>
 struct AddBoundedType_Impl
 {
 typedef T type ;
 };
 
 template <class T>
 struct AddBoundedType_Impl<T, false>  : AddPropertyType<Last, typename ValueType<First, T>::type, T>
 {};
 
 template <class T>
 struct AddBoundedType : AddBoundedType_Impl<T, HasProperty<Last, T>::type::value >
 {};
 
 template <class T>
 struct GetLengthCounted
 {
 typename std::iterator_traits< typename ValueType<First, T>::type >::difference_type
 operator()(T const& x) const
 {
 return getValue<Count>(x);
 }
 };
 
 template <class T>
 struct GetLengthBounded
 {
 typename std::iterator_traits< typename ValueType<First, T>::type >::difference_type
 operator()(T const& x) const
 {
 return std::distance(getValue<First>(x), getValue<Last>(x));
 }
 };
 
 template <class T>
 struct GetLengthPredicated
 {
 typename std::iterator_traits< typename ValueType<First, T>::type >::difference_type
 operator()(T const& x) const
 {
 typename GetValueType<Predicate, T>::type pred (getValue<Predicate>(x));
 typename GetValueType<First, T>::type     iter (getValue<First>(x));
 typename std::iterator_traits< typename ValueType<First, T>::type >::difference_type count = 0;
 while (pred(*iter))
 {
 ++count;
 ++iter;
 }
 return count;
 }
 };
 
 template <class T>
 struct GetLength
 {
 typedef boost::mpl::if_< typename HasProperty<Count, T>::type
 , GetLengthCounted<T>
 , typename boost::mpl::if_<
 typename HasProperty<Last, T>::type
 , GetLengthBounded<T>
 , GetLengthPredicated<T>
 >::type
 > dispatch ;
 
 typename std::iterator_traits< typename ValueType<First, T>::type >::difference_type
 operator()(T const& x) const
 {
 return typename dispatch::type()(x);
 }
 };
 
 
 template <class T>
 struct IsEmptyCounted
 {
 bool
 operator()(T const& x) const
 {
 return !getValue<Count>(x);
 }
 };
 
 template <class T>
 struct IsEmptyBounded
 {
 bool
 operator()(T const& x) const
 {
 return getValue<First>(x) == getValue<Last>(x);
 }
 };
 
 template <class T>
 struct IsEmptyPredicated
 {
 bool
 operator()(T const& x) const
 {
 typename GetValueType<Predicate, T>::type pred (getValue<Predicate>(x));
 return !pred(*getValue<First>(x));
 }
 };
 
 template <class T>
 struct IsEmpty
 {
 typedef boost::mpl::if_< typename HasProperty<Count, T>::type
 , IsEmptyCounted<T>
 , typename boost::mpl::if_<
 typename HasProperty<Last, T>::type
 , IsEmptyBounded<T>
 , IsEmptyPredicated<T>
 >::type
 > dispatch ;
 
 bool
 operator()(T const& x) const
 {
 return typename dispatch::type()(x);
 }
 };
 template<class T>
 bool isEmpty(T const& x)
 {
 return IsEmpty<T>()(x);
 }
 
 
 template <class T>
 struct Next
 {
 void
 operator() (T& x) const
 {
 ++( getValue<First>(x) );
 // Decrement Count?
 }
 };
 
 template <class T>
 void next(T& x)
 {
 Next<T>()(x);
 }
 
 #endif

 
 */
#endif
