#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "humidity.h"
#include "temperature.h"
#include "light.h"
#include "salinity.h"
#include "ph.h"

Adafruit_ADS1115 ads;

#define power_pin 5

Humidity humiditySensor;
Temperature temperatureSensor;
Light lightSensor;
Salinity salinitySensor;
pH pH_sensor;

bool saveEnergy = false;  
const unsigned long sensorTimeout = 5000; 
unsigned long lastSensorTime = 0;  


void setup() {
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(power_pin, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis();
  if (!saveEnergy || (saveEnergy && currentTime - lastSensorTime >= sensorTimeout)) {
  function_humidity();
  function_temperature();
  function_light();
  function_salinity();
  function_php();

  lastSensorTime = currentTime;
  }

  delay(1000);
  
}

void function_humidity() {
  int16_t humiditySensorValue = ads.readADC_SingleEnded(1);
  humiditySensor.calculate(humiditySensorValue);
  humiditySensor.print();
}

void function_temperature() {
  int16_t temperatureSensorValue = ads.readADC_SingleEnded(2);
  temperatureSensor.calculate(temperatureSensorValue);
  temperatureSensor.print();
}

void function_light() {
  int16_t lightSensorValue = ads.readADC_SingleEnded(4);
  lightSensor.checkLevel(lightSensorValue);
  lightSensor.print(lightSensorValue);
}

void function_salinity() {
  salinitySensor.read_calculate();
  salinitySensor.print();
}

void function_php() {
  php.read_calculate();
  php.print();
}