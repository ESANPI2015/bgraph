#include "BehaviorGraph.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <sstream>

namespace Behavior {

const UniqueId Graph::NodeId = "Behavior::Graph::Node";
const UniqueId Graph::ExternId = "Behavior::Graph::Node::Extern";
const UniqueId Graph::SubgraphId = "Behavior::Graph::Node::Subgraph";
const UniqueId Graph::MergeId = "Behavior::Graph::Merge";
const UniqueId Graph::EdgeId = "Behavior::Graph::Edge";
const UniqueId Graph::InterfaceId = "Behavior::Graph::Interface";
const UniqueId Graph::MergeInterfaceId = "Behavior::Graph::Merge::Interface";
const UniqueId Graph::InterfaceValueId = "Behavior::Graph::Interface::Value";

// Predefined nodes & merges
const UniqueId Graph::InputNodeId = "Behavior::Graph::Node::Input";
const UniqueId Graph::OutputNodeId = "Behavior::Graph::Node::Output";

// TODO: Move these to a BGRAPH Generator class!
//const UniqueId Graph::CId = "Behavior::Graph::Interface::C";
//const UniqueId Graph::VHDLId = "Behavior::Graph::Interface::VHDL";
//const UniqueId Graph::CValueId = "Behavior::Graph::Interface::Value::C";
//const UniqueId Graph::VHDLValueId = "Behavior::Graph::Interface::Value::VHDL";

Graph::Graph()
{
    setupMetaModel();
}

Graph::Graph(const Hypergraph& base)
: Software::Network(base)
{
    setupMetaModel();
}

Graph::~Graph()
{
}

void Graph::setupMetaModel()
{
    // FIXME:
    // * Decide whether a MERGE is an algorithm or an interface with subinterfaces? Its both!
    // * Provide default values for 'default' and 'bias'

    // Interface classes
    createInterface(Graph::InterfaceId, "Real");
    createInterface(Graph::MergeInterfaceId, "RealArray");
    // TODO: Shall we make 'Real' partOf 'RealArray' ? And if we do, how many? Fixed amount? Or over TERNARY partOf?
    createValue(Graph::InterfaceValueId, "RealValue");

    // Three main superclasses of the behavior graph domain:
    // NODE -- EDGE -- MERGE -- NODE ... pattern
    createAlgorithm(Graph::NodeId, "NODE");
    if (!exists(Graph::MergeId))
    {
        createAlgorithm(Graph::MergeId, "MERGE");
        // Standard interfaces of merges
        Hyperedges defUids(instantiateInterfaceFor(Hyperedges{Graph::MergeId}, Hyperedges{Graph::InterfaceId}, "default"));
        needsInterface(Hyperedges{Graph::MergeId}, defUids);
        instantiateValueFor(defUids, Hyperedges{Graph::InterfaceValueId}, "0.0");
        Hyperedges biasUids(instantiateInterfaceFor(Hyperedges{Graph::MergeId}, Hyperedges{Graph::InterfaceId}, "bias"));
        needsInterface(Hyperedges{Graph::MergeId}, biasUids);
        instantiateValueFor(biasUids, Hyperedges{Graph::InterfaceValueId}, "0.0");
        needsInterface(Hyperedges{Graph::MergeId}, instantiateInterfaceFor(Hyperedges{Graph::MergeId}, Hyperedges{Graph::MergeInterfaceId}, "in"));
        // NOTE: We could make instances of 'Real' partOf 'RealArray' on the fly!
        providesInterface(Hyperedges{Graph::MergeId}, instantiateInterfaceFor(Hyperedges{Graph::MergeId}, Hyperedges{Graph::InterfaceId}, "out"));
    }
    if (!exists(Graph::EdgeId))
    {
        createAlgorithm(Graph::EdgeId, "EDGE");
        // Standard interfaces of edges
        needsInterface(Hyperedges{Graph::EdgeId}, instantiateInterfaceFor(Hyperedges{Graph::EdgeId}, Hyperedges{Graph::InterfaceId}, "in"));
        Hyperedges weightUid(instantiateInterfaceFor(Hyperedges{Graph::EdgeId}, Hyperedges{Graph::InterfaceId}, "weight"));
        needsInterface(Hyperedges{Graph::EdgeId}, weightUid);
        instantiateValueFor(weightUid, Hyperedges{Graph::InterfaceValueId}, "1.0");
        providesInterface(Hyperedges{Graph::EdgeId}, instantiateInterfaceFor(Hyperedges{Graph::EdgeId}, Hyperedges{Graph::InterfaceId}, "out"));
    }

    // The NODE class subalgorithms
    createAlgorithm(Graph::ExternId, "EXTERN", Hyperedges{Graph::NodeId});
    createAlgorithm(Graph::SubgraphId, "SUBGRAPH", Hyperedges{Graph::NodeId});

    // Built-in Algorithms (PURE BAGEL)
    // The different subclasses of MERGE
    createAlgorithm("Behavior::Graph::Merge::Sum", "SUM", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Product", "PRODUCT", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Min", "MIN", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Max", "MAX", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Mean", "MEAN", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Norm", "NORM", Hyperedges{Graph::MergeId});

    // Arity 1 nodes
    if (!exists("Behavior::Graph::Node::1-1"))
    {
        createAlgorithm("Behavior::Graph::Node::1-1", "NODE 1-1", Hyperedges{Graph::NodeId});
        needsInterface(Hyperedges{"Behavior::Graph::Node::1-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::1-1"}, Hyperedges{Graph::InterfaceId}, "0"));
        providesInterface(Hyperedges{"Behavior::Graph::Node::1-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::1-1"}, Hyperedges{Graph::InterfaceId}, "0"));
    }
    createAlgorithm("Behavior::Graph::Node::Pipe", "PIPE", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm(Graph::InputNodeId, "INPUT", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm(Graph::OutputNodeId, "OUTPUT", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::Divide", "DIVIDE", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::Sine", "SIN", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::Cosine", "COS", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::Tangens", "TAN", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::TangensHyperbolicus", "TANH", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::ArcusCosine", "ACOS", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::ArcusSine", "ASIN", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::ArcusTangens", "ATAN", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::Logarithm", "LOG", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::Exponential", "EXP", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::Absolute", "ABS", Hyperedges{"Behavior::Graph::Node::1-1"});
    createAlgorithm("Behavior::Graph::Node::SquareRoot", "SQRT", Hyperedges{"Behavior::Graph::Node::1-1"});

    // Arity 2 nodes
    if (!exists("Behavior::Graph::Node::2-1"))
    {
        createAlgorithm("Behavior::Graph::Node::2-1", "NODE 2-1", Hyperedges{Graph::NodeId});
        needsInterface(Hyperedges{"Behavior::Graph::Node::2-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::2-1"}, Hyperedges{Graph::InterfaceId}, "0"));
        needsInterface(Hyperedges{"Behavior::Graph::Node::2-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::2-1"}, Hyperedges{Graph::InterfaceId}, "1"));
        providesInterface(Hyperedges{"Behavior::Graph::Node::2-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::2-1"}, Hyperedges{Graph::InterfaceId}, "0"));
    }
    createAlgorithm("Behavior::Graph::Node::ArcusTangens2D", "ATAN2", Hyperedges{"Behavior::Graph::Node::2-1"});
    createAlgorithm("Behavior::Graph::Node::Power", "POW", Hyperedges{"Behavior::Graph::Node::2-1"});
    createAlgorithm("Behavior::Graph::Node::Modulo", "MOD", Hyperedges{"Behavior::Graph::Node::2-1"});

    // Arity 3 nodes
    if (!exists("Behavior::Graph::Node::3-1"))
    {
        createAlgorithm("Behavior::Graph::Node::3-1", "NODE 3-1", Hyperedges{Graph::NodeId});
        needsInterface(Hyperedges{"Behavior::Graph::Node::3-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::3-1"}, Hyperedges{Graph::InterfaceId}, "0"));
        needsInterface(Hyperedges{"Behavior::Graph::Node::3-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::3-1"}, Hyperedges{Graph::InterfaceId}, "1"));
        needsInterface(Hyperedges{"Behavior::Graph::Node::3-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::3-1"}, Hyperedges{Graph::InterfaceId}, "2"));
        providesInterface(Hyperedges{"Behavior::Graph::Node::3-1"}, instantiateInterfaceFor(Hyperedges{"Behavior::Graph::Node::3-1"}, Hyperedges{Graph::InterfaceId}, "0"));
    }
    createAlgorithm("Behavior::Graph::Node::GreaterZero", ">0", Hyperedges{"Behavior::Graph::Node::3-1"});
    createAlgorithm("Behavior::Graph::Node::ApproxZero", "==0", Hyperedges{"Behavior::Graph::Node::3-1"});

    // Pregenerated entities (see Generator class in lib/componentnet)
    // TODO: Make two concrete interface classes (one for C++ and one for VHDL)
    //       Plain types: float and std_logic_vector(31 downto 0)
    // TODO: Move to a BGRAPH Generator class
}

std::string Graph::exportModel(const UniqueId& uid) const
{
    YAML::Node doc;
    doc["model"] = access(uid).label();

    // Handle nodes
    Hyperedges partUids(componentsOf(Hyperedges{uid}));
    Hyperedges nodeUids(instancesOf(algorithmClasses("",Hyperedges{Graph::NodeId})));
    Hyperedges externNodeUids(instancesOf(algorithmClasses("",Hyperedges{Graph::ExternId})));
    Hyperedges subgraphNodeUids(instancesOf(algorithmClasses("",Hyperedges{Graph::SubgraphId})));
    for (const UniqueId& partUid : intersect(partUids, nodeUids))
    {
        YAML::Node node;
        node["id"] = partUid;
        node["name"] = access(partUid).label();
        // Handle inputs
        Hyperedges partInputUids(inputsOf(Hyperedges{partUid}));
        for (const UniqueId& partInputUid : partInputUids)
        {
            YAML::Node input;
            input["name"] = access(partInputUid).label();
            // Handle merge
            Hyperedges mergeUids(outputsOf(endpointsOf(Hyperedges{partInputUid}, "out", TraversalDirection::INVERSE), "", TraversalDirection::INVERSE));
            for (const UniqueId& mergeUid : mergeUids)
            {
                input["type"] = access(mergeUid).label();
                Hyperedges defValueUids(valuesOf(inputsOf(Hyperedges{mergeUid}, "default")));
                Hyperedges biasValueUids(valuesOf(inputsOf(Hyperedges{mergeUid}, "bias")));
                for (const UniqueId& defValueUid : defValueUids)
                {
                    input["default"] = std::stof(access(defValueUid).label());
                    for (const UniqueId& biasValueUid : biasValueUids)
                    {
                        input["bias"] = std::stof(access(biasValueUid).label());
                        node["inputs"].push_back(input);
                    }
                }
            }
        }
        // Handle outputs
        Hyperedges partOutputUids(outputsOf(Hyperedges{partUid}));
        for (const UniqueId& partOutputUid : partOutputUids)
        {
            YAML::Node output;
            output["name"] = access(partOutputUid).label();
            node["outputs"].push_back(output);
        }
        // Handle node types
        Hyperedges superclassUids(instancesOf(Hyperedges{partUid},"",TraversalDirection::FORWARD));
        if (std::find(externNodeUids.begin(), externNodeUids.end(), partUid) != externNodeUids.end())
        {
            node["type"] = access(Graph::ExternId).label();
            for (const UniqueId& superclassUid : superclassUids)
            {
                node["extern_name"] = access(superclassUid).label();
                doc["nodes"].push_back(node);
            }
        }
        else if (std::find(subgraphNodeUids.begin(), subgraphNodeUids.end(), partUid) != subgraphNodeUids.end())
        {
            node["type"] = access(Graph::SubgraphId).label();
            for (const UniqueId& superclassUid : superclassUids)
            {
                node["subgraph_name"] = access(superclassUid).label();
                doc["nodes"].push_back(node);
            }
        }
        else
        {
            for (const UniqueId& superclassUid : superclassUids)
            {
                node["type"] = access(superclassUid).label();
                doc["nodes"].push_back(node);
            }
        }
    }

    // Handle edges
    Hyperedges edgeUids(instancesOf(algorithmClasses("",Hyperedges{Graph::EdgeId})));
    for (const UniqueId& edgeUid : intersect(partUids, edgeUids))
    {
        YAML::Node edge;
        // weight, fromNode, toNode, fromNodeOutput, toNodeOutput
        Hyperedges weightValueUids(valuesOf(inputsOf(Hyperedges{edgeUid}, "weight")));
        Hyperedges predIfUids(endpointsOf(inputsOf(Hyperedges{edgeUid}, "in"),"",TraversalDirection::INVERSE));
        Hyperedges succIfUids(endpointsOf(outputsOf(inputsOf(endpointsOf(outputsOf(Hyperedges{edgeUid}, "out")), "", TraversalDirection::INVERSE),"out")));
        for (const UniqueId& weightValueUid : weightValueUids)
        {
            edge["weight"] = std::stof(access(weightValueUid).label());
            for (const UniqueId& predIfUid : predIfUids)
            {
                edge["fromNodeOutput"] = access(predIfUid).label();
                Hyperedges predNodeUids(outputsOf(Hyperedges{predIfUid}, "", TraversalDirection::INVERSE));
                for (const UniqueId& predNodeUid : predNodeUids)
                {
                    edge["fromNode"] = access(predNodeUid).label();
                    for (const UniqueId& succIfUid : succIfUids)
                    {
                        edge["toNodeInput"] = access(succIfUid).label();
                        Hyperedges succNodeUids(inputsOf(Hyperedges{succIfUid}, "", TraversalDirection::INVERSE));
                        for (const UniqueId& succNodeUid : succNodeUids)
                        {
                            edge["toNode"] = access(succNodeUid).label();
                            doc["edges"].push_back(edge);
                        }
                    }
                }
            }
        }
    }

    std::stringstream ss;
    ss << doc;
    return ss.str();
}

UniqueId Graph::importModel(const std::string& serializedModel)
{
    UniqueId modelUid;
    YAML::Node doc = YAML::Load(serializedModel);
    // Check if nodes exist. If not, reject model
    const YAML::Node& nodesYAML(doc["nodes"]);
    if (!nodesYAML.IsDefined())
        return modelUid;
    // Create SUBGRAPH class
    std::string label("SUBGRAPH");
    if (doc["model"].IsDefined())
        label = doc["model"].as<std::string>();
    modelUid = Graph::SubgraphId+"::"+label;
    createAlgorithm(modelUid, label, Hyperedges{Graph::SubgraphId});

    // Handle nodes
    std::map<UniqueId, Hyperedges> old2new;
    std::map<std::string, Hyperedges> label2node;
    for (YAML::Node::const_iterator nit = nodesYAML.begin(); nit != nodesYAML.end(); ++nit)
    {
        const YAML::Node& nodeYAML(*nit);
        const UniqueId& id(nodeYAML["id"].as<UniqueId>());
        const std::string& type(nodeYAML["type"].as<std::string>());
        std::string label(id);
        if (nodeYAML["name"].IsDefined())
            label = nodeYAML["name"].as<std::string>();
        // Find superclass of given name
        Hyperedges superUids(algorithmClasses(type, Hyperedges{Graph::NodeId}));
        if (superUids.empty())
        {
            // TODO: This should be an error case?!
            continue;
        }

        bool isExternNode(std::find(superUids.begin(), superUids.end(), Graph::ExternId) != superUids.end());
        bool isSubgraphNode(std::find(superUids.begin(), superUids.end(), Graph::SubgraphId) != superUids.end());
        if (isExternNode)
        {
            // Extern node
            // extern_name defines the superclass!
            const std::string& subtype(nodeYAML["extern_name"].as<std::string>());
            superUids = algorithmClasses(subtype, Hyperedges{Graph::ExternId});
            if (superUids.empty())
                superUids = createAlgorithm(Graph::ExternId+"::"+subtype, subtype, Hyperedges{Graph::ExternId});
        }
        else if (isSubgraphNode)
        {
            // Subgraph node
            // subgraph_name defines the superclass!
            const std::string& subtype(nodeYAML["subgraph_name"].as<std::string>());
            superUids = algorithmClasses(subtype, Hyperedges{Graph::SubgraphId});
            if (superUids.empty())
                superUids = createAlgorithm(Graph::SubgraphId+"::"+subtype, subtype, Hyperedges{Graph::SubgraphId});
        }

        const YAML::Node& inputsYAML(nodeYAML["inputs"]);
        const YAML::Node& outputsYAML(nodeYAML["outputs"]);
        // Adding IO info to superclasses if needed
        if (isExternNode || isSubgraphNode)
        {
            // Create IOs (if they do not exist)
            if (inputsYAML.IsDefined())
            {
                for (YAML::Node::const_iterator it = inputsYAML.begin(); it != inputsYAML.end(); it++)
                {
                    const YAML::Node& inputYAML(*it);
                    std::string inputLabel;
                    if (inputYAML["name"].IsDefined())
                    {
                        inputLabel = inputYAML["name"].as<std::string>();
                    } else {
                        inputLabel = inputYAML["idx"].as<std::string>();
                    }
                    // Search for input
                    Hyperedges inputOfComponent(inputsOf(superUids, inputLabel));
                    if (inputOfComponent.size())
                        continue;
                    needsInterface(superUids, instantiateInterfaceFor(superUids, Hyperedges{Graph::InterfaceId}, inputLabel));
                }
            }
            if (outputsYAML.IsDefined())
            {
                for (YAML::Node::const_iterator it = outputsYAML.begin(); it != outputsYAML.end(); it++)
                {
                    const YAML::Node& outputYAML(*it);
                    std::string outputLabel;
                    if (outputYAML["name"].IsDefined())
                    {
                        outputLabel = outputYAML["name"].as<std::string>();
                    } else {
                        outputLabel = outputYAML["idx"].as<std::string>();
                    }
                    // Search for output
                    Hyperedges outputOfComponent(outputsOf(superUids, outputLabel));
                    if (outputOfComponent.size())
                        continue;
                    providesInterface(superUids, instantiateInterfaceFor(superUids, Hyperedges{Graph::InterfaceId}, outputLabel));
                }
            }
        }

        // Instantiate part
        Hyperedges uid(instantiateComponent(superUids, label));
        partOfComponent(uid, Hyperedges{modelUid});
        old2new[id] = uid;
        label2node[label] = uid;

        // Instantiate MERGE given IO info
        if (inputsYAML.IsDefined())
        {
            for (YAML::Node::const_iterator it = inputsYAML.begin(); it != inputsYAML.end(); it++)
            {
                const YAML::Node& inputYAML(*it);
                std::string inputLabel;
                if (inputYAML["name"].IsDefined())
                {
                    inputLabel = inputYAML["name"].as<std::string>();
                } else {
                    inputLabel = inputYAML["idx"].as<std::string>();
                }
                // Search for input
                Hyperedges inputOfComponent(inputsOf(uid, inputLabel));
                if (!inputOfComponent.size())
                    continue;

                // For INPUT nodes: Make their input(s) the input(s) of the toplvl node
                // Also, suppress merge creation
                bool isInputNode(std::find(superUids.begin(), superUids.end(), Graph::InputNodeId) != superUids.end());
                if (isInputNode)
                {
                    for (const UniqueId& inputUid : inputOfComponent)
                    {
                        needsInterface(Hyperedges{modelUid}, instantiateAliasInterfaceFor(Hyperedges{modelUid}, Hyperedges{inputUid}, label));
                    }
                    continue;
                }
                
                // For the found input, create and connect a merge (if it does not exist)
                const std::string& mergeType(inputYAML["type"].as<std::string>());
                const std::string& mergeBias(inputYAML["bias"].as<std::string>());
                const std::string& mergeDefault(inputYAML["default"].as<std::string>());
                Hyperedges mergeUids(outputsOf(endpointsOf(inputOfComponent, "out", TraversalDirection::INVERSE), mergeType, TraversalDirection::INVERSE));
                if (mergeUids.empty())
                {
                    Hyperedges mergeClassUids(algorithmClasses(mergeType, Hyperedges{Graph::MergeId}));
                    if (mergeClassUids.empty())
                        continue;
                    Hyperedges newMergeUids(instantiateComponent(mergeClassUids));
                    Hyperedges defValueUids(valuesOf(inputsOf(newMergeUids, "default")));
                    for (const UniqueId& defValueUid : defValueUids)
                    {
                        access(defValueUid).updateLabel(mergeDefault);
                    }
                    Hyperedges biasValueUids(valuesOf(inputsOf(newMergeUids, "bias")));
                    for (const UniqueId& biasValueUid : biasValueUids)
                    {
                        access(biasValueUid).updateLabel(mergeBias);
                    }
                    dependsOn(inputOfComponent, outputsOf(newMergeUids, "out"));
                    partOfComponent(newMergeUids, Hyperedges{modelUid});
                }
            }
        }

        // Export outputs
        if (outputsYAML.IsDefined())
        {
            for (YAML::Node::const_iterator it = outputsYAML.begin(); it != outputsYAML.end(); it++)
            {
                const YAML::Node& outputYAML(*it);
                std::string outputLabel;
                if (outputYAML["name"].IsDefined())
                {
                    outputLabel = outputYAML["name"].as<std::string>();
                } else {
                    outputLabel = outputYAML["idx"].as<std::string>();
                }
                Hyperedges outputOfComponent(outputsOf(uid, outputLabel));
                if (!outputOfComponent.size())
                    continue;

                // For OUTPUT nodes: Make their output(s) the output(s) of the toplvl node
                bool isOutputNode(std::find(superUids.begin(), superUids.end(), Graph::OutputNodeId) != superUids.end());
                if (isOutputNode)
                {
                    for (const UniqueId& outputUid : outputOfComponent)
                    {
                        providesInterface(Hyperedges{modelUid}, instantiateAliasInterfaceFor(Hyperedges{modelUid}, Hyperedges{outputUid}, label));
                    }
                }
            }
        }
    }

    // Treat edges
    const YAML::Node& edgesYAML(doc["edges"]);
    if (!edgesYAML.IsDefined())
        return modelUid;

    for (YAML::Node::const_iterator eit = edgesYAML.begin(); eit != edgesYAML.end(); ++eit)
    {
        const YAML::Node& edgeYAML(*eit);
        const std::string& weight(edgeYAML["weight"].as<std::string>());
        std::string fromLabel, fromOutputLabel, toLabel, toInputLabel;

        // We can also connect by names, right? Yes :)
        Hyperedges fromNodeIds;
        if (edgeYAML["fromNode"].IsDefined())
        {
            // Find UnqiueIds by label
            fromLabel = edgeYAML["fromNode"].as<std::string>();
            fromNodeIds = label2node[fromLabel];
        } else {
            // Find UniqueIds by remapped ids
            fromLabel = edgeYAML["fromNodeId"].as<std::string>();
            fromNodeIds = old2new[fromLabel];
        }
        if (fromNodeIds.empty())
            continue;
        Hyperedges toNodeIds;
        if (edgeYAML["toNode"].IsDefined())
        {
            // Find UnqiueIds by label
            toLabel = edgeYAML["toNode"].as<std::string>();
            toNodeIds = label2node[toLabel];
        } else {
            // Find UniqueIds by remapped ids
            toLabel = edgeYAML["toNodeId"].as<std::string>();
            toNodeIds = old2new[toLabel];
        }
        if (toNodeIds.empty())
            continue;
        if (edgeYAML["fromNodeOutput"].IsDefined())
        {
            // Find UnqiueIds by label
            fromOutputLabel = edgeYAML["fromNodeOutput"].as<std::string>();
        } else {
            // Find UniqueIds by remapped ids
            fromOutputLabel = edgeYAML["fromNodeOutputIdx"].as<std::string>();
        }
        Hyperedges fromNodeOutputIds(outputsOf(fromNodeIds, fromOutputLabel));
        if (fromNodeOutputIds.empty())
            continue;
        if (edgeYAML["toNodeInput"].IsDefined())
        {
            // Find UnqiueIds by label
            toInputLabel = edgeYAML["toNodeInput"].as<std::string>();
        } else {
            // Find UniqueIds by remapped ids
            toInputLabel = edgeYAML["toNodeInputIdx"].as<std::string>();
        }
        Hyperedges toNodeInputIds(inputsOf(toNodeIds, toInputLabel));
        if (toNodeInputIds.empty())
            continue;
        // Get the merge node and their array input
        Hyperedges mergeInputUids(inputsOf(outputsOf(endpointsOf(toNodeInputIds, "out", TraversalDirection::INVERSE),"",TraversalDirection::INVERSE), "in"));
        if (mergeInputUids.empty())
            continue;

        // Now we can instantiate and connect
        Hyperedges uid(instantiateComponent(Hyperedges{Graph::EdgeId}, fromLabel+"_"+fromOutputLabel+"_to_"+toLabel+"_"+toInputLabel));
        dependsOn(inputsOf(uid,"in"), fromNodeOutputIds);
        dependsOn(mergeInputUids, outputsOf(uid,"out"));
        partOfComponent(uid, Hyperedges{modelUid});

        // Update weight value
        Hyperedges weightValueUids(valuesOf(inputsOf(uid, "weight")));
        for (const UniqueId weightValueUid : weightValueUids)
        {
            access(weightValueUid).updateLabel(weight);
        }
    }

    return modelUid;
}

//std::string Graph::floatToStdLogicVector(const float value)
//{
//    char buf[20];
//    unsigned char *ptr = (unsigned char *)&value;
//    std::snprintf(buf, 20, "x\"%.2x%.2x%.2x%.2x\"", ptr[3], ptr[2], ptr[1], ptr[0]);
//    return std::string(buf);
//}

//std::string Graph::floatToC(const float value)
//{
//    return std::to_string(value)+"f";
//}


}
