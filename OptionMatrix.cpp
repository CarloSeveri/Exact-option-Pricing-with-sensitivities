
#include "OptionMatrix.hpp"

void PrintOptionMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    const vector<double>& expiryTimes)
{
    cout << fixed << setprecision(2);

    for (double T : expiryTimes) {
        cout << "\nExpiry Time: " << T << "\n";
        cout << setw(10) << "K\\Vol";

        for (double vol : volatilities)
            cout << setw(10) << vol;
        cout << endl;

        for (double K : strikes) {
            cout << setw(10) << K;

            for (double vol : volatilities) {
                OptionParams p = { S, K, T, r, vol, r, "C" };

                EuropeanOptionPrice option(p);

                // Now call Price() with the spot price S
                double price = option.Price(S);  // S as an argument here
                cout << setw(10) << price;
            }

            cout << endl;
        }
    }
}

void DeltaMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    const vector<double>& expiryTimes)
{
    cout << fixed << setprecision(4);

    for (double T : expiryTimes) {
        cout << "\nExpiry Time: " << T << "\n";
        cout << setw(10) << "K\\Vol";

        for (double vol : volatilities)
            cout << setw(10) << vol;
        cout << endl;

        for (double K : strikes) {
            cout << setw(10) << K;

            for (double vol : volatilities) {
                OptionParams p = { S, K, T, r, vol, r, "C" };

                EuropeanOptionPrice option(p);

                // Now call Price() with the spot price S
                double price = option.Delta(S);  // Pass S as an argument here
                cout << setw(10) << price;
            }

            cout << endl;
        }
    }
}

void GammaMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    const vector<double>& expiryTimes)
{
    cout << fixed << setprecision(4);

    for (double T : expiryTimes) {
        cout << "\nExpiry Time: " << T << "\n";
        cout << setw(10) << "K\\Vol";

        for (double vol : volatilities)
            cout << setw(10) << vol;
        cout << endl;

        for (double K : strikes) {
            cout << setw(10) << K;

            for (double vol : volatilities) {
                OptionParams p = { S, K, T, r, vol, r, "C" };

                EuropeanOptionPrice option(p);

                // Now call Price() with the spot price S
                double price = option.Gamma(S);  // Pass S as an argument here
                cout << setw(10) << price;
            }

            cout << endl;
        }
    }
}

void PerpetualMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    double b) // added as a separate parameter
{
    cout << fixed << setprecision(4);

    cout << "\nPerpetual Call Option Price Matrix:\n";
    cout << setw(10) << "K\\Vol";

    for (double vol : volatilities)
        cout << setw(10) << vol;
    cout << endl;

    for (double K : strikes) {
        cout << setw(10) << K;

        for (double vol : volatilities) {
            PerpetualOptionParams p1{ S, K, 0.0, vol, r, b, "C" };
            PerpetualOptionParams op(p1.S, p1.K, 0.0, p1.sigma, p1.r, p1.b, p1.optType);
            double tmp = b / (vol * vol);
            double d = (tmp - 0.5) * (tmp - 0.5);

            double y1 = 0.5 - tmp + sqrt(d + ((2 * r) / (vol * vol)));

            double term1 = K / (y1 - 1);
            double term2 = pow(((y1 - 1) / y1) * (S / K), y1);
            double beta = term1 * term2;

            AmericanOptionPrice ao(op);

            double priceCall = ao.Price(S);
            cout << setw(10) << priceCall;
        }

        cout << endl;
    }
}

void PerpetualPutMatrix(double S, double r,
    const vector<double>& strikes,
    const vector<double>& volatilities,
    double b) // added as a separate parameter
{
    cout << fixed << setprecision(4);

    cout << "\nPerpetual Put Option Price Matrix:\n";
    cout << setw(10) << "K\\Vol";

    for (double vol : volatilities)
        cout << setw(10) << vol;
    cout << endl;

    for (double K : strikes) {
        cout << setw(10) << K;

        for (double vol : volatilities) {
            PerpetualOptionParams p1{ S, K, 0.0, vol, r, b, "P" };
            PerpetualOptionParams op(p1.S, p1.K, 0.0, p1.sigma, p1.r, p1.b, p1.optType);
            double tmp = b / (vol * vol);
            double d = (tmp - 0.5) * (tmp - 0.5);

            double y1 = 0.5 - tmp + sqrt(d + ((2 * r) / (vol * vol)));

            double term1 = K / (y1 - 1);
            double term2 = pow(((y1 - 1) / y1) * (S / K), y1);
            double beta = term1 * term2;

            AmericanOptionPrice ao(op);

            double pricePut = ao.Price(S);
            cout << setw(10) << pricePut;
        }

        cout << endl;
    }
}
