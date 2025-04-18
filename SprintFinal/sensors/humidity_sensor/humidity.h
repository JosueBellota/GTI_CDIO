#ifndef HUMIDITY_H
#define HUMIDITY_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class Humidity {
  public:
    Humidity();
    void calculate(int16_t sensorValue);
    void print();
  private:
    Adafruit_ADS1115 ads;
    int humidityValue;
};

#endif
