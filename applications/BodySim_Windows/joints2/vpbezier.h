///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpbezier.h
//  DESCRIPTION.: Contain the VPBezier class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 10/August/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPBEZIER_H
#define __VPBEZIER_H

#include	<vpcurve.h>
#include	<vpvertex3d.h>


///////////////////////////////////////////////////////////////////
// Class Name: VPBezier
// Superclass: VPCurce
// Subclasses: none


class VPBezier : public VPCurve{

	private:
				
	public:
		VPBezier();
		VPBezier( VPVertex3D	*);
		//~VPBezier();
		VPVertex3D	vpGetControlPoint( int i );
		void				vpSetControlPoint( int i, VPVertex3D *n );
		VPPoint3D		vpGetPointAsPoint( float t );
		VPVertex3D	vpGetPointAsVertex( float t );
};

#endif