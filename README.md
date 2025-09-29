[Group A&B.md](https://github.com/user-attachments/files/22605856/Group.A.B.md)
### Group A

#### Design 

`OptionPrice.hpp` serves as base class. `EuropeanOptionPrice.hpp` and `AmericanOptionPrice.hpp` are the derived classes. The other important functions have been isolated in dedicated header files for enhanced readability. I also created a `Parameters.hpp` that contains two structs to group together the parameters used in the pricing of European (`OptionParams`) and American (`PerpetualOptionParams`) options.

##### OptionPrice.hpp
This base class is designed to represent a common interface for the pricing of different options, in our case European and American. We use **polymorphism** so that all option types can be used interchangeably. For convenience (as found in the code given) we place the common attributes of any option in the public interface of the base class. Then we have constructors: the default one, the `optionType` one (allows to set "C" or "P" at construction) and a virtual destructor. The destructor must be virtual as it allows to call the destructor of the derived class before the destructor of the base class. Then, using a virtual destructor ensures proper cleanup of resources when a base class pointer (`unique_ptr<OptionPrice>`) is used to delete a derived class object.
Given the fact that all derived classes must have certain characteristics: a price, delta and gamma function those are declared as virtual and equal to 0 making the class abstract. These methods must show in all derived classes to avoid compiler errors. `toggle()` is not equal to 0 as it works as it is for every derived class, and in that way we avoid redefining and redeclaring the function in every derived class header and source file. 

##### EuropeanOptionPrice.hpp and AmericanOptionPrice.hpp
Both `EuropeanOptionPrice` and `AmericanOptionPrice` are **derived classes** of a common abstract base class `OptionPrice`.
Regarding the design of these there is not much to say because they are for the most part the same as the provided ones. For encapsulation purposes all the core components for the calculation of the price , delta and gamma are private members. The public interface features several constructors the destructor and the core methods. The core methods override the base class' pure virtual functions.

##### Greeks.hpp
This is a **utility class** to analyze the sensitivity (Greeks) of option prices. The private part of the class stores a reference to an `OptionPrice` object enabling polymorphism as the object can be of any derived type. The public interface features a constructor that takes a argument a reference to an `OptionPrice` object. The `Delta` and the `Gamma` methods are taken from the virtual functions in the base class. The reason why, in my opinion, it wouldn't be a good idea to move all Delta and Gamma formulas from the derived classes to a dedicated one because in that way we would make it impossible to access the delta and gamma functions through the class hierarchy. `DeltaFD` `GammaFD` are the methods that estimate the Delta using divided differences. `CompareGamma` and `CompareDelta` use different values of h to estimate the value of Delta and Gamma calculated using the divided difference methods. The last three methods are static, that means it is possible to use these functions without first creating a `GreekCalculator` object. It makes sense here because these functions just take inputs like option data or prices and do the calculations—they don’t need to remember anything about a specific object.

##### CheckParity.hpp
This header file defines a function that verifies the fundamental financial relationship known as put-call parity for European options. . The function takes in a structure of option parameters and uses the `EuropeanOptionPrice` class to compute both the call and the put prices by toggling the option type. It then calculates theoretical equivalents for the call from the put and vice versa using the standard put-call parity formula. These computed prices are compared, and if the difference between the two sides of the parity equation is within a small numerical tolerance, the program confirms that the parity holds; otherwise, it reports a discrepancy.


##### Array.hpp 
This function **generates a vector of evenly spaced values** between two endpoints, `start` and `end`, with a step size of `step`. Each `val` in the loop gets **added to the vector** via `push_back`. We use this function when we need to create a monotonically increasing array of spot prices.

##### OptionMatrix.hpp
The purpose of this header file is to create and **print out matrices** showing option prices of different strikes (Spot price held constant) as a function of time and volatility  and showing the variation of sensitivities to those parameters.

Each function follows a similar pattern: `PrintOptionmatrix` prints **European call option prices** looping over `T` (expiry times), then over `K` and `σ` and using `EuropeanOptionPrice::Price(S)` to compute prices. 

`DeltaMatrix` and `GammaMatrix` work exactly in the same way but they compute delta and gamma instead of option prices.

`PerpetualMatrix`(for calls) and `PerpetualPutMatrix`(for puts) handle american perpetual options (no `T`), hence they loop over K and $\sigma$. We use the `AmericanOptionPrice::Price(S)` method to output price.

##### main.cpp
Now that we have talked about all the components it is time to talk about the main function. The purpose of this is to demonstrate and display:

- Put-Call Parity checks
- Option price evolution over spot prices
- Option pricing matrices (varying strikes, volatilities, maturities)
- Greeks and finite difference approximations
- Perpetual American option prices

This code follows object oriented design using the abstract base class (`OptionPrice`) and derived implementations.
We also use a smart pointer provided by the STL library **unique_ptr** to easily manage dynamic memory. The two structs in `Parameters.hpp` group together the parameters to keep the main as clean and efficient as possible. 

The programs begins by checking the put-call parity across the option parameter sets given, ensuring theoretical consistency. Then, using a mesh of spot prices, it prices both European call and put options, toggling between the two via a polymorphic interface implemented with smart pointers. The results are printed to show how option prices evolve as the underlying asset changes. Next, the program computes and prints an option price matrix, varying strikes, volatilities, and maturities to give a full view of pricing sensitivities. The `GreekCalculator` module is used to compare analytical and finite difference approximations of the Greeks (Delta, Gamma) and to generate their values across the spot price mesh. This is followed by matrix outputs for Delta and Gamma across various input parameters. Finally, the program shifts focus to perpetual American options, pricing them over the same spot mesh and outputting matrices to observe behavior over a range of strikes and volatilities.


##### 1 

  Call Price: 2.1334
  Put  Price: 5.8463
  Call (from Put):   2.1334
  Put  (from Call):  5.8463
  Put-Call parity holds.

  Call Price: 7.9656
  Put  Price: 7.9656
  Call (from Put):   7.9656
  Put  (from Call):  7.9656
  Put-Call parity holds.

  Call Price: 0.2041
  Put  Price: 4.0733
  Call (from Put):   0.2041
  Put  (from Call):  4.0733
  Put-Call parity holds.

  Call Price: 92.1757
  Put  Price: 1.2475
  Call (from Put):   92.1757
  Put  (from Call):  1.2475
  Put-Call parity holds.

These are the result that we get for the first block. They are consistent with the financial literature on the matter. For the first and third batch the put options are in the money , that makes them more valuable than the calls, given also the very short TTM. The second batch exhibits an at the money option with more time value (longer TTM) hence higher price. Interestingly there is a significant mismatch between call and put prices in the fourth batch and that is due a few things: the call has generally more upside potential than the put and hence on the side of the writer it is riskier. Additionally, the option is currently at the money and in order for the put to become in the money the price should drop significantly which is less likely over long time horizons (30 years). The put call parity is respected within $10^{-4}$ . 

4th Batch
S: 95.0000 | Call: 87.2342 | Put: 1.3060
S: 96.0000 | Call: 88.2221 | Put: 1.2939
S: 97.0000 | Call: 89.2103 | Put: 1.2820
S: 98.0000 | Call: 90.1985 | Put: 1.2703
S: 99.0000 | Call: 91.1870 | Put: 1.2588
S: 100.0000 | Call: 92.1757 | Put: 1.2475
S: 101.0000 | Call: 93.1646 | Put: 1.2363
S: 102.0000 | Call: 94.1536 | Put: 1.2254
S: 103.0000 | Call: 95.1428 | Put: 1.2146
S: 104.0000 | Call: 96.1321 | Put: 1.2039
S: 105.0000 | Call: 97.1217 | Put: 1.1935

A few things worth noting here are that: 
	1. As the spot increases the option grows in value (intrinsic value of the option highers)
	2. The put option loses value as S increases: as the option becomes more and more out of the money it becomes less attractive
	3. The call grows more than the put decreases: this is due the higher time value and interest rate.

These results are enforced by the matrix (S = 100) which in this case refers to a call (Can be changed in OptionMtrix.cpp):
	1. For very short maturities the prices are very high, they grow as the option become more ITM and as the time grows : more time -> more things can happen basically
	2. The highest price of the matrix is the one more In the money and the highest volatility

##### 2 

 CALL OPTION
Delta (Analytical): 0.5946
Delta (FD, h=0.1000): 0.5946
Delta (FD, h=0.0100): 0.5946
Delta (FD, h=0.0010): 0.5946
Delta (FD, h=0.0001): 0.5946
Gamma (Analytical): 0.0135
Gamma (FD, h=0.1000): 0.0135
Gamma (FD, h=0.0100): 0.0135
Gamma (FD, h=0.0010): 0.0135
Gamma (FD, h=0.0001): 0.0135


 PUT OPTION
Delta (Analytical): -0.3566
Delta (FD, h=0.1000): -0.3566
Delta (FD, h=0.0100): -0.3566
Delta (FD, h=0.0010): -0.3566
Delta (FD, h=0.0001): -0.3566
Gamma (Analytical): 0.0135
Gamma (FD, h=0.1000): 0.0135
Gamma (FD, h=0.0100): 0.0135
Gamma (FD, h=0.0010): 0.0135
Gamma (FD, h=0.0001): 0.0135

In the above output we calculate delta and gamma for the parameters given by the question : S = 105, K = 100, T = 0.5, sigma = 0.36, r = 0.1. Delta and Gamma are very accurate and so is their estimation using divided differences. I chose arbitrarily three h values (not too small but not too big) and for all three the estimation is accurate.

S: 100.0000 | Call: 9.6341 | Delta (Call): 0.5238 | Put: 9.6341 | Delta (Put): -0.4274| Gamma: 0.0148
S: 101.0000 | Call: 10.1652 | Delta (Call): 0.5385 | Put: 9.2140 | Delta (Put): -0.4128| Gamma: 0.0146
S: 102.0000 | Call: 10.7109 | Delta (Call): 0.5529 | Put: 8.8085 | Delta (Put): -0.3983| Gamma: 0.0143
S: 103.0000 | Call: 11.2710 | Delta (Call): 0.5671 | Put: 8.4173 | Delta (Put): -0.3842| Gamma: 0.0141
S: 104.0000 | Call: 11.8450 | Delta (Call): 0.5810 | Put: 8.0401 | Delta (Put): -0.3702| Gamma: 0.0138
S: 105.0000 | Call: 12.4328 | Delta (Call): 0.5946 | Put: 7.6767 | Delta (Put): -0.3566| Gamma: 0.0135
S: 106.0000 | Call: 13.0342 | Delta (Call): 0.6080 | Put: 7.3268 | Delta (Put): -0.3433| Gamma: 0.0132
S: 107.0000 | Call: 13.6487 | Delta (Call): 0.6210 | Put: 6.9901 | Delta (Put): -0.3302| Gamma: 0.0129
S: 108.0000 | Call: 14.2761 | Delta (Call): 0.6338 | Put: 6.6663 | Delta (Put): -0.3175| Gamma: 0.0126
S: 109.0000 | Call: 14.9161 | Delta (Call): 0.6462 | Put: 6.3551 | Delta (Put): -0.3050| Gamma: 0.0123
S: 110.0000 | Call: 15.5684 | Delta (Call): 0.6583 | Put: 6.0561 | Delta (Put): -0.2929| Gamma: 0.0119
S: 111.0000 | Call: 16.2326 | Delta (Call): 0.6701 | Put: 5.7691 | Delta (Put): -0.2811| Gamma: 0.0116
S: 112.0000 | Call: 16.9085 | Delta (Call): 0.6816 | Put: 5.4937 | Delta (Put): -0.2697| Gamma: 0.0113
S: 113.0000 | Call: 17.5956 | Delta (Call): 0.6927 | Put: 5.2297 | Delta (Put): -0.2585| Gamma: 0.0110
S: 114.0000 | Call: 18.2938 | Delta (Call): 0.7035 | Put: 4.9765 | Delta (Put): -0.2477| Gamma: 0.0106
S: 115.0000 | Call: 19.0025 | Delta (Call): 0.7140 | Put: 4.7341 | Delta (Put): -0.2373| Gamma: 0.0103

The delta for the call option also increases with stock price, meaning the option moves more closely with the underlying asset. The delta for the put option is negative, indicating an inverse relationship with the stock price, and its absolute value decreases, meaning the sensitivity of the put option to price changes declines as the stock price rises. Gamma represents the rate of change of delta and measures how sensitive delta is to fluctuations in the stock price. Its values are relatively low and slowly decrease, which suggests that the options are becoming less responsive to changes in the underlying price. The higher volatility and short time to expiration contribute to these delta and gamma characteristics. The interest rate also plays a role in favoring call options, as higher rates generally increase the value of call options while reducing the value of puts.

This data suggests that as time progresses and the stock price rises, the call options become more valuable, while the put options lose value. The Greeks also indicate that these options exhibit typical behavior in response to changes in the underlying price, with delta increasing for calls, decreasing in magnitude for puts, and gamma gradually declining.

$$
\Delta = \frac{V(S + h) - V(S - h)}{2h}
$$
$$
\Gamma = \frac{V(S + h) - 2V(S) + V(S - h)}{h^2}
$$

The Taylor series expansion around S gives:
$$
V(S \pm h) = V(S) \pm h V'(S) + \frac{h^2}{2} V''(S) \pm \frac{h^3}{6} V^{(3)}(S) + \frac{h^4}{24} V^{(4)}(S) + O(h^5).
$$

Substituting this into the formula of Delta above: 
$$
\Delta \approx \frac{\left( V(S) + h V'(S) + \frac{h^2}{2} V''(S) + O(h^3) \right) - \left( V(S) - h V'(S) + \frac{h^2}{2} V''(S) + O(h^3) \right)}{2h}
$$

Simplifying opposite terms: $$
\Delta = V'(S) + O(h^2)
$$
which confirms second-order accuracy.

For Gamma the reasoning is the same : $$\frac{V(S) + h V'(S) + \frac{h^2}{2} V''(S) + V(S) - h V'(S) + \frac{h^2}{2} V''(S) - 2V(S) + O(h^3)}{h^2}
$$
Simplifying:$$
\Gamma \approx \frac{h^2 V''(S) + O(h^3)}{h^2} = V''(S) + O(h^2)$$which also proves second-order accuracy.


##### 3

S: 95.0000 | Call: 7.5409 | Put: 11.5460
S: 96.0000 | Call: 7.0656 | Put: 11.9416
S: 97.0000 | Call: 6.6248 | Put: 12.3464
S: 98.0000 | Call: 6.2156 | Put: 12.7605
S: 99.0000 | Call: 5.8354 | Put: 13.1842
S: 100.0000 | Call: 5.4819 | Put: 13.6174
S: 101.0000 | Call: 5.1531 | Put: 14.0603
S: 102.0000 | Call: 4.8469 | Put: 14.5131
S: 103.0000 | Call: 4.5617 | Put: 14.9758
S: 104.0000 | Call: 4.2957 | Put: 15.4486
S: 105.0000 | Call: 4.0476 | Put: 15.9316
S: 106.0000 | Call: 3.8160 | Put: 16.4249
S: 107.0000 | Call: 3.5996 | Put: 16.9286
S: 108.0000 | Call: 3.3973 | Put: 17.4429
S: 109.0000 | Call: 3.2081 | Put: 17.9678
S: 110.0000 | Call: 3.0311 | Put: 18.5035

For stock prices below 100, put options are more valuable than calls because they allow the holder to sell at a higher fixed price compared to market rates. As the stock price rises, the call option price decreases slowly, reflecting a diminishing likelihood that the option holder will benefit from buying at the strike price. Conversely, the put price steadily increases as the stock price moves further from the strike, making it more valuable for protection against price declines.
As the stock price approaches and surpasses 100, calls become cheaper and puts more expensive.

After creating a perpetual option matrix here are my observations:
	1. Higher volatility increases call option prices (as expected)
	2. Lower strike prices lead to higher call option values
	3. Nonlinear price increase with volatility: Higher volatilities cause larger relative jumps in call prices, reflecting the impact of risk.
