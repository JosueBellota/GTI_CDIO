#include "humidity.h"

Humidity::Humidity() {
  ads.begin();
  ads.setGain(GAIN_ONE);
}

void Humidity::calculate(int16_t sensorValue) {
  humidityValue = map(sensorValue, 30123, 17290, 0, 100);
}

void Humidity::print() {
  Serial.println();
  Serial.println(" ");
  Serial.print("Humedad: ");
  Serial.print(humidityValue);
  Serial.println("%");
}
