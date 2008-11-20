/// \file vpuniaxialjoint.h
/// \brief Header file for VPUniaxialJoint VPAT class.
/// \version 1.6

// ChangeLog is at the implementation file.

#ifndef __VPUNIAXIALJOINT_H
#define __VPUNIAXIALJOINT_H

#include "vpdof.h"
#include "vpjoint.h"

/// \class VPUniaxialJoint vpuniaxialjoint.h
/// \brief A joint with one rotational degrees of freedom.
///
/// FixMe: Put detailed description here.
class VPUniaxialJoint : public VPJoint {
    public:
        /// \brief Creates an uninitialized biaxial (1 DOF) joint.
        ///
        /// Before using the joint, one DOF should be added to it.
        VPUniaxialJoint();

        //~ VPBiaxialJoint( VPJoint *parent );
        //~ VPBiaxialJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2 );

        virtual ~VPUniaxialJoint();

        /// \brief Sets the first DOF.
        /// \param value [in] A number in the range [0:1] - zero means "set to minimal angle"
        /// and 1 means "set to maximal angle".
        void SetFlexionTo( float value );

        /// \brief Moves the first DOF.
        /// \param variance [in] A number in the range [0:1] - will be added to the first DOF's
        /// position (see VPDof::GetCurrent).
        void Flex(float variance) { dofList.front()->Move(variance); }

        /// \brief Add a DOF to the joint.
        /// \param dof [in] A pointer to the DOF that is to be added.
        ///
        /// Only one DOF can be added to a uniaxial joint. VPJoint's destructor will
        /// dealocate DOF marked as autoDelete.
        void AddDof(VPDof* dof);
    private:
};

/// FixMe: This class is not implementated yet.
class VPHingeJoint : public VPUniaxialJoint {
};

/// FixMe: This class is not implementated yet.
class VPPivotJoint : public VPUniaxialJoint {
};
#endif
