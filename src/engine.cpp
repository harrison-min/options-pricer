#include "engine.hpp"
#include <cmath>
#include <vector>
#include <cassert>

static const double sqrtTotalTradingPeriods = std::sqrt(252.0);

double AnalyticsEngine::closeToCloseVolatility (const TickerData &data) {
    // currently setting dividends to 0 as API has not been set up yet
    size_t totalDataPoints = data.close.size();
    assert(totalDataPoints > 2);

    double meanReturns = 0;
    std::vector<double> dividend (totalDataPoints, 0);
    std::vector<double> returns (totalDataPoints - 1);

    for(size_t i = 0; i < totalDataPoints - 1; ++i) {
        returns[i] = std::log((data.close[i+1] + dividend[i+1]) / data.close[i]);
        meanReturns += returns[i];
    }

    meanReturns /= static_cast<double>(totalDataPoints - 1);

    double sum = 0;
    for (size_t i = 0; i < totalDataPoints - 1; ++ i) {
        double delta = returns[i] - meanReturns;

        sum += (delta * delta);
    }

    double dailyVolatility = std::sqrt (sum / static_cast<double> (totalDataPoints - 1));

    double annualizedVolatility = dailyVolatility * sqrtTotalTradingPeriods;

    return annualizedVolatility;

}


double AnalyticsEngine::parkinsonVolatility (const TickerData &data) {
    size_t totalDataPoints = data.close.size();
    assert(totalDataPoints > 2);

    double sum = 0;

    for (size_t i = 0; i < totalDataPoints; ++i) {
        double open = data.open[i];
        double delta = std::log(data.high[i] / open) - std::log(data.low[i] / open);
       
        sum += (delta * delta);
    }

    double constant = 1.0 / (4.0 * std::log(2.0));

    double dailyVolatility = std::sqrt(constant * sum / static_cast<double>(totalDataPoints));

    double annualizedVolatility = dailyVolatility * sqrtTotalTradingPeriods;

    return annualizedVolatility;
}