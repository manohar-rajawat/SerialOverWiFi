/*
   Project Name : SerialOverWiFi
   Project Details : ESP microcontrollers can only pring serial data when connected to pc using usb. But serial data is important after deplyoing the sulution.
                     So we will host a websocket server on the esp that will send the serial data to the connected clients if subscribed. Then it will be printed
                     on the browser console.
   Author : Manohar Singh Rajawat
   Date : 5-September-2020
*/

#include <ESP8266WiFi.h> // This library is used for Wifi Setup.
#include <ArduinoOTA.h> //This library is used for Arduino Over The Air update.
#include <ESP8266WebServer.h> //This library is used for hosting http web server. 
#include <WebSocketsServer.h> //This library is used to syncronize all the clients in real time without refreshing the web page.

#include "rootPage.h"

unsigned long previousTime = 0;

ESP8266WebServer server(80); // Web server running on port 80.
WebSocketsServer webSocket = WebSocketsServer(81); //Web socket server running on port 81.

class SerialOverWiFi {
  public:
    void subscribe(uint8_t num) {
      webSocket.subscribe(num);
    }
    void unsubscribe(uint8_t num) {
      webSocket.unsubscribe(num);
    }
    void send(String data) {
      webSocket.broadcastSerialOverWiFi(data); //broadcast the data to every connected client.
    }
};
SerialOverWiFi serialOverWiFi;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:
      serialOverWiFi.send("1 Device Disconnected");
      break;
    case WStype_CONNECTED:
      serialOverWiFi.send("1 Device Connected");
      break;
    case WStype_TEXT:
      if (payload[0] == '#') {
        serialOverWiFi.subscribe(num);
      } else if (payload[0] == '$') {
        serialOverWiFi.unsubscribe(num);
      }
      break;
  }
}

void initializeWifi() {
  const char* ssid = "Home";
  const char* password = "12345670";
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_AP);
  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  boolean result = WiFi.softAP(ssid, password);
  if (result == false)
  {
    delay(5000);
    ESP.restart();
  }
}

void initializeOTA() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    //Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    //Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      //Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      //Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      //Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      //Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      //Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  //Serial.println("Ready");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
}

void handleRoot() {
  server.send(200, "text/html", rootPage);
}

void notFound() {
  server.send(404, "text/html", "<h1><Center>This page is not there !</Center></h1>");
}

void initializeServer() {
  server.on("/", handleRoot);
  server.onNotFound(notFound);
  server.begin();
}


void initializeWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void setup() {
  initializeWifi();
  initializeOTA();
  initializeServer();
  initializeWebSocket();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  webSocket.loop();
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= 1000) {
    previousTime = currentTime;
    serialOverWiFi.send(String(currentTime));
  }
}
