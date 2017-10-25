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

// For each algorithm:
// * Create interfaces & types
// * Check parts
//   - If no parts, create atomic class
//   - if parts, call each one in topological order/in BFS order from inputs to outputs TODO
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
    Hyperedges myLanguages = swgraph.languages("C");
    Hyperedges typesOfLang = swgraph.from(intersect(swgraph.relationsOf(myLanguages), swgraph.factsOf(Software::Graph::ExpressedInId)));

    // For each of these algorithms
    for (auto algorithmId : algorithms)
    {
        std::stringstream result;
        auto algorithm = swgraph.get(algorithmId);

        // Get all properties needed for code generation
        // TODO: Do we want transitive closure? Actually we want to REUSE code from parts of parts, right?
        Hyperedges parts = swgraph.partsOf(Hyperedges{algorithmId});
        parts.erase(algorithmId);
        Hyperedges children = swgraph.childrenOf(algorithmId);
        children.erase(algorithmId);
        Hyperedges inputs = intersect(children, swgraph.inputs());
        Hyperedges outputs = intersect(children, swgraph.outputs());
        Hyperedges allInterfaces;
        std::map< UniqueId, Hyperedges > interfacesOfPort;
        for (UniqueId inputId : inputs)
        {
            Hyperedges interfaces;
            interfaces = swgraph.subclassesOf(swgraph.instancesOf(inputId,"",Hypergraph::TraversalDirection::DOWN),"",Hypergraph::TraversalDirection::DOWN);
            interfaces = subtract(interfaces, swgraph.inputClasses());
            interfaces.erase(Software::Graph::InterfaceId);
            interfacesOfPort[inputId] = interfaces;
            allInterfaces.insert(interfaces.begin(), interfaces.end());
        }
        for (UniqueId outputId : outputs)
        {
            Hyperedges interfaces;
            interfaces = swgraph.subclassesOf(swgraph.instancesOf(outputId,"",Hypergraph::TraversalDirection::DOWN),"",Hypergraph::TraversalDirection::DOWN);
            interfaces = subtract(interfaces, swgraph.outputClasses());
            interfaces.erase(Software::Graph::InterfaceId);
            interfacesOfPort[outputId] = interfaces;
            allInterfaces.insert(interfaces.begin(), interfaces.end());
        }

        // PREAMBLE
        result << "// Algorithm to C++ generator\n";
        result << "#ifndef __" << algorithm->label() << "_HEADER\n";
        result << "#define __" << algorithm->label() << "_HEADER\n";
        result << "class " << algorithm->label() << " {\n";
        result << "\tpublic:\n";

        // Handle the collected interface classes
        result << "\n\t\t// Generate interface types\n";
        for (auto interfaceId : allInterfaces)
        {
            auto interface = swgraph.get(interfaceId);
            std::string datatypeName;
            Hyperedges typesOfIf = swgraph.from(intersect(swgraph.relationsOf(Hyperedges{interfaceId}), swgraph.factsOf(Software::Graph::RepresentsId)));
            Hyperedges dataTypes = intersect(typesOfIf, typesOfLang);
            for (auto typeId : dataTypes)
            {
                datatypeName = swgraph.get(typeId)->label();
                result << "\t\ttypedef " << datatypeName << " " << interface->label() << ";\n";
            }
            if (dataTypes.empty())
                result << "\t\ttypedef UNKNOWN " << interface->label() << ";\n";
        }

        // Instantiate parts (to not confuse C++, partId is also included)
        result << "\n\t\t// Instantiate parts\n";
        for (UniqueId partId : parts)
        {
            Hyperedges superclasses = swgraph.instancesOf(partId,"",Hypergraph::TraversalDirection::DOWN);
            result << "\t\t" << swgraph.get(*superclasses.begin())->label() << " " << swgraph.get(partId)->label() << partId << "\n";
        }

        // Generate main function signature
        result << "\n\t\t// Generate function\n";
        result << "\t\tbool operator () (\n";
        for (auto inputId : inputs)
        {
            // This input is an INSTANCE-OF some CLASS X which is a subtype of an INTERFACE SUBCLASS and the INPUT CLASS.
            // To get the INTERFACE SUBCLASS, we have to get rid of CLASS X, INPUT CLASS and INTERFACE CLASS.
            Hyperedges interfaceIds = interfacesOfPort[inputId];
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
            result << "\t\t\tconst " << typeOfInput << "& " << swgraph.get(inputId)->label() << ",\n";
        }
        // Handle Outputs
        for (auto outputId : outputs)
        {
            // This input is an INSTANCE-OF some CLASS X which is a subtype of an INTERFACE SUBCLASS and the INPUT CLASS.
            // To get the INTERFACE SUBCLASS, we have to get rid of CLASS X, INPUT CLASS and INTERFACE CLASS.
            Hyperedges interfaceIds = interfacesOfPort[outputId];
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
            result << "\t\t\t" << typeOfOutput << "& " << swgraph.get(outputId)->label() << ",\n";
        }
        // Create evaluation function
        result << "\t\t\tvoid *ctx)\n";
        result << "\t\t{\n";
        if (!parts.size())
        {
            // Generate a dummy
            result << "\t\t\t// Implement atomic algorithm here\n";
            result << "\t\t\treturn false;\n";
        } else {
            // TODO: How do we call the parts in the correct way?
            // a) start with a queue of all input parts (initial)
            // b) then perform BFS and whenever we visit a node we call it, passing values from previous nodes and storing results
            for (auto partId : parts)
            {
                result << "\t\t\t//Call " << swgraph.get(partId)->label() << partId << "\n";
            }
        }
        result << "\t\t}\n";

        // Close class def
        result << "};\n";
        result << "#endif\n";

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
