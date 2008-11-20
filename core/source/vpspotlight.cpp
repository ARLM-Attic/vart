//! \file vpspotlight.cpp
//! \brief Implementation file for VPSpotLight VPAT class.
//! \version 1.1.1.2

// ChangeLog
// September 16, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Removed "vp" prefix from every method name.
// - Removed "location" attribute because it was added to VPLight.
// - Removed the methods: SetLocation, GetLocation.
// August 10, 2000 - Anderson Maciel
// - Classes declaration and methods addition.
// - Includes and text format.
// June 19, 2000 - Rodrigo Berggevist Martins
// - Classes declaration.
// - Constructors and some methods implementation.
// June 08, 2000 - Luciana Porcher Nedel
// - Classes declaration.
// May 22, 2000 - Isabel Harb Manssour
// - Classes declaration.

#include "vpspotlight.h"

VPSpotLight::VPSpotLight(VPVector3D a, float bw, float coa,
                         VPPoint3D l, VPVector3D d, float r){
    attenuation=a;
    beamWidth=bw;
    cutOffAngle=coa;
    location=l;
    direction=d;
    radius=r;
}

void VPSpotLight::SetAttenuation(VPVector3D a){
    attenuation=a;
}

VPVector3D VPSpotLight::GetAttenuation(){
    return(attenuation);
}

void VPSpotLight::SetBeamWidth(float bw){
    beamWidth=bw;
}

float VPSpotLight::GetBeamWidth(){
    return(beamWidth);
}

void VPSpotLight::SetCutOffAngle(float coa){
    cutOffAngle=coa;
}

float VPSpotLight::GetCutOffAngle(){
    return(cutOffAngle);
}

void VPSpotLight::SetDirection(VPVector3D d) {
    direction = d;
}

VPVector3D VPSpotLight::GetDirection(){
    return(direction);
}

void VPSpotLight::SetRadius(float r){
    radius=r;
}

float VPSpotLight::GetRadius(){
    return(radius);
}
