#pragma once
#include <vector>
#include <string>

struct TickerData {
    std::vector<double> open;
    std::vector<double> high;
    std::vector<double> low;
    std::vector<double> close;
    std::vector<double> volume;
    std::vector<double> returns;
    std::string tickerName;
    double interval;
};

class Parser {
    public:
        static TickerData parseTwelveDataCSV (const std::string & filePath);
};