import os
import sys
import csv
from dotenv import load_dotenv, dotenv_values
from twelvedata import TDClient

def fetchAndStoreCSV(symbol, client):
    tsData = client.time_series(
        symbol = symbol,
        interval = "1day",
        outputsize = 90,
        timezone = "America/New_York"
    )

    csv_data = tsData.as_csv()

    filename = f"data/{symbol}_data.csv"
    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerows(csv_data)

def main():
    if len(sys.argv) < 2:
        print("No arguments passed.")
        sys.exit(1)
    
    ticker = sys.argv[1].upper()

    load_dotenv("secrets.env")
    apikey = os.getenv("TWELVE_DATA_KEY")
    client = TDClient(apikey)

    fetchAndStoreCSV(ticker, client)

if __name__ == "__main__":
    main()