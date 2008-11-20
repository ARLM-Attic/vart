///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppointlight.cpp
//  DESCRIPTION.: Contain the VPPointLight class implementation.
//
//  AUTHOR......: Rodrigo Berggevist Martins
//  DATE........: June/19/2000
//  DESCRIPTION.: Constructors and some methods implementation.
//
///////////////////////////////////////////////////////////////////

#include <vppointlight.h>

///////////////////////////////////////////////////////////////////
// Description: Class "VPPointLight" constructor with parameter.
// Parameters.: -
// Return.....: -

VPPointLight::VPPointLight(VPVector3D a, VPPoint3D l){

	attenuation=a;
	location=l;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetAttenuation" sends a new value to the
//				attenuation attribute.
// Parameters.: VPVector3D a (contains a new value for intensity attribute)
// Return.....: -

void VPPointLight::vpSetAttenuation(VPVector3D a){

	attenuation=a;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetattenuation" returns the value of
//				attenuation attribute.
// Parameters.: -
// Return.....: VPVector3D attenuation (value of intensity attribute)

VPVector3D VPPointLight::vpGetAttenuation( void ){

	return(attenuation);
}

///////////////////////////////////////////////////////////////////
// Description: Method "vpSetLocation" sends a new value to the
//				location attribute.
// Parameters.: VPVector3D l (contains a new value for intensity attribute)
// Return.....: -

void VPPointLight::vpSetLocation(VPPoint3D l){

	location=l;
}

///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLocation" returns the value of
//				location attribute.
// Parameters.: -
// Return.....: VPVector3D location (value of intensity attribute)

VPPoint3D VPPointLight::vpGetLocation( void ){

	return(location);
}

