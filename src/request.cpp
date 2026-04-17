#include "request.hpp"
#include <fstream>

RequestBuilder::RequestBuilder(std::string symbol, RequestType action):
    ticker(symbol), type(action) {

}

std::string RequestBuilder::typeToString() {
    switch(type) {
        case RequestType::FETCH_HISTORICAL_DATA:
            return "FETCH_HISTORICAL_DATA";
            break;
        case RequestType::FETCH_ORDER_DATA:
            return "FETCH_ORDER_DATA:";
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

bool RequestBuilder::writeRequest() {
    std::ofstream file("request/request.json");

    file << "{\n";
    file << "  \"type\": \"" << typeToString() << "\",\n";
    file << "  \"ticker\": \"" << ticker << "\"";
    file << "\n}";

    return true;
}
RequestBuilder & RequestBuilder::setTicker(std::string symbol) {
    ticker = symbol;
    return *this;
}

RequestBuilder & RequestBuilder::setType(RequestType action) {
    type = action;
    return *this;
}