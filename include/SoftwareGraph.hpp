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
    * Subclassing versus Subgraphs: Specify this. How do we handle devices containing other devices?
      -> SEE SUBSUMPTION AND COMPOSITION RELATIONS
    * Should we introduce intermediate concepts like Ownership(has) or others which are shared amongst other domains?
*/

class Type: public Set
{
    public:
        // Constructors
        Type(const std::string& label="Type");

        // Gives a ptr to the type superclass
        static Set* Superclass();
        
        // Promotes a Set to be an Type
        // This means that it will get an isA relation to the interface superclass!!!
        //static Type* promote(Set *set);

    private:
        static Set* superclass;
};
class Interface : public Set
{
    public:
        // Constructors
        Interface(const std::string& label="Interface");

        // bool has(Set* type);

        // Gives a ptr to the interface superclass
        static Set* Superclass();
        
        // Promotes a Set to be an Interface
        // This means that it will get an isA relation to the interface superclass!!!
        //static Interface* promote(Set *set);

    private:
        static Set* superclass;
};
class Input : public Interface
{
    public:
        // Constructors
        Input(const std::string& label="Input");

        // Inputs & Types
        bool needs(Set* type);
        // Inputs & Outputs
        bool depends(Set* output);

        // Gives a ptr to the input superclass
        static Set* Superclass();
        
        // Promotes a Set to be an Input
        // This means that it will get an isA relation to the interface superclass!!!
        //static Input* promote(Set *set);

    private:
        static Set* superclass;
};
class Output : public Interface
{
    public:
        // Constructors
        Output(const std::string& label="Output");

        // Outputs & Types
        bool provides(Set* type);

        // Gives a ptr to the output superclass
        static Set* Superclass();
        
        // Promotes a Set to be an Output
        // This means that it will get an isA relation to the interface superclass!!!
        //static Output* promote(Set *set);

    private:
        static Set* superclass;
};
class Algorithm : public Set
{
    public:
        // Constructors
        Algorithm(const std::string& label="Algorithm");

        // Algorithms & Interfaces
        bool has(Set* interface);
        bool has(Set::Sets interfaces);

        // Queries:
        // Returns a set containing all things which are related to us by a "has" relation
        Set* aggregates();

        // Gives a ptr to the interface superclass
        static Set* Superclass();

        // Promotes a Set to be a Algorithm
        //static Bus* promote(Set *set);

    private:
        static Set* superclass;
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
        // Constructor which creates a Set and all standard subsets
        // Subsets: Algorithms, Interfaces: Inputs/Outputs, Types
        Graph(const std::string& label="Software");

        // Factory functions
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
