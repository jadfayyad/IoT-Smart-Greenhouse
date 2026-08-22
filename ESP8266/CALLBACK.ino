void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  String strTopic = String(topic);

  // 1. LED Control
  if (strTopic == "LED") {
    if (message == "1") {
      digitalWrite(ledPin, HIGH);
      Serial.println("c");
    } else if (message == "0") {
      digitalWrite(ledPin, LOW);
      Serial.println("d");
    }
  } 
  // 2. Set Temperature Threshold
  else if (strTopic == "temperature_threshold") {
    Serial.print("SET_T:");
    Serial.println(message);
  } 
  // 3. Set Soil Moisture Threshold
  else if (strTopic == "soil_moisture_threshold") {
    Serial.print("SET_S:");
    Serial.println(message);
  }
}
