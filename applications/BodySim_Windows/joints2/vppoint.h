///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppoint.h
//  DESCRIPTION.: Contain the VPPoint class hierarchy declarations
//				  and the VPVector3D class declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/10/2000
//  DESCRIPTION.: Classes and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef __VPPOINT_H	
#define __VPPOINT_H	


///////////////////////////////////////////////////////////////////
// Class Name: VPPoint (abstract class)
// Superclass: -
// Subclasses: VPPoint2D, VPPoint3D


class VPPoint {
	public:
		virtual float vpGetX() {return 0;};
		virtual float vpGetY() {return 0;};	
		virtual float vpGetZ() {return 0;};
		virtual void vpGetXY(float &x, float &y) {};
		virtual void vpGetXZ(float &x, float &z) {};
		virtual void vpGetYZ(float &y, float &z) {};
		virtual void vpGetXYZ(float &x, float &y, float &z) {};
		virtual void vpSetX(float x) {};
		virtual void vpSetY(float y) {};	
		virtual void vpSetZ(float z) {};
		virtual void vpSetXY(float x, float y) {};
		virtual void vpSetXZ(float x, float z) {};
		virtual void vpSetYZ(float y, float z) {};
		virtual void vpSetXYZ(float x, float y, float z) {};
};


#endif	// __VPPOINT_H

