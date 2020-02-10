#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DEBUG false

// GPIO pins
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

#define timeDiff 5 //hours

const int hours[] = {D9, D1, D2, D3};
const int minutes[] = {D8, D5, D6, D7};
int lastMinute;

void setup() {
  initWifi();
  pinMode(D9, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  digitalWrite(D9, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D8, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  while(DEBUG) {
    test();
  }
}

void loop() {
  int minute = getMinutes();
  if(minute == -1) {
    delay(500);
    return;
  }
  int hour = (timeDiff + (minute / 60)) % 12; //only care about 12 hr cycle
  int fiveMinSeg = (minute % 60) / 5;

  Serial.print(hour);
  Serial.print(", ");
  Serial.println(fiveMinSeg);

  if (minute != lastMinute) {
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 12; j++) {
        set((j+6)%12, hours);
        set(j, minutes);
        delay(40);
      }
    }
  }
  lastMinute = minute;

  set(hour, hours);
  set(fiveMinSeg, minutes);
  
  delay(500);
}

void set(int val, const int pins[4]) {
  //0000,0001,0010,...
  for(int i = 0; i < 4; i++)
  {
    if(val%2 == 1)
    { onn(pins[i]); }
    else
    { off(pins[i]); }
    val = val/2;
  }
}

void off(const int pinNum) {
  digitalWrite(pinNum, LOW);
}
void onn(const int pinNum) {
  digitalWrite(pinNum, HIGH);
}

void test() {
  for(int i = 0; i < 12; i++) {
    Serial.println(i);
    set(i, hours);
    //set(i, minutes);
    set((i+6)%12, minutes);
    delay(1500);
  }
}
