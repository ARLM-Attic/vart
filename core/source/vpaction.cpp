/// \file vpaction.cpp
/// \brief Implementation file for VPAction VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog
// Mar 28, 2006 - Bruno de Oliveira Schneider 
// - File created.

#include "vpaction.h"
#include "vpjointmover.h"
#include "vpdofmover.h"
#include "vptime.h"
#include "vpdof.h"

//#include <iostream>
using namespace std;

list<VPAction*> VPAction::activeInstances;

VPAction::VPAction()
{
    duration = 0.0f;
    active = false;
}

VPAction::~VPAction()
{
    list<VPJointMover*>::iterator iter;
    // destroy all joint movers
    for (iter = moverList.begin(); iter != moverList.end(); ++iter)
        delete *iter;
}

void VPAction::Set(float newSpeed, unsigned int newPriority, bool cyclic)
{
    speed = newSpeed;
    priority = newPriority;
    cycle = cyclic;
}

VPJointMover* VPAction::AddJointMover(VPJoint* jointPtr,
                                      float newDuration, const VPInterpolator& interpolator)
{
    VPJointMover* jointMoverPtr = new VPJointMover(jointPtr, newDuration, interpolator);
    moverList.push_back(jointMoverPtr);
    if (newDuration > duration)
        duration = newDuration;
    return jointMoverPtr;
}

void VPAction::Move()
// Note: An action passes parameters to joint movers by setting some VPJointMover class attributes.
// This implementation should make animations a bit faster.
// Joint movers pass parameters to dof movers using the same scheme.
{
    static VPTime currentTime;
    // timeDiff holds how much time has passed since last call
    static float timeDiff;
    list<VPJointMover*>::iterator iter;

    // compute timeDiff
    currentTime.Set();
    timeDiff = (currentTime - initialTime).AsFloat() * speed;

    // deactivate if finished
    if (timeDiff > duration)
    {
        if (cycle)
        {
            timeDiff -= duration;
            initialTime = currentTime;
        }
        else
        {
            Deactivate();
            return;
        }
    }
    // pass modified elapsed time to joint movers
    // joint movers see time as [0:action_duration] according to action activation and speed
    VPJointMover::goalTime = timeDiff;
    // pass priority to DOF movers
    VPDofMover::priority = priority;
    // pass cycle information to joint movers
    VPDofMover::cycle = cycle;
    // activate joint movers
    for (iter = moverList.begin(); iter != moverList.end(); ++iter)
        (*iter)->Move();
}

void VPAction::Activate()
{
    if (!active)
    {
        list<VPAction*>::iterator iter;
        bool finishedBeforeInserting = true;

        initialTime.Set();
        for (iter = activeInstances.begin(); iter != activeInstances.end(); ++iter)
        {
            // Actions with higher priority must come first in the list so that those
            // with lower priority will never have the chance to move DOFs. If they did,
            // transitions between DOF movers of the same action would get ugly.
            if ((*iter)->priority < priority)
            {
                activeInstances.insert(iter,this);
                finishedBeforeInserting = false;
            }
        }
        if (finishedBeforeInserting)
            activeInstances.push_back(this);
        active = true;
    }
}

void VPAction::Deactivate()
{
    if (active)
    {
        list<VPAction*>::iterator iter = activeInstances.begin();
        list<VPAction*>::iterator tempIter;

        // Remove this instance from list and deactivate all dof movers so that they may
        // smoothly take control of positions.
        active = false;
        while (iter != activeInstances.end())
        {
            if ((*iter) == this)
            {
                tempIter = iter;
                ++iter;
                activeInstances.erase(tempIter);
            }
            else
            {
                (*iter)->DeactivateDofMovers();
                ++iter;
            }
        }
    }
}

void VPAction::DeactivateDofMovers()
{
    list<VPJointMover*>::iterator iter;
    for (iter = moverList.begin(); iter != moverList.end(); ++iter)
        (*iter)->DeactivateDofMovers();
}

unsigned int VPAction::MoveAllActive()
// static method
{
    list<VPAction*>::iterator iter = activeInstances.begin();
    list<VPAction*>::iterator tempIter;
    // reset dof update priorities -- new draw cycle has begun
    VPDof::ClearPriorities();
    // move joints
    while (iter != activeInstances.end())
    {
        tempIter = iter;
        ++iter;
        // the action could remove itself from the list, so use a private iterator copy
        (*tempIter)->Move();
    }

    return activeInstances.size();
}
