#ifndef _SOFTWARE_GRAPH_HPP
#define _SOFTWARE_GRAPH_HPP

#include "CommonConceptGraph.hpp"

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

    (ALGORITHM -- has --> INTERFACE)                    optional, inferrable
    INPUT -- is-a --> INTERFACE
    OUTPUT -- is-a --> INTERFACE
    ALGORITHM -- needs(has) --> INPUT
    ALGORITHM -- provides(has) --> OUTPUT
    INPUT -- dependsOn(connects) --> OUTPUT

    ALGORITHM <-- realizes(is-a) -- IMPLEMENTATION -- expressedIn(partOf) --> LANGUAGE
    INTERFACE <-- represents(is-a) -- DATATYPE -- expressedIn(partOf) --> LANGUAGE

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

class Graph : public CommonConceptGraph
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

        // Constructor/Destructor
        Graph();
        Graph(CommonConceptGraph& A);
        ~Graph();

        // Generates the dictionary
        void createMainConcepts();

        // Factory functions
        // NOTE: Create classes
        Hyperedges createAlgorithm(const std::string& name="Algorithm");
        Hyperedges createInterface(const std::string& name="Interface");
        Hyperedges createInput(const unsigned interfaceId, const std::string& name="Input"); // TODO: Needed?
        Hyperedges createOutput(const unsigned interfaceId, const std::string& name="Output"); // TODO: Needed?
        Hyperedges createInput(const Hyperedges& interfaceIds, const std::string& name="Input"); // TODO: Needed?
        Hyperedges createOutput(const Hyperedges& interfaceIds, const std::string& name="Output"); // TODO: Needed?
        Hyperedges createImplementation(const std::string& name="Implementation"); // TODO: Needed?
        Hyperedges createDatatype(const std::string& name="DataType");
        Hyperedges createLanguage(const std::string& name="Language"); // TODO: Needed?
        // NOTE: Create instances
        //Hyperedges instantiateAlgorithm(const unsigned superId, const std::string& name="");
        Hyperedges instantiateInput(const unsigned superId, const std::string& name="");
        Hyperedges instantiateOutput(const unsigned superId, const std::string& name="");
        Hyperedges instantiateInput(const Hyperedges& superId, const std::string& name="");
        Hyperedges instantiateOutput(const Hyperedges& superId, const std::string& name="");

        // Queries
        // NOTE: Returns subclasses
        Hyperedges algorithmClasses(const std::string& name="");
        Hyperedges interfaceClasses(const std::string& name="");
        Hyperedges inputClasses(const std::string& name="");
        Hyperedges outputClasses(const std::string& name="");
        Hyperedges implementationClasses(const std::string& name="");
        Hyperedges datatypeClasses(const std::string& name="");
        Hyperedges languageClasses(const std::string& name="");
        // NOTE: Returns instances
        Hyperedges algorithms(const std::string& name="", const std::string& className="");
        Hyperedges interfaces(const std::string& name="", const std::string& className="");
        Hyperedges inputs(const std::string& name="", const std::string& className="");
        Hyperedges outputs(const std::string& name="", const std::string& className="");
        Hyperedges implementations(const std::string& name="", const std::string& className="");
        Hyperedges datatypes(const std::string& name="", const std::string& className="");
        Hyperedges languages(const std::string& name="", const std::string& className="");

        // Facts
        // NOTE: Only the multidimensionals are used here (more generic)
        // Algorithms & I/O/P
        // RULE: A has X -> X is-a Interface
        // RULE: A provides O -> A has O, O is-a Output (, O is-a Interface)
        // RULE: A needs I -> A has I, I is-a Input (, I is-a Interface)
        Hyperedges has(const Hyperedges& algorithmIds, const Hyperedges& interfaceIds);
        Hyperedges provides(const Hyperedges& algorithmIds, const Hyperedges& outputIds);
        Hyperedges needs(const Hyperedges& algorithmIds, const Hyperedges& inputIds);
        // I/O & Dependencies
        // RULE: I dependsOn O -> I is-a Input, O is-a Output
        Hyperedges depends(const Hyperedges& inputIds, const Hyperedges& outputIds);
        Hyperedges realizes(const Hyperedges& implementationIds, const Hyperedges& algorithmIds);
        Hyperedges represents(const Hyperedges& datatypeIds, const Hyperedges& interfaceIds);
        Hyperedges expressedIn(const Hyperedges& implementationOrDatatypeIds, const Hyperedges& languageIds);
};

}

#endif
