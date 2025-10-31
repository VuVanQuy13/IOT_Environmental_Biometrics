#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

 
extern float thresold_temp;
extern float thresold_hump ;
extern float thresold_soil ;
extern float thresold_light;
extern float thresold_dust;
extern float thresold_CO2;
extern float thresold_NH3;
extern float thresold_NO;

extern AsyncWebServer server;
extern AsyncWebSocket ws;

void initWifi(const char* ssid , const char* password);
void initSPIFFS();
void initWebServer();

#endif