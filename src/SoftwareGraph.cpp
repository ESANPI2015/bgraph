#include "SoftwareGraph.hpp"

namespace Software {

Graph::Graph(const std::string& label)
: Set(label)
{
    Set *algorithm = Set::create("Algorithm");
    Set *interface = Set::create("Interface");
    Set *input = Set::create("Input");
    Set *output = Set::create("Output");
    Set *type = Set::create("Type");
    algorithm->memberOf(this);
    interface->memberOf(this);
    input->memberOf(this);
    input->isA(interface);
    output->memberOf(this);
    output->isA(interface);
    type->memberOf(this);
    // Register Ids for easier access
    _algId = algorithm->id();
    _ifId = interface->id();
    _inpId = input->id();
    _outpId = output->id();
    _typeId = type->id();
}

Set* Graph::algorithms()
{
    return algorithm()->subclasses();
}

Set* Graph::interfaces()
{
    return interface()->subclasses();
}

Set* Graph::inputs()
{
    return input()->subclasses();
}

Set* Graph::outputs()
{
    return output()->subclasses();
}

Set* Graph::types()
{
    return type()->subclasses();
}

Set* Graph::algorithm()
{
    return Set::promote(_created[_algId]);
}

Set* Graph::interface()
{
    return Set::promote(_created[_ifId]);
}

Set* Graph::input()
{
    return Set::promote(_created[_inpId]);
}

Set* Graph::output()
{
    return Set::promote(_created[_outpId]);
}

Set* Graph::type()
{
    return Set::promote(_created[_typeId]);
}

Set* Graph::createAlgorithm(const std::string& name)
{
    Set *newbie = (Set::create(name));
    newbie->isA(algorithm());
    return newbie;
}

Set* Graph::createInput(const std::string& name)
{
    Set *newbie = (Set::create(name));
    newbie->isA(input());
    return newbie;
}

Set* Graph::createOutput(const std::string& name)
{
    Set *newbie = (Set::create(name));
    newbie->isA(output());
    return newbie;
}

Set* Graph::createType(const std::string& name)
{
    Set *newbie = (Set::create(name));
    newbie->isA(type());
    return newbie;
}

bool Graph::has(Set* algorithm, Set* interface)
{
    bool result = true;

    // At first, we have to add algorithm & interface to the corresponding sets
    result &= algorithm->isA(this->algorithm());
    result &= interface->isA(this->interface());

    // Find a has relation in algorithm
    auto edges = algorithm->pointingTo("has");
    Relation *has = NULL;
    if (edges.size())
    {
        has = static_cast< Relation *>(edges.begin()->second);
        result &= has->to(interface);
    } else {
        // Finally we create a new relation (1-to-1)
        has = (Relation::create("has"));
        result &= has->from(algorithm);
        result &= has->to(interface);
    }

    return true;
}

bool Graph::has(Set* algorithm, Set::Sets interfaces)
{
    // 1-N relation based on 2-hyperedges (1-1 relations)
    for (auto interfaceIt : interfaces)
    {
        auto interface = interfaceIt.second;
        has(algorithm, interface);
    }
    return true;
}

bool Graph::has(Set::Sets algorithms, Set* interface)
{
    // N-1 relation based on 2-hyperedges
    for (auto algorithmIt : algorithms)
    {
        auto algorithm = algorithmIt.second;
        has(algorithm, interface);
    }
    return true;
}

bool Graph::has(Set::Sets algorithms, Set::Sets interfaces)
{
    // N-M relation based on N * (1-M) relations
    for (auto algorithmIt : algorithms)
    {
        auto algorithm = algorithmIt.second;
        has(algorithm, interfaces);
    }
    return true;
}

bool Graph::depends(Set* input, Set* output)
{
    bool result = true;

    // At first, we have to add input & output to the corresponding sets
    result &= input->isA(this->input());
    result &= output->isA(this->output());

    // Find a depends relation in input
    auto edges = input->pointingTo("depends");
    Relation *depends = NULL;
    if (edges.size())
    {
        depends = static_cast< Relation *>(edges.begin()->second);
        result &= depends->to(output);
    } else {
        // Finally we create a new relation (1-to-1)
        depends = (Relation::create("depends"));
        result &= depends->from(input);
        result &= depends->to(output);
    }

    return true;
}

bool Graph::depends(Set::Sets inputs, Set* output)
{
    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto inputIt : inputs)
    {
        auto input = inputIt.second;
        depends(input, output);
    }
    return true;
}

bool Graph::provides(Set* output, Set* type)
{
    bool result = true;

    // At first, we have to add output & type to the corresponding sets
    result &= output->isA(this->output());
    result &= type->isA(this->type());

    // Find a provides relation in output
    auto edges = output->pointingTo("provides");
    Relation *provides = NULL;
    if (edges.size())
    {
        provides = static_cast< Relation *>(edges.begin()->second);
        result &= provides->to(type);
    } else {
        // Finally we create a new relation (1-to-1)
        provides = (Relation::create("provides"));
        result &= provides->from(output);
        result &= provides->to(type);
    }

    return true;
}

bool Graph::provides(Set::Sets outputs, Set* type)
{
    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto outputIt : outputs)
    {
        auto output = outputIt.second;
        provides(output, type);
    }
    return true;
}

bool Graph::needs(Set* input, Set* type)
{
    bool result = true;

    // At first, we have to add input & type to the corresponding sets
    result &= input->isA(this->input());
    result &= type->isA(this->type());

    // Find a needs relation in input
    auto edges = input->pointingTo("needs");
    Relation *needs = NULL;
    if (edges.size())
    {
        needs = static_cast< Relation *>(edges.begin()->second);
        result &= needs->to(type);
    } else {
        // Finally we create a new relation (1-to-1)
        needs = (Relation::create("needs"));
        result &= needs->from(input);
        result &= needs->to(type);
    }

    return true;
}

bool Graph::needs(Set::Sets inputs, Set* type)
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
