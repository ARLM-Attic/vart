//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcutplane.h
//  DESCRIPTION.: Contain the VPCutPlane class declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: December/12/2000
//  DESCRIPTION.: Class declaration.
//
//  AUTHOR......: Fernando Sola Pereira
//  DATE........: December/30/2000
//  DESCRIPTION.: Class update.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: March/01/2001
//  DESCRIPTION.: File and variables rename.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPCUTPLANE_H	
#define __VPCUTPLANE_H	


// To foresee several "includes"
#ifndef __GL_STUFF
#define __GL_STUFF	
#include <FL/Fl_Gl_Window.h> 
#include <FL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#endif	// __GL_STUFF

#include <vprotation.h>
#include <vptranslation.h>
#include <vpboundingbox.h>
#include <vpvector3d.h>
#include <vpcuttools.h>
#include <math.h>


///////////////////////////////////////////////////////////////////
// Class Name: VPCutPlane 
// Superclass: -
// Subclasses: 
///\deprecated
class VPCutPlane : public VPCutTools {
	private:
		float nD;
		VPBoundingBox boundingBox;
		VPVector3D normal;
		VPPoint3D plane[4];
		VPRotation rotation;
		VPTranslation translation;
	public:
		VPCutPlane();
		VPCutPlane(VPBoundingBox b);
		void vpSetBoundingBox(VPBoundingBox b);
		void vpNormalComputation(float x1,float y1,float z1,
								 float x2,float y2,float z2,
								 float x3,float y3,float z3);
		VPVector3D vpGetNormal(void);
		void vpPlaneComputation(void);
		void vpDComputation(void);
		float vpGetD(void);
		VPRotation* vpGetRotation(void);
		VPTranslation* vpGetTranslation(void);
		void vpRender(void);
};


#endif	// __VPCUTPLANE_H

