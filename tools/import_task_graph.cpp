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
    std::cout << myName << " <yaml-file-in> <lang-spec-file> <yaml-file-out>\n\n";
    std::cout << "Options:\n";
    std::cout << "--help\t" << "Show usage\n";
    std::cout << "\nExample:\n";
    std::cout << myName << " task_graph.yml def_task_graph.yml imported.yml\n";
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
    std::string langSpecIn(argv[optind+1]);
    std::string fileNameOut(argv[optind+2]);
    Hypergraph* hypergraph = YAML::LoadFile(langSpecIn).as<Hypergraph*>();
    Conceptgraph cgraph(*hypergraph);
    CommonConceptGraph ccgraph(cgraph);
    Software::Graph swgraph(ccgraph);

    YAML::Node import = YAML::LoadFile(fileNameIn);
    const YAML::Node& nodes(import["nodes"]);
    if (nodes.IsDefined())
    {
        std::map<unsigned, Hyperedges> old2new;
        for (auto it = nodes.begin(); it != nodes.end(); it++)
        {
            const YAML::Node node(*it);
            unsigned id = node["id"].as<unsigned>();
            std::string label = node["label"].as<std::string>();
            std::string type = node["type"].as<std::string>();
            // First: Find the superclass
            Hyperedges super = swgraph.find(type);
            if (!super.size())
                continue;
            // Second: Instantiate from that superclass
            Hyperedges sub = swgraph.instantiateFrom(super, label);
            old2new[id] = sub;
        }
        const YAML::Node& edges(import["edges"]);
        for (auto it = edges.begin(); it != edges.end(); it++)
        {
            const YAML::Node edge(*it);
            unsigned id = edge["id"].as<unsigned>();
            std::string label = edge["label"].as<std::string>();
            std::string type = edge["type"].as<std::string>();
            unsigned fromId = edge["fromId"].as<unsigned>();
            unsigned toId = edge["toId"].as<unsigned>();
            // First: Find the superclass
            Hyperedges super = swgraph.relations(type);
            if (!super.size())
                continue;
            // Second: Instantiate from that superclass
            Hyperedges sub = swgraph.relateFrom(old2new[fromId], old2new[toId], super);
            if (!sub.size())
                continue;
            Hyperedges edgeId = subtract(subtract(sub, old2new[fromId]), old2new[toId]);
            swgraph.get(*edgeId.begin())->updateLabel(label);
        }
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
