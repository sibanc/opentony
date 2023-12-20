# OpenTONY
Tunes Only Now Yours

![OpenTONY music box](./img/musicbox.png)

## Hardware
- Ai Thinker ESP32 based audio board [ESP32-Audio-Kit from Ai Thinker](https://docs.ai-thinker.com/en/esp32-audio-kit), [schematic](https://docs.ai-thinker.com/_media/esp32-audio-kit_v2.2_sch.pdf). The module on the board is a [ESP32-A1S Audio module](https://docs.ai-thinker.com/en/esp32-a1s)
    * Code tested on version V2.2 A247. Modify compile flag -DAUDIOKIT_BOARD for different version as described [here](https://github.com/pschatzmann/arduino-audiokit/wiki/PlatformIO)
- 4Ω3W speaker (x2)
- 1S LiIon Aku
- RFID reader (RC522)
- Accelerometer (t.b.d)
- LED (WS2812ish)
- 2 buttons (volume up & down)

### SD Card
- up to 64GB, has to be FAT formatted.
- Current S1 DIP switch selections: 0 1 1 0 0, which also works for 4bit SD mode (using SDMMC library)
- https://github.com/pschatzmann/arduino-audiokit/wiki/Problems-with-the-SD-Drive-or-some-conflicting-Buttons
- https://github.com/espressif/arduino-esp32/tree/master/libraries/SD_MMC

### Wiring

#### SD Card (Multidrop SPI Bus)

| Description  | #define in audiokit.h      | ESP32 Module | Ai Thinker Schematics |
| ------------ | -------------------------- | ------------ | --------------------- |
| SD CARD CLK  | PIN_AUDIO_KIT_SD_CARD_CLK  | GPIO 14      | SD CLK                |
| SD CARD MISO | PIN_AUDIO_KIT_SD_CARD_MISO | GPIO 12      | SD DATA2              |
| SD CARD MOSI | PIN_AUDIO_KIT_SD_CARD_MOSI | GPIO 15      | SD CMD                |
| SD CARD SS   | PIN_AUDIO_KIT_SD_CARD_CS   | GPIO 13      | SD DATA3              |

#### RC522 Breakout Board (Multidrop SPI Bus)

| Description  | #define in main.cpp        | ESP32 Module |
| ------------ | -------------------------- | ------------ |
| RFID    CLK  | RFID_CLK                   | P1 MTCK      |
| RFID    MISO | RFID_MISO                  | P1 MTDO      |
| RFID    MOSI | RFID_MOSI                  | P1 MTDI      |
| RFID    SS   | RFID_SS                    | P2 GPIO 21   |
| RFID    RST  | RFID_RST                   | P3 RST       |

## Firmware
Currently the board can be controlled using Serial commands (end witn \n):  
+,- for volume up and down  
N,P for next and previous song  
D to display directories and files  
L to load mp3s from a directory, i.e. a "playlist"  

Based on:  
- https://github.com/pschatzmann/arduino-audiokit
- https://github.com/pschatzmann/arduino-audio-tools

## Todos
- [ ] rfid
- [X] playlists
- [ ] upload files
- [ ] auto limit amount of playtime per day
- [ ] Li Aku saver
- [ ] upload files over web
