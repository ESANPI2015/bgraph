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

    Conceptgraph::relate(Graph::ExpressedInId, Graph::ImplementationId, Graph::LanguageId, "EXPRESSED-IN");
    Conceptgraph::get(Graph::ExpressedInId)->from(Graph::DatatypeId); // OK :)
    // CommonConceptGraph::subrelationOf(Graph::ExpressedInId, CommonConceptGraph::PartOfId); // TODO: True?

    Conceptgraph::relate(Graph::RealizesId, Graph::ImplementationId, Graph::AlgorithmId, "REALIZES");
    // CommonConceptGraph::subrelationOf(Graph::RealizesId, CommonConceptGraph::IsAId); // TODO: True?
    Conceptgraph::relate(Graph::RepresentsId, Graph::DatatypeId, Graph::InterfaceId, "REPRESENTS");
    // CommonConceptGraph::subrelationOf(Graph::RepresentsId, CommonConceptGraph::IsAId); // TODO: True?
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

Hypergraph::Hyperedges Graph::algorithmClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::AlgorithmId, name);
    return result;
}

Hypergraph::Hyperedges Graph::interfaceClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::InterfaceId, name);
    return result;
}

Hypergraph::Hyperedges Graph::inputClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::InputId, name);
    return result;
}

Hypergraph::Hyperedges Graph::outputClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::OutputId, name);
    return result;
}

Hypergraph::Hyperedges Graph::implementationClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::ImplementationId, name);
    return result;
}

Hypergraph::Hyperedges Graph::datatypeClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::DatatypeId, name);
    return result;
}

Hypergraph::Hyperedges Graph::languageClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::LanguageId, name);
    return result;
}

Hypergraph::Hyperedges Graph::algorithms(const std::string& name, const std::string& className)
{
    Hyperedges result;
    // Get all device classes
    Hyperedges classIds = algorithmClasses(className);
    // For each of them get the instances with the given name
    for (auto classId : classIds)
    {
        result = unite(result, CommonConceptGraph::instancesOf(classId, name)); // TODO: Make instancesOf over Hyperedges
    }
    return result;
}
Hypergraph::Hyperedges Graph::interfaces(const std::string& name, const std::string& className)
{
    Hyperedges result;
    // Get all device classes
    Hyperedges classIds = interfaceClasses(className);
    // For each of them get the instances with the given name
    for (auto classId : classIds)
    {
        result = unite(result, CommonConceptGraph::instancesOf(classId, name)); // TODO: Make instancesOf over Hyperedges
    }
    return result;
}
Hypergraph::Hyperedges Graph::inputs(const std::string& name, const std::string& className)
{
    Hyperedges result;
    // Get all device classes
    Hyperedges classIds = inputClasses(className);
    // For each of them get the instances with the given name
    for (auto classId : classIds)
    {
        result = unite(result, CommonConceptGraph::instancesOf(classId, name)); // TODO: Make instancesOf over Hyperedges
    }
    return result;
}
Hypergraph::Hyperedges Graph::outputs(const std::string& name, const std::string& className)
{
    Hyperedges result;
    // Get all device classes
    Hyperedges classIds = outputClasses(className);
    // For each of them get the instances with the given name
    for (auto classId : classIds)
    {
        result = unite(result, CommonConceptGraph::instancesOf(classId, name)); // TODO: Make instancesOf over Hyperedges
    }
    return result;
}
Hypergraph::Hyperedges Graph::implementations(const std::string& name, const std::string& className)
{
    Hyperedges result;
    // Get all device classes
    Hyperedges classIds = implementationClasses(className);
    // For each of them get the instances with the given name
    for (auto classId : classIds)
    {
        result = unite(result, CommonConceptGraph::instancesOf(classId, name)); // TODO: Make instancesOf over Hyperedges
    }
    return result;
}
Hypergraph::Hyperedges Graph::datatypes(const std::string& name, const std::string& className)
{
    Hyperedges result;
    // Get all device classes
    Hyperedges classIds = datatypeClasses(className);
    // For each of them get the instances with the given name
    for (auto classId : classIds)
    {
        result = unite(result, CommonConceptGraph::instancesOf(classId, name)); // TODO: Make instancesOf over Hyperedges
    }
    return result;
}
Hypergraph::Hyperedges Graph::languages(const std::string& name, const std::string& className)
{
    Hyperedges result;
    // Get all device classes
    Hyperedges classIds = languageClasses(className);
    // For each of them get the instances with the given name
    for (auto classId : classIds)
    {
        result = unite(result, CommonConceptGraph::instancesOf(classId, name)); // TODO: Make instancesOf over Hyperedges
    }
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

unsigned Graph::has(const Hyperedges& algorithmIds, const Hyperedges& interfaceIds)
{
    // An algorithm class or instance can only have an interface instance
    Hyperedges fromIds = unite(intersect(this->algorithms(), algorithmIds), intersect(algorithmClasses(), algorithmIds));
    Hyperedges toIds = intersect(this->interfaces(), interfaceIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::HasAId);
    return 0;
}

unsigned Graph::provides(const Hyperedges& algorithmIds, const Hyperedges& outputIds)
{
    // An algorithm class or instance can only have an output instance
    Hyperedges fromIds = unite(intersect(this->algorithms(), algorithmIds), intersect(algorithmClasses(), algorithmIds));
    Hyperedges toIds = intersect(this->outputs(), outputIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::ProvidesId);
    return 0;
}

unsigned Graph::needs(const Hyperedges& algorithmIds, const Hyperedges& inputIds)
{
    // An algorithm class or instance can only have an output instance
    Hyperedges fromIds = unite(intersect(this->algorithms(), algorithmIds), intersect(algorithmClasses(), algorithmIds));
    Hyperedges toIds = intersect(this->inputs(), inputIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::NeedsId);
    return 0;
}

unsigned Graph::depends(const Hyperedges& inputIds, const Hyperedges& outputIds)
{
    // For now only input instances can depend on output instances
    Hyperedges fromIds = intersect(this->inputs(), inputIds);
    Hyperedges toIds = intersect(this->outputs(), outputIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::DependsOnId);
    return 0;
}

unsigned Graph::realizes(const Hyperedges& implementationIds, const Hyperedges& algorithmIds)
{
    // TODO: An algorithm class can be realized by multiple implementations ... are implementations subclasses of an algorithm? Or instances of an algorithm?
    return CommonConceptGraph::relateFrom(implementationIds, algorithmIds, Graph::RealizesId);
}

unsigned Graph::represents(const Hyperedges& datatypeIds, const Hyperedges& interfaceIds)
{
    return CommonConceptGraph::relateFrom(datatypeIds, interfaceIds, Graph::RepresentsId);
}

unsigned Graph::expressedIn(const Hyperedges& implementationOrDatatypeIds, const Hyperedges& languageIds)
{
    return CommonConceptGraph::relateFrom(implementationOrDatatypeIds, languageIds, Graph::ExpressedInId);
}

}
