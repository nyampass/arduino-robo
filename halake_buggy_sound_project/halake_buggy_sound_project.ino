#include <SoftwareSerial.h>
#include <ST7032.h>
#include <Wire.h>

#define CONTROLL1 6
#define DEFAULTSPEED 255
#define DEFAULT_SOUND_THRESHOLD 300 // 1 to 1023

SoftwareSerial softSerial(3, 2); //rx , tx
ST7032 lcd;

boolean status1 = HIGH;
boolean status2 = LOW;

void speedControll(String value){
  if(value != ""){
    analogWrite(CONTROLL1, value.toInt());
  }else{
    analogWrite(CONTROLL1, DEFAULTSPEED); 
  }
}

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

String split(String data, char separator, int index){
  int maxIndex = data.length() - 1;
  int j = 0;
  String chunkVal = "";

  for(int i = 0; i <= maxIndex && j <= index; i++){
    chunkVal.concat(data[i]);

    if (data[i] == separator){
        j++;

        if (j > index){
            chunkVal.trim();
            return chunkVal;
        }
        chunkVal = "";
    }
    
    else if ((i == maxIndex) && (j < index)){
        chunkVal = "";
        return chunkVal;
    }
  }   
}

void setup() {
  pinMode(CONTROLL1, OUTPUT);
  Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.print("?");
  lcdInitialize();
  delay(500);

  Serial.println("設定完了");
  softSerial.println("setteikannryou\r");
}

void loop(){
  if(Serial.available() <= 0){
    int soundSenceValue = analogRead(0);

    if(soundSenceValue > DEFAULT_SOUND_THRESHOLD){
      digitalWrite(CONTROLL1, HIGH);
      speedControll("");
      delay(2000);
      digitalWrite(CONTROLL1, LOW);
    }
  }else{
    String serialStr = Serial.readStringUntil('\r');

    if (serialStr != "") {
      String selector = split(serialStr, ' ', 0);
      String param = split(serialStr, ' ', 1);
    
      if(selector == "go"){
        digitalWrite(CONTROLL1, HIGH);
        speedControll(param);
      }else if(selector == "stop"){
        digitalWrite(CONTROLL1, LOW);
      }else if(selector == "say"){
        Serial.println("say");
        if(param != ""){
          softSerial.println(param + "\r"); 
        }else{
          softSerial.println("\r");
        }
      }else if(selector == "face"){ 
        writeChar(2, 0, 'o');
        writeChar(5, 0, 'o');
        writeChar(3, 1, '[');
        writeChar(4, 1, ']');
      }else{
        Serial.println("unknown command");
      }
    }   
  }
}
