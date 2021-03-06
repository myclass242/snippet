#include "graph.h"

#include <iostream>
#include <unordered_set>
#include <stack>
#include <utility>
#include <algorithm>


void Graph::addVertex(const std::string& value)
{
    auto vertex = findVertex(value);
    if (vertex) {
        std::cout << value << " already exists" << std::endl;
        return;
    }

    vertexes.push_back(std::make_shared<Vertex>(value));
}

void Graph::addEdge(const std::string& from, const std::string& to, int weight)
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

    doAddEdge(vertexFrom, vertexTo, weight, Drection::FORWARD);
}

static void addCycle(std::list<std::list<VertexPtr>>& cycles, std::list<VertexPtr>&& candidateCycle)
{
    for (const auto& cycle : cycles) {
        if (cycle.size() == candidateCycle.size()) {
            bool find = std::all_of(cycle.begin(), cycle.end(), 
                [&](const VertexPtr& vertex) {
                    return std::find(candidateCycle.begin(), candidateCycle.end(), vertex) != candidateCycle.end();
                });
            if (find) {     // find a same cycle
                return;
            }
        }
    }

    cycles.push_back(std::move(candidateCycle));
}

std::list<std::list<VertexPtr>> Graph::getCycles(void)
{
    /* Use DFS to find all cycles.
     * tracePath: Vertexes in order of DFS. tracePath.first: vertex;
     * tracePath.second: iterator of tracePath.first.
     * tracePath records the stack infomation for the non_recursive DFS.
     * When traverse a vertex, add it to the back of tracePath.pop a vertex until
     * all its adjance vertexes have been traverse.If a vertex that has been 
     * visited is find during DFS, then we have find a cycle, and we don't 
     * traverse into that visited vertex, so that we guarantee that we do not
     * loop indefinitely.
    */
    std::list<std::list<VertexPtr>> cycles;
    std::vector<std::pair<VertexPtr, AdjVerIter>> tracePath;
    
    for (const auto& topVertex : vertexes) {
        tracePath.push_back(std::make_pair(topVertex, topVertex->adjanceVertexBegin()));
        while (!tracePath.empty()) {
            auto& currVertex = tracePath.back();
            auto& adjanceIter = currVertex.second;
            if (adjanceIter == currVertex.first->adjanceVertexEnd()) {
                tracePath.pop_back();
                continue;
            }
            while (adjanceIter != currVertex.first->adjanceVertexEnd()) {
                auto adjance = (*adjanceIter).adjance.lock();
                auto visitedVertex = std::find_if(tracePath.begin(), tracePath.end(), 
                    [&](const std::pair<VertexPtr, AdjVerIter>& Vertex)
                    {
                        // find a cycle 
                        return Vertex.first == adjance && !Vertex.first->isAddjance(adjance);
                    });
                if (visitedVertex != tracePath.end()) {
                    if (tracePath.end() - visitedVertex > 2) { // path A,B,A should not be considerd a cycle
                        std::list<VertexPtr> cycle;
                        while (visitedVertex != tracePath.end()) {
                            cycle.push_back(visitedVertex->first);
                            ++visitedVertex;
                        }
                        addCycle(cycles, std::move(cycle));
                    }
                    ++adjanceIter;
                    continue;
                }
                ++adjanceIter;
                tracePath.push_back(std::make_pair(adjance, adjance->adjanceVertexBegin()));
                break;
            }
        }
    }

    return cycles;
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

void Graph::doAddEdge(VertexPtr from, VertexPtr to, int weight, Drection drc)
{
    from->addAdjance(to, weight, drc);
    if (drc == Drection::FORWARD) {
        doAddEdge(to, from, weight, Drection::OPPOSITE);
    }
}