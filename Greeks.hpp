#ifndef GREEKS_HPP
#define GREEKS_HPP

#include "Parameters.hpp"
#include "EuropeanOptionPrice.hpp"
#include <vector>
#include <iostream>

using namespace std;

class GreekCalculator {
private:
    const OptionPrice& option;

public:

    GreekCalculator(const OptionPrice& opt);

    double DeltaFD(double S, double h) const;
    double GammaFD(double S, double h) const;

    void CompareDelta(double S, const vector<double>& h_vals) const;
    void CompareGamma(double S, const vector<double>& h_vals) const;

    static void CompareAllGreeks(double S, const std::vector<double>& h_vals, OptionPrice& option);
    static void ComputeGreeks(const vector<double>& S_mesh, OptionParams p);
    static void DeltaApprox(const vector<double>& S_mesh, OptionParams p);
};

#endif // GREEKS_HPP
