// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"


// Init constants
const int tempPin = A0;
const int buttonPin = 0;
const int lamPin = 13;

// Init global variables

// construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);




void setup() {
  
  Serial.begin(9600);
  pinMode(lamPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  u8g.setFont(u8g_font_unifont);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {

  oledWrite(10, 20, getTime(), 30, 50, String(getTemp()));
  delay(200);
  Serial.println(getTemp());
  if (getTemp() > 28) {
     digitalWrite(lamPin, HIGH);
  } else {
     digitalWrite(lamPin, LOW);
  }

  if(digitalRead(buttonPin) == 1){
    digitalWrite(lamPin, LOW);
  }

}


String getTime() {
  DateTime now = rtc.now();
  return "Time:" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

float getTemp(){

  int Vo;
  float R1 = 10000;
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;


  Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0); 
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;
  return T;
}


void oledWrite(int x, int y, String text, int x2, int y2, String text2) {
  u8g.firstPage();
  do {
    u8g.drawStr(x, y, text.c_str());
    u8g.drawStr(x2, y2, text2.c_str());
  } while (u8g.nextPage());
}

