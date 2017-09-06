#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <fstream>
#include <sstream>

int main(void)
{
    // NOTE: Everything in here could be a constructor of a derived class!
    Software::Graph swgraph;

    // Some constants
    const unsigned int maxMergeInputs = 10;

    // Supported Languages
    auto cId = swgraph.instantiateFrom(swgraph.createLanguage("C"));
    auto vhdlId = swgraph.instantiateFrom(swgraph.createLanguage("VHDL"));

    // Only interface: reals
    // TODO: Shouldn't these be instances?
    auto realId = swgraph.createInterface("RealNumber");
    auto realVecId = swgraph.createInterface("RealNumberVec");
    auto numOfEntriesId = swgraph.createInterface("RealNumberVecEntries");
    // In C: float32
    // TODO: Shouldn't these be instances?
    auto floatId = swgraph.createDatatype("float");
    auto floatVecId = swgraph.createDatatype("float[10]");
    auto uintId = swgraph.createDatatype("uint8_t");
    // In VHDL: std_logic_vector(31 downto 0)
    // TODO: Shouldn't these be instances?
    auto stdvecId = swgraph.createDatatype("std_logic_vector(31 downto 0)");
    auto stdvecVecId = swgraph.createDatatype("array (0 to 9) of std_logic_vector(31 downto 0)");
    auto uintvecId = swgraph.createDatatype("std_logic_vector(7 downto 0)");
    swgraph.represents(unite(floatId, stdvecId), realId);
    swgraph.represents(unite(floatVecId, stdvecVecId), realVecId);
    swgraph.represents(unite(uintId, uintvecId), numOfEntriesId);
    swgraph.expressedIn(unite(floatId, unite(floatVecId, uintId)), cId);
    swgraph.expressedIn(unite(stdvecId, unite(stdvecVecId, uintvecId)), vhdlId);

    // Input/Output classes
    auto inputClassId = swgraph.createInput(realId, "RealInput");
    auto mergeClassId = swgraph.createInput(realVecId, "RealVecInput");
    auto entriesClassId = swgraph.createInput(numOfEntriesId, "VecEntries");
    auto outputClassId = swgraph.createOutput(realId, "RealOutput");

    // Build up the behavior graph components aka Nodes
    // 1-to-1
    Hyperedges id1, id2;
    auto pipeId = swgraph.createAlgorithm("PIPE");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(pipeId, id1);
    swgraph.provides(pipeId, id2);

    auto divId = swgraph.createAlgorithm("DIVIDE");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(divId, id1);
    swgraph.provides(divId, id2);

    auto sinId = swgraph.createAlgorithm("SIN");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(sinId, id1);
    swgraph.provides(sinId, id2);

    auto cosId = swgraph.createAlgorithm("COS");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(cosId, id1);
    swgraph.provides(cosId, id2);

    auto tanId = swgraph.createAlgorithm("TAN");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(tanId, id1);
    swgraph.provides(tanId, id2);

    auto tanhId = swgraph.createAlgorithm("TANH");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(tanhId, id1);
    swgraph.provides(tanhId, id2);

    auto acosId = swgraph.createAlgorithm("ACOS");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(acosId, id1);
    swgraph.provides(acosId, id2);

    auto asinId = swgraph.createAlgorithm("ASIN");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(asinId, id1);
    swgraph.provides(asinId, id2);

    auto atanId = swgraph.createAlgorithm("ATAN");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(atanId, id1);
    swgraph.provides(atanId, id2);

    auto logId = swgraph.createAlgorithm("LOG");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(logId, id1);
    swgraph.provides(logId, id2);

    auto expId = swgraph.createAlgorithm("EXP");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(expId, id1);
    swgraph.provides(expId, id2);

    auto absId = swgraph.createAlgorithm("ABS");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(absId, id1);
    swgraph.provides(absId, id2);

    auto sqrtId = swgraph.createAlgorithm("SQRT");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateOutput(outputClassId, "y");
    swgraph.needs(sqrtId, id1);
    swgraph.provides(sqrtId, id2);

    // 2-to-1
    Hyperedges id3;
    auto atan2Id = swgraph.createAlgorithm("ATAN2");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateInput(inputClassId, "y");
    id3 = swgraph.instantiateOutput(outputClassId, "z");
    swgraph.needs(atan2Id, unite(id1, id2));
    swgraph.provides(atan2Id, id3);

    auto powId = swgraph.createAlgorithm("POW");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateInput(inputClassId, "y");
    id3 = swgraph.instantiateOutput(outputClassId, "z");
    swgraph.needs(powId, unite(id1, id2));
    swgraph.provides(powId, id3);

    auto modId = swgraph.createAlgorithm("MOD");
    id1 = swgraph.instantiateInput(inputClassId, "x");
    id2 = swgraph.instantiateInput(inputClassId, "y");
    id3 = swgraph.instantiateOutput(outputClassId, "z");
    swgraph.needs(modId, unite(id1, id2));
    swgraph.provides(modId, id3);

    // 3-to-1
    Hyperedges id4;
    auto greaterId = swgraph.createAlgorithm(">0");
    id1 = swgraph.instantiateInput(inputClassId, "condition");
    id2 = swgraph.instantiateInput(inputClassId, "true");
    id3 = swgraph.instantiateInput(inputClassId, "false");
    id4 = swgraph.instantiateOutput(outputClassId, "result");
    swgraph.needs(greaterId, unite(unite(id1, id2), id3));
    swgraph.provides(greaterId, id4);

    auto equalId = swgraph.createAlgorithm("==0");
    id1 = swgraph.instantiateInput(inputClassId, "condition");
    id2 = swgraph.instantiateInput(inputClassId, "true");
    id3 = swgraph.instantiateInput(inputClassId, "false");
    id4 = swgraph.instantiateOutput(outputClassId, "result");
    swgraph.needs(equalId, unite(unite(id1, id2), id3));
    swgraph.provides(equalId, id4);

    // (UP-TO-N)-to-1 aka Merges
    // they also have "bias" and "defaultValue"
    Hyperedges inputIds;
    auto sumId = swgraph.createAlgorithm("SUM");
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "values"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "weights"));
    inputIds=unite(inputIds, swgraph.instantiateInput(entriesClassId, "entries"));
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(sumId, inputIds);
    swgraph.provides(sumId, id1);

    inputIds.clear();
    auto prodId = swgraph.createAlgorithm("PRODUCT");
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "values"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "weights"));
    inputIds=unite(inputIds, swgraph.instantiateInput(entriesClassId, "entries"));
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(prodId, inputIds);
    swgraph.provides(prodId, id1);

    inputIds.clear();
    auto minId = swgraph.createAlgorithm("MIN");
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "values"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "weights"));
    inputIds=unite(inputIds, swgraph.instantiateInput(entriesClassId, "entries"));
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(minId, inputIds);
    swgraph.provides(minId, id1);

    inputIds.clear();
    auto maxId = swgraph.createAlgorithm("MAX");
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "values"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "weights"));
    inputIds=unite(inputIds, swgraph.instantiateInput(entriesClassId, "entries"));
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(maxId, inputIds);
    swgraph.provides(maxId, id1);

    inputIds.clear();
    auto meanId = swgraph.createAlgorithm("MEAN");
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "values"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "weights"));
    inputIds=unite(inputIds, swgraph.instantiateInput(entriesClassId, "entries"));
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(meanId, inputIds);
    swgraph.provides(meanId, id1);

    inputIds.clear();
    auto normId = swgraph.createAlgorithm("NORM");
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "defaultValue"));
    inputIds=unite(inputIds,swgraph.instantiateInput(inputClassId, "bias"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "values"));
    inputIds=unite(inputIds, swgraph.instantiateInput(mergeClassId, "weights"));
    inputIds=unite(inputIds, swgraph.instantiateInput(entriesClassId, "entries"));
    id1 = swgraph.instantiateOutput(outputClassId, "merged");
    swgraph.needs(normId, inputIds);
    swgraph.provides(normId, id1);

    // Special CLASSES
    // Extern behavior graph algorithms contain custom implementations
    // This means it is just a class with no interfaces on itself
    auto externClassId = swgraph.createAlgorithm("EXTERN");

    // Subgraph behavior graph algorithms contain compositions of other behavior graph nodes
    // Again, this superclass has no interfaces on its own
    auto subgraphClassId = swgraph.createAlgorithm("SUBGRAPH");

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
