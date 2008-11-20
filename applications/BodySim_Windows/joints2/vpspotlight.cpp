///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpspotlight.cpp
//  DESCRIPTION.: Contain the VPSpotLight class implementation.
//
//  AUTHOR......: Rodrigo Berggevist Martins
//  DATE........: June/19/2000
//  DESCRIPTION.: Constructors and some methods implementation.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: August/10/2000
//  DESCRIPTION.: Includes and text format.
//
///////////////////////////////////////////////////////////////////


#include <vpspotlight.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPSpotLight" constructor with parameter.
// Parameters.: -
// Return.....: -


VPSpotLight::VPSpotLight(VPVector3D a, float bw, float coa, 

						 VPPoint3D l, VPVector3D d, float r){
	
	attenuation=a;
	beamWidth=bw;
	cutOffAngle=coa;
	location=l;
	direction=d;
	radius=r;

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetAttenuation" sends a new value to the
//				attenuation attribute.
// Parameters.: VPVector3D a (contains a new value for intensity attribute)
// Return.....: -

void VPSpotLight::vpSetAttenuation(VPVector3D a){

	attenuation=a;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetAttenuation" returns the value of
//				attenuation attribute.
// Parameters.: -
// Return.....: VPVector3D attenuation (value of intensity attribute)

VPVector3D VPSpotLight::vpGetAttenuation(){

	return(attenuation);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetBeamWidth" sends a new value to the
//				beamWidth attribute.
// Parameters.: float bw (contains a new value for intensity attribute)
// Return.....: -

void VPSpotLight::vpSetBeamWidth(float bw){

	beamWidth=bw;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetBeamWidth" returns the value of
//				beamWidth attribute.
// Parameters.: -
// Return.....: float beamWidth (value of intensity attribute)

float VPSpotLight::vpGetBeamWidth(){

	return(beamWidth);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetCutOffAngle" sends a new value to the
//				cutOffAngle attribute.
// Parameters.: float coa (contains a new value for intensity attribute)
// Return.....: -

void VPSpotLight::vpSetCutOffAngle(float coa){

	cutOffAngle=coa;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetCutOffAngle" returns the value of
//				cutOffAngle attribute.
// Parameters.: -
// Return.....: float cutOffAngle (value of intensity attribute)

float VPSpotLight::vpGetCutOffAngle(){

	return(cutOffAngle);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetLocation" sends a new value to the
//				location attribute.
// Parameters.: VPVector3D l (contains a new value for intensity attribute)
// Return.....: -

void VPSpotLight::vpSetLocation(VPPoint3D l){

	location=l;
}

///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLocation" returns the value of
//				location attribute.
// Parameters.: -
// Return.....: VPPoint3D location (value of intensity attribute)

VPPoint3D VPSpotLight::vpGetLocation( void ){

	return(location);
}

///////////////////////////////////////////////////////////////////
// Description: Method "vpSetDirection" sends a new value to the
//				direction attribute.
// Parameters.: VPVector3D d (contains a new value for intensity attribute)
// Return.....: -

void VPSpotLight::vpSetDirection(VPVector3D d) {

	direction = d;
}

///////////////////////////////////////////////////////////////////
// Description: Method "vpGetDirection" returns the value of
//				direction attribute.
// Parameters.: -
// Return.....: VPVector3D direction (value of intensity attribute)

VPVector3D VPSpotLight::vpGetDirection(){

	return(direction);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRadius" sends a new value to the
//				radius attribute.
// Parameters.: float r (contains a new value for intensity attribute)
// Return.....: -

void VPSpotLight::vpSetRadius(float r){

	radius=r;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRadius" returns the value of
//				radius attribute.
// Parameters.: -
// Return.....: float radius (value of intensity attribute)

float VPSpotLight::vpGetRadius(){

	return(radius);
}