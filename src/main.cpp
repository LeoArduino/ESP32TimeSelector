#include <Arduino.h>
/* ESP32 Time selector v1.0
   Created by: Leo Vaseom
   Date: 2023-02-01
    Description: This sketch is used to select the time and Day of the week 
   To be used with ESP32 board 
   This would not have been possible without all the examples from RandomNerdTutorials.com

*/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "time.h"


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");


// Replace with your network credentials
const char* ssid = "Your SSID here";
const char* password = "Your PWD here";





// Set constant variables
const int ledPin1 = 2;  // Output
const long interval = 3000;  // interval at which the time is checked (milliseconds)
const long interval1 = 5000;  // interval to wait for Wi-Fi connection & actual time/date  (milliseconds)
const long interval2 = 2000; // interval for measuring humidity
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;


//Create Strings for JSON File
String message;
String sliderNumber = "0";
String sliderValue1;  
String startTijd1;
String stopTijd1;
String week1;
String Time;

//Create various integers to use during the sketch
int Zo1; int Ma1; int Di1; int Wo1; int Do1; int Vr1; int Za1;
int ledState = LOW;
int chan1 = LOW; 
int sv1;
int x = 0;
int y = 0;

// Parameters for time setting 
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
int today;
int activeDay1= 0; int activeTime1 = 0; int active1 = LOW;int channel1 = LOW;
int channel1Old;
int uren; 
int minuten;
int startTime1;
int stopTime1;
String string1;
int tijdNu;
int uurNu;
int minNu;
int dst;

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Initialize SPIFFS
void initFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
   Serial.println("SPIFFS mounted successfully");
  }
}

//Json Variables to Hold All Values
JSONVar AllValues; // All values from Webpage to ESP32


// Fill day parameters from Week Array
void WeekDay(){
        Serial.println(week1);
      if (week1[1] == '1'){   Zo1 = 1;  } else { Zo1 = 0;}
      if (week1[3] == '1'){   Ma1 = 1;  } else { Ma1 = 0;}
      if (week1[5] == '1'){   Di1 = 1;  } else { Di1 = 0;}
      if (week1[7] == '1'){   Wo1 = 1;  } else { Wo1 = 0;}
      if (week1[9] == '1'){   Do1 = 1;  } else { Do1 = 0;}
      if (week1[11] == '1'){  Vr1 = 1;  } else { Vr1 = 0;}
      if (week1[13] == '1'){  Za1 = 1;  } else { Za1 = 0;}
}


//Get  Values from the HTML page
String getAllValues(){
  AllValues["sliderValue1"] = String(sliderValue1);
  AllValues["startTijd1"] = String(startTijd1);
  AllValues["stopTijd1"] = String(stopTijd1);
  AllValues["week1"] = String(week1);
  AllValues["channel1"] = String(channel1);
  String jsonString = JSON.stringify(AllValues);
  return jsonString;
}


// Fetch the local time from the internal RTC and prints it to serial.
void printLocalTime()
{
  struct tm timeinfo;
int  var = 0;
  while(!getLocalTime(&timeinfo)){

while (var < 5) {
      Serial.println("Failed to obtain Timeinfo.");
      delay(1000);
      var++;
    }
    ESP.restart(); // As Time is essential for the program, restart if not available
      return ;

}

    dst = timeinfo.tm_isdst;
  if ( y ==0){
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  y=1;  
  }

  
struct tm {
   int tm_sec;         /* seconds,  range 0 to 59          */
   int tm_min;         /* minutes, range 0 to 59           */
   int tm_hour;        /* hours, range 0 to 23             */
   int tm_mday;        /* day of the month, range 1 to 31  */
   int tm_mon;         /* month, range 0 to 11             */
   int tm_year;        /* The number of years since 1900   */
   int tm_wday;        /* day of the week, range 0 to 6    */
   int tm_yday;        /* day in the year, range 0 to 365  */
   int tm_isdst;       /* daylight saving time             */
};
//Serial.println(timeinfo.tm_mday);
//Serial.println(timeinfo.tm_year);
  today = timeinfo.tm_wday;
  uurNu = timeinfo.tm_hour;
  minNu = timeinfo.tm_min;
  tijdNu = (uurNu*100+minNu); // tijdNu is the current time as integer to calculate with

}

// Check if date & time match the set criteria
void compareDayTime() {
printLocalTime();

  // Check the days set in the browser
WeekDay();
// Serial output for debugging
//Serial.print("Zo1 = "); Serial.println(Zo1);
//Serial.print("Ma1 = "); Serial.println(Ma1);
//Serial.print("Di1 = "); Serial.println(Di1);
Serial.print("Wo1 = "); Serial.println(Wo1);
//Serial.print("Do1 = "); Serial.println(Do1);
//Serial.print("Vr1 = "); Serial.println(Vr1);
//Serial.print("Za1 = "); Serial.println(Za1);
Serial.print("today = "); Serial.println(today);
Serial.print("tijdNu = "); Serial.println(tijdNu);
Serial.print("startTijd1 = "); Serial.println(startTime1);
Serial.print("stopTijd1 = "); Serial.println(stopTime1);
Serial.print("activeDay1 = "); Serial.println(activeDay1);
Serial.print("sliderValue1 = "); Serial.println(sliderValue1);
Serial.print("channel1 = "); Serial.println(channel1);

  // Check if today is a day that is set to active
 activeDay1 =0;
if (Zo1 == 1 && today == 0) { activeDay1 =1;} 
if (Ma1 == 1 && today == 1) { activeDay1 =1;} 
if (Di1 == 1 && today == 2) { activeDay1 =1;}
if (Wo1 == 1 && today == 3) { activeDay1 =1;}
if (Do1 == 1 && today == 4) { activeDay1 =1;}
if (Vr1 == 1 && today == 5) { activeDay1 =1;}
if (Za1 == 1 && today == 6) { activeDay1 =1;}

          minuten = ((startTijd1.substring(3,5)).toInt());
          uren =((startTijd1.substring(0,2)).toInt() * 100);
          startTime1 = uren+minuten; // startTime1 is the start time as integer to calculate with
           minuten = ((stopTijd1.substring(3,5)).toInt());
           uren =((stopTijd1.substring(0,2)).toInt() * 100);
          stopTime1 = (uren+minuten); // stopTime1 is the stop time as integer to calculate with


 // Check if actual time (tijdNu) is between start and stoptime.(and take care of the day change)

if (startTime1 < stopTime1) { // if start time is smaller than stop time so same day
  if (tijdNu >= startTime1 && tijdNu < stopTime1) {activeTime1 = 1;} 
else if (tijdNu >= stopTime1) {activeTime1 = 0;} 
  else {activeTime1 = 0;}
}
if (startTime1 > stopTime1) { // if start time is bigger than stop time so stoptime is next day
  if (tijdNu >= startTime1 ) {activeTime1 = 1;} 
else if (tijdNu < stopTime1) {activeTime1 = 1;} 
  else if(tijdNu >= stopTime1 && tijdNu < startTime1){activeTime1 = 0;}
}

// Channel output control
  // if the slider is set to 0 the the channel is always off
if (sv1 == 0) {channel1 = 0;}

// if the slider is set to 1 then the channel is on within the set timeperiod and the day and the humidity is below the setpoint
  if (sv1 == 1 && activeTime1 == 1 && activeDay1 == 1){ 
    channel1 = 1; 
  }
  else {channel1 = 0;}

  // if slider is set to ON, then channel is always on
  if (sv1 == 2){ channel1= 1;} 

   // Finaly set the LedPin ON or OFF
      digitalWrite(ledPin1,channel1);

}

// routine to notify the clients of changed HTML values
void notifyClients(String AllValues) {
  ws.textAll(AllValues); // send the HTML values to all connected clients
}

// Handler for incomming Json Messages.
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    message = (char*)data;
    Serial.println("Incomming message is: " + message);
    if (strcmp((char*)data, "getValues") == 0) {
      // If the message is "getValues" then send all values stored in settings to the client.
      notifyClients(getAllValues());
    }

          JSONVar obj =JSON.parse(message);
          sliderNumber = obj["sliderNumber"];
      if (sliderNumber == "1"){sliderValue1 = obj["sliderValue"];}
              sv1 = sliderValue1.toInt();
      if (obj["startTijd1"] > ""){startTijd1 = obj["startTijd1"];}
      if (obj["stopTijd1"] > ""){stopTijd1 = obj["stopTijd1"];}
      if (obj["week1"] > ""){week1 = obj["week1"];}
  //    Serial.print("week1"); Serial.println(week1);
    compareDayTime();
    notifyClients(getAllValues());

    
  }
}



// handler for websocket events
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}


void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  initFS();
  initWiFi();

 initWebSocket();
    // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();

  // Get NTP and set time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
   printLocalTime();
}
  


void loop() {
// compare the day and time every interval (3 second) period
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {

      compareDayTime();
    previousMillis = currentMillis;
    }
    ws.cleanupClients();
// if channel 1 changes then notify the clients
  if (channel1 != channel1Old) {
    notifyClients(getAllValues());
    channel1Old = channel1;
  }
}
