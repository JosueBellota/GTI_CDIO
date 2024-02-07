#ifndef LIGHT_H
#define LIGHT_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class Light {
  public:
    Light();
    void checkLevel(int16_t sensorValue);
    void print(int16_t sensorValue);
  private:
    Adafruit_ADS1115 ads;
    int lightValue;
};

#endif
