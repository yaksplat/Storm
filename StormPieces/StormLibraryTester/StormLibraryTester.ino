#include "Storm.h"

Storm s;

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  randomSeed(analogRead(A0));
}

void loop()
{
  float Diameter = random(100, 600) / 100.0;
  float Speed = 250.0;
  float DistanceX = random(-700, -500) / 100.0;
  float DistanceY = random(-500, 500) / 100.0;

  s.SetParams(Diameter, Speed, DistanceX, DistanceY);
  s.Start(millis());

  while (s.done == false)
  {
    s.Update(millis());
    Serial.print("Diameter: ");    Serial.print(s.Diameter);
    Serial.print(" Speed: ");    Serial.print(s.MovementSpeed);
    Serial.print(" CurrentX: ");    Serial.print(s.CurrentX);
    Serial.print(" CurrentY: ");    Serial.print(s.DistanceY);
    Serial.print(" CurrentDistance: ");    Serial.print(s.GetDistance(s.CurrentX, s.DistanceY));
    Serial.print(" CurrentQuadrant: ");    Serial.print(s.CurrentQuadrant);
    Serial.print(" CurrentAngle: ");    Serial.print(s.CurrentAngle);
    Serial.print(" CurrentRing: ");    Serial.print(s.CurrentRing);
    Serial.print(" NearestEdge: ");    Serial.println(s.NearestEdge);

    //  Storm::points p = s.GetIntersections();
   // Storm::viewLimits v = 
    //s.CalcViewLimits();
    Serial.print("View Angles: ");    Serial.print(s.CurrentViewAngle1);    Serial.print(", ");    Serial.print(s.CurrentViewAngle2); Serial.print(" Starting angle is: ");    Serial.println(s.StartingAngle);
    Serial.print("N[");Serial.print(s.xN1);Serial.print("][");Serial.print(s.xN2);Serial.print("] start:");Serial.print(s.Nstart);Serial.print(" end:");Serial.println(s.Nend);
    Serial.print("E[");Serial.print(s.yE1);Serial.print("][");Serial.print(s.yE2);Serial.print("] start:");Serial.print(s.Estart);Serial.print(" end:");Serial.println(s.Eend);
    Serial.print("S[");Serial.print(s.xS1);Serial.print("][");Serial.print(s.xS2);Serial.print("] start:");Serial.print(s.Sstart);Serial.print(" end:");Serial.println(s.Send);
    Serial.print("W[");Serial.print(s.yW1);Serial.print("][");Serial.print(s.yW2);Serial.print("] start:");Serial.print(s.Wstart);Serial.print(" end:");Serial.println(s.Wend);
    delay(500);
  }
  
}


char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
