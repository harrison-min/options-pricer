#include <iostream>
#include <cstdlib>
#include <iomanip>

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

TickerData fetchData(const std::string & ticker, std::string source) {
    std::cout << "Fetching Data\n";
    std::string command = "python3 src/fetch_data.py " + ticker + " " + source;

    std::system(command.c_str());

    std::string filePath = "data/" + ticker + "_" + source + "_data.csv";
    TickerData myData = Parser::parseTickerCSV(filePath);

    return myData;
}

double getOptionData (TickerData data, OptionInfo option, const int sampleSize) {
    double closeToCloseVolatility = AnalyticsEngine::closeToCloseVolatility(data);
    double parkinsonVolatility = AnalyticsEngine::parkinsonVolatility(data);
    std::vector<double> chain = AnalyticsEngine::createMCMCChain(closeToCloseVolatility, data, sampleSize);
    double mcmcVolatility = 0;

    for (size_t i = chain.size() * 0.2; i < chain.size(); ++ i) {
        mcmcVolatility += chain [i];
    }
    mcmcVolatility /= (chain.size() - chain.size() * 0.2);



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

    double ctcMCSprice = AnalyticsEngine::monteCarloSimulationPrice(closeToCloseVolatility, option, sampleSize);
    double parkinsonMCSprice = AnalyticsEngine::monteCarloSimulationPrice(parkinsonVolatility, option, sampleSize);
    double mcmcMCSprice = AnalyticsEngine::monteCarloSimulationPrice(mcmcVolatility, option, sampleSize);

    std::cout << "MCS Call price CTC: " << ctcMCSprice 
                << " vs. parkinson price: " << parkinsonMCSprice
                << " vs. mcmc price: " << mcmcMCSprice << "\n";

    return mcmcMCSprice;
}


int main(int argc, char ** argv) {


    std::string ticker = "AMZN";

    TickerData data = fetchData(ticker, "CS");
    OptionInfo option (OptionType::CALL , data.close.back(), data.close.back() + 12, 112.0/252.0);

    printTickerData(data);

//    getOptionData(data, option, 10000);

}