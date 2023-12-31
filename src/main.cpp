/**
 * @file player-sd-audiokit.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-audiokit/player-sdmmc-audiokit/README.md
 * Make sure that the pins are set to on, on, on, on, on (Rok ?? off on on off off works)
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioLibs/AudioSourceIdxSDMMC.h" // or AudioSourceIdxSDMMC.h
#include "AudioCodecs/CodecMP3Helix.h"

const char *startFilePath = "/";
const char *ext = "mp3";
AudioSourceIdxSDMMC source(startFilePath, ext);
AudioKitStream kit;
MP3DecoderHelix decoder; // or change to MP3DecoderMAD
AudioPlayer player(source, kit, decoder);

const int BUFFER_SIZE = 100;
char buf[BUFFER_SIZE];
int buf_pos = 0;

int volume = 4;

bool isDirectory(fs::FS &fs, const char *path)
{
  File root = fs.open(path);
  if (!root)
  {
    // Serial.println("Failed to open directory");
    return false;
  }
  if (!root.isDirectory())
  {
    // Serial.println("Not a directory");
    return false;
  }
  return true;
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.path(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void volume_up(bool, int, void *)
{
  if (volume < 10)
  {
    volume++;
    player.setVolume(volume / 10.0);
  }
}

void volume_down(bool, int, void *)
{
  if (volume > 0)
  {
    volume--;
    player.setVolume(volume / 10.0);
  }
}

void rfid1(bool, int, void *)
{
  source.setPath("/RFID1");
  player.begin();
}

void rfid2(bool, int, void *)
{
  source.setPath("/RFID2");
  player.begin();
}

void startStop(bool, int, void *)
{
  player.setActive(!player.isActive());
}

void setup()
{
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  cfg.sd_active = false;
  kit.begin(cfg);

  // setup additional buttons
  kit.addAction(PIN_KEY1, startStop);
  kit.addAction(PIN_KEY4, rfid1);
  kit.addAction(PIN_KEY3, rfid2);
  kit.addAction(PIN_KEY5, volume_down);
  kit.addAction(PIN_KEY6, volume_up);

  // setup player
  player.setVolume(volume / 10.0);
  player.begin();

  // select file with setPath() or setIndex()
  // player.setPath("/ZZ Top/Unknown Album/Lowrider.mp3");
  player.setIndex(0); // 2nd file
}

void playlist(const char *dir)
{
  if (isDirectory(SD_MMC, dir))
  {
    source.setPath(dir);
    player.begin();
  }
}

void processSerial()
{
  // Check if there are any bytes available in the serial receive buffer
  while (Serial.available() > 0)
  {
    // Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    if (inByte == '\r')
    {
      continue;
    }

    // If the incoming byte is a newline character, process the buffer
    if (inByte == '\n')
    {
      switch (buf[0])
      {
      case 'L':
        buf[0] = '/';
        playlist(buf);
        break;
      case 'D':
        listDir(SD_MMC, "/", 1);
        break;
      case 'P':
        player.previous();
        break;
      case 'N':
        player.next();
        break;
      case '+':
        volume_up(true, 0, nullptr);
        break;
      case '-':
        volume_down(true, 0, nullptr);
        break;

      default:
        return;
      }

      // Clear the buffer after processing
      memset(buf, 0, BUFFER_SIZE);
      buf_pos = 0; // Reset the buffer position
    }
    // If the incoming byte is not a newline character, add it to the buffer
    else
    {
      buf[buf_pos] = inByte;
      buf_pos++;
      if (buf_pos >= BUFFER_SIZE)
      {
        buf_pos = 0; // Reset the buffer position if it reaches the maximum size
      }
    }
  }
}

void loop()
{
  player.copy();
  kit.processActions();

  processSerial();
}