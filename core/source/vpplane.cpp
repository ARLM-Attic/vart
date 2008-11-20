//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpplane.cpp
//  DESCRIPTION.: Contain the VPPlane class implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/11/2000
//  DESCRIPTION.: Implementation of class methods.
//
///////////////////////////////////////////////////////////////////

#include <vpplane.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPPlane" constructor without parameter.
// Parameters.: -
// Return.....: -

VPPlane::VPPlane () {

}


///////////////////////////////////////////////////////////////////
// Description: Class "VPPlane" constructor with parameter.
// Parameters.: VPPlane *plane
// Return.....: -

VPPlane::VPPlane (VPPlane *plane) {
	normal = plane->normal;
	point = plane->point;
	size = plane->size;
	color = plane->color;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetNormal" sends a new value to the "normal"
//				attribute.
// Parameters.: VPVector3D n
// Return.....: -

void VPPlane::vpSetNormal(VPVector3D n) {
	normal = n;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetNormal" returns the value of "normal"
//				attribute.
// Parameters.: -
// Return.....: VPVector3D normal

VPVector3D VPPlane::vpGetNormal(void) {
	return normal;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetPoint" sends a new value to the "point"
//				attribute.
// Parameters.: VPVector3D p
// Return.....: -

void VPPlane::vpSetPoint(VPVector3D p) {
	point = p;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetPoint" returns the value of "point"
//				attribute.
// Parameters.: -
// Return.....: VPVector3D point

VPVector3D VPPlane::vpGetPoint(void) {
	return point;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetSize" sends a new value to the "size"
//				attribute.
// Parameters.: VPPoint2D s
// Return.....: -

void VPPlane::vpSetSize(VPPoint2D s) {
	size = s;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetSize" returns the value of "size"
//				attribute.
// Parameters.: -
// Return.....: VPPoint2D size

VPPoint2D VPPlane::vpGetSize(void) {
	return size;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetColor" sends a new value to the "color"
//				attribute.
// Parameters.: VPColor c
// Return.....: -

void VPPlane::vpSetColor(VPColor c) {
	color = c;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetColor" returns the value of "color"
//				attribute.
// Parameters.: -
// Return.....: VPColor color

VPColor VPPlane::vpGetColor(void) {
	return color;
}


///////////////////////////////////////////////////////////////////
// Description: ?????
// Parameters.: VPVector3D p
// Return.....: float

float VPPlane::TestPoint(VPVector3D p) {

	return (normal.vpGetVector3DX() * ( p.vpGetX() - point.vpGetX() ) +
			normal.vpGetVector3DY() * ( p.vpGetY() - point.vpGetY() ) +
			normal.vpGetVector3DZ() * ( p.vpGetZ() - point.vpGetZ() ) );
}


///////////////////////////////////////////////////////////////////
// Description: ?????
// Parameters.: VPVector3D p
//				VPVector3D dir
// Return.....: VPVector3D

VPVector3D VPPlane::TestLine(VPVector3D p, VPVector3D dir) {
VPVector3D v;
// FAZER
	return (v);
}


