/*
The goal of this class is to implement the longstaff-schwartz american option pricer presented in https://people.math.ethz.ch/~hjfurrer/teaching/LongstaffSchwartzAmericanOptionsLeastSquareMonteCarlo.pdf.

Given interest rate, strike price, and a simulated # of paths for the stock, we can calculate the optimal stopping point before expiration. The option price will be the sum of the cash flow at the optimal stopping point divided by the number of paths.
*/

#pragma once

#include <vector>

class longstaffSchwartz{
private:
    double interest_rate;
    double strike_price;
    double discount_factor;
    std::vector<std::vector<double>> paths;
    std::vector<std::vector<double>> cash_flow;
    
public:
    longstaffSchwartz(const double interest_rate, const double strike_price, std::vector<std::vector<double>> paths);
    void changeStock(const double interest_rate, const double strike_price, std::vector<std::vector<double>> paths);
    double getOptionPrice(const bool isCall);
    std::vector<double> polyfit(const std::vector<double> X, const std::vector<double> Y, const int degree);
    double polyval(const std::vector<double> coefficients, const double value);
};