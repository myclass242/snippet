#include "infinitetrade.h"


static std::vector<std::string> split(const std::string& str, char a)
{
    std::vector<std::string> strvec;
    std::string::size_type pos1, pos2;
    pos1 = 0;
    pos2 = str.find(a);
    while (std::string::npos != pos2) {
        strvec.push_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = str.find(a, pos1);
    }
    strvec.push_back(str.substr(pos1));

    return strvec;
}

void InfiniteTrade::addRule(const std::string& rule)
{
    auto items = split(rule, ' ');
    if (items.size() != 3) {
        return;
    }
    int weight = std::stoi(items[2]);
    graph.addVertex(items[0]);
    graph.addVertex(items[1]);
    graph.addEdge(items[0], items[1], weight);
}

static void removeUninfiniteTradeCycles(std::list<std::vector<VertexPtr>>& cycles)
{
    // For a trade cycle in one direction, for example: A->B->...->X->A
    // the FORWARD weight product means increase, the OPPOSITE weight product
    // means decrease.
    // if the FORWARD weight product is equal to the OPPPOSITE weight product,
    // which mesns 1A eventually exchange 1A, it's not a infinite trade cycle.
    // if the OPPOSITE weight product is bigger than FORWARD weight product,
    // we should reverse the direction of trade, so the infinite trade order is
    // X->...->B->A->X
    for (auto cycleIter = cycles.begin(); cycleIter != cycles.end();) {
        uint64_t weightes[2] = {1, 1};
        auto end2beginWeight = (*(cycleIter->end() - 1))->getWeight(*(cycleIter->begin()));
        weightes[end2beginWeight.second] *= end2beginWeight.first;
        for (auto vertexIter = cycleIter->begin(); vertexIter != (cycleIter->end() - 1); ++vertexIter) {
            auto weight = (*vertexIter)->getWeight(*(vertexIter + 1));
            weightes[weight.second] *= weight.first;
        }
        if (weightes[FORWARD] == weightes[OPPOSITE]) {
            // not a infinite trade cycle
            cycleIter = cycles.erase(cycleIter);
        } else {
            if (weightes[FORWARD] < weightes[OPPOSITE]) {
                std::reverse(cycleIter->begin(), cycleIter->end());
            }
            ++cycleIter;
        }
    }
}

void InfiniteTrade::infiniteTrade(void)
{
    auto cycles = graph.getCycles();
    removeUninfiniteTradeCycles(cycles);
}

std::list<std::vector<VertexPtr>> InfiniteTrade::getAllTrades(void)
{
    auto cycles = graph.getCycles();
    removeUninfiniteTradeCycles(cycles);

    return cycles;
}
