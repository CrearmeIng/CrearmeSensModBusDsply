#ifndef modbusSens_h
#define modbusSens_h

#include "Arduino.h"
#include <ModbusMaster.h>

union unionFlt {
        float flt;
        int16_t int16[2];
    };

float kPa2inh2o(float valkPa);
float inh2o2kPa(float valkin2o);

class ModbusSens : public ModbusMaster
{
private:
    uint8_t _numSens = 1;

public:
    void begin(uint8_t slaveId, Stream &serial, uint8_t numSens);
    uint8_t getNumSens();
    bool getSensValKpa(float *valkPa);
};



#endif