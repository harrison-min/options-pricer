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
        std::string ticker;
        RequestType type;

        std::string typeToString();
    public:
        RequestBuilder(std::string symbol, RequestType action);
        bool writeRequest();
        RequestBuilder & setTicker(std::string symbol);
        RequestBuilder & setType(RequestType action);
};