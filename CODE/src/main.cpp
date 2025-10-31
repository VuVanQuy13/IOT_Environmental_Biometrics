  #include <Arduino.h>
  #include "defines.h"
  #include <WiFi.h>
  #include <SPIFFS.h>
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <Arduino_JSON.h>


  // WIFI && WEBSERVER
  AsyncWebServer server(80);
  AsyncWebSocket ws("/ws");

  JSONVar responeData;
  String message = "";
  String sensor1 = "0";
  String sensor2 = "0";
  String sensor3 = "0";
  String sensor4 = "0";
  String sensor5 = "0";
  String sensor6 = "0";
  String sensor7 = "0";
  String sensor8 = "0";

  void initWifi(const char* ssid , const char* password) {
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid , password);
      Serial.println("Connecting to Wifi ...");
      while (WiFi.status() != WL_CONNECTED)
      {
          delay(500);
          Serial.print(".");
      }
      Serial.print("Wifi connected: ");
      Serial.println(WiFi.localIP());
  }

  void initSPIFFS() {
      if( !SPIFFS.begin() ) {
          Serial.println("Error init SPIFFS!");
          return;
      }
      Serial.println("SPIFFS already");
  }

  String ResponeData() {
      responeData["sensor1"] = String(sensor1);
      responeData["sensor2"] = String(sensor2);
      responeData["sensor3"] = String(sensor3);
      responeData["sensor4"] = String(sensor4);
      responeData["sensor5"] = String(sensor5);
      responeData["sensor6"] = String(sensor6);
      responeData["sensor7"] = String(sensor7);
      responeData["sensor8"] = String(sensor8);

      
      String jsonString = JSON.stringify(responeData);
      return jsonString;
  }

  void notifyClients(String respone) {
      ws.textAll(respone);
  }

  void handleData(void * arg, uint8_t *data, size_t len) {
      AwsFrameInfo *info = (AwsFrameInfo*) arg;

      if(info->final && info->index == 0 && info->len && info->opcode == WS_TEXT) {
          char msg[len+1];
          memcpy(msg , data , len);
          msg[len] = '\0';
          message = String(msg);

          if (message == "getValueFirts") {
              Serial.printf("The first value: %s\n", ResponeData().c_str());
              notifyClients(ResponeData());
          }


          // Temp
          if(message.indexOf("1s") >=0 ) {
              sensor1 = message.substring(2);
              thresold_temp = sensor1.toFloat();
              Serial.printf("Sensor1 nhan duoc: %.2f" , thresold_temp);
              Serial.println(ResponeData());
              notifyClients(ResponeData());
          }

          // Hum
          if(message.indexOf("2s") >=0 ) {
              sensor2 = message.substring(2);
              thresold_hump = sensor2.toFloat();
              Serial.println("sensor2: " + sensor2);
              notifyClients(ResponeData());
          }

          // Soil
          if(message.indexOf("3s") >=0 ) {
              sensor3 = message.substring(2);
              thresold_soil = sensor3.toFloat();
              Serial.println("sensor3: " + sensor3);
              notifyClients(ResponeData());
          }

          // light
          if(message.indexOf("4s") >=0 ) {
              sensor4 = message.substring(2);
              thresold_light = sensor4.toFloat();
              Serial.println("sensor4: " + sensor4);
              notifyClients(ResponeData());
          }

          // Dust
          if(message.indexOf("5s") >=0 ) {
              sensor5 = message.substring(2);
              thresold_dust = sensor5.toFloat();
              Serial.println("sensor5: " + sensor5);
              notifyClients(ResponeData());
          }

          // CO2
          if(message.indexOf("6s") >=0 ) {
              sensor6 = message.substring(2);
              thresold_CO2 = sensor6.toFloat();
              Serial.println("sensor6: " + sensor6);
              notifyClients(ResponeData());
          }

          // NH3
          if(message.indexOf("7s") >=0 ) {
              sensor7 = message.substring(2);
              thresold_NH3 = sensor7.toFloat();
              Serial.println("sensor7: " + sensor7);
              notifyClients(ResponeData());
          }
          
          // NO
          if(message.indexOf("8s") >=0 ) {
              sensor8 = message.substring(2);
              thresold_NO = sensor8.toFloat();
              Serial.println("sensor8: " + sensor8);
              notifyClients(ResponeData());
          }  
      }
  }

  void event(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
      switch (type)
      {
      case WS_EVT_CONNECT:
          Serial.printf("Websocket client #%u connected %s" , client->id() , client->remoteIP().toString().c_str());
          break;

      case WS_EVT_DISCONNECT:
          Serial.printf("WebSocket client #%u disconnected" , client->id());
          break;

      case WS_EVT_DATA:
          handleData(arg, data, len);
          break;

      default:
          break;
      }
  }

  void initWebServer() {
      
      ws.onEvent(event);
      server.addHandler(&ws);

      server.on("/" , HTTP_GET , [](AsyncWebServerRequest *request) {
          if(SPIFFS.exists("/index.html")) {
              request->send(SPIFFS , "/index.html" , "text/html");
          } else {
              request->send(200 , "text/html" , "<h1> No have index.htm </h1>");
          }
      });
      server.serveStatic("/" , SPIFFS , "/");
      server.begin();
      Serial.println("WebServer Started ");
  }






  // OLED
  void readSensor() {
    t = dht.readTemperature();
    h = dht.readHumidity();
    soil = map(analogRead(SOIL_PIN) , 0 , 4095 , 0 , 100);
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
  }

  void printOLED() {
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_6x13);
      
      switch (modeSelect)
      {
        case 0:
          u8g.drawStr(0,10, "Temperature(C): ");
          u8g.setCursor(0,30);
          u8g.print(t  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
          u8g.print(thresold_temp ,2);
          break;
          
        case 1:
          u8g.drawStr(0,10, "Humidity(%): ");
          u8g.setCursor(0,30);
          u8g.print(h  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
          u8g.print(thresold_hump ,2);
          break;
          
        case 2:
          u8g.drawStr(0,10, "soil(%): ");
          u8g.setCursor(0,30);
          u8g.print(soil  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
          u8g.print(thresold_soil ,2);
          break;
          
        case 3:
          u8g.drawStr(0,10, "light(lux): ");
          u8g.setCursor(0,30);
          u8g.print(l  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
          u8g.print(thresold_light ,2);
          break;
          
        case 4:
          u8g.drawStr(0,10, "dust(ug/m3): ");
          u8g.setCursor(0,30);
          u8g.print(dust  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
          u8g.print(thresold_dust ,2);
          break;
          
        case 5:
          u8g.drawStr(0,10, "CO2(ppm): ");
          u8g.setCursor(0,30);
          u8g.print(CO2  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
          u8g.print(thresold_CO2 ,2);
          break;
          
        case 6:
          u8g.drawStr(0,10, "NH3 (ppm): ");
          u8g.setCursor(0,30);
          u8g.print(NH3  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
          u8g.print(thresold_NH3 ,2);
          break;
          
        case 7:
          u8g.drawStr(0,10, "NO (ppm): ");
          u8g.setCursor(0,30);
          u8g.print(NO  , 2);
          u8g.drawStr(0 , 50 , "Thresold: " );
          u8g.setCursor(60 , 50);
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
    static unsigned long timeBTN[3] = {0};
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
    Serial.println("ESP32 setup Running!!");
    Wire.begin(I2C_SDA , I2C_SCL);

    u8g.begin();

    Wire.beginTransmission(0x3C);
    int oledError = Wire.endTransmission();
    Serial.print("OLED I2C Error: ");
    Serial.println(oledError);

    Wire.beginTransmission(0x23);
    int bhError  = Wire.endTransmission();
    Serial.print("BH1750 I2C Error: ");
    Serial.println(bhError);

    // Sensor
    dht.begin();
    dustSensor.begin();
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
    ws.cleanupClients();
    
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