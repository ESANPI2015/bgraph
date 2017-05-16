#ifndef _SOFTWARE_GRAPH_HPP
#define _SOFTWARE_GRAPH_HPP

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

class Type: public Set
{
    public:
        // Gives a ptr to the type superclass
        static Set* Superclass();
        
    private:
        // The classLabel is a string representing the superclass
        static const std::string classLabel;
        static unsigned lastSuperclassId;
};
class Interface : public Set
{
    public:
        // bool has(Set* type);

        // Gives a ptr to the interface superclass
        static Set* Superclass();
        
    private:
        // The classLabel is a string representing the superclass
        static const std::string classLabel;
        static unsigned lastSuperclassId;
};
class Input : public Interface
{
    public:
        // Inputs & Types
        bool needs(Set* type);
        // Inputs & Outputs
        bool depends(Set* output);

        // Gives a ptr to the input superclass
        static Set* Superclass();
        
    private:
        // The classLabel is a string representing the superclass
        static const std::string classLabel;
        static unsigned lastSuperclassId;
};
class Output : public Interface
{
    public:
        // Outputs & Types
        bool provides(Set* type);

        // Gives a ptr to the output superclass
        static Set* Superclass();
        
    private:
        // The classLabel is a string representing the superclass
        static const std::string classLabel;
        static unsigned lastSuperclassId;
};
class Algorithm : public Set
{
    public:
        // Algorithms & Interfaces
        bool has(Set* interface);
        bool has(Set::Sets interfaces);

        // Queries:
        // Returns a set containing all things which are related to us by a "has" relation
        Set* aggregates();

        // Gives a ptr to the interface superclass
        static Set* Superclass();

    private:
        // The classLabel is a string representing the superclass
        static const std::string classLabel;
        static unsigned lastSuperclassId;
};


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
        // Factory functions
        static Graph* create(const std::string& label="Software");
        Algorithm* createAlgorithm(const std::string& name="Algorithm");
        Input* createInput(const std::string& name="Input");
        Output* createOutput(const std::string& name="Output");
        Type* createType(const std::string& name="Type");

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
};

}

#endif
