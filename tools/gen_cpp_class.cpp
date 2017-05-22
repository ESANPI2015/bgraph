#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <iostream>
#include <cassert>

int main(void)
{

    // TODO:
    // * Import software spec
    // * Search for all algorithms
    // * Create a class header for each algorithm

    // Test case:
    auto hypergraph = YAML::LoadFile("swgraph.yml").as<Hypergraph*>();
    auto swgraph = static_cast<Software::Graph*>(hypergraph);

    // Get all algorithms
    auto algorithms = swgraph->members(swgraph->algorithms());

    // For each of these algorithms
    for (auto algorithmId : algorithms)
    {
        auto algorithm = swgraph->get(algorithmId);
        std::cout << "Found algorithm " << algorithm->label() << "\n";
    }
    delete swgraph;
}
