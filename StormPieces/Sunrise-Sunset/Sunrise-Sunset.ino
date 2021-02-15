#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "NeoFade.h"
#include "LinearFader.h"

#define LED_COUNT_S 192
#define LED_COUNT_E 147
#define LED_COUNT_N 147
#define LED_COUNT_W 188

#define LED_PIN_S     5
#define LED_PIN_E     6
#define LED_PIN_N     9
#define LED_PIN_W     10

#define BRIGHTNESS 255


int ColorPair;
bool runOnce;

NeoFade F;
LinearFader LF;

//String colors[] = { "Blue", "Pink", "Orange","yellow", "White" };
uint32_t FadeFromColor;
uint32_t FadeToColor;
uint32_t NextFromColor;
uint32_t NextToColor;
uint32_t PreviousFromColor;
uint32_t PreviousToColor;

uint32_t LinearFadeFromColor;
uint32_t LinearFadeToColor;
uint32_t CurrentColor;
uint32_t NewColor;

int fadeinterval = 1;

const uint32_t Pink = Adafruit_NeoPixel::Color(255, 128, 0, 0); //330

const uint32_t Green = Adafruit_NeoPixel::Color(0, 0, 255, 0); //120,100,100
const uint32_t Magenta = Adafruit_NeoPixel::Color(255, 255, 0, 0); //300,100,100
const uint32_t Violet = Adafruit_NeoPixel::Color(128, 255, 0, 0); //270,100,100
const uint32_t BrightWhite = Adafruit_NeoPixel::Color(255, 255, 255, 0); // 0,0,100
const uint32_t Chartreuse = Adafruit_NeoPixel::Color(128, 0, 255, 0); //90
const uint32_t LightBlue = Adafruit_NeoPixel::Color(0, 255, 128, 0); //210
const uint32_t Cyan = Adafruit_NeoPixel::Color(0, 255, 255, 0);//180
const uint32_t SpringGreen = Adafruit_NeoPixel::Color(0, 32, 255, 0);//128
const uint32_t DarkPink = Adafruit_NeoPixel::Color(255, 64, 0, 0);

const uint32_t LateNight = Adafruit_NeoPixel::Color(0, 16, 0, 0);//120,100,6.3
const uint32_t Blue = Adafruit_NeoPixel::Color(0, 255, 0, 0); //240
const uint32_t PreDawnViolet = Adafruit_NeoPixel::Color(64, 128, 0, 0); //270,100,50.2
const uint32_t Red = Adafruit_NeoPixel::Color(255, 0, 0, 0); //0,100,100
const uint32_t RedOrange = Adafruit_NeoPixel::Color(255, 0, 16, 0); //4
const uint32_t Orange = Adafruit_NeoPixel::Color(255, 0, 32, 0); //8
const uint32_t LightOrange = Adafruit_NeoPixel::Color(255, 0, 64, 0); //15
const uint32_t Yellow = Adafruit_NeoPixel::Color(255, 0, 128, 0); //30
const uint32_t Daylight = Adafruit_NeoPixel::Color(255, 0, 128, 255);
const uint32_t WarmWhite = Adafruit_NeoPixel::Color(0, 0, 0, 255);


//const uint32_t Test = Adafruit_NeoPixel::Color(R, B, G,W);
const uint32_t Test = Adafruit_NeoPixel::Color(64, 128, 0, 0);

//const int potPin = A1;

Adafruit_NeoPixel strip_S(LED_COUNT_S, LED_PIN_S, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_E(LED_COUNT_E, LED_PIN_E, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_N(LED_COUNT_N, LED_PIN_N, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_W(LED_COUNT_W, LED_PIN_W, NEO_GRBW + NEO_KHZ800);

uint32_t colors[][11] = {{LateNight, Blue}, {Blue, PreDawnViolet}, {PreDawnViolet, Red}, {Red, RedOrange}, {RedOrange, Orange}, {Orange, LightOrange}, {LightOrange, Yellow}, {Yellow, Daylight}, {Daylight, WarmWhite}, {WarmWhite, Daylight}, {Daylight, Yellow}, {Yellow, LightOrange}, {LightOrange, Orange}, {Orange, RedOrange}, {RedOrange, Red}, {Red, DarkPink}, {DarkPink, Blue}, {Blue, LateNight}};
uint32_t SunriseColors[][11] = {{LateNight, PreDawnViolet},  {PreDawnViolet, RedOrange}, {RedOrange, Orange}, {Orange, LightOrange}, {LightOrange, Yellow}, {Yellow, Daylight}, {Daylight, WarmWhite}};
//uint32_t SunsetColors2[][11] = {{WarmWhite, Daylight},  {Daylight, Yellow}, {Yellow, LightOrange}, {LightOrange, Orange}, {Orange, RedOrange}, {RedOrange, PreDawnViolet}, {PreDawnViolet, LateNight}};
uint32_t SunsetColors[][11] = {{Daylight, WarmWhite},  {Yellow, Daylight}, {LightOrange, Yellow}, {Orange, LightOrange}, {RedOrange, Orange}, {PreDawnViolet, RedOrange}, {LateNight, PreDawnViolet}};

uint32_t firstEastPixel;
uint32_t firstSouthPixel;
uint32_t lastSouthPixel;
uint32_t firstWestPixel;
uint32_t StripRemainderColor;

uint32_t currentFirstEastPixel;
uint32_t currentLastSouthPixel;
uint32_t currentFirstWestPixel;

unsigned long startingMillis = millis();
long totalTime = 290000;
long runningTime = 0;
int fadestart = 0;
int sunSteps1 = 0;
int sunSteps2 = 0;
int sideSteps = 0;
int sideMovement = 0;
int stepTime = 75;
int fadeWidth = 75;
int sunWidth = 30;
int ColorChangeTime = 10000;
unsigned long previousMillis = millis();
unsigned long lastUpdate;
int  updateInterval = 280; //10 == 42 seconds 280 = 30 min

bool SunsetDone = false;
bool SunriseDone = false;
char output[128];

void setup() {
  Serial.begin(9600);

  F.begin();
  F.setPeriod(10000);
  LF.begin();
  LF.setSteps(LED_COUNT_E );

  strip_S.setBrightness(BRIGHTNESS);
  strip_E.setBrightness(BRIGHTNESS);
  strip_N.setBrightness(BRIGHTNESS);
  strip_W.setBrightness(BRIGHTNESS);

  strip_S.begin();
  strip_E.begin();
  strip_N.begin();
  strip_W.begin();

  ColorPair = 0;
  //SetSunriseStrip();
  SetAllStrips(LateNight);
  runOnce = false;

  //pinMode(potPin, INPUT);
}

int potValue;

void loop() {
  uint16_t r, g, b, w;

  //  potValue = analogRead(potPin);          //Read and save analog value from potentiometer
  //Serial.print("potValue: "); Serial.println(potValue);

  //value = map(value, 0, 1023, 0, 255);
  // RollingFade(strip_S,RedOrange, LateNight, 75 );
  //RollingFade(strip_S,RedOrange, PreDawnViolet, 75 );
  // delay(1000);
  //LF.setSteps(LED_COUNT_E );
  // if (runOnce == false)
  // {
  //   Serial.print("Running Once...");
  //setStrip(strip_E, WarmWhite);
DoSunrise(strip_E);
  ColorPair = 0;
  sunSteps1 = 0;
  sunSteps2 = 0;
  SunriseDone = false;
  DoSunset(strip_W);
  SetAllStrips(LateNight);
  ColorPair = 0;
  sunSteps1 = 0;
  sunSteps2 = 0;
  SunsetDone = false;
  /*DoSunrise(strip_E);
  SetAllStrips(LateNight);
  ColorPair = 0;
  sunSteps1 = 0;
  sunSteps2 = 0;
  SunriseDone = false;
  DoSunrise(strip_N);
  SetAllStrips(LateNight);
  ColorPair = 0;
  sunSteps1 = 0;
  sunSteps2 = 0;
  SunriseDone = false;
  DoSunrise(strip_W);
  SetAllStrips(LateNight);
  ColorPair = 0;
  sunSteps1 = 0;
  sunSteps2 = 0;
  SunriseDone = false;*/
  //   DoSunset();
  //    SetAllStrips(LateNight);
  //SetSunriseStrip();
  //  fadeStrip(strip_S, RedOrange, LateNight, 0,90, false, true);
  //  fadeStrip(strip_N, RedOrange, LateNight, 0,90, false, true);
  // fadeStrip(strip_S, Green, Red, strip_S.numPixels()/2, strip_S.numPixels());
  // strip_S.show();
  //  strip_N.show();
  //  setStrip(strip_E, RedOrange);
  // runOnce = true;
  //fadeStrip(strip_S, RedOrange, LateNight, 60, 90, false, true);
  // strip_S.show();
  // }
  /*
    fadeStrip(strip_S, RedOrange, LateNight, 0, (90 + fadeinterval), false, true);
    fadeStrip(strip_N, RedOrange, LateNight, 0, (90 + fadeinterval), false, true);
    fadeinterval += 1;
    strip_S.show();
    strip_N.show();
  */

  //if (F.isEnded())
  //{
  //if (ColorPair % 9 == 0)
  //{
  //// hold the dark and daylight
  //      delay(10000);
  //}
  //FadeFromColor = colors[ColorPair % 18][0];
  //Serial.print("Fade from: ");
  //GetColorDebug(FadeFromColor);
  //FadeToColor = colors[ColorPair % 18][1];
  //Serial.print("Fade to: ");
  //GetColorDebug(FadeToColor);
  //    NextColor = colors[(ColorPair + 1) % 18][1];
  //    Serial.print("NextColor: ");
  //    GetColorDebug(NextColor);
  //    F.setFade(FadeFromColor, FadeToColor);
  //   ColorPair++;
  //  }



  //  if (F.getNext(r, g, b, w))
  //  {
  //   CurrentColor = Adafruit_NeoPixel::Color(r, b, g, w);
  //  Serial.print("CurrentColor: ");
  //  GetColorDebug(CurrentColor);
  //  LF.setFade(CurrentColor, NextColor);
  //SetAllStrips(r, g, b, w);
  // }
  //SetSunriseStrip();
}




void DoSunrise(Adafruit_NeoPixel &_strip)
{
 // potValue = analogRead(potPin);          //Read and save analog value from potentiometer
  // Serial.print("potValue: "); Serial.println(potValue);

  lastUpdate = 1001;

  //while color of the W strip != Daylight
  while (!SunriseDone)
  {
    unsigned long currentMillis = millis();

    //store the color of the first pixel in the east strip (firstEastPixel)
    //store the color of the last pixel in the South strip (lastSouthPixel)
    //store the color of the first pixel in the west strip (firstWestPixel)
    //  firstEastPixel = strip_E.getPixelColor(0);
    //  firstSouthPixel = strip_S.getPixelColor(0);
    //  lastSouthPixel = strip_S.getPixelColor(strip_S.numPixels() - 1);
    //  firstWestPixel = strip_W.getPixelColor(0);


    FadeFromColor = SunriseColors[ColorPair % 7][0];
    FadeToColor = SunriseColors[ColorPair % 7][1];
    NextFromColor = SunriseColors[(ColorPair + 1) % 7][0];
    NextToColor = SunriseColors[(ColorPair + 1) % 7][1];
    if (ColorPair > 0)
    {
      PreviousFromColor = SunriseColors[(ColorPair - 1) % 7][0];
      PreviousToColor = SunriseColors[(ColorPair - 1) % 7][1];
    }
    //shift colors every 10 seconds
    if (sunSteps1 == (_strip.numPixels() / 2)) //(currentMillis - previousMillis >= ColorChangeTime))
    {
      //previousMillis = currentMillis;
      Serial.print("runningTime: "); Serial.println(runningTime);
      Serial.print("Fade from: "); GetColorDebug(FadeFromColor);
      Serial.print("Fade to: "); GetColorDebug(FadeToColor);
      Serial.print("NextToColor: "); GetColorDebug(NextToColor);
      Serial.print("NextFromColor: "); GetColorDebug(NextFromColor);
      Serial.print("PreviousToColor: "); GetColorDebug(PreviousToColor);
      Serial.print("PreviousFromColor: "); GetColorDebug(PreviousFromColor);
      ColorPair++;
      //reset sunsteps each time the color changes

      sunSteps2 = sunSteps1;
      sunSteps1 = 0;
    }




    // if (runningTime % stepTime == 0  )//&& sunSteps < sunWidth)
    if ((currentMillis - lastUpdate) > updateInterval)
    {
      lastUpdate = currentMillis;
      int _start;
      int _end;
      _start = (_strip.numPixels() / 2) - sunSteps1;
      _end = _strip.numPixels() / 2;
      if (_start < 0)
        _start = 0;

      sprintf(output, "sunSteps1: %d, _start: %d, _end: %d", sunSteps1, _start, _end);
      Serial.println(output);
      Serial.print("Fade from: "); GetColorDebug(FadeFromColor);
      Serial.print("Fade to: "); GetColorDebug(FadeToColor);

      //  Serial.print("sunSteps1:"); Serial.println(sunSteps1);
      //Serial.print("_start:"); Serial.println(_start);
      // Serial.print("_end:"); Serial.println(_end);
      //start updating the color of the East strip (middle out)
      fadeStrip(_strip, FadeFromColor, FadeToColor, _start, _end, true, false);

      if (FadeToColor == WarmWhite && sunSteps1 == (_strip.numPixels() / 2) - 1)
        SunriseDone = true;
      /*  if (_start < 20)
        {
          if (ColorPair >0)
          {
             fadeStrip(strip_E, PreviousToColor, PreviousToColor, _start, 45, true, false);
          }
          else
          {
            fadeStrip(strip_E, PreviousToColor, PreviousToColor, _start, 45, true, false);
          }

          sunSteps2 = 0;
        }
        if (sunSteps2 >= 40)
        {
          Serial.print("sunSteps2:"); Serial.println(sunSteps2);
          fadeStrip(strip_E, PreviousFromColor, PreviousToColor, (strip_E.numPixels() / 2) - sunSteps2, 40, true, false);
          sunSteps2 += 2;
        }*/
      sunSteps1++;
    }
    //show all the changes
    _strip.show();

    runningTime = millis() - startingMillis;
  }  //end loop
}

void DoSunset(Adafruit_NeoPixel &_strip)
{
  // potValue = analogRead(potPin);          //Read and save analog value from potentiometer
  // Serial.print("potValue: "); Serial.println(potValue);
  sunSteps1 = (_strip.numPixels() / 2) + 1;
  lastUpdate = 1001;

  //while color of the W strip != Daylight
  while (!SunsetDone)
  {
    unsigned long currentMillis = millis();
    FadeFromColor = SunsetColors[ColorPair % 7][0];
    FadeToColor = SunsetColors[ColorPair % 7][1];

    if (ColorPair > 0)
    {
      PreviousFromColor = SunsetColors[(ColorPair - 1) % 7][0];
      PreviousToColor = SunsetColors[(ColorPair - 1) % 7][1];
    }
    //shift colors every 10 seconds
    if (sunSteps1 == 0) //(currentMillis - previousMillis >= ColorChangeTime))
    {
      Serial.print("Fade from: "); GetColorDebug(FadeFromColor);
      Serial.print("Fade to: "); GetColorDebug(FadeToColor);
      ColorPair++;
      //sunSteps2 = sunSteps1;
      sunSteps1 = (_strip.numPixels() / 2);
    }

    if ((currentMillis - lastUpdate) > updateInterval)
    {
      lastUpdate = currentMillis;
      int _start;
      int _end;
      _start = 0; //(strip_W.numPixels() / 2) - sunSteps1;
      _end = (_strip.numPixels() / 2) + 1 - sunSteps1;
      if (_end > (_strip.numPixels() / 2))
        _end = (_strip.numPixels() / 2);

      sprintf(output, "sunSteps1: %d, _start: %d, _end: %d", sunSteps1, _start, _end);
      Serial.println(output);
      Serial.print("Fade from: "); GetColorDebug(FadeFromColor);
      Serial.print("Fade to: "); GetColorDebug(FadeToColor);
      //flip start and end
      fadeStrip(_strip, FadeFromColor, FadeToColor, _start, _end, true, false);

      if (FadeToColor == PreDawnViolet && sunSteps1 == 1)
        SunsetDone = true;
      sunSteps1--;
    }
    //show all the changes
    _strip.show();

    runningTime = millis() - startingMillis;
  }  //end loop
}

void GetColorDebug(uint32_t color)
{
  Serial.print("(");
  Serial.print(lowByte(color >> 16), DEC); //red
  Serial.print(",");
  Serial.print(lowByte(color), DEC); //blue
  Serial.print(",");
  Serial.print(lowByte(color >> 8), DEC); //green
  Serial.print(",");
  Serial.print(lowByte(color >> 24), DEC); //white
  Serial.print(")");
  Serial.println("");

  //  byte white1 = (color1 >> 24) & 0xff;
  //      byte red1 = (color1 >> 16) & 0xff;
  //      byte green1 = (color1 >> 8) & 0xff;
  //      byte blue1 = color1 & 0xff;
}

void SetSunriseStrip()
{
  uint16_t l_r, l_g, l_b, l_w;

  //LF.setFade(Red, Green);
  for (int pixel = 0; pixel < LED_COUNT_E ; pixel++) {
    if ( LF.getNext(l_r, l_g, l_b, l_w))
    {
      NewColor = Adafruit_NeoPixel::Color(l_r, l_b, l_g, l_w);
      Serial.print("NewColor: ");
      GetColorDebug(NewColor);

      strip_E.setPixelColor(pixel, l_r, l_g, l_b, l_w);
      // strip_E.setPixelColor(LED_COUNT_E - pixel, l_r, l_g, l_b, l_w);
    }
  }
  strip_E.show();
}

uint32_t flipGreen(uint32_t color)
{
  byte white = (color >> 24) & 0xff;
  byte red = (color >> 16) & 0xff;
  byte green = (color >> 8) & 0xff;
  byte blue = color & 0xff;

  return Adafruit_NeoPixel::Color(red, green, blue, white);
}

void fadeStrip(Adafruit_NeoPixel &_strip, const uint32_t color1, const uint32_t color2, int _start, int _end, bool _mirror, bool _fill) {

  byte white1 = (color1 >> 24) & 0xff;
  byte red1 = (color1 >> 16) & 0xff;
  byte green1 = (color1 >> 8) & 0xff;
  byte blue1 = color1 & 0xff;

  byte white2 = (color2 >> 24) & 0xff;
  byte red2 = (color2 >> 16) & 0xff;
  byte green2 = (color2 >> 8) & 0xff;
  byte blue2 = color2 & 0xff;



  fadeStrip(_strip, red1, green1, blue1, white1, red2, green2, blue2, white2, _start, _end, _mirror, _fill);
}

void fadeStrip(Adafruit_NeoPixel &_strip, int r1, int g1, int b1, int w1, int r2, int g2, int b2, int w2, int _start, int _end, bool _mirror, bool _fill) {
  int steps = _end - _start;
  for (int i = 0; i <= steps; i++) {
    uint8_t red = (((r1 * (steps - i)) + (r2 * i)) / steps);
    uint8_t green = (((g1 * (steps - i)) + (g2 * i)) / steps);
    uint8_t blue = (((b1 * (steps - i)) + (b2 * i)) / steps);
    uint8_t white = (((w1 * (steps - i)) + (w2 * i)) / steps);
    // Sets the pixels to the color adjusted in the fade
    _strip.setPixelColor(_start + i, red, blue, green, white);
    if (_mirror)
      _strip.setPixelColor(_strip.numPixels() - _start - i, red, blue, green, white);
    if (_strip.numPixels() % 2 == 0)
      _strip.setPixelColor(_strip.numPixels() / 2 + 1, _strip.getPixelColor(_strip.numPixels() / 2));

  }
  if (_fill)
  {
    int remainder = _strip.numPixels() - steps;
    for (int i = steps; i <= _strip.numPixels() - 1; i++) {
      _strip.setPixelColor(i + _start, _strip.getPixelColor(steps));
    }
  }

  //  if(_mirror)
  //  {
  //for (int i = _strip.numPixels(); i < steps; i--) {
  ///    uint8_t red = (((r1 * (steps - i)) + (r2 * i)) / steps);
  //    uint8_t green = (((g1 * (steps - i)) + (g2 * i)) / steps);
  //    uint8_t blue = (((b1 * (steps - i)) + (b2 * i)) / steps);
  //    uint8_t white = (((w1 * (steps - i)) + (w2 * i)) / steps);
  //    // Sets the pixels to the color adjusted in the fade
  //    _strip.setPixelColor(i, red, green, blue, white);
  //  }

  //  }
  //_strip.show();
}


void FadeMiddleOut()
{

}

void RollingFade(Adafruit_NeoPixel _strip, const uint32_t _color1, const uint32_t _color2, int _fadeWidth )
{
  unsigned long startingMillis = millis();
  long totalTime = 290000;
  long runningTime = 0;
  int fadestart = 0;
  int iterations = 0;
  int movement = 0;
  int stepTime = 75;

  while (runningTime < totalTime)
  {
    //Serial.print("runningTime:"); Serial.println(runningTime);

    if (runningTime % stepTime == 0  && iterations < _fadeWidth)
    {
      Serial.print("iterations:"); Serial.println(iterations);
      Serial.print("movement:"); Serial.println(movement);
      fadeStrip(_strip, _color1, _color2, 0, iterations, false, false);
      _strip.show();
      iterations++;
    }

    if (runningTime % stepTime == 0  && iterations < (_strip.numPixels() + _fadeWidth) && iterations >= _fadeWidth)
    {
      Serial.print("iterations:"); Serial.println(iterations);
      Serial.print("movement:"); Serial.println(movement);
      fadeStrip(_strip, _color1, _color2, movement, movement + _fadeWidth, false, false);
      _strip.show();
      movement++;
      iterations++;
    }

    runningTime = millis() - startingMillis;

  }

  //start the fade from 0 to fadewidth

  //one it hits that, the fade start begins increasing, with the transition color trailing

  //this proceeds until the entire stip is the transition color
}

void SetAllStrips(uint16_t r, uint16_t b, uint16_t g, uint16_t w)
{
  for (int pixel = 0; pixel < LED_COUNT_S; pixel++) {
    strip_S.setPixelColor(pixel, r, g, b, w);
  }
  for (int pixel = 0; pixel < LED_COUNT_E; pixel++) {
    strip_E.setPixelColor(pixel, r, g, b, w);
  }
  for (int pixel = 0; pixel < LED_COUNT_N; pixel++) {
    strip_N.setPixelColor(pixel, r, g, b, w);
  }
  for (int pixel = 0; pixel < LED_COUNT_W; pixel++) {
    strip_W.setPixelColor(pixel, r, g, b, w);
  }
  strip_S.show();
  strip_E.show();
  strip_N.show();
  strip_W.show();
}

void SetAllStrips(const uint32_t color) {
  byte white = (color >> 24) & 0xff;
  byte red = (color >> 16) & 0xff;
  byte green = (color >> 8) & 0xff;
  byte blue = color & 0xff;
  SetAllStrips(red, green, blue, white);
}

void setStrip(Adafruit_NeoPixel _strip, const uint32_t color) {
  byte white = (color >> 24) & 0xff;
  byte red = (color >> 16) & 0xff;
  byte green = (color >> 8) & 0xff;
  byte blue = color & 0xff;
  setStrip(_strip, red, green, blue, white);
}



void setStrip(Adafruit_NeoPixel _strip, byte red, byte green, byte blue, byte white) {
  for (int pixel = 0; pixel < _strip.numPixels() - 1; pixel++) {
    _strip.setPixelColor(pixel, red, blue, green, white);
  }
  _strip.show(); // Output to strip
}

//void crossFade(const uint32_t startColor, const uint32_t endColor, unsigned long speed) {
//  byte startWhite = (startColor >> 24) & 0xff;
//  byte startRed = (startColor >> 16) & 0xff;
//  byte startGreen = (startColor >> 8) & 0xff;
//  byte startBlue = startColor & 0xff;

//  byte endWhite = (endColor >> 24) & 0xff;
//  byte endRed = (endColor >> 16) & 0xff;
//  byte endGreen = (endColor >> 8) & 0xff;
//  byte endBlue = endColor & 0xff;


// for each step in the cross-fade
//  for (int step = 0; step < 256; step++) {
//    byte red = map(step, 0, 255, startRed, endRed);
//    byte green = map(step, 0, 255, startGreen, endGreen);
//    byte blue = map(step, 0, 255, startBlue, endBlue);
//    byte white = map(step, 0, 255, startWhite, endWhite);
//    setStrip(red, green, blue, white);
//    delay(speed);
//  }
//}
