#ifndef INFINITE_TRADE_H_
#define INFINITE_TRADE_H_

#include "graph.h"


class InfiniteTrade
{
public:
    InfiniteTrade() = default;
    ~InfiniteTrade() {};

    void input(const std::string& configFile);
    void addRule(const std::string& rule);
    void infiniteTrade(void);
private:
    void constructInfiniteTrades(void);
    void tradeAll(void);
private:
    Graph graph;
    // trades.size() == tradesDrection.size() == tradesWeight.size() == tradesBeginIndex.size()
    std::list<std::vector<VertexPtr>> trades;   // all infinite trade cycle
    std::vector<Drection> tradesDrection;   // the drectoin of each trade
	std::vector<std::vector<Vertex::Weight>> tradesWeight;   // the weight of each trade
    std::vector<std::size_t> tradesBeginIndex;  // the index of start trade of each trade in trades[x]
};

#endif