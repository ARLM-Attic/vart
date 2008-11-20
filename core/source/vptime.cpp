/// \file vptime.cpp
/// \brief Implementation file for VPTime VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog
// Mar 23, 2006 - Bruno de Oliveira Schneider 
// - Windows code created by Cleber Souza Ughini.
// - File created.

#include "vptime.h"

#ifdef __linux__  // compiling on a Linux system?
#include <sys/time.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

VPTime::VPTime()
{
}

VPTime::VPTime(double newSeconds)
{
    seconds = newSeconds;
}

unsigned int VPTime::WholeMicroseconds()
{
    return static_cast<unsigned int>(seconds * 1000000);
}

unsigned int VPTime::WholeMilliseconds()
{
    return static_cast<unsigned int>(seconds * 1000);
}

unsigned int VPTime::WholeSeconds()
{
    return static_cast<unsigned int>(seconds);
}

VPTime VPTime::operator-(const VPTime& initialTime)
{
    return VPTime(seconds - initialTime.seconds);
}

void VPTime::Set()
{
#ifdef __linux__
    static timeval temp;
    gettimeofday(&temp, NULL);
    seconds = static_cast<float>(temp.tv_sec) + (static_cast<float>(temp.tv_usec)/1000000.0f);
#endif

#ifdef WIN32
    LARGE_INTEGER perfCounter;
    LARGE_INTEGER perfFrequency;

    QueryPerformanceFrequency(&perfFrequency);

    QueryPerformanceCounter(&perfCounter);
    seconds = (((double)perfCounter.QuadPart) / ((double) perfFrequency.QuadPart));
#endif
}

const VPTime& VPTime::NOW()
// static method
{
    static VPTime time;
    time.Set();
    return time;
}

ostream& operator<<(ostream& output, const VPTime& t)
{
    output << t.seconds;
    return output;
}
