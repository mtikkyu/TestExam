#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;
  // Serial.print("PICC type: ");

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522:: PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522:: PICC_TYPE_MIFARE_1K   &&
      piccType != MFRC522:: PICC_TYPE_MIFARE_4K)
  {
    Serial.println("Your tag is not of type MIFARE Classic."); return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++)
  {
    strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i != 3 ? ":" : "");
  }
  strID.toUpperCase();

  Serial.print("Tap card key: ");
  Serial.print(strID);

  //if (strID.indexOf("2C:FA:54:E9") >= 0)
  if (strID >= "0")
  {
    Serial.println(" Tag OK!!");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
