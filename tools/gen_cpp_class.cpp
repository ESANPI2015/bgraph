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
    Software::Graph swgraph(cgraph);
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
        // Get all interfaces
        auto allInterfaceIds = swgraph.interfaces();
        // Get all "HAS" relations of device
        auto allHasRelationsOfAlg = swgraph.relationsOf(algorithmId, swgraph.get(Software::Graph::HasAId)->label());
        // Get all the concepts the "HAS" relations point to
        auto allChildrenOfAlg = swgraph.to(allHasRelationsOfAlg);
        // The interfaces of the device are in the intersection of allInterfaceIds and allChildrenOfDev
        auto myInterfaceIds = swgraph.intersect(allInterfaceIds, allChildrenOfAlg);
        result << "\t\t// Generate interface types\n";
        for (auto interfaceId : myInterfaceIds)
        {
            auto interface = swgraph.get(interfaceId);
            std::string datatypeName = "UNKNOWN";
            // TODO: Find datatype
            result << "\t\ttypedef " << datatypeName << " " << interface->label() << ";\n";
        }
        // Handle Inputs
        auto myInputIds = swgraph.to(swgraph.relationsOf(algorithmId, swgraph.get(Software::Graph::NeedsId)->label()));
        result << "\n\t\t// Generate functions to retrieve input values\n";
        for (auto inputId : myInputIds)
        {
            auto interfaceIds = swgraph.intersect(allInterfaceIds, swgraph.to(swgraph.relationsOf(inputId, swgraph.get(Software::Graph::IsAId)->label())));
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
        auto myOutputIds = swgraph.to(swgraph.relationsOf(algorithmId, swgraph.get(Software::Graph::ProvidesId)->label()));
        result << "\n\t\t// Generate functions to write output values\n";
        for (auto outputId : myOutputIds)
        {
            auto interfaceIds = swgraph.intersect(allInterfaceIds, swgraph.to(swgraph.relationsOf(outputId, swgraph.get(Software::Graph::IsAId)->label())));
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
