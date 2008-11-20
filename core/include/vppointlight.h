//! \file vppointlight.h
//! \brief Header file for VPPointLight VPAT class.
//! \version 1.1.1.2

// ChangeLog is at the implementation file

#ifndef __VPPOINTLIGHT_H
#define __VPPOINTLIGHT_H

#include "vplight.h"

//! \brief Point light source.

//! This is a VPLight with attenuation.
class VPPointLight : public VPLight {
    private:
        VPVector3D attenuation;
    public:
        VPPointLight();
        VPPointLight(VPVector3D a, VPPoint4D l);
        void SetAttenuation( VPVector3D a );
        VPVector3D GetAttenuation();
};

#endif
