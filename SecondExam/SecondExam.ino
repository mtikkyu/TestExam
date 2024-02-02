#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define sw1 8
#define buzzer 7

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(10, 9);

String strID = "";


uint8_t step_cmd[4][4] = {{1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

#define IN1 6
#define IN2 5
#define IN3 4
#define IN4 3

#define ONE_ROUND 510 // 510 step 

int32_t count_enc = 0;

void setup() {
  lcd.begin();
  lcd.backlight();

  SPI.begin();
  rfid.PCD_Init();
  pinMode(sw1, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
}

int16_t set_step = 0;
String input_string[2];

bool state_sw = 0;
bool pre_state_sw = 0;

bool state_count_up = 0;

void loop() {
  if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    strID = "";
    for(uint8_t i = 0; i < 4; i++) {
      strID += (rfid.uid.uidByte[i]<0x10?"0":"") + String(rfid.uid.uidByte[i], HEX) + (i != 3?":":"");
    }
    strID.toUpperCase();
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("ID = " + strID);
    digitalWrite(buzzer, 1); delay(300); digitalWrite(buzzer, 0);
    delay(1000);

    if(digitalRead(sw1) == 0) {
      lcd.setCursor(0, 1);
      if(strID == "09:14:2B:01") {
        lcd.print("ID=OK");
        setStep(125);
      }
      else lcd.print("ID=ERROR");
      while(digitalRead(sw1) == 0); 
      setStep(-125);
    }
  }
}

void setStep(int32_t _step) {
  for (uint16_t count_step = 0; count_step < abs(_step); count_step++) {
    for (uint16_t run_step = 0; run_step < 4; run_step++) {
      if (_step > 0) {
        digitalWrite(IN1, step_cmd[run_step][0]);
        digitalWrite(IN2, step_cmd[run_step][1]);
        digitalWrite(IN3, step_cmd[run_step][2]);
        digitalWrite(IN4, step_cmd[run_step][3]);
      }
      else if (_step < 0) {
        digitalWrite(IN4, step_cmd[run_step][0]);
        digitalWrite(IN3, step_cmd[run_step][1]);
        digitalWrite(IN2, step_cmd[run_step][2]);
        digitalWrite(IN1, step_cmd[run_step][3]);
      }
      delay(5);
    }
  }
}
