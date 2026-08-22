const int DRY_VALUE = 1000;   // Calibration in air
const int WET_VALUE = 380;    // Calibration in water
const int SOIL_PIN = A2;      // Analog pin connected to soil sensor

// Reads, transmits over Serial, and returns Soil Moisture percentage (-1 on failure)
int getAndSendSoilMoisture() {
  int rawValue = analogRead(SOIL_PIN);
  
  if (rawValue <= 0 || rawValue >= 1023) {
    Serial.println("FailedS");
    return -1;
  }

  int moisturePercent = map(rawValue, DRY_VALUE, WET_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("S:");
  Serial.println(moisturePercent);

  return moisturePercent;
}
