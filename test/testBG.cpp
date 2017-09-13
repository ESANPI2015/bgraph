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

    auto sumClass = swgraph.algorithmClasses("SUM");
    auto divClass = swgraph.algorithmClasses("DIVIDE");

    // Deep instantiation of a sum and a div algorithm
    auto sum = swgraph.instantiateDeepFrom(sumClass);
    auto div = swgraph.instantiateDeepFrom(divClass);

    std::cout << "Children of DIV: " << swgraph.childrenOf(div) << "\n";

    // Conncet the merged output of sum with the x input of div
    swgraph.depends(swgraph.childrenOf(div,"x"), swgraph.childrenOf(sum,"merged"));

    // Create a new class of algorithm which is a subtype of SUBGRAPH
    swgraph.isA(swgraph.createAlgorithm("myFirstSubgraph"), swgraph.algorithmClasses("SUBGRAPH"));
    swgraph.partOf(sum, swgraph.algorithmClasses("myFirstSubgraph"));
    swgraph.partOf(div, swgraph.algorithmClasses("myFirstSubgraph"));

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
