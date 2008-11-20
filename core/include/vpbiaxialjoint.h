/// \file vpbiaxialjoint.h
/// \brief Header file for VPBiaxialJoint VPAT class.
/// \version 1.2

// ChangeLog is at the implementation file.

#ifndef __VPBIAXIALJOINT_H
#define __VPBIAXIALJOINT_H

#include "vpdof.h"
#include "vpjoint.h"

/// \class VPBiaxialJoint vpbiaxialjoint.h
/// \brief A joint with two rotational degrees of freedom.
///
/// FixMe: Put detailed description here.
class VPBiaxialJoint : public VPJoint {
    public:
        /// \brief Creates an uninitialized biaxial (2 DOFs) joint.
        ///
        /// Before using the joint, two DOFs should be added to it.
        VPBiaxialJoint();

        //~ VPBiaxialJoint( VPJoint *parent );
        //~ VPBiaxialJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2 );

        virtual ~VPBiaxialJoint();

        /// \brief Sets the first DOF.
        /// \param value [in] A number in the range [0:1] - zero means "set to minimal angle"
        /// and 1 means "set to maximal angle".
        void SetFlexionTo( float value );

        /// \brief Moves the first DOF.
        /// \param variance [in] A number in the range [0:1] - will be added to the first DOF's
        /// position (see VPDof::GetCurrent).
        void Flex(float variance) { dofList.front()->Move(variance); }

        /// \brief Sets the second DOF.
        /// \deprecated Spelling has been corrected to SetAdductionTo.
        void SetAductionTo( float value );

        /// \brief Sets the second DOF.
        /// \param value [in] A number in the range [0:1] - zero means "set to minimal angle"
        /// and 1 means "set to maximal angle".
        void SetAdductionTo( float value );

        /// \brief Moves the second DOF.
        /// \param variance [in] A number in the range [0:1] - will be added to the second
        /// DOF's position (see VPDof::GetCurrent).
        void Adduct(float variance);

        /// \brief Add a DOF to the joint.
        /// \param dof [in] A pointer to the DOF that is to be added.
        ///
        /// At most two DOFs can be added to a biaxial joint. VPJoint's destructor will
        /// dealocate DOFs marked as autoDelete.
        void AddDof(VPDof* dof);
    private:
};

/// FixMe: This class is not implementated yet.
class VPCondylarJoint : public VPBiaxialJoint {
};

/// FixMe: This class is not implementated yet.
class VPSaddlelJoint : public VPBiaxialJoint {
};

/// FixMe: This class is not implementated yet.
class VPEllipsoidJoint : public VPBiaxialJoint {
};

#endif
