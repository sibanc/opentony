/**
 * Baseline script to test SPI functionality with RFID chip
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/SPI/examples/SPI_Multiple_Buses/SPI_Multiple_Buses.ino
 */

#include <Arduino.h>
#include <SPI.h>

// Test 1:
// No defines // RFID on VSPI Pins // MISO:19:IO19, MOSI:23:IO23, SCK:18:IO18, SS:5:IO5 -- Result: Had to remove chip prior to flashing. No success with RFID readout.
// USE_HSPI // RFID on HSPI Pins // MISO:12:MTDI, MOSI:13:MTCK, SCK:14:MTMS, SS:15:MTDO -- Result: No success with RFID module
// ALTERNATE_PINS // RFID on VSPI Pins // MISO:21:IO21, MOSI:22:IO22, SCK:19:IO19, SS:23:IO23 -- Result: No success with RFID module

#define USE_HSPI

// Define ALTERNATE_PINS to use non-standard GPIO pins for SPI bus
#ifdef ALTERNATE_PINS
  #define VSPI_MISO   21
  #define VSPI_MOSI   22
  #define VSPI_SCLK   19
  #define VSPI_SS     23

  #define HSPI_MISO   26
  #define HSPI_MOSI   27
  #define HSPI_SCLK   25
  #define HSPI_SS     32
#else
  #define VSPI_MISO   MISO
  #define VSPI_MOSI   MOSI
  #define VSPI_SCLK   SCK
  #define VSPI_SS     SS

  #define HSPI_MISO   2
  #define HSPI_MOSI   15
  #define HSPI_SCLK   14
  #define HSPI_SS     12
#endif

#ifdef USE_HSPI
  #define SS_PIN    HSPI_SS
#else
  #define SS_PIN    VSPI_SS
#endif

#define RST_PIN   -1 // Connected to HW RST Button

#define MFRC522_SPICLOCK (1000000u)
#include <MFRC522.h>

static const int spiClk = MFRC522_SPICLOCK; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;

// RFID instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(115200);

  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);
  hspi = new SPIClass(HSPI);
  
  //clock miso mosi ss

#ifndef ALTERNATE_PINS
  //initialise vspi with default pins
  //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  vspi->begin();
#else
  //alternatively route through GPIO pins of your choice
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS); //SCLK, MISO, MOSI, SS
#endif

#ifndef ALTERNATE_PINS
  //initialise hspi with default pins
  //SCLK = 14, MISO = 12, MOSI = 13, SS = 15
  hspi->begin();
#else
  //alternatively route through GPIO pins
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS
#endif

  //set up slave select pins as outputs as the Arduino API
  //doesn't handle automatically pulling SS low
  pinMode(vspi->pinSS(), OUTPUT); //VSPI SS
  pinMode(hspi->pinSS(), OUTPUT); //HSPI SS

  // setup rfid module
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