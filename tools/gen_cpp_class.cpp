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

    // Get all algorithm classes (without overall superclass)
    auto algorithms = swgraph.algorithmClasses();
    algorithms.erase(Software::Graph::AlgorithmId);

    // For each of these algorithms
    for (auto algorithmId : algorithms)
    {
        std::stringstream result;
        auto algorithm = swgraph.get(algorithmId);
        result << "// Algorithm to C++ generator\n";
        result << "class " << algorithm->label() << " {\n";
        result << "\tpublic:\n";
        Hyperedges myInterfaceClassIds;
        // Handle Inputs (input instances which are children of algorithmId
        auto myInputIds = intersect(swgraph.inputs(), swgraph.childrenOf(algorithmId));
        result << "\n\t\t// Generate functions to retrieve input values\n";
        for (auto inputId : myInputIds)
        {
            // This input is an INSTANCE-OF some CLASS X which is a subtype of an INTERFACE SUBCLASS and the INPUT CLASS.
            // To get the INTERFACE SUBCLASS, we have to get rid of CLASS X, INPUT CLASS and INTERFACE CLASS.
            auto interfaceIds = subtract(swgraph.subclassesOf(swgraph.instancesOf(inputId,"",Hypergraph::TraversalDirection::DOWN),"",Hypergraph::TraversalDirection::DOWN), swgraph.inputClasses());
            interfaceIds.erase(Software::Graph::InterfaceId);
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
            // Put input classes to the interface classes we need later
            myInterfaceClassIds.insert(interfaceIds.begin(), interfaceIds.end());
        }
        // Handle Outputs
        auto myOutputIds = intersect(swgraph.outputs(), swgraph.childrenOf(algorithmId));
        result << "\n\t\t// Generate functions to write output values\n";
        for (auto outputId : myOutputIds)
        {
            // This input is an INSTANCE-OF some CLASS X which is a subtype of an INTERFACE SUBCLASS and the INPUT CLASS.
            // To get the INTERFACE SUBCLASS, we have to get rid of CLASS X, INPUT CLASS and INTERFACE CLASS.
            auto interfaceIds = subtract(swgraph.subclassesOf(swgraph.instancesOf(outputId,"",Hypergraph::TraversalDirection::DOWN),"",Hypergraph::TraversalDirection::DOWN), swgraph.outputClasses());
            interfaceIds.erase(Software::Graph::InterfaceId);
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
            myInterfaceClassIds.insert(interfaceIds.begin(), interfaceIds.end());
        }

        // Handle the collected interface classes
        result << "\n\t\t// Generate interface types\n";
        for (auto interfaceId : myInterfaceClassIds)
        {
            auto interface = swgraph.get(interfaceId);
            std::string datatypeName = "UNKNOWN";
            // TODO: Find datatype. There can be multiple!!!
            result << "\t\ttypedef " << datatypeName << " " << interface->label() << ";\n";
        }
        // Insert evaluation method
        result << "\n\t\t// Evaluation method\n";
        result << "\t\tbool evaluate();\n";
        result << "};\n";
        std::cout << result.str();
    }
    return 0;
}
