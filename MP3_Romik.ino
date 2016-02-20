
#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library
#include <EEPROM.h>
#include "EEPROMAnything.h"

SdFat sd; // Create object to handle SD functions

SFEMP3Shield MP3player; // Create Mp3 library object

struct config_t
{
  unsigned int last_file_number;
} 
configuration;

unsigned long num;
char mfile[20];

SdFile file;

unsigned int total_files = 0;

void setup()
{

  Serial.begin(9600);

  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield

  sd.chdir("/");

  while (file.openNext(sd.vwd(), O_READ)) { // while file entries available
    file.close();
    total_files++;
  }

  EEPROM_readAnything(0, configuration);
 
 randomSeed(analogRead(0));

 
}

// All the loop does is continuously step through the trigger
//  pins to see if one is pulled low. If it is, it'll stop any
//  currently playing track, and start playing a new one.

void loop()
{

  if (!MP3player.isPlaying()) {
    EEPROM_readAnything(0, configuration);
    num = random(0,total_files+1); 
    while(configuration.last_file_number == num) {
      num = random(0,total_files+1);  
    }
    configuration.last_file_number = num;
    EEPROM_writeAnything(0, configuration);
    sprintf(mfile,"track%03d.mp3",num);
    MP3player.playMP3(mfile);
    Serial.println(mfile);
    while(MP3player.isPlaying());
  }

}

void initSD()
{
  sd.begin(SD_SEL, SPI_HALF_SPEED);
  sd.chdir("/");
} 

void initMP3Player()
{
  uint8_t result = MP3player.begin(); // init the mp3 player shield
  MP3player.setVolume(10, 10);
}




