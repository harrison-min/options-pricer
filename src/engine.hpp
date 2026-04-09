#pragma once
#include "parser.hpp"

enum class OptionType {
    CALL,
    PUT
};

struct OptionInfo {
    OptionType type;
    double spotPrice;
    double strikePrice;
    double timeToExpiry;


    OptionInfo (OptionType t, double spot, double strike, double time)
        : type (t), spotPrice(spot), strikePrice(strike), timeToExpiry(time) {
    }
};


class AnalyticsEngine {
    private:
        static double getMCSPrice (double volatility, OptionInfo option, double timeStep);
        static void createGBMMCSPath (double volatility, OptionInfo option, std::vector<double>::iterator path);
        static double volatilityLikelihood (double volatility, const TickerData & data, const std::vector<double>& returns);
        static std::pair<double, double> metroHastingsChoice (double vCandidate, double pCandidate, double vCurr, double pCurr);
        static std::vector<double> precomputeLogReturns (const TickerData & data);
    public:
        static double parkinsonVolatility (const TickerData &data);
        static double closeToCloseVolatility (const TickerData &data);
        static double blackScholesPrice (double volatility, OptionInfo option);
        static double monteCarloSimulationPrice (double volatility, OptionInfo option, int sampleSize);
        static std::vector<double> createMCMCChain (double startingVolatility, const TickerData & data, int chainLength);
};