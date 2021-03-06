cpp-ranges
==========

Aim is to create an workable implementation of the Range "concept", dealing with unbounded, bounded and counted ranges.

Elements of Programming (EoP, http://www.elementsofprogramming.com/ ) by Alexander Stepanov & Paul McJones treats ranges as implicit, in a multitude of similarly named functions. It feels like fighting a combinatorial problem with copy-paste.

The approach here is to :-

1) algo_iterator.h Encapsulate fundamental iterator operations (e.g. predecessor, successor, dereference, advance, distance). into templates that can be specialised by ranges.

2) algo_property.h Create a PropertySet that maps from a tag to a value, allowing for easy extension and compile-time reflection.

3) algo_range.h Create Ranges on the top of PropertySet, and inject range-specific versions of fundamental operations into the framework created in 1)

4) algo.h (?) Create algorithms that use the framework from 1) to express EoP more clearly.

5) (TDB?) Finally specialise the generic operations for pointers and values with special properties (bit-wise copyable, bit-wise moveable, trivially destructible etc).

Note that algo_sort.h is an experiment with attempting to use CPU level parallelism to speed up sorting of short sequences, using unstable sorting networks ( http://en.wikipedia.org/wiki/Sorting_network ) as well as stable odd/even sorting ( http://en.wikipedia.org/wiki/Odd%E2%80%93even_sort ).

Currently building this only on XCode 5.1.1