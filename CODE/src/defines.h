#ifndef DEFINES_H
#define DEFINES_H

#include "SharpGP2Y10.h"
#include <DHT.h>
#include <BH1750.h>
#include <Wire.h>
#include "MQ135.h"
#include <U8g2lib.h>

// DHT11
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN , DHTTYPE);

// Soil
#define SOIL_PIN 35

// SharpGP2Y10
#define v0Pin 34
#define ledPin 25
SharpGP2Y10 dustSensor(v0Pin , ledPin);

// MQ135 
#define PIN_MQ135 26
MQ135 mq135_sensor = MQ135(PIN_MQ135);

// BH1750 && OLED
    // BH1750
#define I2C_SDA 32
#define I2C_SCL 33
BH1750 lightMeter;
    // OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
#define btn_Increase 21
#define btn_Decrease 22
#define btn_Select 23
int modeSelect = 0;


// Relay 
#define temp_relay 12
#define hump_relay 13
#define soil_relay 14
#define light_relay 15
#define dust_relay 16
#define co2_relay 17
#define nh3_relay 18
#define no_relay 19


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


// Wifi & WebSocket
const char* ssid = "Wifi Chua";
const char* password = "7780990204@Ok";



#endif