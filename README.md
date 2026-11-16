# European Options Pricing Model in C++ with Charles Schwab dev API in python  

A custom options pricing model written in C++ using Markov Chain Monte Carlo (MCMC) and Black-Scholes models. The volatility used in the models are based off of close to close and Parkinson volatility. The python portion handles requests out to Charles Schwab Developer API.

## Features

* **Analytics Engine:** Analytics Engine class handles the modeling of the price for a given derivatives contract (European) with a given strike price and time to expiry. Models use historical price data as the primary data input.
* **API handler:** Support for price data of Charles Schwab REST API and Twelve Data REST API.  
* **Custom File-Based Request/Response System** JSON based requests are made from the C++ engine to the python based API handler with CSV responses that contain the relevant data. Requests and responses are uniquely numbered for clear response-request pairs. 
