#include <Wire.h>
#include <Adafruit_ADS1X15.h> 
// ----------------------------------
// Programa de Humedad y Temperatura
// ------------------------------------
Adafruit_ADS1115 ads;

int channelValue = 0;
int sensorValue = 0;
int humidityValue = 0;

void setup() {
  Serial.begin(9600);
  // Inicialización del ADS1115
  ads.begin();
  // Ajuste de la ganancia (elige la ganancia adecuada)
  ads.setGain(GAIN_ONE); // Ejemplo: Ganancia de 1x
}

void loop() {
  // Lectura del canal del ADS1115

  // canal de humedad
  sensorValue = ads.readADC_SingleEnded(0);
  // canal de temperatura
  int16_t sensorValue2 = ads.readADC_SingleEnded(2); // Lee el canal 0 (ajusta el número de canal según tu configuración)
  // Realiza el cálculo para obtener la humedad (ajusta según la fórmula y calibración necesaria)
  
  float v0= (sensorValue2*4.096)/32767;

  // formula de la recta
  float T = (v0 - 0.79)/0.035;

// regla de tres de humedad
  humidityValue = map(sensorValue, 30123,17290, 0, 100);

// double kelvin = T + 274.15;

  //Serial.print(v0);
  Serial.print(T);
  Serial.println("Celcius ");
  Serial.println(v0);

  // Serial.println(kelvin);
  // Serial.println("Kelvin");

  Serial.print("Humedad: ");
  Serial.print(humidityValue );
  Serial.println("%");
  Serial.print(sensorValue);



  //

  delay(1000);
}
