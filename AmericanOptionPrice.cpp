
#include "AmericanOptionPrice.hpp"
#include <cmath>
#include <iostream>
#include <vector>
//#include "Parameters.hpp"
using namespace std;

AmericanOptionPrice::AmericanOptionPrice() {

	init();

}

double AmericanOptionPrice::N(double x) const
{
	// Cumulative standard normal distribution using the error function
	return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

double AmericanOptionPrice::n(double x) const
{

	double A = 1.0 / sqrt(2.0 * 3.1415);
	return A * exp(-x * x * 0.5);

}

AmericanOptionPrice::AmericanOptionPrice(const PerpetualOptionParams& op) {
	K = op.K;
	T = op.T;
	r = op.r;
	sigma = op.sigma;
	b = op.b;
	optType = op.optType;
	S = op.S;
};


AmericanOptionPrice::~AmericanOptionPrice() {}


void AmericanOptionPrice::init()
{	// Initialise all default values
	r = 0.1;
	sigma = 0.1;
	T = 0.0;
	K = 100.0;
	b = 0.02;			// Black and Scholes stock option model (1973)
	optType = "C";		// European Call Option (this is the default type)
}

AmericanOptionPrice::AmericanOptionPrice(const string& optionType)
{	// Create option type
	init();
	optType = optionType;

	if (optType == "c")
		optType = "C";
}

AmericanOptionPrice::AmericanOptionPrice(const OptionParams& p) {
	K = p.K;
	T = p.T;
	r = p.r;
	sigma = p.sigma;
	b = p.b;
	optType = p.optType;
	S = p.S;
}


double AmericanOptionPrice::CallPrice(double U) const {
	double tmp = b / (sigma * sigma);
	double d = (tmp - 0.5) * (tmp - 0.5);

	double y1 = 0.5 - tmp + sqrt(d + ((2 * r) / (sigma * sigma)));

	double term1 = K / (y1 - 1);
	double term2 = std::pow(((y1 - 1) / y1) * (U / K), y1);

	return term1 * term2;
}

double AmericanOptionPrice::PutPrice(double U) const {

	double tmp = b / (sigma * sigma);
	double d = (tmp - 0.5) * (tmp - 0.5);
	double y2 = 0.5 - tmp - sqrt(d + ((2 * r) / (sigma * sigma)));

	double term1 = K / (1 - y2);
	double term2 = pow(((y2 - 1) / y2) * (U / K), y2);

	return term1 * term2;
}

double AmericanOptionPrice::Price(double U) const
{
	if (optType == "C")
	{
		//cout << "calling call\n";
		return CallPrice(U);
	}
	else
	{
		//cout << "calling put\n";
		return PutPrice(U);
	}
}


double AmericanOptionPrice::CallDelta(double U) const
{
	double tmp = sigma * sqrt(T);

	double d1 = (log(U / K) + (b + (sigma * sigma) * 0.5) * T) / tmp;


	return exp((b - r) * T) * N(d1);
}

double AmericanOptionPrice::PutDelta(double U) const
{
	double tmp = sigma * sqrt(T);

	double d1 = (log(U / K) + (b + (sigma * sigma) * 0.5) * T) / tmp;

	return exp((b - r) * T) * (N(d1) - 1.0);
}

double AmericanOptionPrice::PutCallGamma(double U) const
{

	double tmp = sigma * sqrt(T);

	double d1 = (log(U / K) + (b + (sigma * sigma) * 0.5) * T) / tmp;

	return (n(d1) * exp((b - r) * T)) / (U * tmp);
}

double AmericanOptionPrice::Delta(double U) const {
	if (optType == "C")
		return CallDelta(U);
	else
		return PutDelta(U);
}

double AmericanOptionPrice::Gamma(double U) const {
	return PutCallGamma(U);
}

