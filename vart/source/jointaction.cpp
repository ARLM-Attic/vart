/// \file jointaction.cpp
/// \brief Implementation file for V-ART class "JointAction".
/// \version $Revision: 1.1 $

#include "vart/dofmover.h"
#include "vart/jointaction.h"
#include "vart/jointmover.h"
#include "vart/dof.h"

#include <iostream>
using namespace std;

VART::JointAction::Initializer::Initializer()
{
    // There can be only one!
    static unsigned char counter = 0;
    if (counter == 0)
    {
        // Add itself to the list of initializers
        BaseAction::initializers.push_back(this);
    }
    else
        cerr << "\aCreate just one instance of JointAction::Initializer!" << endl;
    //FixMe: - throw an exception instead of error message
}

void VART::JointAction::Initializer::Initialize()
{
    VART::Dof::ClearPriorities();
}

VART::JointAction::JointAction()
{
    // Create an initializer when the first joint action is constructed
    static Initializer jointActionInitializer;
}

VART::JointMover* VART::JointAction::AddJointMover(Joint* jointPtr,
                                                   float newDuration,
                                                   const Interpolator& interpolator)
{
    JointMover* jointMoverPtr = new JointMover(jointPtr, newDuration, interpolator);
    jointMoverList.push_back(jointMoverPtr);
    if (newDuration > duration)
        duration = newDuration;
    return jointMoverPtr;
}

// virtual
void VART::JointAction::Move()
{
    ComputePositionIndex();

    // pass modified elapsed time to joint movers
    // joint movers see time as [0:action_duration] according to action activation and speed
    // FixMe: Perhaps the multiplication should be taken away. It was keep when porting the
    //        old Action class to this new JointAction.
    VART::JointMover::goalTime = positionIndex * duration;

    // pass priority to DOF movers
    VART::DofMover::priority = priority;
    // pass cycle flag to joint movers
    VART::DofMover::cycle = cyclic;
    
    // Tell joint movers to move their joints:
    list<VART::JointMover*>::iterator iter = jointMoverList.begin();
    for (; iter != jointMoverList.end(); ++iter)
        (*iter)->Move();
}
