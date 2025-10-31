#include <Arduino.h>
#include "wifiServer.h"
#include "defines.h"

void readSensor() {
  t = dht.readTemperature();
  h = dht.readHumidity();
  soil = map(analogRead(SOIL_PIN) , 0 , 1023 , 0 , 100);
  l = lightMeter.readLightLevel();
  dust = dustSensor.readDustDensity();
  CO2 = mq135_sensor.getPPM();
  NH3 = CO2 * 0.7;
  NO = CO2 * 0.3;
}

void sendSensorWeb() {

  JSONVar data;
  data["temperature"] = t;
  data["humidity"] = h;
  data["soil"] = soil;
  data["light"] = l;
  data["dust"] = dust;
  data["CO2"] = CO2;
  data["NH3"] = NH3;
  data["NO"] = NO;
  String payload = JSON.stringify(data);

  Serial.println("du lieu gui len Web:");
  ws.textAll(payload);
  Serial.println(payload);
  ws.cleanupClients();
}

void printOLED() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x13);
    
    switch (modeSelect)
    {
      case 0:
        u8g.drawStr(0,10, "Temperature(C): ");
        u8g.setPrintPos(0,30);
        u8g.print(t  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_temp ,2);
        break;
        
      case 1:
        u8g.drawStr(0,10, "Humidity(%): ");
        u8g.setPrintPos(0,30);
        u8g.print(h  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_hump ,2);
        break;
        
      case 2:
        u8g.drawStr(0,10, "soil(%): ");
        u8g.setPrintPos(0,30);
        u8g.print(soil  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_soil ,2);
        break;
        
      case 3:
        u8g.drawStr(0,10, "light(lux): ");
        u8g.setPrintPos(0,30);
        u8g.print(l  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_light ,2);
        break;
        
      case 4:
        u8g.drawStr(0,10, "dust(ug/m3): ");
        u8g.setPrintPos(0,30);
        u8g.print(dust  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_dust ,2);
        break;
        
      case 5:
        u8g.drawStr(0,10, "CO2(ppm): ");
        u8g.setPrintPos(0,30);
        u8g.print(CO2  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_CO2 ,2);
        break;
        
      case 6:
        u8g.drawStr(0,10, "NH3 (ppm): ");
        u8g.setPrintPos(0,30);
        u8g.print(NH3  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_NH3 ,2);
        break;
        
      case 7:
        u8g.drawStr(0,10, "NO (ppm): ");
        u8g.setPrintPos(0,30);
        u8g.print(NO  , 2);
        u8g.drawStr(0 , 50 , "Thresold: " );
        u8g.setPrintPos(60 , 50);
        u8g.print(thresold_NO ,2);
        break;
      
      default:
        break;
    }
  } while (u8g.nextPage());
}

void increase_BTN() {
  switch (modeSelect)
  {
    case 0: thresold_temp +=1; 
    break;

    case 1: thresold_hump +=1; 
    break;
    
    case 2: thresold_light +=10; 
    break;
    
    case 3: thresold_soil +=1; 
    break;
  
    case 4: thresold_dust +=5; 
    break;
    
    case 5: thresold_CO2 +=10; 
    break;
    
    case 6: thresold_NH3 +=2; 
    break;
    
    case 7: thresold_NO +=2; 
    break;
  
  default:
    break;
  }
}

void decrease_BTN() {

  switch (modeSelect)
  {
    case 0: thresold_temp -=1; 
    break;

    case 1: thresold_hump -=1; 
    break;
    
    case 2: thresold_light -=10; 
    break;
    
    case 3: thresold_soil -=1; 
    break;
  
    case 4: thresold_dust -=5; 
    break;
    
    case 5: thresold_CO2 -=10; 
    break;
    
    case 6: thresold_NH3 -=2; 
    break;
    
    case 7: thresold_NO -=2; 
    break;
  
  default:
    break;
  } 
}

// Time Debounce
#define TIME_DEBOUNCE 50
bool debounce(int indexBTN , int pin ) {
  unsigned long timeBTN[2] = {0};
  unsigned long currentTime = millis();

  if(digitalRead(pin) == LOW && currentTime - timeBTN[indexBTN] > TIME_DEBOUNCE ) {
    timeBTN[indexBTN] = currentTime;
    return true;
  }

  return false;
}

void setup() {
  // Begin
  Serial.begin(9600);
  dht.begin();
  dustSensor.begin();
  Wire.begin(I2C_SDA , I2C_SCL);
  lightMeter.begin();
  delay(1000);

  // Relay
  pinMode(temp_relay , OUTPUT);
  pinMode(hump_relay , OUTPUT);
  pinMode(soil_relay , OUTPUT);
  pinMode(light_relay , OUTPUT);
  pinMode(dust_relay , OUTPUT);
  pinMode(co2_relay , OUTPUT);
  pinMode(nh3_relay , OUTPUT);
  pinMode(no_relay , OUTPUT);

  // BTN
  pinMode(btn_Increase , INPUT_PULLUP);
  pinMode(btn_Decrease , INPUT_PULLUP);
  pinMode(btn_Select , INPUT_PULLUP);
  
  // Wifi & WebServer
  initSPIFFS();
  initWifi(ssid , password);
  initWebServer();

}

void loop() {
  unsigned long now = millis();

  if(now - lastSend >= send_Interval) {
    lastSend = now;
    readSensor();
    sendSensorWeb();
  }
  
  if(debounce(0 , btn_Select)) {
    delay(150);
    modeSelect++;
    if(modeSelect > 7) modeSelect = 0 ;
  }

  if(debounce(1 , btn_Increase)) {
    delay(150);
    increase_BTN();
  }

  if(debounce(2 ,btn_Decrease )) {
    delay(150);
    decrease_BTN();
  }

  printOLED();
}