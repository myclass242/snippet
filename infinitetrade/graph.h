#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

#include "vertex.h"


class Graph
{
public:
    Graph() = default;
    ~Graph() {};

    void addVertex(const std::string& value);
    void addEdge(const std::string& from, const std::string& to, double weight);
    const std::vector<std::vector<VertexPtr>>& getCycles(void);
private:
    VertexPtr findVertex(const std::string& value);
    void doAddEdge(VertexPtr from, VertexPtr to, double weight, bool orient);
    void addCycle(std::vector<VertexPtr>&& candidateCycle);
private:
    std::vector<VertexPtr> vertexes;
    std::vector<std::vector<VertexPtr>> cycles;
};

#endif