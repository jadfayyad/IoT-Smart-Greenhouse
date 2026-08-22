#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// WiFi Configuration
const char* ssid = "WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// HiveMQ SSL Configuration
const char* mqtt_server = "example.s1.eu.hivemq.cloud";
const int mqtt_port = MQTT_port_number;
const char* mqtt_user = "MQTT_USER_USENAME";
const char* mqtt_pass = "MQTT_USER_PASSWORD";

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
