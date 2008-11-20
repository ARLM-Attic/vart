///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppoint3d.h
//  DESCRIPTION.: Contain the VPPoint3D class declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: August/10/2000
//  DESCRIPTION.: Classes and new methods declaration.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: January/20/2000
//  DESCRIPTION.: New overload methods declaration.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: April/18/2001
//  DESCRIPTION.: Change of attributes.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPPOINT3D_H	
#define __VPPOINT3D_H	

#include	<vppoint.h>



///////////////////////////////////////////////////////////////////
// Class Name: VPPoint3D (storage and manipulation of 3D points)
// Superclass: VPPoint
// Subclasses: -

class VPPoint3D : public VPPoint {
    public:
		float x;
		float y;
		float z;
		VPPoint3D();
		VPPoint3D(float x, float y, float z);
		float vpGetX();
		float vpGetY();
		float vpGetZ();
		void vpGetXY(float &xx, float &yy);
		void vpGetXZ(float &xx, float &zz);
		void vpGetYZ(float &yy, float &zz);
		void vpGetXYZ(float &xx, float &yy, float &zz);

		VPPoint3D vpGetPoint3D(void);
		float* vpGetPointAsArray(void);
		void vpGetPoint(float v[]);
		void vpSetX(float xx);
		void vpSetY(float yy);
		void vpSetZ(float zz);
		void vpSetXY(float xx, float yy);
		void vpSetXZ(float xx, float zz);
		void vpSetYZ(float yy, float zz);
		void vpSetXYZ(float xx, float yy, float zz);
		VPPoint3D operator= (VPPoint3D p);
		VPPoint3D operator- (const VPPoint3D&);
		VPPoint3D operator+ (const VPPoint3D&);
		VPPoint3D operator* (float num);
		VPPoint3D operator-= (int num);
		bool operator!= (const VPPoint3D&);
		bool operator== (const VPPoint3D&);
		bool operator<= (const VPPoint3D&);
		bool operator< (const VPPoint3D&);
		bool operator>= (const VPPoint3D&);
		bool operator> (const VPPoint3D&);
		float vpDistance(VPPoint3D p1);
};

#endif