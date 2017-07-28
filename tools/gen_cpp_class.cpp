#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <iostream>
#include <sstream>
#include <cassert>

int main(void)
{

    // TODO:
    // * Import software spec
    // * Search for all algorithms
    // * Create a class header for each algorithm

    // Test case:
    auto hypergraph = YAML::LoadFile("swgraph.yml").as<Hypergraph*>();
    Conceptgraph cgraph(*hypergraph);
    CommonConceptGraph ccgraph(cgraph);
    Software::Graph swgraph(ccgraph);
    // Get all algorithms
    auto algorithms = swgraph.algorithms();
    // For each of these algorithms
    for (auto algorithmId : algorithms)
    {
        std::stringstream result;
        auto algorithm = swgraph.get(algorithmId);
        result << "// Algorithm to C++ generator\n";
        result << "class " << algorithm->label() << " {\n";
        result << "\tpublic:\n";
        // The interfaces of the device are in the intersection of allInterfaceIds and allChildrenOfDev TODO: without Inputs/Outputs
        auto myInterfaceIds = swgraph.intersect(swgraph.interfaces(), swgraph.childrenOf(algorithmId));
        result << "\t\t// Generate interface types\n";
        for (auto interfaceId : myInterfaceIds)
        {
            auto interface = swgraph.get(interfaceId);
            std::string datatypeName = "UNKNOWN";
            // TODO: Find datatype
            result << "\t\ttypedef " << datatypeName << " " << interface->label() << ";\n";
        }
        // Handle Inputs
        auto myInputIds = swgraph.intersect(swgraph.inputs(), swgraph.childrenOf(algorithmId));
        result << "\n\t\t// Generate functions to retrieve input values\n";
        for (auto inputId : myInputIds)
        {
            auto interfaceIds = swgraph.intersect(swgraph.interfaces(), swgraph.to(swgraph.intersect(swgraph.relationsFrom(inputId),swgraph.factsOf(Software::Graph::IsAId)))); //superclassesOf
            std::string typeOfInput = "UNDEFINED";
            if (interfaceIds.size() > 1)
            {
                std::cerr << "Multiple interfaces for input " << swgraph.get(inputId)->label() << "\n";
                return 1;
            }
            if (interfaceIds.size() == 1)
            {
                typeOfInput = swgraph.get(*interfaceIds.begin())->label();
            }
            result << "\t\tbool read_" << swgraph.get(inputId)->label() << "(" << typeOfInput << "& value);\n";
        }
        // Handle Outputs
        auto myOutputIds = swgraph.intersect(swgraph.outputs(), swgraph.childrenOf(algorithmId));
        result << "\n\t\t// Generate functions to write output values\n";
        for (auto outputId : myOutputIds)
        {
            auto interfaceIds = swgraph.intersect(swgraph.interfaces(), swgraph.to(swgraph.intersect(swgraph.relationsFrom(outputId),swgraph.factsOf(Software::Graph::IsAId)))); //superclassesOf
            std::string typeOfOutput = "UNDEFINED";
            if (interfaceIds.size() > 1)
            {
                std::cerr << "Multiple interfaces for output " << swgraph.get(outputId)->label() << "\n";
                return 1;
            }
            if (interfaceIds.size() == 1)
            {
                typeOfOutput = swgraph.get(*interfaceIds.begin())->label();
            }
            result << "\t\tbool write_" << swgraph.get(outputId)->label() << "(const " << typeOfOutput << "& value);\n";
        }
        // Insert evaluation method
        result << "\n\t\t// Evaluation method\n";
        result << "\t\tbool evaluate();\n";
        result << "};\n";
        std::cout << result.str();
    }
    return 0;
}
