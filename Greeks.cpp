#include "Greeks.hpp"
#include <iomanip>
#include <cmath>

GreekCalculator::GreekCalculator(const OptionPrice& opt) : option(opt) {}

double GreekCalculator::DeltaFD(double S, double h) const {
    double price_plus = option.Price(S + h);
    double price_minus = option.Price(S - h);
    return (price_plus - price_minus) / (2 * h);
}

double GreekCalculator::GammaFD(double S, double h) const {
    double price_plus = option.Price(S + h);
    double price_minus = option.Price(S - h);
    double price = option.Price(S);
    return (price_plus - 2 * price + price_minus) / (h * h);
}

void GreekCalculator::CompareDelta(double S, const std::vector<double>& h_vals) const {
    cout << "Delta (Analytical): " << option.Delta(S) << "\n";
    for (double h : h_vals) {
        cout << "Delta (FD, h=" << h << "): " << DeltaFD(S, h) << "\n";
    }
}

void GreekCalculator::CompareGamma(double S, const vector<double>& h_vals) const {
    cout << "Gamma (Analytical): " << option.Gamma(S) << "\n";
    for (double h : h_vals) {
        cout << "Gamma (FD, h=" << h << "): " << GammaFD(S, h) << "\n";
    }
}


void GreekCalculator::CompareAllGreeks(double S, const vector<double>& h_vals, OptionPrice& option) {
    std::cout << "\n CALL OPTION \n";
    //option.OptType("C");  // or ensure it starts as call
    GreekCalculator callCalc(option);
    callCalc.CompareDelta(S, h_vals);
    callCalc.CompareGamma(S, h_vals);

    std::cout << "\n\n PUT OPTION \n";
    option.toggle();  // switch to put
    GreekCalculator putCalc(option);
    putCalc.CompareDelta(S, h_vals);
    putCalc.CompareGamma(S, h_vals);

    option.toggle();  // restore original type
}




void GreekCalculator::ComputeGreeks(const vector<double>& S_mesh, OptionParams p) {
    for (double S : S_mesh) {
        p.S = S;

        EuropeanOptionPrice option(p);
        double c = option.Price(S);
        double d = option.Delta(S);

        option.toggle();
        double put = option.Price(S);
        double put_d = option.Delta(S);

        double g = option.Gamma(S);

        std::cout << "S: " << S
            << " | Call: " << c << " | Delta (Call): " << d
            << " | Put: " << put << " | Delta (Put): " << put_d
            << "| Gamma: " << g << std::endl;
    }
}

void GreekCalculator::DeltaApprox(const vector<double>& S_mesh, OptionParams p) {
    for (double S : S_mesh) {
        p.S = S;
        double h = 0.01;

        EuropeanOptionPrice callOption(p);

        double V_plus_call = callOption.Price(S + h);
        double V_minus_call = callOption.Price(S - h);
        double V_center_call = callOption.Price(S);

        double delta_call = (V_plus_call - V_minus_call) / (2 * h);
        double gamma_call = (V_plus_call - 2 * V_center_call + V_minus_call) / (h * h);
        double analytic_delta_call = callOption.Delta(S);
        double c = V_center_call;

        callOption.toggle();  // now it's a PUT

        double V_plus_put = callOption.Price(S + h);
        double V_minus_put = callOption.Price(S - h);
        double V_center_put = callOption.Price(S);

        double delta_put = (V_plus_put - V_minus_put) / (2 * h);
        double gamma_put = (V_plus_put - 2 * V_center_put + V_minus_put) / (h * h);

        double analytic_delta_put = callOption.Delta(S);
        double put = V_center_put;

        cout << fixed << setprecision(4);
        cout << "S: " << S
            << " | Call: " << c << " | Delta (Call): " << delta_call
            << " | Put: " << put << " | Delta (Put): " << delta_put
            << "| Gamma: " << gamma_call << endl;
    }
}
