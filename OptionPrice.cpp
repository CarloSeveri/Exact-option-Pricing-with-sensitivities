

#include "OptionPrice.hpp"

OptionPrice::OptionPrice(const OptionParams& p)
    : S(p.S), K(p.K), T(p.T), r(p.r), sigma(p.sigma), b(p.b) {
}

OptionPrice::~OptionPrice() {}

double OptionPrice::Price(double U) const {
    if (optType == "C")
        return CallPrice(U);
    else
        return PutPrice(U);
}

void OptionPrice::toggle() {
    optType = (optType == "C") ? "P" : "C";
}
