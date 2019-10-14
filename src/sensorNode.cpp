#include "sensorNode.h"

/**
 * @brief Initialize class object.
 * 
 * Assigns the Modbus slave ID, serial port and number of sensors installed in the node.
 * Call once class has been instantiated, typically within setup().
 * 
 * @param slaveId Modbus slave ID (1..255)
 * @param serial reference to serial port object (Serial, Serial1, ... Serial3)
 * @param numSens number of differential pressure sensor in the node (1-6)
 */
void sensorNode::begin(uint8_t slaveId, Stream &serial, uint8_t numSens = 1)
{
    _numSens = numSens;
    Stream *_serial;
    _serial = &serial;
    ModbusMaster::begin(slaveId, *_serial);
}

uint8_t sensorNode::getNumSens()
{
    return _numSens;
}

/**
 * @brief Get all available sensor data in kPa, depends on numSens
 * 
 * @param valkPa array buffer to get sensor value. Length must be, at least, same as numSens
 * @return true 
 * @return false 
 */
bool sensorNode::getSensValKpa(float *valkPa)
{
    bool ret = true;
    uint16_t reg = 0x00DA;
    for (uint8_t i = 0; i < _numSens; i++)
    {
        uint16_t result = readInputRegisters(reg, 2);
        if (result == ku8MBSuccess)
        {
            unionFlt unfl;
            unfl.int16[0] = getResponseBuffer(0);
            unfl.int16[1] = getResponseBuffer(1);
            valkPa[i] = unfl.flt; // Value in kPa
        }
        else
        {
            valkPa[i] = NULL;
            ret = false;
        }
        reg += 4;
    }
    return ret;
}

float kPa2inh2o(float valkPa)
{
    return valkPa * 4.01865;
}

float inh2o2kPa(float valkin2o)
{
    return valkin2o / 4.01865;
}