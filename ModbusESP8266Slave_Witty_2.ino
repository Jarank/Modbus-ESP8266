#include <ESP8266WiFi.h>
#include <ModbusTCPSlave.h>

////https://forums.adafruit.com/viewtopic.php?f=31&t=61774&sid=06d9885dcf852259d1f568fa96e40265&start=15

////WIFI Settings
///*
byte ip[]      = { 192, 168, 2, 201};
byte gateway[] = { 192, 168, 2, 1 };
byte subnet[]  = { 255, 255, 255, 0 };
//*/
ModbusTCPSlave Mb;

unsigned long timer;
unsigned long checkRSSIMillis;

const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;
 

byte checkRSSI() {
  byte quality;
  long rssi = WiFi.RSSI();
  if (rssi <= -100)
    quality = 0;
  else if (rssi >= -50)
    quality = 100;
  else
    rssi = rssi + 100;
  quality = byte(rssi * 2);

  return quality;
}

void setup()
{
    Serial.begin(9600);
 
    pinMode(LDR, INPUT);
    pinMode(BUTTON, INPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    
  Mb.begin("Sila2Gxxx", "0809xxxx", ip, gateway, subnet);
//  Mb.begin("1503523", "D2E7D32DBC883");

  delay(1000);
  Mb.MBInputRegister[0] = 100;
  Mb.MBInputRegister[1] = 65500;
  Mb.MBInputRegister[2] = 300;
  Mb.MBInputRegister[3] = 400;
  Mb.MBInputRegister[4] = 500;

  Mb.MBHoldingRegister[0] = 1;
  Mb.MBHoldingRegister[1] = 2;
  Mb.MBHoldingRegister[2] = 3;
  Mb.MBHoldingRegister[3] = 4;
  Mb.MBHoldingRegister[4] = 5;


  if (millis() - checkRSSIMillis >= 100) {
    checkRSSIMillis = millis();
    Mb.MBInputRegister[0] = checkRSSI();
  }
    delay(1000);
    Serial.print("LDR: ");
    Serial.println(analogRead(LDR));
    Serial.print("BUTTON: ");
    Serial.println(digitalRead(BUTTON));
    Serial.print("RSSI: ");
    Serial.println(Mb.MBInputRegister[0]);

}

void loop()
{
  Mb.Run();
  delay(10);

  if (millis() - timer >= 100) {
    timer = millis();
    Mb.MBInputRegister[1]++;
  }

  if (millis() - checkRSSIMillis >= 100) {
    checkRSSIMillis = millis();
    Mb.MBInputRegister[0] = checkRSSI();
  }
    Mb.MBInputRegister[1] = analogRead(LDR);
  //  Mb.MBInputCoil[1] = digitalRead(BUTTON);
    analogWrite(RED, Mb.MBHoldingRegister[0]);
    analogWrite(GREEN, Mb.MBHoldingRegister[1]);
    analogWrite(BLUE, Mb.MBHoldingRegister[2]);
    
    delay(20);
        
}






