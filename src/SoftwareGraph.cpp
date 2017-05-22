#include "SoftwareGraph.hpp"

namespace Software {


// Type
const std::string Type::superclassLabel = "Type";

// Interface
const std::string Interface::superclassLabel = "SoftwareInterface";

// Input
const std::string Input::superclassLabel = "Input";

unsigned Input::needs(Graph* graph, const unsigned typeId)
{
    return graph->needs(id(), typeId);
}

unsigned Input::depends(Graph* graph, const unsigned outputId)
{
    return graph->needs(id(), outputId);
}

// Output
const std::string Output::superclassLabel = "Output";

unsigned Output::provides(Graph* graph, const unsigned typeId)
{
    return graph->needs(id(), typeId);
}

// Algorithm
const std::string Algorithm::superclassLabel = "Algorithm";

// Algorithms & Interfaces
unsigned Algorithm::has(Graph* graph, const unsigned interfaceId)
{
    return graph->has(id(), interfaceId);
}

// GRAPH STUFF

Graph::Graph()
{
}

Graph::~Graph()
{
}

unsigned Graph::algorithmClass()
{
    return getClass(Algorithm::superclassLabel);
}

unsigned Graph::interfaceClass()
{
    return getClass(Interface::superclassLabel);
}

unsigned Graph::inputClass()
{
    auto superId = getClass(Input::superclassLabel);
    relateTo(superId, interfaceClass(), Relation::isALabel);
    return superId;
}

unsigned Graph::outputClass()
{
    auto superId = getClass(Output::superclassLabel);
    relateTo(superId, interfaceClass(), Relation::isALabel);
    return superId;
}

unsigned Graph::typeClass()
{
    return getClass(Type::superclassLabel);
}

unsigned Graph::algorithms()
{
    // FIXME: This is at least problematic. If we do not make sure, that we only have one representative, then we might produce inconsistent queries here
    auto superSets = find(Algorithm::superclassLabel);
    unsigned relId = Hypergraph::create("DUMMY RELATION");
    for (auto superId : superSets)
    {
        // For each of the superSets we have to get the instances
        // and merge the results
        auto otherId = superclassOf(superId); // Reads: superId -- superclassOf --> ?
        auto nextId  = Hypergraph::unite(relId, otherId);
        Hypergraph::destroy(relId);
        relId = nextId;
    }
    auto members = Hypergraph::get(relId)->pointingTo();
    Hypergraph::destroy(relId);
    return create(members, "Algorithms");
}

unsigned Graph::interfaces()
{
    // FIXME: This is at least problematic. If we do not make sure, that we only have one representative, then we might produce inconsistent queries here
    auto superSets = find(Interface::superclassLabel);
    unsigned relId = Hypergraph::create("DUMMY RELATION");
    for (auto superId : superSets)
    {
        // For each of the superSets we have to get the instances
        // and merge the results
        auto otherId = superclassOf(superId); // Reads: superId -- superclassOf --> ?
        auto nextId  = Hypergraph::unite(relId, otherId);
        Hypergraph::destroy(relId);
        relId = nextId;
    }
    auto members = Hypergraph::get(relId)->pointingTo();
    Hypergraph::destroy(relId);
    return create(members, "Interfaces");
}

unsigned Graph::inputs()
{
    // FIXME: This is at least problematic. If we do not make sure, that we only have one representative, then we might produce inconsistent queries here
    auto superSets = find(Input::superclassLabel);
    unsigned relId = Hypergraph::create("DUMMY RELATION");
    for (auto superId : superSets)
    {
        // For each of the superSets we have to get the instances
        // and merge the results
        auto otherId = superclassOf(superId); // Reads: superId -- superclassOf --> ?
        auto nextId  = Hypergraph::unite(relId, otherId);
        Hypergraph::destroy(relId);
        relId = nextId;
    }
    auto members = Hypergraph::get(relId)->pointingTo();
    Hypergraph::destroy(relId);
    return create(members, "Inputs");
}

unsigned Graph::outputs()
{
    // FIXME: This is at least problematic. If we do not make sure, that we only have one representative, then we might produce inconsistent queries here
    auto superSets = find(Output::superclassLabel);
    unsigned relId = Hypergraph::create("DUMMY RELATION");
    for (auto superId : superSets)
    {
        // For each of the superSets we have to get the instances
        // and merge the results
        auto otherId = superclassOf(superId); // Reads: superId -- superclassOf --> ?
        auto nextId  = Hypergraph::unite(relId, otherId);
        Hypergraph::destroy(relId);
        relId = nextId;
    }
    auto members = Hypergraph::get(relId)->pointingTo();
    Hypergraph::destroy(relId);
    return create(members, "Outputs");
}

unsigned Graph::types()
{
    // FIXME: This is at least problematic. If we do not make sure, that we only have one representative, then we might produce inconsistent queries here
    auto superSets = find(Type::superclassLabel);
    unsigned relId = Hypergraph::create("DUMMY RELATION");
    for (auto superId : superSets)
    {
        // For each of the superSets we have to get the instances
        // and merge the results
        auto otherId = superclassOf(superId); // Reads: superId -- superclassOf --> ?
        auto nextId  = Hypergraph::unite(relId, otherId);
        Hypergraph::destroy(relId);
        relId = nextId;
    }
    auto members = Hypergraph::get(relId)->pointingTo();
    Hypergraph::destroy(relId);
    return create(members, "Types");
}

unsigned Graph::createAlgorithm(const std::string& name)
{
    auto id = create(name);
    relateTo(id, algorithmClass(), Relation::isALabel);
    return id;
}

unsigned Graph::createInput(const std::string& name)
{
    auto id = create(name);
    relateTo(id, inputClass(), Relation::isALabel);
    return id;
}

unsigned Graph::createOutput(const std::string& name)
{
    auto id = create(name);
    relateTo(id, outputClass(), Relation::isALabel);
    return id;
}

unsigned Graph::createType(const std::string& name)
{
    auto id = create(name);
    relateTo(id, typeClass(), Relation::isALabel);
    return id;
}

unsigned Graph::has(const unsigned algorithmId, const unsigned interfaceId)
{
    // Either we
    // * check if algorithmId -- isA --> algorithm
    // * or, we imply algorithmId -- isA --> algorithm
    // For now, we follow the second approach (although this ALWAYS creates a new relation!!!)
    relateTo(algorithmId, algorithmClass(), Relation::isALabel);
    relateTo(interfaceId, interfaceClass(), Relation::isALabel);
    return relateTo(algorithmId, interfaceId, Relation::hasLabel);
}

unsigned Graph::provides(const unsigned outputId, const unsigned typeId)
{
    // Either we
    // * check if outputId -- isA --> output
    // * or, we imply outputId -- isA --> output
    // For now, we follow the second approach (although this ALWAYS creates a new relation!!!)
    relateTo(outputId, outputClass(), Relation::isALabel);
    relateTo(typeId, typeClass(), Relation::isALabel);
    return relateTo(outputId, typeId, "provides"); // TODO: This should be registered somewhere...Actually it is some form of isA, right?
}

unsigned Graph::needs(const unsigned inputId, const unsigned typeId)
{
    // Either we
    // * check if inputId -- isA --> input
    // * or, we imply inputId -- isA --> input
    // For now, we follow the second approach (although this ALWAYS creates a new relation!!!)
    relateTo(inputId, inputClass(), Relation::isALabel);
    relateTo(typeId, typeClass(), Relation::isALabel);
    return relateTo(inputId, typeId, "needs"); // TODO: This should be registered somewhere...Actually it is some form of isA, right?
}

unsigned Graph::depends(const unsigned inputId, const unsigned outputId)
{
    // Either we
    // * check if inputId -- isA --> input
    // * or, we imply inputId -- isA --> input
    // For now, we follow the second approach (although this ALWAYS creates a new relation!!!)
    relateTo(inputId, inputClass(), Relation::isALabel);
    relateTo(outputId, outputClass(), Relation::isALabel);
    return relateTo(inputId, outputId, "depends"); // TODO: This should be registered somewhere...Actually it is some form of connectedTo, right?
}

}
