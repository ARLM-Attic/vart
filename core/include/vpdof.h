/// \file vpdof.h
/// \brief Header file for VPDof VPAT class.
/// \version $Revision: 1.12 $

// ChangeLog is at the implementation file

#ifndef __VPDOF_H
#define __VPDOF_H

#ifndef __VPJOINT_H
class VPJoint;
#endif

#ifndef __VPMODIFIER_H
class VPModifier;
#endif

#include "vppoint4d.h"
#include "vptransform.h"
#include "vpbezier.h"
#include "vpmemoryobj.h"
#include <string>
#include <list>

/// \class VPDof vpdof.h
/// \brief Degree Of Freedom - basic component of a VPJoint.
///
/// A DOF is an axis of rotation, that may move along a 3D curve (the evoluta). The rotation
/// of a DOF has a limited range that may change according to external elements, this is
/// controlled by the rangeModifier (VPModifier). There may exist a hierachy of DOFs, so
/// that by changing one DOF, hierarchically lower DOFs are rotated. DOFs may not be shared
/// among joints because they have a single pointer to the owner joint and because the
/// joint destructor may destroy DOFs marked as autoDelete.
class VPDof : public VPMemoryObj {
    public:
    // PUBLIC METHODS
        VPDof();
        VPDof(const VPDof& dof);
        /// \brief Creates and initializes a DOF.
        /// \param vec [in] A vector. Together with "pos", defines the rotation axis.
        /// \param pos [in] A point. Together with "vec", defines the roatation axis.
        /// \param min [in] An angle (radians). Minimal allowed rotation.
        /// \param max [in] An angle (radians). Maximal allowed rotation.
        /// The DOF is created so that "current position" refers to zero rotation. If roatation
        /// range does not allow zero rotation, then the programmer should manually fix this
        /// using MoveTo.
        VPDof(const VPPoint4D& vec, const VPPoint4D& pos, float min, float max);
        ~VPDof();
        VPDof& operator=(const VPDof& dof);
        void SetDescription(const std::string& desc);
        const std::string& GetDescription() const { return description; }
        VPPoint4D GetAxis() const;
        VPPoint4D GetOrigin() const;
        VPBezier* GetEvoluta() { return evoluta; }
        void GetLim(VPTransform* ptrResult);
        const VPTransform& GetLim() const;
        float GetMin() const;
        float GetMax() const;
        float GetCurrentMin() const;
        float GetCurrentMax() const;
        /// \brief Returns DOF's rest position.
        float GetRest() const;
        VPJoint* GetOwnerJoint();
        void SetEvoluta( VPBezier *evol );
        void SetLim(const VPTransform& t);
        void SetAxis( VPVector3D v );
        void SetMin( float min );
        void SetMax( float max );
        void SetOwnerJoint( VPJoint *ow );
        /// \brief Sets DOF's current position.
        /// \param pos [in] A number in the range [0:1]
        ///
        /// A value of zero means minimal rotation (see GetMin, SetMin) around the initial
        /// axis position (see GetAxis, GetOrigin, GetEvoluta and related "Set" methods).
        /// A value of one means maximal rotation. Other values are linear.
        void MoveTo(float pos);
        /// \brief Sets DOF's current position.
        /// \param pos [in] A number in the range [0:1]
        /// \param newPriority [in] Priority for DOF update (greater means greater priority)
        ///
        /// Moves the DOF to given position if current update has priority over last update.
        void MoveTo(float pos, unsigned int newPriority);

        /// \brief Gets DOF's current position.
        float GetCurrent() const;

        /// \brief Changes DOF

        /// Changes how much the DOF is "bent"
        /// \param variance [in] How much to change the "current position".
        /// \sa MoveTo()
        void Move(float variance) { MoveTo(currentPosition+variance); }

        void SetRest( float rest );

        /// \brief Initializes a DOF previouly created with default constructor

        /// Newly created DOFs are set at zero rotation. Use GetCurrent to find
        /// initial positial.
        /// \param vec [in] Rotation vector;
        /// \param pos [in] DOF position (defines an axis along with "vec");
        /// \param min [in] Minimal bending angle in radians;
        /// \param max [in] Maximal bending angle in radians;
        void Set(const VPPoint4D& vec, const VPPoint4D& pos, float min, float max);

        void Rest();
        void SetRangeModifier( VPModifier *m );
        VPModifier* GetRangeModifier();

        /// \brief Apply internal transform to some external transform.

        /// Pre-multiply the internal transform to the argument. As VPTransform
        /// is a pre-multiplication matrix, the resulting transform has the effect
        /// of its previous value followed by the DOF's internal transform. This
        /// method is called by VPJoint when it is collection DOFs' transforms in
        /// order to compose its final transform.
        /// \param ptrTrans [in,out] where to apply internal transform.
        void ApplyTransformTo(VPTransform* ptrTrans) const;
#ifdef VISUAL_JOINTS
        bool DrawInstanceOGL() const;
    // PUBLIC STATIC ATTRIBUTES
        static float axisSize;
#endif
    // PUBLIC STATIC METHODS
        /// \brief Resets priorities of all DOF instances
        ///
        /// For every instance of VPDof, sets its priority to zero. Should be called at
        /// every render cycle, in a z-buffer-like scheme.
        static void ClearPriorities();
    protected:
    // PROTECTED METHODS
        void setAxis(VPVector3D ax);
        void ComputeLIM();
    // PROTECTED ATTRIBUTES
        /// Together with "axis", defines the rotation axis. Relative to the parent reference system.
        VPPoint4D position;
        /// Together with "position", defines the rotation axis. Relative to the parent reference system.
        VPPoint4D axis;
        /// \brief Priority of last DOF change
        ///
        /// When several elements try to update a DOF, the priority attribute controls
        /// which of them will really affect the DOF. Lower numbers mean lower priority.
        unsigned int priority;
    private:
    // PRIVATE ATTRIBUTES
        std::string description;// Name of the Dof; often related to the dof's type of motion
        VPBezier* evoluta; // 3D path related to the axis position along its rotation
        VPTransform lim; // Local Instance Matrix
        float minAngle;           // Min base angle in rad.
        float maxAngle;           // Max base angle in rad.
        float currentMinAngle;            // Min angle in rad currently valid.
        float currentMaxAngle;            // Max angle in rad currently valid.
        float confortMinAngle;            // Min angle in rad for comfortable position.
        float confortMaxAngle;            // Max angle in rad for comfortable position.
        VPModifier* rangeModifier; // Entity used to change the motion range along motion.
        float currentPosition; //A real number from 0 to 1
        float restPosition;           //Another real number from 0 to 1
        VPJoint* ownerJoint;            //Reference to the joint where this dof is set up
    // PRIVATE STATIC ATTRIBUTES
        // List of all instances of the class
        static std::list<VPDof*> instanceList;
};

#endif
