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
    std::cout << myName << " bg.yml def_bg_graph.yml imported_bg.yml\n";
}

Hyperedges getInputsOf(Software::Graph& swgraph, const Hyperedges& algorithms, const std::string& label)
{
    Hyperedges allInputs = swgraph.inputs(label);
    Hyperedges allChildren = swgraph.childrenOf(algorithms, label);
    //std::cout << allInputs << std::endl;
    //std::cout << allChildren << std::endl;
    return intersect(allInputs, allChildren);
}

Hyperedges getOutputsOf(Software::Graph& swgraph, const Hyperedges& algorithms, const std::string& label)
{
    Hyperedges allOutputs = swgraph.outputs(label);
    Hyperedges allChildren = swgraph.childrenOf(algorithms, label);
    return intersect(allOutputs, allChildren);
}

Hyperedges getMergesOfInput(Software::Graph& swgraph, const Hyperedges& inputs, const std::string& label)
{
    Hyperedges allMerges = swgraph.instancesOf(swgraph.subclassesOf(swgraph.algorithmClasses("MERGE")), label);
    Hyperedges allEndpoints = swgraph.endpointsOf(inputs, "merged"); // all outputs of some merges?
    Hyperedges allParentsOfEndpoints = swgraph.childrenOf(allEndpoints, label, Software::Graph::TraversalDirection::UP);
    std::cout << allMerges << std::endl;
    std::cout << allEndpoints << std::endl;
    std::cout << allParentsOfEndpoints << std::endl;
    return intersect(allMerges, allParentsOfEndpoints);
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
    Conceptgraph cgraph(*hypergraph);
    CommonConceptGraph ccgraph(cgraph);
    Software::Graph swgraph(ccgraph);

    // Prerequisites
    Hyperedges realInputClassId = swgraph.inputClasses("RealInput");
    Hyperedges realOutputClassId = swgraph.inputClasses("RealOutput");
    Hyperedges nodeClassId = swgraph.algorithmClasses("NODE");
    Hyperedges mergeClassId = swgraph.algorithmClasses("MERGE");
    Hyperedges edgeClassId = swgraph.algorithmClasses("EDGE");

    YAML::Node import = YAML::LoadFile(fileNameIn);
    const YAML::Node& nodes(import["nodes"]);
    if (nodes.IsDefined())
    {
        std::map<UniqueId, Hyperedges> old2new;
        // Parse nodes first
        for (auto it = nodes.begin(); it != nodes.end(); it++)
        {
            bool isContainer = false;
            const YAML::Node node(*it);
            UniqueId id = node["id"].as<UniqueId>();
            std::string label, type;

            // First: Find the superclass
            type = node["type"].as<std::string>();
            Hyperedges super = swgraph.algorithmClasses(type);
            if (!super.size())
            {
                std::cout << "Warning: Could not find " << type << " superclass\n";
                continue;
            }
            const YAML::Node name(node["name"]);
            const YAML::Node subgraph_name(node["subgraph_name"]);
            const YAML::Node inputs(node["inputs"]);
            const YAML::Node outputs(node["outputs"]);
            // Special: A BG node can have a name || subgraph_name
            if (subgraph_name.IsDefined())
            {
                isContainer = true;
                label = node["subgraph_name"].as<std::string>();
            } else {
                label = node["name"].as<std::string>();
            }

            // Second:
            // For containers we have to create a subclass of <type> which will be a superclass for an instance later
            Hyperedges sub;
            if (isContainer)
            {
                // FOR NON-ATOMIC NODES:
                // Find or create the superclass
                sub = swgraph.algorithmClasses(label);
                if (!sub.size())
                {
                    std::cout << "Create subclass " << label << " of type " << super << "\n";
                    sub = swgraph.createAlgorithm(label);
                    swgraph.isA(sub, super);
                    // Third: For all inputs, we have to create them if super had to be created AND
                    // we have to create the "correct" merge nodes and connect them
                    if (inputs.IsDefined())
                    {
                        for (YAML::Node::const_iterator it = inputs.begin(); it != inputs.end(); it++)
                        {
                            const YAML::Node& input(*it);
                            std::string inputIdx = input["idx"].as<std::string>();
                            //std::string inputName = input["name"]; // TODO: Either by idx || name!!!
                            //std::string mergeType = input["type"].as<std::string>();
                            //std::string mergeBias = input["bias"].as<std::string>();
                            //std::string mergeDefault = input["default"].as<std::string>();
                            swgraph.needs(sub, swgraph.instantiateInput(realInputClassId, inputIdx));
                            std::cout << "Create input " << inputIdx << "\n";
                        }
                    }
                    // Also handle outputs:
                    // Create them if super had to be created
                    if (outputs.IsDefined())
                    {
                        for (YAML::Node::const_iterator it = outputs.begin(); it != outputs.end(); it++)
                        {
                            const YAML::Node& output(*it);
                            std::string outputIdx = output["idx"].as<std::string>();
                            //std::string outputName = output["name"]; // TODO: Either by idx || name!!!
                            swgraph.provides(sub, swgraph.instantiateOutput(realOutputClassId, outputIdx));
                            std::cout << "Create output " << outputIdx << "\n";
                        }
                    }
                }
                // Use the found or created subclass for instantiation
                super = sub;
            }
            // FOR ATOMIC & OTHER NODES:
            // For pure bg nodes, the things will be instances!
            std::cout << "Instantiating " << label << " of type " << super << "\n";
            sub = swgraph.instantiateDeepFrom(super, label);
            old2new[id] = sub;
            // Third: For all inputs, we have to create the "correct" merge nodes and connect them
            if (inputs.IsDefined())
            {
                for (YAML::Node::const_iterator it = inputs.begin(); it != inputs.end(); it++)
                {
                    const YAML::Node& input(*it);
                    std::string inputIdx = input["idx"].as<std::string>();
                    //std::string inputName = input["name"]; // TODO: Either by idx || name!!!
                    std::string mergeType = input["type"].as<std::string>();
                    std::string mergeBias = input["bias"].as<std::string>();
                    std::string mergeDefault = input["default"].as<std::string>();
                    // Find the input of the instance with matching idx || name
                    Hyperedges inputsOfSub = getInputsOf(swgraph, sub, inputIdx);
                    if (!inputsOfSub.size())
                    {
                        std::cout << "Warning: Could not find input " << inputIdx << " of node " << label << std::endl;
                        continue;
                    }
                    // Then instantiate a merge and connect its output to the input
                    Hyperedges mergeSuper = intersect(swgraph.subclassesOf(mergeClassId), swgraph.find(mergeType));
                    if (!mergeSuper.size())
                    {
                        std::cout << "Warning: Could not find merge type " << mergeType << "\n";
                        continue;
                    }
                    std::cout << "Instantiating merge " << mergeSuper << " for input " << inputIdx << "\n";
                    Hyperedges mergeInst = swgraph.instantiateDeepFrom(mergeSuper);
                    Hyperedges outputsOfMerge = getOutputsOf(swgraph, mergeInst, "merged");
                    swgraph.depends(inputsOfSub, outputsOfMerge);
                    // TODO: For later use we could remember the input merge instances per input
                }
            }
            // Also handle outputs
            if (outputs.IsDefined())
            {
                // Nothing to do, right?
            }
        }
        const YAML::Node& edges(import["edges"]);
        // Then parse edges
        for (auto it = edges.begin(); it != edges.end(); it++)
        {
            const YAML::Node edge(*it);
            std::string label = edge["weight"].as<std::string>();
            Hyperedges fromNodeId = old2new[edge["fromNodeId"].as<UniqueId>()];
            Hyperedges toNodeId = old2new[edge["toNodeId"].as<UniqueId>()];
            std::string fromNodeOutputIdx = edge["fromNodeOutputIdx"].as<UniqueId>();
            std::string toNodeInputIdx = edge["toNodeInputIdx"].as<UniqueId>();
            // First: Find the superclass
            Hyperedges super = edgeClassId;
            if (!super.size())
            {
                std::cout << "Warning: " << edgeClassId << " is empty\n";
                continue;
            }
            // Second: Instantiate from that superclass
            Hyperedges edgeInstance = swgraph.instantiateFrom(super, label);
            if (!edgeInstance.size())
            {
                std::cout << "Warning: Could not instantiate edge with weight " << label << "\n";
                continue;
            }
            // Third: connect IN of EDGE with OUT of fromId and OUT of edge with UNCONNECTED MERGE INPUT of toId
            Hyperedges inputsOfEdge = getInputsOf(swgraph, edgeInstance, "in");
            Hyperedges outputsOfEdge = getOutputsOf(swgraph, edgeInstance, "out");
            Hyperedges outputsOfSource = getOutputsOf(swgraph, fromNodeId, fromNodeOutputIdx);
            Hyperedges inputsOfTarget = getInputsOf(swgraph, toNodeId, toNodeInputIdx);
            Hyperedges mergesOfTargetInputs = getMergesOfInput(swgraph, inputsOfTarget, ""); // any merge
            Hyperedges inputsOfMerge = getInputsOf(swgraph,mergesOfTargetInputs,"");
            // Find unconnected input
            Hyperedges unconnectedInputs;
            for (UniqueId unconnectedInput : inputsOfMerge)
            {
                if (swgraph.endpointsOf(Hyperedges{unconnectedInput}).size() < 1)
                {
                    unconnectedInputs = unite(unconnectedInputs, Hyperedges{unconnectedInput});
                    break;
                }
            }
            if (!unconnectedInputs.size())
            {
                std::cout << "Warning: Found no unconnected input of merge(s) " << mergesOfTargetInputs << "\n";
                continue;
            }
            // Connect
            std::cout << "Connecting " << fromNodeId << "," << fromNodeOutputIdx << " to " << toNodeId << "," << toNodeInputIdx << "\n";
            swgraph.depends(inputsOfEdge, outputsOfSource);
            swgraph.depends(outputsOfEdge, unconnectedInputs);
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
