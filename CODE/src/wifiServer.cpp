#include "wifiServer.h"

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
            Serial.println("sensor1: " + sensor1);
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
        sensor1 = String(thresold_temp);
        sensor2 = String(thresold_hump);
        sensor3 = String(thresold_light);
        sensor4 = String(thresold_soil);
        sensor5 = String(thresold_dust);
        sensor6 = String(thresold_CO2);
        sensor7 = String(thresold_NH3);
        sensor8 = String(thresold_NO);
        notifyClients(ResponeData());
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
