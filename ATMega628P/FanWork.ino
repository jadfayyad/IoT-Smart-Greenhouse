#define FAN_PIN 5  // Pin connected to MOSFET/Relay for fan

void initFan() {
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
}

// Turns fan ON if either Temperature OR Humidity exceeds its threshold
void checkFanControl(float currentTemp, float tempThreshold, float currentHum, float humThreshold) {
  // Turn OFF fan if sensor reading failed
  if (currentTemp == -999.0 || currentHum == -999.0) {
    digitalWrite(FAN_PIN, LOW);
    return;
  }

  // Trigger fan if temperature IS TOO HIGH or humidity IS TOO HIGH
  if (currentTemp > tempThreshold || currentHum > humThreshold) {
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, LOW);
  }
}
