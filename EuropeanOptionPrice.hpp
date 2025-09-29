
#ifndef EuropeanOptionPrice_hpp
#define EuropeanOptionPrice_hpp

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "Parameters.hpp"
#include "OptionPrice.hpp"



class EuropeanOptionPrice : public OptionPrice
{
private:
    void init();
    void copy(const EuropeanOptionPrice& o2);

    double CallPrice(double U) const;
    double PutPrice(double U) const;

    double N(double x) const;
    double n(double x) const;

    double CallDelta(double U) const;
    double PutDelta(double U) const;

    double PutCallGamma(double U) const;



public:
    EuropeanOptionPrice();
    EuropeanOptionPrice(const string& optionType);
    EuropeanOptionPrice(const OptionParams& p);
    EuropeanOptionPrice(const EuropeanOptionPrice& option2);
    virtual ~EuropeanOptionPrice();

    EuropeanOptionPrice& operator = (const EuropeanOptionPrice& option2);

    

    double Price(double U) const override;

    double Delta(double U) const override;
    double Gamma(double U) const override;
};

#endif