#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// WiFi Configuration
const char* ssid = "Hadi_Jad";
const char* password = "03528373M";

// HiveMQ SSL Configuration
const char* mqtt_server = "48e3b1d5a4f5449cbfd05672763e2ee3.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "Controller";
const char* mqtt_pass = "1111111111@Controller";

const int ledPin = 12;

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Send initial stop command to ATmega over UART
  delay(100);
  Serial.println("d");

  connectWiFi(); // From WiFi tab

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); // From Callback tab
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT(); // From Reconnect tab
  }
  client.loop();

  handleIncomingSerialTelemetry(); // From Telemetry tab
}
