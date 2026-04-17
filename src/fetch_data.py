import os
import sys
import csv
from dotenv import load_dotenv, dotenv_values
from twelvedata import TDClient

import schwab
from schwab.auth import easy_client
import datetime
import httpx

class RequestHandler:
    def __init__ (self):
        load_dotenv("secrets.env")

        CSSecret = os.getenv("CHARLES_SCHWAB_SECRET")
        CSClientID = os.getenv("CHARLES_SCHWAB_CLIENT_ID")
        self.csClient = easy_client(
            api_key=CSClientID,
            app_secret=CSSecret,
            callback_url='https://127.0.0.1:8182',
            token_path='token/token.json',
            enforce_enums=False)

        TDKey = os.getenv("TWELVE_DATA_KEY")
        self.tdClient = TDClient(TDKey)

    #def parseRequest (self, filePath):         
        

    def fetchTDTickerData(self, ticker):
        tsData = self.tdClient.time_series(
            symbol = ticker,
            interval = "1day",
            outputsize = 90,
            timezone = "America/New_York",
            order = "ASC",
            adjust="true"
        )

        csv_data = tsData.as_csv()

        filename = f"data/{symbol}_TD_data.csv"
        with open(filename, "w", newline="") as f:
            writer = csv.writer(f)
            writer.writerows(csv_data)
 
    def fetchCSTickerData(self, ticker):
        endTime = datetime.datetime.now()
        startTime = endTime - datetime.timedelta(days = 90)

        resp = self.csClient.get_price_history_every_day(ticker, start_datetime = startTime, end_datetime = endTime, need_extended_hours_data=False)
        assert resp.status_code == httpx.codes.OK

        history = resp.json()

        filename = f"data/{ticker}_CS_data.csv"
        with open(filename, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(['datetime', 'open', 'high', 'low', 'close', 'volume'])
            
            for candle in history['candles']:
                writer.writerow([
                    candle['datetime'], 
                    candle['open'], 
                    candle['high'], 
                    candle['low'], 
                    candle['close'], 
                    candle['volume']
                ])


def getAccountInfo (client):
    response = client.get_account_numbers() 
    accounts = response.json()
    
    accountDetails = client.get_account(accounts[0]['hashValue'])
    print (accountDetails.json())


def getOptionsChain (ticker, client):
    response = response = client.get_option_chain(
        symbol=ticker, 
        contract_type="CALL", 
        strike=280, 
        strike_count=1,
        strategy="SINGLE"
    )
    options = response.json()
    
    print(options)

def main():
    if len(sys.argv) < 3:
        print("Too few arguments passed.")
        sys.exit(1)
    
    ticker = sys.argv[1].upper()
    source = sys.argv[2]

    if (source == 'CS'): 
        load_dotenv("secrets.env")

        apiSecret = os.getenv("CHARLES_SCHWAB_SECRET")
        apiClientKey = os.getenv("CHARLES_SCHWAB_CLIENT_ID")
        client = easy_client(
            api_key=apiClientKey,
            app_secret=apiSecret,
            callback_url='https://127.0.0.1:8182',
            token_path='token/token.json',
            enforce_enums=False)
        fetchCSTickerData(ticker, client)
    elif (source == 'TD'):
        fetchTDTickerData(ticker)

if __name__ == "__main__":
    myHandler = RequestHandler()
    ticker = 'AAPL'
    myHandler.fetchCSTickerData(ticker)