#include "light.h"

Light::Light() {
  ads.begin();
  ads.setGain(GAIN_ONE);
}

void Light::checkLevel(int16_t sensorValue) {
  lightValue = sensorValue;
  if (sensorValue < 64) {
    Serial.println("Oscuridad");
  } else if (sensorValue < 66) {
    Serial.println("Sombra");
  } else if (sensorValue < 80) {
    Serial.println("Luz Ambiente");
  } else {
    Serial.println("Mucha Iluminación");
  }
}

void Light::print(int16_t sensorValue) {
  Serial.print("Light sensor value: ");
  Serial.println(sensorValue);
}
