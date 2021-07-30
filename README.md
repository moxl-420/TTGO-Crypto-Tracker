# TTGO-Crypto-Tracker
Tracks crypto prices from coinbase api
TTGO T-Display ESP32

# How does it work ?

It sends a GET request to coinbase API without any account needed.

You have 2 getPrice() duplicates, these can be used for tracking several coins (like BTC/ETH in example)
The url needs to be changed

- String url = "https://api.coinbase.com/v2/prices/ETH-EUR/sell";
- String url = "https://api.coinbase.com/v2/prices/BTC-EUR/sell";
- 
Only the tracker has changed but you can change the currency to USD too.

You'll need to adjust the NTP timezone aswell if you're not in central Europe.

# Installation

Watch the following video from 2:30 to install arduino IDE for TTGO T-display
https://youtu.be/b8254--ibmM?t=153

*Following libraries should be installed from Arduino IDE
TFT_eSPI
NTPClient
esp32


# I have no saying if this will be suported in the future.
It's working in 07/2021
