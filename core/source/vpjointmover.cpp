/// \file vpjointmover.cpp
/// \brief Implementation file for VPJointMover VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog
// Mar 22, 2006 - Bruno de Oliveira Schneider 
// - File created.

#include "vpjointmover.h"
#include "vpdof.h"
#include "vpdofmover.h"
#include "vptime.h"
#include "vplinearinterpolator.h"
#include <algorithm>
#include <cassert>

//#include <iostream>
using namespace std;

float VPJointMover::goalTime;

VPJointMover::VPJointMover()
{
    jointPtr = NULL;
    interpolatorPtr = NULL;
}

VPJointMover::VPJointMover(VPJoint* newJointPtr,
                           float newDuration, const VPInterpolator& interpolator)
{
    jointPtr = newJointPtr;
    duration = newDuration;
    interpolatorPtr = &interpolator;
}

VPJointMover::~VPJointMover()
{
    list<VPDofMover*>::iterator iter;
    // remove all DOF movers from memory
    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
        delete *iter;
}

void VPJointMover::Move()
// Note: A joint mover passes parameters to dof movers by setting some VPDofMover class attributes.
// This implementation should make animations a bit faster.
{
    list<VPDofMover*>::iterator iter;

    // pass normalized elapsed time to dof movers
    VPDofMover::goalTime = goalTime/duration;
    // pass interpolator to dof movers
    VPDofMover::interpolatorPtr = interpolatorPtr;
    // activate dof movers
    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
        (*iter)->Move();
}

void VPJointMover::AddDofMover(VPJoint::DofID dof, float iniTime, float finTime, float finPos)
{
    VPDofMover* moverPtr = new VPDofMover(iniTime, finTime, finPos);
    moverPtr->SetDof(const_cast<VPDof*>(&(jointPtr->GetDof(dof))));
    assert(moverPtr->targetDofPtr != NULL);
    dofMoverList.push_back(moverPtr);
}

void VPJointMover::DeactivateDofMovers()
{
    list<VPDofMover*>::iterator iter;
    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
        (*iter)->active = false;
}
