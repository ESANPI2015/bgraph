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
        // Identifiers for the main concepts
        static const unsigned AlgorithmId;
        static const unsigned InterfaceId;
        static const unsigned InputId;
        static const unsigned OutputId;
        // Ids for identifiing IsA, HasA and Connects
        static const unsigned IsAId;
        static const unsigned HasAId;
        static const unsigned NeedsId;
        static const unsigned ProvidesId;
        static const unsigned DependsOnId;

        // Constructor/Destructor
        Graph();
        Graph(Conceptgraph& A);
        ~Graph();

        // Generates the dictionary
        void createMainConcepts();

        // Factory functions
        unsigned createAlgorithm(const std::string& name="Algorithm");
        unsigned createInterface(const std::string& name="Interface");
        unsigned createInput(const unsigned interfaceId, const std::string& name="Input");
        unsigned createOutput(const unsigned interfaceId, const std::string& name="Output");

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
};

}

#endif
