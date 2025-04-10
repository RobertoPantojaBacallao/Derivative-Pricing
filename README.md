# Longstaff-Schwarts Option Pricer

European options and American options differ in that European options can only be exercised on the expiration date, while American ones can be exercised early.

For this reason, pricing American options has been trickier. The [Longstaff-Schwarts](https://people.math.ethz.ch/~hjfurrer/teaching/LongstaffSchwartzAmericanOptionsLeastSquareMonteCarlo.pdf) pricing method is an interesting pricing algorithm.

We simulate different paths the stock price can take and use this information to derive a price.

# Setup

If you're on mac, use the optionPricer executable. If you're on windows, use the optionPricer.exe executable.

The output is the computed price for the research paper example and the output for a randomly generated set of stock prices.

# Build

If you'd like to make any changes, run `g++ -arch arm64 -o optionPricer main.cc longstaff_schwartz.cc geometric_brownian_motion.cc` to compile on mac and `g++ -o optionPricer main.cc longstaff_schwartz.cc geometric_brownian_motion.cc` to compile on windows.
