#include "SoftwareGraph.hpp"

namespace Software {


// Type
Set* Type::superclass = NULL;

Type::Type(const std::string& label)
: Set(label)
{
    // Create isA relation to the superclass
    this->isA(Type::Superclass());
}

Set* Type::Superclass()
{
    if (!Type::superclass)
    {
        Type::superclass = Set::create("Type");
    }
    return Type::superclass;
}

// Interface
Set* Interface::superclass = NULL;

Interface::Interface(const std::string& label)
: Set(label)
{
    // Create isA relation to the superclass
    this->isA(Interface::Superclass());
}

Set* Interface::Superclass()
{
    if (!Interface::superclass)
    {
        Interface::superclass = Set::create("Interface");
    }
    return Interface::superclass;
}

// Input
Set* Input::superclass = NULL;

Input::Input(const std::string& label)
: Interface(label)
{
    // Create isA relation to the superclass
    this->isA(Input::Superclass());
}

Set* Input::Superclass()
{
    if (!Input::superclass)
    {
        Input::superclass = Set::create("Input");
        Input::superclass->isA(Interface::Superclass());
    }
    return Input::superclass;
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
        needs = static_cast< Relation* >(Hyperedge::find(*edges.begin()));
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
        depends = static_cast< Relation* >(Hyperedge::find(*edges.begin()));
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
Set* Output::superclass = NULL;

Output::Output(const std::string& label)
: Interface(label)
{
    // Create isA relation to the superclass
    this->isA(Output::Superclass());
}

Set* Output::Superclass()
{
    if (!Output::superclass)
    {
        Output::superclass = Set::create("Output");
        Output::superclass->isA(Interface::Superclass());
    }
    return Output::superclass;
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
        provides = static_cast< Relation* >(Hyperedge::find(*edges.begin()));
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
Set* Algorithm::superclass = NULL;

Algorithm::Algorithm(const std::string& label)
: Set(label)
{
    // Create isA relation to the superclass
    this->isA(superclass);
}

Set* Algorithm::Superclass()
{
    if (!Algorithm::superclass)
    {
        Algorithm::superclass = Set::create("Algorithm");
    }
    return Algorithm::superclass;
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
        has = static_cast< Relation* >(Hyperedge::find(*edges.begin()));
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

Graph::Graph(const std::string& label)
: Set(label)
{
    Algorithm::Superclass()->memberOf(this);
    Interface::Superclass()->memberOf(this);
    Input::Superclass()->memberOf(this);
    //input->isA(interface);
    Output::Superclass()->memberOf(this);
    //output->isA(interface);
    Type::Superclass()->memberOf(this);
}

Set* Graph::algorithms()
{
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
    Set *newbie = Set::create(name);
    newbie->isA(Algorithm::Superclass());
    return static_cast<Algorithm*>(newbie);
}

Input* Graph::createInput(const std::string& name)
{
    Set *newbie = Set::create(name);
    newbie->isA(Input::Superclass());
    return static_cast<Input*>(newbie);
}

Output* Graph::createOutput(const std::string& name)
{
    Set *newbie = Set::create(name);
    newbie->isA(Output::Superclass());
    return static_cast<Output*>(newbie);
}

Type* Graph::createType(const std::string& name)
{
    Set *newbie = Set::create(name);
    newbie->isA(Type::Superclass());
    return static_cast<Type*>(newbie);
}

bool Graph::has(Set* algorithm, Set* interface)
{
    return static_cast<Algorithm*>(algorithm)->has(interface);
}

bool Graph::has(Set* algorithm, Set::Sets interfaces)
{
    return static_cast<Algorithm*>(algorithm)->has(interfaces);
}

bool Graph::has(Set::Sets algorithms, Set* interface)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges
    for (auto algorithmId : algorithms)
    {
        auto algorithm = static_cast<Algorithm*>(Hyperedge::find(algorithmId));
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
        auto algorithm = static_cast<Algorithm*>(Hyperedge::find(algorithmId));
        result &= has(algorithm, interfaces);
    }
    return result;
}

bool Graph::depends(Set* input, Set* output)
{
    return static_cast<Input*>(input)->depends(output);
}

bool Graph::depends(Set::Sets inputs, Set* output)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto inputId : inputs)
    {
        auto input = static_cast<Input*>(Hyperedge::find(inputId));
        result &= input->depends(output);
    }
    return result;
}

bool Graph::provides(Set* output, Set* type)
{
    return static_cast<Output*>(output)->provides(type);
}

bool Graph::provides(Set::Sets outputs, Set* type)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto outputId : outputs)
    {
        auto output = static_cast<Output*>(Hyperedge::find(outputId));
        result &= output->provides(type);
    }
    return result;
}

bool Graph::needs(Set* input, Set* type)
{
    return static_cast<Input*>(input)->needs(type);
}

bool Graph::needs(Set::Sets inputs, Set* type)
{
    bool result = true;

    // N-1 relation based on 2-hyperedges (1-1 relations)
    for (auto inputId : inputs)
    {
        auto input = static_cast<Input*>(Hyperedge::find(inputId));
        result &= input->needs(type);
    }
    return result;
}

}
