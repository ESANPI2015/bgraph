# Domain Specific Component Network: Behavior Graph

This library serves as an interface to the behavior graph domain.
In this domain, the algorithms & implementations mostly are pure functional.
The model knows 3 entites:
* Computation functions
* Merge functions
* Edge functions

Merge functions are of variing arity and serve as gates to the inputs of the computation functions.
The edge functions connect the outputs of computation functions with the inputs of merge functions (and therefore inputs of other computation functions).
