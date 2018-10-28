#include <ST7032.h>
#include <ArduinoESPAT.h>
#include <Wire.h>

#define CONTROLL1 6
#define DEFAULTSPEED 255

SoftwareSerial softSerial(5, 4); //rx , tx
ESPAT espat(xxxx, xxxx);
ST7032 lcd;

boolean status1 = HIGH;
boolean status2 = LOW;

void writeChar(uint8_t x, uint8_t y, char c){
  lcd.setCursor(x, y);
  lcd.print(c);
}

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

void go_Fast(){
  speedControll("255");
  delay(3000);
  stop();
}

void go(String key, String data){
  if(key == "speed" && data != ""){
    speedControll(data);
  }else{
    speedControll("180"); 
  }
  delay(3000);
  stop();
}

void stop(){
  digitalWrite(CONTROLL1, LOW);
}

void say(String key, String data){
  Serial.println("say");
  softSerial.listen();
  if(key == "mes" && data != ""){
    softSerial.println(data + "\r");  
  }else{
    softSerial.println(String("tesuto") + "\r"); 
  }
}

void face(){
  writeChar(2, 0, 'o');
  writeChar(5, 0, 'o');
  writeChar(3, 1, '[');
  writeChar(4, 1, ']');
}

void opened(){
  if(espat.clientIP() == NOIP || espat.clientIP() != ""){ 
    Serial.println("Server Opened At: " + espat.clientIP());
    Serial.println("設定完了");
    softSerial.listen();
    softSerial.println("setteikannryou\r");
    Serial.println("OK");
  }else{
    Serial.println("設定失敗");
    softSerial.listen();
    softSerial.println("setteisippai\r"); 
  }
}
 
void setup() {
  pinMode(CONTROLL1, OUTPUT);
  Serial.begin(115200);
  softSerial.listen();
  softSerial.begin(9600);
  softSerial.print("?");
  lcdInitialize();
  delay(500);

  if(espat.begin()){
    Serial.println("Initialize Ok");
  }else{
    Serial.println("Initialize Fail");
  }

  espat.setGetRecieveEvents("/go_fast", "Moving at Fast(PWM255)", go_Fast);
  espat.setGetRecieveEvents("/go", "Moving at PWM180", go);
  espat.setGetRecieveEvents("/say", "Talking", say);
  espat.setGetRecieveEvents("/face", "Face changed", face);

  if(!espat.openServer(88, opened)){
    Serial.println("fail");
  }
}

void loop(){
}
