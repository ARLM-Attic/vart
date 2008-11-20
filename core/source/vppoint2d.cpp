//deprecated
/////////////////////////////////////////////////////////////////////
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
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: September/06/2001
//  DESCRIPTION.: Attributes change for "x", "y", and public.
//
///////////////////////////////////////////////////////////////////

#include <vppoint2d.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPPoint2D" constructor without parameter.
// Parameters.: -
// Return.....: -

VPPoint2D::VPPoint2D () {
	x = 0.0;
	y = 0.0;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPPoint2D" constructor with parameter.
// Parameters.: float xi (initial x value), 
//				float yi (initial y value)
// Return.....: -

VPPoint2D::VPPoint2D (float xi, float yi) {
	x = xi;
	y = yi;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetX" returns the value of x attribute.
// Parameters.: -
// Return.....: x (value of x attribute)

float VPPoint2D::vpGetX() {
	return x;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetY" returns the value of y attribute.
// Parameters.: -
// Return.....: y(value of y attribute)

float VPPoint2D::vpGetY() {
	return y;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetXY" set the value of x and y  
//				atributes to the parameters.
// Parameters.: float x (which receives the x value)
//				float y (which receives the y value)	
// Return.....: -

void VPPoint2D::vpGetXY(float &_x, float &_y) {
	_x = x;
	_y = y;
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

void VPPoint2D::vpSetX(float _x) {
	x = _x;
}
	

///////////////////////////////////////////////////////////////////
// Description: Method "vpSetY" sends a new value to the y attribute.
// Parameters.: float y (contains a new value for y attribute)
// Return.....: -		

void VPPoint2D::vpSetY(float _y) {
	y = _y;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetXY" sends a new value to the x and y 
//				attribute.
// Parameters.: float x (contains a new value for x attribute)
//				float y (contains a new value for y attribute)
// Return.....: -		

void VPPoint2D::vpSetXY(float _x, float _y) {
	x = _x;
	y = _y;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPPoint2D p (object that has the new value for the
//					       attributes of the class)
// Return.....: VPPoint2D (current object)

VPPoint2D VPPoint2D::operator= (VPPoint2D p) {
	x = p.x;
	y = p.y;
	return *this;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: int num (number that will be subtracted)
// Return.....: VPPoint2D (current object)

VPPoint2D VPPoint2D::operator- (int num) {
	x = x - num;
	y = y - num;
	return *this;
}
