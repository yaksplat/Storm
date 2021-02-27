#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>



#include "colors.h"
#include "config.h"

Adafruit_NeoPixel strip_S(LED_COUNT_S, LED_PIN_S, NEO_GRBW + NEO_KHZ800);

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

bool musicPlaying = false;
colors c;
void setup() {
  Serial.begin(115200);

  // if you're using Bluefruit or LoRa/RFM Feather, disable the radio module
  //pinMode(8, INPUT_PULLUP);

  // Wait for serial port to be opened, remove this line for 'standalone' operation
  while (!Serial) { delay(1); }
  delay(500);
  Serial.println("\n\nAdafruit VS1053 Feather Test");
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

  Serial.println(F("VS1053 found"));
 
  //musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
   while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10,10);
  
#if defined(__AVR_ATmega32U4__) 
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  // but we don't have them on the 32u4 feather...
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
#else
  // If DREQ is on an interrupt pin we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
#endif

  strip_S.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_S.show();            // Turn OFF all pixels ASAP
  strip_S.setBrightness(BRIGHTNESS);
//   pinMode(buttonPin, INPUT);
  // Play a file in the background, REQUIRES interrupts!
 // Serial.println(F("Playing full track 001"));
 // musicPlayer.playFullFile("/THUNDER/03_27.MP3");
 // musicPlaying = true;
  //Serial.println(F("Playing track 002"));
  //musicPlayer.startPlayingFile("/Cannibal.mp3");
    // musicPlayer.startPlayingFile("/track003.mp3");
      // Play a file in the background, REQUIRES interrupts!
  Serial.println(F("Playing full track 001"));
  musicPlayer.playFullFile("/THUNDER/01_03.MP3");

 // Serial.println(F("Playing track 002"));
 // musicPlayer.startPlayingFile("/THUNDER/03_27.MP3");
}



void loop()
{
   // setStrip(strip_S, 0,0,0,0);
//delay(150);
  //musicPlaying = true;
   if (musicPlayer.stopped()) {
    Serial.println(F("music has stopped"));
    musicPlaying=false;
     musicPlayer.playFullFile("/THUNDER/03_27.MP3");
     musicPlayer.playFullFile("/THUNDER/02_06.MP3");
     musicPlayer.playFullFile("/THUNDER/02_09.MP3");
     musicPlayer.playFullFile("/THUNDER/02_11.MP3");
     musicPlayer.playFullFile("/THUNDER/02_18.MP3");
     musicPlayer.playFullFile("/THUNDER/02_122.MP3");
     musicPlayer.playFullFile("/THUNDER/03_03.MP3");
     musicPlayer.playFullFile("/THUNDER/03_15.MP3");
     musicPlayer.playFullFile("/THUNDER/03_19.MP3");
     musicPlayer.playFullFile("/THUNDER/04_07.MP3");
     musicPlayer.playFullFile("/THUNDER/04_17.MP3");
     musicPlayer.playFullFile("/THUNDER/04_23.MP3");
     musicPlayer.playFullFile("/THUNDER/04_62.MP3");
     musicPlayer.playFullFile("/THUNDER/05_08.MP3");
     musicPlayer.playFullFile("/THUNDER/05_11.MP3");
     musicPlaying=true;
   }
 // setStrip(strip_S, c.LateNight);
delay(150);
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void setStrip(Adafruit_NeoPixel _strip, const uint32_t color) {
  byte white = (color >> 24) & 0xff;
  byte red = (color >> 16) & 0xff;
  byte blue = (color >> 8) & 0xff;
  byte green = color & 0xff;
  setStrip(_strip, red, green, blue, white);
}



void setStrip(Adafruit_NeoPixel _strip, byte red, byte green, byte blue, byte white) {
  for (int pixel = 0; pixel < _strip.numPixels() - 1; pixel++) {
    _strip.setPixelColor(pixel, red, blue, green, white);
  }
  _strip.show(); // Output to strip
}
