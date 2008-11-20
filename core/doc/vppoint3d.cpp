///////////////////////////////////////////////////////////////////
///  \file               vppoint3d.cpp
///  \brief              Contain the VPPoint3D class declarations.	
///  \version            0.1
///////////////////////////////////////////////////////////////////
//  Change log:
//  August 10/2000 Isabel Harb Manssour <isabel @ provedor.com>
//  - Classes and new methods declaration.
//  January 20/2000 Isabel Harb Manssour
//  - New overload methods declaration.
//  April 18/2001 Isabel Harb Manssour
//  - Change of attributes.


#include <cmath>
#include <vppoint3d.h>

VPPoint3D::VPPoint3D () {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

VPPoint3D::VPPoint3D (float xi, float yi, float zi) {
	x = xi;
	y = yi;
	z = zi;
}

inline float VPPoint3D::vpGetX() {
	return x;
}

inline float VPPoint3D::vpGetY() {
	return y;
}

inline float VPPoint3D::vpGetZ() {
	return z;
}

void VPPoint3D::vpGetXY(float &xx, float &yy) {
	xx = x;
	yy = y;
}

void VPPoint3D::vpGetXZ(float &xx, float &zz) {
	xx = x;
	zz = z;
}

void VPPoint3D::vpGetYZ(float &yy, float &zz) {
	yy = y;
	zz = z;
}

void VPPoint3D::vpGetXYZ(float &xx, float &yy, float &zz) {
	xx = x;	
	yy = y;
	zz = z;
}

VPPoint3D VPPoint3D::vpGetPoint3D(void) {
	return *this;
}

float* VPPoint3D::vpGetPointAsArray( void ) 
{
	float* array = new float[3]; 
	array[0] = x; 
	array[1] = y; 
	array[2] = z; 
	return array;
}

void VPPoint3D::vpGetPoint(float v[]) 
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

inline void VPPoint3D::vpSetX(float xx) {
	x = xx;
}
	
inline void VPPoint3D::vpSetY(float yy) {
	y = yy;
}

inline void VPPoint3D::vpSetZ(float zz) {
	z = zz;
}

void VPPoint3D::vpSetXY(float xx, float yy) {
	x = xx;
	y = yy;
}

void VPPoint3D::vpSetXZ(float xx, float zz) {
	x = xx;
	z = zz;
}

void VPPoint3D::vpSetYZ(float yy, float zz) {
	y = yy;
	z = zz;
}

void VPPoint3D::vpSetXYZ(float xx, float yy, float zz) {
	x = xx;
	y = yy;
	z = zz;
}

VPPoint3D VPPoint3D::operator- (const VPPoint3D& p) {
 VPPoint3D vv(x-p.x, y-p.y, z-p.z);
 return ( vv ) ;
} 

VPPoint3D VPPoint3D::operator+ (const VPPoint3D& p) {
 VPPoint3D vv(x+p.x, y+p.y, z+p.z);
 return ( vv ) ;
} 

VPPoint3D VPPoint3D::operator= (VPPoint3D p) {
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}

VPPoint3D VPPoint3D::operator-= (int num) 
{
 x -= num;
 y -= num;
 z -= num;
 return *this;
}

VPPoint3D VPPoint3D::operator* (float num) 
{
 x *= num;
 y *= num;
 z *= num;
 return *this;
}

bool VPPoint3D::operator!= (const VPPoint3D& p) 
{
	if ( (x!=p.x) || (y!=p.y) || (z!=p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator== (const VPPoint3D& p) 
{
	if ( (x==p.x) && (y==p.y) && (z==p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator<= (const VPPoint3D& p) 
{
	if ( (x<=p.x) && (y<=p.y) && (z<=p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator< (const VPPoint3D& p) 
{
	if ( (x<p.x) && (y<p.y) && (z<p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator>= (const VPPoint3D& p) 
{
	if ( (x>=p.x) && (y>=p.y) && (z>=p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator> (const VPPoint3D& p) {
	if ( (x>p.x) && (y>p.y) && (z>p.z) )
		return true;
	else
		return false;
}

float VPPoint3D::vpDistance(VPPoint3D p1) {
	float d;
	d = (float)sqrt((x-p1.x)*(x-p1.x) +
			 (y-p1.y)*(y-p1.y) +
			 (z-p1.z)*(z-p1.z));
	return d;
}



