#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ModbusMaster.h>

#define SLAVE1_ID 0x01
#define SLAVE2_ID 0x02
#define CTRL_PIN PIN_D0
#define BAUDRATE 115200

#define smplTm    200   //ms

//#define DEBUG

TFT_eSPI tft = TFT_eSPI();
ModbusMaster node1;
ModbusMaster node2;

bool state = true;
long lstTm = 0;
long currTm = 0;

union unFlt {
  float flt;
  int16_t int16[2];
} ufl;

void preTransmission()
{
  digitalWrite(CTRL_PIN, 1);
}

void postTransmission()
{
  digitalWrite(CTRL_PIN, 0);
}

void showSensVal(float valkPa, int32_t poX, int32_t poY){
  if (valkPa == NULL) {
    tft.setTextSize(2);
    tft.fillRect(poX+25, poY, 116, 55, TFT_BLACK);
    tft.drawString("No Disp", poX+25, poY + 25);
    delay(50);
  } else {
    float valinh2o = valkPa * 4.01865;
    //tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setTextSize(1);
    tft.drawString("in",poX+3,poY+5);
    tft.drawString("h2o",poX,poY+15);
    tft.drawString("kPa",poX,poY+43);
    tft.setTextSize(4);
    tft.fillRect(poX+25, poY, 116, 55, TFT_BLACK);
    tft.drawFloat(valinh2o, 2, poX+25, poY);
    tft.setTextSize(2);
    tft.drawFloat(valkPa, 3, poX+25, poY+40);
    //tft.drawNumber(sensor, poX, poY);
    delay(50);
  }
}

float getSensValKpa(ModbusMaster node, uint8_t sensNum){
  uint16_t reg = 0;
  if (sensNum == 0){
    reg = 0x00DA;
  } else if (sensNum == 1){
    reg = 0x00DC;
  }
  uint16_t result = node.readInputRegisters(reg,2);
  if (result == node.ku8MBSuccess) {
    ufl.int16[0] = node.getResponseBuffer(0);
    ufl.int16[1] = node.getResponseBuffer(1);
    float valkPa = ufl.flt;                           // Value in kPa
    return valkPa;
  } else {
    return NULL;
  }
}

void setup() {
  // Modbus setup
  pinMode(CTRL_PIN, OUTPUT);
  Serial.begin(BAUDRATE);
  node1.begin(SLAVE1_ID, Serial);
  node1.preTransmission(preTransmission);
  node1.postTransmission(postTransmission);
  //node2.begin(SLAVE2_ID, Serial);
  //node2.preTransmission(preTransmission);
  //node2.postTransmission(postTransmission);
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
  currTm = millis();
  if (abs(currTm - lstTm) >= smplTm){
    showSensVal(getSensValKpa(node1,0), 10, 100);
    showSensVal(getSensValKpa(node1,1), 170, 100);
    lstTm = currTm;
  }
}
