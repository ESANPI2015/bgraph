#include "SoftwareGraph.hpp"

namespace Software {


// Interface
const std::string Graph::InterfaceLabel = "SoftwareInterface";
// Input
const std::string Graph::InputLabel = "Input";
// Output
const std::string Graph::OutputLabel = "Output";
// Algorithm
const std::string Graph::AlgorithmLabel = "Algorithm";

// GRAPH STUFF
Graph::Graph()
{
}

Graph::Graph(Conceptgraph& A)
: Conceptgraph(A)
{
    // We have to search the underlying concept graph and sort everything into 3 sets
    // A <-- ISA --> DeviceLabel means that A is to be inserted into _devices set
    Hyperedges candidates = Conceptgraph::find(Graph::AlgorithmLabel);
    for (unsigned candidateId : candidates)
    {
        // We now have access to all ROOT NODES: traverse the subgraph spanned by IS-A relations and add every hedge to the corresponding set
        // This means that we have to go in opposite direction of the IS-A relation
        Hyperedges algorithms = traverse(candidateId, "", "IS-A", UP);
        _algorithms.insert(algorithms.begin(), algorithms.end());
    }
    // Handle interface concept
    candidates = Conceptgraph::find(Graph::InterfaceLabel);
    for (unsigned candidateId : candidates)
    {
        // We now have access to all ROOT NODES: traverse the subgraph spanned by IS-A relations and add every hedge to the corresponding set
        Hyperedges interfaces = traverse(candidateId, "", "IS-A", UP);
        _interfaces.insert(interfaces.begin(), interfaces.end());
    }
    // Handle input concept
    candidates = Conceptgraph::find(Graph::InputLabel);
    for (unsigned candidateId : candidates)
    {
        // We now have access to all ROOT NODES: traverse the subgraph spanned by IS-A relations and add every hedge to the corresponding set
        Hyperedges inputs = traverse(candidateId, "", "IS-A", UP);
        _inputs.insert(inputs.begin(), inputs.end());
    }
    // Handle output concept
    candidates = Conceptgraph::find(Graph::OutputLabel);
    for (unsigned candidateId : candidates)
    {
        // We now have access to all ROOT NODES: traverse the subgraph spanned by IS-A relations and add every hedge to the corresponding set
        Hyperedges outputs = traverse(candidateId, "", "IS-A", UP);
        _outputs.insert(outputs.begin(), outputs.end());
    }
    // Now we have sorted everything ... maybe we should have this also as a reparse function?
}

Graph::~Graph()
{
}

unsigned Graph::algorithmConcept()
{
    // TODO: This could be a nice method at concept graph level (named findOrCreate(label))
    unsigned id;
    Hyperedges candidates = Conceptgraph::find(Graph::AlgorithmLabel);
    if (candidates.size())
    {
        id = *candidates.begin();
    } else {
        id = Conceptgraph::create(Graph::AlgorithmLabel);
    }
    return id;
}

unsigned Graph::interfaceConcept()
{
    // TODO: This could be a nice method at concept graph level (named findOrCreate(label))
    unsigned id;
    Hyperedges candidates = Conceptgraph::find(Graph::InterfaceLabel);
    if (candidates.size())
    {
        id = *candidates.begin();
    } else {
        id = Conceptgraph::create(Graph::InterfaceLabel);
    }
    return id;
}

unsigned Graph::inputConcept()
{
    // TODO: This could be a nice method at concept graph level (named findOrCreate(label))
    unsigned id;
    Hyperedges candidates = Conceptgraph::find(Graph::InputLabel);
    if (candidates.size())
    {
        id = *candidates.begin();
    } else {
        id = Conceptgraph::create(Graph::InputLabel);
    }
    return id;
}

unsigned Graph::outputConcept()
{
    // TODO: This could be a nice method at concept graph level (named findOrCreate(label))
    unsigned id;
    Hyperedges candidates = Conceptgraph::find(Graph::OutputLabel);
    if (candidates.size())
    {
        id = *candidates.begin();
    } else {
        id = Conceptgraph::create(Graph::OutputLabel);
    }
    return id;
}

Hypergraph::Hyperedges Graph::algorithms(const std::string& name)
{
    Hyperedges result;
    for (auto id : _algorithms)
    {
        if (name.empty() || (name == get(id)->label()))
        {
            result.insert(id);
        }
    }
    return result;
}

Hypergraph::Hyperedges Graph::interfaces(const std::string& name)
{
    Hyperedges result;
    for (auto id : _interfaces)
    {
        if (name.empty() || (name == get(id)->label()))
        {
            result.insert(id);
        }
    }
    return result;
}

Hypergraph::Hyperedges Graph::inputs(const std::string& name)
{
    Hyperedges result;
    for (auto id : _inputs)
    {
        if (name.empty() || (name == get(id)->label()))
        {
            result.insert(id);
        }
    }
    return result;
}

Hypergraph::Hyperedges Graph::outputs(const std::string& name)
{
    Hyperedges result;
    for (auto id : _outputs)
    {
        if (name.empty() || (name == get(id)->label()))
        {
            result.insert(id);
        }
    }
    return result;
}

unsigned Graph::createAlgorithm(const std::string& name)
{
    // Find the concept
    unsigned A = algorithmConcept();
    unsigned a = create(name);
    relate(a, A, "IS-A"); // TODO: This should be in a base class
    _algorithms.insert(a);
    return a;
}

unsigned Graph::createInput(const std::string& name)
{
    // Find the concept
    unsigned A = inputConcept();
    unsigned a = create(name);
    relate(a, A, "IS-A"); // TODO: This should be in a base class
    _inputs.insert(a);
    return a;
}

unsigned Graph::createOutput(const std::string& name)
{
    // Find the concept
    unsigned A = outputConcept();
    unsigned a = create(name);
    relate(a, A, "IS-A"); // TODO: This should be in a base class
    _outputs.insert(a);
    return a;
}

unsigned Graph::createInterface(const std::string& name)
{
    // Find the concept
    unsigned A = interfaceConcept();
    unsigned a = create(name);
    relate(a, A, "IS-A"); // TODO: This should be in a base class
    _interfaces.insert(a);
    return a;
}

unsigned Graph::has(const unsigned algorithmId, const unsigned interfaceId)
{
    // Either we
    // * check if algorithmId -- isA --> algorithm
    // * or, we imply algorithmId -- isA --> algorithm
    // This is the first approach
    if (_algorithms.count(algorithmId) && _interfaces.count(interfaceId))
    {
        return relate(algorithmId, interfaceId, "HAS"); // TODO: This relation label should be in a base class/dictionary
    }
    return 0;
}

unsigned Graph::provides(const unsigned algorithmId, const unsigned outputId)
{
    // Either we
    // * check if outputId -- isA --> output
    // * or, we imply outputId -- isA --> output
    // This is the first approach
    if (_algorithms.count(algorithmId) && _outputs.count(outputId))
    {
        return relate(algorithmId, outputId, "PROVIDES"); // TODO: This relation label should be in a base class/dictionary
    }
    return 0;
}

unsigned Graph::needs(const unsigned algorithmId, const unsigned inputId)
{
    // Either we
    // * check if inputId -- isA --> input
    // * or, we imply inputId -- isA --> input
    if (_algorithms.count(algorithmId) && _inputs.count(inputId))
    {
        return relate(algorithmId, inputId, "PROVIDES"); // TODO: This relation label should be in a base class/dictionary
    }
    return 0;
}

unsigned Graph::depends(const unsigned inputId, const unsigned outputId)
{
    // Either we
    // * check if inputId -- isA --> input
    // * or, we imply inputId -- isA --> input
    // This is the first approach
    if (_inputs.count(inputId) && _outputs.count(outputId))
    {
        return relate(inputId, outputId, "DEPENDS"); // TODO: This relation label should be in a base class/dictionary
    }
    return 0;
}

}
