#ifndef INFINITE_TRADE_H_
#define INFINITE_TRADE_H_

#include "graph.h"


class InfiniteTrade
{
public:
    InfiniteTrade() = default;
    ~InfiniteTrade() {};

    void addRule(const std::string& rule);
    void infiniteTrade(void);
    // for test
    std::list<std::list<VertexPtr>> getTrades(void);
private:
    void removeUninfiniteTradeCycles(void);
    // std::size_t getTradeStardIndex()
private:
    Graph graph;
    std::list<std::list<VertexPtr>> trades;
    std::vector<std::vector<Vertex::Weight>> tradesWeight;
};

#endif