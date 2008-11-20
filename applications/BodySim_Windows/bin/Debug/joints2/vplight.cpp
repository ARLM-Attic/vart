///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vplight.cpp
//  DESCRIPTION.: Contain the VPLight class implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: May/30/2000
//  DESCRIPTION.: Constructors and some methods implementation.
//
//  AUTHOR......: Rodrigo Berggevist Martins
//  DATE........: June/19/2000
//  DESCRIPTION.: Constructors and some methods implementation.
//
///////////////////////////////////////////////////////////////////


#include <vplight.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPLight" constructor without parameter.
// Parameters.: -
// Return.....: -

VPLight::VPLight(): color(1, 1, 1, 1) {
	intensity = 1;
	ambientIntensity = 1;
	on = ON;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPLight" constructor with parameter.
// Parameters.: -
// Return.....: -

VPLight::VPLight (float i, float ai, VPColor c, bool o) {
	intensity=i;
	ambientIntensity=ai;
	color=c;
	on=o;
}

///////////////////////////////////////////////////////////////////
// Description: Desctructor of VPLight class.
// Parameters.: -
// Return.....: -

VPLight::~VPLight() {
}

///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: -
// Return.....: -

std::string VPLight::vpGetDescription( void ){
	return description;
}


///////////////////////////////////////////////////////////////////
// Description: 
// Parameters.: -
// Return.....: -

void VPLight::vpSetDescription(std::string desc){
	description = desc;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetIntensity" sends a new value to the
//				intensity attribute.
// Parameters.: float i (contains a new value for intensity attribute)
// Return.....: -

void VPLight::vpSetIntensity (float i) {
	intensity = i;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetIntensity" returns the value of
//				intensity attribute.
// Parameters.: -
// Return.....: float intensity(value of intensity attribute)

float VPLight::vpGetIntensity ( void ) {
	return(intensity);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetAmbientIntensity" sends a new value to the
//				ambientIntensity attribute.
// Parameters.: float ai (contains a new value for ambientIntensity attribute)
// Return.....: -

void VPLight::vpSetAmbientIntensity(float ai){
	ambientIntensity = ai;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetAmbientIntensity" returns the value of
//				ambientIntensity attribute.
// Parameters.: -
// Return.....: float ambientIntensity(value of ambientIntensity attribute)

float VPLight::vpGetAmbientIntensity( void ){
	return(ambientIntensity);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetColor" sends a new value to the
//				color attribute.
// Parameters.: VPColor c (contains a new value for color attribute)
// Return.....: -

void VPLight::vpSetColor(VPColor c){
	color = c;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetColor" returns the value of
//				color attribute.
// Parameters.: -
// Return.....:VPColor color(value of color attribute)

VPColor VPLight::vpGetColor( void ){
	return(color);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpTurn" sends a new value to the
//				on attribute.
// Parameters.:boolean o (contains a new value for on attribute)
// Return.....: -

void VPLight::vpTurn( bool o ){
	on=o;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetOn" returns the value of
//				on attribute.
// Parameters.: -
// Return.....:boolean on(value of on attribute)

bool VPLight::vpIsOn( void ){
	return on;
}
