#ifndef PRINTMATRIX_HPP
#define PRINTMATRIX_HPP

#include <vector>
#include <iostream>
#include <iomanip>
#include "EuropeanOptionPrice.hpp"
#include "AmericanOptionPrice.hpp"

using namespace std;

void PrintOptionMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    const vector<double>& expiryTimes);

void DeltaMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    const vector<double>& expiryTimes);

void GammaMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    const vector<double>& expiryTimes);

void PerpetualMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    double b);

void PerpetualPutMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    double b);

#endif // PRINTMATRIX_HPP
