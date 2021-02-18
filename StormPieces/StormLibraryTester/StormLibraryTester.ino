#include "Storm.h"
#include "colors.h"

#define LED_COUNT_S 192
#define LED_COUNT_E 147
#define LED_COUNT_N 147
#define LED_COUNT_W 188

#define LED_PIN_S     5
#define LED_PIN_E     6
#define LED_PIN_N     9
#define LED_PIN_W     10

#define BRIGHTNESS 255

Adafruit_NeoPixel strip_S(LED_COUNT_S, LED_PIN_S, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_E(LED_COUNT_E, LED_PIN_E, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_N(LED_COUNT_N, LED_PIN_N, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_W(LED_COUNT_W, LED_PIN_W, NEO_GRBW + NEO_KHZ800);

Storm::projection proj;

colors c;
Storm s;

struct flash
{
  int EastStart;
  int EastEnd;
  int WestStart;
  int WestEnd;
  int NorthStart;
  int NorthEnd;
  int SouthStart;
  int SouthEnd;
  int Location;
  char Direction;
  int Size;
};

 // float Diameter, Speed, DistanceX, DistanceY;
int Nstart, Nend, Wstart, Wend, Sstart, Send, Estart, Eend;
unsigned long StartTime;
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long LastUpdate;
unsigned long LastLightning;
int UpdateInterval = 500;
int lightningInterval = 5000;

int randomInclusiveAngle;
int randomAngle;
int temp;

bool debug;
bool Nflipped, Wflipped, Sflipped, Eflipped ;
int RunCount;
int totalRuns =0;
void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  randomSeed(analogRead(A0));

  strip_S.setBrightness(BRIGHTNESS);
  strip_E.setBrightness(BRIGHTNESS);
  strip_N.setBrightness(BRIGHTNESS);
  strip_W.setBrightness(BRIGHTNESS);

  strip_S.begin();
  strip_E.begin();
  strip_N.begin();
  strip_W.begin();
  StartTime = millis();
  //s.done == true;
  debug = false;
  Nflipped = true;
  Wflipped = true;
  Eflipped = false;
  Sflipped = false;

  RunCount =1000;
}


void loop()
{
  float Diameter = random(100, 800) / 100.0;
  float Speed = 250.0;
  float DistanceX = random(-700, -500) / 100.0;
  float DistanceY = random(-500, 500) / 100.0;

  s.SetParams(Diameter, Speed, DistanceX, DistanceY);
  s.Start(millis());
if(totalRuns < RunCount)
{
  while (s.done == false )
  {
    
    CurrentTime = millis();
    if (CurrentTime - LastUpdate > UpdateInterval)
    {     
      LastUpdate = CurrentTime;      
      s.Update(millis(), debug);
      if(debug)
      {
       // ShowDebug();
      }
      if(s.AllFour)
      {
        //Get a random angle.  since we're in the middle, it can be anywhere
        randomAngle = random(0,360);
      }
      else
      {      
        randomAngle = GetRandomAngleBetweenValues(s.CurrentViewAngle1,s.CurrentViewAngle2);// random(s.CurrentViewAngle1*10000, s.CurrentViewAngle2*10000) / 10000;      
      }
     // Serial.print("Test Angle: ");    Serial.print(randomAngle);
      proj = s.CalcMappingForAngle(randomAngle);
     // Serial.print(" Projection Location: ");Serial.print(proj.intersectionPoint); Serial.print(" Direction: ");Serial.print(proj.direction); Serial.print(" Mapping: ");Serial.println(proj.mapping);
      
     // GetTheMappings(Nflipped, Wflipped, Sflipped, Eflipped, false);
     // LightUpStormLocations(proj); 
      if (CurrentTime - LastLightning > lightningInterval) 
      {   
        //lightningInterval should change based on the ring;
        LastLightning=CurrentTime;
        MakeSomeLightning(proj,s.CurrentRing);
      }
    }
  }
  totalRuns +=1;
}
}

int GetRandomAngleBetweenValues(float angle1, float angle2)
{
  float span = angle1 - angle2;
  int angle;
  if (angle1 - angle2 > 180)
    span = 360 - span;

  int adder;
  angle = random(angle1 * 10000, angle2 * 10000) / 10000;

  if (angle1 + span > 360 && angle2 - span < 0)
  {
    //the angles bridge the 0/360 threshold where angle 1 is in Q4 and Angle 2 is in Q1
    //add the adder to angle1
    adder = random(0, span * 10000) / 10000;
    angle = (int)(angle1 + adder) % 360;
  }

  if (angle2 + span > 360 && angle1 - span < 0)
  {
    //the angles bridge the 0/360 threshold where angle2 is in Q4 and angle1 is in Q1
    adder = random(0, span * 10000) / 10000;
    angle = (int)(angle2 + adder) % 360;
  }

  return angle;
}

int GetTheMapping(char Direction, int Location)
{
  int result;
  if (Direction == 'N')
  {
  //  if (Nflipped)
   // {
   //   result = map(Location, 0, 100, LED_COUNT_N, 0);
  //  }
   // else
   // {
      result = map(Location, 0, 100, 0, LED_COUNT_N);
   // }
  }
  if (Direction == 'W')
  {
   // if (Wflipped)
   // {
   //    result = map(Location, 0, 100, 0, LED_COUNT_W);
   // }
   // else
   // {
      result = map(Location, 0, 100, LED_COUNT_W, 0);
     
   // }
  }
  if (Direction == 'S')
  {
   // if (Sflipped)
   // {
   //   result = map(Location, 0, 100, 0, LED_COUNT_S);
     
   // }
   // else
   // {
       result = map(Location, 0, 100, LED_COUNT_S, 0);
   // }
  }
  if (Direction == 'E')
  {
  //  if (Eflipped)
   // {
   //  result = map(Location, 0, 100, LED_COUNT_E, 0);
   // }
  //  else
  //  {
      
      result = map(Location, 0, 100, 0, LED_COUNT_E);
   // }
  }
  return result;
}

void GetTheMappings(bool InvertN, bool InvertW, bool InvertS, bool InvertE, bool DoTheFlip)
{
  // it should be assumed that there is a pair of wires touching at south and east
  // as well as north and west
  // if any directions are flipped from this orientation, then the inversion should be used
  if (DoTheFlip)
  {
    if (InvertN)
    {
      Nend = map(s.Nstart, 0, 100, LED_COUNT_N, 0);
      Nstart = map(s.Nend, 0, 100, LED_COUNT_N, 0);
    }
    else
    {
      Nend = map(s.Nend, 0, 100, 0, LED_COUNT_N);
      Nstart = map(s.Nstart, 0, 100, 0, LED_COUNT_N);
    }

    if (InvertW)
    {
      Wend = map(s.Wstart, 0, 100, LED_COUNT_W, 0);
      Wstart = map(s.Wend, 0, 100, LED_COUNT_W, 0);
    }
    else
    {
      Wend = map(s.Wend, 0, 100, 0, LED_COUNT_W);
      Wstart = map(s.Wstart, 0, 100, 0, LED_COUNT_W);
    }

    if (InvertS)
    {
      Send = map(s.Sstart, 0, 100, LED_COUNT_S, 0);
      Sstart = map(s.Send, 0, 100, LED_COUNT_S, 0);
    }
    else
    {
      Send = map(s.Send, 0, 100, 0, LED_COUNT_S);
      Sstart = map(s.Sstart, 0, 100, 0, LED_COUNT_S);
    }

    if (InvertE)
    {
      Eend = map(s.Estart, 0, 100, LED_COUNT_E, 0);
      Estart = map(s.Eend, 0, 100, LED_COUNT_E, 0);
    }
    else
    {
      Eend = map(s.Eend, 0, 100, 0, LED_COUNT_E);
      Estart = map(s.Estart, 0, 100, 0, LED_COUNT_E);
    }
  }
  else
  {
    Nend = map(s.Nend, 0, 100, 0, LED_COUNT_N);
    Nstart = map(s.Nstart, 0, 100, 0, LED_COUNT_N);
    Eend = map(s.Eend, 0, 100, 0, LED_COUNT_E);
    Estart = map(s.Estart, 0, 100, 0, LED_COUNT_E);
    Send = map(s.Send, 0, 100, 0, LED_COUNT_S);
    Sstart = map(s.Sstart, 0, 100, 0, LED_COUNT_S);
    Wend = map(s.Wend, 0, 100, 0, LED_COUNT_W);
    Wstart = map(s.Wstart, 0, 100, 0, LED_COUNT_W);
  }
}

void LightUpStormLocations(Storm::projection p)
{
  //set everything to blue
  SetAllStrips(c.LateNight,false);
  
  //Set the lightning
  for (int pixel = Nstart; pixel < Nend - 1; pixel++)
  {
    strip_N.setPixelColor(pixel, 10,10,10,0);
  }
  for (int pixel = Wstart; pixel < Wend - 1; pixel++)
  {
    strip_W.setPixelColor(pixel, 10,10,10,0);
  }
  for (int pixel = Sstart; pixel < Send - 1; pixel++)
  {
    strip_S.setPixelColor(pixel, 10,10,10,0);
  }
  for (int pixel = Estart; pixel < Eend - 1; pixel++)
  {
    strip_E.setPixelColor(pixel, 10,10,10,0);
  }

  if (p.direction == 'N')
    strip_N.setPixelColor(GetTheMapping(p.direction,p.mapping ), c.Red);
  if (p.direction == 'W')
    strip_W.setPixelColor(GetTheMapping(p.direction,p.mapping ), c.Red);
  if (p.direction == 'S')
    strip_S.setPixelColor(GetTheMapping(p.direction,p.mapping ), c.Red);
  if (p.direction == 'E')
    strip_E.setPixelColor(GetTheMapping(p.direction,p.mapping ), c.Red);
  
  ShowAll();
}

void ShowAll()
{
  strip_N.show();
  strip_W.show();
  strip_S.show();
  strip_E.show();
}

void ShowDebug(int level)
{  
    Serial.print("Diameter: ");    Serial.print(s.Diameter);
    Serial.print(" Speed: ");    Serial.print(s.MovementSpeed);
    Serial.print(" CurrentX: ");    Serial.print(s.CurrentX);
    Serial.print(" CurrentY: ");    Serial.print(s.DistanceY);
    Serial.print(" CurrentDistance: ");    Serial.print(s.CurrentDistance); //GetDistance(s.CurrentX, s.DistanceY));
    Serial.print(" CurrentQuadrant: ");    Serial.print(s.CurrentQuadrant);
    Serial.print(" CurrentAngle: ");    Serial.print(s.CurrentAngle);
    Serial.print(" CurrentRing: ");    Serial.print(s.CurrentRing);
    Serial.print(" NearestEdge: ");    Serial.println(s.NearestEdge);

    //  Storm::points p = s.GetIntersections();
   // Storm::viewLimits v = 
    //s.CalcViewLimits();
    if(level == 2)
    {
    Serial.print("View Angles: ");    Serial.print(s.CurrentViewAngle1);    Serial.print(", ");    Serial.print(s.CurrentViewAngle2); Serial.print(" Starting angle is: ");    Serial.println(s.StartingAngle);
    Serial.print("N[");Serial.print(s.xN1);Serial.print("][");Serial.print(s.xN2);Serial.print("] start:");Serial.print(s.Nstart);Serial.print(" end:");Serial.println(s.Nend);
    Serial.print("E[");Serial.print(s.yE1);Serial.print("][");Serial.print(s.yE2);Serial.print("] start:");Serial.print(s.Estart);Serial.print(" end:");Serial.println(s.Eend);
    Serial.print("S[");Serial.print(s.xS1);Serial.print("][");Serial.print(s.xS2);Serial.print("] start:");Serial.print(s.Sstart);Serial.print(" end:");Serial.println(s.Send);
    Serial.print("W[");Serial.print(s.yW1);Serial.print("][");Serial.print(s.yW2);Serial.print("] start:");Serial.print(s.Wstart);Serial.print(" end:");Serial.println(s.Wend);
    Serial.print("Nstart:");Serial.print(Nstart);Serial.print(" end:");Serial.println(Nend);
    Serial.print("Wstart:");Serial.print(Wstart);Serial.print(" end:");Serial.println(Wend);
    Serial.print("Sstart:");Serial.print(Sstart);Serial.print(" end:");Serial.println(Send);
    Serial.print("Estart:");Serial.print(Estart);Serial.print(" end:");Serial.println(Eend);
    }
}

 
 // int flashBrightnessMin =  6;           // LED flash min. brightness (0-255)
 // int flashBrightnessMax =  1;          // LED flash max. brightness (0-255)
  int thunderDelay; // = random (500, 3000);  // Min. and max. delay between flashing and playing sound
 // int thunderFile = random (1, 17);       // There are 17 soundfiles: 0001.mp3 ... 0017.mp3
 // int loopDelay = random (5000, 30000);   // Min. and max. delay between each loop



  char output[128];
  flash f;
void MakeSomeLightning(Storm::projection p, int intensity)
{
   int flashCount = random (3, 15);        // Min. and max. number of flashes each loop
  int flashDurationMin = 1;               // Min. duration of each seperate flash
  int flashDurationMax = 45;              // Max. duration of each seperate flash
  int nextFlashDelayMin = 1;              // Min, delay between each flash and the next
  int nextFlashDelayMax = 125;            // Max, delay between each flash and the next
  int flashSize;// = random (5, 100);
  int flashVariation;
  int bright; //divisor on the brightness
  int location = GetTheMapping(p.direction,p.mapping );
  //intensity will be 1-5 based on the storm ring
  thunderDelay = s.CurrentDistance*5280/1000;
  switch (intensity)
  {
  case 0:
    bright = 20;
    flashSize = random (3, 8);
    flashVariation =3;
    lightningInterval=random (4500, 5500);
    break;
  case 1:
    bright = 10;
    flashSize = random (5, 18);
    flashVariation = 5;
    lightningInterval=random (4500, 5500);
    break;
  case 2:
    bright = 4;
    flashSize = random (12, 25);
    flashVariation=12;
    lightningInterval=random (4500, 5500);
    break;
  case 3:
    bright = 3;
    flashSize = random (20, 40);
    flashVariation =20;
    lightningInterval=random (3500, 4500);
    break;
  case 4:
    bright = 2;
    flashSize = random (35, 80);
    flashVariation=35;
    lightningInterval=random (2750, 3500);
    break;
  default:
    //it's a category 5!!!! the most intense
    bright = 1;
    flashSize = random (175, 200);
    flashVariation=60;
    lightningInterval= random (1500, 3500);
    break;
  }
  ShowDebug(1);


  //  sprintf(output, "flashLocation: %d, flashSize: %d, bright: %d", location, flashSize, bright);
   // Serial.println(output);

  for (int flash = 0 ; flash <= flashCount; flash += 1) { // Flashing LED strip in a loop, random count
    flashSize = flashSize + random(-flashVariation,flashVariation);  //vary the size of the flashes a little
  //  Serial.print("Direction: ");Serial.print(p.direction);
   // Serial.print(" location: ");Serial.print(location);
   // Serial.print(" flashSize: ");Serial.println(flashSize);

    SetAllStrips(c.LateNight,true);

    f.Direction = p.direction;
    f.Location = location;
    f.Size = flashSize;

    if(location > random(50,80))
        location -= random(1,8);
    else
        location += random(1,8);
    /*
    //Use for testing to force a location
    //good for edge tests
    f.Direction = 'E';
    f.Location = LED_COUNT_E -5;
    f.Size = 50;
    */

    f = CalculateOverflows(f);
    DoFlash(f,bright);

    //for (int pixel = flashLocation; pixel < flashSize+flashLocation; pixel++) {
    //  strip_S.setPixelColor(pixel, 255/bright,255/bright,255/bright,0);
    //}
    //strip_S.show();
    
    delay(random(flashDurationMin, flashDurationMax)); // Keep it tured on, random duration

    SetAllStrips(c.LateNight,true);

    delay(random(nextFlashDelayMin, nextFlashDelayMax)); // Random delay before next flash
  }
}

flash CalculateOverflows(flash f)
{
  // this takes the direction and size and then compares it against the location within the defined strips
  // if the flash boundaries exceed the beginning or end of the strip, it will overflow onto the neighboring strips
  // all data in the Flash Object should be calculated
  // this data is then passed on to the DoFlash function, which lights the strips.

  int start, end;
  //initialize everything to 0
  f.EastStart = 0;
  f.EastEnd = 0;
  f.WestStart = 0;
  f.WestEnd = 0;
  f.NorthStart = 0;
  f.NorthEnd = 0;
  f.SouthStart = 0;
  f.SouthEnd = 0;

  int OverflowAmount;
  if (f.Direction == 'E')
  {
    if (f.Location + (f.Size / 2) > LED_COUNT_E)
    {
      //overflow into north
      OverflowAmount = f.Location + (f.Size / 2) - LED_COUNT_E;
      f.EastStart = f.Location - (f.Size / 2);
      f.EastEnd = LED_COUNT_E - 1;
      f.NorthStart = LED_COUNT_N - 1 - OverflowAmount;
      f.NorthEnd = LED_COUNT_N - 1;
    }
    else if (f.Location - (f.Size / 2) < 0)
    {
      //overflow into south
      OverflowAmount = abs(f.Location - (f.Size / 2));
      f.EastStart = 0;
      f.EastEnd = f.Location + (f.Size / 2);
      f.SouthStart = 0;
      f.SouthEnd = OverflowAmount;
    }
    else
    {
      //the flash occurs in the middle of the strip
      f.EastStart = f.Location - (f.Size / 2);
      f.EastEnd = f.Location + (f.Size / 2);
    }

  }
  else if (f.Direction == 'W')
  {
    if (f.Location + (f.Size / 2) > LED_COUNT_W)
    {
      //overflow into south
      OverflowAmount = f.Location + (f.Size / 2) - LED_COUNT_W;
      f.WestStart = f.Location - (f.Size / 2);
      f.WestEnd = LED_COUNT_W ;
      f.SouthStart = LED_COUNT_S - 1 - OverflowAmount;
      f.SouthEnd = LED_COUNT_S;
    }
    else if (f.Location - (f.Size / 2) < 0)
    {
      //overflow into north
      OverflowAmount = abs(f.Location - (f.Size / 2));
      f.WestStart = 0;
      f.WestEnd = f.Location + (f.Size / 2);
      f.NorthStart = 0;
      f.NorthEnd = OverflowAmount;
    }
    else
    {
      //the flash occurs in the middle of the strip
      f.WestStart = f.Location - (f.Size / 2);
      f.WestEnd = f.Location + (f.Size / 2);
    }

  }
  else if (f.Direction == 'N')
  {
    if (f.Location + (f.Size / 2) > LED_COUNT_N)
    {
      //overflow into east
      OverflowAmount = f.Location + (f.Size / 2) - LED_COUNT_N;
      f.NorthStart = f.Location - (f.Size / 2);
      f.NorthEnd = LED_COUNT_N;
      f.EastStart = f.Location - OverflowAmount;
      f.EastEnd = LED_COUNT_E - 1;
    }
    else if (f.Location - (f.Size / 2) < 0)
    {
      //overflow into west
      OverflowAmount = abs(f.Location - (f.Size / 2));
      f.NorthStart = 0;
      f.NorthEnd = f.Location + (f.Size / 2);
      f.WestStart = 0;
      f.WestEnd = OverflowAmount;
    }
    else
    {
      //the flash occurs in the middle of the strip
      f.NorthStart = f.Location - (f.Size / 2);
      f.NorthEnd = f.Location + (f.Size / 2);
    }

  }
  else //(f.Direction == 'S')
  {
    if (f.Location + (f.Size / 2) > LED_COUNT_S)
    {
      //overflow into west
      OverflowAmount = f.Location + (f.Size / 2) - LED_COUNT_S;
      f.SouthStart = LED_COUNT_S - 1 - OverflowAmount;
      f.SouthEnd = LED_COUNT_S - 1;
      f.WestStart = LED_COUNT_W - 1 - OverflowAmount;
      f.WestEnd = LED_COUNT_W;
    }
    else if (f.Location - (f.Size / 2) < 0)
    {
      //overflow into east
      OverflowAmount = abs(f.Location - (f.Size / 2));
      f.SouthStart = 0;
      f.SouthEnd = f.Location + (f.Size / 2);
      f.EastStart = 0;
      f.EastEnd = OverflowAmount;
    }
    else
    {
      //the flash occurs in the middle of the strip
      f.SouthStart = f.Location - (f.Size / 2);
      f.SouthEnd = f.Location + (f.Size / 2);
    }

    
  }

  if (Wflipped)
  {
    /*
      Serial.print("WestStartO: ");
      Serial.print(f.WestStart);
      Serial.print(" WestEndO: ");
      Serial.print(f.WestEnd);
*/
    start = f.WestStart;
    end = f.WestEnd;
    f.WestStart = abs(LED_COUNT_W - end);
    f.WestEnd = abs(LED_COUNT_W - start);
    /*
      Serial.print(" WestStartN: ");
      Serial.print(f.WestStart);
      Serial.print(" WestEndN: ");
      Serial.println(f.WestEnd);
      */
  }
  if (Sflipped)
  {
    start = f.SouthStart;
    end = f.SouthEnd;
    f.SouthStart = abs(LED_COUNT_S - end);
    f.SouthEnd = abs(LED_COUNT_S - start);
  }
  if (Nflipped)
  {
    start = f.NorthStart;
    end = f.NorthEnd;
    f.NorthStart = abs(LED_COUNT_N - end);
    f.NorthEnd = abs(LED_COUNT_N - start);
  }
  if (Eflipped)
  {
    start = f.EastStart;
    end = f.EastEnd;
    f.EastStart = abs(LED_COUNT_E - end);
    f.EastEnd = abs(LED_COUNT_E - start);
  }

  return f;
}

void DoFlash(flash f, int bright)
{
  for (int pixel = f.NorthStart; pixel < f.NorthEnd; pixel++)
  {
    strip_N.setPixelColor(pixel, 255 / bright, 255 / bright, 255 / bright, 0);
  }
  for (int pixel = f.WestStart; pixel < f.WestEnd; pixel++)
  {
    strip_W.setPixelColor(pixel, 255 / bright, 255 / bright, 255 / bright, 0);
  }
  for (int pixel = f.SouthStart; pixel < f.SouthEnd; pixel++)
  {
    strip_S.setPixelColor(pixel, 255 / bright, 255 / bright, 255 / bright, 0);
  }
  for (int pixel = f.EastStart; pixel < f.EastEnd; pixel++)
  {
    strip_E.setPixelColor(pixel, 255 / bright, 255 / bright, 255 / bright, 0);
  }

  ShowAll();
}

char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

void SetAllStrips(uint16_t r, uint16_t b, uint16_t g, uint16_t w, bool show)
{
  for (int pixel = 0; pixel < LED_COUNT_S; pixel++)
  {
    strip_S.setPixelColor(pixel, r, g, b, w);
  }
  for (int pixel = 0; pixel < LED_COUNT_E; pixel++)
  {
    strip_E.setPixelColor(pixel, r, g, b, w);
  }
  for (int pixel = 0; pixel < LED_COUNT_N; pixel++)
  {
    strip_N.setPixelColor(pixel, r, g, b, w);
  }
  for (int pixel = 0; pixel < LED_COUNT_W; pixel++)
  {
    strip_W.setPixelColor(pixel, r, g, b, w);
  }
  if (show)
  {
    ShowAll();
  }
}

void SetAllStrips(const uint32_t color, bool show)
{
  byte white = (color >> 24) & 0xff;
  byte red = (color >> 16) & 0xff;
  byte blue = (color >> 8) & 0xff;
  byte green = color & 0xff;
  SetAllStrips(red, green, blue, white, show);
}
