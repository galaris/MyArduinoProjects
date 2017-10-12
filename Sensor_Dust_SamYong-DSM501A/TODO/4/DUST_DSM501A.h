#ifndef DUST_DSM501A_h
#define DUST_DSM501A_h

#include <Arduino.h>

namespace DiditalPWMSensor {
  class DUST_DSM501A {
    public:
      /**
       * Constructor
       */
      DUST_DSM501A(int pin, int readTime);
    
      /**
       * Returns the mg/m3 concentration
       */
      float read();

    private:
      int _pin;
      int _sampleTime;
      unsigned long _startTime;
      unsigned long _endTime;
      unsigned long _lowPulseOccupancy = 0;
      float _ratio = 0;
      float _concentration = 0;
      unsigned long duration;
  };
}
#endif