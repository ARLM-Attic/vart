///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppoint3D.cpp
//  DESCRIPTION.: Contain the VPPoint3D class implementation that 
//                is responsible for the 3D point operations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/10/2000
//  DESCRIPTION.: Implementation of the class VPPoint3D methods.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: January/20/2000
//  DESCRIPTION.: New overload methods implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: April/18/2001
//  DESCRIPTION.: Change of attributes.
//
///////////////////////////////////////////////////////////////////


#include <cmath>
#include <vppoint3d.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPPoint3D" constructor without parameter.
// Parameters.: -
// Return.....: -

VPPoint3D::VPPoint3D () {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPPoint3D" constructor with parameter.
// Parameters.: float xi (initial x value), 
//				float yi (initial y value),
//				float zi (initial z value)
// Return.....: -

VPPoint3D::VPPoint3D (float xi, float yi, float zi) {
	x = xi;
	y = yi;
	z = zi;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetX" returns the value of x attribute.
// Parameters.: -
// Return.....: x (value of x attribute)

inline float VPPoint3D::vpGetX() {
	return x;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetY" returns the value of y attribute.
// Parameters.: -
// Return.....: y (value of y attribute)

inline float VPPoint3D::vpGetY() {
	return y;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetZ" returns the value of z attribute.
// Parameters.: -
// Return.....: z (value of z attribute)

inline float VPPoint3D::vpGetZ() {
	return z;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetXY" set the value of x and y  
//				atributes to the parameters.
// Parameters.: float xx (which receives the x value)
//				float yy (which receives the y value)	
// Return.....: -

void VPPoint3D::vpGetXY(float &xx, float &yy) {
	xx = x;
	yy = y;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetXZ" set the value of x and z  
//				atributes to the parameters.
// Parameters.: float xx (which receives the x value)
//				float zz (which receives the z value)	
// Return.....: -

void VPPoint3D::vpGetXZ(float &xx, float &zz) {
	xx = x;
	zz = z;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetYZ" set the value of y and z  
//				atributes to the parameters.
// Parameters.: float yy (which receives the y value)
//				float zz (which receives the z value)	
// Return.....: -

void VPPoint3D::vpGetYZ(float &yy, float &zz) {
	yy = y;
	zz = z;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetXYZ" set the value of x, y and z  
//				atributes to the parameters.
// Parameters.: float xx (which receives the x value)
//				float yy (which receives the y value)
//				float zz (which receives the z value)	
// Return.....: -

void VPPoint3D::vpGetXYZ(float &xx, float &yy, float &zz) {
	xx = x;	
	yy = y;
	zz = z;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetPoint3D" get the point value.     
// Parameters.: -	
// Return.....: VPPoint3D (object)

VPPoint3D VPPoint3D::vpGetPoint3D(void) {
	return *this;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetPointAsArray" get the point value.     
// Parameters.: -	
// Return.....: float * (pointer to the array)

float* VPPoint3D::vpGetPointAsArray( void ) {
	float* array = new float[3]; 
	array[0] = x; 
	array[1] = y; 
	array[2] = z; 
	return array;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetPoint" put the point value in an array.     
// Parameters.: float v	
// Return.....: -

void VPPoint3D::vpGetPoint(float v[]) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetX" sends a new value to the x attribute.
// Parameters.: float xx (contains a new value for x attribute)
// Return.....: -		

inline void VPPoint3D::vpSetX(float xx) {
	x = xx;
}
	

///////////////////////////////////////////////////////////////////
// Description: Method "vpSetY" sends a new value to the y attribute.
// Parameters.: float yy (contains a new value for y attribute)
// Return.....: -		

inline void VPPoint3D::vpSetY(float yy) {
	y = yy;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetZ" sends a new value to the z attribute.
// Parameters.: float z (contains a new value for z attribute)
// Return.....: -		

inline void VPPoint3D::vpSetZ(float zz) {
	z = zz;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetXY" sends a new value to the x and y 
//				attribute.
// Parameters.: float xx (contains a new value for x attribute)
//				float yy (contains a new value for y attribute)
// Return.....: -		

void VPPoint3D::vpSetXY(float xx, float yy) {
	x = xx;
	y = yy;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetXZ" sends a new value to the x and z 
//				attribute.
// Parameters.: float xx (contains a new value for x attribute)
//				float zz (contains a new value for z attribute)
// Return.....: -		

void VPPoint3D::vpSetXZ(float xx, float zz) {
	x = xx;
	z = zz;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetYZ" sends a new value to the y and z 
//				attribute.
// Parameters.: float yy (contains a new value for y attribute)
//				float zz (contains a new value for z attribute)
// Return.....: -		

void VPPoint3D::vpSetYZ(float yy, float zz) {
	y = yy;
	z = zz;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetXYZ" sends a new value to the x, y and  
//				z attribute.
// Parameters.: float xx (contains a new value for x attribute)
//				float yy (contains a new value for y attribute)
//				float zz (contains a new value for z attribute)
// Return.....: -		

void VPPoint3D::vpSetXYZ(float xx, float yy, float zz) {
	x = xx;
	y = yy;
	z = zz;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of - operator
// Parameters.: VPPoint3D p (object that has the value for the
//					         operation)
// Parameters.: const VPPoint3D& p
// Return.....: VPPoint3D (result from "-" operation)

VPPoint3D VPPoint3D::operator- (const VPPoint3D& p) {
 VPPoint3D vv(x-p.x, y-p.y, z-p.z);
 return ( vv ) ;
} 


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of + operator
// Parameters.: VPPoint3D p (object that has the value for the
//					         operation)
// Parameters.: const VPPoint3D& p
// Return.....: VPPoint3D (result from "+" operation)

VPPoint3D VPPoint3D::operator+ (const VPPoint3D& p) {
 VPPoint3D vv(x+p.x, y+p.y, z+p.z);
 return ( vv ) ;
} 


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPPoint3D p (object that has the new value for the
//					       attributes of the class)
// Parameters.: const VPPoint3D& p
// Return.....: VPPoint3D (current object)

VPPoint3D VPPoint3D::operator= (VPPoint3D p) {
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of -= operator
// Parameters.: int num (number for subtraction)
// Return.....: VPPoint3D (current object)

VPPoint3D VPPoint3D::operator-= (int num) {
 x -= num;
 y -= num;
 z -= num;
 return *this;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of -= operator
// Parameters.: float num (number for multiplication)
// Return.....: VPPoint3D (current object)

VPPoint3D VPPoint3D::operator* (float num) {
 x *= num;
 y *= num;
 z *= num;
 return *this;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of != operator
// Parameters.: const VPPoint3D& p (object to be compared)
// Return.....: true or false

bool VPPoint3D::operator!= (const VPPoint3D& p) {
	if ( (x!=p.x) || (y!=p.y) || (z!=p.z) )
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of == operator
// Parameters.: const VPPoint3D p (object to be compared)
// Return.....: true or false

bool VPPoint3D::operator== (const VPPoint3D& p) {
	if ( (x==p.x) && (y==p.y) && (z==p.z) )
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of <= operator
// Parameters.: const VPPoint3D p (object to be compared)
// Return.....: true or false

bool VPPoint3D::operator<= (const VPPoint3D& p) {
	if ( (x<=p.x) && (y<=p.y) && (z<=p.z) )
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of < operator
// Parameters.: const VPPoint3D p (object to be compared)
// Return.....: true or false

bool VPPoint3D::operator< (const VPPoint3D& p) {
	if ( (x<p.x) && (y<p.y) && (z<p.z) )
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of >= operator
// Parameters.: const VPPoint3D p (object to be compared)
// Return.....: true or false

bool VPPoint3D::operator>= (const VPPoint3D& p) {
	if ( (x>=p.x) && (y>=p.y) && (z>=p.z) )
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of > operator
// Parameters.: const VPPoint3D p (object to be compared)
// Return.....: true or false

bool VPPoint3D::operator> (const VPPoint3D& p) {
	if ( (x>p.x) && (y>p.y) && (z>p.z) )
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////
// Description: Method that calculates the distance between two points
// Parameters.: VPPoint3D p 
// Return.....: float distance

float VPPoint3D::vpDistance(VPPoint3D p1) {
	float d;
	d = (float)sqrt((x-p1.x)*(x-p1.x) +
			 (y-p1.y)*(y-p1.y) +
			 (z-p1.z)*(z-p1.z));
	return d;
}


