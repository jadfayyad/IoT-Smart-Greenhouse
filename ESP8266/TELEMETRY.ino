String serialRxBuffer = "";

void handleIncomingSerialTelemetry() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    if (c == '\n') {
      serialRxBuffer.trim();

      // Publish Temperature
      if (serialRxBuffer.startsWith("T:")) {
        String tempVal = serialRxBuffer.substring(2);
        client.publish("temperature", tempVal.c_str());
      }
      // Publish Soil Moisture
      else if (serialRxBuffer.startsWith("S:")) {
        String soilVal = serialRxBuffer.substring(2);
        client.publish("moisture", soilVal.c_str());
      }
      // Sensor Error Forwarding
      else if (serialRxBuffer.equals("FailedT")) {
        client.publish("temperature", "Failed");
      } else if (serialRxBuffer.equals("FailedS")) {
        client.publish("moisture", "Failed");
      }

      serialRxBuffer = "";
    } else if (c != '\r') {
      serialRxBuffer += c;
    }
  }
}
