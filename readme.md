# OpenTONY
Tunes Only Now Yours

![OpenTONY music box](./img/musicbox.png)

## Hardware
- ESP32 based audio board [ESP32-Audio-Kit from Ai Thinker](https://docs.ai-thinker.com/en/esp32-audio-kit), [schematic](https://docs.ai-thinker.com/_media/esp32-audio-kit_v2.2_sch.pdf). The module on the board is a [ESP32-A1S Audio module](https://docs.ai-thinker.com/en/esp32-a1s)
- 4Ω3W speaker (x2)
- 1S LiIon Aku
- rfid reader
- accelerometer (t.b.d)
- LED (WS2812ish)
- 2 buttons (volume up & down)

free pins (potentianly)
P1
- IO15 (JT_MTDO, SD_CMD)
- IO13 (JT_MTCK, SD card... maybe also in 1 bit mode)
- IO12 (SD2)
- IO14 (SD_CLK)

P2  
- IO21 not free (AMP enable)
- IO22 (connected to LED)
- IO19 (connected to LED and KEY3)
- IO23 (KEY4)
- IO18 (KEY5)
- IO5 (KEY6 ESP32 thing...?)

### SD Card
- up to 64GB, has to be FAT formatted.
- Current S1 DIP switch selections: 0 1 1 0 0, which also works for 4bit SD mode (using SDMMC library)
- https://github.com/pschatzmann/arduino-audiokit/wiki/Problems-with-the-SD-Drive-or-some-conflicting-Buttons
- https://github.com/espressif/arduino-esp32/tree/master/libraries/SD_MMC

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
- [ ] rfid over vspi - connected to P2 pins
- [ ] rfid over i2c?
- [ ] secondary board - via uart0, reading spi rfid reader, imu, LED, akku voltage reader, a few buttons...
- [X] playlists
- [ ] upload files
- [ ] auto limit amount of playtime per day
- [ ] Li Aku saver
- [ ] upload files over web

### code snippets
vspi = new SPIClass(VSPI);
vspi.begin(VSPI_CLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);