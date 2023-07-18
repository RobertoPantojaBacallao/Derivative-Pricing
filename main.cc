#include "geometric_brownian_motion.h"
#include "longstaff_schwartz.h"
#include <iostream>

int main(){
    std::vector<std::vector<double>> values;
    // First, let's run through the example in the research paper

    values.push_back(std::vector<double>{1.0, 1.09, 1.08, 1.34});
    values.push_back(std::vector<double>{1.0, 1.16, 1.26, 1.54});
    values.push_back(std::vector<double>{1.0, 1.22, 1.07, 1.03});
    values.push_back(std::vector<double>{1.0, 0.93, 0.97, 0.92});
    values.push_back(std::vector<double>{1.0, 1.11, 1.56, 1.52});
    values.push_back(std::vector<double>{1.0, 0.76, 0.77, 0.90});
    values.push_back(std::vector<double>{1.0, 0.92, 0.84, 1.01});
    values.push_back(std::vector<double>{1.0, 0.88, 1.22, 1.34});

    longstaffSchwartz ls = longstaffSchwartz(
        0.06, // interest rate
        1.1, // strike price
        values // stock value path simulation
    );

    std::cout << "Research Paper Ex. Option Price: " << ls.getOptionPrice(false) << std::endl;

    values.clear();

    // Now, let's use brownian motion to generate paths of a stock price

    geometricBrownianMotion gbm(
        0.01, // Interest rate
        0.15, // Volatility
        100, // Time to expiry
        1.31 // Stock Price
        );

    // Generate paths of stock movement using brownian motion
    for(int i = 0; i < 50; i++){
        values.push_back(gbm.generate_values(gbm.time_to_expiry));
    }

    // Run longstaff-schwartz for a call option

    ls.changeStock(
        gbm.getInterestRate(), // interest rate
        1.1, // strike price
        values // stock value path simulation
    );

    std::cout << "Random Generated Option Price: " << ls.getOptionPrice(true) << std::endl;
}