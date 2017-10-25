#include "SoftwareGraph.hpp"

namespace Software {


// Concept Ids
const UniqueId Graph::InterfaceId       = "Software::Graph::Interface";
const UniqueId Graph::InputId           = "Software::Graph::Input";
const UniqueId Graph::OutputId          = "Software::Graph::Output";
const UniqueId Graph::AlgorithmId       = "Software::Graph::Algorithm";
const UniqueId Graph::ImplementationId  = "Software::Graph::Implementation";
const UniqueId Graph::DatatypeId        = "Software::Graph::Datatype";
const UniqueId Graph::LanguageId        = "Software::Graph::Language";
// Relation Ids
const UniqueId Graph::IsAId             = CommonConceptGraph::IsAId;
const UniqueId Graph::HasAId            = "Software::Graph::HasA";
const UniqueId Graph::DependsOnId       = "Software::Graph::DependsOn";
const UniqueId Graph::NeedsId           = "Software::Graph::Needs";
const UniqueId Graph::ProvidesId        = "Software::Graph::Provides";
const UniqueId Graph::ExpressedInId     = "Software::Graph::Expressed";
const UniqueId Graph::RealizesId        = "Software::Graph::Realizes";
const UniqueId Graph::RepresentsId      = "Software::Graph::Represents";

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
    Conceptgraph::relate(Graph::HasAId, Hyperedges{Graph::AlgorithmId}, Hyperedges{Graph::InterfaceId}, "HAS-A");
    CommonConceptGraph::subrelationOf(Graph::HasAId, CommonConceptGraph::HasAId);

    Conceptgraph::relate(Graph::DependsOnId, Hyperedges{Graph::InputId}, Hyperedges{Graph::OutputId}, "DEPENDS-ON");
    CommonConceptGraph::subrelationOf(Graph::DependsOnId, CommonConceptGraph::ConnectsId);

    Conceptgraph::relate(Graph::NeedsId, Hyperedges{Graph::AlgorithmId}, Hyperedges{Graph::InputId}, "NEEDS");
    CommonConceptGraph::subrelationOf(Graph::NeedsId, Graph::HasAId);

    Conceptgraph::relate(Graph::ProvidesId, Hyperedges{Graph::AlgorithmId}, Hyperedges{Graph::OutputId}, "PROVIDES");
    CommonConceptGraph::subrelationOf(Graph::ProvidesId, Graph::HasAId);

    Conceptgraph::relate(Graph::ExpressedInId, Hyperedges{Graph::ImplementationId}, Hyperedges{Graph::LanguageId}, "EXPRESSED-IN");
    Conceptgraph::get(Graph::ExpressedInId)->from(Graph::DatatypeId); // OK :)
    // CommonConceptGraph::subrelationOf(Graph::ExpressedInId, CommonConceptGraph::PartOfId); // TODO: True?

    Conceptgraph::relate(Graph::RealizesId, Hyperedges{Graph::ImplementationId}, Hyperedges{Graph::AlgorithmId}, "REALIZES");
    CommonConceptGraph::subrelationOf(Graph::RealizesId, CommonConceptGraph::IsAId); // Every Implementation is also an Algorithm
    Conceptgraph::relate(Graph::RepresentsId, Hyperedges{Graph::DatatypeId}, Hyperedges{Graph::InterfaceId}, "REPRESENTS");
    CommonConceptGraph::subrelationOf(Graph::RepresentsId, CommonConceptGraph::IsAId); // Every Datatype is also a (Software)Interface
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

Hyperedges Graph::algorithmClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::AlgorithmId, name);
    return result;
}

Hyperedges Graph::interfaceClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::InterfaceId, name);
    return result;
}

Hyperedges Graph::inputClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::InputId, name);
    return result;
}

Hyperedges Graph::outputClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::OutputId, name);
    return result;
}

Hyperedges Graph::implementationClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::ImplementationId, name);
    return result;
}

Hyperedges Graph::datatypeClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::DatatypeId, name);
    return result;
}

Hyperedges Graph::languageClasses(const std::string& name)
{
    Hyperedges result = CommonConceptGraph::subclassesOf(Graph::LanguageId, name);
    return result;
}

Hyperedges Graph::algorithms(const std::string& name, const std::string& className)
{
    // Get all super classes
    Hyperedges classIds = algorithmClasses(className);
    // ... and then the instances of them
    return CommonConceptGraph::instancesOf(classIds, name);
}
Hyperedges Graph::interfaces(const std::string& name, const std::string& className)
{
    // Get all super classes
    Hyperedges classIds = interfaceClasses(className);
    // ... and then the instances of them
    return CommonConceptGraph::instancesOf(classIds, name);
}
Hyperedges Graph::inputs(const std::string& name, const std::string& className)
{
    // Get all super classes
    Hyperedges classIds = inputClasses(className);
    // ... and then the instances of them
    return CommonConceptGraph::instancesOf(classIds, name);
}
Hyperedges Graph::outputs(const std::string& name, const std::string& className)
{
    // Get all super classes
    Hyperedges classIds = outputClasses(className);
    // ... and then the instances of them
    return CommonConceptGraph::instancesOf(classIds, name);
}
Hyperedges Graph::implementations(const std::string& name, const std::string& className)
{
    // Get all super classes
    Hyperedges classIds = implementationClasses(className);
    // ... and then the instances of them
    return CommonConceptGraph::instancesOf(classIds, name);
}
Hyperedges Graph::datatypes(const std::string& name, const std::string& className)
{
    // Get all super classes
    Hyperedges classIds = datatypeClasses(className);
    // ... and then the instances of them
    return CommonConceptGraph::instancesOf(classIds, name);
}
Hyperedges Graph::languages(const std::string& name, const std::string& className)
{
    // Get all super classes
    Hyperedges classIds = languageClasses(className);
    // ... and then the instances of them
    return CommonConceptGraph::instancesOf(classIds, name);
}

Hyperedges Graph::createAlgorithm(const std::string& name)
{
    Hyperedges a = create(name);
    CommonConceptGraph::isA(a, Graph::AlgorithmId);
    return a;
}

Hyperedges Graph::createInput(const UniqueId interfaceId, const std::string& name)
{
    return createInput(Hyperedges{interfaceId}, name);
}

Hyperedges Graph::createOutput(const UniqueId interfaceId, const std::string& name)
{
    return createOutput(Hyperedges{interfaceId}, name);
}

Hyperedges Graph::createInput(const Hyperedges& interfaceIds, const std::string& name)
{
    // TODO: One input which belongs to all interface classes or a new input for each interfaceId?
    Hyperedges a = create(name);
    CommonConceptGraph::isA(a, Graph::InputId);
    CommonConceptGraph::isA(a, interfaceIds);
    return a;
}

Hyperedges Graph::createOutput(const Hyperedges& interfaceIds, const std::string& name)
{
    Hyperedges a = create(name);
    CommonConceptGraph::isA(a, Graph::OutputId);
    CommonConceptGraph::isA(a, interfaceIds);
    return a;
}

Hyperedges Graph::createInterface(const std::string& name)
{
    Hyperedges a = create(name);
    CommonConceptGraph::isA(a, Graph::InterfaceId);
    return a;
}

Hyperedges Graph::createImplementation(const std::string& name)
{
    Hyperedges a = create(name);
    CommonConceptGraph::isA(a, Graph::ImplementationId);
    return a;
}

Hyperedges Graph::createDatatype(const std::string& name)
{
    Hyperedges a = create(name);
    CommonConceptGraph::isA(a, Graph::DatatypeId);
    return a;
}

Hyperedges Graph::createLanguage(const std::string& name)
{
    Hyperedges a = create(name);
    CommonConceptGraph::isA(a, Graph::LanguageId);
    return a;
}

Hyperedges Graph::instantiateInput(const UniqueId superId, const std::string& name)
{
    return instantiateInput(Hyperedges{superId}, name);
}

Hyperedges Graph::instantiateOutput(const UniqueId superId, const std::string& name)
{
    return instantiateOutput(Hyperedges{superId}, name);
}

Hyperedges Graph::instantiateInput(const Hyperedges& superIds, const std::string& name)
{
    Hyperedges id = CommonConceptGraph::instantiateFrom(superIds, name);
    // TODO:
    // needs(deviceId, id);
    return id;
}

Hyperedges Graph::instantiateOutput(const Hyperedges& superIds, const std::string& name)
{
    Hyperedges id = CommonConceptGraph::instantiateFrom(superIds, name);
    // TODO:
    // provides(deviceId, id);
    return id;
}

Hyperedges Graph::has(const Hyperedges& algorithmIds, const Hyperedges& interfaceIds)
{
    // An algorithm class or instance can only have an interface instance
    Hyperedges fromIds = unite(intersect(this->algorithms(), algorithmIds), intersect(algorithmClasses(), algorithmIds));
    Hyperedges toIds = intersect(this->interfaces(), interfaceIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::HasAId);
    return Hyperedges();
}

Hyperedges Graph::provides(const Hyperedges& algorithmIds, const Hyperedges& outputIds)
{
    // An algorithm class or instance can only have an output instance
    Hyperedges fromIds = unite(intersect(this->algorithms(), algorithmIds), intersect(algorithmClasses(), algorithmIds));
    Hyperedges toIds = intersect(this->outputs(), outputIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::ProvidesId);
    return Hyperedges();
}

Hyperedges Graph::needs(const Hyperedges& algorithmIds, const Hyperedges& inputIds)
{
    // An algorithm class or instance can only have an output instance
    Hyperedges fromIds = unite(intersect(this->algorithms(), algorithmIds), intersect(algorithmClasses(), algorithmIds));
    Hyperedges toIds = intersect(this->inputs(), inputIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::NeedsId);
    return Hyperedges();
}

Hyperedges Graph::depends(const Hyperedges& inputIds, const Hyperedges& outputIds)
{
    // For now only input instances can depend on output instances
    Hyperedges fromIds = intersect(this->inputs(), inputIds);
    Hyperedges toIds = intersect(this->outputs(), outputIds);
    if (fromIds.size() && toIds.size())
        return CommonConceptGraph::relateFrom(fromIds, toIds, Graph::DependsOnId);
    return Hyperedges();
}

Hyperedges Graph::realizes(const Hyperedges& implementationIds, const Hyperedges& algorithmIds)
{
    // NOTE: Implementations are true subclasses of algorithm. While every implementation is an algorithm, not every algorithm is also an implementation/program
    return CommonConceptGraph::relateFrom(implementationIds, algorithmIds, Graph::RealizesId);
}

Hyperedges Graph::represents(const Hyperedges& datatypeIds, const Hyperedges& interfaceIds)
{
    return CommonConceptGraph::relateFrom(datatypeIds, interfaceIds, Graph::RepresentsId);
}

Hyperedges Graph::expressedIn(const Hyperedges& implementationOrDatatypeIds, const Hyperedges& languageIds)
{
    return CommonConceptGraph::relateFrom(implementationOrDatatypeIds, languageIds, Graph::ExpressedInId);
}

}
