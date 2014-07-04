#ifndef INCLUDED_ALGO_BASICS_H
#define INCLUDED_ALGO_BASICS_H

#ifndef INCLUDED_CASSERT
#include <cassert>
#define INCLUDED_CASSERT
#endif


#ifndef ALGO_INLINE
#define ALGO_INLINE __attribute__((always_inline)) inline
#endif

#ifndef ALGO_ASSERT
#define ALGO_ASSERT(x) assert ( ( x ) )
#endif

#ifndef ALGO_SAFE_MODE
#define ALGO_SAFE_MODE(x) x
#endif

#ifndef ALGO_LIKELIHOOD
#define ALGO_LIKELIHOOD(test, likelihood) __builtin_expect ( !!( test ), ( likelihood ) )
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
} // namespace algo

#endif
