#ifndef _SOFTWARE_GRAPH_HPP
#define _SOFTWARE_GRAPH_HPP

#include "Conceptgraph.hpp"

namespace Software {

/*
    Some notes:
    The underlying fundament is the concept graph.
    In this environment there are two main entities, concepts & relations between them.
    This class is derived from the concept graph class.

    It introduces these different concepts:
        ALGORITHM, INPUT, OUTPUT, INTERFACE (for logical specification and skeleton generation)
        IMPLEMENTATION, DATATYPE, LANGUAGE (for storing already available implementations of algorithms in different languages)
    The domain is encoded as follows:
    
    ALGORITHM -- has --> INTERFACE
    INPUT -- is-a --> INTERFACE
    OUTPUT -- is-a --> INTERFACE
    ALGORITHM -- needs --> INPUT
    ALGORITHM -- provides --> OUTPUT
    INPUT -- dependsOn --> OUTPUT

    ALGORITHM <-- realizes -- IMPLEMENTATION -- expressedIn --> LANGUAGE
    INTERFACE <-- represents -- DATATYPE -- expressedIn --> LANGUAGE
    (IMPLEMENTATION -- uses --> DATATYPE)               questionable
    (IMPLEMENTATION -- dependsOn --> IMPLEMENTATION)    optional, not needed yet
    (LANGUAGE -- has --> COMPILER)                      optional, not needed yet
    (IMPLEMENTATION -- needs --> INPUT)                 redundant
    (IMPLEMENTATION -- provides --> OUTPUT)             redundant

    If some X is a ALGORITHM then there exists a path of IS-A relations from X to ALGORITHM

    Some example:
    
        |---------- realizes -- disparity.c -- expressedIn --> C
        v
    DisparityMap -- needs --> left -- is-a --> Input
      | |                       |---- is-a --> Image <-- represents -- uint8_t[MAX_X][MAX_Y] -- expressedIn --> C
      | |---------- needs --> right ...
      |------------ provides --> disparity ...

    NOTE: When merging with the concept of Finite State Machines, this whole thing would become an even whiter box :)
*/

class Graph : public Conceptgraph
{
    public:
        // Identifiers for the main concepts
        static const unsigned AlgorithmId;
        static const unsigned InterfaceId;
        static const unsigned InputId;
        static const unsigned OutputId;
        static const unsigned ImplementationId;
        static const unsigned LanguageId;
        static const unsigned DatatypeId;

        // Ids for identifiing main relations
        static const unsigned IsAId;
        static const unsigned HasAId;
        static const unsigned NeedsId;
        static const unsigned ProvidesId;
        static const unsigned DependsOnId;
        static const unsigned ExpressedInId;
        static const unsigned RealizesId;
        static const unsigned RepresentsId;
        //static const unsigned UsesId;

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
        unsigned createImplementation(const std::string& name="Implementation");
        unsigned createDatatype(const std::string& name="DataType");
        unsigned createLanguage(const std::string& name="Language");

        // Queries
        Hyperedges algorithms(const std::string& name="");
        Hyperedges interfaces(const std::string& name="");
        Hyperedges inputs(const std::string& name="");
        Hyperedges outputs(const std::string& name="");
        Hyperedges implementations(const std::string& name="");
        Hyperedges datatypes(const std::string& name="");
        Hyperedges languages(const std::string& name="");
    
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
        // Algorithm & Implementation
        unsigned realizes(const unsigned implementationId, const unsigned algorithmId);
        // Interface & Datatype
        unsigned represents(const unsigned datatypeId, const unsigned interfaceId);
        // Implementation/Datatype & Language
        unsigned expressedIn(const unsigned implementationOrDatatypeId, const unsigned languageId);
        // Implementation & Datatype
        //unsigned uses(const unsigned implementationId, const unsigned datatypeId);
};

}

#endif
