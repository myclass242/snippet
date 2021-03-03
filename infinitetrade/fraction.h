#ifndef FRACTOIN_H_
#define FRACTION_H_

#include <cstdint>

class Fraction
{
public:
    Fraction() = delete;
    Fraction(const Fraction&) = default;
private:
    uint32_t numerator;
    uint32_t denominator;
};

#endif