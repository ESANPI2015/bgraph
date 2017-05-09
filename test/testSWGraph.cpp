#include "SoftwareGraph.hpp"

#include <iostream>
#include <cassert>

int main(void)
{
    Software::Graph swgraph;
    std::cout << "*** Dump empty initialization\n";
    std::cout << Hyperedge::serialize(&swgraph) << std::endl;
    std::cout << "*** Create example\n";
    
    auto alg = swgraph.createAlgorithm("DisparityMap");
    swgraph.has(alg, swgraph.createInput("left_image"));
    swgraph.has(alg, swgraph.createInput("right_image"));
    swgraph.has(alg, swgraph.createOutput("disparity")); // (x,y) -> z
    auto type = swgraph.createType("Image");
    
    // All interfaces of DisparityMap have the image type
    // How about some [] operator which can also get a parameter for string or id search
    swgraph.needs(swgraph.inputs()->members(), type);
    swgraph.provides(swgraph.outputs()->members(), type);

    std::cout << Hyperedge::serialize(&swgraph) << std::endl;

    std::cout << "*** End of test ***" << std::endl;
    Hyperedge::cleanup();
}
