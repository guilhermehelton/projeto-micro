#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Wire.h> 
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
const char* ssid     = "******";
const char* password = "******";

WiFiServer server(80);

#include <WebSocketsClient.h>

WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

LiquidCrystal_I2C LCD(0x27, 16, 2);


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Desconectado!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Conectado na url: %s\n", payload);

      webSocket.sendTXT("Conectado");
      break;
    case WStype_TEXT:
      String cpuPercent = "CPU: ";
      String ramPercent = "RAM: ";

      char *token;
      char *payloadCopy = strdup((char*)payload);
      token = strtok(payloadCopy, ";");

      if (token != NULL) {
        cpuPercent.concat(String(token));
      }

      token = strtok(NULL, ";");

      if (token != NULL) {
        ramPercent.concat(String(token));
      }

      cpuPercent.concat("%");
      ramPercent.concat("%");

      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print(cpuPercent.c_str());

      LCD.setCursor(0, 1);
      LCD.print(ramPercent.c_str());

      LCD.setCursor(10, 0);
      LCD.print("(AVG)");

      free(payloadCopy);

      break;
  }

}

void setup() {
  Serial.begin(115200);

  Serial.setDebugOutput(true);

  LCD.init();
  LCD.backlight();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  LCD.setCursor(0, 0);
  LCD.print("WiFi conectado");
  LCD.setCursor(0, 1);
  LCD.print(WiFi.localIP());  

  webSocket.begin("Guilherme", 8765, "/");

  webSocket.onEvent(webSocketEvent);

  webSocket.setReconnectInterval(5000);
}

void loop() {
  
  webSocket.loop();

  if (Serial.available())
  {
    char teste = char(Serial.read());
    Serial.println(teste);
    webSocket.sendTXT(teste);
  }

}
