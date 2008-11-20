/// \file vpdofmover.h
/// \brief Header file for VPDofMover VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog is at the implementation file.

#ifndef __VPDOFMOVER_H
#define __VPDOFMOVER_H

class VPDof;
class VPInterpolator;
class VPJointMover;

/// \class VPDofMover vpdofmover.h
/// \brief Controller for DOF movement.
///
/// DOF movers are entities that control movement of a DOF (see VPDof).
/// They should "live" inside a joint mover (see VPJointMover) which takes
/// care of their memory management.
/// VPDofMover is not meant to be used by the application programmer. Use
/// the interface methods in VPAction and VPJointMover.
class VPDofMover
{
    friend class VPJointMover;
    friend class VPAction;
    protected:
    // PROTECTED METHODS
        /// \brief Creates an unitialized DOF mover.
        VPDofMover();
        VPDofMover(float iniTime, float finTime, float finPos);
        /// \brief Sets the target DOF.
        void SetDof(VPDof* dofPtr) { targetDofPtr = dofPtr; }
        /// \brief Changes target DOF.
        void Move();
    // PROTECTED ATTRIBUTES
        /// \brief Target position.
        float targetPosition;
        /// \brief Position of target DOF upon activation.
        float initialPosition;
        /// \brief Time of predicted activation (normalized).
        float initialTime;
        /// \brief Time of actual activation (normalized).
        float activationTime;
        /// \brief Normalized deactivation time.
        float finalTime;
        /// \brief How far from target at activation time.
        float positionRange;
        /// \brief How much time to reach target position.
        float timeRange;
        /// \brief Target DOF.
        VPDof* targetDofPtr;
        /// \brief Tells whether a DOF is active.
        ///
        /// An active DOF mover has already checked its initial position and therefore knows
        /// the speed needed to get to target position. An inactive DOF mover must do these
        /// computations before moving its target DOF.
        bool active;
    //  STATIC PROTECTED ATTRIBUTES
        /// \brief Time of next snapshot, normalized to joint movement's duration.
        static float goalTime;
        /// \brief Indicates whether the current action is cyclic.
        static bool cycle;
        /// \brief Position interpolator.
        static const VPInterpolator* interpolatorPtr;
        /// \brief Priority of active action.
        static unsigned int priority;
    private:
        void MoveDof();
        void Activate();
};

#endif
