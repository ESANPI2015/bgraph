#ifndef _SOFTWARE_GRAPH_HPP
#define _SOFTWARE_GRAPH_HPP

#include "Set.hpp"
#include "Relation.hpp"

namespace Software {

/*
    CONSISTENCY CHECKS
    - Only one type / input,output,parameter
    - Either having parameters OR bidirectional interfaces (but not both?)
    - Types should be trees but not DAGs (?)
    - A dependency between input and output implies that their types match/are compatible
*/
class Graph : public Set
{
    public:
        // Constructor which creates a Set and all standard subsets
        // Subsets: Algorithms, Interfaces: Inputs/Outputs, Types
        Graph(const std::string& label="Software");

        // Factory functions
        Set* createAlgorithm(const std::string& name="Algorithm");
        Set* createInput(const std::string& name="Input");
        Set* createOutput(const std::string& name="Output");
        Set* createType(const std::string& name="Type");

        // Read access
        // NOTE: Return the Superclasses
        Set* algorithm();
        Set* interface();
        Set* input();
        Set* output();
        Set* type();

        // Queries
        // NOTE: Return true sets whose members are all of the same type/superclass
        Set *algorithms();
        Set *interfaces();
        Set *inputs();
        Set *outputs();
        Set *types();
    
        // Algorithms & I/O/P
        bool has(Set* algorithm, Set* interface);
        bool has(Set* algorithm, Set::Sets interfaces);
        // The following functions are allowed because you want to have ALGORITHM CLASSES using the same INTERFACE CLASSES.
        bool has(Set::Sets algorithms, Set* interface);
        bool has(Set::Sets algorithms, Set::Sets interfaces);

        // I/O & Type
        bool provides(Set* output, Set* type);
        bool provides(Set::Sets outputs, Set* type);
        bool needs(Set* input, Set* type);
        bool needs(Set::Sets inputs, Set* type);

        // I/O & Dependencies
        // TODO: Multiple outputs to one input make only sense IFF their types do not overlap! But maybe its something nice to do?
        bool depends(Set* input, Set* output);
        bool depends(Set::Sets inputs, Set* output);
    private:
        unsigned _algId, _ifId, _inpId, _outpId, _typeId;
};

}

#endif
