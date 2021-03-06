#include "vertex.h"

Vertex::Vertex(const std::string& value)
    : name(value)
{}

Vertex::~Vertex()
{}

const std::string& Vertex::getName() const noexcept
{
    return name;
}

void Vertex::addAdjance(VertexPtr adjance, int weight, Drection drc)
{
    adjanceVertexes.push_back({adjance, weight, drc});
}

bool Vertex::isAddjance(VertexPtr vertex) const
{
    return std::find_if(adjanceVertexes.begin(), adjanceVertexes.end(), 
        [&](const AdjanceVertex& adjance)
        {
            return adjance.adjance.lock() == vertex;
        }) != adjanceVertexes.end();
}

std::pair<int, Drection> Vertex::getWeight(VertexPtr vertex) const
{
    auto iter = std::find_if(adjanceVertexes.begin(), adjanceVertexes.end(), 
        [&](const AdjanceVertex& adjance)
        {
            return adjance.adjance.lock() == vertex;
        });
    
    // not need to check iter != adjanceVertexes.end()
    // because the caller guaranteed the adjance property 
    return {iter->weight, iter->driction};
}

AdjVerIter Vertex::adjanceVertexBegin()
{
    return adjanceVertexes.begin();
}

AdjVerIter Vertex::adjanceVertexEnd()
{
    return adjanceVertexes.end();
}
