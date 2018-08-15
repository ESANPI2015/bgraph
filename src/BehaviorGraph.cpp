#include "BehaviorGraph.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <sstream>

namespace Behavior {

const UniqueId Graph::NodeId = "Behavior::Graph::Node";
const UniqueId Graph::MergeId = "Behavior::Graph::Merge";
const UniqueId Graph::EdgeId = "Behavior::Graph::Edge";
const UniqueId Graph::InterfaceId = "Behavior::Graph::Interface";
const UniqueId Graph::InputId = "Behavior::Graph::Input";
const UniqueId Graph::OutputId = "Behavior::Graph::Output";
const UniqueId Graph::CId = "Behavior::Graph::Interface::C";
const UniqueId Graph::VHDLId = "Behavior::Graph::Interface::VHDL";
const UniqueId Graph::ExternId = "Behavior::Graph::Node::Extern";
const UniqueId Graph::SubgraphId = "Behavior::Graph::Node::Subgraph";
const UniqueId Graph::CValueId = "Behavior::Graph::Interface::Value::C";
const UniqueId Graph::VHDLValueId = "Behavior::Graph::Interface::Value::VHDL";

Graph::Graph()
{
    setupMetaModel();
}

Graph::Graph(const Hypergraph& base)
: Software::Graph(base)
{
    setupMetaModel();
}

Graph::~Graph()
{
}

void Graph::setupMetaModel()
{
    Hyperedges uid, inputIds;
    const unsigned maxMergeInputs = 10;
    // Three main superclasses of the behavior graph domain:
    // NODE -- EDGE -- MERGE -- NODE ... pattern
    createAlgorithm(Graph::NodeId, "NODE");
    createAlgorithm(Graph::MergeId, "MERGE");
    createAlgorithm(Graph::EdgeId, "EDGE");
    // Main interfaces:
    // One common interface type, two main input/output classes
    createInterface(Graph::InterfaceId, "IEEE754::binary32");
    uid = createInput(Graph::InputId, "bg_input_t");
    uid = unite(uid, createOutput(Graph::OutputId, "bg_output_t"));
    isA(uid, Hyperedges{Graph::InterfaceId});

    // Datatypes:
    // In C: float
    // In VHDL: 32 Bit std_logic_vector
    uid = createInterface(Graph::CId, "float");
    uid = unite(uid, createInterface(Graph::VHDLId, "std_logic_vector(31 downto 0)"));
    isA(uid, Hyperedges{Graph::InterfaceId, Graph::InputId, Graph::OutputId});

    // Values:
    // One class for C and one for VHDL values
    // TODO: Do we need a toplvl entity?
    createValue(Graph::CValueId, "C value");
    createValue(Graph::VHDLValueId, "VHDL value");

    // The EDGE algorithm has
    // one input: in
    needsInterface(Hyperedges{Graph::EdgeId}, instantiateFrom(Hyperedges{Graph::InputId}, "in"));
    // one output: out
    providesInterface(Hyperedges{Graph::EdgeId}, instantiateFrom(Hyperedges{Graph::OutputId}, "out"));

    // MERGE class interfaces (the subclasses inherit them)
    inputIds.clear();
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "defaultValue"));
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "bias"));
    for (unsigned index = 0; index < maxMergeInputs; index++)
        inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, std::to_string(index)));
    needsInterface(Hyperedges{Graph::MergeId}, inputIds);
    providesInterface(Hyperedges{Graph::MergeId}, instantiateFrom(Hyperedges{Graph::OutputId}, "merged"));
    // The different subclasses of MERGE
    createAlgorithm("Behavior::Graph::Merge::Sum", "SUM", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Product", "PRODUCT", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Min", "MIN", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Max", "MAX", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Mean", "MEAN", Hyperedges{Graph::MergeId});
    createAlgorithm("Behavior::Graph::Merge::Norm", "NORM", Hyperedges{Graph::MergeId});

    // The NODE class subalgorithms
    createAlgorithm(Graph::ExternId, "EXTERN", Hyperedges{Graph::NodeId});
    createAlgorithm(Graph::SubgraphId, "SUBGRAPH", Hyperedges{Graph::NodeId});
    isA(Hyperedges{Graph::SubgraphId}, Hyperedges{Component::Network::NetworkId});

    // Subclassing by arity makes sense
    uid = createAlgorithm("Behavior::Graph::Node::1-1", "arity1", Hyperedges{Graph::NodeId});
    inputIds.clear();
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "0"));
    needsInterface(uid, inputIds);
    providesInterface(uid, instantiateFrom(Hyperedges{Graph::OutputId}, "0"));
    createAlgorithm("Behavior::Graph::Node::Pipe", "PIPE", uid);
    createAlgorithm("Behavior::Graph::Node::Input", "INPUT", uid);
    createAlgorithm("Behavior::Graph::Node::Output", "OUTPUT", uid);
    createAlgorithm("Behavior::Graph::Node::Divide", "DIVIDE", uid);
    createAlgorithm("Behavior::Graph::Node::Sine", "SIN", uid);
    createAlgorithm("Behavior::Graph::Node::Cosine", "COS", uid);
    createAlgorithm("Behavior::Graph::Node::Tangens", "TAN", uid);
    createAlgorithm("Behavior::Graph::Node::TangensHyperbolicus", "TANH", uid);
    createAlgorithm("Behavior::Graph::Node::ArcusCosine", "ACOS", uid);
    createAlgorithm("Behavior::Graph::Node::ArcusSine", "ASIN", uid);
    createAlgorithm("Behavior::Graph::Node::ArcusTangens", "ATAN", uid);
    createAlgorithm("Behavior::Graph::Node::Logarithm", "LOG", uid);
    createAlgorithm("Behavior::Graph::Node::Exponential", "EXP", uid);
    createAlgorithm("Behavior::Graph::Node::Absolute", "ABS", uid);
    createAlgorithm("Behavior::Graph::Node::SquareRoot", "SQRT", uid);
    // 2-to-1
    uid = createAlgorithm("Behavior::Graph::Node::2-1", "arity2", Hyperedges{Graph::NodeId});
    inputIds.clear();
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "0"));
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "1"));
    needsInterface(uid, inputIds);
    providesInterface(uid, instantiateFrom(Hyperedges{Graph::OutputId}, "0"));
    createAlgorithm("Behavior::Graph::Node::ArcusTangens2D", "ATAN2", uid);
    createAlgorithm("Behavior::Graph::Node::Power", "POW", uid);
    createAlgorithm("Behavior::Graph::Node::Modulo", "MOD", uid);
    // 3-to-1
    uid = createAlgorithm("Behavior::Graph::Node::3-1", "arity3", Hyperedges{Graph::NodeId});
    inputIds.clear();
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "0"));
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "1"));
    inputIds=unite(inputIds,instantiateFrom(Hyperedges{Graph::InputId}, "2"));
    needsInterface(uid, inputIds);
    providesInterface(uid, instantiateFrom(Hyperedges{Graph::OutputId}, "0"));
    createAlgorithm("Behavior::Graph::Node::GreaterZero", ">0", uid);
    createAlgorithm("Behavior::Graph::Node::ApproxZero", "==0", uid);
}

std::string Graph::domainSpecificExport(const UniqueId& uid)
{
    std::map< UniqueId,unsigned > unique2nodeId;
    std::stringstream ss;
    YAML::Node spec;

    // Store toplvl model name
    spec["model"] = get(uid)->label();
    // Handle nodes
    YAML::Node nodesYAML(spec["nodes"]);
    // Important queries
    Hyperedges allParts(componentsOf(Hyperedges{uid}));
    Hyperedges allDefaultValues(inputs("defaultValue"));
    Hyperedges allBiasValues(inputs("bias"));
    Hyperedges allInputs(inputs());
    Hyperedges allOutputs(outputs());
    Hyperedges superClassesOfAllNodes(algorithmClasses("",Hyperedges{Graph::NodeId}));
    Hyperedges nodes(intersect(instancesOf(superClassesOfAllNodes), allParts));
    // TODO: Shall we preserve ids? As we did in hwgraph lib?
    unsigned nodeId = 1;
    for (const UniqueId& nodeUid : nodes)
    {
        YAML::Node nodeYAML;
        Hyperedges superClasses(instancesOf(Hyperedges{nodeUid}, "", TraversalDirection::FORWARD));
        Hyperedges superSuperClasses(subclassesOf(superClasses,"", TraversalDirection::FORWARD));
        if (superClasses.empty())
        {
            std::cout << "No superclass for " << nodeUid << "\n";
            continue;
        }
        // Handle interfaces
        Hyperedges interfaceUids(interfacesOf(Hyperedges{nodeUid}));
        YAML::Node inputsYAML(nodeYAML["inputs"]);
        for (const UniqueId& inputUid : intersect(interfaceUids, allInputs))
        {
            YAML::Node inputYAML;
            inputYAML["name"] = get(inputUid)->label();
            Hyperedges mergeUids(getMergesOfInput(Hyperedges{inputUid}));
            if (!mergeUids.size())
            {
                std::cout << "No merge for " << get(inputUid)->label() << std::endl;
            }
            for (const UniqueId& mergeUid : mergeUids)
            {
                inputYAML["type"] = get(mergeUid)->label();
                Hyperedges defaultValueUids(valuesOf(intersect(allDefaultValues, interfacesOf(Hyperedges{mergeUid}))));
                Hyperedges biasValueUids(valuesOf(intersect(allBiasValues, interfacesOf(Hyperedges{mergeUid}))));
                for (const UniqueId& defUid : defaultValueUids)
                {
                    inputYAML["default"] = get(defUid)->label();
                    for (const UniqueId& biasUid : biasValueUids)
                    {
                        inputYAML["bias"] = get(biasUid)->label();
                        inputsYAML.push_back(inputYAML);
                    }
                }
            }
        }
        YAML::Node outputsYAML(nodeYAML["outputs"]);
        for (const UniqueId& outputUid : intersect(interfaceUids, allOutputs))
        {
            YAML::Node outputYAML;
            outputYAML["name"] = get(outputUid)->label();
            outputsYAML.push_back(outputYAML);
        }
        nodeYAML["id"] = nodeId;
        nodeYAML["name"] = get(nodeUid)->label();
        // Special cases for SUBGRAPH && EXTERN
        if (std::find(superSuperClasses.begin(), superSuperClasses.end(), Graph::SubgraphId) != superSuperClasses.end())
        {
            nodeYAML["type"] = get(Graph::SubgraphId)->label();
            nodeYAML["subgraph_name"] = get(*superClasses.begin())->label();
        }
        else if (std::find(superSuperClasses.begin(), superSuperClasses.end(), Graph::ExternId) != superSuperClasses.end())
        {
            nodeYAML["type"] = get(Graph::ExternId)->label();
            nodeYAML["extern_name"] = get(*superClasses.begin())->label();
        } else {
            nodeYAML["type"] = get(*superClasses.begin())->label();
        }
        nodesYAML.push_back(nodeYAML);
        unique2nodeId[nodeUid] = nodeId++;
    }

    // Handle edges
    YAML::Node edgesYAML(spec["edges"]);
    Hyperedges edges(intersect(instancesOf(algorithmClasses("",Hyperedges{Graph::EdgeId})), allParts));
    for (const UniqueId& edgeUid : edges)
    {
        YAML::Node edgeYAML;
        edgeYAML["weight"] = std::stof(get(edgeUid)->label());
        // Get edge input and follow chain
        Hyperedges fromOutputUids(endpointsOf(interfacesOf(Hyperedges{edgeUid},"in"),"",TraversalDirection::INVERSE));
        Hyperedges toMergeInputUids(endpointsOf(interfacesOf(Hyperedges{edgeUid},"out")));
        Hyperedges toMergeNodeUids(interfacesOf(toMergeInputUids,"",TraversalDirection::INVERSE));
        Hyperedges toMergeOutputUids(interfacesOf(toMergeNodeUids, "merged"));
        Hyperedges toInputUids(endpointsOf(toMergeOutputUids));
        for (const UniqueId& fromOutputUid : fromOutputUids)
        {
            // TODO: If label can be converted to a unsigned int ... use fromNodeOutputIdx
            edgeYAML["fromNodeOutput"] = get(fromOutputUid)->label();
            Hyperedges fromNodeUids(interfacesOf(Hyperedges{fromOutputUid},"",TraversalDirection::INVERSE));
            for (const UniqueId& fromNodeUid : fromNodeUids)
            {
                edgeYAML["fromNode"] = get(fromNodeUid)->label();
                // Get edge output and follow chain
                // Here we will get to a merge in between!!!
                for (const UniqueId& toInputUid : toInputUids)
                {
                    edgeYAML["toNodeInput"] = get(toInputUid)->label();
                    Hyperedges toNodeUids(interfacesOf(Hyperedges{toInputUid},"",TraversalDirection::INVERSE));
                    for (const UniqueId& toNodeUid : toNodeUids)
                    {
                        edgeYAML["toNode"] = get(toNodeUid)->label();
                        edgesYAML.push_back(edgeYAML);
                    }
                }
            }
        }
    }

    ss << spec;
    return ss.str();
}

Hyperedges Graph::getMergesOfInput(const Hyperedges& inputs, const std::string& label)
{
    Hyperedges allMerges = instancesOf(algorithmClasses("", Hyperedges{Graph::MergeId}), label);
    Hyperedges allEndpoints = endpointsOf(inputs, "merged", Software::Graph::TraversalDirection::INVERSE); // all outputs of some merges?
    Hyperedges allParentsOfEndpoints = childrenOf(allEndpoints, label, Software::Graph::TraversalDirection::INVERSE);
    return intersect(allMerges, allParentsOfEndpoints);
}

std::string Graph::floatToStdLogicVector(const float value)
{
    char buf[20];
    unsigned char *ptr = (unsigned char *)&value;
    std::snprintf(buf, 20, "x\"%.2x%.2x%.2x%.2x\"", ptr[3], ptr[2], ptr[1], ptr[0]);
    return std::string(buf);
}

std::string Graph::floatToC(const float value)
{
    return std::to_string(value)+"f";
}

Hyperedges Graph::createOrFindValueClassesFor(const std::string& value)
{
    Hyperedges vClasses(valueClasses(value));
    if (!vClasses.size())
    {
        std::cout << "Creating abstract value class for " << value << "\n";
        vClasses = unite(vClasses, createValue(Component::Network::ValueId+"::"+value, value));
        // * Generate a SPECIFIC value classes for each language (whereas the C Class is just a copy)
        isA(createValue(Graph::CValueId+"::"+value, floatToC(std::stof(value)), vClasses), Hyperedges{Graph::CValueId});
        // * In the VHDL case, call floatToStdLogicVector()
        isA(createValue(Graph::VHDLValueId+"::"+value, floatToStdLogicVector(std::stof(value)), vClasses), Hyperedges{Graph::VHDLValueId});
    }
    return vClasses;
}

bool Graph::domainSpecificImport(const std::string& serialized)
{
    // Map mergeUid to index
    std::map<UniqueId, unsigned> mergeIndex;
    // Map from old ids (in file) to new ids
    std::map<UniqueId, Hyperedges> old2new;
    YAML::Node spec = YAML::Load(serialized);
    // Create a network class
    std::string name("NONAME");
    if (spec["model"].IsDefined())
        name = spec["model"].as<std::string>();
    Hyperedges partsOfNet;
    Hyperedges networkUid(createNetwork(Graph::SubgraphId+"::"+name, name, Hyperedges{Graph::SubgraphId}));
    if (!spec["nodes"].IsDefined())
        return false;
    const YAML::Node& nodesYAML(spec["nodes"]);
    if (nodesYAML.IsDefined())
    {
        for (YAML::Node::const_iterator nit = nodesYAML.begin(); nit != nodesYAML.end(); ++nit)
        {
            const YAML::Node& nodeYAML(*nit);
            const UniqueId& id(nodeYAML["id"].as<UniqueId>());
            const std::string& type(nodeYAML["type"].as<std::string>());
            Hyperedges super(algorithmClasses(type));
            if (super.empty())
            {
                std::cout << "Warning, superclass " << type << " not found\n";
                continue;
            }
            std::string label(id);
            if (nodeYAML["name"].IsDefined())
                label = nodeYAML["name"].as<std::string>();
            const YAML::Node& inputsYAML(nodeYAML["inputs"]);
            const YAML::Node& outputsYAML(nodeYAML["outputs"]);
            const YAML::Node& subgraph_nameYAML(nodeYAML["subgraph_name"]);
            const YAML::Node& extern_nameYAML(nodeYAML["extern_name"]);

            // For SUBGRAPH and EXTERN nodes we first have to create a subclass & interfaces (if it does not exist yet)
            if ((type == "SUBGRAPH") || (type == "EXTERN"))
            {
                // Define the superclassLabel
                std::string superLabel(label);
                if (subgraph_nameYAML.IsDefined())
                    superLabel = subgraph_nameYAML.as<std::string>();
                else if (extern_nameYAML.IsDefined())
                    superLabel = extern_nameYAML.as<std::string>();
                // super contains now either SUBGRAPH || EXTERN
                Hyperedges sub(algorithmClasses(superLabel, super));
                if (sub.empty())
                {
                    // First occurrence: Create a subclass with interfaces
                    std::cout << "Create subclass " << superLabel << " of type " << type << "\n";
                    sub = createAlgorithm(superLabel, superLabel, super);
                }
                // Create interfaces
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
                        Hyperedges inputOfComponent(intersect(inputs(inputLabel), interfacesOf(sub, inputLabel)));
                        if (inputOfComponent.size())
                            continue;
                        std::cout << "Create input " << inputLabel << " for " << superLabel << " node\n";
                        needsInterface(sub, instantiateInterfaceFor(sub, Hyperedges{Graph::InputId}, inputLabel));
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
                        Hyperedges outputOfComponent(intersect(outputs(outputLabel), interfacesOf(sub, outputLabel)));
                        if (outputOfComponent.size())
                            continue;
                        std::cout << "Create output " << outputLabel << " for " << superLabel << " node\n";
                        providesInterface(sub, instantiateInterfaceFor(sub, Hyperedges{Graph::OutputId}, outputLabel));
                    }
                }
                // The new sub class is the super class of the following code
                super = sub;
            }

            // Instantiate node
            std::cout << "Instantiating " << label << " of type " << super << "\n";
            Hyperedges uid(instantiateComponent(super, label));
            old2new[id] = uid;
            partsOfNet = unite(partsOfNet, uid);

            // For every input, create and connect the correct MERGE algorithm
            if (inputsYAML.IsDefined())
            {
                for (YAML::Node::const_iterator it = inputsYAML.begin(); it != inputsYAML.end(); it++)
                {
                    // Create input
                    const YAML::Node& inputYAML(*it);
                    // Search by string or idx
                    std::string inputLabel;
                    if (inputYAML["name"].IsDefined())
                    {
                        inputLabel = inputYAML["name"].as<std::string>();
                    } else {
                        inputLabel = inputYAML["idx"].as<std::string>();
                    }
                    // Search for input
                    Hyperedges inputOfComponent(intersect(inputs(inputLabel), interfacesOf(uid, inputLabel)));
                    if (!inputOfComponent.size())
                    {
                        std::cout << "Could not find input " << inputLabel << " for " << label << " node\n";
                        continue;
                    }
                    // For INPUT nodes: Make their input(s) the input(s) of the toplvl node
                    // Also, suppress merge creation
                    if (type == "INPUT")
                    {
                        for (const UniqueId& inputUid : inputOfComponent)
                        {
                            std::cout << "Export input " << inputUid << " by creating alias interface named " << label << std::endl;
                            needsInterface(networkUid, instantiateAliasInterfaceFor(networkUid, Hyperedges{inputUid}, label));
                        }
                        std::cout << "Skipping merge creation for " << label << " node\n";
                        continue;
                    }

                    // Create merge
                    const std::string& mergeType(inputYAML["type"].as<std::string>());
                    const std::string& mergeBias(inputYAML["bias"].as<std::string>());
                    const std::string& mergeDefault(inputYAML["default"].as<std::string>());
                    // Search for merge class
                    Hyperedges mergeClasses(algorithmClasses(mergeType, Hyperedges{Graph::MergeId}));
                    if (!mergeClasses.size())
                    {
                        std::cout << "Warning, could not find merge type " << mergeType << "\n";
                        continue;
                    }
                    // NOTE: The merge NEVER exists at this point (even if the input exists)
                    // Instantiate merge
                    std::cout << "Instantiating merge " << mergeType << " for input " << inputLabel << "\n";
                    Hyperedges mergeUid(instantiateComponent(mergeClasses));
                    partsOfNet = unite(partsOfNet, mergeUid);
                    Hyperedges outputOfMerge(intersect(interfacesOf(mergeUid, "merged"), outputs("merged")));
                    // Assign values to defaultValue and bias!
                    Hyperedges vClasses(createOrFindValueClassesFor(mergeDefault));
                    Hyperedges interfacesOfMerge(interfacesOf(mergeUid));
                    Hyperedges defaultUids(intersect(interfacesOfMerge, inputs("defaultValue")));
                    Hyperedges defaultValueUids(instantiateValueFor(defaultUids, vClasses));
                    for (const UniqueId& defUid : defaultValueUids)
                    {
                        std::cout << "Assinging default value " << mergeDefault << "\n";
                        get(defUid)->updateLabel(mergeDefault);
                    }
                    vClasses = createOrFindValueClassesFor(mergeBias);
                    Hyperedges biasUids(intersect(interfacesOfMerge, inputs("bias")));
                    Hyperedges biasValueUids(instantiateValueFor(biasUids, vClasses));
                    for (const UniqueId& biasUid : biasValueUids)
                    {
                        std::cout << "Assinging bias value " << mergeBias << "\n";
                        get(biasUid)->updateLabel(mergeBias);
                    }
                    std::cout << dependsOn(inputOfComponent, outputOfMerge) << "\n";
                }
            }

            // Create specified outputs
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
                    Hyperedges outputOfComponent(intersect(outputs(outputLabel), interfacesOf(uid, outputLabel)));
                    if (!outputOfComponent.size())
                    {
                        std::cout << "Could not find output " << outputLabel << " for " << label << " node\n";
                        continue;
                    }
                    // For OUTPUT nodes: Make their output(s) the output(s) of the toplvl node
                    if (type == "OUTPUT")
                    {
                        for (const UniqueId& outputUid : outputOfComponent)
                        {
                            std::cout << "Export output " << outputUid << " by creating alias interface named " << label << std::endl;
                            providesInterface(networkUid, instantiateAliasInterfaceFor(networkUid, Hyperedges{outputUid}, label));
                        }
                    }
                }
            }
        }
    } else {
        std::cout << "No nodes section\n";
        return false;
    }

    // Treat edges
    const YAML::Node& edgesYAML(spec["edges"]);
    if (edgesYAML.IsDefined())
    {
        for (YAML::Node::const_iterator eit = edgesYAML.begin(); eit != edgesYAML.end(); ++eit)
        {
            const YAML::Node& edgeYAML(*eit);
            const std::string& label(edgeYAML["weight"].as<std::string>());
            std::string fromLabel, fromOutputLabel, toLabel, toInputLabel;
            // We can also connect by names, right? Yes :)
            Hyperedges fromNodeIds;
            if (edgeYAML["fromNode"].IsDefined())
            {
                // Find UnqiueIds by label
                fromLabel = edgeYAML["fromNode"].as<std::string>();
                fromNodeIds = algorithms(fromLabel);
            } else {
                // Find UniqueIds by remapped ids
                fromLabel = edgeYAML["fromNodeId"].as<std::string>();
                fromNodeIds = old2new[fromLabel];
            }
            if (fromNodeIds.empty())
            {
                std::cout << "Could not find node " << fromLabel << "\n";
                continue;
            }
            Hyperedges toNodeIds;
            if (edgeYAML["toNode"].IsDefined())
            {
                // Find UnqiueIds by label
                toLabel = edgeYAML["toNode"].as<std::string>();
                toNodeIds = algorithms(toLabel);
            } else {
                // Find UniqueIds by remapped ids
                toLabel = edgeYAML["toNodeId"].as<std::string>();
                toNodeIds = old2new[toLabel];
            }
            if (toNodeIds.empty())
            {
                std::cout << "Could not find node " << toLabel << "\n";
                continue;
            }
            if (edgeYAML["fromNodeOutput"].IsDefined())
            {
                // Find UnqiueIds by label
                fromOutputLabel = edgeYAML["fromNodeOutput"].as<std::string>();
            } else {
                // Find UniqueIds by remapped ids
                fromOutputLabel = edgeYAML["fromNodeOutputIdx"].as<std::string>();
            }
            Hyperedges fromNodeOutputIds(intersect(interfacesOf(fromNodeIds, fromOutputLabel), outputs(fromOutputLabel)));
            if (fromNodeOutputIds.empty())
            {
                std::cout << "Node " << fromLabel << " has no output " << fromOutputLabel << "\n";
                continue;
            }
            if (edgeYAML["toNodeInput"].IsDefined())
            {
                // Find UnqiueIds by label
                toInputLabel = edgeYAML["toNodeInput"].as<std::string>();
            } else {
                // Find UniqueIds by remapped ids
                toInputLabel = edgeYAML["toNodeInputIdx"].as<std::string>();
            }
            Hyperedges toNodeInputIds(intersect(interfacesOf(toNodeIds, toInputLabel), inputs(toInputLabel)));
            if (toNodeInputIds.empty())
            {
                std::cout << "Node " << toLabel << " has no input " << toInputLabel << "\n";
                continue;
            }

            //  Wire free merge inputs
            //  However, we have to create indices. Otherwise we could not identify them properly
            //  That means, that we have to keep track on the current index per mergeId
            Hyperedges mergeIds(getMergesOfInput(toNodeInputIds));
            Hyperedges unconnectedInputs;
            for (const UniqueId& mergeId : mergeIds)
            {
                Hyperedges inputsOfMerge(intersect(inputs(), interfacesOf(Hyperedges{mergeId})));
                for (const UniqueId& mergeInput : inputsOfMerge)
                {
                    // Do not connect defaultValue or bias
                    if (get(mergeInput)->label() == "defaultValue")
                        continue;
                    if (get(mergeInput)->label() == "bias")
                        continue;
                    Hyperedges others(endpointsOf(Hyperedges{mergeInput},"",Hypergraph::TraversalDirection::INVERSE));
                    if (!others.size())
                    {
                        unconnectedInputs = unite(unconnectedInputs, Hyperedges{mergeInput});
                        break;
                    }
                }
                if (unconnectedInputs.size())
                    break;
            }

            // Now we can instantiate and connect
            std::cout << "Connecting " << fromLabel << "," << fromOutputLabel << " to " << toLabel << "," << toInputLabel << "\n";
            Hyperedges uid(instantiateComponent(Hyperedges{Graph::EdgeId}, label));
            partsOfNet = unite(partsOfNet, uid);
            std::cout << dependsOn(interfacesOf(uid,"in"), fromNodeOutputIds) << " ";
            std::cout << dependsOn(unconnectedInputs, interfacesOf(uid,"out")) << "\n";
        }
    }
    // Make all stuff part of the network
    partOfNetwork(partsOfNet, networkUid);
    return true;
}

}
