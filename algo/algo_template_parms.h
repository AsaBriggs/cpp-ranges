#ifndef INCLUDED_ALGO_TEMPLATE_PARAMS
#define INCLUDED_ALGO_TEMPLATE_PARAMS

namespace algo
{
// The named template parameter scheme relies on tagged parameters being passed in
// The tag is an identifier for the Find metafunction to scan over.
template < typename Tag, typename Value >
struct Param
{};

    
    
template < typename T >
struct ParamTag ;

template < typename Tag, typename Value >
struct ParamTag <
    Param < Tag, Value > >
{
    typedef Tag type ;
};

    
    
template < typename T >
struct ParamValue ;

template < typename Tag, typename Value >
struct ParamValue <
    Param < Tag, Value > >
{
    typedef Value type ;
};
    
    
    
struct DefaultTag {} ;
struct DefaultValue {} ;
typedef Param < DefaultTag, DefaultValue > DefaultParam ;

    
    
struct NotFoundTag {} ; // To be used by the Find metafunction only
typedef Param < NotFoundTag, DefaultValue > NotFoundParam ; // To be used by the Find metafunction only

    
    
template <
    typename P0 = DefaultParam
    , typename P1 = DefaultParam
    , typename P2 = DefaultParam
    , typename P3 = DefaultParam
    , typename P4 = DefaultParam
    , typename P5 = DefaultParam
    , typename P6 = DefaultParam
    , typename P7 = DefaultParam
    , typename P8 = DefaultParam
    , typename P9 = DefaultParam >
struct Parameters
{
    typedef P0 Param0 ;
    typedef P1 Param1 ;
    typedef P2 Param2 ;
    typedef P3 Param3 ;
    typedef P4 Param4 ;
    typedef P5 Param5 ;
    typedef P6 Param6 ;
    typedef P7 Param7 ;
    typedef P8 Param8 ;
    typedef P9 Param9 ;
} ;

// Do not define the primary template; this prevents problems when clients specify a Parameter that is not a Param.
// Find returns the first Param Value with Tag matching TagToFind, or else DefaultIfNotFound if this does not exist.
// It recurses, filling in the right-hand parameter with NotFoundParam rather than DefaultParam in order to
// prevent ambiguities, and to ensure that all ParamX get scanned.
// Each ParamX must be of type Param.
template <
    typename TagToFind
    , typename DefaultIfNotFound
    , typename Param0
    , typename Param1
    , typename Param2
    , typename Param3
    , typename Param4
    , typename Param5
    , typename Param6
    , typename Param7
    , typename Param8
    , typename Param9 >
struct Find ;



// Base case, TagToFind not found so return DefaultIfNotFound
template < typename TagToFind
, typename DefaultIfNotFound >
struct Find <
    TagToFind
    , DefaultIfNotFound
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam
    , NotFoundParam >
{
    typedef DefaultIfNotFound type ;
} ;



// General case, Param0 is not the tag we are looking for, so
template <
    typename TagToFind
    , typename DefaultIfNotFound
    , typename Param0Tag
    , typename Param0Value
    , typename Param1
    , typename Param2
    , typename Param3
    , typename Param4
    , typename Param5
    , typename Param6
    , typename Param7
    , typename Param8
    , typename Param9 >
struct Find <
    TagToFind
    , DefaultIfNotFound
    , Param < Param0Tag, Param0Value >
    , Param1
    , Param2
    , Param3
    , Param4
    , Param5
    , Param6
    , Param7
    , Param8
    , Param9 >
{
    typedef typename Find <
        TagToFind
        , DefaultIfNotFound
        , Param1
        , Param2
        , Param3
        , Param4
        , Param5
        , Param6
        , Param7
        , Param8
        , Param9
        , NotFoundParam
    > ::type type ;
} ;


// Base case, tag is found
template <
    typename TagToFind
    , typename DefaultIfNotFound
    , typename Param0Value
    , typename Param1
    , typename Param2
    , typename Param3
    , typename Param4
    , typename Param5
    , typename Param6
    , typename Param7
    , typename Param8
    , typename Param9 >
struct Find <
    TagToFind
    , DefaultIfNotFound
    , Param < TagToFind, Param0Value >
    , Param1
    , Param2
    , Param3
    , Param4
    , Param5
    , Param6
    , Param7
    , Param8
    , Param9 >
{
    typedef Param0Value type ;
} ;

    
// Convenience function to unpack TagToFindAndDefault and Params into constituents
template <
    typename TagToFindAndDefault
    , typename Params >
struct FindWrapper
{
    typedef typename Find <
        typename ParamTag < TagToFindAndDefault >::type
        , typename ParamValue < TagToFindAndDefault >::type
        , typename Params::Param0
        , typename Params::Param1
        , typename Params::Param2
        , typename Params::Param3
        , typename Params::Param4
        , typename Params::Param5
        , typename Params::Param6
        , typename Params::Param7
        , typename Params::Param8
        , typename Params::Param9
    >::type type ;
} ;

template <
    typename Params
    , typename Defaults >
struct DeduceTypes
{
    typedef Parameters <
        typename FindWrapper < typename Defaults::Param0, Params >::type
        , typename FindWrapper < typename Defaults::Param1, Params >::type
        , typename FindWrapper < typename Defaults::Param2, Params >::type
        , typename FindWrapper < typename Defaults::Param3, Params >::type
        , typename FindWrapper < typename Defaults::Param4, Params >::type
        , typename FindWrapper < typename Defaults::Param5, Params >::type
        , typename FindWrapper < typename Defaults::Param6, Params >::type
        , typename FindWrapper < typename Defaults::Param7, Params >::type
        , typename FindWrapper < typename Defaults::Param8, Params >::type
        , typename FindWrapper < typename Defaults::Param9, Params >::type
    > type ;
} ;

} // namespace algo
    
#endif
