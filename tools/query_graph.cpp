#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <iostream>
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
    std::cout << myName << " <yaml-file>\n\n";
    std::cout << "Options:\n";
    std::cout << "--help\t" << "Show usage\n";
    std::cout << "\nExample:\n";
    std::cout << myName << " myspec.yml\n";
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

    if ((argc - optind) < 1)
    {
        usage(argv[0]);
        return 1;
    }

    // Load graph
    std::string fileName(argv[optind]);
    //std::string classLabel(argv[optind+1]);
    Hypergraph* hypergraph = YAML::LoadFile(fileName).as<Hypergraph*>();
    Conceptgraph cgraph(*hypergraph);
    CommonConceptGraph ccgraph(cgraph);
    Software::Graph swgraph(ccgraph);

    // List all classes
    std::cout << "Classes:\n";
    Hyperedges algorithmClasses = swgraph.algorithmClasses();
    if (algorithmClasses.size())
    {
        std::cout << "\tAlgorithms:\n";
        for (auto id : algorithmClasses)
            std::cout << "\t\t" << swgraph.get(id)->label() << "\n";
    }
    //Hyperedges interfaceClasses(const std::string& name="");
    //Hyperedges inputClasses(const std::string& name="");
    //Hyperedges outputClasses(const std::string& name="");
    //Hyperedges implementationClasses(const std::string& name="");
    //Hyperedges datatypeClasses(const std::string& name="");
    //Hyperedges languageClasses(const std::string& name="");
    std::cout << "Instances:\n";
    Hyperedges algorithms = swgraph.algorithms();;
    if (algorithms.size())
    {
        std::cout << "\tAlgorithms:\n";
        for (auto id : algorithms)
            std::cout << "\t\t" << swgraph.get(id)->label() << "\n";
    }
    //Hyperedges interfaces(const std::string& name="", const std::string& className="");
    //Hyperedges inputs(const std::string& name="", const std::string& className="");
    //Hyperedges outputs(const std::string& name="", const std::string& className="");
    //Hyperedges implementations(const std::string& name="", const std::string& className="");
    //Hyperedges datatypes(const std::string& name="", const std::string& className="");
    //Hyperedges languages(const std::string& name="", const std::string& className="");
    return 0;
}
