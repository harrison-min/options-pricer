#pragma once
#include "parser.hpp"

class AnalyticsEngine {
        static double getMCSPrice (double volatility, double spotPrice, double timeStep);
    public:
        static double parkinsonVolatility (const TickerData &data);
        static double closeToCloseVolatility (const TickerData &data);
        static double blackScholesCallPrice (double volatility, double spotPrice, double strikePrice, double timeToMaturity);
        static std::vector<double> createGBMMCSPath (double volatility, double spotPrice, double timeFrame);
};