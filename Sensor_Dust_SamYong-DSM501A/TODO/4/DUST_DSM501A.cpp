#include "DUST_DSM501A.h"

namespace DiditalPWMSensor {

  const int SEC = 1000;

  DUST_DSM501A::DUST_DSM501A(int pin, int readTime) {
    this->_pin = pin;
    this->_sampleTime = SEC * readTime;
  }
  
  float DUST_DSM501A::read() {
    pinMode(this->_pin, INPUT);
    this->_startTime = millis();
    this->_endTime = this->_startTime + this->_sampleTime;
    while (millis() < this->_endTime) {
      //this->_endTime = millis();
      this->duration = pulseIn(this->_pin, LOW);
      this->_lowPulseOccupancy += this->duration;
    }

    this->_ratio = (this->_lowPulseOccupancy - this->_endTime + this->_startTime + this->_sampleTime)/(this->_sampleTime * 10.0);  // Integer percentage 0=>100
    this->_concentration = 1.1 * pow(this->_ratio, 3) - 3.8 * pow(this->_ratio, 2) + 520 * this->_ratio + 0.62; // using spec sheet curve

    return this->_concentration;
  }
}