#include "geometric_brownian_motion.h"
#include <ctime>
#include <iostream>
#include <random>

geometricBrownianMotion::geometricBrownianMotion(const double riskless_rate, const double sigma, const int t, const double stock_price) :
        drift{std::exp((riskless_rate - 0.5 * (pow(sigma, 2))))},
        vol{sigma * std::sqrt(1)},
        curr_stock_price{stock_price},
        rand{std::mt19937::result_type(std::time(0))},
        distribution{0.0, 1.0},
        time_to_expiry{t},
        interest_rate{riskless_rate}
{}

double geometricBrownianMotion::getInterestRate(){
    return this->interest_rate;
}

std::vector<double> geometricBrownianMotion::generate_values(int steps){
    if(steps == -1)
        steps = time_to_expiry;
    // Generate one stock path for steps number of steps
    std::vector<double> values(steps+1, curr_stock_price);
    std::generate(std::begin(values)+1, std::end(values), *this);
    return values;
}

double geometricBrownianMotion::operator()(){
    const double Zi = distribution(rand);
    return curr_stock_price *= drift * std::exp(vol*Zi);
}

