//! \file spotlight.cpp
//! \brief Implementation file for V-ART class "SpotLight".
//! \version $Revision: 1.1 $

#include "vart/spotlight.h"

VART::SpotLight::SpotLight(VART::Vector3D a, float bw, float coa,
                         VART::Point3D l, VART::Vector3D d, float r){
    attenuation=a;
    beamWidth=bw;
    cutOffAngle=coa;
    location=l;
    direction=d;
    radius=r;
}

void VART::SpotLight::SetAttenuation(VART::Vector3D a){
    attenuation=a;
}

VART::Vector3D VART::SpotLight::GetAttenuation(){
    return(attenuation);
}

void VART::SpotLight::SetBeamWidth(float bw){
    beamWidth=bw;
}

float VART::SpotLight::GetBeamWidth(){
    return(beamWidth);
}

void VART::SpotLight::SetCutOffAngle(float coa){
    cutOffAngle=coa;
}

float VART::SpotLight::GetCutOffAngle(){
    return(cutOffAngle);
}

void VART::SpotLight::SetDirection(VART::Vector3D d) {
    direction = d;
}

VART::Vector3D VART::SpotLight::GetDirection(){
    return(direction);
}

void VART::SpotLight::SetRadius(float r){
    radius=r;
}

float VART::SpotLight::GetRadius(){
    return(radius);
}
