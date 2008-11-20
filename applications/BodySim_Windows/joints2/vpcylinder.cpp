///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcylinder.cpp
//  DESCRIPTION.: Contain the VPCylinder class implementation.
//
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 28/June/2000
//  DESCRIPTION.: Methods atualization.
//
///////////////////////////////////////////////////////////////////


#include	<vpcylinder.h>


VPCylinder::VPCylinder(){
	height = radius = 1.0;
        side = top = bottom = true;
}

VPCylinder::VPCylinder( float fHi, float fRad ){
	height = fHi;
        radius = fRad;
}

VPCylinder::VPCylinder( float fHi, float fRad, bool bS, bool bT, bool bB ){
	height = fHi;
        radius = fRad;
        side = bS;
        top = bT;
        bottom = bB;
}

void
VPCylinder::vpSetHeight( float h ){
	height = h;
}

void
VPCylinder::vpSetRadius( float r ){
	radius = r;
}

void
VPCylinder::vpShowSide( bool yesno ){
	side = yesno;
}

void
VPCylinder::vpShowTop( bool yesno ){
	top = yesno;
}

void
VPCylinder::vpShowBottom( bool yesno ){
	bottom = yesno;
}

float
VPCylinder::vpGetHeight( void ){
	return height;
}

float
VPCylinder::vpGetRadius( void ){
	return radius;
}

bool
VPCylinder::vpShowSide( void ){
	return side;
}

bool
VPCylinder::vpShowTop( void ){
	return top;
}

bool
VPCylinder::vpShowBottom( void ){
	return bottom;
}

void
VPCylinder::vpRender( void ){
	// meanwhile do nothing
		
}