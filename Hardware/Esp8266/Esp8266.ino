#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>

const char* ssid="Livebox-8FE2";
const char* password="xVJCxFCva3zP5Xtpg6";
//const char* ssid = "Galaxy S10+5909";
//const char* password = "ibdr9540";

SoftwareSerial check(3, 1);
SocketIOclient socketIO;

//#define USE_SERIAL Serial

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            //USE_SERIAL.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            //USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);
            // join default namespace (no auto join in Socket.IO V3)
            socketIO.send(sIOtype_CONNECT, "/");
            break;
        case sIOtype_EVENT:
            //USE_SERIAL.printf("[IOc] get event: %s\n", payload);
            check.write((char*)payload);
            //Serial.println(test);
            delay(2);
            break;
        case sIOtype_ACK:
            //USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            //USE_SERIAL.printf("[IOc] get error: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            //USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            //USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            hexdump(payload, length);
            break;
    }
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  check.begin(19200);
  //Serial.begin(115200);
  //Serial.println();
  //Serial.print("Wifi connecting to ");
  //Serial.println( ssid );

  //Serial.setDebugOutput(true);

  WiFi.begin(ssid, password);
  //Serial.print("Connecting");

  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    //Serial.print(".");
  }
  //socketIO.begin("192.168.196.162", 8080, "/socket.io/?EIO=4");
  socketIO.begin("192.168.1.14", 8080, "/socket.io/?EIO=4");
  delay(2000);
  socketIO.onEvent(socketIOEvent);
}

void loop() {
  socketIO.loop();
}
