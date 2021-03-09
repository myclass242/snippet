#include "test.h"
#include "graph.h"
#include "infinitetrade.h"
#include <iostream>

static void printCycles(const std::list<std::vector<VertexPtr>>& cycles)
{
    for (const auto& cycle : cycles) {
        for (const auto& vertex : cycle) {
            std::cout << vertex->getName() << " ";
        }
        std::cout << std::endl;
    }
}

void testFindAllCycles1(void)
{
    Graph graph;

    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");

    graph.addEdge("A", "B", 2);
    graph.addEdge("B", "C", 2);
    graph.addEdge("A", "C", 3);

    printCycles(graph.getCycles());
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

    printCycles(graph.getCycles());
}

void testFindAllTrades(void)
{
    InfiniteTrade trade;
    trade.addRule("A B 2");
    trade.addRule("B C 2");
    trade.addRule("A C 3");

    trade.infiniteTrade();
}

void testFindAllTrades2(void)
{
    InfiniteTrade trade;
    trade.addRule("A B 2");
    trade.addRule("A C 4");
    trade.addRule("A D 6");
    trade.addRule("A E 4");
    trade.addRule("A F 2");
    trade.addRule("B C 2");
    trade.addRule("B E 2");
    trade.addRule("C D 2");

    trade.infiniteTrade();
}

void testFindAllTrads3(void)
{
    InfiniteTrade trade;
    trade.addRule("B A 2");
    trade.addRule("A C 4");
    trade.addRule("A D 6");
    trade.addRule("A E 4");
    trade.addRule("A F 2");
    trade.addRule("B C 2");
    trade.addRule("B E 2");
    trade.addRule("C D 2");
    trade.addRule("F D 3");
    trade.addRule("F E 7");
    trade.addRule("F C 2");
    trade.addRule("B F 5");

    trade.infiniteTrade();
}

void testFindAllTrads_configFile(void)
{
    InfiniteTrade trade;
    trade.input("./trade.txt");

    trade.infiniteTrade();
}