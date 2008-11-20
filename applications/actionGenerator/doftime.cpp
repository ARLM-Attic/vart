#include "doftime.h"
#include <iostream>

using namespace std;
using namespace VART;

DofTime::DofTime()
{
    initialTime = 0;
    finalTime = 0;
    dofType = " ";
    jointMover = NULL;
}

DofTime::DofTime(float inTime, float finTime, string dMov, Joint* jMov)
{
    initialTime = inTime;
    finalTime = finTime;
    dofType = dMov;
    jointMover = jMov;
}

void DofTime::SetInitialTime(float inTime)
{
    initialTime = inTime;
}

float DofTime::GetInitialTime()
{
    return initialTime;
}

void DofTime::SetFinalTime(float finTime)
{
    finalTime = finTime;
}

float DofTime::GetFinalTime()
{
    return finalTime;
}

void DofTime::SetDofType(string dPtr)
{
    dofType = dPtr;
}

string DofTime::GetDofType()
{
    return dofType;
}

void DofTime::SetJoint(Joint* jPtr)
{
    jointMover = jPtr;
}

Joint* DofTime::GetJoint()
{
    return jointMover;
}
