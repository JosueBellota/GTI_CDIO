#include "ph.h"

pH::pH() {
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(power_pin, OUTPUT);
  samplingTime = millis();
  pHArrayIndex = 0;
}

void pH::read_calculate() {
  if (millis() - samplingTime > samplingInterval) {
    adc = ads.readADC_SingleEnded(0);
    voltage = adc * 4.096 / 32767;
    pHValue = 3.5 * voltage + offset;

    pHArray[pHArrayIndex++] = pHValue;
    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    samplingTime = millis();
  }
}

void pH::print() {
  if (millis() - samplingTime >= printInterval) {
    for (int i = 0; i < ArrayLength; i++) {
      Serial.print("pHArray[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(pHArray[i], 2);
        if( pHArray[i] < 5.8 && pHArray[i] > 5.5 ){
          Serial.print("Valor óptimo");
        }
    }
    Serial.println(); 
  }
}
