#include "request.hpp"
#include <fstream>
#include <filesystem>

RequestBuilder::RequestBuilder(std::string symbol, RequestType action):
    ticker(symbol), type(action) {

}

std::string RequestBuilder::typeToString() {
    switch(type) {
        case RequestType::FETCH_HISTORICAL_DATA:
            return "FETCH_HISTORICAL_DATA";
            break;
        case RequestType::FETCH_ORDER_DATA:
            return "FETCH_ORDER_DATA";
            break;
        case RequestType::CANCEL_ORDER:
            return "CANCEL_ORDER";
            break;
        case RequestType::PLACE_ORDER:
            return "PLACE_ORDER";
            break;
    }
    return "";
}

RequestBuilder & RequestBuilder::sendRequest() {
    std::string command = "python3 src/fetch_data.py " + filePath; 
    std::system(command.c_str());

    return *this;
}

RequestBuilder & RequestBuilder::writeRequest() {
    std::string baseName = "request/request_";
    std::string extension = ".json";
    int fileCounter = 0;

    std::string path = baseName + std::to_string(fileCounter) + extension;
    while (std::filesystem::exists(path)) {
        fileCounter ++;
        path = baseName + std::to_string(fileCounter) + extension;
    }

    filePath = path;

    std::ofstream file(filePath);

    file << "{\n";
    file << "  \"Destination\": \"" << destination << "\",\n";
    file << "  \"Request Type\": \"" << typeToString() << "\",\n";
    file << "  \"Ticker\": \"" << ticker << "\"";

    for (const auto & [key, value] : extraParameters) {
        file << ",\n";
        file << "  \"" << key << "\": \"" << value << "\"";
    }

    file << "\n}";

    return *this;
}

RequestBuilder & RequestBuilder::setExtraParameter(const std::string & key, const std::string & value) {
    extraParameters [key] = value;
    return *this;
}

RequestBuilder & RequestBuilder::setDestination(const std::string & source) {
    destination = source;
    return *this;
}

RequestBuilder & RequestBuilder::setTicker(const std::string & symbol) {
    ticker = symbol;
    return *this;
}

RequestBuilder & RequestBuilder::setType(RequestType action) {
    type = action;
    return *this;
}

RequestBuilder & RequestBuilder::clearExtraParameters() {
    extraParameters.erase(extraParameters.begin(), extraParameters.end());
    return *this;
}