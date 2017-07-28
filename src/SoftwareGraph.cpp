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
const unsigned Graph::IsAId             = CommonConceptGraph::IsAId;
const unsigned Graph::HasAId            = 2101;
const unsigned Graph::DependsOnId       = 2102;
const unsigned Graph::NeedsId           = 2103;
const unsigned Graph::ProvidesId        = 2104;
const unsigned Graph::ExpressedInId     = 2105;
const unsigned Graph::RealizesId        = 2106;
//const unsigned Graph::UsesId            = 2107;
const unsigned Graph::RepresentsId      = 2108;

// GRAPH STUFF
void Graph::createMainConcepts()
{
    // Concepts
    Conceptgraph::create(Graph::AlgorithmId, "ALGORITHM");
    Conceptgraph::create(Graph::InterfaceId, "INTERFACE");
    Conceptgraph::create(Graph::InputId, "INPUT");
    Conceptgraph::create(Graph::OutputId, "OUTPUT");
    Conceptgraph::create(Graph::ImplementationId, "IMPLEMENTATION");
    Conceptgraph::create(Graph::DatatypeId, "DATATYPE");
    Conceptgraph::create(Graph::LanguageId, "LANGUAGE");

    // Relations
    Conceptgraph::relate(Graph::HasAId, Graph::AlgorithmId, Graph::InterfaceId, "HAS-A");
    CommonConceptGraph::subrelationOf(Graph::HasAId, CommonConceptGraph::HasAId);

    Conceptgraph::relate(Graph::DependsOnId, Graph::InputId, Graph::OutputId, "DEPENDS-ON");
    CommonConceptGraph::subrelationOf(Graph::DependsOnId, CommonConceptGraph::ConnectsId);

    Conceptgraph::relate(Graph::NeedsId, Graph::AlgorithmId, Graph::InputId, "NEEDS");
    CommonConceptGraph::subrelationOf(Graph::NeedsId, Graph::HasAId);

    Conceptgraph::relate(Graph::ProvidesId, Graph::AlgorithmId, Graph::OutputId, "PROVIDES");
    CommonConceptGraph::subrelationOf(Graph::ProvidesId, Graph::HasAId);

    // TODO: Are these relations subrelations of other, known relations?
    Conceptgraph::relate(Graph::ExpressedInId, Graph::ImplementationId, Graph::LanguageId, "EXPRESSED-IN");
    Conceptgraph::get(Graph::ExpressedInId)->from(Graph::DatatypeId); // OK :)
    Conceptgraph::relate(Graph::RealizesId, Graph::ImplementationId, Graph::AlgorithmId, "REALIZES"); // Inverse of HAS-A?
    Conceptgraph::relate(Graph::RepresentsId, Graph::DatatypeId, Graph::InterfaceId, "REPRESENTS");
    //Conceptgraph::relate(Graph::UsesId, Graph::ImplementationId, Graph::DatatypeId, "USES"); // TODO: This is a questionable relation...
}

Graph::Graph()
{
    createMainConcepts();
}

Graph::Graph(CommonConceptGraph& A)
: CommonConceptGraph(A)
{
    createMainConcepts();
}

Graph::~Graph()
{
}

Hypergraph::Hyperedges Graph::algorithms(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::AlgorithmId, name);
    result.erase(Graph::AlgorithmId);
    return result;
}

Hypergraph::Hyperedges Graph::interfaces(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::InterfaceId, name);
    result.erase(Graph::InterfaceId);
    return result;
}

Hypergraph::Hyperedges Graph::inputs(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::InputId, name);
    result.erase(Graph::InputId);
    return result;
}

Hypergraph::Hyperedges Graph::outputs(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::OutputId, name);
    result.erase(Graph::OutputId);
    return result;
}

Hypergraph::Hyperedges Graph::implementations(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::ImplementationId, name);
    result.erase(Graph::ImplementationId);
    return result;
}

Hypergraph::Hyperedges Graph::datatypes(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::DatatypeId, name);
    result.erase(Graph::DatatypeId);
    return result;
}

Hypergraph::Hyperedges Graph::languages(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::LanguageId, name);
    result.erase(Graph::LanguageId);
    return result;
}

unsigned Graph::createAlgorithm(const std::string& name)
{
    unsigned a = create(name);
    CommonConceptGraph::isA(a, Graph::AlgorithmId);
    return a;
}

unsigned Graph::createInput(const unsigned interfaceId, const std::string& name)
{
    unsigned a = create(name);
    CommonConceptGraph::isA(a, Graph::InputId);
    CommonConceptGraph::isA(a, interfaceId);
    return a;
}

unsigned Graph::createOutput(const unsigned interfaceId, const std::string& name)
{
    unsigned a = create(name);
    CommonConceptGraph::isA(a, Graph::OutputId);
    CommonConceptGraph::isA(a, interfaceId);
    return a;
}

unsigned Graph::createInterface(const std::string& name)
{
    unsigned a = create(name);
    CommonConceptGraph::isA(a, Graph::InterfaceId);
    return a;
}

unsigned Graph::createImplementation(const std::string& name)
{
    unsigned a = create(name);
    CommonConceptGraph::isA(a, Graph::ImplementationId);
    return a;
}

unsigned Graph::createDatatype(const std::string& name)
{
    unsigned a = create(name);
    CommonConceptGraph::isA(a, Graph::DatatypeId);
    return a;
}

unsigned Graph::createLanguage(const std::string& name)
{
    unsigned a = create(name);
    CommonConceptGraph::isA(a, Graph::LanguageId);
    return a;
}

unsigned Graph::has(const unsigned algorithmId, const unsigned interfaceId)
{
    if (algorithms().count(algorithmId) && interfaces().count(interfaceId))
    {
        return CommonConceptGraph::relateFrom(algorithmId, interfaceId, Graph::HasAId);
    }
    return 0;
}

unsigned Graph::provides(const unsigned algorithmId, const unsigned outputId)
{
    if (algorithms().count(algorithmId) && outputs().count(outputId))
    {
        return CommonConceptGraph::relateFrom(algorithmId, outputId, Graph::ProvidesId);
    }
    return 0;
}

unsigned Graph::needs(const unsigned algorithmId, const unsigned inputId)
{
    if (algorithms().count(algorithmId) && inputs().count(inputId))
    {
        return CommonConceptGraph::relateFrom(algorithmId, inputId, Graph::NeedsId);
    }
    return 0;
}

unsigned Graph::depends(const unsigned inputId, const unsigned outputId)
{
    if (inputs().count(inputId) && outputs().count(outputId))
    {
        return CommonConceptGraph::relateFrom(inputId, outputId, Graph::DependsOnId);
    }
    return 0;
}

unsigned Graph::realizes(const unsigned implementationId, const unsigned algorithmId)
{
    if (implementations().count(implementationId) && algorithms().count(algorithmId))
    {
        return CommonConceptGraph::relateFrom(implementationId, algorithmId, Graph::RealizesId);
    }
    return 0;
}

unsigned Graph::represents(const unsigned datatypeId, const unsigned interfaceId)
{
    if (datatypes().count(datatypeId) && interfaces().count(interfaceId))
    {
        return CommonConceptGraph::relateFrom(datatypeId, interfaceId, Graph::RepresentsId);
    }
    return 0;
}

unsigned Graph::expressedIn(const unsigned implementationOrDatatypeId, const unsigned languageId)
{
    if ((implementations().count(implementationOrDatatypeId) || datatypes().count(implementationOrDatatypeId)) && languages().count(languageId))
    {
        return CommonConceptGraph::relateFrom(implementationOrDatatypeId, languageId, Graph::ExpressedInId);
    }
    return 0;
}

//unsigned Graph::uses(const unsigned implementationId, const unsigned datatypeId)
//{
//    if (implementations().count(implementationId) && datatypes().count(datatypeId))
//    {
//        return relate(implementationId, datatypeId, Graph::UsesId);
//    }
//    return 0;
//}

}
