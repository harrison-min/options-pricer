#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <random>

#include "parser.hpp"
#include "engine.hpp"

void printTickerData(const TickerData & data) {
    std::cout << "TickerData for ticker: " << data.tickerName << std::endl;

    for (size_t i = 0; i < data.close.size(); ++ i) {
        std::cout << std::setw (10) << "Row " << i
            << std::setw (15) << data.open[i]
            << std::setw (15) << data.high[i]
            << std::setw (15) << data.low[i]
            << std::setw (15) << data.close[i]
            << std::setw (15) << data.volume[i] << "\n";
    }
}

void fetchData(const std::string & ticker) {
    std::cout << "Fetching Data\n";
    std::string command = "python3 src/fetch_data.py " + ticker;

    std::system(command.c_str());

}

int main(int argc, char ** argv) {
    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::string ticker = "AMZN";
    std::string filePath = "data/" + ticker + "_data.csv";

    fetchData(ticker);
    TickerData myData = Parser::parseTwelveDataCSV(filePath);

    double closeToCloseVolatility = AnalyticsEngine::closeToCloseVolatility(myData);
    double parkinsonVolatility = AnalyticsEngine::parkinsonVolatility(myData);
    const int chainLength = 100000;
    std::vector<double> chain = AnalyticsEngine::createMCMCChain(closeToCloseVolatility, myData, chainLength);
    double mcmcVolatility = 0;

    for (size_t i = chain.size() * 0.2; i < chain.size(); ++ i) {
        mcmcVolatility += chain [i];
    }
    mcmcVolatility /= (chain.size() - chain.size() * 0.2);


    OptionInfo option (OptionType::CALL , myData.close.back(), myData.close.back() + 10, 100.0/252.0);

    std::cout << "Options stats:\nSpotPrice: " << option.spotPrice << ", strikePrice: " << option.strikePrice << ", timeToExpiry: " << option.timeToExpiry << "\n";

    std::cout << "Close to Close volatility : " << closeToCloseVolatility 
                << " vs. Parkinson Volatility : " << parkinsonVolatility 
                << " vs. MCMC Volatility : " << mcmcVolatility << "\n";

    double callCTCPrice = AnalyticsEngine::blackScholesPrice(closeToCloseVolatility, option);
    double parkinsonPrice = AnalyticsEngine::blackScholesPrice(parkinsonVolatility, option);
    double mcmcPrice = AnalyticsEngine::blackScholesPrice(mcmcVolatility, option);

    std::cout << "Black scholes Call price CTC: " << callCTCPrice 
                << " vs. parkinson price: " << parkinsonPrice 
                << " vs. MCMC price: " << mcmcPrice << "\n";

    const int numSteps = 100000;                
    double ctcMCSprice = AnalyticsEngine::monteCarloSimulationPrice(closeToCloseVolatility, option, numSteps);
    double parkinsonMCSprice = AnalyticsEngine::monteCarloSimulationPrice(parkinsonVolatility, option, numSteps);
    double mcmcMCSprice = AnalyticsEngine::monteCarloSimulationPrice(mcmcVolatility, option, numSteps);

    std::cout << "MCS Call price CTC: " << ctcMCSprice 
                << " vs. parkinson price: " << parkinsonMCSprice
                << " vs. mcmc price: " << mcmcMCSprice << "\n";
}