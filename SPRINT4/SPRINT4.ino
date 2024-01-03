#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define power_pin 5

void setup() {
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(power_pin, OUTPUT);
  Serial.println("Inicializando el medidor de pH");
}

void loop() {

  int16_t sensorValue = ads.readADC_SingleEnded(0);
  // int16_t humiditySensorValue = ads.readADC_SingleEnded(1);
  // int16_t temperatureSensorValue = ads.readADC_SingleEnded(2);
  // int16_t lightSensorValue = ads.readADC_SingleEnded(4);

  // int16_t salinitySensorValue = ads.readADC_SingleEnded(salinity_channel);
  // int16_t pHSensorValue = ads.readADC_SingleEnded(sensorValue);


  // calculateAndPrintHumidity(humiditySensorValue);
  // calculateAndPrintTemperature(temperatureSensorValue);
  // readAndCalculateSalinity();
  checkLightLevel();
  readAndCalculatepH();
  delay(1000);
}

void calculateAndPrintHumidity(int16_t sensorValue) {
  int humidityValue = map(sensorValue, 30123, 17290, 0, 100);
  Serial.println();
   Serial.println(" ");
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
  const int n = 6;
  double valorDigital[n] = {427.0, 438.84 , 456.01 , 467.46, 477.0, 490.36};
   double salinidad[n] = {0, 4.74, 11.60, 16.18, 20.0, 25.34};
  int16_t adc0;

  digitalWrite(power_pin, HIGH);
  delay(100);
  adc0 = analogRead(0);
  // adc0 = ads.readADC_SingleEnded(0);

  digitalWrite(power_pin, LOW);
  delay(100);
  
  int16_t Salinidad = lagrangeInterpolation(valorDigital, salinidad, n, adc0);

  Serial.print("Salinidad (g): ");
  Serial.println(Salinidad); 
  Serial.println(adc0); 
}


void readAndCalculatepH() {
  // pH-related variables moved inside the function
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  int16_t adc;

  #define offset 0
  #define samplingInterval 20
  #define printInterval 800
  #define ArrayLength 40

  int pHArray[ArrayLength]; // almacena las muestras
  int pHArrayIndex = 0;

  if (millis() - samplingTime > samplingInterval) {
    adc = ads.readADC_SingleEnded(0);
    voltage = adc * 4.096 / 32767;
    pHValue = ((3.5 * voltage) + offset);
    
    Serial.print("adc: ");
    Serial.println(adc, DEC);
    Serial.print("Voltage: ");
    Serial.println(voltage, 2);
    Serial.print("pH value: ");
    Serial.println(pHValue, 2);

    printTime = millis();
    pHArray[pHArrayIndex++] = pHValue;

    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    samplingTime = millis();
  }

  if (millis() - printTime >= printInterval) {
    // Your code to execute if the condition is true
    printTime = millis();
  }
}


void checkLightLevel() {
  // UMBRAL_OSCURO

  int16_t adclight;
  adclight = ads.readADC_SingleEnded(1);

  if (adclight < 64) {
    Serial.println("Oscuridad");
    Serial.println(" ");
  // UMBRAL_SOMBRA
  } else if (adclight < 66) {
    Serial.println("Sombra");
  // UMBRAL_LUZ_AMBIENTE
  } else if ( adclight < 80
  
  ) {
    Serial.println("Luz ambiente");
  } else {
    Serial.println("Niv el alto de iluminación");
  }


    Serial.print("lights: ");
   Serial.println(adclight);
   Serial.println(" ");
}

