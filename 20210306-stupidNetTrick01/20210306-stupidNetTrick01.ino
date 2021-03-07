/* Stupid Net Trick Demo
Modified from Tom's Arduino HTTP Client example 
Github repo : https://github.com/tigoe/hue-control/tree/master/ArduinoExamples
06 March 2021
*/

#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

// define wifi id & pass
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// hue hub config
char hueHubIP[] = SECRET_HUEIP;
String hueUser = SECRET_HUEUSER;

int lights[] = {18,16,17,3,21,22};

// wifi Client
int status = WL_IDLE_STATUS;
WiFiClient wifi;

// HTTP client
HttpClient httpClient = HttpClient(wifi,hueHubIP);

boolean state1 = false;
boolean state2 = false;
boolean state3 = false;


void setup() {
Serial.begin(9600);
pinMode(6,INPUT_PULLUP);
pinMode(7,INPUT_PULLUP);
pinMode(8,INPUT_PULLUP);

while(Serial);
connectedToWifi();

//set all light to off
for(int i=0; i<=5; i++){
  sendRequest(lights[i],"on","false");
  }
  
}

void loop() {
int button1 = digitalRead(6);
int button2 = digitalRead(7);
int button3 = digitalRead(8);
boolean newState1 = false;
boolean newState2 = false;
boolean newState3 = false;

//change button state
if(button1 == 0){
   newState1 = !newState1;
  }
if(button2 == 0){
   newState2 = !newState2;
  } 
if(button3 == 0){
   newState3 = !newState3;
  } 

//check which direction the buttons were changed 
if(state1 == false && newState1 == true){
  Serial.println("button1 on"); 
  for(int i=0; i<=1; i++){
  sendRequest(lights[i],"on","true");
  }
  }else if(state1 == true && newState1 == false){
    Serial.println("button1 off"); 
    for(int i=0; i<=1; i++){
  sendRequest(lights[i],"on","false");
  }
  }

if(state2 == false && newState2 == true){
  Serial.println("button2 on"); 
  for(int i=2; i<=3; i++){
  sendRequest(lights[i],"on","true");
  }
  }else if(state2 == true && newState2 == false){
    Serial.println("button2 off"); 
    for(int i=2; i<=3; i++){
  sendRequest(lights[i],"on","false");
  }
  }

 if(state3 == false && newState3 == true){
  Serial.println("button3 on"); 
  for(int i=4; i<=5; i++){
  sendRequest(lights[i],"on","true");
  }
  }else if(state3 == true && newState3 == false){
    Serial.println("button3 off"); 
    for(int i=4; i<=5; i++){
  sendRequest(lights[i],"on","false");
  }
  } 
 //reset state   
 state1 = newState1;
 state2 = newState2;
 state3 = newState3;

}

void connectedToWifi(){
// if arduino don't have a module
  if(WiFi.status() == WL_NO_MODULE){
    Serial.print("Communication with WiFi failed!");
    while(true);
    }
// if wifi not connect >> attempting to connect to the wifi
    while(status != WL_CONNECTED){
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      status = WiFi.begin(ssid,pass);
      delay(5);
      }
      Serial.println("Connected to Wifi");
  }

  void sendRequest(int light, String cmd, String value){
    // set up request string
    String request = "/api/" + hueUser;
    request += "/lights/";
    request += light;
    request += "/state/";

    String contentType = "application/json";

    String hueCmd = "{\""+cmd;
    hueCmd += "\":";
    hueCmd += value;
    hueCmd += ", \"bri\":254 ";
    hueCmd += ", \"transitiontime\":0 ";
    hueCmd += "}";

    Serial.print("PUT request to server: ");
    Serial.println(request);

    Serial.print("JSON command to server: ");
    Serial.println(hueCmd);

    httpClient.put(request,contentType,hueCmd); // put request

    // checking response 
    int statusCode = httpClient.responseStatusCode(); //200 = success
    String response = httpClient.responseBody();

    Serial.print("Status from server:");
    Serial.println(statusCode);

    Serial.print("Sever response");
    Serial.println(response);
    Serial.println();
    }
