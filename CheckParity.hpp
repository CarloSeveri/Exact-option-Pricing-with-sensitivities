
// This header's purpose is to check if the put call parity holds.

#ifndef Option_parity_HPP
#define Option_parity_HPP

#include "Parameters.hpp"

void CheckPutCallParity(const OptionParams& p) {

    // We use polymorphism: a pointer to the base class to refer to EOP object
    OptionPrice* option = new EuropeanOptionPrice("C");
    option->K = p.K;
    option->T = p.T;
    option->r = p.r;
    option->sigma = p.sigma;
    option->b = p.b;

    double callPrice = option->Price(p.S);

    option->toggle();

    double putPrice = option->Price(p.S);

    // The two sides of the put call parity equation
    double C = callPrice + p.K * exp(-p.r * p.T);
    double P = putPrice + p.S;

    // One of the two approaches
    double callFromPut = putPrice + p.S - p.K * exp(-p.r * p.T);
    double putFromCall = callPrice + p.K * exp(-p.r * p.T) - p.S;

    cout << fixed << setprecision(4);
    cout << "  Call Price: " << callPrice << "\n";
    cout << "  Put  Price: " << putPrice << "\n";
    cout << "  Call (from Put):   " << callFromPut << "\n";
    cout << "  Put  (from Call):  " << putFromCall << "\n";

    // The secon approach: call and put prices must be 0.0001 accurate
    if (abs(C - P) < 1e-4)
        cout << "  Put-Call parity holds.\n";
    else
        cout << "  Put-Call parity does not hold.\n";
}


#endif