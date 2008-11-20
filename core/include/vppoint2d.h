//deprecated
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
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: September/06/2001
//  DESCRIPTION.: Attributes change for "x", "y", and public.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPPOINT2D_H	
#define __VPPOINT2D_H	

#include	<vppoint.h>

///////////////////////////////////////////////////////////////////
// Class Name: VPPoint2D (storage and manipulation of 2D points)
// Superclass: VPPoint
// Subclasses: -
///\deprecated

class VPPoint2D : public VPPoint {
	private:
    public:
		float x;
		float y;
		VPPoint2D();
		VPPoint2D(float xi, float yi);
		float vpGetX();
		float vpGetY();
		void vpGetXY(float &_x, float &_y);

		VPPoint2D vpGetPoint2D(void);
		void vpSetX(float _x);
		void vpSetY(float _y);
		void vpSetXY(float _x, float _y);
		VPPoint2D operator= (VPPoint2D p);
		VPPoint2D operator- (int num);

};

#endif
