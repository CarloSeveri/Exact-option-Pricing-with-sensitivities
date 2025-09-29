#include <iostream>
#include <iomanip>
#include <memory>
#include "Array.hpp"
#include "OptionMatrix.hpp"
#include "EuropeanOptionPrice.hpp"
#include "CheckParity.hpp"
#include "Greeks.hpp"
#include "AmericanOptionPrice.hpp"
#include <vector>
#include <memory>

using namespace std;

int main() {

    // This part creates a vector called batch, where each element is an OptionParams struct with six values
    vector<OptionParams> batch = {
        {102, 122, 1.65, 0.045, 0.43, 0.0},
        {100, 100, 1.0, 0.0, 0.2, 0.0},
        {5, 10, 1.0, 0.12, 0.5, 0.12},
        {100, 100, 30.0, 0.08, 0.3, 0.08}
    };
    // This for iterates through each set of parameters in the batch vector
    // For each p (which is an OptionParams object) it prints a line
    for (const auto& p : batch) {
        cout << "==============================\n";
        
        // Computes and verifies whether the put-call parity holds for that specific set of parameters.
        CheckPutCallParity(p);
    }

    // We generale a vector of monotonically increasing spot prices
    vector<double> S_mesh = GenerateMeshArray(80, 123, 1.0);
    OptionParams p = { 102, 122, 1.65, 0.045, 0.43, 0.0 };


    // We create a dynamically allocated object of type EuropeanOptionPrice using the p argument for its constructor
    unique_ptr<OptionPrice> option = make_unique<EuropeanOptionPrice>(p);
    vector<double> callPrices, putPrices;

    cout << '\n';

    // Range based loop
    // We output a vector of call and put prices for different spots (S_mesh)
    for (double S : S_mesh) {
        p.S = S;
        //option->toggle(); // call
        double c = option->Price(S);
        option->toggle(); // put
        double put = option->Price(S);
        option->toggle();

        callPrices.push_back(c);
        putPrices.push_back(put);

        cout << "S: " << S << " | Call: " << c << " | Put: " << put << endl;
    }

    // PrintOptionMatrix input vectors
    vector<double> expiryTimes = { 0.25, 0.5, 0.75, 1.0, 1.5 };
    vector<double> strikes = { 90.0, 95.0, 100.0, 105.0, 110.0, 120 , 130, 140 };
    vector<double> volatilities = { 0.1, 0.15, 0.20, 0.25, 0.3 , 0.35, 0.4, 0.5 };

    //PrintOptionMatrix default Parameters
    double r = 0.05, S = 100.0;

    //option Matrix of spot prices
    PrintOptionMatrix(S, r, strikes, volatilities, expiryTimes);

    // Greeks
    OptionParams dp = { 102, 122, 1.65 , 0.045 , 0.43, 0 };
    option = make_unique<EuropeanOptionPrice>(dp);
    option->optType = "C";

    cout << fixed << setprecision(4);
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "\nS = 105, K = 100, T = 0.5, sigma = 0.36, r = 0.1" << endl;

    // These are the values of h used tried out for the divided difference calculations
    vector<double> h_vals = { 0.1, 0.01, 0.001, 0.0001 };

    // Here we compare delta and gamma analytical and with the divided difference method
    // We pass a dereferenced smart pointer as a third argument. the option object points to EuropeanOptionPrice  
    GreekCalculator::CompareAllGreeks(102, h_vals, *option);

    // Call and put prices with sensitivities
    cout << "\n\nMonotonically increasing array of spot prices with sensitivities\n" << endl;
    GreekCalculator::ComputeGreeks(S_mesh, dp);

    // Call and put prices with sensitivities calculated using divided differences
    cout << "\n\nMonotonically increasing array of spot prices with sensitivities estimated using divided differences" << endl;
    GreekCalculator::DeltaApprox(S_mesh, dp);

    // Matrix with deltas for different strikes and volatilities
    cout << "\n\nDelta matrix" << endl;
    DeltaMatrix(S, r, strikes, volatilities, expiryTimes);
    
    // Matrix with gammas for different strikes and volatilities
    cout << "\n\nGamma matrix" << endl;
    GammaMatrix(S, r, strikes, volatilities, expiryTimes);

    // American perpetual options 
    // American perpetual option parameters (T = 0)
    PerpetualOptionParams p1{ 110, 100, 0.0, 0.1, 0.1, 0.02, "C" };

    // We create a dynamically allocated object p1 of type AmericanOptionPrice using the p1 argument for its constructor
    unique_ptr<OptionPrice> ao = make_unique<AmericanOptionPrice>(p1);

    /* Testing single perpetual option price
    cout << "Perpetual " << ao->optType << " option price: " << ao->Price(p1.S) << endl;
    ao->toggle();
    cout << "Perpetual " << ao->optType << " option price: " << ao->Price(p1.S) << endl;
    */

    cout << "\n" << endl;
    cout << "Perpetual American Options:" << endl;
    // This loop iterates through the mesh array of spots and calculated the price of perpetual calls and puts for those spots
    for (double S : S_mesh) {
        PerpetualOptionParams p2{ S, 100, 0.0, 0.1, 0.1, 0.02, "C" };

        // we create an ao (american option) object using the base class
        unique_ptr<OptionPrice> ao = make_unique<AmericanOptionPrice>(p2);

        ao->toggle(); // call
        double c = ao->Price(S);
        ao->toggle(); // put
        double put = ao->Price(S);

        callPrices.push_back(c);
        putPrices.push_back(put);

        cout << "S: " << S << " | Call: " << c << " | Put: " << put << endl;
    }

    // We print perpetual call prices as a function of K and sigma
    PerpetualMatrix(110, 0.1, strikes, volatilities, 0.02);

    // We print perpetual put prices as a function of K and sigma
    PerpetualPutMatrix(110, 0.1, strikes, volatilities, 0.02);

    return 0;
}
