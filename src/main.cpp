#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ModbusMaster.h>

#define SLAVE1_ID 0x01
#define SLAVE2_ID 0x02
#define CTRL_PIN PIN_D0
#define BAUDRATE 115200

#define DEBUG

TFT_eSPI tft = TFT_eSPI();
ModbusMaster node1;
ModbusMaster node2;

bool state = true;

void preTransmission()
{
  digitalWrite(CTRL_PIN, 1);
}

void postTransmission()
{
  digitalWrite(CTRL_PIN, 0);
}

void getSensVal(ModbusMaster node){
  uint16_t result = node.readInputRegisters(0,1);
  if (result == node.ku8MBSuccess) {
    uint16_t sensor = node.getResponseBuffer(0);
    //tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setTextSize(5);
    tft.fillRect(200,120,150,40,TFT_BLACK);
    tft.drawNumber(sensor, 200, 120);
    delay(50);
  }
}

void setup() {
  // Modbus setup
  pinMode(CTRL_PIN, OUTPUT);
  Serial.begin(BAUDRATE);
  node1.begin(SLAVE1_ID, Serial);
  node1.preTransmission(preTransmission);
  node1.postTransmission(postTransmission);
  node2.begin(SLAVE2_ID, Serial);
  node2.preTransmission(preTransmission);
  node2.postTransmission(postTransmission);
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
  //uint8_t result;
  /*result = node1.writeSingleCoil(3, false);
  delay(50);
  Serial.print("Resultado: "); Serial.println(result,HEX);
  delay(500);
  result = node1.writeSingleCoil(3, true);
  delay(50);
  Serial.print("Resultado: "); Serial.println(result,HEX);
  delay(500);*/
  getSensVal(node1);
  getSensVal(node2);
  delay(200);
}
