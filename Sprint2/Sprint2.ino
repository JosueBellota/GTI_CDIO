#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define power_pin 5
#define offset 0
#define samplingInterval 20
#define printInterval 800
#define ArrayLength 40

int pHArray[ArrayLength]; // almacena las muestras
int pHArrayIndex = 0;

void setup() {
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(power_pin, OUTPUT);
  Serial.println("Inicializando el medidor de pH");
}

void loop() {
  // int16_t humiditySensorValue = ads.readADC_SingleEnded(humidity_channel);
  // int16_t temperatureSensorValue = ads.readADC_SingleEnded(temperature_channel);
  // int16_t salinitySensorValue = ads.readADC_SingleEnded(salinity_channel);
  // int16_t pHSensorValue = ads.readADC_SingleEnded(pH_channel);
  int16_t sensorValue = ads.readADC_SingleEnded(0);

  // calculateAndPrintHumidity(sensorValue);
  // calculateAndPrintTemperature(sensorValue);
  readAndCalculateSalinity();
  checkLightLevel(sensorValue);
  // readAndCalculatepH();
  delay(1000);
}

void calculateAndPrintHumidity(int16_t sensorValue) {
  int humidityValue = map(sensorValue, 30123, 17290, 0, 100);
  Serial.println();
  Serial.print("Humedad: ");
  Serial.print(humidityValue);
  Serial.println("%");
}

void calculateAndPrintTemperature(int16_t sensorValue) {
  float v0 = (sensorValue * 4.096) / 32767;
  float T = (v0 - 0.79) / 0.035;
  Serial.print(T);
  Serial.println(" Celsius");
}

double lagrangeInterpolation(double* x, double* y, int n, double xi) {
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

void readAndCalculateSalinity() {
  const int n = 5;
  double valorDigital[n] = {519, 654, 666, 670, 674};
  double salinidad[n] = {0, 10, 15, 20, 25};
  int16_t adc0;

  digitalWrite(power_pin, HIGH);
  delay(100);

  adc0 = ads.readADC_SingleEnded(0);

  digitalWrite(power_pin, LOW);
  delay(100);
  
  int16_t Salinidad = lagrangeInterpolation(valorDigital, salinidad, n, adc0);

  Serial.print("Salinidad (g): ");
  Serial.println(Salinidad); 
  Serial.println(adc0); 
}

void readAndCalculatepH() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  int16_t adc;

  if (millis() - samplingTime > samplingInterval) {
    adc = ads.readADC_SingleEnded(0);
    voltage = adc * 4.096 / 32767;
    pHValue = ((3.5 * voltage) + offset);


    double adcv[n] = {1.77, 654, 666, 670, 674};
    double phv[n] = {4.0, 10, 15, 20, 25};

    Serial.print("adc: ");
    Serial.println(adc, DEC);
    Serial.print("Voltage: ");
    Serial.println(voltage, 2);
    Serial.print(" pH value: ");
    Serial.println(pHValue, 2);

    printTime = millis();
    pHArray[pHArrayIndex++] = pHValue;

    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    samplingTime = millis();
  }

  if (millis() - printTime >= 10000) {
    // Your code to execute if the condition is true
    printTime = millis();
  }
}

void checkLightLevel(int16_t sensorValue) {
  if (sensorValue < UMBRAL_OSCURO) {
    Serial.println("Oscuridad");
  } else if (sensorValue < UMBRAL_SOMBRA) {
    Serial.println("Sombra");
  } else if (sensorValue < UMBRAL_LUZ_AMBIENTE) {
    Serial.println("Luz ambiente");
  } else {
    Serial.println("Nivel alto de iluminación");
  }
}

