/// \file vpsphere.h
/// \brief Header file for VPSphere VPAT class.
/// \version 1.5

// ChangeLog is at the implementation file

#ifndef __VPSPHERE_H
#define __VPSPHERE_H

#include "vpgraphicobj.h"
#include "vpmaterial.h"

/// \class VPSphere vpsphere.h
/// \brief A sphere.
///
/// Spheres are created with center at the origin.
class VPSphere : public VPGraphicObj {
    public:
        VPSphere();
        VPSphere( float fRad );
        void SetRadius( float r );
        /// Material assignment.
        void SetMaterial(const VPMaterial& mat) { material = mat; }
        /// Returns of copy of the cylinder's material.
        VPMaterial GetMaterial() const { return material; }
        float GetRadius();
        /// \brief Draws the sphere using OpenGL engine.
        /// \return false if VPAT has not been compiled with OpenGL support.
        bool DrawInstanceOGL() const;
        virtual void ComputeBoundingBox();
    private:
        VPMaterial material;
        float radius;
};

#endif
