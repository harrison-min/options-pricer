#include "parser.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>

TickerData Parser::parseTwelveDataCSV (const std::string & filePath) {
    TickerData data;
    data.open.reserve(100);
    data.high.reserve(100);
    data.low.reserve(100);
    data.close.reserve(100);
    data.volume.reserve(100);

    data.interval = 1;

    auto firstSlash = std::find(filePath.begin(), filePath.end(), '/');
    auto firstUnderscore = std::find (filePath.begin(), filePath.end(), '_');
    data.tickerName = std::string(firstSlash + 1, firstUnderscore);

    std::ifstream file;     
    file.open(filePath);

    std::string headerLine;
    std::getline(file, headerLine);

    std::string currentLine;
    std::string chunk;
    while (std::getline(file, currentLine)) {
        if (currentLine.empty()) continue;

        std::stringstream stream(currentLine);
        
        std::getline(stream, chunk, ',');
        // skip the date

        std::getline(stream, chunk, ',');
        data.open.push_back(std::stod(chunk));

        std::getline(stream, chunk, ',');
        data.high.push_back(std::stod(chunk));

        std::getline(stream, chunk, ',');
        data.low.push_back(std::stod(chunk));

        std::getline(stream, chunk, ',');
        data.close.push_back(std::stod(chunk));

        std::getline(stream, chunk, ',');
        data.volume.push_back(std::stod(chunk));
    }

    file.close();

    return data;
}