#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <ModbusMaster.h>

#define SLAVE_ID 0x01
#define CTRL_PIN PIN_D0
#define BAUDRATE 115200

// Hola Mundo
#define DEBUG

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
ModbusMaster node;


bool state = true;

void preTransmission()
{
  digitalWrite(CTRL_PIN, 1);
}

void postTransmission()
{
  digitalWrite(CTRL_PIN, 0);
}

void setup() {
  // Modbus setup
  pinMode(CTRL_PIN, OUTPUT);
  Serial.begin(BAUDRATE);
  node.begin(SLAVE_ID, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  // Setup the TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("Monitoreo de Presion Diferencial",160,0,2);
  tft.drawLine(10,17,310,17,TFT_WHITE);
  tft.fillRoundRect(10,190,70,40,3,TFT_BLUE);
}


void loop() {
  uint8_t result;
  /*result = node.writeSingleCoil(3, false);
  delay(50);
  Serial.print("Resultado: "); Serial.println(result,HEX);
  delay(500);
  result = node.writeSingleCoil(3, true);
  delay(50);
  Serial.print("Resultado: "); Serial.println(result,HEX);
  delay(500);*/
  result = node.readInputRegisters(0,1);
  if (result == node.ku8MBSuccess) {
    uint16_t sensor = node.getResponseBuffer(0);
    //tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setTextSize(5);
    tft.fillRect(100,120,150,40,TFT_BLACK);
    tft.drawNumber(sensor, 100, 120);
  }
  delay(200);
}
