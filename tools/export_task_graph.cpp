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
    std::cout << myName << " hypergraph.yml def_task_graph.yml exported.yml\n";
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
    std::string langSpecIn(argv[optind+1]);
    std::string fileNameOut(argv[optind+2]);
    Hypergraph* hypergraph = YAML::LoadFile(langSpecIn).as<Hypergraph*>();
    Hypergraph* incoming = YAML::LoadFile(fileNameIn).as<Hypergraph*>();
    Hypergraph merged(*hypergraph, *incoming);
    Conceptgraph cgraph(merged);
    CommonConceptGraph ccgraph(cgraph);
    Software::Graph swgraph(ccgraph);

    // Find domain
    UniqueId domainId = "system_modelling::task_graph";
    Hyperedges domainParts = swgraph.componentsOf(Hyperedges{domainId});

    // EXPORT
    YAML::Node doc;
    YAML::Node nodes(doc["nodes"]);
    YAML::Node edges(doc["edges"]);
    Hyperedges allConcepts(swgraph.find());
    Mapping old2new;
    unsigned int nodeId = 1;
    // I. Export all concepts whose superclasses are known to belong to domain
    for (UniqueId concept : allConcepts)
    {
        // Get the direct superclasses which are also part of the domain
        Hyperedges super = swgraph.to(intersect(swgraph.relationsFrom(Hyperedges{concept}), swgraph.factsOf(CommonConceptGraph::IsAId)));
        super = intersect(super, domainParts);
        if (!super.size())
        {
            std::cout << "Ignoring " << concept << " which has no superclass in this domain\n";
            continue;
        }
        YAML::Node node;
        node["id"] = nodeId;
        node["label"] = swgraph.get(concept)->label();
        node["type"] = *super.begin();
        nodes.push_back(node);
        old2new[concept] = std::to_string(nodeId);
        nodeId++;
    }
    // II. Export all relations whose super relations are known to belong to domain
    Hyperedges allRelations(swgraph.relations());
    for (UniqueId rel : allRelations)
    {
        // Since system_modelling knows no subrelations, we only export facts
        // And only those which are part of the domain
        Hyperedges super = swgraph.factsOf(rel,"",CommonConceptGraph::DOWN);
        super = intersect(super, domainParts);
        if (!super.size())
        {
            std::cout << "Ignoring " << rel << " which is no fact of a super relation of this domain\n";
            continue;
        }
        // since we can only export binary edges, we have to go through every source,target pair
        for (UniqueId fromId : swgraph.get(rel)->pointingFrom())
        {
            // if not exported, skip
            if (!old2new.count(fromId))
            {
                std::cout << fromId << " has not been exported. Skipping " << rel << "\n";
                continue;
            }
            for (UniqueId toId : swgraph.get(rel)->pointingTo())
            {
                // if not exported, skip
                if (!old2new.count(toId))
                {
                    std::cout << toId << " has not been exported. Skipping " << rel << "\n";
                    continue;
                }
                YAML::Node edge;
                edge["id"] = nodeId;
                edge["label"] = swgraph.get(rel)->label();
                edge["type"] = *super.begin();
                edge["fromId"] = old2new[fromId];
                edge["toId"] = old2new[toId];
                edges.push_back(edge);
                nodeId++;
            }
        }
    }

    // Store result
    std::ofstream fout;
    fout.open(fileNameOut);
    if(!fout.good()) {
        std::cout << "FAILED\n";
    }
    fout << doc;
    fout.close();

    return 0;
}
