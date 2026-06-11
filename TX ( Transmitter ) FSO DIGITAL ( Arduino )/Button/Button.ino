#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define LASER_PIN 13
#define BIT_DELAY 120
#define DISPLAY_TIMEOUT 15000 
#define INPUT_DELAY 300

SoftwareSerial BT(10,11);
LiquidCrystal_I2C lcd(0x27,16,2);

unsigned long lastActivity=0, lastCharTime=0, titleTimer=0, roleTimer=0;
bool backlightState=true, roleState=true;
String buffer="";

// ===== GLITCH TITLE ANIMATION ===== SUBSCRIBE REGITE YOUTUBE 
void glitchTitle(){
  String text = " REGITE YOUTUBE ";
  String glitchChars = "!@#$%^&*<>?/0123456789";

  for(int g=0; g<5; g++){
    lcd.setCursor(0,0);
    for(int i=0;i<16;i++){
      if(i < text.length()){
        if(random(0,3)==0)
          lcd.print(glitchChars[random(glitchChars.length())]);
        else
          lcd.print(text[i]);
      } else {
        lcd.print(" ");
      }
    }
    delay(70);
  }
  lcd.setCursor(0,0);
  lcd.print(" REGITE YOUTUBE  ");
}

// Bit manipulation function ( no extra delay ) | SUBSCRIBE REGITE YOUTUBE 

void sendByteInternal(byte data) {
  digitalWrite(LASER_PIN, HIGH); delay(1); 
  for(int i=0; i<8; i++) {
    digitalWrite(LASER_PIN, data & (1<<i));
    delay(1); 
  }
  digitalWrite(LASER_PIN, LOW); delay(1);
}

void sendBit(bool b){
  digitalWrite(LASER_PIN, b ? HIGH : LOW);
  delay(BIT_DELAY);
}

void sendByte(byte data){
  sendBit(1);
  for(int i=0; i<8; i++) sendBit(data & (1<<i));
  sendBit(0);
}

// ===== FAST LOOPING SCROLL DURING TRANSMISSION ===== SUBSCRIBE REGITE YOUTUBE 

void scrollDuringTransmit(String msg) {
  int msgLen = msg.length();
  String scrollMsg = msg + "     "; 
  int scrollPos = 0;

  sendByte((byte)msgLen); 
  delay(BIT_DELAY); 

  for(int i=0; i<msgLen; i++) {
    if (msgLen > 16) {
      for (int step = 0; step < 6; step++) {
        lcd.setCursor(0, 1);
        lcd.print(scrollMsg.substring(scrollPos, scrollPos + 16));
        scrollPos++;
        if (scrollPos > scrollMsg.length() - 16) scrollPos = 0;
        delay((BIT_DELAY * 10) / 6); 
      }
      sendByteInternal(msg[i]); 
    } else {
      int spaces = (16 - msgLen) / 2;
      lcd.setCursor(0, 1);
      for(int s=0; s<16; s++) lcd.print(" ");
      lcd.setCursor(spaces, 1);
      lcd.print(msg);
      sendByte(msg[i]); 
    }
  }
}

void typeCentered(int row, String text) {
  if (text.length() > 16) text = text.substring(0, 16);
  int spaces = (16 - text.length()) / 2;
  lcd.setCursor(0, row);
  for (int i = 0; i < 16; i++) lcd.print(" ");
  lcd.setCursor(spaces, row);
  for (int i = 0; i < text.length(); i++) {
    lcd.print(text[i]);
    delay(35);
  }
}

void showHome(){
  if(millis() - titleTimer > 2500){ 
    titleTimer = millis(); 
    glitchTitle(); 
  }
  if(millis() - roleTimer > 2000){
    roleTimer = millis();
    roleState = !roleState;
    lcd.setCursor(0,1);
    if(roleState) lcd.print("  SYSTEM READY   ");
    else          lcd.print("  TRANSMITTER    ");
  }
}

void transmitMessage(String msg){
  msg.trim();
  if(msg.length() == 0) return;
  lastActivity = millis();
  if(!backlightState) { lcd.backlight(); backlightState = true; }

  float estTime = (msg.length() * 10.0 * BIT_DELAY) / 1000.0;
  lcd.clear();
  typeCentered(0, "PROCESSING");
  lcd.setCursor(0,1);
  lcd.print(" EST TIME: "); lcd.print(estTime, 1); lcd.print("s");
  delay(2000); 

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  TRANSMITTING");
  
  scrollDuringTransmit(msg);

  digitalWrite(LASER_PIN, LOW);
  delay(300);
  lcd.clear();
  typeCentered(0, "TRANSMISSION");
  typeCentered(1, "SUCCESSFUL");
  delay(1200);
  lcd.clear();
}

void checkInput(){
  while(BT.available()){ char c = BT.read(); lastCharTime = millis(); if(c != '\r' && c != '\n') buffer += c; }
  while(Serial.available()){ char c = Serial.read(); lastCharTime = millis(); if(c != '\r' && c != '\n') buffer += c; }
  if(buffer.length() > 0 && millis() - lastCharTime > INPUT_DELAY){ transmitMessage(buffer); buffer = ""; }
}

void setup(){
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);
  Serial.begin(9600); BT.begin(9600);
  lcd.init(); lcd.backlight();
  glitchTitle();
  lastActivity = millis();
}

void loop(){
  if(buffer.length() == 0) showHome();
  checkInput();
}