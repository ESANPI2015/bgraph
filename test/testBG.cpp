#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <fstream>
#include <iostream>

int main(void)
{
    Hypergraph* hypergraph = YAML::LoadFile("def_bg.yml").as<Hypergraph*>();
    Conceptgraph cgraph(*hypergraph);
    CommonConceptGraph ccgraph(cgraph);
    Software::Graph swgraph(ccgraph);

    // Lets instantiate a small network
    // First we say, that we want a algorithm SUM connected to algorithm DIV
    auto sumClass = swgraph.algorithmClasses("SUM");
    auto divClass = swgraph.algorithmClasses("DIVIDE");
    std::cout << "#SUM: " << sumClass.size() << " #DIV: " << divClass.size() << "\n";
    // Get also all inputs & outputs
    auto sumClassOutputs = intersect(swgraph.childrenOf(sumClass), swgraph.outputs());
    auto divClassInputs = intersect(swgraph.childrenOf(divClass), swgraph.inputs());
    std::cout << "#SUM Out: " << sumClassOutputs.size() << " #DIV In: " << divClassInputs.size() << "\n";

    // Now, instantiate a SUM and a DIV
    auto sum = swgraph.instantiateFrom(sumClass);
    auto div = swgraph.instantiateFrom(divClass);

    // TODO: Test if we have now both in algorithms() result!!!
    std::cout << "Algorithms: " << swgraph.algorithms() << "\n";

    // For each of the inputs/outputs: find superclass and instantiate from that
    Hyperedges sumOutputs;
    for (auto sumClassOutputId : sumClassOutputs)
    {
        std::string label = swgraph.get(sumClassOutputId)->label();
        // instantiate also all sumClassOutputs and connect them to sum instance
        auto sumClassOutputClass = swgraph.instancesOf(sumClassOutputId,"", CommonConceptGraph::TraversalDirection::DOWN);
        sumOutputs = unite(sumOutputs, swgraph.instantiateFrom(sumClassOutputClass, label));
    }
    swgraph.provides(sum, sumOutputs);
    // the same with div but now only inputs
    Hyperedges divInputs;
    for (auto divClassInputId : divClassInputs)
    {
        std::string label = swgraph.get(divClassInputId)->label();
        // instantiate also all divClassInputs and connect them to sum instance
        auto divClassInputClass = swgraph.instancesOf(divClassInputId,"", CommonConceptGraph::TraversalDirection::DOWN);
        divInputs = unite(divInputs, swgraph.instantiateFrom(divClassInputClass, label));
    }
    swgraph.needs(div, divInputs);

    // Finally, we create the network!
    swgraph.depends(divInputs, sumOutputs);

    // Store the graph
    YAML::Node test;
    test = static_cast<Hypergraph*>(&swgraph);

    std::ofstream fout;
    fout.open("net_bg.yml");
    if(fout.good()) {
        fout << test;
    } else {
        std::cout << "FAILED\n";
    }
    fout.close();

    return 0;
}
