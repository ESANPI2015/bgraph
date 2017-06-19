#ifndef _SOFTWARE_GRAPH_HPP
#define _SOFTWARE_GRAPH_HPP

#include "Conceptgraph.hpp"

namespace Software {

/*
    Some notes:
    The underlying fundament is the concept graph.
    In this environment there are two main entities, concepts & relations between them.
    This class is derived from the concept graph class.

    It introduces these different concepts: ALGORITHM, INPUT, OUTPUT, INTERFACE
    The domain is encoded as follows:
    
    ALGORITHM -- has --> INTERFACE
    INPUT <-- is-a --> INTERFACE
    OUTPUT <-- is-a --> INTERFACE
    ALGORITHM <-- needs --> INPUT
    ALGORITHM <-- provides --> OUTPUT
    INPUT <-- dependsOn --> OUTPUT

    If some X is a ALGORITHM then there exists a path of IS-A relations from X to ALGORITHM
    
    Open questions:
    * Does it makes sense to have 1-1, 1-N, N-1 and N-M versions of the relations?
    * Does it makes sense to use (N*1-1) and (N*1-M) versions of the N-1,N-M relations?
    * Would it be better to just use SIMPLE 2-hyperedges for the relations (everything based on 1-1 relations)?
      NOTE: This would better fit to databases with fixed tables like ID -> (ID, ID)
    * How are the constraints on AGGREGATION (has)? Or, can an interface be owned by only one device?
    * Should we introduce intermediate concepts like Ownership(has) or others which are shared amongst other domains?
*/

class Graph;

/*
    CONSISTENCY CHECKS
    - Only one type / input,output,parameter
    - Either having parameters OR bidirectional interfaces (but not both?)
    - Types should be trees but not DAGs (?)
    - A dependency between input and output implies that their types match/are compatible
*/
class Graph : public Conceptgraph
{
    public:
        // Labels for identifiing the main concepts
        static const std::string AlgorithmLabel;
        static const std::string InterfaceLabel;
        static const std::string InputLabel;
        static const std::string OutputLabel;

        // Constructor/Destructor
        Graph();
        Graph(Conceptgraph& A);
        ~Graph();

        // Factory functions
        unsigned createAlgorithm(const std::string& name="Algorithm");
        unsigned createInterface(const std::string& name="Interface");
        unsigned createInput(const std::string& name="Input");
        unsigned createOutput(const std::string& name="Output");

        // Queries
        // NOTE: Return true sets whose members are all of the same type/superclass
        Hyperedges algorithms(const std::string& name="");
        Hyperedges interfaces(const std::string& name="");
        Hyperedges inputs(const std::string& name="");
        Hyperedges outputs(const std::string& name="");
    
        // Algorithms & I/O/P
        // RULE: A has X -> X is-a Interface
        // RULE: A provides O -> A has O, O is-a Output (, O is-a Interface)
        // RULE: A needs I -> A has I, I is-a Input (, I is-a Interface)
        unsigned has(const unsigned algorithmId, const unsigned interfaceId);
        unsigned provides(const unsigned algorithmId, const unsigned outputId);
        unsigned needs(const unsigned algorithmId, const unsigned inputId);

        // I/O & Dependencies
        // RULE: I dependsOn O -> I is-a Input, O is-a Output
        unsigned depends(const unsigned inputId, const unsigned outputId);

    protected:
        // Returns the id of an representative of the classes
        unsigned algorithmConcept();
        unsigned interfaceConcept();
        unsigned inputConcept();
        unsigned outputConcept();

        // The sets holding the different concepts of the domain
        Hyperedges _algorithms;
        Hyperedges _interfaces;
        Hyperedges _inputs;
        Hyperedges _outputs;
};

}

#endif
