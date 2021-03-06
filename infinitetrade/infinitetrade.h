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
    std::list<std::vector<VertexPtr>> getAllTrades(void);
private:
    Graph graph;
};

#endif