#include "SoftwareGraph.hpp"

namespace Software {


// Type
const std::string Type::classLabel = "Type";
unsigned Type::lastSuperclassId = 0;

Set* Type::Superclass()
{
    Hyperedge *edge;
    if (!Type::lastSuperclassId || !(edge = Hyperedge::find(Type::lastSuperclassId)))
    {
        // First call or previous superclass has been destroyed
        edge = Set::create(Type::classLabel);
        lastSuperclassId = edge->id();
    }
    return static_cast<Set*>(edge);
}

// Interface
const std::string Interface::classLabel = "SoftwareInterface";
unsigned Interface::lastSuperclassId = 0;

Set* Interface::Superclass()
{
    Hyperedge *edge;
    if (!Interface::lastSuperclassId || !(edge = Hyperedge::find(Interface::lastSuperclassId)))
    {
        // First call or previous superclass has been destroyed
        edge = Set::create(Interface::classLabel);
        lastSuperclassId = edge->id();
    }
    return static_cast<Set*>(edge);
}

// Input
const std::string Input::classLabel = "Input";
unsigned Input::lastSuperclassId = 0;

Set* Input::Superclass()
{
    Hyperedge *edge;
    if (!Input::lastSuperclassId || !(edge = Hyperedge::find(Input::lastSuperclassId)))
    {
        // First call or previous superclass has been destroyed
        edge = Set::create(Input::classLabel);
        lastSuperclassId = edge->id();
    }
    return static_cast<Set*>(edge);
}

bool Input::needs(Set* type)
{
    bool result = true;

    // At first, we have to add device & type to the corresponding sets
    result &= this->isA(Input::Superclass());
    result &= type->isA(Type::Superclass());

    // Find a needs relation in device
    auto edges = pointingTo("needs");
    Relation *needs = NULL;
    if (edges.size())
    {
        needs = Relation::promote(Hyperedge::find(*edges.begin()));
        result &= needs->to(type);
    } else {
        // Finally we create a new relation (1-to-1)
        needs = Relation::create("needs");
        result &= needs->from(this);
        result &= needs->to(type);
    }

    return result;
}

bool Input::depends(Set* output)
{
    bool result = true;

    // At first, we have to add device & output to the corresponding sets
    result &= this->isA(Input::Superclass());
    result &= output->isA(Output::Superclass());

    // Find a depends relation in device
    auto edges = pointingTo("depends");
    Relation *depends = NULL;
    if (edges.size())
    {
        depends = Relation::promote(Hyperedge::find(*edges.begin()));
        result &= depends->to(output);
    } else {
        // Finally we create a new relation (1-to-1)
        depends = Relation::create("depends");
        result &= depends->from(this);
        result &= depends->to(output);
    }

    return result;
}

// Output
const std::string Output::classLabel = "Output";
unsigned Output::lastSuperclassId = 0;

Set* Output::Superclass()
{
    Hyperedge *edge;
    if (!Output::lastSuperclassId || !(edge = Hyperedge::find(Output::lastSuperclassId)))
    {
        // First call or previous superclass has been destroyed
        edge = Set::create(Output::classLabel);
        lastSuperclassId = edge->id();
    }
    return static_cast<Set*>(edge);
}

bool Output::provides(Set* type)
{
    bool result = true;

    // At first, we have to add output & type to the corresponding sets
    result &= this->isA(Output::Superclass());
    result &= type->isA(Type::Superclass());

    // Find a provides relation in device
    auto edges = pointingTo("provides");
    Relation *provides = NULL;
    if (edges.size())
    {
        provides = Relation::promote(Hyperedge::find(*edges.begin()));
        result &= provides->to(type);
    } else {
        // Finally we create a new relation (1-to-1)
        provides = Relation::create("provides");
        result &= provides->from(this);
        result &= provides->to(type);
    }

    return result;
}

// Algorithm
const std::string Algorithm::classLabel = "Algorithm";
unsigned Algorithm::lastSuperclassId = 0;

Set* Algorithm::Superclass()
{
    Hyperedge *edge;
    if (!Algorithm::lastSuperclassId || !(edge = Hyperedge::find(Algorithm::lastSuperclassId)))
    {
        // First call or previous superclass has been destroyed
        edge = Set::create(Algorithm::classLabel);
        lastSuperclassId = edge->id();
    }
    return static_cast<Set*>(edge);
}

// Algorithms & Interfaces
bool Algorithm::has(Set* interface)
{
    bool result = true;

    // At first, we have to add device & interface to the corresponding sets
    result &= this->isA(Algorithm::Superclass());
    result &= interface->isA(Interface::Superclass());

    // Find a has relation in device
    auto edges = pointingTo("has");
    Relation *has = NULL;
    if (edges.size())
    {
        has = Relation::promote(Hyperedge::find(*edges.begin()));
        result &= has->to(interface);
    } else {
        // Finally we create a new relation (1-to-1)
        has = Relation::create("has");
        result &= has->from(this);
        result &= has->to(interface);
    }

    return result;
}

bool Algorithm::has(Set::Sets interfaces)
{
    bool result = true;
    // 1-N relation based on 2-hyperedges (1-1 relations)
    for (auto interfaceId : interfaces)
    {
        auto interface = Set::promote(Hyperedge::find(interfaceId));
        result &= has(interface);
    }
    return result;
}

Set* Algorithm::aggregates()
{
    Set::Sets result;
    Hyperedge::Hyperedges hasRels = pointingTo("has");
    for (auto relId : hasRels)
    {
        auto others = Set::promote(Hyperedge::find(relId)->pointingTo());
        result.insert(others.begin(), others.end());
    }
    return Set::create(result, "aggregates");
}   

// GRAPH STUFF

Graph* Graph::create(const std::string& label)
{
    Set* neu = Set::create(label);
    Algorithm::Superclass()->memberOf(neu);
    Interface::Superclass()->memberOf(neu);
    Input::Superclass()->memberOf(neu);
    Output::Superclass()->memberOf(neu);
    Type::Superclass()->memberOf(neu);
    return static_cast<Graph*>(neu);
}

Set* Graph::algorithms()
{
    // FIXME: This is not working anymore!!!!! Because it is not possible to guarantee one and only one superclass!!!! FUUUUCKKKK
    Relation *superOf = Algorithm::Superclass()->superclassOf();
    Set *result = Set::create(Set::promote(superOf->pointingTo()), "Algorithms");
    delete superOf;
    return result;
}

Set* Graph::interfaces()
{
    Relation *superOf = Interface::Superclass()->superclassOf();
    Set *result = Set::create(Set::promote(superOf->pointingTo()), "Interfaces");
    delete superOf;
    return result;
}

Set* Graph::inputs()
{
    Relation *superOf = Input::Superclass()->superclassOf();
    Set *result = Set::create(Set::promote(superOf->pointingTo()), "Inputs");
    delete superOf;
    return result;
}

Set* Graph::outputs()
{
    Relation *superOf = Output::Superclass()->superclassOf();
    Set *result = Set::create(Set::promote(superOf->pointingTo()), "Outputs");
    delete superOf;
    return result;
}

Set* Graph::types()
{
    Relation *superOf = Type::Superclass()->superclassOf();
    Set *result = Set::create(Set::promote(superOf->pointingTo()), "Types");
    delete superOf;
    return result;
}

Algorithm* Graph::createAlgorithm(const std::string& name)
{
    return Set::create<Algorithm>(name);
}

Input* Graph::createInput(const std::string& name)
{
    return Set::create<Input>(name);
}

Output* Graph::createOutput(const std::string& name)
{
    return Set::create<Output>(name);
}

Type* Graph::createType(const std::string& name)
{
    return Set::create<Type>(name);
}

bool Graph::has(Set* algorithm, Set* interface)
{
    return algorithm->promote<Algorithm>()->has(interface);
}

bool Graph::has(Set* algorithm, Set::Sets interfaces)
{
    return algorithm->promote<Algorithm>()->has(interfaces);
}

bool Graph::has(Set::Sets algorithms, Set* interface)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges
    for (auto algorithmId : algorithms)
    {
        auto algorithm = Set::promote(Hyperedge::find(algorithmId))->promote<Algorithm>();
        result &= algorithm->has(interface);
    }
    return result;
}

bool Graph::has(Set::Sets algorithms, Set::Sets interfaces)
{
    bool result = true;

    // N-M relation based on N * (1-M) relations
    for (auto algorithmId : algorithms)
    {
        auto algorithm = Set::promote(Hyperedge::find(algorithmId))->promote<Algorithm>();
        result &= has(algorithm, interfaces);
    }
    return result;
}

bool Graph::depends(Set* input, Set* output)
{
    return input->promote<Input>()->depends(output);
}

bool Graph::depends(Set::Sets inputs, Set* output)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto inputId : inputs)
    {
        auto input = Set::promote(Hyperedge::find(inputId))->promote<Input>();
        result &= input->depends(output);
    }
    return result;
}

bool Graph::provides(Set* output, Set* type)
{
    return output->promote<Output>()->provides(type);
}

bool Graph::provides(Set::Sets outputs, Set* type)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto outputId : outputs)
    {
        auto output = Set::promote(Hyperedge::find(outputId))->promote<Output>();
        result &= output->provides(type);
    }
    return result;
}

bool Graph::needs(Set* input, Set* type)
{
    return input->promote<Input>()->needs(type);
}

bool Graph::needs(Set::Sets inputs, Set* type)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto inputId : inputs)
    {
        auto input = Set::promote(Hyperedge::find(inputId))->promote<Input>();
        result &= input->needs(type);
    }
    return result;
}

}
