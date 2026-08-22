#include <TM1637Display.h>

// Pins for TM1637 4-Digit Display (Change pin numbers if needed)
#define CLK_PIN 2
#define DIO_PIN 3

// 1. Declare the TM1637 display object
TM1637Display display(CLK_PIN, DIO_PIN);

// 2. Define custom segment patterns
const uint8_t MY_SEG_C = SEG_A | SEG_F | SEG_E | SEG_D;          // Custom 'C' for Temp
const uint8_t MY_SEG_H = SEG_F | SEG_E | SEG_G | SEG_B | SEG_C;  // Custom 'H' for Humidity
const uint8_t MY_SEG_L = SEG_F | SEG_E | SEG_D;                  // Custom 'L' for Soil (Liquid/Moisture)

// 3. Initialization function to call in setup()
void initDisplay() {
  display.setBrightness(0x0f); // Set to maximum brightness
  display.clear();
}

// 4. Function to display cycles (Temp -> Humidity -> Soil Moisture)
void displayData(float tempC, float humidity, int soilPercent) {
  static unsigned long lastDisplaySwitch = 0;
  static int displayMode = 0; // 0 = Temp, 1 = Humidity, 2 = Soil
  unsigned long currentMs = millis();

  // Cycle mode every 3 seconds
  if (currentMs - lastDisplaySwitch >= 3000) {
    lastDisplaySwitch = currentMs;
    displayMode = (displayMode + 1) % 3;
  }

  uint8_t data[4];

  if (displayMode == 0) {
    // === 1. TEMPERATURE ===
    if (isnan(tempC) || tempC == -999.0) {
      display.showNumberDec(-99, false);
    } else {
      int tempVal = constrain((int)tempC, 0, 99);
      data[0] = (tempVal < 10) ? 0x00 : display.encodeDigit(tempVal / 10);
      data[1] = display.encodeDigit(tempVal % 10);
      data[2] = 0b01100011; // Degree symbol (°)
      data[3] = MY_SEG_C;
      display.setSegments(data);
    }

  } else if (displayMode == 1) {
    // === 2. HUMIDITY ===
    if (isnan(humidity) || humidity == -999.0) {
      display.showNumberDec(-99, false);
    } else {
      int humVal = constrain((int)humidity, 0, 99);
      data[0] = 0x00;
      data[1] = (humVal < 10) ? 0x00 : display.encodeDigit(humVal / 10);
      data[2] = display.encodeDigit(humVal % 10);
      data[3] = MY_SEG_H;
      display.setSegments(data);
    }

  } else if (displayMode == 2) {
    // === 3. SOIL MOISTURE ===
    if (soilPercent == -1) {
      display.showNumberDec(-99, false);
    } else if (soilPercent == 100) {
      data[0] = display.encodeDigit(1);
      data[1] = display.encodeDigit(0);
      data[2] = display.encodeDigit(0);
      data[3] = MY_SEG_L;
      display.setSegments(data);
    } else {
      data[0] = 0x00;
      data[1] = (soilPercent >= 10) ? display.encodeDigit(soilPercent / 10) : 0x00;
      data[2] = display.encodeDigit(soilPercent % 10);
      data[3] = MY_SEG_L;
      display.setSegments(data);
    }
  }
}
