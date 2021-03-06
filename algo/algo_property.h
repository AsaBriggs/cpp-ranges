#ifndef INCLUDED_ALGO_PROPERTY
#define INCLUDED_ALGO_PROPERTY

#ifndef INCLUDED_TYPE_TRAITS
#include <type_traits>
#define INCLUDED_TYPE_TRAITS
#endif

#ifndef INCLUDED_ALGO_BASICS
#include "algo_basics.h"
#endif

namespace algo
{
    namespace detail {
        
        template < typename T >
        struct RemoveCVAndReference : std::remove_cv < typename std::remove_reference < T >::type >
        {} ;
        
    } // namespace detail
    
    
    
    template < typename PropertyName, typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct HasProperty
        : ALGO_LOGIC_CALL::false_type
    {} ;
    
    
    
    template < typename PropertyName, typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct ValueType ;
    
    template < typename PropertyName, typename AssociatedType >
    struct ValueType < PropertyName, AssociatedType const, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef typename ALGO_CALL::ValueType < PropertyName, AssociatedType >::type const type ;
    } ;
    
    
    
    struct ByReference {} ;
    struct ByValue {} ;
    
    template < typename PropertyName, typename PassByType, typename PropertySet >
    struct ValueReturnType ;
    
    template < typename PropertyName, typename PropertySet >
    struct ValueReturnType < PropertyName, ByReference, PropertySet >
    {
        typedef typename std::add_lvalue_reference < typename ALGO_CALL::ValueType < PropertyName, PropertySet >::type >::type type ;
    } ;
    
    template < typename PropertyName, typename PropertySet  >
    struct ValueReturnType < PropertyName, ByValue, PropertySet >
    {
        typedef typename ALGO_CALL::ValueType < PropertyName, PropertySet >::type type ;
    } ;
    
    template < typename PropertyName, typename PropertySet  >
    struct ValueReturnType < PropertyName, ByValue, PropertySet const >
    {
        // Returning a value from a const PropertySet should not make the returned value const.
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
        bool operator == ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return x.x == y.x ;
        }
        
        friend
        ALGO_INLINE
        bool operator != ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return !( x == y );
        }
        
        friend
        ALGO_INLINE
        bool operator < ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return x.x < y.x ;
        }
        
        friend
        ALGO_INLINE
        bool operator <= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return ! ( y < x ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator > ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return y < x ;
        }
        
        friend
        ALGO_INLINE
        bool operator >= ( ValueAndProperty const& x, ValueAndProperty const& y )
        {
            return ! ( x < y ) ;
        }
    } ;
    
    
    
    template < typename PropertyName, typename AssociatedType >
    struct HasProperty < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, ALGO_ENABLE_IF_PARAM_DEFAULT >
        : ALGO_LOGIC_CALL::true_type
    {} ;
    
    template < typename PropertyName, typename AssociatedType >
    struct ValueType < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, ALGO_ENABLE_IF_PARAM_DEFAULT >
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
        bool operator == ( Compound const& x, Compound const& y )
        {
            return ( x.m0 == y.m0 ) && ( x.m1 == y.m1 ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator != ( Compound const& x, Compound const& y )
        {
            return !( x == y );
        }
        
        friend
        ALGO_INLINE
        bool operator < ( Compound const& x, Compound const& y )
        {
            return x.m0 < y.m0
                ? true
                : !( y.m0 < x.m0 ) && ( x.m1 < y.m1 ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator <= ( Compound const& x, Compound const& y )
        {
            return ! ( y < x ) ;
        }
        
        friend
        ALGO_INLINE
        bool operator > ( Compound const& x, Compound const& y )
        {
            return y < x ;
        }
        
        friend
        ALGO_INLINE
        bool operator >= ( Compound const& x, Compound const& y )
        {
            return ! ( x < y ) ;
        }
    } ;
    
    
    
    template < typename PropertyName, typename M0, typename M1 >
    struct HasProperty < PropertyName, ALGO_CALL::Compound < M0, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT >
        : ALGO_LOGIC_CALL::or_ < ALGO_CALL::HasProperty < PropertyName, M0 >, ALGO_CALL::HasProperty < PropertyName, M1 > >
    {} ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct ValueType < PropertyName, ALGO_CALL::Compound < M0, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        ALGO_STATIC_ASSERT ( (ALGO_CALL::HasProperty < PropertyName, ALGO_CALL::Compound < M0, M1 > > ()), "Type must have the property in order to obtain it." ) ;
        
        typedef typename ALGO_LOGIC_CALL::eval_if <
            typename ALGO_CALL::HasProperty < PropertyName, M0 >::type
            , ALGO_CALL::ValueType < PropertyName, M0 >
            , ALGO_CALL::ValueType < PropertyName, M1 >
        >::type type ;
    } ;
    
    
    
    
    
    template < typename PropertyName, typename PassByType, typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct GetValue ;
    
    template < typename PropertyName, typename PassByType, typename PropertySet >
    struct GetValue < PropertyName, PassByType, PropertySet const, ALGO_ENABLE_IF_PARAM_DEFAULT > ;
    
    template < typename PropertyName, typename PassByType, typename AssociatedType >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, ALGO_ENABLE_IF_PARAM_DEFAULT > ;
    
    template < typename PropertyName, typename PassByType, typename M0, typename M1 >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT > ;
    
    
    
    template < typename PropertyName, typename PassByType, typename AssociatedType >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef GetValue type ;
        
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >& x )
        {
            return x.x ;
        }
    } ;
    
    namespace detail {
        
    // Not bothering with the final enable-if param as this is an implementation detail class.
    template < typename PropertyName, typename PassByType, typename M0, typename M1, bool InM0 >
    struct GetValue_Compound
    {
        typedef GetValue_Compound type ;
        
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
        typedef GetValue_Compound type ;
        
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 > >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( ALGO_CALL::Compound < M0, M1 >& x )
        {
            return ALGO_CALL::GetValue < PropertyName, PassByType, M1 >::apply ( x.m1 ) ;
        }
    } ;
        
    } // namespace detail
    
    template < typename PropertyName, typename PassByType, typename M0, typename M1 >
    struct GetValue < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef GetValue type ;
        
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, ALGO_CALL::Compound < M0, M1 > >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( ALGO_CALL::Compound < M0, M1 >& x )
        {
            return ALGO_DETAIL_CALL::GetValue_Compound < PropertyName, PassByType, M0, M1, ALGO_CALL::HasProperty < PropertyName, M0 >::type::value >::apply ( x ) ;
        }
    } ;
    
    template < typename PropertyName, typename PassByType, typename PropertySet >
    struct GetValue < PropertyName, PassByType, PropertySet const, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef GetValue type ;
        
        typedef typename ALGO_CALL::ValueReturnType < PropertyName, PassByType, PropertySet const >::type returnType ;
        
        ALGO_INLINE
        static returnType apply ( PropertySet const& x )
        {
            // Strip away const-ness and re-apply on the way out.
            return ALGO_CALL::GetValue < PropertyName, PassByType, PropertySet >::apply ( const_cast < PropertySet& > ( x ) ) ;
        }
    } ;
    
    template < typename PropertyName, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::ValueReturnType < PropertyName, ALGO_CALL::ByReference, PropertySet >::type getValueByReference ( PropertySet& x )
        ALGO_NOEXCEPT_DECL ( true )
    {
        return ALGO_CALL::GetValue < PropertyName, ALGO_CALL::ByReference, PropertySet >::apply ( x ) ;
    }
    
    template < typename PropertyName, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::ValueReturnType < PropertyName, ALGO_CALL::ByReference, PropertySet const >::type getValueByReference ( PropertySet const& x )
        ALGO_NOEXCEPT_DECL ( true )
    {
        return ALGO_CALL::GetValue < PropertyName, ALGO_CALL::ByReference, PropertySet const >::apply ( x ) ;
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
        return ALGO_CALL::GetValue < PropertyName, ALGO_CALL::ByValue, PropertySet const >::apply ( x ) ;
    }
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct AddPropertyType
    {
        ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty < PropertyName, PropertySet > ()), "Cant add a property to a structure which already has it" ) ;
        
        // Keep the first to be added into the set the first value in the aggregate assignment.
        typedef ALGO_CALL::Compound <
            typename ALGO_DETAIL_CALL::RemoveCVAndReference < PropertySet >::type
            , ALGO_CALL::ValueAndProperty <
                PropertyName
                , typename ALGO_DETAIL_CALL::RemoveCVAndReference < AssociatedType >::type > > type ;
    };
    
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::AddPropertyType < PropertyName, AssociatedType , PropertySet >::type addProperty ( PropertySet const& x, AssociatedType&& y )
    {
        return { x, ALGO_CALL::forward < AssociatedType > ( y ) } ;
    }
    
    
    
    template < typename PropertySet1, typename PropertySet2 ALGO_COMMA_ENABLE_IF_PARAM >
    struct MergePropertySetsType
    {
        // Requires set of Properties do not overlap.
        typedef ALGO_CALL::Compound < typename ALGO_DETAIL_CALL::RemoveCVAndReference < PropertySet1 >::type
                                    , typename ALGO_DETAIL_CALL::RemoveCVAndReference < PropertySet2 >::type > type ;
    } ;
     
    template < typename PropertySet1, typename PropertySet2 >
    ALGO_INLINE
    typename ALGO_CALL::MergePropertySetsType < PropertySet1, PropertySet2 >::type mergePropertySets ( PropertySet1&& x, PropertySet2&& y)
    {
        return { ALGO_CALL::forward < PropertySet1 > ( x ), ALGO_CALL::forward < PropertySet2 > ( y ) } ;
    }
    
    
    
    template < typename PropertyName, typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct SetValue ;
    
    template < typename PropertyName, typename AssociatedType >
    struct SetValue < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, ALGO_ENABLE_IF_PARAM_DEFAULT > ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct SetValue < PropertyName, ALGO_CALL::Compound < M0, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT > ;
    
    
    
    template < typename PropertyName, typename AssociatedType >
    struct SetValue < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef SetValue type ;
        
        template < class T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >& x, T&& y )
        {
            x.x = ALGO_CALL::forward < T > ( y ) ;
        }
    } ;
    
    namespace detail {
    
    // Not bothering with the final enable-if param as this is an implementation detail class.
    template < typename PropertyName, typename M0, typename M1, bool InM0 >
    struct SetValue_Compound
    {
        typedef SetValue_Compound type ;
        
        template < typename T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::Compound < M0, M1 >& x, T&& y )
        {
            ALGO_CALL::SetValue < PropertyName, M0 >::apply ( x.m0, ALGO_CALL::forward < T > ( y ) ) ;
        }
    } ;
    
    template < typename PropertyName, typename M0, typename M1 >
    struct SetValue_Compound < PropertyName, M0, M1, false >
    {
        typedef SetValue_Compound type ;
        
        template < typename T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::Compound < M0, M1 >& x, T&& y )
        {
            ALGO_CALL::SetValue < PropertyName, M1 >::apply ( x.m1, ALGO_CALL::forward < T > ( y ) ) ;
        }
    } ;
    
    } // namespace detail
    
    template < typename PropertyName, typename M0, typename M1 >
    struct SetValue < PropertyName, ALGO_CALL::Compound < M0, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef SetValue type ;
        
        template < typename T >
        ALGO_INLINE
        static void apply ( ALGO_CALL::Compound < M0, M1 >& x, T&& y )
        {
            ALGO_DETAIL_CALL::SetValue_Compound < PropertyName, M0, M1, ALGO_CALL::HasProperty < PropertyName, M0 >::type::value >::apply ( x, ALGO_CALL::forward < T > ( y ) ) ;
        }
    } ;
    
    // Do not implement this function! Can't update x as it is a const-reference.
    template < typename PropertyName, typename Value, typename PropertySet >
    void setValue ( PropertySet const& x, Value&& y, ALGO_CALL::InPlace ) ;
    
    template < typename PropertyName, typename Value, typename PropertySet >
    ALGO_INLINE
    void setValue ( PropertySet& x, Value&& y, ALGO_CALL::InPlace )
    {
#ifdef ALGO_OPTMISED_BUILD
        // Gives zero const set on optimised builds.
        ALGO_CALL::SetValue < PropertyName, PropertySet >::apply ( x, ALGO_CALL::forward < Value > ( y ) ) ;
#else
        // For some reason this produces quicker code on -O0 optimisation level.
        ALGO_CALL::getValueByReference < PropertyName > ( x ) = ALGO_CALL::forward < Value > ( y ) ;
#endif
    }
    
    template < typename PropertyName, typename Value, typename PropertySet >
    ALGO_INLINE
    PropertySet setValue ( PropertySet x, Value&& y, ALGO_CALL::ByReturnValue )
    {
        ALGO_CALL::setValue < PropertyName > ( x, ALGO_CALL::forward < Value > ( y ), ALGO_CALL::InPlace () ) ;
        return x ;
    }
    
    

    template < typename PropertyName, typename AssociatedType, typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct AddOrUpdateValueType
    {
        typedef typename ALGO_LOGIC_CALL::eval_if<
            ALGO_CALL::HasProperty < PropertyName, PropertySet >
            , PropertySet
            , ALGO_CALL::AddPropertyType < PropertyName, AssociatedType, PropertySet > >::type type ;
    } ;
    
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet, bool HasProperty ALGO_COMMA_ENABLE_IF_PARAM >
    struct AddOrUpdateValue
    {
        typedef AddOrUpdateValue type ;
        
        template < class T >
        ALGO_INLINE
        static
        typename ALGO_CALL::AddOrUpdateValueType < PropertyName, AssociatedType, PropertySet >::type
        apply ( PropertySet const& x, T&& y )
        {
            return ALGO_CALL::setValue < PropertyName > ( x, ALGO_CALL::forward < T > ( y ), ALGO_CALL::ByReturnValue () ) ;
        }
    } ;
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    struct AddOrUpdateValue < PropertyName, AssociatedType, PropertySet, false, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef AddOrUpdateValue type ;
        
        template < typename T >
        ALGO_INLINE
        static
        typename ALGO_CALL::AddOrUpdateValueType < PropertyName, AssociatedType, PropertySet >::type
        apply ( PropertySet const& x, T&& y )
        {
            return ALGO_CALL::addProperty < PropertyName > ( x, ALGO_CALL::forward < T > ( y ) ) ;
        }
    } ;
    
    template < typename PropertyName, typename AssociatedType, typename PropertySet >
    ALGO_INLINE
    typename ALGO_CALL::AddOrUpdateValueType < PropertyName, AssociatedType, PropertySet >::type addOrUpdateValue ( PropertySet const& x, AssociatedType&& y )
    {
        return ALGO_CALL::AddOrUpdateValue < PropertyName, AssociatedType, PropertySet, ALGO_CALL::HasProperty < PropertyName, PropertySet >::type::value >::apply ( x, ALGO_CALL::forward < AssociatedType > ( y ) ) ;
    }
    
    
    
    template < typename PropertySet, typename Visitor ALGO_COMMA_ENABLE_IF_PARAM >
    struct VisitValue ;
    
    template < typename PropertyName, typename AssociatedType, typename Visitor >
    struct VisitValue < ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, Visitor, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef VisitValue type ;
        
        typedef ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType > VisitedType ;
        
        ALGO_INLINE
        static
        void apply ( VisitedType& x, Visitor& v )
        {
            v.template visit < PropertyName > ( x.x ) ;
        }
        
        ALGO_INLINE
        static
        void apply ( VisitedType const& x, Visitor& v )
        {
            v.template visit < PropertyName > ( x.x ) ;
        }
    } ;
    
    template < typename M0, typename M1, typename Visitor >
    struct VisitValue < ALGO_CALL::Compound < M0, M1 >, Visitor, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef VisitValue type ;
        
        typedef ALGO_CALL::Compound < M0, M1 > VisitedType ;
        
        ALGO_INLINE
        static
        void apply ( VisitedType& x, Visitor& v )
        {
            VisitValue < M0, Visitor >::apply ( x.m0, v ) ;
            VisitValue < M1, Visitor >::apply ( x.m1, v ) ;
        }
        
        ALGO_INLINE
        static
        void apply ( VisitedType const& x, Visitor& v )
        {
            VisitValue < M0, Visitor >::apply ( x.m0, v ) ;
            VisitValue < M1, Visitor >::apply ( x.m1, v ) ;
        }
        
    } ;
    
    template < typename Visitor, typename PropertySet >
    Visitor visit ( PropertySet& x, Visitor v )
    {
        // Calls v.template visit < PropertyName > ( associatedValue ) for all items in x.
        ALGO_CALL::VisitValue < PropertySet, Visitor >::apply ( x, v ) ;
        return v ;
    }
    
    template < typename Visitor, typename PropertySet >
    Visitor visit ( PropertySet const& x, Visitor v )
    {
        // Calls v.template visit < PropertyName > ( associatedValue ) for all items in x.
        ALGO_CALL::VisitValue < PropertySet, Visitor >::apply ( x, v ) ;
        return v ;
    }
    
    namespace detail {
        
    template < typename ToPropertySet >
    struct PropertySetVisitor
    {
        typedef PropertySetVisitor type ;
        
        ToPropertySet* toUpdate ;
        
        template < typename PropertyName, typename Value >
        ALGO_INLINE
        typename ALGO_LOGIC_CALL::enable_if_pred < ALGO_CALL::HasProperty < PropertyName, ToPropertySet >, void >::type
        visit ( Value const& x )
        {
            ALGO_CALL::setValue < PropertyName > ( *toUpdate, x, ALGO_CALL::InPlace () ) ;
        }
        
        template < typename PropertyName, typename Value >
        ALGO_INLINE
        static typename ALGO_LOGIC_CALL::disable_if_pred < ALGO_CALL::HasProperty < PropertyName, ToPropertySet >, void >::type
        visit ( Value const& )
        {}
    } ;
        
    } // namespace detail
    
    template < typename FromPropertySet, typename ToPropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct ConvertPropertySet
    {
        typedef ConvertPropertySet type ;
        
        ALGO_INLINE
        static ToPropertySet apply ( FromPropertySet x )
        {
            ToPropertySet y = {} ;
            ALGO_DETAIL_CALL::PropertySetVisitor < ToPropertySet > v = { &y } ;
            
            ALGO_CALL::visit ( x, v ) ;
            
            return y ;
        }
    } ;
    
    template < typename FromPropertySet >
    struct ConvertPropertySet < FromPropertySet, FromPropertySet, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        ALGO_INLINE
        static FromPropertySet apply ( FromPropertySet x )
        {
            return x ;
        }
    } ;
    
    template < typename ToPropertySet, typename FromPropertySet >
    ALGO_INLINE
    ToPropertySet convertPropertySet ( FromPropertySet x )
    {
        return ALGO_CALL::ConvertPropertySet < FromPropertySet, ToPropertySet >::apply ( x ) ;
    }
    
    
    template < typename PropertyName, typename PropertySet ALGO_COMMA_ENABLE_IF_PARAM >
    struct RemoveProperty ;
    
    // Left undefined as if the algorithm gets to this point all bets are off
    template < typename PropertyName, typename AssociatedType >
    struct RemoveProperty < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType >, ALGO_ENABLE_IF_PARAM_DEFAULT > ;
    
    // No match
    template < typename PropertyName, typename PropertyName1, typename AssociatedType1 >
    struct RemoveProperty < PropertyName, ALGO_CALL::ValueAndProperty < PropertyName1, AssociatedType1 >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef ALGO_CALL::ValueAndProperty < PropertyName1, AssociatedType1 > type ;
    } ;
    
    // Compound cases
    
    template < typename PropertyName, typename M0, typename AssociatedType1 >
    struct RemoveProperty < PropertyName, ALGO_CALL::Compound < M0, ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType1 > >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty < PropertyName, M0 > ()), "Found two matches within a set!" ) ;
        typedef M0 type ;
    } ;
    
    template < typename PropertyName, typename AssociatedType0, typename M1 >
    struct RemoveProperty < PropertyName, ALGO_CALL::Compound < ALGO_CALL::ValueAndProperty < PropertyName, AssociatedType0 >, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        ALGO_STATIC_ASSERT ( (!ALGO_CALL::HasProperty < PropertyName, M1 > ()), "Found two matches within a set!" ) ;
        typedef M1 type ;
    } ;
    
    // Recursive case
    template < typename PropertyName, typename M0, typename M1 >
    struct RemoveProperty < PropertyName, ALGO_CALL::Compound < M0, M1 >, ALGO_ENABLE_IF_PARAM_DEFAULT >
    {
        typedef ALGO_CALL::Compound < typename ALGO_CALL::RemoveProperty < PropertyName, M0 >::type, typename ALGO_CALL::RemoveProperty<PropertyName, M1 >::type > type ;
    } ;
    
} // namespace algo

#endif
