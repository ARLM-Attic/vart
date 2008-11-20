///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpvector3d.h
//  DESCRIPTION.: Contain the VPVector3D class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: August/22/2000
//  DESCRIPTION.: Classe and methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef __VPVERTEX3D_H
#define __VPVERTEX3D_H

#include <vppoint3d.h>
#include <vpvector3d.h>

enum {CL_RED, CL_GREEN, CL_BLUE, CL_APLHA};


/////////////////////////////////////////////////////////////////////////
// Class Name: VPVertex3D
// Superclass: VPPoint3D
// Subclass: -

class VPVertex3D: public VPPoint3D {
	private:
		short int color[4]; //Usar CL_RED, CL_GREEN, CL_BLUE e CL_ALPHA como índice
		VPVector3D normal;
	public:
		VPVertex3D ( void );
		VPVertex3D( float x, float y, float z);
		VPVertex3D( VPVertex3D *v);
		VPVertex3D( float x, float y, float z, short int color[4]);
		VPVertex3D( float x, float y, float z, VPPoint3D n);
		VPVertex3D( float x, float y, float z, short int c[4], VPVector3D n);
		short int *vpGetColor( void );
		VPVector3D *vpGetNormal( void );
		void vpSetColor (short int *c);
		void vpSetNormal (VPVector3D *n);
};

#endif