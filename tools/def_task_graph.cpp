#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <fstream>
#include <sstream>

int main(void)
{
    // NOTE: Everything in here could be a constructor of a derived class!
    Software::Graph swgraph;

    // Some constants
    // Supported Languages
    auto cId = swgraph.createLanguage("C");

    // Upper classes
    auto taskClassId = swgraph.createAlgorithm("system_modelling::task_graph::Task"); // Implementation?
    auto networkClassId = swgraph.createAlgorithm("system_modelling::task_graph::Network");
    auto transportClassId = swgraph.createAlgorithm("system_modelling::task_graph::PortConnection"); // Implementation?
    auto portClassId = swgraph.createInterface("system_modelling::task_graph::Port");
    auto inputClassId = swgraph.createInput(portClassId, "system_modelling::task_graph::InputPort");
    auto outputClassId = swgraph.createOutput(portClassId, "system_modelling::task_graph::OutputPort");
    auto dataTypeId = swgraph.createDatatype("system_modelling::graph_basics::DataType");
    // TODO: We need a class which holds all UNMODELED stuff e.g. configurations/annotations etc.
    // In system_modelling this is modeled as a TREE OF PROPERTY NODES with DATATYPE and DATAVALUE nodes as leafs!!!
    // In our model such CONFIGURATIONs should be INTERFACES which already have DEFAULT VALUES (also a TODO!)
    // We could have an ALGORITHM which assigns CONFIGURATIONS for other ALGORITHMs :D

    // Now the language def
    swgraph.isA(networkClassId, taskClassId); // Every network is a task on its own
    // A port connection has one input and one output: It is a channel
    swgraph.needs(transportClassId, swgraph.instantiateInput(inputClassId,"IN"));
    swgraph.provides(transportClassId, swgraph.instantiateOutput(outputClassId,"OUT"));
    swgraph.represents(dataTypeId, portClassId);
    swgraph.expressedIn(dataTypeId, cId);

    // Upper relations
    // PortConnection -- ConnectsTo --> OutputPort  =   PortConnection.InputPort <-- dependsOn --> OutputPort
    // PortConnection -- ConnectsTo --> InputPort   =   InputPort <-- dependsOn --> PortConnection.OutputPort
    auto taskHasPortId = swgraph.relate(taskClassId, portClassId, "system_modelling::graph_basics::Has");
    auto taskInstanceOfTemplateId = swgraph.relate(taskClassId, taskClassId, "system_modelling::graph_basics::InstanceOf");
    auto transportConnectsPortId = swgraph.relate(transportClassId, portClassId, "system_modelling::graph_basics::ConnectsTo");
    auto taskIsPartOfId = swgraph.relate(taskClassId, networkClassId, "system_modelling::graph_basics::PartOf");
    auto taskContainsId = swgraph.relate(networkClassId, taskClassId, "system_modelling::graph_basics::Contains");
    auto portHasUniqueTypeId = swgraph.relate(portClassId, dataTypeId, "system_modelling::graph_basics::HasUnique");

    // Refer new relations to our common and/or special relations!
    swgraph.subrelationOf(taskHasPortId, Hyperedges{Software::Graph::HasAId});
    // FIXME: This is the only way ... Otherwise we have to treat instances of datatype as classes!
    // One way to do it (once we have an inference engine is to say: Whenever a port has a type, transform it to port is-of type!
    swgraph.subrelationOf(portHasUniqueTypeId, Hyperedges{CommonConceptGraph::HasAId});
    swgraph.subrelationOf(taskInstanceOfTemplateId, Hyperedges{CommonConceptGraph::InstanceOfId});
    swgraph.subrelationOf(transportConnectsPortId, Hyperedges{CommonConceptGraph::ConnectsId});
    swgraph.subrelationOf(taskIsPartOfId, Hyperedges{CommonConceptGraph::PartOfId});
    // TODO: What do we do about Contains?

    // Is that all?
    // Now we can load stuff, right?

    // Store the setup in a YAML for inspection
    YAML::Node test;
    test = static_cast<Hypergraph*>(&swgraph);

    std::ofstream fout;
    fout.open("def_task_graph.yml");
    if(fout.good()) {
        fout << test;
    } else {
        std::cout << "FAILED\n";
    }
    fout.close();
}
