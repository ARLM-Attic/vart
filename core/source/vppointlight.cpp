//! \file vppointlight.cpp
//! \brief Implementation file for VPPointLight VPAT class.
//! \version 1.1.1.2

// ChangeLog
// September 16, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Removed "vp" prefix from every method name.
// - Removed "location" attribute because it was added to VPLight.
// - Removed the methods: SetLocation, GetLocation.
// March 08, 2001 - Isabel H. Manssour
// - Constructor declaration addition.
// August 10, 2000 - Anderson Maciel
// - Classes declaration and methods addition.
// June 19, 2000 - Rodrigo Berggevist Martins
// - Classes declaration.
// - Constructors and some methods implementation.
// June 08, 2000 - Luciana Porcher Nedel
// - Classes declaration.
// May 22, 2000 - Isabel Harb Manssour
// - Classes declaration.

#include "vppointlight.h"

VPPointLight::VPPointLight(VPVector3D a, VPPoint4D l){
    attenuation=a;
    location=l;
}

void VPPointLight::SetAttenuation(VPVector3D a){
    attenuation=a;
}

VPVector3D VPPointLight::vpGetAttenuation(){
    return(attenuation);
}
