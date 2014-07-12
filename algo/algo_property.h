#ifndef INCLUDED_ALGO_PROPERTY
#define INCLUDED_ALGO_PROPERTY

#ifndef INCLUDED_TYPE_TRAITS
#include <type_traits>
#define INCLUDED_TYPE_TRAITS
#endif

#ifndef INCLUDED_ALGO_BASICS
#include "algo_basics.h"
#endif

#define ALGO_IMPL_CALL ALGO_CALL::property_impl

namespace algo
{
    namespace property_impl
    {
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
            typedef typename ALGO_IMPL_CALL::eval_if_impl < Condition::type::value, IfTrue, IfFalse >::type type ;
        } ;
        
        
        
        template < bool Condition1, typename Condition2, typename Condition3 >
        struct or_impl ;
        
        template < typename Condition2, typename Condition3 >
        struct or_impl < true, Condition2, Condition3 >
            : std::true_type
        {} ;
        
        template <>
        struct or_impl < false, std::false_type, std::false_type >
            : std::false_type
        {} ;
        
        template < typename Condition2, typename Condition3 >
        struct or_impl < false, Condition2, Condition3 >
            : ALGO_IMPL_CALL::or_impl < Condition2::type::value, Condition3, std::false_type >
        {} ;
        
        
        template < typename Condition1, typename Condition2 = std::false_type, typename Condition3 = std::false_type >
        struct or_
            : ALGO_IMPL_CALL::or_impl < Condition1::type::value, Condition2, Condition3 >
        {} ;
        
        template < typename T >
        struct RemoveCVAndReference : std::remove_cv < typename std::remove_reference < T >::type >
        {} ;
        
    } // namespace property_impl
    
    
    
    template < typename PropertyName, typename PropertySet >
    struct HasProperty
        : std::false_type
    {} ;
    
    
    
    template < typename PropertyName, typename PropertySet >
    struct ValueType ;
    
    template < typename PropertyName, typename AssociatedType >
    struct ValueType < PropertyName, AssociatedType const >
    {
        typedef typename ALGO_CALL::ValueType < PropertyName, AssociatedType >::type const type ;
    } ;
    
    
    
    struct ByReference {} ;
    struct ByValue {} ;
    
    template < typename PropertyName, typename PassByType, typename PropertySet >
    struct ValueReturnType
    {
        typedef typename std::add_lvalue_reference < typename ALGO_CALL::ValueType < PropertyName, PropertySet >::type >::type type ;
    } ;
    
    template < typename PropertyName, typename PropertySet  >
    struct ValueReturnType < PropertyName, ByValue, PropertySet >
    {
        typedef typename ALGO_CALL::ValueType < PropertyName, PropertySet >::type type ;
    } ;
    
    
    
    // Deliberately kept as an aggregate data-type. No constructors/destructors and public member variable.
    template < typename PropertyName, typename AssociatedType >
    struct ValueAndProperty
    {
        typedef ValueAndProperty type ;
        
        AssociatedType x ;
        
        friend
        ALGO_INLINE
        bool operator== ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return x.x == y.x ;
        }
        
        friend
        ALGO_INLINE
        bool operator!= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return !( x == y );
        }
        
        friend
        ALGO_INLINE
        bool operator< ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return x.x < y.x ;
        }
        
        friend
        ALGO_INLINE
        bool operator<= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return ! ( y < x ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator> ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return y < x ;
        }
        
        friend
        ALGO_INLINE
        bool operator>= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return ! ( x < y ) ;
        }
    } ;
    
    
    
    template < typename PropertyName, typename AssociatedType >
    struct HasProperty < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > >
        : std::true_type
    {} ;
    
    template < typename PropertyName, typename AssociatedType >
    struct ValueType < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > >
    {
        typedef AssociatedType type ;
    } ;
    
    
    
    
    // Deliberately kept as an aggregate data-type. No constructors/destructors and all public member variables.
    template < typename M0, typename M1 >
    struct Compound
    {
        typedef Compound type ;
        
        M0 m0 ;
        M1 m1 ;
        
        friend
        ALGO_INLINE
        bool operator== ( Compound const& x, Compound const& y )
        {
            return ( x.m0 == y.m0 ) && ( x.m1 == y.m1 ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator!= ( Compound const& x, Compound const& y )
        {
            return !( x == y );
        }
        
        friend
        ALGO_INLINE
        bool operator< ( Compound const& x, Compound const& y )
        {
            return x.m0 < y.m0
                ? true
                : !( y.m0 < x.m0 ) && ( x.m1 < y.m1 ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator<= ( Compound const& x, Compound const& y )
        {
            return ! ( y < x ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator> ( Compound const& x, Compound const& y )
        {
            return y < x ;
        }
        
        friend
        ALGO_INLINE
        bool operator>= ( Compound const& x, Compound const& y )
        {
            return ! ( x < y ) ;
        }
    } ;
    
    
    
    template < typename PropertyName, typename M0, typename M1 >
    struct HasProperty < PropertyName, ALGO_CALL::Compound < M0, M1 > >
        : ALGO_IMPL_CALL::or_ < ALGO_CALL::HasProperty < PropertyName, M0 >, ALGO_CALL::HasProperty < PropertyName, M1 > >
    {} ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct ValueType < PropertyName, ALGO_CALL::Compound < M0, M1 > >
    {
        static_assert ( ALGO_CALL::HasProperty < PropertyName, ALGO_CALL::Compound < M0, M1 > >::type::value, "Type must have the property in order to obtain it." ) ;
        
        typedef typename ALGO_IMPL_CALL::eval_if <
            typename ALGO_CALL::HasProperty < PropertyName, M0 >::type
            , ALGO_CALL::ValueType < PropertyName, M0 >
            , ALGO_CALL::ValueType < PropertyName, M1 >
        >::type type ;
    } ;
    
    
    
    
    
    template < typename PropertyName, typename PassByType, typename PropertySet >
    struct GetValue ;
    
    template < typename PropertyName, typename PassByType, typename AssociatedType >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > > ;
    
    template < typename PropertyName, typename PassByType, typename M0, typename M1 >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 > > ;
    
    
    
    template < typename PropertyName, typename PassByType, typename AssociatedType >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > >
    {
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >& x )
        {
            return x.x ;
        }
    } ;
    
    
    template < typename PropertyName, typename PassByType, typename M0, typename M1, bool InM0 >
    struct GetValue_Compound
    {
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 > >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( ALGO_CALL::Compound < M0, M1 >& x )
        {
            return ALGO_CALL::GetValue < PropertyName, PassByType, M0 >::apply ( x.m0 ) ;
        }
    } ;
    
    template < typename PropertyName, typename PassByType, typename M0, typename M1 >
    struct GetValue_Compound < PropertyName, PassByType, M0, M1, false >
    {
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 > >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( ALGO_CALL::Compound < M0, M1 >& x )
        {
            return ALGO_CALL::GetValue < PropertyName, PassByType, M1 >::apply ( x.m1 ) ;
        }
    } ;
    
    
    template < typename PropertyName, typename PassByType, typename M0, typename M1 >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 > >
    {
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 > >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( ALGO_CALL::Compound < M0, M1 >& x )
        {
            return ALGO_CALL::GetValue_Compound < PropertyName, PassByType, M0, M1, ALGO_CALL::HasProperty < PropertyName, M0 >::type::value >::apply ( x ) ;
        }
    } ;
    
    
    template < typename PropertyName, typename PropertySet >
    
    typename ALGO_CALL::ValueReturnType < PropertyName, ALGO_CALL::ByReference, PropertySet >::type getValueByReference ( PropertySet& x )
        ALGO_NOEXCEPT_DECL ( noexcept ( true ) )
    {
        return ALGO_CALL::GetValue < PropertyName, ALGO_CALL::ByReference, PropertySet >::apply ( x ) ;
    }
    
    template < typename PropertyName, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::ValueReturnType < PropertyName, ALGO_CALL::ByReference, PropertySet const >::type getValueByReference ( PropertySet const& x )
        ALGO_NOEXCEPT_DECL ( noexcept ( true ) )
    {
        return ALGO_CALL::getValueByReference < PropertyName, ALGO_CALL::ByReference > ( const_cast < PropertySet& > ( x ) ) ;
    }
    
    template < typename PropertyName, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::ValueReturnType < PropertyName, ALGO_CALL::ByValue, PropertySet >::type getValue ( PropertySet& x )
    {
        return ALGO_CALL::GetValue < PropertyName, ALGO_CALL::ByValue, PropertySet >::apply ( x ) ;
    }
    
    template < typename PropertyName, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::ValueReturnType < PropertyName, ALGO_CALL::ByValue, PropertySet const >::type getValue ( PropertySet const& x )
    {
        return ALGO_CALL::getValue < PropertyName > ( const_cast < PropertySet& > ( x ) ) ;
    }
    
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    struct AddPropertyType
    {
        static_assert ( !ALGO_CALL::HasProperty < PropertyName, PropertySet >::type::value, "Cant add a property to a structure which already has it" ) ;
        
        // Keep the first to be added into the set the first value in the aggregate assignment.
        typedef ALGO_CALL::Compound <
            typename ALGO_IMPL_CALL::RemoveCVAndReference < PropertySet >::type
            , ALGO_CALL::ValueAndProperty <
                PropertyName
                , typename ALGO_IMPL_CALL::RemoveCVAndReference < AssociatedType >::type > > type ;
    };
    
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::AddPropertyType < PropertyName, AssociatedType , PropertySet >::type addProperty ( PropertySet const& x, AssociatedType&& y )
    {
        return { x, std::forward < AssociatedType > ( y ) } ;
    }
    
    
    
    template < typename PropertySet1, typename PropertySet2 >
    struct MergePropertySetsType
    {
        // Requires set of Properties do not overlap.
        typedef ALGO_CALL::Compound < typename ALGO_IMPL_CALL::RemoveCVAndReference < PropertySet1 >::type
                                    , typename ALGO_IMPL_CALL::RemoveCVAndReference < PropertySet2 >::type > type ;
    } ;
     
    template < typename PropertySet1, typename PropertySet2 >
    ALGO_INLINE
    typename MergePropertySetsType < PropertySet1, PropertySet2 >::type mergePropertySets ( PropertySet1&& x, PropertySet2&& y)
    {
        return { std::forward < PropertySet1 > ( x ), std::forward < PropertySet2 > ( y ) } ;
    }
    
    
    
    template < typename PropertyName, typename PropertySet >
    struct SetValue ;
    
    template < typename PropertyName, typename AssociatedType >
    struct SetValue < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > > ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct SetValue < PropertyName, ALGO_CALL::Compound < M0, M1 > > ;
    
    
    
    template < typename PropertyName, typename AssociatedType >
    struct SetValue < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > >
    {
        template < class T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >& x, T&& y )
        {
            x.x = std::forward < T > ( y ) ;
        }
    } ;
    
    
    template < typename PropertyName, typename M0, typename M1, bool InM0 >
    struct SetValue_Compound
    {
        template < typename T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::Compound < M0, M1 >& x, T&& y )
        {
            ALGO_CALL::SetValue < PropertyName, M0 >::apply ( x.m0, std::forward < T > ( y ) ) ;
        }
    } ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct SetValue_Compound < PropertyName, M0, M1, false >
    {
        template < typename T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::Compound < M0, M1 >& x, T&& y )
        {
            ALGO_CALL::SetValue < PropertyName, M1 >::apply ( x.m1, std::forward < T > ( y ) ) ;
        }
    } ;
    
    
    template < typename PropertyName, typename M0, typename M1 >
    struct SetValue < PropertyName, ALGO_CALL::Compound < M0, M1 > >
    {
        template < typename T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::Compound < M0, M1 >& x, T&& y )
        {
            ALGO_CALL::SetValue_Compound < PropertyName, M0, M1, ALGO_CALL::HasProperty < PropertyName, M0 >::type::value >::apply ( x, std::forward < T > ( y ) ) ;
        }
    } ;

    
    
    // Do not implement this function! Can't update x as it is a const-reference.
    template < typename PropertyName, typename Value, typename PropertySet >
    void setValue ( PropertySet const& x, Value&& y, ALGO_CALL::InPlace ) ;
    
    template < typename PropertyName, typename Value, typename PropertySet >
    ALGO_INLINE
    void setValue ( PropertySet& x, Value&& y, ALGO_CALL::InPlace )
    {
        ALGO_CALL::getValueByReference < PropertyName > ( x ) = std::forward < Value > ( y ) ;
        //ALGO_CALL::SetValue < PropertyName, PropertySet >::apply ( x, std::forward < Value > ( y ) ) ;
    }
    
    template < typename PropertyName, typename Value, typename PropertySet >
    ALGO_INLINE
    PropertySet setValue ( PropertySet const& x, Value&& y )
    {
        PropertySet returnValue = x ;
        ALGO_CALL::getValueByReference < PropertyName > ( returnValue ) = std::forward < Value > ( y ) ;
        //ALGO_CALL::setValue < PropertyName > ( returnValue, std::forward < Value > ( y ), ALGO_CALL::InPlace () ) ;
        return returnValue ;
    }
    
    

    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    struct AddOrUpdateValueType
    {
        typedef typename property_impl::eval_if<
            HasProperty < PropertyName, PropertySet >
            , PropertySet
            , AddPropertyType < PropertyName, AssociatedType, PropertySet > >::type type ;
    } ;
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet, bool HasProperty >
    struct AddOrUpdateValue
    {
        template < class T >
        ALGO_INLINE
        static
        typename AddOrUpdateValueType < PropertyName, AssociatedType, PropertySet >::type
        apply ( PropertySet const& x, T&& y )
        {
            return setValue < PropertyName > ( x, std::forward < T > ( y ) ) ;
        }
    } ;
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    struct AddOrUpdateValue < PropertyName, AssociatedType, PropertySet, false >
    {
        template < typename T >
        ALGO_INLINE
        static
        typename AddOrUpdateValueType < PropertyName, AssociatedType, PropertySet >::type
        apply ( PropertySet const& x, T&& y )
        {
            return addProperty < PropertyName > ( x, std::forward < T > ( y ) ) ;
        }
    } ;
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    ALGO_INLINE
    typename AddOrUpdateValueType < PropertyName, AssociatedType, PropertySet >::type addOrUpdateValue ( PropertySet const& x, AssociatedType&& y )
    {
        return AddOrUpdateValue < PropertyName, AssociatedType, PropertySet, HasProperty < PropertyName, PropertySet >::type::value >::apply ( x, std::forward < AssociatedType > ( y ) ) ;
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
