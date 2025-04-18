#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class Temperature {
  public:
    Temperature();
    void calculate(int16_t sensorValue);
    void print();
  private:
    Adafruit_ADS1115 ads;
    float temperatureValue;
};

#endif
