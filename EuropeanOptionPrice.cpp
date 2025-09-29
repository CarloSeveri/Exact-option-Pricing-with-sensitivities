
#include "EuropeanOptionPrice.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include "Parameters.hpp"




EuropeanOptionPrice::EuropeanOptionPrice() {
	init();
};

double EuropeanOptionPrice::N(double x) const
{
	// Cumulative standard normal distribution using the error function
	return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

double EuropeanOptionPrice::n(double x) const
{

	double A = 1.0 / sqrt(2.0 * 3.1415);
	return A * exp(-x * x * 0.5);

}

// Kernel Functions (Haug)
double EuropeanOptionPrice::CallPrice(double U) const
{
	double tmp = sigma * sqrt(T);

	double d1 = (log(U / K) + (b + 0.5 * pow(sigma, 2)) * T) / tmp;
	double d2 = d1 - tmp;

	return (U * exp((b - r) * T) * N(d1)) - (K * exp(-r * T) * N(d2));
}

double EuropeanOptionPrice::PutPrice(double U) const
{

	double tmp = sigma * sqrt(T);
	double d1 = (log(U / K) + (b + (sigma * sigma) * 0.5) * T) / tmp;
	double d2 = d1 - tmp;

	return (K * exp(-r * T) * N(-d2)) - (U * exp((b - r) * T) * N(-d1));

}


double EuropeanOptionPrice::CallDelta(double U) const
{
	double tmp = sigma * sqrt(T);

	double d1 = (log(U / K) + (b + (sigma * sigma) * 0.5) * T) / tmp;


	return exp((b - r) * T) * N(d1);
}

double EuropeanOptionPrice::PutDelta(double U) const
{
	double tmp = sigma * sqrt(T);

	double d1 = (log(U / K) + (b + (sigma * sigma) * 0.5) * T) / tmp;

	return exp((b - r) * T) * (N(d1) - 1.0);
}

double EuropeanOptionPrice::PutCallGamma(double U) const
{

	double tmp = sigma * sqrt(T);

	double d1 = (log(U / K) + (b + (sigma * sigma) * 0.5) * T) / tmp;

	return (n(d1) * exp((b - r) * T)) / (U * tmp);
}



void EuropeanOptionPrice::init()
{	// Initialise all default values
	r = 0.05;
	sigma = 0.2;
	K = 25.0;
	T = 1;
	b = r;			// Black and Scholes stock option model (1973)
	optType = "C";		// European Call Option (this is the default type)
}

void EuropeanOptionPrice::copy(const EuropeanOptionPrice& o2)
{
	r = o2.r;
	sigma = o2.sigma;
	K = o2.K;
	T = o2.T;
	b = o2.b;
	optType = o2.optType;
}

// Copy constructor
EuropeanOptionPrice::EuropeanOptionPrice(const EuropeanOptionPrice& o2) {
	copy(o2); // Reuse the copy method
}

//EuropeanOptionPrice::EuropeanOptionPrice()
//{ // Default call option
//	init();
//{

EuropeanOptionPrice::~EuropeanOptionPrice() {}

EuropeanOptionPrice& EuropeanOptionPrice::operator = (const EuropeanOptionPrice& option2)
{
	if (this == &option2) return *this;
	copy(option2);
	return *this;
}

double EuropeanOptionPrice::Price(double U) const
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



EuropeanOptionPrice::EuropeanOptionPrice(const string& optionType)
{	// Create option type
	init();
	optType = optionType;

	if (optType == "c")
		optType = "C";
}

EuropeanOptionPrice::EuropeanOptionPrice(const OptionParams& p) {
	K = p.K;
	T = p.T;
	r = p.r;
	sigma = p.sigma;
	b = p.b;
	optType = p.optType;
	S = p.S;
}

double EuropeanOptionPrice::Delta(double U) const {
    if (optType == "C")
        return CallDelta(U);
    else
        return PutDelta(U);
}

double EuropeanOptionPrice::Gamma(double U) const {
    return PutCallGamma(U);
}

