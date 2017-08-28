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

    std::cout << "> Create image interface\n";
    auto imageId = swgraph.createInterface("Image");

    std::cout << "> Create image input\n";
    auto secondId = swgraph.createInput(imageId, "Image Input");

    std::cout << "> Create image output\n";
    auto outputId = swgraph.createOutput(imageId, "Image Output");

    std::cout << "> Instantiate inputs & outputs\n";
    auto leftId = swgraph.instantiateFrom(secondId, "left");
    auto rightId = swgraph.instantiateFrom(secondId, "right");
    auto dispId = swgraph.instantiateFrom(outputId, "disparity");

    std::cout << "> Assign inputs and outputs to algorithm\n";
    swgraph.needs(firstId, unite(leftId, rightId));
    swgraph.provides(firstId, dispId);

    std::cout << "> Query input classes\n";
    for (auto inputId : swgraph.inputClasses())
    {
        std::cout << "Input " << swgraph.get(inputId)->label() << "\n";
    }

    std::cout << "> Query output classes\n";
    for (auto outputId : swgraph.outputClasses())
    {
        std::cout << "Output " << swgraph.get(outputId)->label() << "\n";
    }

    std::cout << "> Query interface classes\n";
    for (auto interfaceId : swgraph.interfaceClasses())
    {
        std::cout << "Interface " << swgraph.get(interfaceId)->label() << "\n";
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
