#include "salinity.h"

Salinity::Salinity() {
  ads.begin();
  ads.setGain(GAIN_ONE);
}

void Salinity::read_calculate() {
  digitalWrite(power_pin, HIGH);
  delay(100);
  adc0 = analogRead(0);
  digitalWrite(power_pin, LOW);
  delay(100);
  
  salinityValue = lagrangeInterpolation(valorDigital, salinidad, n, adc0);
}

void Salinity::print() {
  Serial.print("Salinidad (g): ");
  Serial.println(salinityValue); 
  Serial.println(adc0); 
  if( salinityValue < 2.5 && salinityValue > 1.5 ){
  Serial.print("Valor óptimo");
  }
}

double Salinity::lagrangeInterpolation(double* x, double* y, int n, double xi) {
  double result = 0.0;

  for (int i = 0; i < n; i++) {
    double term = y[i];
    for (int j = 0; j < n; j++) {
      if (j != i) {
        term = term * (xi - x[j]) / (x[i] - x[j]);
      } 
    }
    result += term;
  }
  return result;
}
