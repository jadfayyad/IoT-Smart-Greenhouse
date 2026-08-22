#include <DHT.h>

#define DHTPIN 4       // Digital pin connected to DHT11 DATA pin
#define DHTTYPE DHT11  // Sensor type

DHT dht(DHTPIN, DHTTYPE);

void initDHT() {
  dht.begin();
}

// Reads, transmits over Serial, and returns Temperature (-999.0 on failure)
float getAndSendTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("FailedT");
    return -999.0;
  } else {
    Serial.print("T:");
    Serial.println(temp);
    return temp;
  }
}

// Reads, transmits over Serial, and returns Humidity (-999.0 on failure)
float getAndSendHumidity() {
  float hum = dht.readHumidity();
  if (isnan(hum)) {
    Serial.println("FailedH");
    return -999.0;
  } else {
    Serial.print("H:");
    Serial.println(hum);
    return hum;
  }
}
