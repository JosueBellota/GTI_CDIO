#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#define power_pin 5
#define channelValue 0
#define offset 0.4
#define samplingInterval 20
#define printInterval 800
#define ArrayLength 40 // numero de muestras

const int n = 5;
double valorDigital[n] = {519, 654, 666, 670, 674};
double salinidad[n] = {0, 10, 15, 20, 25};

// Function prototypes
void salinidadSetup();
void salinidadLoop();
void pHSetup();
void pHLoop();

void setup() {
  salinidadSetup();
  pHSetup();
}

void loop() {
  salinidadLoop();
  pHLoop();
}

void salinidadSetup() {
  pinMode(power_pin, OUTPUT);
  Serial.begin(9600);
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

void salinidadLoop() {
  int16_t adc0;

  // Alimentamos la sonda con un tren de pulsos
  digitalWrite(power_pin, HIGH);
  delay(100);

  // Leemos cuando hay un nivel alto
  // adc0 = analogRead(0);
  adc0 = 639;

  // Calculamos la salinidad usando interpolación de Lagrange
  int16_t Salinidad = lagrangeInterpolation(valorDigital, salinidad, n, adc0);

  digitalWrite(power_pin, LOW);
  delay(100);

  Serial.print("Salinidad (g): ");
  Serial.println(Salinidad);
  Serial.println(adc0);
}

void pHSetup() {
  // Inicializamos el monitor serie
  Serial.begin(9600);
  Serial.println("Inicializando el medidor de pH");

  // Inicializamos el ADS1115
  ads.begin();

  // Configuramos la ganancia del ADS1115
  ads.setGain(GAIN_ONE);
}

void pHLoop() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  int16_t adc;

  if (millis() - samplingTime > samplingInterval) { // Cada samplingTime segundos se toma una muestra
    adc = ads.readADC_SingleEnded(channelValue);
    voltage = adc * 4.096 / 32767;
    pHValue = 3.5 * voltage + offset;

    Serial.print("adc: ");
    Serial.print(adc, DEC);
    Serial.print(" Voltage: ");
    Serial.print(voltage, 2);
    Serial.print(" pH value: ");
    Serial.println(pHValue, 2);

    printTime = millis();
    pHArray[pHArrayIndex++] = pHValue;

    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    samplingTime = millis();
  }

  if (millis() - printTime >= 1000) {
    // Tu código que se ejecutará si la condición es verdadera
    printTime = millis(); // Actualiza el tiempo de la última ejecución
  }
}
