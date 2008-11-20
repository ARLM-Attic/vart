///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpsphere.cpp
//  DESCRIPTION.: Contain the VPSphere class implementation.
//
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 10/October/2000
//  DESCRIPTION.: Methods atualization.
//
///////////////////////////////////////////////////////////////////


#include	<vpsphere.h>


VPSphere::VPSphere(){
	radius = 1.0;
}

VPSphere::VPSphere( float fRad ){
        radius = fRad;
}

void
VPSphere::vpSetRadius( float r ){
	radius = r;
}

float
VPSphere::vpGetRadius( void ){
	return radius;
}

void
VPSphere::vpRender( void ){
		
	//meanwhile do nothing	
}