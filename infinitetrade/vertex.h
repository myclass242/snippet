#ifndef VERTEX_H_
#define VERTEX_H_

#include <list>
#include <memory>
#include <string>


class Vertex;
struct Vertex::AdjanceVertex;
using VertexPtr = std::shared_ptr<Vertex>;
using AdjVerIter = std::list<Vertex::AdjanceVertex>::iterator;

class Vertex : public std::enable_shared_from_this<Vertex>
{
public:
    struct AdjanceVertex
    {
        std::weak_ptr<Vertex> adjance;
        double weight;
    };

public:
    Vertex(const std::string& value);
    ~Vertex();

    const std::string& getName() const noexcept;
    void addAdjance(VertexPtr adjance, double weight);
    bool isAddjance(VertexPtr vertex) const;
    AdjVerIter adjanceVertexBegin();
    AdjVerIter adjanceVertexEnd();
    void setVisit(bool visited);
    bool isVisited(void) const noexcept;
private:
    std::string name;
    std::list<AdjanceVertex> adjanceVertexes;
    bool visit;
};

#endif
