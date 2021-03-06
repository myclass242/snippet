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
    void addEdge(const std::string& from, const std::string& to, int weight);
    std::list<std::list<VertexPtr>> getCycles(void);
private:
    VertexPtr findVertex(const std::string& value);
    void doAddEdge(VertexPtr from, VertexPtr to, int weight, Drection drc);
private:
    std::vector<VertexPtr> vertexes;
};

#endif