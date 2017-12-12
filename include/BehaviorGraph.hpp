#ifndef _BEHAVIOUR_GRAPH_HPP
#define _BEHAVIOUR_GRAPH_HPP

#include "SoftwareGraph.hpp"

namespace Behavior {

class Graph : public Software::Graph
{
    public:
        Graph();
        Graph(const Hypergraph& base);
        ~Graph();

        std::string domainSpecificExport(const UniqueId& uid);
        bool domainSpecificImport(const std::string& serialized);

        Hyperedges getMergesOfInput(const Hyperedges& inputs, const std::string& label="");
    protected:
        void setupMetaModel();
};

}

#endif
