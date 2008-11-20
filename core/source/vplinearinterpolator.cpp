/// \file vplinearinterpolator.cpp
/// \brief Implementation file for VPLinearInterpolator VPAT class.
/// \version $Revision: 1.2 $

// ChangeLog
// Apr 03, 2006 - Bruno de Oliveira Schneider 
// - GetValue is now "const".
// Mar 27, 2006 - Bruno de Oliveira Schneider 
// - File created.

#include "vplinearinterpolator.h"
#include <cmath>

//#include <iostream>
using namespace std;

VPLinearInterpolator::VPLinearInterpolator()
{
}

float VPLinearInterpolator::GetValue(float n) const
// virtual method
{
    return fabsf(fmodf(n + 1.0f,2.0f) - 1.0f);
}
