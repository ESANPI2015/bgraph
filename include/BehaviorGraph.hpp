#ifndef _BEHAVIOUR_GRAPH_HPP
#define _BEHAVIOUR_GRAPH_HPP

#include "SoftwareGraph.hpp"

namespace Behavior {

class Graph : public Software::Graph
{
    public:
        static const UniqueId NodeId;
        static const UniqueId MergeId;
        static const UniqueId EdgeId;

        static const UniqueId InterfaceId;
        static const UniqueId InputId;
        static const UniqueId OutputId;

        static const UniqueId CId;
        static const UniqueId VHDLId;
        static const UniqueId CValueId;
        static const UniqueId VHDLValueId;

        static const UniqueId ExternId;
        static const UniqueId SubgraphId;

        //static const UniqueId SumId;
        //static const UniqueId ProductId;
        //static const UniqueId MinId;
        //static const UniqueId MaxId;
        //static const UniqueId MeanId;
        //static const UniqueId NormId;

        //static const UniqueId Arity1Id;
        //static const UniqueId PipeId;
        //static const UniqueId InputId;
        //static const UniqueId OutputId;
        //static const UniqueId DivideId;
        //static const UniqueId SineId;
        //static const UniqueId CosineId;
        //static const UniqueId TangensId;
        //static const UniqueId TangensHyperbolicusId;
        //static const UniqueId ArcusCosineId;
        //static const UniqueId ArcusSineId;
        //static const UniqueId ArcusTangensId;
        //static const UniqueId LogarithmId;
        //static const UniqueId ExponentialId;
        //static const UniqueId AbsoluteId;
        //static const UniqueId SquareRootId;

        //static const UniqueId Arity2Id;
        //static const UniqueId ArcusTangens2Id;
        //static const UniqueId PowerId;
        //static const UniqueId ModuloId;

        //static const UniqueId Arity3Id;
        //static const UniqueId GreaterZeroId;
        //static const UniqueId ApproxZeroId;

        Graph();
        Graph(const Hypergraph& base);
        ~Graph();

        std::string domainSpecificExport(const UniqueId& uid);
        bool domainSpecificImport(const std::string& serialized);

        Hyperedges getMergesOfInput(const Hyperedges& inputs, const std::string& label="");

        std::string floatToStdLogicVector(const float value);

    protected:
        void setupMetaModel();
};

}

#endif
