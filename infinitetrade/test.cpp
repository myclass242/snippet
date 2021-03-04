#include "test.h"
#include "graph.h"
#include <iostream>

void testFindAllCycles1(void)
{
    Graph graph;

    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");

    graph.addEdge("A", "B", 2);
    graph.addEdge("B", "C", 2);
    graph.addEdge("A", "C", 3);

    auto cycles = graph.getCycles();
    for (const auto& cycle : cycles) {
        for (const auto& vertex : cycle) {
            std::cout << vertex->getName() << " ";
        }
        std::cout << std::endl;
    }
}

void testFindAllCycles2(void)
{
    Graph graph;

    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");
    graph.addVertex("D");
    graph.addVertex("E");
    graph.addVertex("F");

    graph.addEdge("A", "B", 2);
    graph.addEdge("A", "C", 3);
    graph.addEdge("A", "D", 2);
    graph.addEdge("A", "E", 2);
    graph.addEdge("A", "F", 2);
    graph.addEdge("B", "C", 2);
    graph.addEdge("B", "E", 2);
    graph.addEdge("C", "D", 2);

    auto cycles = graph.getCycles();
    for (const auto& cycle : cycles) {
        for (const auto& vertex : cycle) {
            std::cout << vertex->getName() << " ";
        }
        std::cout << std::endl;
    }
}