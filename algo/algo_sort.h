#ifndef INCLUDED_ALGO_SORT
#define INCLUDED_ALGO_SORT

#ifndef INCLUDED_ALGO
#include "algo.h"
#endif


#define ALGO_SORT_SORT_2 ALGO_SWAP(0, 1)

//Network for N=3, using Bose-Nelson Algorithm.
#define ALGO_SORT_SORT_3 ALGO_SWAP(1, 2) ALGO_SWAP(0, 2) ALGO_SWAP(0, 1)

//Network for N=4, using Bose-Nelson Algorithm.
#define ALGO_SORT_SORT_4 ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(0, 2) ALGO_SWAP(1, 3) ALGO_SWAP(1, 2)

//Network for N=5, using Bose-Nelson Algorithm.
#define ALGO_SORT_SORT_5 ALGO_SWAP(0, 1) ALGO_SWAP(3, 4) ALGO_SWAP(2, 4) ALGO_SWAP(2, 3) ALGO_SWAP(1, 4) ALGO_SWAP(0, 3) ALGO_SWAP(0, 2) ALGO_SWAP(1, 3) ALGO_SWAP(1, 2)

//Network for N=6, using Bose-Nelson Algorithm.
#define ALGO_SORT_SORT_6 ALGO_SWAP(1, 2) ALGO_SWAP(0, 2) ALGO_SWAP(0, 1) ALGO_SWAP(4, 5) ALGO_SWAP(3, 5) ALGO_SWAP(3, 4) ALGO_SWAP(0, 3) ALGO_SWAP(1, 4) ALGO_SWAP(2, 5) ALGO_SWAP(2, 4) ALGO_SWAP(1, 3) ALGO_SWAP(2, 3)

//Network for N=7, using Bose-Nelson Algorithm.
#define ALGO_SORT_SORT_7 ALGO_SWAP(1, 2) ALGO_SWAP(0, 2) ALGO_SWAP(0, 1) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(3, 5) ALGO_SWAP(4, 6) ALGO_SWAP(4, 5) ALGO_SWAP(0, 4) ALGO_SWAP(0, 3) ALGO_SWAP(1, 5) ALGO_SWAP(2, 6) ALGO_SWAP(2, 5) ALGO_SWAP(1, 3) ALGO_SWAP(2, 4) ALGO_SWAP(2, 3)

//Network for N=8, using Bose-Nelson Algorithm.
#define ALGO_SORT_SORT_8 ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(0, 2) ALGO_SWAP(1, 3) ALGO_SWAP(1, 2) ALGO_SWAP(4, 5) ALGO_SWAP(6, 7) ALGO_SWAP(4, 6) ALGO_SWAP(5, 7) ALGO_SWAP(5, 6) ALGO_SWAP(0, 4) ALGO_SWAP(1, 5) ALGO_SWAP(1, 4) ALGO_SWAP(2, 6) ALGO_SWAP(3, 7) ALGO_SWAP(3, 6) ALGO_SWAP(2, 4) ALGO_SWAP(3, 5) ALGO_SWAP(3, 4)

//Network for N=9, using Best Known Arrangement.
#define ALGO_SORT_SORT_9 ALGO_SWAP(0, 1) ALGO_SWAP(3, 4) ALGO_SWAP(6, 7) ALGO_SWAP(1, 2) ALGO_SWAP(4, 5) ALGO_SWAP(7, 8) ALGO_SWAP(0, 1) ALGO_SWAP(3, 4) ALGO_SWAP(6, 7) ALGO_SWAP(0, 3) ALGO_SWAP(3, 6) ALGO_SWAP(0, 3) ALGO_SWAP(1, 4) ALGO_SWAP(4, 7) ALGO_SWAP(1, 4) ALGO_SWAP(2, 5) ALGO_SWAP(5, 8) ALGO_SWAP(2, 5) ALGO_SWAP(1, 3) ALGO_SWAP(5, 7) ALGO_SWAP(2, 6) ALGO_SWAP(4, 6) ALGO_SWAP(2, 4) ALGO_SWAP(2, 3) ALGO_SWAP(5, 6)

//Network for N=10, using Best Known Arrangement.
#define ALGO_SORT_SORT_10 ALGO_SWAP(4, 9) ALGO_SWAP(3, 8) ALGO_SWAP(2, 7) ALGO_SWAP(1, 6) ALGO_SWAP(0, 5) ALGO_SWAP(1, 4) ALGO_SWAP(6, 9) ALGO_SWAP(0, 3) ALGO_SWAP(5, 8) ALGO_SWAP(0, 2) ALGO_SWAP(3, 6) ALGO_SWAP(7, 9) ALGO_SWAP(0, 1) ALGO_SWAP(2, 4) ALGO_SWAP(5, 7) ALGO_SWAP(8, 9) ALGO_SWAP(1, 2) ALGO_SWAP(4, 6) ALGO_SWAP(7, 8) ALGO_SWAP(3, 5) ALGO_SWAP(2, 5) ALGO_SWAP(6, 8) ALGO_SWAP(1, 3) ALGO_SWAP(4, 7) ALGO_SWAP(2, 3) ALGO_SWAP(6, 7) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(4, 5)

//Network for N=11, using Best Known Arrangement.
#define ALGO_SORT_SORT_11 ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(4, 5) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9) ALGO_SWAP(1, 3) ALGO_SWAP(5, 7) ALGO_SWAP(0, 2) ALGO_SWAP(4, 6) ALGO_SWAP(8, 10) ALGO_SWAP(1, 2) ALGO_SWAP(5, 6) ALGO_SWAP(9, 10) ALGO_SWAP(1, 5) ALGO_SWAP(6, 10) ALGO_SWAP(5, 9) ALGO_SWAP(2, 6) ALGO_SWAP(1, 5) ALGO_SWAP(6, 10) ALGO_SWAP(0, 4) ALGO_SWAP(3, 7) ALGO_SWAP(4, 8) ALGO_SWAP(0, 4) ALGO_SWAP(1, 4) ALGO_SWAP(7, 10) ALGO_SWAP(3, 8) ALGO_SWAP(2, 3) ALGO_SWAP(8, 9) ALGO_SWAP(2, 4) ALGO_SWAP(7, 9) ALGO_SWAP(3, 5) ALGO_SWAP(6, 8) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(7, 8)

//Network for N=SWAP12, using Best Known Arrangement.
#define ALGO_SORT_SORT_12 ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(4, 5) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9) ALGO_SWAP(10, 11) ALGO_SWAP(1, 3) ALGO_SWAP(5, 7) ALGO_SWAP(9, 11) ALGO_SWAP(0, 2) ALGO_SWAP(4, 6) ALGO_SWAP(8, 10) ALGO_SWAP(1, 2) ALGO_SWAP(5, 6) ALGO_SWAP(9, 10) ALGO_SWAP(1, 5) ALGO_SWAP(6, 10) ALGO_SWAP(5, 9) ALGO_SWAP(2, 6) ALGO_SWAP(1, 5) ALGO_SWAP(6, 10) ALGO_SWAP(0, 4) ALGO_SWAP(7, 11) ALGO_SWAP(3, 7) ALGO_SWAP(4, 8) ALGO_SWAP(0, 4) ALGO_SWAP(7, 11) ALGO_SWAP(1, 4) ALGO_SWAP(7, 10) ALGO_SWAP(3, 8) ALGO_SWAP(2, 3) ALGO_SWAP(8, 9) ALGO_SWAP(2, 4) ALGO_SWAP(7, 9) ALGO_SWAP(3, 5) ALGO_SWAP(6, 8) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(7, 8)

//Network for N=13, using Best Known Arrangement.
#define ALGO_SORT_SORT_13 ALGO_SWAP(1, 7) ALGO_SWAP(9, 11) ALGO_SWAP(3, 4) ALGO_SWAP(5, 8) ALGO_SWAP(0, 12) ALGO_SWAP(2, 6) ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(4, 6) ALGO_SWAP(8, 11) ALGO_SWAP(7, 12) ALGO_SWAP(5, 9) ALGO_SWAP(0, 2) ALGO_SWAP(3, 7) ALGO_SWAP(10, 11) ALGO_SWAP(1, 4) ALGO_SWAP(6, 12) ALGO_SWAP(7, 8) ALGO_SWAP(11, 12) ALGO_SWAP(4, 9) ALGO_SWAP(6, 10) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(8, 9) ALGO_SWAP(10, 11) ALGO_SWAP(1, 7) ALGO_SWAP(2, 6) ALGO_SWAP(9, 11) ALGO_SWAP(1, 3) ALGO_SWAP(4, 7) ALGO_SWAP(8, 10) ALGO_SWAP(0, 5) ALGO_SWAP(2, 5) ALGO_SWAP(6, 8) ALGO_SWAP(9, 10) ALGO_SWAP(1, 2) ALGO_SWAP(3, 5) ALGO_SWAP(7, 8) ALGO_SWAP(4, 6) ALGO_SWAP(2, 3) ALGO_SWAP(4, 5) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6)

//Network for N=14, using Best Known Arrangement.
#define ALGO_SORT_SORT_14 ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(4, 5) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9) ALGO_SWAP(10, 11) ALGO_SWAP(12, 13) ALGO_SWAP(0, 2) ALGO_SWAP(4, 6) ALGO_SWAP(8, 10) ALGO_SWAP(1, 3) ALGO_SWAP(5, 7) ALGO_SWAP(9, 11) ALGO_SWAP(0, 4) ALGO_SWAP(8, 12) ALGO_SWAP(1, 5) ALGO_SWAP(9, 13) ALGO_SWAP(2, 6) ALGO_SWAP(3, 7) ALGO_SWAP(0, 8) ALGO_SWAP(1, 9) ALGO_SWAP(2, 10) ALGO_SWAP(3, 11) ALGO_SWAP(4, 12) ALGO_SWAP(5, 13) ALGO_SWAP(5, 10) ALGO_SWAP(6, 9) ALGO_SWAP(3, 12) ALGO_SWAP(7, 11) ALGO_SWAP(1, 2) ALGO_SWAP(4, 8) ALGO_SWAP(1, 4) ALGO_SWAP(7, 13) ALGO_SWAP(2, 8) ALGO_SWAP(2, 4) ALGO_SWAP(5, 6) ALGO_SWAP(9, 10) ALGO_SWAP(11, 13) ALGO_SWAP(3, 8) ALGO_SWAP(7, 12) ALGO_SWAP(6, 8) ALGO_SWAP(10, 12) ALGO_SWAP(3, 5) ALGO_SWAP(7, 9) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(7, 8) ALGO_SWAP(9, 10) ALGO_SWAP(11, 12) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9)

//Network for N=15, using Best Known Arrangement.
#define ALGO_SORT_SORT_15 ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(4, 5) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9) ALGO_SWAP(10, 11) ALGO_SWAP(12, 13) ALGO_SWAP(0, 2) ALGO_SWAP(4, 6) ALGO_SWAP(8, 10) ALGO_SWAP(12, 14) ALGO_SWAP(1, 3) ALGO_SWAP(5, 7) ALGO_SWAP(9, 11) ALGO_SWAP(0, 4) ALGO_SWAP(8, 12) ALGO_SWAP(1, 5) ALGO_SWAP(9, 13) ALGO_SWAP(2, 6) ALGO_SWAP(10, 14) ALGO_SWAP(3, 7) ALGO_SWAP(0, 8) ALGO_SWAP(1, 9) ALGO_SWAP(2, 10) ALGO_SWAP(3, 11) ALGO_SWAP(4, 12) ALGO_SWAP(5, 13) ALGO_SWAP(6, 14) ALGO_SWAP(5, 10) ALGO_SWAP(6, 9) ALGO_SWAP(3, 12) ALGO_SWAP(13, 14) ALGO_SWAP(7, 11) ALGO_SWAP(1, 2) ALGO_SWAP(4, 8) ALGO_SWAP(1, 4) ALGO_SWAP(7, 13) ALGO_SWAP(2, 8) ALGO_SWAP(11, 14) ALGO_SWAP(2, 4) ALGO_SWAP(5, 6) ALGO_SWAP(9, 10) ALGO_SWAP(11, 13) ALGO_SWAP(3, 8) ALGO_SWAP(7, 12) ALGO_SWAP(6, 8) ALGO_SWAP(10, 12) ALGO_SWAP(3, 5) ALGO_SWAP(7, 9) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(7, 8) ALGO_SWAP(9, 10) ALGO_SWAP(11, 12) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9)

#define ALGO_SORT_SORT_16 ALGO_SWAP(0, 1) ALGO_SWAP(2, 3) ALGO_SWAP(4, 5) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9) ALGO_SWAP(10, 11) ALGO_SWAP(12, 13) ALGO_SWAP(14, 15) ALGO_SWAP(0, 2) ALGO_SWAP(4, 6) ALGO_SWAP(8, 10) ALGO_SWAP(12, 14) ALGO_SWAP(1, 3) ALGO_SWAP(5, 7) ALGO_SWAP(9, 11) ALGO_SWAP(13, 15) ALGO_SWAP(0, 4) ALGO_SWAP(8, 12) ALGO_SWAP(1, 5) ALGO_SWAP(9, 13) ALGO_SWAP(2, 6) ALGO_SWAP(10, 14) ALGO_SWAP(3, 7) ALGO_SWAP(11, 15) ALGO_SWAP(0, 8) ALGO_SWAP(1, 9) ALGO_SWAP(2, 10) ALGO_SWAP(3, 11) ALGO_SWAP(4, 12) ALGO_SWAP(5, 13) ALGO_SWAP(6, 14) ALGO_SWAP(7, 15) ALGO_SWAP(5, 10) ALGO_SWAP(6, 9) ALGO_SWAP(3, 12) ALGO_SWAP(13, 14) ALGO_SWAP(7, 11) ALGO_SWAP(1, 2) ALGO_SWAP(4, 8) ALGO_SWAP(1, 4) ALGO_SWAP(7, 13) ALGO_SWAP(2, 8) ALGO_SWAP(11, 14) ALGO_SWAP(2, 4) ALGO_SWAP(5, 6) ALGO_SWAP(9, 10) ALGO_SWAP(11, 13) ALGO_SWAP(3, 8) ALGO_SWAP(7, 12) ALGO_SWAP(6, 8) ALGO_SWAP(10, 12) ALGO_SWAP(3, 5) ALGO_SWAP(7, 9) ALGO_SWAP(3, 4) ALGO_SWAP(5, 6) ALGO_SWAP(7, 8) ALGO_SWAP(9, 10) ALGO_SWAP(11, 12) ALGO_SWAP(6, 7) ALGO_SWAP(8, 9)



#define ODD_EVEN_2() ALGO_SWAP(0,1) ;

#define ODD_EVEN_3_SUBa() ALGO_SWAP(0,1) ;
#define ODD_EVEN_3_SUBb() ALGO_SWAP(1,2) ;
#define ODD_EVEN_3() ODD_EVEN_3_SUBa() ODD_EVEN_3_SUBb() ODD_EVEN_3_SUBa()

#define ODD_EVEN_4_SUB() ALGO_SWAP(0,1), ALGO_SWAP(2,3) ; ALGO_SWAP(1,2) ;
#define ODD_EVEN_4() ODD_EVEN_4_SUB() ODD_EVEN_4_SUB()

#define ODD_EVEN_5_SUBa() ALGO_SWAP(0,1), ALGO_SWAP(2,3) ;
#define ODD_EVEN_5_SUBb() ALGO_SWAP(1,2), ALGO_SWAP(3,4) ;
#define ODD_EVEN_5() ODD_EVEN_5_SUBa() ODD_EVEN_5_SUBb() ODD_EVEN_5_SUBa() ODD_EVEN_5_SUBb() ODD_EVEN_5_SUBa()

#define ODD_EVEN_6_SUB() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5) ; ALGO_SWAP(1,2), ALGO_SWAP(3,4) ;
#define ODD_EVEN_6() ODD_EVEN_6_SUB() ODD_EVEN_6_SUB() ODD_EVEN_6_SUB()

#define ODD_EVEN_7_SUBa() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5) ;
#define ODD_EVEN_7_SUBb() ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6) ;
#define ODD_EVEN_7() ODD_EVEN_7_SUBa() ODD_EVEN_7_SUBb() ODD_EVEN_7_SUBa() ODD_EVEN_7_SUBb() ODD_EVEN_7_SUBa() ODD_EVEN_7_SUBb() ODD_EVEN_7_SUBa()

#define ODD_EVEN_8_SUB() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7) ; ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6) ;
#define ODD_EVEN_8() ODD_EVEN_8_SUB() ODD_EVEN_8_SUB() ODD_EVEN_8_SUB() ODD_EVEN_8_SUB()

#define ODD_EVEN_9_SUBa() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7) ;
#define ODD_EVEN_9_SUBb() ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8) ;
#define ODD_EVEN_9() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa()

#define ODD_EVEN_10_SUB() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7), ALGO_SWAP(8,9) ; ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8) ;
#define ODD_EVEN_10() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB()

#define ODD_EVEN_11_SUBa() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7), ALGO_SWAP(8,9) ;
#define ODD_EVEN_11_SUBb() ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8), ALGO_SWAP(9,10) ;
#define ODD_EVEN_11() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa()

#define ODD_EVEN_12_SUB() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7), ALGO_SWAP(8,9), ALGO_SWAP(10,11) ; ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8), ALGO_SWAP(9,10) ;
#define ODD_EVEN_12() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB()

#define ODD_EVEN_13_SUBa() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7), ALGO_SWAP(8,9), ALGO_SWAP(10,11) ;
#define ODD_EVEN_13_SUBb() ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8), ALGO_SWAP(9,10), ALGO_SWAP(11,12) ;
#define ODD_EVEN_13() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa()

#define ODD_EVEN_14_SUB() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7), ALGO_SWAP(8,9), ALGO_SWAP(10,11), ALGO_SWAP(12,13)  ; ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8), ALGO_SWAP(9,10), ALGO_SWAP(11,12) ;
#define ODD_EVEN_14() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB()

#define ODD_EVEN_15_SUBa() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7), ALGO_SWAP(8,9), ALGO_SWAP(10,11), ALGO_SWAP(12,13) ;
#define ODD_EVEN_15_SUBb() ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8), ALGO_SWAP(9,10), ALGO_SWAP(11,12), ALGO_SWAP(13,14) ;
#define ODD_EVEN_15() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa()

#define ODD_EVEN_16_SUB() ALGO_SWAP(0,1), ALGO_SWAP(2,3), ALGO_SWAP(4,5), ALGO_SWAP(6,7), ALGO_SWAP(8,9), ALGO_SWAP(10,11), ALGO_SWAP(12,13), ALGO_SWAP(14,15) ; ALGO_SWAP(1,2), ALGO_SWAP(3,4), ALGO_SWAP(5,6), ALGO_SWAP(7,8), ALGO_SWAP(9,10), ALGO_SWAP(11,12), ALGO_SWAP(13,14) ;
#define ODD_EVEN_16() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB()

namespace algo
{
    struct Unstable {} ;
    struct Stable {} ;
    struct Exchange {} ;
    struct ExchangeIndices {} ;
    
    struct UnstableExchange : Unstable, Exchange {} ;
    struct StableExchange : Stable, Exchange {} ;
    
    struct UnstableExchangeIndices : Unstable, ExchangeIndices {} ;
    struct StableExchangeIndices : Stable, ExchangeIndices {} ;
    
    typedef unsigned int IndexType ;
    

    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort0 ( Iter x, Cmp cmp, StableExchange )
    {}
    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort1 ( Iter x, Cmp cmp, StableExchange )
    {}
    
// Note the missing semi-colon or comma at the end of the macro definition, to allow a smarter compiler/optimiser to take better advantage of the parallel operations.
#define ALGO_SWAP(lower, upper) swap_if ( cmp ( tmp [ lower + 1 ], tmp [ lower ] ), tmp [ lower + 1 ], tmp [ lower ], Unpredictable () )
    
#define ALGO_SORT(n) \
template < class Iter, class Cmp >\
void sort ## n  ( Iter x, Cmp cmp, StableExchange )\
{\
    typename std::iterator_traits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted ( x, n, tmp, ALGO_CALL::MoveForwardOperation::type () ) ;\
    ODD_EVEN_ ## n ()\
    ALGO_CALL::stepCounted ( tmp, n, x, ALGO_CALL::MoveForwardOperation::type () ) ;\
}

    ALGO_SORT(2)
    ALGO_SORT(3)
    ALGO_SORT(4)
    ALGO_SORT(5)
    ALGO_SORT(6)
    ALGO_SORT(7)
    ALGO_SORT(8)
    ALGO_SORT(9)
    ALGO_SORT(10)
    ALGO_SORT(11)
    ALGO_SORT(12)
    ALGO_SORT(13)
    ALGO_SORT(14)
    ALGO_SORT(15)
    ALGO_SORT(16)
    
#undef ALGO_SORT
#undef ALGO_SWAP
    
    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort0 ( Iter x, Cmp cmp, StableExchangeIndices )
    {}
    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort1 ( Iter x, Cmp cmp, StableExchangeIndices )
    {}
    
#define ALGO_SWAP(lower, upper) swap_if ( cmp ( tmp [ indices [ lower + 1 ] ], tmp [ indices [ lower ] ] ), indices [ lower + 1 ], indices [ lower ], Ternary () )
    
#define ALGO_SORT(n) \
template < class Iter, class Cmp >\
void sort ## n  ( Iter x, Cmp cmp, StableExchangeIndices )\
{\
    typename std::iterator_traits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted ( x, n, tmp, ALGO_CALL::MoveForwardOperation::type () ) ;\
    IndexType indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } ;\
    ODD_EVEN_ ## n ()\
    for ( int i = 0 ; i <  n ; ++i ){ *x = std::move ( tmp [ indices [ i ] ] ) ; ++x ; }\
}
    
    ALGO_SORT(2)
    ALGO_SORT(3)
    ALGO_SORT(4)
    ALGO_SORT(5)
    ALGO_SORT(6)
    ALGO_SORT(7)
    ALGO_SORT(8)
    ALGO_SORT(9)
    ALGO_SORT(10)
    ALGO_SORT(11)
    ALGO_SORT(12)
    ALGO_SORT(13)
    ALGO_SORT(14)
    ALGO_SORT(15)
    ALGO_SORT(16)
    
#undef ALGO_SORT
#undef ALGO_SWAP
    
    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort0 ( Iter x, Cmp cmp, UnstableExchange )
    {}
    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort1 ( Iter x, Cmp cmp, UnstableExchange )
    {}
    
#define ALGO_SWAP(lower, upper) swap_if ( cmp ( tmp [ upper ], tmp [ lower ] ), tmp [ upper ], tmp [ lower ], Unpredictable () ) ;
    
#define ALGO_SORT(n) \
template < class Iter, class Cmp > \
void sort ## n ( Iter x, Cmp cmp, UnstableExchange )\
{\
    typename std::iterator_traits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted ( x, n, tmp, ALGO_CALL::MoveForwardOperation::type () ) ;\
    ALGO_SORT_SORT_ ## n \
    ALGO_CALL::stepCounted ( tmp, n, x, ALGO_CALL::MoveForwardOperation::type () ) ;\
}

    ALGO_SORT(2)
    ALGO_SORT(3)
    ALGO_SORT(4)
    ALGO_SORT(5)
    ALGO_SORT(6)
    ALGO_SORT(7)
    ALGO_SORT(8)
    ALGO_SORT(9)
    ALGO_SORT(10)
    ALGO_SORT(11)
    ALGO_SORT(12)
    ALGO_SORT(13)
    ALGO_SORT(14)
    ALGO_SORT(15)
    ALGO_SORT(16)
    
#undef ALGO_SORT
#undef ALGO_SWAP
    
    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort0 ( Iter x, Cmp cmp, UnstableExchangeIndices )
    {}
    
    template < class Iter, class Cmp >
    ALGO_INLINE
    inline void sort1 ( Iter x, Cmp cmp, UnstableExchangeIndices )
    {}
    
#define ALGO_SWAP(lower, upper) swap_if ( cmp ( tmp [ indices [ upper ] ], tmp [ indices [ lower ] ] ), indices [ upper ], indices [ lower ], Ternary () ) ;
    
#define ALGO_SORT(n) \
template < class Iter, class Cmp > \
void sort ## n ( Iter x, Cmp cmp, UnstableExchangeIndices )\
{\
    typename std::iterator_traits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted ( x, n, tmp, ALGO_CALL::MoveForwardOperation::type () ) ;\
    IndexType indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } ;\
    ALGO_SORT_SORT_ ## n \
    for ( int i = 0 ; i < n ; ++i ){ *x = std::move ( tmp [ indices [ i ] ] ) ; ++x ; }\
}
    
    ALGO_SORT(2)
    ALGO_SORT(3)
    ALGO_SORT(4)
    ALGO_SORT(5)
    ALGO_SORT(6)
    ALGO_SORT(7)
    ALGO_SORT(8)
    ALGO_SORT(9)
    ALGO_SORT(10)
    ALGO_SORT(11)
    ALGO_SORT(12)
    ALGO_SORT(13)
    ALGO_SORT(14)
    ALGO_SORT(15)
    ALGO_SORT(16)
    
#undef ALGO_SORT
#undef ALGO_SWAP
    
    
    
    template < int N >
    struct Int {} ;
    
    
    
#define ALGO_SORT(n) \
template < typename Iter, typename Cmp, typename Tag >\
ALGO_INLINE \
void sort ( Iter x, Cmp cmp, Tag, Int < n > )\
{\
    sort ## n ( x, cmp, Tag () ) ;\
}
    
    ALGO_SORT(0)
    ALGO_SORT(1)
    ALGO_SORT(2)
    ALGO_SORT(3)
    ALGO_SORT(4)
    ALGO_SORT(5)
    ALGO_SORT(6)
    ALGO_SORT(7)
    ALGO_SORT(8)
    ALGO_SORT(9)
    ALGO_SORT(10)
    ALGO_SORT(11)
    ALGO_SORT(12)
    ALGO_SORT(13)
    ALGO_SORT(14)
    ALGO_SORT(15)
    ALGO_SORT(16)
    
#undef ALGO_SORT
    
} // namespace algo

#endif
