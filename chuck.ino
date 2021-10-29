#include <M5Core2.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "noris.h"
#include "chuck.h"

#define GREY 0x5AEB
RTC_TimeTypeDef RTCtime;
RTC_TimeTypeDef RTCtime_Now;
RTC_DateTypeDef RTC_DateStruct;
RTC_DateTypeDef RTCdate_Now;
char timeStrbuff[64];

String months[12]={"January","Febrary","March","April","May","June","July","August","September","October","November","December"};
String days[7]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
 
//Provide your own WiFi credentials
const char* ssid = "xxxx";
const char* password = "xxxxx";

String response = "";
DynamicJsonDocument doc(2048);
 
int line=0;
int frame=0;
int jokes=1;

void setup(void) {
  
  M5.begin();
  M5.Lcd.setSwapBytes(true); 
   //set time
   RTCtime.Hours = 8;
   RTCtime.Minutes = 10;
   RTCtime.Seconds = 0;
   //M5.Rtc.SetTime(&RTCtime); //upload only once

    RTC_DateStruct.WeekDay = 2;
    RTC_DateStruct.Month   = 9;
    RTC_DateStruct.Date    = 21;
    RTC_DateStruct.Year    = 2021;
    //M5.Rtc.SetDate(&RTC_DateStruct);//upload only once
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(200);

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,10,2);
    M5.Lcd.println("connecting");
 
  // Wait for connection
     while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     M5.Lcd.print(".");}    
  
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0,6,2);
  M5.Lcd.setTextColor(0x0775);
  M5.Lcd.println("Random Joke");

   M5.Lcd.setCursor(156,118,1);
   M5.Lcd.setTextColor(0xFE1F);
   M5.Lcd.println("Cause the eyes of the");

  //Cause the eyes of the Ranger are upon you…

  M5.Lcd.fillRect(90,14,230,3,0x04D4);
  M5.Lcd.fillRect(0,112,320,1,0x04D4);

  M5.Lcd.setTextColor(0x0775);
  M5.Lcd.setCursor(106,122,2);
  M5.Lcd.println("DATE");

  M5.Lcd.setTextColor(WHITE,BLACK);

  M5.Lcd.fillRect(0,120,90,28,0x04D4);
  M5.Lcd.fillRect(0,154,90,28,0x700D);
  M5.Lcd.fillRect(96,142,52,40,0xE420);
}

void getJoke()
  {
    HTTPClient http;
    String request = "https://api.chucknorris.io/jokes/random";
    http.begin(request);
    http.GET();
    response = http.getString();
    DeserializationError error = deserializeJson(doc, response);
    if(error) {
     return; }
 
  M5.Lcd.setCursor(2,24,2);
  String r=String(doc["value"].as<char*>());
  
    if(r.length()<210){
    M5.Lcd.fillRect(0,26,340,84,BLACK);
  
    M5.Lcd.print(r); 
    jokes++;
    M5.Lcd.setTextColor(WHITE,PURPLE);
    M5.Lcd.setCursor(216,100,1);
  M5.Lcd.print(" JOKES SHOWN: "+String(jokes)+" ");
  M5.Lcd.setTextColor(WHITE,BLACK);
  }
  }
 
void loop(void) {

  M5.Lcd.setCursor(0,140,7);
  M5.Rtc.GetTime(&RTCtime_Now);
  M5.Rtc.GetDate(&RTCdate_Now);
  sprintf(timeStrbuff,"RTC Time Now is %02d:%02d:%02d",
         RTCtime_Now.Hours,RTCtime_Now.Minutes,RTCtime_Now.Seconds);
  M5.Lcd.drawString(String(RTCtime_Now.Hours)+":"+String(RTCtime_Now.Minutes),0,190,7);

  M5.Lcd.setTextColor(WHITE,0xE420);
  M5.Lcd.setCursor(108,150,4);
 
  M5.Lcd.print(RTCdate_Now.Date);

  M5.Lcd.setTextColor(WHITE,0x04D4);
  M5.Lcd.setCursor(10,126,2);
  M5.Lcd.print(days[RTCdate_Now.WeekDay-1]);

  M5.Lcd.setTextColor(WHITE,0x700D);
  M5.Lcd.setCursor(10,160,2);
  M5.Lcd.print(months[RTCdate_Now.Month-1]);

 
  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.fillRect(0,0,line,3,RED);
  if(line==0)
  getJoke();
  
  line++;
  frame++;

  if(frame>105)
  frame=0;

  if(frame<19)
  M5.Lcd.pushImage(156,130,160,108,chuck[frame]);
  
  if(line>340)
  { line=0;
  M5.Lcd.fillRect(0,0,340,3,BLACK);}
  
  delay(18);
}
