#include "graph.h"

#include <iostream>
#include <unordered_set>
#include <stack>
#include <utility>


void Graph::addVertex(const std::string& value)
{
    auto vertex = findVertex(value);
    if (vertex) {
        std::cout << value << " already exists" << std::endl;
        return;
    }

    vertexes.push_back(std::make_shared<Vertex>(value));
}

void Graph::addEdge(const std::string& from, const std::string& to, double weight)
{
    auto vertexFrom = findVertex(from);
    if (!vertexFrom) {
        std::cout << from << " not exists" << std::endl;
        return;
    }
    auto vertexTo = findVertex(to);
    if (!vertexTo) {
        std::cout << to << " not exists" << std::endl;
        return;
    }

    doAddEdge(vertexFrom, vertexTo, weight, false);
}

std::vector<std::vector<VertexPtr>> Graph::getCycle(void)
{
    /* Use DFS to find all cycles.
     * If a vertex that has been visited is find during DFS, then we have 
     * find a cycle.
     * tracePath: Vertexes in order of DFS
    */

    std::vector<std::vector<VertexPtr>> cycles;
    std::vector<std::pair<VertexPtr, AdjVerIter>> tracePath;
    
    for (const auto& topVertex : vertexes) {
        tracePath.push_back(std::make_pair(topVertex, topVertex->adjanceVertexBegin()));
        while (!tracePath.empty()) {
            auto& currVertex = tracePath.back();
            for (auto& adjanceIter = currVertex.second; adjanceIter != currVertex.first->adjanceVertexEnd(); ++adjanceIter) {
                if (tracePath.size() > 1) {
                    auto visitedVertex = std::find_if(tracePath.begin(), tracePath.end(), 
                        [&](const std::pair<VertexPtr, AdjVerIter>& Vertex)
                        {

                        });
                }
            }
        }
    }
}

VertexPtr Graph::findVertex(const std::string& value)
{
    auto findVertex = std::find_if(vertexes.begin(), vertexes.end(), 
        [&](const VertexPtr& vertex) { return vertex->getName() == value; });
    
    if (findVertex == vertexes.end()) {
        return nullptr;
    }

    return *findVertex;
}

void Graph::doAddEdge(VertexPtr from, VertexPtr to, double weight, bool orient)
{
    from->addAdjance(to, weight);
    if (!orient) {
        doAddEdge(to, from, 1 / weight, !orient);
    }
}