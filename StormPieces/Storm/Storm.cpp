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

void Storm::CalcViewLimits()
{
    //viewLimits v;
    //int StartingAngle;

    Storm::points p = Storm::GetIntersections();

    if (p.x1 == 9 && p.x2 == 9 && p.y1 == 9 && p.y2 == 9)
    {
        //all 9's indicates that the storm is overhead and encompassing the island
        //the storm is visible on all walls
        /*
        //North
        limits[0][0] = 0;
        limits[0][1] = 100;
        //East
        limits[1][0] = 0;
        limits[1][1] = 100;
        //South
        limits[2][0] = 0;
        limits[2][1] = 100;
        //West
        limits[3][0] = 0;
        limits[3][1] = 100;
*/
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
        if (CurrentViewAngle1 >= 135)
            yW1 = 5 * tan(DegreesToRad(CurrentViewAngle1));
        else
            yW1 = -5 * tan(DegreesToRad(360 - CurrentViewAngle1));
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
        if (CurrentViewAngle2 >= 135)
            yW2 = 5 * tan(DegreesToRad(CurrentViewAngle2));
        else
            yW2 = -5 * tan(DegreesToRad(360 - CurrentViewAngle2));
    }
    if (CurrentViewAngle2 >= 225 && CurrentViewAngle2 <= 315)
    {
        //it intersects the south
        if (CurrentViewAngle2 < 270)
            xS2 = -5 * tan(DegreesToRad(270 - CurrentViewAngle2));
        else
            xS2 = 5 * tan(DegreesToRad(CurrentViewAngle2 - 270));
    }

/*
    //North
    limits[0][0] = xN1;
    limits[0][1] = xN2;
    //East
    limits[1][0] = yE1;
    limits[1][1] = yE2;
    //South
    limits[2][0] = xS1;
    limits[2][1] = xS2;
    //West
    limits[3][0] = yW1;
    limits[3][1] = yW2;
    AllFour = false;
*/
    AllFour = false;
    return;
}

float Storm::RadToDegrees(float rad)
{
    return rad * 180 / PI;
}

float Storm::DegreesToRad(float degrees)
{
    return degrees * PI / 180;
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
