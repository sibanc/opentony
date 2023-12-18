/**
 * @file player-sd-audiokit.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-audiokit/player-sdmmc-audiokit/README.md
 * Make sure that the pins are set to on, on, on, on, on
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioLibs/AudioSourceIdxSDMMC.h" // or AudioSourceIdxSDMMC.h
#include "AudioCodecs/CodecMP3Helix.h"

const char *startFilePath="/";
const char* ext="mp3";
AudioSourceIdxSDMMC source(startFilePath, ext);
AudioKitStream kit;
MP3DecoderHelix decoder;  // or change to MP3DecoderMAD
AudioPlayer player(source, kit, decoder);


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

int volume = 7;

void volume_up(bool, int, void*){
  if(volume < 10){
    volume++;
    player.setVolume(volume/10.0);
  }
}

void volume_down(bool, int, void*){
  if(volume > 0){
    volume--;
    player.setVolume(volume/10.0);
  }
}

void next(bool, int, void*) {
   player.next();
}

void previous(bool, int, void*) {
   player.previous();
}

void startStop(bool, int, void*) {
   player.setActive(!player.isActive());
}

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  cfg.sd_active = false;
  kit.begin(cfg);

 // setup additional buttons 
  kit.addAction(PIN_KEY1, startStop);
  kit.addAction(PIN_KEY4, next);
  kit.addAction(PIN_KEY3, previous);
  kit.addAction(PIN_KEY5, volume_down);
  kit.addAction(PIN_KEY6, volume_up);

  // setup player
  player.setVolume(volume/10.0);
  player.begin();

  // select file with setPath() or setIndex()
  //player.setPath("/ZZ Top/Unknown Album/Lowrider.mp3");
  player.setIndex(0); // 2nd file

  listDir(SD_MMC,"/",1);

}

void loop() {
  player.copy();
  kit.processActions();
}