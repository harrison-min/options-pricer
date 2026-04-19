#pragma once
#include <string>

enum class RequestType {
    PLACE_ORDER,
    CANCEL_ORDER,
    FETCH_HISTORICAL_DATA,
    FETCH_ORDER_DATA
};

class RequestBuilder {
    private:
        std::string filePath;
        std::string destination; 
        std::string ticker;
        RequestType type;

        std::string typeToString();
    public:
        RequestBuilder(std::string symbol, RequestType action);
        RequestBuilder & writeRequest();
        RequestBuilder & sendRequest();
        RequestBuilder & setDestination(std::string source);
        RequestBuilder & setTicker(std::string symbol);
        RequestBuilder & setType(RequestType action);

};