#pragma once
#include "parser.hpp"

class AnalyticsEngine {
    public:
        static double parkinsonVolatility (const TickerData &data);
        static double closeToCloseVolatility (const TickerData &data);
};