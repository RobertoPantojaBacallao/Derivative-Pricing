/*
The goal of this class is to be able to generate potential paths of a stock price using the geometric brownian motion (GBM) equation.

GBM: 

(ΔS)/S = μΔt + δE(sqrt(Δt))

where
    S = stock price
    μ = expected return
    δ = standard deviation of returns
    E = random var
    t = time

Solving for ΔS/S, we get the following equation:

    S(t) = S(0) * e^( (μ - (δ**2)/2)*t +  δ)

    ΔS = S * ( μΔt + δE(sqrt(Δt)) )

Given S, we would like to calculate possible stock price fluctuations
*/
#pragma once

#include <vector>
#include <random>
#include <array>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iterator>

class geometricBrownianMotion{
    private:
        double curr_stock_price;
        double interest_rate;
        std::mt19937 rand;
        std::normal_distribution<double> distribution;

    public:
        const double drift;
        const double vol;
        const int time_to_expiry;

        geometricBrownianMotion(const double riskless_rate, const double sigma, const int t, const double stock_price);
        std::vector<double> generate_values(int steps=-1);
        double getInterestRate();
        double operator()();
};