#include "jointtime.h"
#include <iostream>

using namespace std;
using namespace VART;

JointTime::JointTime()
{
    duration = 0;
    interpolator = " ";
}

JointTime::JointTime(float dur, string interp, Joint* jPtr)
{
    duration = dur;
    interpolator = interp;
    jointPtr = jPtr;
}

void JointTime::SetDuration(float dur)
{
    duration = dur;
}

float JointTime::GetDuration()
{
    return duration;
}

void JointTime::SetInterpolation(string interp)
{
    interpolator = interp;
}

string JointTime::GetInterpolation()
{
    return interpolator;
}

Joint* JointTime::GetJoint()
{
    return jointPtr;
}
