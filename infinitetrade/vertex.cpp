#include "vertex.h"


Vertex::Weight::Weight(int w, Drection drc)
    : weight(w), drection(drc)
{}

Vertex::Weight& Vertex::Weight::operator=(const Weight& rhs)
{
    if (this != &rhs) {
        weight = rhs.weight;
        drection = rhs.drection;
    }

    return *this;
}

Vertex::AdjanceVertex::AdjanceVertex(VertexPtr vertex, int w, Drection drc)
    : adjance(vertex), weight(w, drc)
{}

Vertex::AdjanceVertex& Vertex::AdjanceVertex::operator=(const AdjanceVertex& rhs)
{
    if (this != &rhs) {
        adjance = rhs.adjance;
        weight = rhs.weight;
    }

    return *this;
}

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

Vertex::Weight Vertex::getWeight(VertexPtr vertex) const
{
    auto iter = std::find_if(adjanceVertexes.begin(), adjanceVertexes.end(), 
        [&](const AdjanceVertex& adjance)
        {
            return adjance.adjance.lock() == vertex;
        });
    
    // not need to check iter != adjanceVertexes.end()
    // because the caller guaranteed the adjance property 
    return iter->weight;
}

AdjVerIter Vertex::adjanceVertexBegin()
{
    return adjanceVertexes.begin();
}

AdjVerIter Vertex::adjanceVertexEnd()
{
    return adjanceVertexes.end();
}
