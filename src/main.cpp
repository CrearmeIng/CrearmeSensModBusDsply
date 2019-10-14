#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
//#include <ModbusMaster.h>
#include <ModbusSens.h>
#include "Free_Fonts.h"

#define SLAVE1_ID 0x01
#define SLAVE2_ID 0x02
//#define CTRL_PIN			5     //D1
#define CTRL_PIN 25
#define IRQ_PIN 27
#define BAUDRATE 115200
#define smplTm 1000 //ms

//#define DEBUG

static TFT_eSPI tft = TFT_eSPI();
static ModbusSens node1;
static float sensValkPa[] = {0.0, 0.0};
ModbusSens node2;
TFT_eSPI_Button btnPrev;
TFT_eSPI_Button btnNext;

bool state = true;
long lstTm = 0;
long currTm = 0;

bool next = 0;

// ---------------------- RS-485 Methods ----------------------

void preTransmission1()
{
	digitalWrite(CTRL_PIN, 1);
}

void postTransmission1()
{
	digitalWrite(CTRL_PIN, 0);
}

// --------------------- Display Methods ----------------------
void showSensVal(float valkPa, char *title, int32_t poX, int32_t poY)
{
	tft.drawRect(poX, poY + 7, 148, 72, TFT_WHITE);
	tft.setFreeFont(FSS9);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.drawCentreString(title, poX + 74, poY, 1);
	if (valkPa == NULL)
	{
		tft.setTextColor(TFT_RED);
		tft.setFreeFont(FSS12);
		tft.fillRect(poX + 1, poY + 17, 146, 58, TFT_BLACK);
		tft.drawString("No Disp", poX + 30, poY + 32);
		tft.setFreeFont(FSS9);
		delay(50);
	}
	else
	{
		if ((valkPa < 0.0) || (valkPa > inh2o2kPa(2.5)))
		{
			tft.setTextColor(TFT_RED);
		}
		else
		{
			tft.setTextColor(TFT_WHITE);
		}
		float valinh2o = kPa2inh2o(valkPa);
		//tft.setTextColor(TFT_WHITE,TFT_BLACK);
		tft.setFreeFont(FF0);
		tft.drawString("in", poX + 10, poY + 28);
		tft.drawString("h2o", poX + 6, poY + 38);
		tft.drawString("kPa", poX + 6, poY + 62);
		tft.setFreeFont(FSS24);
		tft.fillRect(poX + 27, poY + 18, 114, 55, TFT_BLACK);
		tft.drawFloat(valinh2o, 2, poX + 27, poY + 18);
		tft.setFreeFont(FSS9);
		tft.drawFloat(valkPa, 3, poX + 27, poY + 58);
		delay(50);
	}
}

void screen1(char *title)
{
	tft.setFreeFont(FSS9);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.drawCentreString(title, 160, 205, 1);
	showSensVal(sensValkPa[0], " Sensor 1  ", 96, 25);
	showSensVal(sensValkPa[1], " Sensor 2  ", 96, 110);
	/*showSensVal(getSensValKpa(node,0)," Sensor 1  ", 6, 25);
	showSensVal(getSensValKpa(node,1)," Sensor 2  ", 166, 25);
	showSensVal(getSensValKpa(node,0)," Sensor 3  ", 6, 110);
	showSensVal(getSensValKpa(node,1)," Sensor 4  ", 166, 110);*/
}

void IRAM_ATTR touchISR()
{
	detachInterrupt(digitalPinToInterrupt(IRQ_PIN));
	uint16_t x = 0;
	uint16_t y = 0;
	tft.getTouch(&x, &y);
	// ------------------------ Prev Button ------------------------
	if (btnPrev.contains(x, 240 - y))
	{
		btnPrev.press(true); // tell the button it is pressed
	}
	else
	{
		btnPrev.press(false); // tell the button it is NOT pressed
	}
	if (btnPrev.justReleased())
	{
		btnPrev.drawButton(); // draw normal
	}
	if (btnPrev.justPressed())
	{
		btnPrev.drawButton(true); // draw invert!
	}
	// ------------------------ Next Button ------------------------
	if (btnNext.contains(x, 240 - y))
	{
		//Serial.print("Pressing: "); Serial.println(b);
		btnNext.press(true); // tell the button it is pressed
	}
	else
	{
		btnNext.press(false); // tell the button it is NOT pressed
	}
	if (btnNext.justReleased())
	{
		btnNext.drawButton(); // draw normal
		next = 1;
	}
	if (btnNext.justPressed())
	{
		btnNext.drawButton(true); // draw invert!
	}
	digitalWrite(LED_BUILTIN, !digitalRead(IRQ_PIN));
	attachInterrupt(digitalPinToInterrupt(IRQ_PIN), touchISR, CHANGE);
}

void TaskRS485(void *pvParameters) // This is a task.
{
	(void)pvParameters;

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(CTRL_PIN, OUTPUT);
	Serial.begin(BAUDRATE);
	node1.begin(SLAVE1_ID, Serial, 2);
	node1.preTransmission(preTransmission1);
	node1.postTransmission(postTransmission1);

	for (;;)
	{
		node1.getSensValKpa(sensValkPa);
		vTaskDelay(100);
	}
}

void TaskTFT(void *pvParameters) // This is a task.
{
	(void)pvParameters;

	tft.init();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);
	tft.setTextColor(TFT_WHITE);
	tft.setFreeFont(FSS9);
	tft.drawCentreString("Monitoreo de Presion Diferencial", 160, 3, 1);
	tft.drawLine(10, 20, 310, 20, TFT_WHITE);

	/*pinMode(IRQ_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(IRQ_PIN), touchISR, CHANGE);
	btnPrev.initButtonUL(&tft, 5, 195, 70, 40, TFT_WHITE, TFT_BLACK, TFT_WHITE, "<<", 1);
	btnPrev.drawButton();
	btnNext.initButtonUL(&tft, 245, 195, 70, 40, TFT_WHITE, TFT_BLACK, TFT_WHITE, ">>", 1);
	btnNext.drawButton();
	//uint16_t calData[5];
	//tft.calibrateTouch(calData,TFT_BLUE,TFT_CYAN,15);*/

	for (;;)
	{
		screen1("Maquina 1");
		vTaskDelay(100);
	}
}

void setup()
{

	xTaskCreatePinnedToCore(TaskRS485, "Task RS-485", 1024, NULL, 2, NULL, 0);
	xTaskCreatePinnedToCore(TaskTFT, "Task TFT", 1024, NULL, 2, NULL, 1);
}

void loop()
{
	/*if (next)
	{
		tft.fillScreen(TFT_BLACK);
		tft.setTextColor(TFT_WHITE);
		tft.setFreeFont(FSS9);
		tft.drawCentreString("Siguiente Pantalla 2", 160, 3, 1);
		next = 0;
	}*/
}
