import os
import sys
import csv
import json
from dotenv import load_dotenv, dotenv_values
from twelvedata import TDClient

import schwab
from schwab.auth import easy_client
import datetime
import httpx

class RequestInfo:
    def __init__(self):
        self.destination = ''
        self.requestType = ''
        self.ticker = ''

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

    def parseRequest (self, filePath):         
        with open(filePath, 'r') as file:
            data = json.load(file)

        info = RequestInfo()


        info.destination = data['destination'] 
        info.requestType =  data['type'] 
        info.ticker = data['ticker'] 

        match info.requestType:
            case FETCH_HISTORICAL_DATA:
                if (info.destination == 'CS'): 
                    self.fetchCSTickerData(info)
                elif (info.destination == 'TD'): 
                    self.fetchTDTickerData(info)
                
            #case FETCH_ORDER_DATA:

            #case CANCEL_ORDER:

            #case PLACE_ORDER:


    def fetchTDTickerData(self, info):
        tsData = self.tdClient.time_series(
            symbol = info.ticker,
            interval = "1day",
            outputsize = 90,
            timezone = "America/New_York",
            order = "ASC",
            adjust="true"
        )

        csv_data = tsData.as_csv()

        filename = f"data/{info.ticker}_TD_data.csv"
        with open(filename, "w", newline="") as f:
            writer = csv.writer(f)
            writer.writerows(csv_data)
 
    def fetchCSTickerData(self, info):
        endTime = datetime.datetime.now()
        startTime = endTime - datetime.timedelta(days = 90)

        resp = self.csClient.get_price_history_every_day(info.ticker, start_datetime = startTime, end_datetime = endTime, need_extended_hours_data=False)
        assert resp.status_code == httpx.codes.OK

        history = resp.json()

        filename = f"data/{info.ticker}_CS_data.csv"
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
    if len(sys.argv) < 2:
        print("Too few arguments passed.")
        sys.exit(1)
    
    filePath = sys.argv[1]

    myHandler = RequestHandler()
    myHandler.parseRequest(filePath)


if __name__ == "__main__":
    main()