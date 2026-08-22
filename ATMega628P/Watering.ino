#define PUMP_PIN 9

unsigned long pumpStartTime = 0;
unsigned long pumpLastRunTime = 0;

bool isPumpActive = false;

const unsigned long PUMP_DURATION = 1000;   // 1 second duration
const unsigned long PUMP_COOLDOWN = 600000; // 10 minutes in milliseconds

void initWatering() {
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW); // Default OFF
}

// Dedicated function to handle non-blocking 1-second pump cutoff
void updatePumpState() {
  if (isPumpActive) {
    unsigned long currentMillis = millis();
    if (currentMillis - pumpStartTime >= PUMP_DURATION) {
      digitalWrite(PUMP_PIN, LOW);
      isPumpActive = false;
      pumpLastRunTime = currentMillis; // Start 10-minute cooldown timer
    }
  }
}

// Checks soil moisture and triggers pump start
void checkSoilWatering(int currentMoisture, int moistureThreshold) {
  unsigned long currentMillis = millis();

  // Handle cutoff timer first
  updatePumpState();

  // Check 10-minute cooldown status
  bool cooldownPassed = (pumpLastRunTime == 0) || (currentMillis - pumpLastRunTime >= PUMP_COOLDOWN);

  // Trigger pump if moisture is below threshold, not already running, cooldown passed, and sensor valid
  if (currentMoisture < moistureThreshold && !isPumpActive && cooldownPassed && currentMoisture != -1) {
    isPumpActive = true;
    pumpStartTime = currentMillis;
    digitalWrite(PUMP_PIN, HIGH);
  }
}
