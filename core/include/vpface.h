//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpface.h
//  DESCRIPTION.: Contain the VPFace class declarations.
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

#ifndef __VPFACE_H				// Verify previous includes of vpmesh.h header
#define __VPFACE_H

#include <vpvertex3d.h>
#include <vpvector3d.h>
#include <vpedge.h>
#include <list>	//STL include
using namespace std;

//-----------------------------------------------------------------------
// V P F A C E
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPFace
// Superclass: -
// Subclass: -
///\deprecated

class VPFace {
	private:
		list<VPVertex3D> vertices;
		list<VPEdge> edges;
		VPPoint3D normal;
		short int color[4]; //Usar CL_RED, CL_GREEN, CL_BLUE e CL_ALPHA como índice
	public:
		VPFace ( void );
		VPFace (int numvertices, VPVertex3D* ...);
		VPFace (int numedges, VPEdge* ...); //Variable number of vertices and edges
		VPFace (list<VPVertex3D> *v);
		VPFace (list<VPEdge> *e);
		short int *vpGetColor( void );
		VPVector3D *vpGetNormal( void );
		void vpSetColor (short int *c);
		void vpSetNormal (VPVector3D *n);
};

#endif
