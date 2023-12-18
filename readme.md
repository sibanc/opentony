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

### SD Card
up to 64GB, has to be FAT formatted.
Current S1 DIP switch selections: 0 1 1 0 0, which also works for 4bit SD mode (using SDMMC library)
https://github.com/pschatzmann/arduino-audiokit/wiki/Problems-with-the-SD-Drive-or-some-conflicting-Buttons
https://github.com/espressif/arduino-esp32/tree/master/libraries/SD_MMC

## Firmware
based on
- https://github.com/pschatzmann/arduino-audiokit
- https://github.com/pschatzmann/arduino-audio-tools


## Todos
- [ ] rfid
- [ ] playlists
- [ ] upload files
- [ ] auto limit amount of playtime per day
- [ ] Li Aku saver

