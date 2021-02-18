#include "Storm.h"

//const float PI = 3.1415926;

Storm::Storm()
{
}

void Storm::SetParams(float _diameter, float _speed, float _distanceX, float _distanceY)
{
    Diameter = _diameter;              // 1 to 5 miles
    MovementSpeed = _speed;            //8-10 knots
    DistanceX = CurrentX = _distanceX; //-5 to -7 miles
    DistanceY = CurrentY = _distanceY; //-5 to 5 miles

    done = false;
}

void Storm::Start(long startTime)
{
    //storm
    StartTime = startTime;
}

Storm::point Storm::GetCurrentLocation(long currentTime)
{

    CurrentTime = currentTime;
    ElapsedTime = CurrentTime - StartTime;

    CurrentX = DistanceX + (ElapsedTime / 997.0) * (MovementSpeed / 3600.0);

    point pt;
    pt.x = CurrentX;
    pt.y = CurrentY;
    return pt;
}

int Storm::GetQuadrant(float x, float y)
{
    //adjust x and y in case they fall on an axis
    if (x == 0)
        x += .01;
    if (y == 0)
        y += .01;

    if (x > 0 && y >= 0)
        return 1;
    if (x < 0 && y > 0)
        return 2;
    if (x < 0 && y < 0)
        return 3;
    else
        return 4;
}

float Storm::GetDistance(float x, float y)
{
    return sqrt(pow(x, 2) + pow(y, 2));
}
float Storm::GetAngle(float x, float y)
{
    float angle = RadToDegrees(atan(abs(y) / abs(x)));
    float totalAngle;
    switch (GetQuadrant(x, y))
    {
    case 1:
        totalAngle = angle;
        break;
    case 2:
        totalAngle = 180 - angle;
        break;
    case 3:
        totalAngle = 180 + angle;
        break;
    case 4:
        totalAngle = 360 - angle;
        break;
    default:
        totalAngle = 0;
        break;
    }
    return totalAngle;
}

float Storm::GetAngleRad(float x, float y)
{
    return atan(abs(y) / abs(x));
}

Storm::quadrants Storm::WhichQuadrants(float x, float y)
{
    quadrants q;
    int quadrant = GetQuadrant(x, y);
    float distance = GetDistance(x, y);
    switch (quadrant)
    {
    case 1:
        q.one = true;
        if (Diameter / 2 > x)
            q.two = true;
        if (Diameter / 2 > y)
            q.four = true;
        if (distance > Diameter / 2)
            q.three = true;
        break;
    case 2:
        q.two = true;
        if (Diameter / 2 > x)
            q.one = true;
        if (Diameter / 2 > y)
            q.three = true;
        if (distance > Diameter / 2)
            q.four = true;
        break;
    case 3:
        q.three = true;
        if (Diameter / 2 > x)
            q.four = true;
        if (Diameter / 2 > y)
            q.two = true;
        if (distance > Diameter / 2)
            q.one = true;
        break;
    case 4:
        q.four = true;
        if (Diameter / 2 > x)
            q.three = true;
        if (Diameter / 2 > y)
            q.one = true;
        if (distance > Diameter / 2)
            q.two = true;
        break;
    default:
        break;
    }

    return q;
}

int Storm::GetRing(float x, float y)
{
    float distance = GetDistance(x, y);
    float ringDistance = distance - (Diameter / 2);
    if (ringDistance < 1)
        return 5;
    if (ringDistance < 2)
        return 4;
    if (ringDistance < 3)
        return 3;
    if (ringDistance < 4)
        return 2;
    if (ringDistance < 5)
        return 1;
    return 0;
}

Storm::points Storm::GetIntersections()
{
    points p;
    float x0 = 0;
    float y0 = 0;
    float r0 = Storm::GetDistance(CurrentX, CurrentY);
    float r1 = Diameter / 2.0;
    float x1 = CurrentX;
    float y1 = CurrentY;

    if (r0 < r1 || r0 == 0)
    {
        //The island is inside the storm
        p.x1 = 9;
        p.y1 = 9;
        p.x2 = 9;
        p.y2 = 9;
        return p;
    }

    float dist = sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
    float a = (pow(r0, 2) - pow(r1, 2) + pow(dist, 2)) / (2 * dist);
    float h = sqrt(pow(r0, 2) - pow(a, 2));
    float x2 = x0 + a * (x1 - x0) / dist;
    float y2 = y0 + a * (y1 - y0) / dist;
    float x3 = x2 + h * (y1 - y0) / dist; // also x3=x2-h*(y1-y0)/d
    float y3 = y2 - h * (x1 - x0) / dist; // also y3=y2+h*(x1-x0)/d
    float x4 = x2 - h * (y1 - y0) / dist;
    float y4 = y2 + h * (x1 - x0) / dist;

    //return the coordinates of the view angles
    p.x1 = x3;
    p.y1 = y3;
    p.x2 = x4;
    p.y2 = y4;
    return p;
}

float Storm::GetViewLimitSpan()
{
    CurrentViewLimitSpan = CurrentViewAngle1 - CurrentViewAngle2;

    if (CurrentViewLimitSpan > 180)
        CurrentViewLimitSpan = 360 - CurrentViewLimitSpan;

    return CurrentViewLimitSpan;
}

void Storm::CalcViewAngles()
{
    //North
    xN1 = 0;
    xN2 = 0;
    //East
    yE1 = 0;
    yE2 = 0;
    //South
    xS1 = 0;
    xS2 = 0;
    //West
    yW1 = 0;
    yW2 = 0;
    Storm::points p = Storm::GetIntersections();

    if (p.x1 == 9 && p.x2 == 9 && p.y1 == 9 && p.y2 == 9)
    {
        //all 9's indicates that the storm is overhead and encompassing the island
        //the storm is visible on all walls
        //North
        xN1 = 0;
        xN2 = 100;
        //East
        yE1 = 0;
        yE2 = 100;
        //South
        xS1 = 0;
        xS2 = 100;
        //West
        yW1 = 0;
        yW2 = 100;
        AllFour = true;
        return;
    }
    AllFour = false;
    CurrentViewAngle1 = GetAngle(p.x1, p.y1);
    CurrentViewAngle2 = GetAngle(p.x2, p.y2);
}
void Storm::CalcViewLimits()
{
    if(AllFour)
    return;

    float span = GetViewLimitSpan();
    if (CurrentViewAngle1 - CurrentViewAngle2 < 180)
    {
        if (CurrentViewAngle1 - CurrentViewAngle2 > 0)
        {
            StartingAngle = 2;
        }
        else
        {
            StartingAngle = 1;
        }
    }
    else
    {
        /* code */
    }

    if (CurrentViewAngle1 <= 45 || CurrentViewAngle1 >= 315)
    {
        //it intersects the East
        if (CurrentViewAngle1 <= 45)
            yE1 = 5 * tan(DegreesToRad(CurrentViewAngle1));
        else
            yE1 = -5 * tan(DegreesToRad(360 - CurrentViewAngle1));
    }
    if (CurrentViewAngle1 >= 45 && CurrentViewAngle1 <= 135)
    {
        //it intersects the North
        if (CurrentViewAngle1 < 90)
            xN1 = 5 * tan(DegreesToRad(90 - CurrentViewAngle1));
        else
            xN1 = -5 * tan(DegreesToRad(CurrentViewAngle1 - 90));
    }
    if (CurrentViewAngle1 >= 135 && CurrentViewAngle1 <= 225)
    {
        //it intersects the West
        if (CurrentViewAngle1 >= 135 && CurrentViewAngle1 <= 180)
            yW1 = 5 * tan(DegreesToRad(180-CurrentViewAngle1));
        else
            yW1 = -5 * tan(DegreesToRad(CurrentViewAngle1-180));
    }
    if (CurrentViewAngle1 >= 225 && CurrentViewAngle1 <= 315)
    {
        //it intersects the south
        if (CurrentViewAngle1 < 270)
            xS1 = -5 * tan(DegreesToRad(270 - CurrentViewAngle1));
        else
            xS1 = 5 * tan(DegreesToRad(CurrentViewAngle1 - 270));
    }

    if (CurrentViewAngle2 <= 45 || CurrentViewAngle2 >= 315)
    {
        //it intersects the East
        if (CurrentViewAngle2 <= 45)
            yE2 = 5 * tan(DegreesToRad(CurrentViewAngle2));
        else
            yE2 = -5 * tan(DegreesToRad(360 - CurrentViewAngle2));
    }
    if (CurrentViewAngle2 >= 45 && CurrentViewAngle2 <= 135)
    {
        //it intersects the North
        if (CurrentViewAngle2 < 90)
            xN2 = 5 * tan(DegreesToRad(90 - CurrentViewAngle2));
        else
            xN2 = -5 * tan(DegreesToRad(CurrentViewAngle2 - 90));
    }
    if (CurrentViewAngle2 >= 135 && CurrentViewAngle2 <= 225)
    {
        //it intersects the West
        if (CurrentViewAngle2 >= 135 && CurrentViewAngle2 <= 180)
            yW2 = 5 * tan(DegreesToRad(180-CurrentViewAngle2));
        else
            yW2 = -5 * tan(DegreesToRad(CurrentViewAngle2-180));
    }
    if (CurrentViewAngle2 >= 225 && CurrentViewAngle2 <= 315)
    {
        //it intersects the south
        if (CurrentViewAngle2 < 270)
            xS2 = -5 * tan(DegreesToRad(270 - CurrentViewAngle2));
        else
            xS2 = 5 * tan(DegreesToRad(CurrentViewAngle2 - 270));
    }

    AllFour = false;
    return;
}

void Storm::CalculateStartsAndEnds()
{
    if(AllFour)
    {
        Nstart = 0;
        Nend = 100;
        Wstart =0;
        Wend = 100;
        Sstart =0;
        Send = 100;
        Estart =0;
        Eend = 100;
    }

    if (StartingAngle == 1)
    {

        if (yE1 != 0)
        {
            //the lightning starts on E, but how far does it go?
            Estart = map(yE1*100, -500, 500, 0, 100);

            if (yE2 != 0)
            {
                Eend = map(yE2*100, -500, 500, 0, 100);
                return;
            }
            if (xN2 != 0)
            {
                Eend = 100;
                Nend = 100;
                Nstart = map(xN2*100, -500, 500, 0, 100);
                return;
            }
            if(yW2 !=0)
            {
                 Eend = 100;
                 Nend = 100;
                 Nstart = 0;
                 Wstart = 0;
                 Wend = map(yW2*100, -500, 500, 100, 0);  // the map is flipped
                 return;
            }
        }
        if (yW1 != 0)
        {
            //the lightning starts on E, but how far does it go?
            Wstart = map(yW1*100, -500, 500, 100, 0);  // the map is flipped
            if (yW2 != 0)
            {
                Wend = map(yW2*100, -500, 500, 100, 0);  // the map is flipped
                return;
            }
            if (xS2 != 0)
            {
                Wend = 100;
                Send = 100;
                Sstart = map(xS2*100, -500, 500, 100, 0);
                return;
            }
            if(yE2 !=0)
            {
                Wend = 100;
                 Send = 100;
                 Sstart = 0;
                 Estart = 0;
                Eend = map(yE2*100, -500, 500, 0, 100);  
                 return;
            }
        }
         if (xN1 != 0)
        {
            //the lightning starts on E, but how far does it go?
            Nend = map(xN1*100, -500, 500, 0, 100);  
            if (xN2 != 0)
            {
               Nstart = map(xN2*100, -500, 500, 0, 100);  // the map is flipped
                return;
            }
            if (yW2 != 0)
            {
                Nstart = 0;
                Wstart = 0;
                Wend = map(yW2*100, -500, 500, 100, 0);
                return;
            }
            if(xS2 !=0)
            {
                Nstart = 0;
                 Wend = 100;
                 Wstart = 0;
                 Send = 100;
                Sstart = map(xS2*100, -500, 500, 100, 0);  
                 return;
            }
        }
        if (xS1 != 0)
        {
            //the lightning starts on E, but how far does it go?
            Send = map(xS1*100, -500, 500, 100, 0);  
            if (xS2 != 0)
            {
               Sstart = map(xS2*100, -500, 500, 100, 0);    // the map is flipped
                return;
            }
            if (yE2 != 0)
            {
                Sstart = 0;
                Estart = 0;
                Eend = map(yE2*100, -500, 500, 0, 100);
                return;
            }
            if(xN2 !=0)
            {
               Sstart = 0;
                Estart = 0;
                 Eend = 100;
                 Nend = 100;
                Nstart = map(xN2*100, -500, 500, 0, 100);  
                 return;
            }
        }
    }
}

Storm::projection Storm::CalcMappingForAngle(float angle)
{
    projection p;

 if (angle <= 45 || angle >= 315)
    {
        p.direction = 'E';
        //it intersects the East
        if (angle <= 45)
            p.intersectionPoint = 5 * tan(DegreesToRad(angle));
        else
            p.intersectionPoint = -5 * tan(DegreesToRad(360 - angle));
    }
    if (angle >= 45 && angle <= 135)
    {
         p.direction = 'N';
        //it intersects the North
        if (angle < 90)
            p.intersectionPoint = 5 * tan(DegreesToRad(90 - angle));
        else
            p.intersectionPoint = -5 * tan(DegreesToRad(angle - 90));
    }
    if (angle >= 135 && angle <= 225)
    {
         p.direction = 'W';
        //it intersects the West
        if (angle >= 135 && angle <= 180)
            p.intersectionPoint = 5 * tan(DegreesToRad(180-angle));
        else
            p.intersectionPoint = -5 * tan(DegreesToRad(angle-180));
    }
    if (angle >= 225 && angle <= 315)
    {
         p.direction = 'S';
        //it intersects the south
        if (angle < 270)
            p.intersectionPoint = -5 * tan(DegreesToRad(270 - angle));
        else
            p.intersectionPoint = 5 * tan(DegreesToRad(angle - 270));
    }   
    p.mapping = map(p.intersectionPoint*100, -500, 500, 0, 100);
    return p;
}

float Storm::RadToDegrees(float rad)
{
    return rad * 180 / PI;
}

float Storm::DegreesToRad(float degrees)
{
    return degrees * PI / 180;
}

void Storm::Update(long currentTime, bool debug)
{
    CurrentTime = currentTime;
    ElapsedTime = CurrentTime - StartTime;
    Nstart = 0;
    Nend = 0;
    Wstart = 0;
    Wend = 0;
    Sstart = 0;
    Send = 0;
    Estart = 0;
    Eend = 0;
    CurrentX = DistanceX + (ElapsedTime / 997) * (MovementSpeed / 3600);
    CurrentQuadrant = GetQuadrant(CurrentX, CurrentY);
    CurrentAngle = GetAngle(CurrentX, CurrentY);
    CurrentRing = GetRing(CurrentX, CurrentY);

    CurrentDistance = Storm::GetDistance(CurrentX, CurrentY);
    Storm::CalcViewAngles();  //<==== This will calculate AllFour

    //*****Only Needed for Testing*****//
    if (debug)
    {
        Storm::CalcViewLimits();
        Storm::CalculateStartsAndEnds();
    }
    //*****Only Needed for Testing*****//
    //*****just call CalcMappingForAngle instead to get the exact mapping

    NearestEdge = CurrentDistance - Diameter / 2.0;
    if (NearestEdge < 0)
        NearestEdge *= -1;
    if (NearestEdge > 5 && (CurrentQuadrant == 1 || CurrentQuadrant == 4))
        {
            AllFour = false;
            done = true;
        }
}
