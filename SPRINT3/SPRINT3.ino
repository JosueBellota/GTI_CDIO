#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define power_pin 5
#define offset 0
#define samplingInterval 20
#define printInterval 800
#define ArrayLength 40

int pHArray[ArrayLength]; // Stores pH samples
int pHArrayIndex = 0;

// Function to set up the initial configuration
void setup() {
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(power_pin, OUTPUT);
  Serial.println("Initializing pH meter");
}

// Main execution loop
void loop() {
  int16_t sensorValue = ads.readADC_SingleEnded(0);

  // Uncomment the following lines to enable additional sensor readings
  // calculateAndPrintHumidity(sensorValue);
  // calculateAndPrintTemperature(sensorValue);
  readAndCalculateSalinity();
  checkLightLevel(sensorValue);
  // readAndCalculatepH();
  delay(1000);
}

// Function to calculate and print humidity based on sensor value
void calculateAndPrintHumidity(int16_t sensorValue) {
  int humidityValue = map(sensorValue, 30123, 17290, 0, 100);
  Serial.println();
  Serial.print("Humidity: ");
  Serial.print(humidityValue);
  Serial.println("%");
}

// Function to calculate and print temperature based on sensor value
void calculateAndPrintTemperature(int16_t sensorValue) {
  float v0 = (sensorValue * 4.096) / 32767;
  float T = (v0 - 0.79) / 0.035;
  Serial.print(T);
  Serial.println(" Celsius");
}

// Lagrange interpolation function for salinity calculation
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

// Function to read and calculate salinity
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

  Serial.print("Salinity (g): ");
  Serial.println(Salinidad); 
  Serial.println(adc0); 
}

// Function to read and calculate pH
void readAndCalculatepH() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;  
  int16_t adc;

  if (millis() - samplingTime > samplingInterval) {
    adc = ads.readADC_SingleEnded(0);
    voltage = adc * 4.096 / 32767;
    pHValue = ((3.5 * voltage) + offset);
    
    int n2 = 5;
    double v[n2] = {6.07, 6.44, 7.0, 7.62, 7.70};
    double phv[n2] = {4.0, 4.70, 7.0, 9.53, 9.88};

    double pHResult = lagrangeInterpolation(v, phv, n2, pHValue);  
    Serial.print("adc: ");
    Serial.println(adc, DEC);
    Serial.print("Voltage: ");
    Serial.println(voltage, 2);
    Serial.print("pH value: ");
    Serial.println(pHResult, 2);

    printTime = millis();
    pHArray[pHArrayIndex++] = pHResult;

    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    samplingTime = millis();
  }

  if (millis() - printTime >= 10000) {
    // Your code to execute if the condition is true
    printTime = millis();
  }
}

// Function to check and print light level based on sensor value
void checkLightLevel(int16_t sensorValue) {
  // DARK_THRESHOLD
  if (sensorValue < 1000) {
    Serial.println("Darkness");
  // SHADE_THRESHOLD
  } else if (sensorValue < 2000) {
    Serial.println("Shade");
  // AMBIENT_LIGHT_THRESHOLD
  } else if (sensorValue < 3000) {
    Serial.println("Ambient light");
  } else {
    Serial.println("High light level");
  }
}
