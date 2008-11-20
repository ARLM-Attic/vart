/// \file vprangesineinterpolator.cpp
/// \brief Implementation file for VPXml VPAT class.
/// \version $Revision: 1.2 $

// ChangeLog
// May 31, 2006 - Dalton Reis
// - Added ifdef necessary to windows to treat M_PI.
// Apr 27, 2006 - Andreia Schneider 
// - File created.

#include "vprangesineinterpolator.h"

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <iostream>
#include <cmath>

using namespace std;

VPRangeSineInterpolator::VPRangeSineInterpolator()
{
    minLimit = 0.0;
    maxLimit = 1.0;
}

void VPRangeSineInterpolator::SetMinimunLimit(float minLim)
{
    minLimit = minLim;
}

void VPRangeSineInterpolator::SetMaximunLimit(float maxLim)
{
    maxLimit = maxLim;
}

float VPRangeSineInterpolator::GetValue(float n) const
// virtual method
{
    return (((sinf((n+1.5)*M_PI)+1)/2.0f)*(maxLimit-minLimit))+minLimit;
}
