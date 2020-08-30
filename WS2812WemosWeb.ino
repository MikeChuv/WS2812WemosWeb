/*
  Controller for WS2812 led strip based on Wemos D1 mini
  - Web interface
  - Websockets
  - Various effects (15) with color, speed and scale adjustment

  Based on:
  https://github.com/Links2004/arduinoWebSockets
  https://github.com/Links2004/arduinoWebSockets/tree/master/examples/esp8266/WebSocketServerAllFunctionsDemo
  https://github.com/AlexGyver/WS2812_FX

  Made by:
  Mike Chuv (https://github.com/MikeChuv)

*/

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include "index.h"

// amount of leds in a strip
#define LED_COUNT 144
// strip pin (4 - D2 on Wemos D1 mini)
#define LED_DT 4

// your wifi ssids and passwords
const char* ssid1 = "ssid";
const char* password1 = "password";
// optional
// const char* ssid2 = "ssid2";
// const char* password2 = "password2";

struct CRGB leds[LED_COUNT];

ESP8266WiFiMulti WiFiMulti;

// create servers on 80 and 81 ports
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

String currentMode;

// variables for led effects
uint16_t rainbowIndex = 0;  // rainbowCycle, theaterChaseRainbow
int runningPosition = 0;    // sin_bright_wave
float runPosition = 0.0;    // RunningLights
int ihue = 0;               // new_rainbow_loop, rainbow_loop, random_burst, random_color_pop, rainbow_fade
int idex = 0;               // a lot of
int ibright = 0;            // pulse_one_color_all, sin_bright_wave
int bouncedirection = 0;    // color_bounceFADE, pulse_one_color_all
float tcount = 0.0;         // sin_bright_wave

int BOTTOM_INDEX = 0;
int TOP_INDEX = int(LED_COUNT / 2);

// HSV variables to be converted from RGB
volatile int cHue = 0;
volatile int cSat = 255;
volatile int cVal = 255;

// RGB colors array
// values getting from socket
int rgbColors[3] = {255, 0, 0};

// values from sliders
volatile int globalDelay = 10;
volatile int globalScale = 1;

// if you want to print info about connected sockets
// see more in commented section in loop()
// unsigned long last_10sec = 0;
// unsigned int counter = 0;


void setup() {
    Serial.begin(74880);
    LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);
    one_color_all(0, 0, 0); // switch off leds
    Serial.println();
    delay(10);
    // add Access Points
    WiFiMulti.addAP(ssid1, password1);
	
    // WiFiMulti.addAP(ssid2, password2); // optional

    
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    IPAddress espIP = WiFi.localIP();
    Serial.print("Got IP: ");
    Serial.println(espIP);

    // if you want to show your local ip on the strip (seconds):
    // showIP(espIP, 5);


    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // try connecting http://esp8266.local
    // if there is an error, check mdns support on your client device
    // (for Windows try this: https://superuser.com/questions/1330027/how-to-enable-mdns-on-windows-10-build-17134)
    // should work fine on Apple devices by default
    if(MDNS.begin("esp8266", WiFi.localIP())) {
        Serial.println("MDNS responder started");
    }
    else{
      Serial.println("Error setting up MDNS responder!");
    }

    // MAIN_page is in index.h as const char*
    server.on("/", []() {
      server.send(200, "text/html", MAIN_page);
    });

    server.begin();

    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

    // if you want to print size of MAIN_page
    // Serial.print("MAIN_page size(symbols):"); Serial.println(strlen_P(MAIN_page));
    // Serial.print("MAIN_page size(bytes):"); Serial.println(sizeof(MAIN_page));
}




void loop() {

    webSocket.loop();
    server.handleClient();

    // if you want to print info about connected sockets
    // unsigned long t = millis();
    // if((t - last_10sec) > 10 * 1000) {
    //     counter++;
    //     bool ping = (counter % 2);
    //     int i = webSocket.connectedClients(ping);
    //     Serial.printf("%d Connected websocket clients ping: %d\n", i, ping);
    //     last_10sec = millis();
    // }

    // calling fx function due to currentMode
    if(currentMode == "running"){
      float runningScale = map_float((float)globalScale, 1.0, 100.0, 0.0, TWO_PI);
      runPosition += runningScale;
      RunningLights(rgbColors[0], rgbColors[1], rgbColors[2], globalDelay, runPosition);
    }
    if(currentMode == "rainbowcycle"){
      if (rainbowIndex >= 256 * 5){
        rainbowIndex = 0;
      }
      rainbowCycle(globalDelay, rainbowIndex++);
    }
    if(currentMode == "fire"){
      int fireCooling = map(globalScale, 1, 100, 60, 5);
      int fireSparks = map(globalScale, 1, 100, 20, 200);
      Fire(fireCooling, fireSparks, globalDelay);
    }
    if(currentMode == "rainbowloop"){
      new_rainbow_loop(globalDelay);
    }
    if(currentMode == "rainbowloop2"){
      rainbow_loop(globalDelay);
    }
    if(currentMode == "randomburst"){
      random_burst(globalDelay);
    }
    if(currentMode == "randomcolorpop"){
      random_color_pop(globalDelay);
    }
    if(currentMode == "rainbowfade"){
      rainbow_fade(globalDelay);
    }
    if(currentMode == "rgbpropeller"){
      rgb_propeller(globalDelay);
    }
    if(currentMode == "colorbouncefade"){
      color_bounceFADE(globalDelay);
    }
    if(currentMode == "emslights"){
      ems_lightsALL(globalDelay);
    }
    if(currentMode == "pulse"){
      pulse_one_color_all(globalDelay);
    }
    if(currentMode == "sinbrightwave"){
      if (runningPosition >= LED_COUNT){
        runningPosition = 0;
      }
      float sinWaveScale = map_float((float)globalDelay, 1.0, 100.0, 0.05, 2.0);
      sin_bright_wave(globalDelay, runningPosition++, sinWaveScale);
    }
    if(currentMode == "chaserainbow"){
      if (rainbowIndex >= 256){
        rainbowIndex = 0;
      }
      theaterChaseRainbow(globalDelay, rainbowIndex++);
    }
    if(currentMode == "onecolorall"){
      one_color_all(rgbColors[0], rgbColors[1], rgbColors[2]);
    }
}
