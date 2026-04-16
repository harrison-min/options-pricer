import os
import sys
import csv
from dotenv import load_dotenv, dotenv_values
from twelvedata import TDClient

import schwab
from schwab.auth import easy_client
import datetime
import httpx

def fetchTDTickerData(symbol):
    load_dotenv("secrets.env")
    apikey = os.getenv("TWELVE_DATA_KEY")
    client = TDClient(apikey)

    tsData = client.time_series(
        symbol = symbol,
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
 
def fetchCSTickerData(ticker):
    load_dotenv("secrets.env")

    apiSecret = os.getenv("CHARLES_SCHWAB_SECRET")
    apiClientKey = os.getenv("CHARLES_SCHWAB_CLIENT_ID")
    c = easy_client(
        api_key=apiClientKey,
        app_secret=apiSecret,
        callback_url='https://127.0.0.1:8182',
        token_path='token/token.json')

    endTime = datetime.datetime.now()
    startTime = endTime - datetime.timedelta(days = 90)

    resp = c.get_price_history_every_day('AAPL', start_datetime = startTime, end_datetime = endTime, need_extended_hours_data=False)
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



def main():
    if len(sys.argv) < 3:
        print("Too few arguments passed.")
        sys.exit(1)
    
    ticker = sys.argv[1].upper()
    source = sys.argv[2]

    if (source == 'CS'): 
        fetchCSTickerData(ticker)
    elif (source == 'TD'):
        fetchTDTickerData(ticker)

if __name__ == "__main__":
    main()