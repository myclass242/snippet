#include "vertex.h"

Vertex::Vertex(const std::string& value)
    : name(value), visit(false)
{}

Vertex::~Vertex()
{}

const std::string& Vertex::getName() const noexcept
{
    return name;
}

void Vertex::addAdjance(VertexPtr adjance, double weight)
{
    adjanceVertexes.push_back({adjance, weight});
}

bool Vertex::isAddjance(VertexPtr vertex) const
{
    return std::find_if(adjanceVertexes.begin(), adjanceVertexes.end(), 
        [&](const AdjanceVertex& adjance)
        {
            return adjance.adjance.lock() == vertex;
        }) != adjanceVertexes.end();
}

AdjVerIter Vertex::adjanceVertexBegin()
{
    return adjanceVertexes.begin();
}

AdjVerIter Vertex::adjanceVertexEnd()
{
    return adjanceVertexes.end();
}

void Vertex::setVisit(bool visited)
{
    visit = visited;
}

bool Vertex::isVisited(void) const noexcept
{
    return visit;
}