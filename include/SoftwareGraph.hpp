#ifndef _SOFTWARE_GRAPH_HPP
#define _SOFTWARE_GRAPH_HPP

#include "Hyperedge.hpp"

namespace Software {

/*
    CONSISTENCY CHECKS
    - Only one type / input,output,parameter
    - Either having parameters OR bidirectional interfaces (but not both?)
    - Types should be trees but not DAGs (?)
    - A dependency between input and output implies that their types match/are compatible
*/
class Graph : public Hyperedge
{
    public:
        // Constructor which creates a Hyperedge and all standard subsets
        // Subsets: Algorithms, Interfaces: Inputs/Outputs/Parameters, Types
        Graph(const std::string& label="Software");

        // Factory functions
        Hyperedge* createAlgorithm(const std::string& name="Algorithm");
        Hyperedge* createInput(const std::string& name="Input");
        Hyperedge* createOutput(const std::string& name="Output");
        Hyperedge* createParameter(const std::string& name="Parameter"); // TODO: A parameter is an OPTIONAL input ... is it needed?
        Hyperedge* createType(const std::string& name="Type");

        // Read access
        Hyperedge *algorithms();
        Hyperedge *interfaces();
        Hyperedge *inputs();
        Hyperedge *outputs();
        Hyperedge *parameters();
        Hyperedge *types();
    
        // Algorithms & I/O/P
        bool has(Hyperedge* algorithm, Hyperedge* interface);
        bool has(Hyperedge* algorithm, Hyperedge::Hyperedges interfaces);
        // The following functions are allowed because you want to have ALGORITHM CLASSES using the same INTERFACE CLASSES.
        // TODO: However, a constraint should be that all 0-hyperedge algorithms must have their own interfaces
        bool has(Hyperedge::Hyperedges algorithms, Hyperedge* interface);
        bool has(Hyperedge::Hyperedges algorithms, Hyperedge::Hyperedges interfaces);

        // I/O & Type
        // TODO: Can't we use a TYPE INSTANCE to encode a connection? Hmmm, or would this be weird/inconsistent?
        // TODO: we could also encode this: x in Inputs in Types && x in T in Types ... so input is just another type ...
        // But then we have to support multiple types (see below)
        // TODO: Multiple types make only sense IFF their composition is always the same (e.g. UNION, RECORD or such)
        bool provides(Hyperedge* output, Hyperedge* type);
        bool provides(Hyperedge::Hyperedges outputs, Hyperedge* type);
        bool needs(Hyperedge* input, Hyperedge* type);
        bool needs(Hyperedge::Hyperedges inputs, Hyperedge* type);

        // I/O & Dependencies
        // TODO: Multiple outputs to one input make only sense IFF their types do not overlap! But maybe its something nice to do?
        // TODO: How do we prevent dependencies between parameters?
        bool depends(Hyperedge* input, Hyperedge* output);
        bool depends(Hyperedge::Hyperedges inputs, Hyperedge* output);
    private:
        unsigned _algId, _ifId, _inpId, _outpId, _paramId, _typeId;
};

}

#endif
