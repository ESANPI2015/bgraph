#include "SoftwareGraph.hpp"
#include "HyperedgeYAML.hpp"

#include <iostream>
#include <cassert>

int main(void)
{
    Software::Graph* swgraph = Software::Graph::create();

    // TODO:
    // * Import software spec
    // * Search for all algorithms
    // * Create a class header for each algorithm

    // Test case:
    auto newEdges = YAML::load(YAML::LoadFile("test.yml"));
    // Get all algorithms
    auto algorithms = swgraph->algorithms()->members();

    // For each of these algorithms
    for (auto algorithmId : algorithms)
    {
        auto algorithm = Software::Algorithm::promote(Set::promote(Hyperedge::find(algorithmId)));
        std::cout << "Found algorithm " << algorithm->label() << "\n";
    }

    Hyperedge::cleanup();
}
