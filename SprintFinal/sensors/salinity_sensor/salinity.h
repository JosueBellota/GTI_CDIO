#ifndef SALINITY_H
#define SALINITY_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

class Salinity {
  public:
    Salinity();
    void read_calculate();
    void print();
  private:
    Adafruit_ADS1115 ads;
    const int n = 6;
    double valorDigital[6] = {427.0, 438.84 , 456.01 , 467.46, 477.0, 490.36};
    double salinidad[6] = {0, 4.74, 11.60, 16.18, 20.0, 25.34};
    int16_t adc0;
    double salinityValue;
    double lagrangeInterpolation(double* x, double* y, int n, double xi);
};

#endif
