# TTGO-Crypto-Tracker
Tracks crypto prices from coinbase api
TTGO T-Display ESP32

# How does it work ?

It sends a GET request to coinbase API without any account needed.

You have 2 getPrice() duplicates, these can be used for tracking several coins (like BTC/ETH in example)
The url needs to be changed
```C++
 String url = "https://api.coinbase.com/v2/prices/ETH-EUR/sell";
 String url = "https://api.coinbase.com/v2/prices/BTC-EUR/sell";
 ``` 
Only the tracker has changed but you can change the currency to USD too.

You'll need to adjust the NTP timezone aswell if you're not in central Europe.

# Installation

Watch the following video from 2:30 to install arduino IDE for TTGO T-display
https://youtu.be/b8254--ibmM?t=153

*Following libraries should be installed from Arduino IDE
- TFT_eSPI
- NTPClient
- esp32

# Adjust the timezone:
```C++
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(7200);
  ``` 
  # Change the text Color: 
  
  These are all the possible colors below:
  - tft.setTextColor(TFT_YELLOW, TFT_BLACK); //Yellow text, Black background

This is the indicator of the text to change the colors
  ```C++
  // Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F      
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */
```  


# I have no saying if this will be suported in the future.
It's working in 07/2021
