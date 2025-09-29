// OptionParams.hpp
#ifndef OPTIONPARAMS_HPP
#define OPTIONPARAMS_HPP

#include <string>
using namespace std;

struct OptionParams {
    double S;       // Spot price
    double K;       // Strike price
    double T;       // Time to maturity
    double r;       // Risk-free interest rate
    double sigma;   // Volatility
    double b;       // Cost of carry
    string optType = "C"; // Option type: "C" for Call, "P" for Put


};



struct PerpetualOptionParams {

    double S;       // Spot price
    double K;
    double T;// Risk-free interest rate
    double r;
    double sigma;   // Volatility
    double b;       // Cost of carry
    string optType = "C"; // Option type: "C" for Call, "P" for Put


    // parametric constructor
    PerpetualOptionParams(double S_, double K_, double T_, double sigma_, double r_, double b_ = 0.0, std::string optType_ = "C")
        : S(S_), K(K_), T(T_), sigma(sigma_), r(r_), b(b_), optType(optType_) {
    }




};

#endif // OPTIONPARAMS_HPP

