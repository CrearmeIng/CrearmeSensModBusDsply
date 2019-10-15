#ifndef TFTSens_h
#define TFTSens_h
#include "Arduino.h"
#include <TFT_eSPI.h>
#include <SPI.h>

class TFTSens : public TFT_eSPI
{
private:
	void showSensVal(float valkPa, char *title, int32_t poX, int32_t poY);

public:
	void header(char *title);
	void screenSens(float *sensValkPa, uint8_t len, char *title);
};

#endif