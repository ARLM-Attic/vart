///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpsphere.h
//  DESCRIPTION.: Contain the VPSphere class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 10/August/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPSPHERE_H
#define __VPSPHERE_H

#include	<vpgraphicobj.h>


///////////////////////////////////////////////////////////////////
// Class Name: VPSphere
// Superclass: VPGraphicObj
// Subclasses: none


class VPSphere : public VPGraphicObj{

	private:
		float  radius;
		
	public:
		VPSphere();
		VPSphere( float fRad );
		void vpSetRadius( float r );
		float vpGetRadius( void );
		void vpRender( void );
	
};

#endif