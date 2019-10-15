#include <Arduino.h>
//#include <ModbusMaster.h>
#include <TFTSens.h>
#include <ModbusSens.h>

#define SLAVE1_ID 0x01
#define SLAVE2_ID 0x02
//#define CTRL_PIN			5     //D1
#define CTRL_PIN 25
#define IRQ_PIN 27
#define BAUDRATE 115200
#define smplTm 1000 //ms

//#define DEBUG

static float sensValkPa[] = {0.0, 0.0};
//TFT_eSPI_Button btnPrev;
//TFT_eSPI_Button btnNext;

SemaphoreHandle_t xMutex;

// ---------------------- RS-485 Methods ----------------------

void preTransmission1()
{
	digitalWrite(CTRL_PIN, 1);
}

void postTransmission1()
{
	digitalWrite(CTRL_PIN, 0);
}

/*void IRAM_ATTR touchISR()
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
}*/

void TaskRS485(void *pvParameters) // This is a task.
{
	//(void)pvParameters;

	static ModbusSens node1;

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(CTRL_PIN, OUTPUT);
	Serial.begin(BAUDRATE);
	node1.begin(SLAVE1_ID, Serial, 2);
	node1.preTransmission(preTransmission1);
	node1.postTransmission(postTransmission1);

	for (;;)
	{
		if (xSemaphoreTake(xMutex, portMAX_DELAY))
		{
			node1.getSensValKpa((float *)pvParameters);
			xSemaphoreGive(xMutex);
		}
		vTaskDelay(100);
	}
}

void TaskTFT(void *pvParameters) // This is a task.
{
	//(void)pvParameters;

	static TFTSens tft;

	tft.init();
	tft.setRotation(1);
	tft.header("Monitoreo de Presion Diferencial");

	/*pinMode(IRQ_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(IRQ_PIN), touchISR, CHANGE);
	btnPrev.initButtonUL(&tft, 5, 195, 70, 40, TFT_WHITE, TFT_BLACK, TFT_WHITE, "<<", 1);
	btnPrev.drawButton();
	btnNext.initButtonUL(&tft, 245, 195, 70, 40, TFT_WHITE, TFT_BLACK, TFT_WHITE, ">>", 1);
	btnNext.drawButton();
	//uint16_t calData[5];
	//tft.calibrateTouch(calData,TFT_BLUE,TFT_CYAN,15);*/

	float sensValkPa[] = {3.0, 1.5, 2.3, 0.326};

	for (;;)
	{
		/*if (xSemaphoreTake(xMutex, portMAX_DELAY))
		{
			//sensValkPa = (float *)pvParameters;
			xSemaphoreGive(xMutex);
		}*/
		tft.screenSens(sensValkPa, 4, "Maquina 1");
		vTaskDelay(200);
	}
}

void setup()
{
	xTaskCreatePinnedToCore(TaskRS485, "Task RS-485", 1024, sensValkPa, 2, NULL, 0);
	xTaskCreatePinnedToCore(TaskTFT, "Task TFT", 1024, sensValkPa, 2, NULL, 1);
	xMutex = xSemaphoreCreateMutex();
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
