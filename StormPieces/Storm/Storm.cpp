#include "Storm.h"

const float PI = 3.1415926;

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

Storm::viewLimits Storm::GetViewLimits()
{
    viewLimits v;
    int StartingAngle;

    Storm::points p = Storm::GetIntersections();
    CurrentViewAngle1 = GetAngle(p.x1, p.y1);
    CurrentViewAngle2 = GetAngle(p.x2, p.y2);

    float span = GetViewLimitSpan();
    if (CurrentViewAngle1 - CurrentViewAngle2 < 180)
    {
        if (CurrentViewAngle1 - CurrentViewAngle2 > 0)
        {
            StartingAngle = 1;
        }
        else
        {
            StartingAngle = 2;
        }
    }
    else
    {
        /* code */
    }
    
    switch (Storm::GetQuadrant(p.x1,p.y1))
    {
    case 1:
        //check N
        
        //Check E
        /* code */
        break;
     case 2:
        //check N and W
        /* code */
        break;
            case 3:
        //check S and W
        break;
            case 4:
        //check S and E
        break;   
    default:
        break;
    }
    return v;
}

    float Storm::RadToDegrees(float rad)
    {
        return rad * 180/PI;
    }

    float Storm::DegreesToRad(float degrees)
    {
        return degrees * PI/180;
    }

void Storm::Update(long currentTime)
{
    CurrentTime = currentTime;
    ElapsedTime = CurrentTime - StartTime;

    CurrentX = DistanceX + (ElapsedTime / 997) * (MovementSpeed / 3600);
    CurrentQuadrant = GetQuadrant(CurrentX, CurrentY);
    CurrentAngle = GetAngle(CurrentX, CurrentY);
    CurrentRing = GetRing(CurrentX, CurrentY);

    float distance = Storm::GetDistance(CurrentX, CurrentY);

   // viewLimits v = Storm::GetViewLimits();

    NearestEdge = distance - Diameter / 2.0;
    if (NearestEdge < 0)
        NearestEdge *= -1;
    if (NearestEdge > 5 && (CurrentQuadrant == 1 || CurrentQuadrant == 4))
        done = true;
}
