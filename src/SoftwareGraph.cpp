#include "SoftwareGraph.hpp"

namespace Software {


// Concept Ids
const unsigned Graph::InterfaceId = 2001;
const unsigned Graph::InputId = 2002;
const unsigned Graph::OutputId = 2003;
const unsigned Graph::AlgorithmId = 2000;
// Relation Ids
const unsigned Graph::IsAId      = 111;
const unsigned Graph::HasAId     = 222;
const unsigned Graph::DependsOnId = 444;
const unsigned Graph::NeedsId = 555;
const unsigned Graph::ProvidesId = 666;

// GRAPH STUFF
void Graph::createMainConcepts()
{
    Conceptgraph::create(Graph::AlgorithmId, "ALGORITHM");
    Conceptgraph::create(Graph::InterfaceId, "INTERFACE");
    Conceptgraph::create(Graph::InputId, "INPUT");
    Conceptgraph::create(Graph::OutputId, "OUTPUT");
    // FIXME: The following concepts should be relations not concepts ... or?
    Conceptgraph::create(Graph::IsAId, "IS-A");
    Conceptgraph::create(Graph::HasAId, "HAS-A");
    Conceptgraph::create(Graph::DependsOnId, "DEPENDS-ON");
    Conceptgraph::create(Graph::NeedsId, "NEEDS");
    Conceptgraph::create(Graph::ProvidesId, "PROVIDES");
}

Graph::Graph()
{
    createMainConcepts();
}

Graph::Graph(Conceptgraph& A)
: Conceptgraph(A)
{
    createMainConcepts();
}

Graph::~Graph()
{
}

Hypergraph::Hyperedges Graph::algorithms(const std::string& name)
{
    Hyperedges result;
    result = Conceptgraph::traverse(Graph::AlgorithmId, name, get(Graph::IsAId)->label(), UP);
    result.erase(Graph::AlgorithmId);
    return result;
}

Hypergraph::Hyperedges Graph::interfaces(const std::string& name)
{
    Hyperedges result;
    result = Conceptgraph::traverse(Graph::InterfaceId, name, get(Graph::IsAId)->label(), UP);
    result.erase(Graph::InterfaceId);
    return result;
}

Hypergraph::Hyperedges Graph::inputs(const std::string& name)
{
    Hyperedges result;
    result = Conceptgraph::traverse(Graph::InputId, name, get(Graph::IsAId)->label(), UP);
    result.erase(Graph::InputId);
    return result;
}

Hypergraph::Hyperedges Graph::outputs(const std::string& name)
{
    Hyperedges result;
    result = Conceptgraph::traverse(Graph::OutputId, name, get(Graph::IsAId)->label(), UP);
    result.erase(Graph::OutputId);
    return result;
}

unsigned Graph::createAlgorithm(const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::AlgorithmId, get(Graph::IsAId)->label());
    return a;
}

unsigned Graph::createInput(const unsigned interfaceId, const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::InputId, get(Graph::IsAId)->label());
    relate(a, interfaceId, get(Graph::IsAId)->label());
    return a;
}

unsigned Graph::createOutput(const unsigned interfaceId, const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::OutputId, get(Graph::IsAId)->label()); // TODO: This should be in a base class
    relate(a, interfaceId, get(Graph::IsAId)->label());
    return a;
}

unsigned Graph::createInterface(const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::InterfaceId, get(Graph::IsAId)->label()); // TODO: This should be in a base class
    return a;
}

unsigned Graph::has(const unsigned algorithmId, const unsigned interfaceId)
{
    if (algorithms().count(algorithmId) && interfaces().count(interfaceId))
    {
        return relate(algorithmId, interfaceId, get(Graph::HasAId)->label());
    }
    return 0;
}

unsigned Graph::provides(const unsigned algorithmId, const unsigned outputId)
{
    if (algorithms().count(algorithmId) && outputs().count(outputId))
    {
        return relate(algorithmId, outputId, get(Graph::ProvidesId)->label());
    }
    return 0;
}

unsigned Graph::needs(const unsigned algorithmId, const unsigned inputId)
{
    if (algorithms().count(algorithmId) && inputs().count(inputId))
    {
        return relate(algorithmId, inputId, get(Graph::NeedsId)->label());
    }
    return 0;
}

unsigned Graph::depends(const unsigned inputId, const unsigned outputId)
{
    if (inputs().count(inputId) && outputs().count(outputId))
    {
        return relate(inputId, outputId, get(Graph::DependsOnId)->label());
    }
    return 0;
}

}
