#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define sw1 8
#define buzzer 7

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(10, 9);

String strID = "";

void setup() {
  lcd.begin();
  lcd.backlight();

  SPI.begin();
  rfid.PCD_Init();
  pinMode(sw1, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

}

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
      if(strID == "09:14:2B:01") lcd.print("ID=OK");
      else lcd.print("ID=ERROR");
      while(digitalRead(sw1) == 0) ;
    }
  }
}
