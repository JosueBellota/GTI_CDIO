#include "temperature.h"

Temperature::Temperature() {
  ads.begin();
  ads.setGain(GAIN_ONE);
}

void Temperature::calculate(int16_t sensorValue) {
  float v0 = (sensorValue * 4.096) / 32767;
  temperatureValue = (v0 - 0.79) / 0.035;
}

void Temperature::print() {
  Serial.print(temperatureValue);
  Serial.println(" Celsius");
  if( temperatureValue < 24 && temperatureValue > 18 ){
    Serial.print("Valor óptimo");
  }
}
