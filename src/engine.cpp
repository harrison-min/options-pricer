#include "engine.hpp"
#include <cmath>
#include <vector>
#include <cassert>
#include <random>

static const double sqrtTotalTradingPeriods = std::sqrt(252.0);
static const double riskFreeRate = 0.0431; //10 y treasury yield
static const int stepsPerPath = 100;

static inline double cumulativeNormal (double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

static double generateRandomNormal (double mean = 0.0, double deviation = 1.0) {
    static std::random_device rd{};
    static std::mt19937 gen{rd()};

    std::normal_distribution<double> distribution(mean, deviation);

    return distribution (gen);
}


double AnalyticsEngine::getMCSPrice (double volatility, double spotPrice, double sqrtTimeStep) {
    double delta = riskFreeRate - 0.5 * volatility * volatility; 

    double randomNumber = generateRandomNormal();

    double normalizedPrice = std::exp(delta * sqrtTimeStep * sqrtTimeStep + volatility * sqrtTimeStep * randomNumber);

    return normalizedPrice * spotPrice;
}

void AnalyticsEngine::createGBMMCSPath (double volatility, double spotPrice, double timeFrame, std::vector<double>::iterator path ) {
    const double timeStep = timeFrame / static_cast<double>(stepsPerPath);
    const double sqrtTimeStep = std::sqrt(timeStep);

    double currentPrice = spotPrice;
    *path = currentPrice;

    for (int i = 1; i < stepsPerPath; ++ i) {
        *(path + i) = getMCSPrice(volatility, currentPrice, sqrtTimeStep);
        currentPrice = *(path + i);
    }
}

double AnalyticsEngine::monteCarloSimulationCallPrice (double volatility, double spotPrice, double strikePrice, double timeFrame, int sampleSize) {
    std::vector<double> path;
    path.resize(stepsPerPath * sampleSize);
    double average = 0;

    for (int i = 0; i < sampleSize; ++ i) {
        createGBMMCSPath(volatility, spotPrice, timeFrame, path.begin() + (i * stepsPerPath));
        double finalPrice=(path [(i + 1) * stepsPerPath - 1]);
        average += std::max(0.0, finalPrice - strikePrice);
    }

    average /= static_cast<double> (sampleSize);

    return std::exp (-riskFreeRate * timeFrame) * average;
}

double AnalyticsEngine::blackScholesCallPrice (double volatility, double spotPrice, double strikePrice, double timeToMaturity) {
    
    assert(volatility > 0 && timeToMaturity > 0);

    const double totalVolatility = volatility * std::sqrt (timeToMaturity);

    double delta = (std::log (spotPrice / strikePrice) + (riskFreeRate + volatility * volatility * 0.5) * timeToMaturity) / totalVolatility;

    double probExercise = delta - totalVolatility;

    double n1 = cumulativeNormal(delta);
    double n2 = cumulativeNormal(probExercise);

    double callPrice = spotPrice * n1 - strikePrice * std::exp (- riskFreeRate * timeToMaturity) * n2;


    return callPrice;

}

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

