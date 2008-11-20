///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppoint2D.cpp
//  DESCRIPTION.: Contain the VPPoint2D class implementation that 
//                is responsible for the 2D point operations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/10/2000
//  DESCRIPTION.: Implementation of the class VPPoint2D methods.
//
///////////////////////////////////////////////////////////////////


#include <vppoint2d.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPPoint2D" constructor without parameter.
// Parameters.: -
// Return.....: -

VPPoint2D::VPPoint2D () {
	point[0] = 0.0;
	point[1] = 0.0;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPPoint2D" constructor with parameter.
// Parameters.: float xi (initial x value), 
//				float yi (initial y value)
// Return.....: -

VPPoint2D::VPPoint2D (float xi, float yi) {
	point[0] = xi;
	point[1] = yi;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetX" returns the value of x attribute.
// Parameters.: -
// Return.....: point[0] (value of x attribute)

float VPPoint2D::vpGetX() {
	return point[0];
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetY" returns the value of y attribute.
// Parameters.: -
// Return.....: point[1] (value of y attribute)

float VPPoint2D::vpGetY() {
	return point[1];
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetXY" set the value of x and y  
//				atributes to the parameters.
// Parameters.: float x (which receives the x value)
//				float y (which receives the y value)	
// Return.....: -

void VPPoint2D::vpGetXY(float &x, float &y) {
	x = point[0];
	y = point[1];
}



///////////////////////////////////////////////////////////////////

// Description: Method "vpGetPoint2D" get the point value.     

// Parameters.: -	

// Return.....: VPPoint2D (object)



VPPoint2D VPPoint2D::vpGetPoint2D(void) {

	return *this;

}




///////////////////////////////////////////////////////////////////
// Description: Method "vpSetX" sends a new value to the x attribute.
// Parameters.: float x (contains a new value for x attribute)
// Return.....: -		

void VPPoint2D::vpSetX(float x) {
	point[0] = x;
}
	

///////////////////////////////////////////////////////////////////
// Description: Method "vpSetY" sends a new value to the y attribute.
// Parameters.: float y (contains a new value for y attribute)
// Return.....: -		

void VPPoint2D::vpSetY(float y) {
	point[1] = y;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetXY" sends a new value to the x and y 
//				attribute.
// Parameters.: float x (contains a new value for x attribute)
//				float y (contains a new value for y attribute)
// Return.....: -		

void VPPoint2D::vpSetXY(float x, float y) {
	point[0] = x;
	point[1] = y;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPPoint2D p (object that has the new value for the
//					       attributes of the class)
// Return.....: VPPoint2D (current object)

VPPoint2D VPPoint2D::operator= (VPPoint2D p) {
	point[0] = p.point[0];
	point[1] = p.point[1];
	return *this;
}
