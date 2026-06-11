#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RX_PIN A0
#define BIT_DELAY 120
#define THRESHOLD 700
#define MIN_START_TIME (BIT_DELAY * 2)
#define IDLE_TIMEOUT 400

LiquidCrystal_I2C lcd(0x27,16,2);

bool receiving = false;

// ===== RECEIVE BYTE ===== SUBSCRIBE REGITE YOUTUBE 
byte receiveByte(){

  // Wait for start HIGH | SUBSCRIBE REGITE YOUTUBE 
  while(analogRead(RX_PIN) < THRESHOLD);

  delay(BIT_DELAY + BIT_DELAY/2);

  byte data = 0;

  for(int i=0;i<8;i++){
    if(analogRead(RX_PIN) > THRESHOLD)
      data |= (1<<i);
    delay(BIT_DELAY);
  }

  delay(BIT_DELAY); // Stop bit delay | SUBSCRIBE REGITE YOUTUBE 

  return data;
}

void setup(){

  Wire.begin();
  lcd.init();
  lcd.backlight();
  pinMode(RX_PIN,INPUT);

  lcd.print(" REGITE YOUTUBE ");
  delay(1500);
  lcd.clear();
}

void loop(){

  static unsigned long highStart = 0;
  static bool waitingForHigh = true;

  int signal = analogRead(RX_PIN);

  // ---- Detect Stable HIGH ---- SUBSCRIBE REGITE YOUTUBE 
  if(signal > THRESHOLD){

    if(waitingForHigh){
      highStart = millis();
      waitingForHigh = false;
    }

    if(millis() - highStart > MIN_START_TIME){

      waitingForHigh = true;
      receiving = true;

      String message = "";

      lcd.clear();
      lcd.print(" DECODING...");

      unsigned long lastBit = millis();

      while(receiving){

        if(analogRead(RX_PIN) > THRESHOLD){

          byte c = receiveByte();
          message += (char)c;
          lastBit = millis();
        }

        // Stop if no signal for some time | SUBSCRIBE REGITE YOUTUBE 
        if(millis() - lastBit > IDLE_TIMEOUT){
          receiving = false;
        }
      }

      lcd.clear();
      lcd.print(" RECEIVED DATA ");
      lcd.setCursor(0,1);
      lcd.print(message.substring(0,16));

      delay(2000);
      lcd.clear();
    }
  }
  else{
    waitingForHigh = true;
  }
}
