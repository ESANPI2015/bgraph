#include "SoftwareGraph.hpp"

#include <iostream>
#include <cassert>

int main(void)
{
    Software::Graph swgraph;
    std::cout << "*** Dump empty initialization\n";
    std::cout << Hyperedge::serialize(&swgraph) << std::endl;
    //assert(hwgraph.members().size() == 3);
    std::cout << "*** Create example\n";

    std::cout << "*** End of test ***" << std::endl;
    Hyperedge::cleanup();
}
