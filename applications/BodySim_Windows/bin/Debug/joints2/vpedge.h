///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpedge.h
//  DESCRIPTION.: Contain the VPEdge class declarations.
//
//  AUTHOR......: Julierme Krüger Gavião
//  DATE........: June/07/2000
//  DESCRIPTION.: Class and methods declaration.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: August/22/2000
//  DESCRIPTION.: Class standarization.
//
///////////////////////////////////////////////////////////////////

#ifndef __VPEDGE_H			// Verify previous includes of vpvertex.h header
#define __VPEDGE_H

#include <vpvertex3d.h>


//-----------------------------------------------------------------------
// V P E D G E
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPEdge
// Superclass: -
// Subclass: -

class VPEdge {
	private:
		VPVertex3D initial, final;
		
	public:
		VPEdge( void );
		VPEdge( VPVertex3D i, VPVertex3D f);
		VPVertex3D *vpGetI( void );
		VPVertex3D *vpGetF( void );
		void vpSetI( VPVertex3D *i );
		void vpSetF( VPVertex3D *f );
};

#endif