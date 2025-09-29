#ifndef OptionPrice_hpp
#define OptionPrice_hpp

#include <string>
using namespace std;

class OptionPrice {

public:
    double S;       // Spot price
    double K;       // Strike price
    double T;       // Time to maturity
    double r;       // Risk-free interest rate
    double sigma;   // Volatility
    double b;       // Cost of carry
    string optType = "C"; // Option type: "C" or "P"

    OptionPrice() {};
    OptionPrice(const string& optionType) : optType(optionType) {}
    virtual ~OptionPrice() {}

    virtual double Price(double U) const = 0;
    virtual void toggle() {
        optType = (optType == "C") ? "P" : "C";
    }

    virtual double Delta(double S) const = 0;
    virtual double Gamma(double S) const = 0;

    



};

#endif
