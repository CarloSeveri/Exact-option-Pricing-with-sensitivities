
#ifndef AmericanOptionPrice_HPP
#define AmericanOptionPrice_HPP

#include <iostream>
#include <string>
#include "Parameters.hpp"
#include "OptionPrice.hpp"
#include <vector>
#include <cmath>


class AmericanOptionPrice : public OptionPrice
{
private:
    double CallPrice(double U) const;
    double PutPrice(double U) const;
    void init();

    double N(double x) const;
    double n(double x) const;

    double CallDelta(double U) const;
    double PutDelta(double U) const;

    double PutCallGamma(double U) const;

public:
    AmericanOptionPrice();
    AmericanOptionPrice(const string& optionType);
    AmericanOptionPrice(const OptionParams& p);
    AmericanOptionPrice(const PerpetualOptionParams& op);
    virtual ~AmericanOptionPrice();

    double Price(double U) const override;

    double Delta(double U) const override;
    double Gamma(double U) const override;

};
#endif