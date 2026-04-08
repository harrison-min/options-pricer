#pragma once
#include "parser.hpp"

class AnalyticsEngine {
    private:
        static double getMCSPrice (double volatility, double spotPrice, double timeStep);
        static void createGBMMCSPath (double volatility, double spotPrice, double timeFrame, std::vector<double>::iterator path);
    public:
        static double parkinsonVolatility (const TickerData &data);
        static double closeToCloseVolatility (const TickerData &data);
        static double blackScholesCallPrice (double volatility, double spotPrice, double strikePrice, double timeToMaturity);
        static double monteCarloSimulationCallPrice (double volatility, double spotPrice, double strikePrice, double timeFrame, int sampleSize);
};