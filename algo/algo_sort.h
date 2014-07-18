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

#define ODD_EVEN_PARALLEL_START() (void)0
#define ODD_EVEN_PARALLEL_END() ;


#define ODD_EVEN_2() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ODD_EVEN_PARALLEL_END()

#define ODD_EVEN_3_SUBa() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_3_SUBb() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_3() ODD_EVEN_3_SUBa() ODD_EVEN_3_SUBb() ODD_EVEN_3_SUBa()

#define ODD_EVEN_4_SUB() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ODD_EVEN_PARALLEL_END() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_4() ODD_EVEN_4_SUB() ODD_EVEN_4_SUB()

#define ODD_EVEN_5_SUBa() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_5_SUBb() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_5() ODD_EVEN_5_SUBa() ODD_EVEN_5_SUBb() ODD_EVEN_5_SUBa() ODD_EVEN_5_SUBb() ODD_EVEN_5_SUBa()

#define ODD_EVEN_6_SUB() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ODD_EVEN_PARALLEL_END() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_6() ODD_EVEN_6_SUB() ODD_EVEN_6_SUB() ODD_EVEN_6_SUB()

#define ODD_EVEN_7_SUBa() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_7_SUBb() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_7() ODD_EVEN_7_SUBa() ODD_EVEN_7_SUBb() ODD_EVEN_7_SUBa() ODD_EVEN_7_SUBb() ODD_EVEN_7_SUBa() ODD_EVEN_7_SUBb() ODD_EVEN_7_SUBa()

#define ODD_EVEN_8_SUB() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ODD_EVEN_PARALLEL_END() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_8() ODD_EVEN_8_SUB() ODD_EVEN_8_SUB() ODD_EVEN_8_SUB() ODD_EVEN_8_SUB()

#define ODD_EVEN_9_SUBa() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_9_SUBb() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_9() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa() ODD_EVEN_9_SUBb() ODD_EVEN_9_SUBa()

#define ODD_EVEN_10_SUB() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ALGO_SWAP(8,9) ODD_EVEN_PARALLEL_END() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_10() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB() ODD_EVEN_10_SUB()

#define ODD_EVEN_11_SUBa() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ALGO_SWAP(8,9) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_11_SUBb() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ALGO_SWAP(9,10) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_11() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa() ODD_EVEN_11_SUBb() ODD_EVEN_11_SUBa()

#define ODD_EVEN_12_SUB() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ALGO_SWAP(8,9) ALGO_SWAP(10,11) ODD_EVEN_PARALLEL_END() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ALGO_SWAP(9,10) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_12() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB() ODD_EVEN_12_SUB()

#define ODD_EVEN_13_SUBa() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ALGO_SWAP(8,9) ALGO_SWAP(10,11) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_13_SUBb() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ALGO_SWAP(9,10) ALGO_SWAP(11,12) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_13() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa() ODD_EVEN_13_SUBb() ODD_EVEN_13_SUBa()

#define ODD_EVEN_14_SUB() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ALGO_SWAP(8,9) ALGO_SWAP(10,11) ALGO_SWAP(12,13) ODD_EVEN_PARALLEL_END() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ALGO_SWAP(9,10) ALGO_SWAP(11,12) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_14() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB() ODD_EVEN_14_SUB()

#define ODD_EVEN_15_SUBa() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ALGO_SWAP(8,9) ALGO_SWAP(10,11) ALGO_SWAP(12,13) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_15_SUBb() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ALGO_SWAP(9,10) ALGO_SWAP(11,12) ALGO_SWAP(13,14) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_15() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa() ODD_EVEN_15_SUBb() ODD_EVEN_15_SUBa()

#define ODD_EVEN_16_SUB() ODD_EVEN_PARALLEL_START() ALGO_SWAP(0,1) ALGO_SWAP(2,3) ALGO_SWAP(4,5) ALGO_SWAP(6,7) ALGO_SWAP(8,9) ALGO_SWAP(10,11) ALGO_SWAP(12,13) ALGO_SWAP(14,15) ODD_EVEN_PARALLEL_END() ODD_EVEN_PARALLEL_START() ALGO_SWAP(1,2) ALGO_SWAP(3,4) ALGO_SWAP(5,6) ALGO_SWAP(7,8) ALGO_SWAP(9,10) ALGO_SWAP(11,12) ALGO_SWAP(13,14) ODD_EVEN_PARALLEL_END()
#define ODD_EVEN_16() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB() ODD_EVEN_16_SUB()

namespace algo
{
    struct Unstable {} ;
    struct Stable {} ;
    struct Exchange {} ;
    struct ExchangeIndices {} ;
    
    struct UnstableExchange : ALGO_CALL::Unstable, ALGO_CALL::Exchange {} ;
    struct StableExchange : ALGO_CALL::Stable, ALGO_CALL::Exchange {} ;
    
    struct UnstableExchangeIndices : ALGO_CALL::Unstable, ALGO_CALL::ExchangeIndices {} ;
    struct StableExchangeIndices : ALGO_CALL::Stable, ALGO_CALL::ExchangeIndices {} ;
    
    typedef unsigned int DefaultIndexType ;
    
// upper = lower + 1 for the stable sorting
#define ALGO_SWAP(lower, upper) , swap_if ( cmp ( tmp [ upper ], tmp [ lower ] ), tmp [ upper ], tmp [ lower ], SwapIfKind () )
    
#define ALGO_SORT(n) \
template < typename Iter, typename  Cmp, typename SwapIfKind >\
void sortImpl ## n  ( Iter x, Cmp cmp, ALGO_CALL::StableExchange, SwapIfKind )\
{\
    typename ALGO_CALL::IteratorTraits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::MoveAssign > > ( x, n, tmp ) ;\
    ODD_EVEN_ ## n ()\
    ALGO_CALL::stepCounted < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::MoveAssign > > ( tmp, n, x ) ;\
}\
template < typename IndexType, typename Iter, typename Cmp, typename SwapIfKind > \
ALGO_INLINE \
void sort ## n ( Iter x, Cmp cmp, ALGO_CALL::StableExchange, SwapIfKind )\
{\
    ALGO_CALL::sortImpl## n ( x, cmp, ALGO_CALL::StableExchange (), SwapIfKind () ) ;\
}
// Note the above function strips away the unneeded IndexType

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
    
// upper = lower + 1 for the stable sorting
#define ALGO_SWAP(lower, upper) , swap_if ( cmp ( tmp [ indices [ upper ] ], tmp [ indices [ lower ] ] ), indices [ upper ], indices [ lower ], SwapIfKind () )
    
#define ALGO_SORT(n) \
template < typename IndexType, typename Iter, typename Cmp, typename SwapIfKind >\
void sort ## n  ( Iter x, Cmp cmp, ALGO_CALL::StableExchangeIndices, SwapIfKind )\
{\
    typename ALGO_CALL::IteratorTraits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::MoveAssign > > ( x, n, tmp ) ;\
    IndexType indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } ;\
    ODD_EVEN_ ## n ()\
    for ( int i = 0 ; i <  n ; ++i ){ *x = ALGO_CALL::move ( tmp [ indices [ i ] ] ) ; ++x ; }\
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
    
#define ALGO_SWAP(lower, upper) swap_if ( cmp ( tmp [ upper ], tmp [ lower ] ), tmp [ upper ], tmp [ lower ], SwapIfKind () ) ;
    
#define ALGO_SORT(n) \
template < typename Iter, typename Cmp, typename SwapIfKind > \
void sortImpl ## n ( Iter x, Cmp cmp, ALGO_CALL::UnstableExchange, SwapIfKind )\
{\
    typename ALGO_CALL::IteratorTraits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::MoveAssign > > ( x, n, tmp ) ;\
    ALGO_SORT_SORT_ ## n \
    ALGO_CALL::stepCounted < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::MoveAssign > > ( tmp, n, x ) ;\
}\
template < typename IndexType, typename Iter, typename Cmp, typename SwapIfKind > \
ALGO_INLINE \
void sort ## n ( Iter x, Cmp cmp, ALGO_CALL::UnstableExchange, SwapIfKind )\
{\
    ALGO_CALL::sortImpl## n ( x, cmp, ALGO_CALL::UnstableExchange (), SwapIfKind () ) ;\
}
// Note the above function strips away the unneeded IndexType

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
    
#define ALGO_SWAP(lower, upper) swap_if ( cmp ( tmp [ indices [ upper ] ], tmp [ indices [ lower ] ] ), indices [ upper ], indices [ lower ], SwapIfKind () ) ;
    
#define ALGO_SORT(n) \
template < typename IndexType, typename Iter, typename Cmp, typename SwapIfKind > \
void sort ## n ( Iter x, Cmp cmp, ALGO_CALL::UnstableExchangeIndices, SwapIfKind )\
{\
    typename ALGO_CALL::IteratorTraits < Iter> ::value_type tmp [ n ] ;\
    ALGO_CALL::stepCounted < ALGO_CALL::TransferOperatorWrapper < ALGO_CALL::MoveAssign > > ( x, n, tmp ) ;\
    IndexType indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } ;\
    ALGO_SORT_SORT_ ## n \
    for ( int i = 0 ; i < n ; ++i ){ *x = ALGO_CALL::move ( tmp [ indices [ i ] ] ) ; ++x ; }\
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
template < typename Iter, typename Cmp, typename Tag, typename SwapIfKind, typename IndexType >\
ALGO_INLINE \
void sort ( Iter x, Cmp cmp, Tag, Int < n >, SwapIfKind, IndexType )\
{\
    ALGO_CALL::sort ## n < IndexType > ( x, cmp, Tag (), SwapIfKind () ) ;\
}\
template < typename Iter, typename Cmp, typename Tag >\
ALGO_INLINE \
void sort ( Iter x, Cmp cmp, Tag, Int < n > )\
{\
    ALGO_CALL::sort ## n < DefaultIndexType > ( x, cmp, Tag (), ALGO_CALL::Ternary () ) ;\
}
    template < typename Iter, typename Cmp, typename Tag, typename SwapIfKind, typename IndexType >
    ALGO_INLINE
    void sort ( Iter x, Cmp cmp, Tag, Int < 0 >, SwapIfKind, IndexType )
    {}
    
    template < typename Iter, typename Cmp, typename Tag >
    ALGO_INLINE
    void sort ( Iter x, Cmp cmp, Tag, Int < 0 > )
    {}
    
    template < typename Iter, typename Cmp, typename Tag, typename SwapIfKind, typename IndexType >
    ALGO_INLINE
    void sort ( Iter x, Cmp cmp, Tag, Int < 1 >, SwapIfKind, IndexType )
    {}
    
    template < typename Iter, typename Cmp, typename Tag >
    ALGO_INLINE
    void sort ( Iter x, Cmp cmp, Tag, Int < 1 > )
    {}
    
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
    
    template < typename Iter, typename Cmp >
    ALGO_INLINE
    void insertCurrentStable ( Iter f, Iter current, Cmp cmp )
    {
        typename ALGO_CALL::IteratorTraits < Iter >::value_type tmp ( ALGO_CALL::derefMove ( current ) ) ;
        Iter hole = current ;
        Iter prev = ALGO_CALL::predecessor ( current, ALGO_CALL::ByReturnValue () ) ;
        
        while ( cmp ( tmp, ALGO_CALL::deref ( prev ) ) )
        {
            ALGO_CALL::moveAssign ( prev, hole ) ;
            hole = prev ;
            if ( f == prev ) break ;
            ALGO_CALL::predecessor ( prev, ALGO_CALL::InPlace () ) ;
        }
        ALGO_CALL::moveAssign ( &tmp, hole ) ;
    }
    
    template < typename Iter, typename Cmp >
    ALGO_INLINE
    void insertCurrentUnguarded ( Iter current, Cmp cmp )
    {
        typename ALGO_CALL::IteratorTraits < Iter >::value_type tmp ( ALGO_CALL::derefMove ( current ) ) ;
        Iter hole = current ;
        Iter prev = ALGO_CALL::predecessor ( current, ALGO_CALL::ByReturnValue () ) ;
        
        while ( cmp ( tmp, ALGO_CALL::deref ( prev ) ) )
        {
            ALGO_CALL::moveAssign ( prev, hole ) ;
            hole = prev ;
            ALGO_CALL::predecessor ( prev, ALGO_CALL::InPlace () ) ;
        }
        
        ALGO_CALL::moveAssign ( &tmp, hole ) ;
    }
    
    template < typename Iter, typename Cmp >
    ALGO_INLINE
    void sort_insertion_sentinel_impl ( Iter f, Iter l, Cmp cmp )
    {
        if ( f == l ) return ;
        
        Iter current = ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ) ;
        
        while ( current != l )
        {
            ALGO_CALL::insertCurrentUnguarded ( current, cmp ) ;
            
            ALGO_CALL::successor ( current, ALGO_CALL::InPlace () ) ;
        }
    }
    
    template < typename Iter, typename Cmp, typename N >
    ALGO_INLINE
    void sort_insertion_sentinel_impl ( Iter f, N n, Cmp cmp )
    {
        if ( n < N ( 2 ) ) return ;
        
        ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ) ;
        --n ;
        
        while ( n )
        {
            ALGO_CALL::insertCurrentUnguarded ( f, cmp ) ;
            
            ALGO_CALL::successor ( f, ALGO_CALL::InPlace () ),
            --n;
        }
    }
    
    
    
    template < typename Iter, typename Cmp, typename N >
    ALGO_INLINE
    void sort_insertion ( Iter f, N n, Cmp cmp )
    {
        if ( n < N ( 2 ) ) return ;
        
        Iter current = ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ) ;
        --n ;
        
        while ( n )
        {
            ALGO_CALL::insertCurrentStable ( f, current, cmp ) ;
            
            ALGO_CALL::successor ( current, ALGO_CALL::InPlace () ),
            --n ;
        }
    }
    
    template < typename Iter, typename Cmp >
    ALGO_INLINE
    void sort_insertion ( Iter f, Iter l, Cmp cmp )
    {
        if ( f == l ) return ;
        
        Iter current = ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ) ;

        while ( current != l )
        {
            ALGO_CALL::insertCurrentStable ( f, current, cmp ) ;

            ALGO_CALL::successor ( current, ALGO_CALL::InPlace () ) ;
        }
    }
    
    
    template < typename Iter, typename Cmp, typename N >
    ALGO_INLINE
    void sort_insertion_sentinel ( Iter f, N n, Cmp cmp )
    {
        ALGO_CALL::rotateRightByOne ( f, ALGO_CALL::minIter ( f, n, cmp ), ALGO_CALL::RotateNoChoice () ) ;
        ALGO_CALL::sort_insertion_sentinel_impl ( ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ), n - 1, cmp ) ;
    }
    
    template < typename Iter, typename Cmp >
    ALGO_INLINE
    void sort_insertion_sentinel ( Iter f, Iter l, Cmp cmp )
    {
        ALGO_CALL::rotateRightByOne ( f, ALGO_CALL::minIter ( f, l, cmp ), ALGO_CALL::RotateNoChoice () ) ;
        ALGO_CALL::sort_insertion_sentinel_impl ( ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ), l, cmp ) ;
    }
    
    
    template < typename Iter, typename Cmp, typename N >
    ALGO_INLINE
    void sort_insertion_sentinel_unstable ( Iter f, N n, Cmp cmp )
    {
        ALGO_CALL::iterSwap ( f, ALGO_CALL::minIter ( f, n, cmp ) ) ;
        ALGO_CALL::sort_insertion_sentinel_impl ( ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ), n - 1, cmp ) ;
    }
    
    template < typename Iter, typename Cmp >
    ALGO_INLINE
    void sort_insertion_sentinel_unstable ( Iter f, Iter l, Cmp cmp )
    {
        ALGO_CALL::iterSwap ( f, ALGO_CALL::minIter ( f, l, cmp ) ) ;
        ALGO_CALL::sort_insertion_sentinel_impl ( ALGO_CALL::successor ( f, ALGO_CALL::ByReturnValue () ), l, cmp ) ;
    }

} // namespace algo

#endif
