#ifndef DEFINES_H
#define DEFINES_H

#include "SharpGP2Y10.h"
#include <DHT.h>
#include <BH1750.h>
#include <Wire.h>
#include "MQ135.h"
#include "U8glib.h"

// DHT11
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN , DHTTYPE);

// Soil
#define SOIL_PIN 34

// SharpGP2Y10
#define v0Pin 34
#define ledPin 25
SharpGP2Y10 dustSensor(v0Pin , ledPin);

// MQ135 
#define PIN_MQ135 14
MQ135 mq135_sensor = MQ135(PIN_MQ135);

// BH1750 && OLED
    // BH1750
#define I2C_SDA 21
#define I2C_SCL 22
BH1750 lightMeter;
    // OLED
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#define btn_Increase 12
#define btn_Decrease 13
#define btn_Select 10
int modeSelect = 0;


// Sensor virtual
float t = 0;
float h = 0;
float l = 0;
float dust = 0;
float soil = 0;
float CO2 = 0;
float NH3 = 0;
float NO = 0;

// Time
unsigned long lastSend = 0;
unsigned long send_Interval = 2000;

// Thresold 
float thresold_temp = 35;
float thresold_hump = 80 ;
float thresold_soil = 50 ;
float thresold_light = 500;
float thresold_dust = 80;
float thresold_CO2 = 50;
float thresold_NH3 = 50;
float thresold_NO = 50;

// Relay 
#define temp_relay 8
#define hump_relay 9
#define soil_relay 7
#define light_relay 6
#define dust_relay 3
#define co2_relay 2
#define nh3_relay 4
#define no_relay 5

// Wifi & WebSocket
const char* ssid = "Wifi Chua";
const char* password = "7780990204@Ok";



#endif