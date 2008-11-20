//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpdirectionallight.cpp
//  DESCRIPTION.: Contain the VPDirectionalLight class implementation.
//
//  AUTHOR......: Rodrigo Berggevist Martins
//  DATE........: June/19/2000
//  DESCRIPTION.: Constructors and some methods implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: April/17/2001
//  DESCRIPTION.: Constructor without parameter implementation.
//
///////////////////////////////////////////////////////////////////


#include <vpdirectionallight.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPDirectionalLight" constructor without 
//				parameter.
// Parameters.: -
// Return.....: -

VPDirectionalLight::VPDirectionalLight(){

}


///////////////////////////////////////////////////////////////////
// Description: Class "VPDirectionalLight" constructor with parameter.
// Parameters.: -
// Return.....: -

VPDirectionalLight::VPDirectionalLight(VPVector3D d){
	direction=d;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetDirection" sends a new value to the
//				direction attribute.
// Parameters.: VPVector3D d (contains a new value for intensity attribute)
// Return.....: -

void VPDirectionalLight::vpSetDirection(VPVector3D d){
	direction=d;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetDirection" returns the value of
//				direction attribute.
// Parameters.: -
// Return.....: VPVector3D direction (value of intensity attribute)

VPVector3D VPDirectionalLight::vpGetDirection( void ){
	return(direction);
}
