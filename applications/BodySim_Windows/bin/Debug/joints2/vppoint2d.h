///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppoint2d.h
//  DESCRIPTION.: Contain the VPPoint2D class declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: August/10/2000
//  DESCRIPTION.: Classes and new methods declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPPOINT2D_H	
#define __VPPOINT2D_H	

#include	<vppoint.h>

///////////////////////////////////////////////////////////////////
// Class Name: VPPoint2D (storage and manipulation of 2D points)
// Superclass: VPPoint
// Subclasses: -

class VPPoint2D : public VPPoint {
	private:
		float point[2];
    public:
		VPPoint2D();
		VPPoint2D(float xi, float yi);
		float vpGetX();
		float vpGetY();
		void vpGetXY(float &x, float &y);

		VPPoint2D vpGetPoint2D(void);
		void vpSetX(float x);
		void vpSetY(float y);
		void vpSetXY(float x, float y);
		VPPoint2D operator= (VPPoint2D p);
};

#endif