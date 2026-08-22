// Default Thresholds
float tempThreshold = 30.0;
float humThreshold  = 70.0;
int soilThreshold   = 20; // Default soil threshold: 50%

// Non-blocking timer for reading sensors and updating outputs
unsigned long lastSensorSend = 0;
const unsigned long SENSOR_INTERVAL = 2000; // Read/Check every 2 seconds

// Local variables to hold last readings for the display loop
float currentTemp = -999.0;
float currentHum  = -999.0;
int currentSoil   = -1;

// Buffer for parsing UART commands from ESP
String rxBuffer = "";

void setup() {
  Serial.begin(115200);

  initDHT();
  initFan();
  initWatering();
  initDisplay(); // Initialize 4-digit TM1637 display
}

void loop() {
  // 1. Process Incoming Serial Commands from ESP8266
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n') {
      rxBuffer.trim();

      if (rxBuffer.startsWith("SET_T:")) {
        tempThreshold = rxBuffer.substring(6).toFloat();
      } else if (rxBuffer.startsWith("SET_H:")) {
        humThreshold = rxBuffer.substring(6).toFloat();
      } else if (rxBuffer.startsWith("SET_S:")) {
        soilThreshold = rxBuffer.substring(6).toInt();
      }

      rxBuffer = "";
    } else if (c != '\r') {
      rxBuffer += c;
    }
  }

  // 2. Timed Execution for Sensors and Control Logic
  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorSend >= SENSOR_INTERVAL) {
    lastSensorSend = currentMillis;

    // Read, transmit over UART to ESP, and update stored values
    currentTemp = getAndSendTemperature();
    currentHum  = getAndSendHumidity();
    currentSoil   = getAndSendSoilMoisture();

    // Check environment thresholds
    checkFanControl(currentTemp, tempThreshold, currentHum, humThreshold);
    checkSoilWatering(currentSoil, soilThreshold);
  }

  // 3. Continuously run pump cutoff logic for precise 1-second duration
  updatePumpState();

  // 4. Update display rotation (runs continuously to maintain smooth 3-second mode switching)
  displayData(currentTemp, currentHum, currentSoil);
}
