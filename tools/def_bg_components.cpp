#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <fstream>
#include <sstream>

int main(void)
{
    // NOTE: Everything in here could be a constructor of a derived class!
    Software::Graph swgraph;

    // Some constants
    //const unsigned int maxMergeInputs = 256;
    const unsigned int maxMergeInputs = 10;
    //const unsigned int maxInputPorts = 512;

    // Supported Languages
    auto cId = swgraph.instantiateFrom(swgraph.createLanguage("C"));
    auto vhdlId = swgraph.instantiateFrom(swgraph.createLanguage("VHDL"));

    // Only interface: reals
    auto realId = swgraph.createInterface("RealNumber");

    // In C: float32
    // TODO: Shouldn't these be instances?
    auto floatId = swgraph.createDatatype("float");
    auto uintId = swgraph.createDatatype("uint8_t");

    // In VHDL: std_logic_vector(31 downto 0)
    // TODO: Shouldn't these be instances?
    auto stdvecId = swgraph.createDatatype("std_logic_vector(31 downto 0)");
    swgraph.represents(unite(floatId, stdvecId), realId);
    swgraph.expressedIn(floatId, cId);
    swgraph.expressedIn(stdvecId, vhdlId);

    // Input/Output classes
    auto inputClassId = swgraph.createInput(realId, "RealInput");
    auto outputClassId = swgraph.createOutput(realId, "RealOutput");

    // Upper classes:
    // NODE class represents the primitive nodes of the behavior graph domain
    // MERGE class represents the merge functions which preprocess the incoming data before it is used by nodes
    // EDGE class represents the computations performed by edges - currently this should be a simple multiplication
    auto nodeClassId = swgraph.createAlgorithm("NODE");
    auto mergeClassId = swgraph.createAlgorithm("MERGE");
    auto edgeClassId = swgraph.createAlgorithm("EDGE");

    // The NODE class
    // Build up the behavior graph components aka Nodes
    // 1-to-1
    Hyperedges id1, id2;
    auto pipeId = swgraph.createAlgorithm("PIPE");
    swgraph.isA(pipeId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(pipeId, id1);
    swgraph.provides(pipeId, id2);

    auto divId = swgraph.createAlgorithm("DIVIDE");
    swgraph.isA(divId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(divId, id1);
    swgraph.provides(divId, id2);

    auto sinId = swgraph.createAlgorithm("SIN");
    swgraph.isA(sinId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(sinId, id1);
    swgraph.provides(sinId, id2);

    auto cosId = swgraph.createAlgorithm("COS");
    swgraph.isA(cosId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(cosId, id1);
    swgraph.provides(cosId, id2);

    auto tanId = swgraph.createAlgorithm("TAN");
    swgraph.isA(tanId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(tanId, id1);
    swgraph.provides(tanId, id2);

    auto tanhId = swgraph.createAlgorithm("TANH");
    swgraph.isA(tanhId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(tanhId, id1);
    swgraph.provides(tanhId, id2);

    auto acosId = swgraph.createAlgorithm("ACOS");
    swgraph.isA(acosId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(acosId, id1);
    swgraph.provides(acosId, id2);

    auto asinId = swgraph.createAlgorithm("ASIN");
    swgraph.isA(asinId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(asinId, id1);
    swgraph.provides(asinId, id2);

    auto atanId = swgraph.createAlgorithm("ATAN");
    swgraph.isA(atanId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(atanId, id1);
    swgraph.provides(atanId, id2);

    auto logId = swgraph.createAlgorithm("LOG");
    swgraph.isA(logId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(logId, id1);
    swgraph.provides(logId, id2);

    auto expId = swgraph.createAlgorithm("EXP");
    swgraph.isA(expId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(expId, id1);
    swgraph.provides(expId, id2);

    auto absId = swgraph.createAlgorithm("ABS");
    swgraph.isA(absId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(absId, id1);
    swgraph.provides(absId, id2);

    auto sqrtId = swgraph.createAlgorithm("SQRT");
    swgraph.isA(sqrtId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(sqrtId, id1);
    swgraph.provides(sqrtId, id2);

    // 2-to-1
    Hyperedges id3;
    auto atan2Id = swgraph.createAlgorithm("ATAN2");
    swgraph.isA(atan2Id, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateInput(inputClassId, "y");
    id3 = swgraph.instantiateOutput(outputClassId, "z");
    swgraph.needs(atan2Id, unite(id1, id2));
    swgraph.provides(atan2Id, id3);

    auto powId = swgraph.createAlgorithm("POW");
    swgraph.isA(powId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateInput(inputClassId, "y");
    id3 = swgraph.instantiateOutput(outputClassId, "z");
    swgraph.needs(powId, unite(id1, id2));
    swgraph.provides(powId, id3);

    auto modId = swgraph.createAlgorithm("MOD");
    swgraph.isA(modId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateInput(inputClassId, "y");
    id3 = swgraph.instantiateOutput(outputClassId, "z");
    swgraph.needs(modId, unite(id1, id2));
    swgraph.provides(modId, id3);

    // 3-to-1
    Hyperedges id4;
    auto greaterId = swgraph.createAlgorithm(">0");
    swgraph.isA(greaterId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "condition");
    id2 = swgraph.instantiateInput(inputClassId, "true");
    id3 = swgraph.instantiateInput(inputClassId, "false");
    id4 = swgraph.instantiateOutput(outputClassId, "result");
    swgraph.needs(greaterId, unite(unite(id1, id2), id3));
    swgraph.provides(greaterId, id4);

    auto equalId = swgraph.createAlgorithm("==0");
    swgraph.isA(equalId, nodeClassId);
    id1 = swgraph.instantiateInput(inputClassId, "condition");
    id2 = swgraph.instantiateInput(inputClassId, "true");
    id3 = swgraph.instantiateInput(inputClassId, "false");
    id4 = swgraph.instantiateOutput(outputClassId, "result");
    swgraph.needs(equalId, unite(unite(id1, id2), id3));
    swgraph.provides(equalId, id4);

    // The MERGE class
    // (UP-TO-N)-to-1 aka Merges
    // they also have "bias" and "defaultValue"
    Hyperedges inputIds;
    auto sumId = swgraph.createAlgorithm("SUM");
    swgraph.isA(sumId, mergeClassId);
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    for (unsigned i = 0; i < maxMergeInputs; ++i)
    {
        inputIds=unite(inputIds, swgraph.instantiateInput(inputClassId, std::to_string(i)));
    }
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(sumId, inputIds);
    swgraph.provides(sumId, id1);

    inputIds.clear();
    auto prodId = swgraph.createAlgorithm("PRODUCT");
    swgraph.isA(prodId, mergeClassId);
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    for (unsigned i = 0; i < maxMergeInputs; ++i)
    {
        inputIds=unite(inputIds, swgraph.instantiateInput(inputClassId, std::to_string(i)));
    }
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(prodId, inputIds);
    swgraph.provides(prodId, id1);

    inputIds.clear();
    auto minId = swgraph.createAlgorithm("MIN");
    swgraph.isA(minId, mergeClassId);
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    for (unsigned i = 0; i < maxMergeInputs; ++i)
    {
        inputIds=unite(inputIds, swgraph.instantiateInput(inputClassId, std::to_string(i)));
    }
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(minId, inputIds);
    swgraph.provides(minId, id1);

    inputIds.clear();
    auto maxId = swgraph.createAlgorithm("MAX");
    swgraph.isA(maxId, mergeClassId);
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    for (unsigned i = 0; i < maxMergeInputs; ++i)
    {
        inputIds=unite(inputIds, swgraph.instantiateInput(inputClassId, std::to_string(i)));
    }
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(maxId, inputIds);
    swgraph.provides(maxId, id1);

    inputIds.clear();
    auto meanId = swgraph.createAlgorithm("MEAN");
    swgraph.isA(meanId, mergeClassId);
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    for (unsigned i = 0; i < maxMergeInputs; ++i)
    {
        inputIds=unite(inputIds, swgraph.instantiateInput(inputClassId, std::to_string(i)));
    }
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(meanId, inputIds);
    swgraph.provides(meanId, id1);

    inputIds.clear();
    auto normId = swgraph.createAlgorithm("NORM");
    swgraph.isA(normId, mergeClassId);
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    for (unsigned i = 0; i < maxMergeInputs; ++i)
    {
        inputIds=unite(inputIds, swgraph.instantiateInput(inputClassId, std::to_string(i)));
    }
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(normId, inputIds);
    swgraph.provides(normId, id1);

    // The EDGE class
    inputIds.clear();
    auto edgeId = swgraph.createAlgorithm("EDGE");
    swgraph.isA(edgeId, edgeClassId);
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "in"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "weight"));
    id1 = swgraph.instantiateOutput(outputClassId, "out");
    swgraph.needs(edgeId, inputIds);
    swgraph.provides(edgeId, id1);

    // Special CLASSES
    // Extern behavior graph algorithms contain custom implementations
    // This means it is just a class with no interfaces on itself
    auto externClassId = swgraph.createAlgorithm("EXTERN");
    swgraph.isA(externClassId, nodeClassId);

    // Subgraph behavior graph algorithms contain compositions of other behavior graph nodes
    // Again, this superclass has no interfaces on its own
    auto subgraphClassId = swgraph.createAlgorithm("SUBGRAPH");
    swgraph.isA(subgraphClassId, nodeClassId);

    // Here is the end of the language definition
    // For the language to become usable, we now have to provide implementations for each of the atomic nodes for each of the languages
    // In addition to that, the relation DEPENDS-ON has to be interpreted as well ... It will trigger the generation of GLUE CODE
    // Actually, for each DEPENDS-ON a 1-TO-1 IMPLEMENTATION has to be given which takes care about passing information from A to B
    // For this IMPLEMENTATION we have 4 different possibilities: passing data between C components, passing data between VHDL components and 2 interlanguage versions
    // However, the generation of GLUE is only possible if SW to HW mapping is available.
    // In VHDL: just wiring including activation
    // In C: passing values and calling functions
    // Furthermore, this amount will also increase whether components are executed in the same context/hardware or not!

    // Store the setup in a YAML for inspection
    YAML::Node test;
    test = static_cast<Hypergraph*>(&swgraph);

    std::ofstream fout;
    fout.open("def_bg.yml");
    if(fout.good()) {
        fout << test;
    } else {
        std::cout << "FAILED\n";
    }
    fout.close();
}
