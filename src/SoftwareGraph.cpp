#include "SoftwareGraph.hpp"

namespace Software {

Graph::Graph(const std::string& label)
: Hyperedge(label)
{
    Hyperedge *algorithms = Hyperedge::create("Algorithms");
    Hyperedge *interfaces = Hyperedge::create("Interfaces");
    Hyperedge *inputs = Hyperedge::create("Inputs");
    Hyperedge *outputs = Hyperedge::create("Outputs");
    Hyperedge *parameters = Hyperedge::create("Parameters");
    Hyperedge *types = Hyperedge::create("Types");
    contains(algorithms);
    contains(interfaces);
    contains(inputs);
    interfaces->contains(inputs);
    contains(outputs);
    interfaces->contains(outputs);
    contains(parameters);
    interfaces->contains(parameters);
    contains(types);
    // Register Ids for easier access
    _algId = algorithms->id();
    _ifId = interfaces->id();
    _inpId = inputs->id();
    _outpId = outputs->id();
    _paramId = parameters->id();
    _typeId = types->id();
}

Hyperedge* Graph::algorithms()
{
    return members()[_algId];
}

Hyperedge* Graph::interfaces()
{
    return members()[_ifId];
}

Hyperedge* Graph::inputs()
{
    return members()[_inpId];
}

Hyperedge* Graph::outputs()
{
    return members()[_outpId];
}

Hyperedge* Graph::parameters()
{
    return members()[_paramId];
}

Hyperedge* Graph::types()
{
    return members()[_typeId];
}

Hyperedge* Graph::createAlgorithm(const std::string& name)
{
    Hyperedge *newbie = (Hyperedge::create(name));
    algorithms()->contains(newbie);
    return newbie;
}

Hyperedge* Graph::createInput(const std::string& name)
{
    Hyperedge *newbie = (Hyperedge::create(name));
    //interfaces()->contains(newbie);
    inputs()->contains(newbie);
    return newbie;
}

Hyperedge* Graph::createOutput(const std::string& name)
{
    Hyperedge *newbie = (Hyperedge::create(name));
    //interfaces()->contains(newbie);
    outputs()->contains(newbie);
    return newbie;
}

Hyperedge* Graph::createParameter(const std::string& name)
{
    Hyperedge *newbie = (Hyperedge::create(name));
    //interfaces()->contains(newbie);
    parameters()->contains(newbie);
    return newbie;
}

Hyperedge* Graph::createType(const std::string& name)
{
    Hyperedge *newbie = (Hyperedge::create(name));
    types()->contains(newbie);
    return newbie;
}

bool Graph::has(Hyperedge* algorithm, Hyperedge* interface)
{
    // At first, we have to add algorithm & interface to the corresponding sets
    algorithms()->contains(algorithm);
    interfaces()->contains(interface);

    // Finally we create a new relation (1-to-1)
    Hyperedge *has = (Hyperedge::create("has"));
    has->contains(algorithm);
    has->contains(interface);
    contains(has);
    return true;
}

bool Graph::has(Hyperedge* algorithm, Hyperedge::Hyperedges interfaces)
{
    // 1-N relation based on 2-hyperedges (1-1 relations)
    for (auto interfaceIt : interfaces)
    {
        auto interface = interfaceIt.second;
        has(algorithm, interface);
    }
    return true;
}

bool Graph::has(Hyperedge::Hyperedges algorithms, Hyperedge* interface)
{
    // N-1 relation based on 2-hyperedges
    for (auto algorithmIt : algorithms)
    {
        auto algorithm = algorithmIt.second;
        has(algorithm, interface);
    }
    return true;
}

bool Graph::has(Hyperedge::Hyperedges algorithms, Hyperedge::Hyperedges interfaces)
{
    // N-M relation based on N * (1-M) relations
    for (auto algorithmIt : algorithms)
    {
        auto algorithm = algorithmIt.second;
        has(algorithm, interfaces);
    }
    return true;
}

bool Graph::depends(Hyperedge* input, Hyperedge* output)
{
    // At first, we have to add input & output to the corresponding sets
    inputs()->contains(input);
    outputs()->contains(output);

    // Finally we create a new relation (1-to-1)
    Hyperedge *depends = (Hyperedge::create("depends"));
    depends->contains(input);
    depends->contains(output);
    contains(depends);
    return true;
}

bool Graph::depends(Hyperedge::Hyperedges inputs, Hyperedge* output)
{
    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto inputIt : inputs)
    {
        auto input = inputIt.second;
        depends(input, output);
    }
    return true;
}

bool Graph::provides(Hyperedge* output, Hyperedge* type)
{
    // At first, we have to add output & type to the corresponding sets
    outputs()->contains(output);
    types()->contains(type);

    // Finally we create a new relation (1-to-1)
    Hyperedge *provides = (Hyperedge::create("provides"));
    provides->contains(output);
    provides->contains(type);
    contains(provides);
    return true;
}

bool Graph::provides(Hyperedge::Hyperedges outputs, Hyperedge* type)
{
    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto outputIt : outputs)
    {
        auto output = outputIt.second;
        provides(output, type);
    }
    return true;
}

bool Graph::needs(Hyperedge* input, Hyperedge* type)
{
    // At first, we have to add output & type to the corresponding sets
    inputs()->contains(input);
    types()->contains(type);

    // Finally we create a new relation (1-to-1)
    Hyperedge *needs = (Hyperedge::create("needs"));
    needs->contains(input);
    needs->contains(type);
    contains(needs);
    return true;
}

bool Graph::needs(Hyperedge::Hyperedges inputs, Hyperedge* type)
{
    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto inputIt : inputs)
    {
        auto input = inputIt.second;
        needs(input, type);
    }
    return true;
}

}
