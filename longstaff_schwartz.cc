#include "longstaff_schwartz.h"
#include "geometric_brownian_motion.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cassert>

std::vector<double> longstaffSchwartz::polyfit(const std::vector<double> x, const std::vector<double> y, const int degree){
    assert(x.size() == y.size());
    assert(x.size() > 0);
    std::vector<double> coefficients (degree+1, 0.0);
    int i, j, k;
    
    // Store independent components of normal matrix
    std::vector<double> X((2 * degree + 1), 0.0);
    for(i = 0; i < X.size(); i++){
        for(j = 0; j < x.size(); j++){
            X[i] += pow(x[j], i);
        }
    }

    // Store right hand side values
    std::vector<double> Y((degree + 1), 0.0);
    for(i = 0; i < Y.size(); i++){
        for(j = 0; j < x.size(); j++){
            Y[i] += pow(x[j], i)*y[j];
        }
    }

    // Generate normal augmented matrix
    std::vector<std::vector<double>> B((degree + 1), std::vector<double>(degree+2, 0.0));
    for(i = 0; i < degree+1; i++){
        for(j = 0; j < degree+1; j++){
            B[i][j] = X[i+j];
        }
    }
    for(i = 0; i < Y.size(); i++){
        B[i][degree+1] = Y[i];
    }

    for(i = 0; i < degree; i++){
        // If diagonal element < terms below, swap rows
        for(j = i+1; j < degree+1; j++){
            if(fabs(B[i][i]) < fabs(B[j][i])){
                for(k = 0; k < degree + 2; k++){
                    double temp = B[i][k];
                    B[i][k] = B[j][k];
                    B[j][k] = temp;
                }
            }
        }

        // Gaussian Elimination
        for(j = i+1; j < degree+1; j++){
            double term = B[j][i] / B[i][i];
            for(k = 0; k < degree+2; k++){
                B[j][k] = B[j][k] - term * B[i][k];
            }
        }
    }

    // Back substitution
    for(i = degree; i >= 0; i--){
        coefficients[i] = B[i][degree+1];
        for(j = i+1; j < degree+2; j++){
            coefficients[i] -= B[i][j] * coefficients[j];
        }
        coefficients[i] /= B[i][i];
    }

    return coefficients;
}

double longstaffSchwartz::polyval(const std::vector<double> coefficients, const double X){
    double result = 0.0;

    // Evaluate polynomial for X
    for(int i = 0; i < coefficients.size(); i++){
        result += coefficients[i] * std::pow(X, i);
    }

    return result;
}

longstaffSchwartz::longstaffSchwartz(const double interest_rate, const double strike_price,  std::vector<std::vector<double>> paths) :
    interest_rate{interest_rate},
    strike_price{strike_price},
    discount_factor{exp(-interest_rate)},
    paths{paths},
    cash_flow{std::vector<std::vector<double>>(paths.size(), std::vector<double>(paths[0].size(), 0.0) )}
{
}

 void longstaffSchwartz::changeStock(const double interest_rate, const double strike_price, std::vector<std::vector<double>> paths)
 {
    this->interest_rate = interest_rate;
    this->strike_price = strike_price;
    this->discount_factor = exp(-interest_rate);
    this->paths = paths;
    this->cash_flow = std::vector<std::vector<double>>(paths.size(), std::vector<double>(paths[0].size(), 0.0) );
 }

double longstaffSchwartz::getOptionPrice(const bool isCall){
    double option_price = 0.0;
    std::vector<std::vector<double>> regression;
    std::vector<double> coefficients;

    // Fill cash flow at last time step
    for(int idx = 0; idx < cash_flow.size(); idx++){
        cash_flow[idx][cash_flow[idx].size() - 1] = std::max(strike_price - paths[idx][paths[idx].size() - 1], 0.0);
    }

    // Iterate from last to first step
    for(int i = cash_flow[0].size()-2; i >= 0; i--){
        // Generate values for polynomial regression
        // X = stock price, Y = cash flow * discount factor
        regression = std::vector<std::vector<double>>(2, std::vector<double>{});
        for(int a = 0; a < cash_flow.size(); a++){
            if((isCall) ? paths[a][i] > strike_price : paths[a][i] < strike_price){
                regression[0].push_back(paths[a][i]);
                regression[1].push_back(cash_flow[a][i+1] * discount_factor);
            }
        }
        if(regression[0].size() > 0){
            // Regress on X and Y
            coefficients = polyfit(regression[0], regression[1], 2);

            for(int a = 0; a < cash_flow.size(); a++){
                // For Call option
                if(isCall){
                    if(paths[a][i] > strike_price){
                        // If cash flow at current t is greater than continuation, update cash flow at current point, then change all future cash flows to 0
                        if(paths[a][i] - strike_price  >= polyval(coefficients, paths[a][i])){
                            cash_flow[a][i] = paths[a][i] - strike_price;
                            for(int j = i+1; j < cash_flow[0].size(); j++)
                                cash_flow[a][j] = 0.0;
                        }
                    }
                // For Put option
                } else {
                    // If cash flow at current t is greater than continuation, update cash flow at current point, then change all future cash flows to 0
                    if(paths[a][i] < strike_price){
                        if(strike_price - paths[a][i]  >= polyval(coefficients, paths[a][i])){
                            cash_flow[a][i] = strike_price - paths[a][i];
                            for(int j = i+1; j < cash_flow[0].size(); j++)
                                cash_flow[a][j] = 0.0;
                        }
                    }
                }
            }

        }
    }

    // Add all values in cash flow matrix, then divide by the number of paths
    for(int i = 0; i < cash_flow.size(); i++){
        for(int j = 0; j < cash_flow[0].size(); j++){
            option_price += cash_flow[i][j] * std::pow(discount_factor, j);
        }
    }
    option_price /= cash_flow.size();

    return option_price;
}