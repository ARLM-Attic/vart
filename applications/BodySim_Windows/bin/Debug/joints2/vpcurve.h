///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcurve.h
//  DESCRIPTION.: Contain the VPCurve class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 10/August/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPCURVE_H
#define __VPCURVE_H

#include	<vpgraphicobj.h>
#include	<vpvertex3d.h>

///////////////////////////////////////////////////////////////////
// Class Name: VPCurve
// Superclass: VPGraphicObj
// Subclasses: VPBezier


class VPCurve : public VPGraphicObj{

	private:
		VPVertex3D	*controlPoints;
		
	public:
		VPCurve();
		VPCurve( VPVertex3D	*);
		~VPCurve();
		void	vpSetControlPoints( VPVertex3D	* );
		VPVertex3D*	vpGetControlPoints( void );
		VPPoint3D		vpGetPointAsPoint( float t );
		VPVertex3D	vpGetPointAsVertex( float t );
		void	vpRender();

};

#endif