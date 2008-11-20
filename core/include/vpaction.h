/// \file vpaction.h
/// \brief Header file for VPAction VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog is at the implementation file.

#ifndef __VPACTION_H
#define __VPACTION_H

#include "vptime.h"
#include <list>

class VPJointMover;
class VPJoint;
class VPInterpolator;

/// \class VPAction vpaction.h
/// \brief A coordinated movement of joints in an articulated body
///
/// Actions represent simple tasks such as walking, waving or scratching the head.
/// They are implementated as a collection of joint movers (see VPJointMover).
class VPAction
{
    public:
    // PUBLIC METHODS
        /// \brief Creates an unitialized action
        VPAction();
        ~VPAction();
        /// \brief Adds a joint mover to the action.
        /// \return A pointer to a newly created joint mover.
        ///
        /// Creates a new joint mover which is assigned to the action. The memory allocated
        /// by the joint mover will be deallocated by the action's destructor.
        VPJointMover* AddJointMover(VPJoint* jointPtr,
                                    float newDuration, const VPInterpolator& interpolator);
        /// \brief Sets the priority of an action.
        void SetPriority(unsigned int newPriority) { priority = newPriority; }
        /// \brief Sets the speed of an action.
        void SetSpeed(float newSpeed) { speed = newSpeed; }
        /// \brief Sets whether an action is cyclic.
        void SetCyclic(bool value) { cycle = value; }
        /// \brief Sets speed, priority and cycle attibutes.
        void Set(float newSpeed, unsigned int newPriority, bool cyclic);
        /// \brief Activate action.
        void Activate();
        /// \brief Deactivate action.
        void Deactivate();
        /// \brief Returns true if the action is currently active.
        bool IsActive() const { return active; }
    // STATIC PUBLIC METHODS
        /// \brief Moves all active actions.
        /// \return The number of active actions.
        static unsigned int MoveAllActive();
    protected:
    // PROTECTED METHODS
        /// \brief Animate joints.
        void Move();
        /// \brief Deactivates DOF movers in every joint mover.
        void DeactivateDofMovers();
    // PROTECTED ATTRIBUTES
        bool cycle;
        bool active;
        float speed;
        float duration;
        unsigned int priority;
        std::list<VPJointMover*> moverList;
        VPTime initialTime;
    // STATIC PROTECTED ATTRIBUTES
        static std::list<VPAction*> activeInstances;
    private:
        // keep programmers from creating copies of actions
        VPAction(const VPAction& action) {}
};

#endif
