#ifndef VERTEX_H_
#define VERTEX_H_

#include <list>
#include <memory>
#include <string>
#include <utility>


class Vertex;
using VertexPtr = std::shared_ptr<Vertex>;


/* A B 2
 * 1A exchange 2B is FORWARD
 * 2B exchange 1A is OPPOSITE
*/
enum Drection
{
    FORWARD = 0,
    OPPOSITE
};

class Vertex
{
public:
    struct Weight
    {
        Weight() = delete;
        Weight(int w, Drection drc);
        Weight(const Weight& other) = default;
        Weight& operator=(const Weight& rhs);

        int weight;
        Drection drection;
    };
    struct AdjanceVertex
    {
        AdjanceVertex() = delete;
        AdjanceVertex(VertexPtr vertex, int w, Drection drc);
        AdjanceVertex(const AdjanceVertex& other) = default;
        AdjanceVertex& operator=(const AdjanceVertex& rhs);

        std::weak_ptr<Vertex> adjance;
        Weight weight;
    };

public:
    Vertex(const std::string& value);
    ~Vertex();

    const std::string& getName() const noexcept;
    void addAdjance(VertexPtr adjance, int weight, Drection drc);
    bool isAddjance(VertexPtr vertex) const;
    Weight getWeight(VertexPtr vertex) const;
    std::list<AdjanceVertex>::iterator adjanceVertexBegin();
    std::list<AdjanceVertex>::iterator adjanceVertexEnd();
private:
    std::string name;
    std::list<AdjanceVertex> adjanceVertexes;
};

using AdjVerIter = std::list<Vertex::AdjanceVertex>::iterator;

#endif
