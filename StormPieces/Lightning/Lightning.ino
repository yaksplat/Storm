#include <Adafruit_NeoPixel.h>
#include "colors.h"

//#define LED_COUNT_S 192
//#define LED_PIN_S     5
#define BRIGHTNESS 255

#define LED_COUNT_S 192
#define LED_COUNT_E 147
#define LED_COUNT_N 147
#define LED_COUNT_W 188

#define LED_PIN_S     5
#define LED_PIN_E     6
#define LED_PIN_N     9
#define LED_PIN_W     10

Adafruit_NeoPixel strip_S(LED_COUNT_S, LED_PIN_S, NEO_GRBW + NEO_KHZ800);

colors c;

char output[128];

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  strip_S.setBrightness(BRIGHTNESS);

  strip_S.begin();
   
 // randomSeed(analogRead(0));
    for (int pixel = 0; pixel < strip_S.numPixels() - 1; pixel++) {
      strip_S.setPixelColor(pixel, c.Red);
    }
    strip_S.show();
  
}

void loop()
{
  int flashCount = random (3, 15);        // Min. and max. number of flashes each loop
  int flashBrightnessMin =  1;           // LED flash min. brightness (0-255)
  int flashBrightnessMax =  6;          // LED flash max. brightness (0-255)

  int flashDurationMin = 1;               // Min. duration of each seperate flash
  int flashDurationMax = 80;              // Max. duration of each seperate flash

  int nextFlashDelayMin = 1;              // Min, delay between each flash and the next
  int nextFlashDelayMax = 150;            // Max, delay between each flash and the next

  int thunderDelay = random (500, 3000);  // Min. and max. delay between flashing and playing sound
  int thunderFile = random (1, 17);       // There are 17 soundfiles: 0001.mp3 ... 0017.mp3
  int loopDelay = random (5000, 30000);   // Min. and max. delay between each loop
  int flashSize = random (5, 100);
  int flashLocation = random(50, 150);
  Serial.println();
  Serial.print(F("Flashing, count: "));
  Serial.println( flashCount );


  for (int flash = 0 ; flash <= flashCount; flash += 1) { // Flashing LED strip in a loop, random count
    int bright = random (flashBrightnessMin, flashBrightnessMax);
   // strip_S.setBrightness(bright);

    flashSize = flashSize + random(-10,10);
    sprintf(output, "flashLocation: %d, flashSize: %d, bright: %d", flashLocation, flashSize, bright);
    Serial.println(output);
    for (int pixel = 0; pixel < strip_S.numPixels() - 1; pixel++) {
      strip_S.setPixelColor(pixel, c.LateNight);
    }
    for (int pixel = flashLocation; pixel < flashSize+flashLocation; pixel++) {
      strip_S.setPixelColor(pixel, 255/bright,255/bright,255/bright,0);
    }
    strip_S.show();
    

   // analogWrite(ledPin, random (flashBrightnessMin, flashBrightnessMax)); // Turn LED strip on, random brightness
    delay(random(flashDurationMin, flashDurationMax)); // Keep it tured on, random duration
    for (int pixel = 0; pixel < strip_S.numPixels() - 1; pixel++) {
      strip_S.setPixelColor(pixel, c.LateNight);
    }
     strip_S.show();
   // SetAllStrips(LateNight);
    //analogWrite(ledPin, 0); // Turn the LED strip off
    delay(random(nextFlashDelayMin, nextFlashDelayMax)); // Random delay before next flash
  }



Serial.print(F("Pausing before playing thunder sound, milliseconds: "));
Serial.println(thunderDelay);
delay(thunderDelay);

Serial.print(F("Playing thunder sound, file number: "));
Serial.println(thunderFile);
// myDFPlayer.playMp3Folder(thunderFile);
// delay(1000); // Give the DFPlayer some time

// while (digitalRead(busyPin) == LOW) { // Wait for the DFPlayer to finish playing the MP3 file
// }

Serial.print(F("Pausing before next loop, milliseconds: "));
Serial.println(loopDelay);
delay(loopDelay);

}
