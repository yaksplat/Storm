#include <Storm.h>


Storm s;

void setup() {
    Serial.begin(9600);
  // put your setup code here, to run once:
randomSeed(analogRead(A0));

  }

void loop() {
float Diameter = random(100,600)/100.0;
float Speed = 250.0;
float DistanceX = random(-700,-500)/100.0;
float DistanceY = random(-500,500)/100.0;  

  s.SetParams(Diameter, Speed, DistanceX, DistanceY);
  s.Start(millis());
   
  while (s.done == false)
  {
    s.Update(millis());
//dtostrf(s.CurrentX, 4, 1, str_temp);
    //    sprintf(output, "CurrentX: %s, CurrentQuadrant: %d, CurrentAngle: %g, CurrentRing: %d, NearestEdge: %g", str_temp, s.CurrentQuadrant, s.CurrentAngle, s.CurrentRing, s.NearestEdge);
   // Serial.println(output);
   Serial.print("Diameter: "); Serial.print(s.Diameter);
   Serial.print(" Speed: "); Serial.print(s.MovementSpeed);
    Serial.print(" CurrentX: "); Serial.print(s.CurrentX);
    Serial.print(" CurrentY: "); Serial.print(s.DistanceY);
    Serial.print(" CurrentDistance: "); Serial.print(s.GetDistance(s.CurrentX, s.DistanceY));
    Serial.print(" CurrentQuadrant: "); Serial.print(s.CurrentQuadrant);
    Serial.print(" CurrentAngle: "); Serial.print(s.CurrentAngle);
    Serial.print(" CurrentRing: "); Serial.print(s.CurrentRing);
    Serial.print(" NearestEdge: "); Serial.println(s.NearestEdge);

  //  Storm::points p = s.GetIntersections();
    Storm::viewLimits v = s.GetViewLimits();
    Serial.print("View Angles: "); Serial.print(s.CurrentViewAngle1);   Serial.print(", "); Serial.println(s.CurrentViewAngle2);
    // Serial.print("("); Serial.print(p.x1);   Serial.print(","); Serial.print(p.y1);    Serial.print("),("); Serial.print(p.x2);    Serial.print(","); Serial.print(p.y2);Serial.println(")");
    
    delay(500);
  }
  //*/
}

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
