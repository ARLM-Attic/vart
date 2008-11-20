//deprecated
//! \file vpspotlight.h
//! \brief Header file for VPSpotLight VPAT class.
//! \version 1.1.1.2

// ChangeLog is at the implementation file

#ifndef __VPSPOTLIGHT_H
#define __VPSPOTLIGHT_H

#include "vplight.h"

//! \brief Spot Light representation.

//! A spot light is an ordinary light plus: attenuation, beamWidth, cutOffAngle,
//! direction and radius.
///\deprecated
class VPSpotLight : public VPLight {
    public:
        VPSpotLight(VPVector3D a, float bw, float coa,
                    VPPoint3D l, VPVector3D d, float r);
        void SetAttenuation( VPVector3D a );
        VPVector3D GetAttenuation( void );
        void SetBeamWidth( float bw );
        float GetBeamWidth( void );
        void SetCutOffAngle( float coa );
        float GetCutOffAngle( void );
        void SetDirection( VPVector3D d );
        VPVector3D GetDirection( void );
        void SetRadius( float r );
        float GetRadius( void );
    private:
        VPVector3D attenuation;
        float beamWidth;
        float cutOffAngle;
        VPVector3D direction;
        float radius;
};

#endif
