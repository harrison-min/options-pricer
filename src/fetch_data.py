import os
import sys
import csv
from dotenv import load_dotenv, dotenv_values
from twelvedata import TDClient
import schwab
from schwab.auth import easy_client

# def fetchAndStoreCSV(symbol, client):
#     tsData = client.time_series(
#         symbol = symbol,
#         interval = "1day",
#         outputsize = 90,
#         timezone = "America/New_York",
#         order = "ASC",
#         adjust="true"
#     )
# 
#     csv_data = tsData.as_csv()
# 
#     filename = f"data/{symbol}_data.csv"
#     with open(filename, "w", newline="") as f:
#         writer = csv.writer(f)
#         writer.writerows(csv_data)
# 
# 
# def main():
#     if len(sys.argv) < 2:
#         print("No arguments passed.")
#         sys.exit(1)
#     
#     ticker = sys.argv[1].upper()
# 
#     load_dotenv("secrets.env")
#     apikey = os.getenv("TWELVE_DATA_KEY")
#     client = TDClient(apikey)
# 
#     fetchAndStoreCSV(ticker, client)
# 
def testSchwab():
    ticker = "AAPL"

    load_dotenv("secrets.env")

    apiSecret = os.getenv("CHARLES_SCHWAB_SECRET")
    apiClientKey = os.getenv("CHARLES_SCHWAB_CLIENT_ID")
    c = easy_client(
        api_key=apiClientKey,
        app_secret=apiSecret,
        callback_url='https://127.0.0.1:8182',
        token_path='/tmp/token.json')

    resp = c.get_price_history_every_day('AAPL')
    assert resp.status_code == httpx.codes.OK
    history = resp.json()

if __name__ == "__main__":
    testSchwab()
