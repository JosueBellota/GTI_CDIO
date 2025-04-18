#ifndef PH_H
#define PH_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class pH {
  public:
    pH();
    void read_calculate();
    void print();
  private:
    Adafruit_ADS1115 ads;
    const int power_pin = 5;
    const float offset = 0;
    const unsigned long samplingInterval = 20;
    const unsigned long printInterval = 800;
    const int ArrayLength = 40;
    unsigned long samplingTime;
    float pHValue;
    float voltage;
    int16_t adc;
    float pHArray[40]; 
    int pHArrayIndex;
};

#endif
