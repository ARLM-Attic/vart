///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpvector3d.h
//  DESCRIPTION.: Contain the VPVector3D class declarations.
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

#ifndef __VPMESH_H				// Verify previous includes of vpmesh.h header
#define __VPMESH_H

#include <vpvertex3d.h>
#include <vpedge.h>
#include <vpface.h>
#include <list>	//STL include
using namespace std;

/////////////////////////////////////////////////////////////////////////
// Class Name: VPMesh
// Superclass: -
// Subclass: -

class VPMesh {
	private:
		std::list<VPVertex3D> vertices;
		std::list<VPEdge> edges;
		std::list<VPFace> faces;
	public:
		VPMesh ( void );
		VPMesh (std::list<VPVertex3D> *vertices);
		VPMesh (std::list<VPEdge> *edges);
		VPMesh (std::list<VPFace> *faces);
		VPMesh (std::list<VPVertex3D> *vertices, std::list<VPFace> *faces); //Are edges needed?
		VPMesh (std::list<VPVertex3D> *vertices, std::list<VPEdge> *edges, std::list<VPFace> *faces);
};

#endif