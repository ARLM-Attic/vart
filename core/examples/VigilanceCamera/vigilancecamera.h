/// \file vigilancecamera.h
/// \brief Example class for VPAT (headers)
/// \version $Revision: 1.3 $

// ChangeLog is at the implementation file

#ifndef VIGILANCECAMERA_H
#define VIGILANCECAMERA_H

#include <vpgraphicobj.h>
#include <vpcylinder.h>
#include <vpsphere.h>
#include <vpbiaxialjoint.h>

class clVigilanceCamera : public VPGraphicObj
{
    public:
        clVigilanceCamera();
        void Flex(float variance);
        void Adduct(float variance);
        void SimulationStep(); // FixMe: Not yet implemented
        /// Computes the bounding box.
        virtual void ComputeBoundingBox();
    private:
    // PRIVATE METHODS
        virtual bool DrawInstanceOGL() const;
    // PRIVATE ATTRIBUTES
        VPCylinder base;
        VPSphere globe;
        VPTransform globeTrans;
        VPCylinder lens;
        VPTransform lensTrans;

        // First DOF is along the Z axis, passing by the center of the globe, ranging
        // from -PI to PI. Z axis is relative to the vigilance camera, and will probably
        // be the Y axis on world coordinates. Therefore, the vigilance camera has full
        // 360 degrees of freedom to look around
        VPDof flexion;
        // Second DOF is along the X axis, passing by the center of the globe, ranging from
        // 0 to PI/2. The local X axis will probably be horizontal in world coordinates
        // therefore the vigilance camera may turn directly to the floor (0 degrees), or
        // aim at the horizon (90 degrees).
        VPDof aduction;
        VPBiaxialJoint joint;
};

#endif
