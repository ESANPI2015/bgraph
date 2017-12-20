#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <getopt.h>

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {0,0,0,0}
};

void usage (const char *myName)
{
    std::cout << "Usage:\n";
    std::cout << myName << " <yaml-file-in> <label> <yaml-file-out>\n\n";
    std::cout << "Options:\n";
    std::cout << "--help\t" << "Show usage\n";
    std::cout << "\nExample:\n";
    std::cout << myName << " myspec.yml myAlgorithm myspec2.yml\n";
}

int main (int argc, char **argv)
{

    // Parse command line
    int c;
    while (1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, "h", long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
            case 'h':
            case '?':
                break;
            default:
                std::cout << "W00t?!\n";
                return 1;
        }
    }

    if ((argc - optind) < 3)
    {
        usage(argv[0]);
        return 1;
    }

    // Set vars
    std::string fileNameIn(argv[optind]);
    std::string classLabel(argv[optind+1]);
    std::string fileNameOut(argv[optind+2]);
    Hypergraph* hypergraph = YAML::LoadFile(fileNameIn).as<Hypergraph*>();
    Conceptgraph cgraph(*hypergraph);
    CommonConceptGraph ccgraph(cgraph);
    Software::Graph swgraph(ccgraph);

    // Get all algorithm classes (without overall superclass)
    Hyperedges algorithms = swgraph.algorithmClasses(classLabel);
    algorithms.erase(Software::Graph::AlgorithmId);
    Hyperedges myLanguages = swgraph.languages("VHDL");
    Hyperedges typesOfLang = swgraph.from(intersect(swgraph.relationsOf(Hyperedges{myLanguages}), swgraph.factsOf(Software::Graph::ExpressedInId)));

    // For each of these algorithms
    for (auto algorithmId : algorithms)
    {
        std::stringstream result;
        auto algorithm = swgraph.get(algorithmId);

        result << "-- Algorithm to VHDL entity generator --\n";
        result << "library IEEE;\n";
        result << "use IEEE.STD_LOGIC_1164.ALL;\n";
        result << "\nuse work." << algorithm->label() << "_types.all;\n";
        result << "\nentity " << algorithm->label() << " is\n";
        result << "port(\n";
        Hyperedges myInterfaceClassIds;
        // Handle Inputs (input instances which are children of algorithmId
        auto myInputIds = intersect(swgraph.inputs(), swgraph.childrenOf(algorithmId));
        result << "\n\t-- Inputs --\n";
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
            result << "\t" << swgraph.get(inputId)->label() << " : in " << typeOfInput << ";\n";
            // Put input classes to the interface classes we need later
            myInterfaceClassIds.insert(interfaceIds.begin(), interfaceIds.end());
        }
        // Handle Outputs
        auto myOutputIds = intersect(swgraph.outputs(), swgraph.childrenOf(algorithmId));
        result << "\n\t-- Outputs --\n";
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
            result << "\t" << swgraph.get(outputId)->label() << " : out " << typeOfOutput << ";\n";
            myInterfaceClassIds.insert(interfaceIds.begin(), interfaceIds.end());
        }
        result << "\n\t-- Standard Signals --\n";
        result << "\tstart : in std_logic;\n";
        result << "\tvalid : out std_logic;\n";
        result << "\tclk : in std_logic;\n";
        result << "\trst : in std_logic\n";
        result << ");\n";
        result << "end\n";

        // Handle architecture
        result << "\n-- Architecture def --\n";
        result << "architecture BEHAVIOURAL of " << algorithm->label() << " is\n";
        result << "-- signals here --\n";
        result << "\nbegin\n";
        result << "-- processes here --\n";
        result << "end BEHAVIORAL;\n";

        // Handle the collected interface classes
        result << "\n-- Package def --\n";
        result << "package " << algorithm->label() << "_types is\n";
        for (auto interfaceId : myInterfaceClassIds)
        {
            auto interface = swgraph.get(interfaceId);
            std::string datatypeName;
            Hyperedges typesOfIf = swgraph.from(intersect(swgraph.relationsOf(Hyperedges{interfaceId}), swgraph.factsOf(Software::Graph::RepresentsId)));
            Hyperedges dataTypes = intersect(typesOfIf, typesOfLang);
            for (auto typeId : dataTypes)
            {
                datatypeName = swgraph.get(typeId)->label();
                result << "\ttype " << interface->label() << " is " << datatypeName << ";\n";
            }
            if (dataTypes.empty())
                result << "\ttype " << interface->label() << " is UNKNOWN;\n";
        }
        result << "end " << algorithm->label() << "_types;\n";

        // Create implementation for algorithm with this result
        Hyperedges implId = swgraph.createImplementation(result.str());
        swgraph.realizes(implId, Hyperedges{algorithmId});
        swgraph.expressedIn(implId, myLanguages);
    }

    // Store modified graph
    std::ofstream fout;
    fout.open(fileNameOut);
    if(!fout.good()) {
        std::cout << "FAILED\n";
    }
    YAML::Node test;
    test = static_cast<Hypergraph*>(&swgraph);
    fout << test;
    fout.close();

    return 0;
}