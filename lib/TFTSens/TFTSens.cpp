#include "TFTSens.h"
#include "Free_Fonts.h"
#include "ModbusSens.h"

void TFTSens::showSensVal(float valkPa, char *title, int32_t poX, int32_t poY)
{
	drawRect(poX, poY + 7, 148, 72, TFT_WHITE);
	setFreeFont(FSS9);
	setTextColor(TFT_WHITE, TFT_BLACK);
	drawCentreString(title, poX + 74, poY, 1);
	if (valkPa == NULL)
	{
		setTextColor(TFT_RED);
		setFreeFont(FSS12);
		fillRect(poX + 1, poY + 17, 146, 58, TFT_BLACK);
		drawString("No Disp", poX + 30, poY + 32);
		setFreeFont(FSS9);
		delay(50);
	}
	else
	{
		if ((valkPa < 0.0) || (valkPa > inh2o2kPa(2.5)))
		{
			setTextColor(TFT_RED);
		}
		else if (valkPa >= inh2o2kPa(1.5))
		{
			setTextColor(TFT_YELLOW);
		}
		else
		{
			setTextColor(TFT_WHITE);
		}
		float valinh2o = kPa2inh2o(valkPa);
		//setTextColor(TFT_WHITE,TFT_BLACK);
		setFreeFont(FF0);
		drawString("in", poX + 10, poY + 28);
		drawString("h2o", poX + 6, poY + 38);
		drawString("kPa", poX + 6, poY + 62);
		setFreeFont(FSS24);
		fillRect(poX + 27, poY + 18, 114, 55, TFT_BLACK);
		drawFloat(valinh2o, 2, poX + 27, poY + 18);
		setFreeFont(FSS9);
		drawFloat(valkPa, 3, poX + 27, poY + 58);
		delay(50);
	}
}

void TFTSens::header(char *title)
{
	fillScreen(TFT_BLACK);
	setTextColor(TFT_WHITE);
	setFreeFont(FSS9);
	drawCentreString(title, 160, 3, 1);
	drawLine(10, 20, 310, 20, TFT_WHITE);
}

void TFTSens::screenSens(float *sensValkPa, uint8_t len, char *title)
{
	setFreeFont(FSS9);
	setTextColor(TFT_WHITE, TFT_BLACK);
	drawCentreString(title, 160, 205, 1);
	switch (len)
	{
	case 1:
		showSensVal(sensValkPa[0], " Sensor 1  ", 87, 80);
		break;
	case 2:
		showSensVal(sensValkPa[0], " Sensor 1  ", 87, 25);
		showSensVal(sensValkPa[1], " Sensor 2  ", 87, 110);
		break;
	case 3:
		showSensVal(sensValkPa[0], " Sensor 1  ", 87, 25);
		showSensVal(sensValkPa[1], " Sensor 2  ", 6, 110);
		showSensVal(sensValkPa[2], " Sensor 3  ", 166, 110);
		break;
	case 4:
		showSensVal(sensValkPa[0], " Sensor 1  ", 6, 25);
		showSensVal(sensValkPa[1], " Sensor 2  ", 166, 25);
		showSensVal(sensValkPa[2], " Sensor 3  ", 6, 110);
		showSensVal(sensValkPa[3], " Sensor 4  ", 166, 110);
		break;
	default:
		break;
	}
}