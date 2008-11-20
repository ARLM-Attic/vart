/// \file vpdofmover.cpp
/// \brief Implementation file for VPDofMover VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog
// Mar 23, 2006 - Bruno de Oliveira Schneider 
// - File created.

#include "vpdofmover.h"
#include "vpinterpolator.h"
#include "vpdof.h"

//#include <iostream>
using namespace std;

float VPDofMover::goalTime;
bool VPDofMover::cycle;
const VPInterpolator* VPDofMover::interpolatorPtr = NULL;
unsigned int VPDofMover::priority;

VPDofMover::VPDofMover()
{
    active = false;
}

VPDofMover::VPDofMover(float iniTime, float finTime, float finPos)
{
    initialTime = iniTime;
    finalTime = finTime;
    targetPosition = finPos;
    active = false;
}

void VPDofMover::Move()
{ // FixMe: There might be a faster way to do this...
    if (active)
        MoveDof();
    else
        Activate();
}


void VPDofMover::Activate()
{
    if ((goalTime > initialTime) && (goalTime < finalTime))
    // test both as to not re-activate finished dof movers
    {
        initialPosition = targetDofPtr->GetCurrent();
        active = true;
        positionRange = targetPosition - initialPosition;
        timeRange = finalTime - goalTime;
        activationTime = goalTime;
    }
}

void VPDofMover::MoveDof()
{
    if ((goalTime < finalTime) && (goalTime > initialTime))
    // test both, so that cycles deactivate movers with initialTime > 0.
    {
        // goalTime is normalized in respect to joint movement, normalize it in respect to dof movement
        float interpolationIndex = (goalTime - activationTime)/timeRange;
        float goalPosition = initialPosition + (interpolatorPtr->GetValue(interpolationIndex) * positionRange);
        targetDofPtr->MoveTo(goalPosition, priority);
    }
    else
    {
        active = false;
    }
}
