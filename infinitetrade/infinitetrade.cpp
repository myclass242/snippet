#include "infinitetrade.h"

#include <unordered_map>


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

std::size_t getTradeBeginIndex(const std::vector<Vertex::Weight>& weights, Drection drection, uint64_t base)
{
    uint64_t min = base;

    if (drection == FORWARD) {
        std::size_t beginIndex = 0;
        uint64_t currValue = min;
        for (int index = 0; index != weights.size(); ++index) {
            if (weights[index].drection == FORWARD) {
                currValue *= weights[index].weight;
            } else {
                currValue /= weights[index].weight;
            }
            if (min < currValue) {
                min = currValue;
                beginIndex = index + 1;
            }
        }
        return beginIndex;
    } else {
        std::size_t beginIndex = weights.size() - 1;
        uint64_t currValue = min;
        for (int index = weights.size() - 1; index >= 0; --index) {
            if (weights[index].drection == FORWARD) {
                currValue /= weights[index].weight;
            } else {
                currValue *= weights[index].weight;
            }
            if (min < currValue) {
                min = currValue;
                beginIndex = index;
            }
        }
        return beginIndex;
    }
}

void InfiniteTrade::removeUninfiniteTradeCycles(void)
{
    // For a trade cycle in one direction, for example: A->B->...->X->A,
    // the FORWARD weight product means increase, the OPPOSITE weight product
    // means decrease.
    // if the FORWARD weight product is equal to the OPPPOSITE weight product,
    // which mesns 1A eventually exchange 1A, it's not a infinite trade cycle.
    // if the OPPOSITE weight product is bigger than FORWARD weight product,
    // we should reverse the direction of trade, so the infinite trade order is
    // X->...->B->A->X.
    // In vector, A is the begin element and X is the end element
    for (auto cycleIter = trades.begin(); cycleIter != trades.end();) {
        std::vector<Vertex::Weight> weights;
        uint64_t weightProduct[2] = {1, 1}; // weightProduct[0]: FORWARD weight product, weightProduct[1]: OPPOSITE weight product
        auto end2beginWeight = (*(cycleIter->end() - 1))->getWeight(*(cycleIter->begin())); // X->A
        weightProduct[end2beginWeight.drection] *= end2beginWeight.weight;
        for (auto vertexIter = cycleIter->begin(); vertexIter != (cycleIter->end() - 1); ++vertexIter) {
            auto weight = (*vertexIter)->getWeight(*(vertexIter + 1));
            weightProduct[weight.drection] *= weight.weight;
            weights.push_back(weight);
        }
        weights.push_back(end2beginWeight);
        if (weightProduct[FORWARD] == weightProduct[OPPOSITE]) {
            // not a infinite trade cycle
            cycleIter = trades.erase(cycleIter);
        } else {
            if (weightProduct[FORWARD] < weightProduct[OPPOSITE]) {
                tradesDrection.push_back(OPPOSITE);
                tradesBeginIndex.push_back(getTradeBeginIndex(weights, OPPOSITE, weightProduct[FORWARD]));
            } else {
                tradesDrection.push_back(FORWARD);
                tradesBeginIndex.push_back(getTradeBeginIndex(weights, FORWARD, weightProduct[OPPOSITE]));
            }
            tradesWeight.push_back(std::move(weights));
            ++cycleIter;
        }
    }
}

void InfiniteTrade::tradeAll(void)
{
    auto tradeIter = trades.begin();
    auto drectionIter = tradesDrection.begin();
    auto weightsIter = tradesWeight.begin();

    std::unordered_map<std::string, int> productsInHand; // trade products in hand 
    while (tradeIter != trades.end()) {
        if (*drectionIter == FORWARD) {
            
        } else {

        }
    }
}

void InfiniteTrade::infiniteTrade(void)
{
    trades = graph.getCycles();
    removeUninfiniteTradeCycles();
    tradeAll();
}

std::list<std::vector<VertexPtr>> InfiniteTrade::getTrades(void)
{
    trades = graph.getCycles();
    removeUninfiniteTradeCycles();

    return trades;
}
