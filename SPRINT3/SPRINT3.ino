#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define power_pin 5
#define offset 0
#define samplingInterval 20
#define printInterval 800
#define ArrayLength 40

int pHArray[ArrayLength]; // Almacena las muestras de pH
int pHArrayIndex = 0;

// Función para configurar la configuración inicial
void setup() {
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(power_pin, OUTPUT);
  Serial.println("Inicializando el medidor de pH");
}

// Bucle principal de ejecución
void loop() {
  int16_t sensorValue = ads.readADC_SingleEnded(0);

  // Descomenta las siguientes líneas para habilitar lecturas adicionales del sensor
  // calculateAndPrintHumidity(sensorValue);
  // calculateAndPrintTemperature(sensorValue);
  readAndCalculateSalinity();
  checkLightLevel(sensorValue);
  // readAndCalculatepH();
  delay(1000);
}

// Función para calcular e imprimir la humedad basada en el valor del sensor
void calculateAndPrintHumidity(int16_t sensorValue) {
  int humidityValue = map(sensorValue, 30123, 17290, 0, 100);
  Serial.println();
  Serial.print("Humedad: ");
  Serial.print(humidityValue);
  Serial.println("%");
}

// Función para calcular e imprimir la temperatura basada en el valor del sensor
void calculateAndPrintTemperature(int16_t sensorValue) {
  float v0 = (sensorValue * 4.096) / 32767;
  float T = (v0 - 0.79) / 0.035;
  Serial.print(T);
  Serial.println(" Celsius");
}

// Función de interpolación de Lagrange para el cálculo de salinidad
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

// Función para leer y calcular la salinidad
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

// Función para leer y calcular el pH
void readAndCalculatepH() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;  
  int16_t adc;

  if (millis() - samplingTime > samplingInterval) {
    adc = ads.readADC_SingleEnded(0);
    voltage = adc * 4.096 / 32767;
    pHValue = ((3.5 * voltage) + offset);
    
    Serial.print("adc: ");
    Serial.println(adc, DEC);
    Serial.print("Voltage: ");
    Serial.println(voltage, 2);
    Serial.print("Valor de pH: ");
    Serial.println(pHResult, 2);

    printTime = millis();
    pHArray[pHArrayIndex++] = pHResult;

    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    samplingTime = millis();
  }

  if (millis() - printTime >= 10000) {
    // Tu código para ejecutar si la condición es verdadera
    printTime = millis();
  }
}

// Función para verificar e imprimir el nivel de luz según el valor del sensor
void checkLightLevel(int16_t sensorValue) {
  // UMBRAL_OSCURO
  if (sensorValue < 1000) {
    Serial.println("Oscuridad");
  // UMBRAL_SOMBRA
  } else if (sensorValue < 2000) {
    Serial.println("Sombra");
  // UMBRAL_LUZ_AMBIENTE
  } else if (sensorValue < 3000) {
    Serial.println("Luz ambiente");
  } else {
    Serial.println("Nivel alto de iluminación");
  }
}
