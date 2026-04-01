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

void fetchData(const std::string & ticker) {
    std::cout << "Fetching Data\n";
    std::string command = "python3 src/fetch_data.py " + ticker;

    std::system(command.c_str());

}

int main(int argc, char ** argv) {
    std::string ticker = "AAPL";
    std::string filePath = "data/" + ticker + "_data.csv";

    fetchData(ticker);
    TickerData myData = Parser::parseTwelveDataCSV(filePath);

    std::cout << "Close to Close volatility : " << AnalyticsEngine::closeToCloseVolatility(myData)
        << " vs. Parkinson Volatility : " << AnalyticsEngine::parkinsonVolatility(myData);
}