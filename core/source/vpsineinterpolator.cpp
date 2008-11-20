/// \file vpsineinterpolator.cpp
/// \brief Implementation file for VPSineInterpolator VPAT class.
/// \version $Revision: 1.3 $

// ChangeLog
// Apr 03, 2006 - Bruno de Oliveira Schneider 
// - GetValue is now "const".
// Mar 28, 2006 - Bruno de Oliveira Schneider 
// - File created.

#include "vpsineinterpolator.h"

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>

//#include <iostream>
//using namespace std;

VPSineInterpolator::VPSineInterpolator()
{
}

float VPSineInterpolator::GetValue(float n) const
// virtual method
{
    return (sinf((n+1.5)*M_PI)+1)/2.0f;
}
