/// \file vppolyaxialjoint.h
/// \brief Header file for VPPolyaxialJoint VPAT class.
///  \version $Revision: 1.8 $

// ChangeLog is at the implementation file.

#ifndef __VPPOLYAXIALJOINT_H
#define __VPPOLYAXIALJOINT_H

#include "vpjoint.h"
#include "vpdof.h"

/// \class VPPolyaxialJoint vppolyaxialjoint.h
/// \brief A joint with three rotational degrees of freedom.
///
/// FixMe: Put detailed description here.

class VPPolyaxialJoint : public VPJoint
{
    public:
        VPPolyaxialJoint();
        virtual ~VPPolyaxialJoint() {}
        void SetFlexionTo(float value);
        void SetAductionTo(float value);
        void SetTwistTo(float value);
        /// \brief Moves the first DOF.
        void Flex(float variance) { dofList.front()->Move(variance); }
        void Adduct(float variance);
        void Twist(float variance);
        void AddDof(VPDof* dof);
     private:
};

#endif
