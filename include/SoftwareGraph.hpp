#ifndef _SOFTWARE_GRAPH_HPP
#define _SOFTWARE_GRAPH_HPP

#include "SetSystem.hpp"
#include "Set.hpp"
#include "Relation.hpp"

namespace Software {

/*
    Some notes:
    We introduce some SUPERCLASSES as C++ classes AS WELL.
    But under the hood a IS-A relationship ensures that e.g. every subdevice is a device.
    
    Open questions:
    * Does it makes sense to have 1-1, 1-N, N-1 and N-M versions of the relations?
    * Does it makes sense to use (N*1-1) and (N*1-M) versions of the N-1,N-M relations?
    * Would it be better to just use SIMPLE 2-hyperedges for the relations (everything based on 1-1 relations)?
      NOTE: This would better fit to databases with fixed tables like ID -> (ID, ID)
    * How are the constraints on AGGREGATION (has)? Or, can an interface be owned by only one device?
    * Should we introduce intermediate concepts like Ownership(has) or others which are shared amongst other domains?
*/

class Graph;

class Type: public Set
{
    public:
        /* Constructor */
        Type(const unsigned id, const std::string& label="")
        : Set(id,label)
        {}

        // This is holding a label to identify instances of this class via (* -- isA --> superclassLabel) patterns
        static const std::string superclassLabel;
};

class Interface : public Set
{
    public:
        /* Constructor */
        Interface(const unsigned id, const std::string& label="")
        : Set(id,label)
        {}

        // This is holding a label to identify instances of this class via (* -- isA --> superclassLabel) patterns
        static const std::string superclassLabel;
};

class Input : public Interface
{
    public:
        /* Constructor */
        Input(const unsigned id, const std::string& label="")
        : Interface(id,label)
        {}

        // This is holding a label to identify instances of this class via (* -- isA --> superclassLabel) patterns
        static const std::string superclassLabel;

        // Inputs & Types
        unsigned needs(Graph* graph, const unsigned typeId);
        // Inputs & Outputs
        unsigned depends(Graph* graph, const unsigned outputId);
};

class Output : public Interface
{
    public:
        /* Constructor */
        Output(const unsigned id, const std::string& label="")
        : Interface(id,label)
        {}

        // This is holding a label to identify instances of this class via (* -- isA --> superclassLabel) patterns
        static const std::string superclassLabel;

        // Outputs & Types
        unsigned provides(Graph* graph, const unsigned typeId);

};

class Algorithm : public Set
{
    public:
        /* Constructor */
        Algorithm(const unsigned id, const std::string& label="")
        : Set(id,label)
        {}

        // This is holding a label to identify instances of this class via (* -- isA --> superclassLabel) patterns
        static const std::string superclassLabel;

        // Algorithms & Interfaces
        unsigned has(Graph* graph, const unsigned interfaceId);
};


/*
    CONSISTENCY CHECKS
    - Only one type / input,output,parameter
    - Either having parameters OR bidirectional interfaces (but not both?)
    - Types should be trees but not DAGs (?)
    - A dependency between input and output implies that their types match/are compatible
*/
class Graph : public SetSystem
{
    public:
        // Constructor/Destructor
        Graph();
        ~Graph();

        // Returns the id of an representative of the classes
        unsigned algorithmClass();
        unsigned interfaceClass();
        unsigned inputClass();
        unsigned outputClass();
        unsigned typeClass();

        // Factory functions
        unsigned createAlgorithm(const std::string& name="Algorithm");
        unsigned createInput(const std::string& name="Input");
        unsigned createOutput(const std::string& name="Output");
        unsigned createType(const std::string& name="Type");

        // Queries
        // NOTE: Return true sets whose members are all of the same type/superclass
        unsigned algorithms();
        unsigned interfaces();
        unsigned inputs();
        unsigned outputs();
        unsigned types();
    
        // Algorithms & I/O/P
        unsigned has(const unsigned algorithmId, const unsigned interfaceId);

        // I/O & Type
        unsigned provides(const unsigned outputId, const unsigned typeId);
        unsigned needs(const unsigned inputId, const unsigned typeId);

        // I/O & Dependencies
        unsigned depends(const unsigned inputId, const unsigned outputId);
};

}

#endif
