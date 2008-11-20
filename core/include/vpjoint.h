/// \file vpjoint.h
/// \brief Header file for VPJoint VPAT class.
/// \version $Revision: 1.10 $


// ChangeLog is at the implementation file.

#ifndef __VPJOINT_H
#define __VPJOINT_H

#ifndef __VPDOF_H
class VPDof;
#endif
#ifdef VISUAL_JOINTS
#include "vpmaterial.h"
#endif
#include "vptransform.h"

#include <list>

/// \class VPJoint vpjoint.h
/// \brief Representation of joints
///
/// A joint can be seen as a special kind of geometric transformation. It deals
/// with the complexities of real articulations, producing geometric transformations
/// that can be delivered to the graphics pipeline. A joint is a collecion of DOFs
/// (see VPDof) in which their order is important - the transformation of a joint is
/// (... DOF3 * DOF2 * DOF1), that is, DOF1's transform, followed by DOF2's transform...
/// In the current implementation, when DOFs change, their hierarchically inferior
/// DOFs are transformed,
/// that is, on a 3-DOF joint, changing the 2nd DOF tranforms the 3rd DOF. Their order
/// is defined by AddDof method. On future implementations this should change to allow
/// creation of more sophisticated joints.
/// Joints may not share DOFs, see VPDof for an explanation.
/// Compile with symbol VISUAL_JOINTS if you want to see DOFs for debugging purposes.
class VPJoint : public VPTransform {
    public:
        enum DofID { FLEXION, ADDUCTION, TWIST };
        /// Creates an uninitialized joint.
        VPJoint();
        virtual ~VPJoint();
        unsigned short int  GetNumDofs();
        void AddDof(VPDof *dof);

        /// \brief Updates the LIM, based on DOFs' situation.
        ///
        /// Not of interest to the application programmer. This method is called by
        /// DOFs when they are updated in order to keep the joint updated as well.
        void MakeLim();

        /// \brief Returns a joint's DOF.
        ///
        /// Returns a read-only version of a DOF from the joint. See DofID constants.
        const VPDof& GetDof(DofID dof);
        
        /// Put the joint in a rest position
        void SetAtRest();
        
        /// \brief Moves a joint's DOF.
        /// \return True if the specified DOF exists.
        /// \param dof [in] Specifies which DOF should be modified.
        /// \param variance [in] Specifies how much the DOF should move (using [0:1] position range).
        bool MoveDof(DofID dof, float variance);

#ifdef VISUAL_JOINTS
        virtual bool DrawOGL() const;
#endif

    protected:
#ifdef VISUAL_JOINTS
        /// \brief Returns materials for the visual representation of DOFs.
        static const VPMaterial& GetMaterial(int num);
#endif
        std::list<VPDof*> dofList;
};

#endif
