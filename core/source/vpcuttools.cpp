//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcuttools.cpp
//  DESCRIPTION.: Contain the VPCutTools class implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: December/12/2000
//  DESCRIPTION.: Implementation of class methods.
//
///////////////////////////////////////////////////////////////////


#include <vpcuttools.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPCutTools" constructor without parameter. 
// Parameters.: -
// Return.....: -

VPCutTools::VPCutTools () {

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetPointLeft" sends a new value to the 
//				"left" attribute.
// Parameters.: VPPoint3D l
// Return.....: -

void VPCutTools::vpSetPointLeft(VPPoint3D l) {
	left = l;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetPointLeft" returns the value of "left"
//				attribute.
// Parameters.: -
// Return.....: VPPoint3D left

VPPoint3D VPCutTools::vpGetPointLeft(void) {
	return left;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetPointRight" sends a new value to the 
//				"right" attribute.
// Parameters.: VPPoint3D r
// Return.....: -

void VPCutTools::vpSetPointRight(VPPoint3D r) {
	right = r;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetPointRight" returns the value of "right"
//				attribute.
// Parameters.: -
// Return.....: VPPoint3D right

VPPoint3D VPCutTools::vpGetPointRight(void) {
	return right;
}

