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

    std::cout << "> Create interface\n";
    auto imageId = swgraph.createInterface("Image");

    std::cout << "> Create input\n";
    auto secondId = swgraph.createInput(imageId, "left");
    std::cout << "Second id: " << secondId << "\n";

    std::cout << "> Assign input to algorithm\n";
    swgraph.needs(firstId, secondId);

    std::cout << "> Create & Assign others\n";
    swgraph.needs(firstId, swgraph.createInput(imageId, "right"));
    swgraph.provides(firstId, swgraph.createOutput(imageId, "disparity"));

    std::cout << "> Query inputs\n";
    for (auto inputId : swgraph.inputs())
    {
        std::cout << "Input " << swgraph.get(inputId)->label() << "\n";
    }

    std::cout << "> Query outputs\n";
    for (auto outputId : swgraph.outputs())
    {
        std::cout << "Output " << swgraph.get(outputId)->label() << "\n";
    }
    
    std::cout << "> All concepts" << std::endl;
    auto concepts = swgraph.find();
    for (auto conceptId : concepts)
    {
        std::cout << conceptId << " " << swgraph.get(conceptId)->label() << std::endl;
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
