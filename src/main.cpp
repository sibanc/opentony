/**
 * @file player-sd-audiokit.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-audiokit/player-sdmmc-audiokit/README.md
 * Make sure that the pins are set to on, on, on, on, on (Rok ?? off on on off off works)
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#include <Arduino.h>
#include <SPI.h>

#define MFRC522_SPICLOCK (1000000u)
#include <MFRC522.h>

#define RST_PIN   -1 // Connected to HW RST Button
#define SS_PIN    22

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(115200);
  SPI.begin();

  // setup rfid module
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, LOW);
  mfrc522.PCD_Init();
	delay(4);	// Optional delay. Some board do need time after init to be ready
	mfrc522.PCD_DumpVersionToSerial();
}

void readRFID()
{
  digitalWrite(SS_PIN, LOW);
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      Serial.print("Detected UID:");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0x0" : " 0x");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
  }
  Serial.println();

  mfrc522.PICC_HaltA();
}

void loop()
{
  readRFID();
}