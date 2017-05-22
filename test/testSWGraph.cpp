#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <fstream>
#include <iostream>
#include <cassert>

int main(void)
{
    std::cout << "*** SW GRAPH TEST ***\n";
    Software::Graph swgraph;

    std::cout << "> Create algorithm\n";
    auto firstId = swgraph.createAlgorithm("DisparityMap");
    std::cout << "First id: " << firstId << "\n";

    std::cout << "> Create input\n";
    auto secondId = swgraph.createInput("left");
    std::cout << "Second id: " << secondId << "\n";

    std::cout << "> Assign input to algorithm\n";
    swgraph.has(firstId, secondId);

    std::cout << "> Create & Assign others\n";
    swgraph.has(firstId, swgraph.createInput("right"));
    swgraph.has(firstId, swgraph.createOutput("disparity"));

    std::cout << "> Create type\n";
    auto typeId = swgraph.createType("Image");

    std::cout << "> Query inputs\n";
    for (auto inputId : swgraph.get(swgraph.inputs())->members(&swgraph)) // TODO: Nasty!
    {
        std::cout << "Input " << swgraph.get(inputId)->label() << "\n";
        swgraph.needs(inputId, typeId);
    }

    std::cout << "> Query outputs\n";
    for (auto outputId : swgraph.get(swgraph.outputs())->members(&swgraph)) // TODO: Nasty!
    {
        std::cout << "Input " << swgraph.get(outputId)->label() << "\n";
        swgraph.provides(outputId, typeId);
    }
    
    std::cout << "> All sets" << std::endl;
    auto sets = swgraph.find();
    for (auto setId : sets)
    {
        std::cout << setId << " " << swgraph.get(setId)->label() << std::endl;
    }

    std::cout << "> Store swgraph using YAML" << std::endl;

    YAML::Node test;
    test = static_cast<Hypergraph*>(&swgraph);

    std::ofstream fout;
    fout.open("swgraph.yml");
    if(fout.good()) {
        fout << test;
    } else {
        std::cout << "FAILED\n";
    }
    fout.close();


    std::cout << "*** TEST DONE ***\n";
}
