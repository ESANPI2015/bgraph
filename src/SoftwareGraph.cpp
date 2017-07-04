#include "SoftwareGraph.hpp"

namespace Software {


// Concept Ids
const unsigned Graph::InterfaceId       = 2001;
const unsigned Graph::InputId           = 2002;
const unsigned Graph::OutputId          = 2003;
const unsigned Graph::AlgorithmId       = 2000;
const unsigned Graph::ImplementationId  = 2004;
const unsigned Graph::DatatypeId        = 2005;
const unsigned Graph::LanguageId        = 2006;
// Relation Ids
const unsigned Graph::IsAId             = 11;
const unsigned Graph::HasAId            = 22;
const unsigned Graph::DependsOnId       = 44;
const unsigned Graph::NeedsId           = 55;
const unsigned Graph::ProvidesId        = 66;
const unsigned Graph::ExpressedInId     = 77;
const unsigned Graph::RealizesId        = 88;
const unsigned Graph::UsesId            = 99;

// GRAPH STUFF
void Graph::createMainConcepts()
{
    Conceptgraph::create(Graph::AlgorithmId, "ALGORITHM");
    Conceptgraph::create(Graph::InterfaceId, "INTERFACE");
    Conceptgraph::create(Graph::InputId, "INPUT");
    Conceptgraph::create(Graph::OutputId, "OUTPUT");
    Conceptgraph::create(Graph::ImplementationId, "IMPLEMENTATION");
    Conceptgraph::create(Graph::DatatypeId, "DATATYPE");
    Conceptgraph::create(Graph::LanguageId, "LANGUAGE");
    // FIXME: The following concepts should be relations not concepts ... or?
    //Conceptgraph::defineRelation(Graph::HasAId, "IS-A", Graph::AlgorithmId, Graph::InterfaceId);
    //When stating a fact, we should do this (it will also instantiate from MASTER HAS-A)
    //Conceptgraph::relate(algorithmId, interfaceId, Graph::HasAId);
    Conceptgraph::create(Graph::IsAId, "IS-A");
    Conceptgraph::create(Graph::HasAId, "HAS-A");
    Conceptgraph::create(Graph::DependsOnId, "DEPENDS-ON");
    Conceptgraph::create(Graph::NeedsId, "NEEDS");
    Conceptgraph::create(Graph::ProvidesId, "PROVIDES");
    Conceptgraph::create(Graph::ExpressedInId, "EXPRESSED-IN");
    Conceptgraph::create(Graph::RealizesId, "REALIZES");
    Conceptgraph::create(Graph::UsesId, "USES");
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

Hypergraph::Hyperedges Graph::implementations(const std::string& name)
{
    Hyperedges result;
    result = Conceptgraph::traverse(Graph::ImplementationId, name, get(Graph::IsAId)->label(), UP);
    result.erase(Graph::ImplementationId);
    return result;
}

Hypergraph::Hyperedges Graph::datatypes(const std::string& name)
{
    Hyperedges result;
    result = Conceptgraph::traverse(Graph::DatatypeId, name, get(Graph::IsAId)->label(), UP);
    result.erase(Graph::DatatypeId);
    return result;
}

Hypergraph::Hyperedges Graph::languages(const std::string& name)
{
    Hyperedges result;
    result = Conceptgraph::traverse(Graph::LanguageId, name, get(Graph::IsAId)->label(), UP);
    result.erase(Graph::LanguageId);
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
    relate(a, Graph::OutputId, get(Graph::IsAId)->label());
    relate(a, interfaceId, get(Graph::IsAId)->label());
    return a;
}

unsigned Graph::createInterface(const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::InterfaceId, get(Graph::IsAId)->label());
    return a;
}

unsigned Graph::createImplementation(const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::ImplementationId, get(Graph::IsAId)->label());
    return a;
}

unsigned Graph::createDatatype(const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::DatatypeId, get(Graph::IsAId)->label());
    return a;
}

unsigned Graph::createLanguage(const std::string& name)
{
    createMainConcepts();
    unsigned a = create(name);
    relate(a, Graph::LanguageId, get(Graph::IsAId)->label());
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

unsigned Graph::realizes(const unsigned implementationOrDatatypeId, const unsigned algorithmOrInterfaceId)
{
    if ((implementations().count(implementationOrDatatypeId) && algorithms().count(algorithmOrInterfaceId)) ||
        (datatypes().count(implementationOrDatatypeId) && interfaces().count(algorithmOrInterfaceId)))
    {
        return relate(implementationOrDatatypeId, algorithmOrInterfaceId, get(Graph::RealizesId)->label());
    }
    return 0;
}

unsigned Graph::expressedIn(const unsigned implementationOrDatatypeId, const unsigned languageId)
{
    if ((implementations().count(implementationOrDatatypeId) || datatypes().count(implementationOrDatatypeId)) && languages().count(languageId))
    {
        return relate(implementationOrDatatypeId, languageId, get(Graph::ExpressedInId)->label());
    }
    return 0;
}

unsigned Graph::uses(const unsigned implementationId, const unsigned datatypeId)
{
    if (implementations().count(implementationId) && datatypes().count(datatypeId))
    {
        return relate(implementationId, datatypeId, get(Graph::UsesId)->label());
    }
    return 0;
}

}
