#include <SoftwareSerial.h>
#include <ST7032.h>
#include <Wire.h>

#define CONTROLL1 6
#define DEFAULTSPEED 255

SoftwareSerial softSerial(3, 2); //rx , tx
ST7032 lcd;

boolean status1 = HIGH;
boolean status2 = LOW;

void lcdInitialize(){
  lcd.begin(8, 2);
  lcd.setContrast(5);

  writeChar(2, 0, 'o');
  writeChar(5, 0, 'o');
  writeChar(3, 1, '-');
  writeChar(4, 1, '-');
}

void writeChar(uint8_t x, uint8_t y, char c){
  lcd.setCursor(x, y);
  lcd.print(c);
}

void setup() {
  pinMode(CONTROLL1, OUTPUT);
  Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.print("?");
  lcdInitialize();
  delay(500);

  Serial.println("設定完了");
  softSerial.println("setteikannryou");
}

void loop() {
  int sound = analogRead(0);

  if (sound > 300) {
      analogWrite(6, 255);
      delay(2000);
      analogWrite(6, 0);
    }
  }
}
g
