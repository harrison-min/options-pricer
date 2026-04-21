#pragma once
#include <string>
#include <map>

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
        std::map <std::string, std::string> extraParameters;
    public:
        RequestBuilder(std::string symbol, RequestType action);
        RequestBuilder & writeRequest();
        RequestBuilder & sendRequest();
        RequestBuilder & setDestination(const std::string & source);
        RequestBuilder & setTicker(const std::string & symbol);
        RequestBuilder & setType(RequestType action);
        RequestBuilder & setExtraParameter(const std::string & key, const std::string & value);
        RequestBuilder & clearExtraParameters();
};