void reconnectMQTT() {
  while (!client.connected()) {
    String clientId = "ESP_Direct_";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      client.subscribe("LED");
      client.subscribe("temperature_threshold");
      client.subscribe("soil_moisture_threshold");
    } else {
      delay(5000);
    }
  }
}
