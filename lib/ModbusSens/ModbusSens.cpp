#include "ModbusSens.h"

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
void ModbusSens::begin(uint8_t slaveId, Stream &serial, uint8_t numSens = 1)
{
	_numSens = numSens;
	Stream *_serial;
	_serial = &serial;
	ModbusMaster::begin(slaveId, *_serial);
}

uint8_t ModbusSens::getNumSens()
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
bool ModbusSens::getSensValKpa(float *valkPa)
{
	*valkPa = -0.123f;
	bool ret = true;
	unionFlt unfl;
	float buff = 0.0f;
	uint16_t result;
	result = readInputRegisters(0x00DA, 2);
	if (result == ku8MBSuccess)
	{
		unfl.int16[0] = getResponseBuffer(0);
		unfl.int16[1] = getResponseBuffer(1);
		buff = unfl.flt; // Value in kPa
	}
	else
	{
		ret = false;
	}

	//valkPa[0] = 3.0;
	//valkPa[1] = 1.5;
	*valkPa = buff;

	return ret;
}

float kPa2inh2o(float valkPa)
{
	return valkPa * 4.01865f;
}

float inh2o2kPa(float valkin2o)
{
	return valkin2o / 4.01865f;
}