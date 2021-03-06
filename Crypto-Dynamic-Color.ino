/***************************************************************************************
**                        Sources
***************************************************************************************/
/*
Time getting and displaying:
https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/

HTTPclient
https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/examples/BasicHttpClient/BasicHttpClient.ino

Market
https://documenter.getpostman.com/view/4021156/SVtWvmVu

Coinbase errors
https://developers.coinbase.com/api/v2#error-response
Coinbase endpoints
https://developers.coinbase.com/api/v2#prices
*/
/***************************************************************************************
**                        Help
***************************************************************************************/

//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

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
/***************************************************************************************
**                         Start of program
***************************************************************************************/

#include <NTPClient.h>
// change next line to use with another board/shield
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>
#include "ArduinoJson.h"
#include <HTTPClient.h>

//      --> Screen
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

//      --> Wifi Credentials
const char *ssid     = "SSID";
const char *password = "password";
WiFiUDP ntpUDP;
HTTPClient http;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "3.pool.ntp.org", 3600, 60000);

/***************************************************************************************
**                         Variables
***************************************************************************************/
//crypto tracker
      float ticker;
      float prevprice;
      String price;
      String Symbol;
      
      float ticker2;
      float prevprice2;
      String price2;
      String Symbol2;
      
// Timers
      unsigned long timer;
      unsigned long timepreviouscalc;
      unsigned long timecalctimer = 60000; //1 sec= 1000 - 1Min = 60000
      
      unsigned long timer2;
      unsigned long timepreviouscalc2;
      unsigned long timecalctimer2 = 10000; //1 sec= 1000 - 1Min = 60000

//Week Days
      String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
      String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
      String i;
      String weekDay;
      String currentDate;
/***************************************************************************************
**                         Setup
***************************************************************************************/

void setup(){
  Serial.begin(115200);
// ------------------------------------------------------------
//      --> Screen
// ------------------------------------------------------------

Serial.println("------------------------------------------------------------");
Serial.println("Moxl"); 
Serial.println("Crypto Tracker + Clock");
Serial.println("------------------------------------------------------------");
Serial.println("");
Serial.println("Setup started");
Serial.println("setting up screen");
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize (2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour
  tft.fillScreen(TFT_BLACK);

  
  tft.setTextDatum(MC_DATUM);
  tft.drawString("-- MoxL --", 120, 10);
  tft.drawString("checking wifi", 120, 67);

// ------------------------------------------------------------
//      --> Wifi
// ------------------------------------------------------------
Serial.println("Connecting to WiFi:");
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    tft.print ( "." );
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("");
// ------------------------------------------------------------
//      --> Timeclient offset
// ------------------------------------------------------------

  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(7200);
  
// ------------------------------------------------------------
//      --> Fetch BTC data + display screen
// ------------------------------------------------------------
prevprice = 0;
prevprice2 = 0;
getPrice();
getPrice2();

// ------------------------------------------------------------
//      --> exit setup
// ------------------------------------------------------------

tft.setTextColor(TFT_GREEN, TFT_BLACK); // Note: the new fonts do not draw the background colour
  tft.setTextSize (2);
  tft.fillScreen(TFT_BLACK);
   tft.setTextDatum(MC_DATUM);
  tft.drawString("-- MoxL --", 120, 100);
   tft.setTextDatum(TL_DATUM);
  tft.drawString("Ip: ", 0, 2);
  tft.println(WiFi.localIP());
  delay(2000);
  tft.fillScreen(TFT_BLACK);

  Serial.println("Setup ended, starting loop");
  Serial.println("------------------------------------------------------------");
}
/***************************************************************************************
**                         Loop
***************************************************************************************/
void loop() {


   
      timeClient.update();
      timer = millis();
      timer2 = millis();
    
   //get data only once every 1 minute
      if(timer >= (timepreviouscalc + timecalctimer)== true){
        Serial.print(i);
        Serial.println(" - Starting loop 1");
        
        timepreviouscalc = millis();
        getPrice();
        getPrice2();
        
       }

    //Display Screen every 10 seconds
      if(timer2 >= (timepreviouscalc2 + timecalctimer2)== true){
        Serial.print(i);
        Serial.println(" - Starting loop 2");
        timepreviouscalc2 = millis();
        printScreen();
        
       }
           
   //Get & Print time
      i = timeClient.getFormattedTime();
      
   //Get weekday and print to screen
      
      weekDay = weekDays[timeClient.getDay()];
      
   //Get a date structure
        unsigned long epochTime = timeClient.getEpochTime();
        struct tm *ptm = gmtime ((time_t *)&epochTime); 
        int monthDay = ptm->tm_mday;
        int currentMonth = ptm->tm_mon+1;
        String currentMonthName = months[currentMonth-1];
        //Print complete date
        currentDate = String(monthDay) + " - " + String(currentMonth);
        
// ------------------------------------------------------------
//      --> exit loop
// ------------------------------------------------------------    

}

// ------------------------------------------------------------
//      --> print screen
// ------------------------------------------------------------  

void printScreen(){
      tft.fillScreen(TFT_BLACK);
    //Print Clock
    
      tft.setTextColor(TFT_SILVER, TFT_BLACK);
      tft.setTextFont(4);
      tft.setTextDatum(TC_DATUM);
      tft.drawString(i, 120, 2);
      
    //Get weekday and print to screen
      tft.setTextFont(2);
      tft.drawString(weekDay, 60, 40);
      
    //Print complete date
      tft.drawString(currentDate, 180, 40);
      
    //Print cryptoprice
        if (ticker > prevprice == true){
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
        }else{
          tft.setTextColor(TFT_MAROON, TFT_BLACK);
          }
        String buf = Symbol + ": " +  String(ticker, 2);
        tft.drawString(buf, 120, 75);


        if (ticker2 > prevprice2 == true){
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
        }else{
          tft.setTextColor(TFT_MAROON, TFT_BLACK);
          }
        String buf2 = Symbol2 + ": " + String(ticker2, 2);
        tft.drawString(buf2, 120, 105);
  }

// ------------------------------------------------------------
//      --> fetch crypto 1
// ------------------------------------------------------------  

void getPrice(){
      prevprice = price.toFloat();
      
      String url = "https://api.coinbase.com/v2/prices/BTC-EUR/sell";
      Serial.print("-- Fetching data from: ");
      Serial.print(url);
      Serial.println(" --");
      http.begin(url); //Specify the URL
      int httpCode = http.GET();   
        if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
         Serial.print("Http reply code: ");
        Serial.println(httpCode);
        //Serial.println(payload); // Decomment for full payload, clutters the serial
        Symbol = payload.substring(17,20);  //if 4 long change 20 to 21
        price = payload.substring(49);
        int val=0;
        val = price.indexOf(".");
        price.replace(".", ",");
        val=val + 3;
        price.remove(val, 3);
        ticker = price.toFloat();;
        Serial.println(Symbol + ": " + ticker);
        
      }else {
        Serial.println("Error on HTTP request");
      }
Serial.println("");
http.end();
}
// ------------------------------------------------------------
//      --> fetch crypto 2
// ------------------------------------------------------------  
void getPrice2(){
    prevprice2 = price2.toFloat();
  
    String url = "https://api.coinbase.com/v2/prices/ETH-EUR/sell";
    Serial.print("-- Fetching data from: ");
    Serial.print(url);
    Serial.println(" --");
    http.begin(url); //Specify the URL
    int httpCode = http.GET();   
      if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.print("Http reply code: ");
        Serial.println(httpCode);
        //Serial.println(payload); // Decomment for full payload, clutters the serial
        Symbol2 = payload.substring(17,20);  //if 4 long change 20 to 21
        price2 = payload.substring(49);
        int val=0;
        val = price2.indexOf(".");
        price2.replace(".", ",");
        val=val + 3;
        price2.remove(val, 3);
        ticker2 = price2.toFloat();
        Serial.println(Symbol2 + ": " + ticker2);
        
      }else {
        Serial.println("Error on HTTP request");
      }
http.end();
Serial.println("");
}
