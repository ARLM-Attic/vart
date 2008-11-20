/// \file vpjointmover.h
/// \brief Header file for VPJointMover VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog is at the implementation file.

#ifndef __VPJOINTMOVER_H
#define __VPJOINTMOVER_H

#include "vpjoint.h"
#include <list>

class VPDofMover;
class VPInterpolator;

/// \class VPJointMover vpjointmover.h
/// \brief Controllers for joint movement.
///
/// Joint movers contain a set of DOF movers (see VPDofMover). They control how a joint
/// moves in a particular action (see VPAction).
class VPJointMover
{
    public:
    // PUBLIC METHODS
        VPJointMover();
        VPJointMover(VPJoint* newJointPtr,
                     float newDuration, const VPInterpolator& interpolator);
        ~VPJointMover();
        /// \brief Moves the associated joint.
        void Move();
        /// \brief Sets the associated joint.
        void AttachToJoint(VPJoint* newJointPtr) { jointPtr = newJointPtr; }
        /// \brief Creates a DofMover.
        /// \param dof [in] DOF ID on which the new DOF mover will act
        /// \param iniTime [in] Initial movement time (in range [0..1])
        /// \param finTime [in] Final  movement time (in range [0..1])
        /// \param finPos [in] Final movement position  (in range [0..1])
        void AddDofMover(VPJoint::DofID dof, float iniTime, float finTime, float finPos);
        /// \brief Sets the total movement duration (in seconds).
        void SetDuration(float newDuration) { duration = newDuration; }
        /// \brief Deactivates all DOF movers.
        void DeactivateDofMovers();
    // PUBLIC STATIC ATTRIBUTES
        //static bool cycle;
        static float goalTime;
        //static unsigned int priority;
    protected:
    // PROTECTED ATTRIBUTES
        /// \brief Associated joint
        VPJoint* jointPtr;
        float duration;
        const VPInterpolator* interpolatorPtr;
        std::list<VPDofMover*> dofMoverList;
};

#endif
