//-------------------------------//
// Programa para el monitoreo de sensores 
//--------------------------------//

// Inclusión de bibliotecas necesarias
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "humidity.h"
#include "temperature.h"
#include "light.h"
#include "salinity.h"
#include "ph.h"

// Declaración de objeto para el ADC
Adafruit_ADS1115 ads;

// Definición de pin para el control de energía
#define power_pin 5

// Declaración de objetos para cada sensor
Humidity humiditySensor;
Temperature temperatureSensor;
Light lightSensor;
Salinity salinitySensor;
pH pH_sensor;

// Variables de control de energía y tiempo de muestreo de los sensores
bool saveEnergy = false;  
const unsigned long sensorTimeout = 5000; 
unsigned long lastSensorTime = 0;  

void setup() {
  // Inicialización de comunicación serial y ADC
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(power_pin, OUTPUT);
}

void loop() {
  // Obtención del tiempo actual
  unsigned long currentTime = millis();
  
  // Verificación del tiempo transcurrido para tomar muestras de sensores
  if (!saveEnergy || (saveEnergy && currentTime - lastSensorTime >= sensorTimeout)) {
    // Llamada a las funciones de lectura de cada sensor
    function_humidity();
    function_temperature();
    function_light();
    function_salinity();
    function_php();

    // Actualización del último tiempo de muestreo
    lastSensorTime = currentTime;
  }

  // Retardo para controlar la frecuencia de muestreo
  delay(1000);
}

// Función para la lectura y procesamiento de la humedad
void function_humidity() {
  int16_t humiditySensorValue = ads.readADC_SingleEnded(1);
  humiditySensor.calculate(humiditySensorValue);
  humiditySensor.print();
}

// Función para la lectura y procesamiento de la temperatura
void function_temperature() {
  int16_t temperatureSensorValue = ads.readADC_SingleEnded(2);
  temperatureSensor.calculate(temperatureSensorValue);
  temperatureSensor.print();
}

// Función para la lectura y procesamiento de la luz
void function_light() {
  int16_t lightSensorValue = ads.readADC_SingleEnded(4);
  lightSensor.checkLevel(lightSensorValue);
  lightSensor.print(lightSensorValue);
}

// Función para la lectura y procesamiento de la salinidad
void function_salinity() {
  salinitySensor.read_calculate();
  salinitySensor.print();
}

// Función para la lectura y procesamiento del pH
void function_php() {
  php.read_calculate();
  php.print();
}
