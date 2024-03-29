#ifndef STORM_H
#define STORM_H

#include <math.h>
#include <Arduino.h>

class Storm
{
private:
    unsigned long StartTime;
    unsigned long CurrentTime;

public:
    float Diameter;
    float MovementSpeed;
    float DistanceX;
    float DistanceY;
    float CurrentX;
    float CurrentY;
    bool done;
    float NearestEdge;
    long ElapsedTime;
    int CurrentQuadrant;
    float CurrentAngle;
    int CurrentRing;
    float CurrentDistance;
    float CurrentViewAngle1;
    float CurrentViewAngle2;
    float CurrentViewLimitSpan;
    float yE1, yE2, yW1, yW2, xN1, xN2, xS1, xS2;
    int Nstart, Nend, Wstart, Wend, Sstart, Send, Estart, Eend;

    int StartingAngle;
    bool AllFour;

    struct point
    {
        float x;
        float y;
    };

    struct points
    {
        float x1;
        float y1;
        float x2;
        float y2;
    };

    struct quadrants
    {
        bool one;
        bool two;
        bool three;
        bool four;
    };
    struct projection
    {
        float intersectionPoint;
        char direction;
        float mapping;
    };

    Storm();
    void SetParams(float _diameter, float _speed, float _distanceX, float _distanceY);
    void Start(long _startTime);
    struct point GetCurrentLocation(long _currentTime);
    int GetQuadrant(float x, float y);
    float GetDistance(float x, float y);
    float GetAngle(float x, float y);
    float GetAngleRad(float x, float y);
    struct quadrants WhichQuadrants(float x, float y);
    int GetRing(float x, float y);
    void Update(long _currentTime, bool debug);
    struct points GetIntersections();
    void CalcViewLimits();
    void CalcViewAngles();
    float GetViewLimitSpan();
    float RadToDegrees(float rad);
    float DegreesToRad(float degrees);
    void CalculateStartsAndEnds();
    struct projection CalcMappingForAngle(float angle);
};
#endif