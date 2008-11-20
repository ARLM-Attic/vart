//deprecated
///  \file               vppoint3d.cpp
///  \brief              Contain the VPPoint3D class declarations.
///  \version            1.1.1.3

// Change log:
// Sep 30/2004 - Bruno de Oliveira Schneider
// - Added "const" to identify constant methods.
// - Minor changes to DoxyGen documentation.
// - Methods vp(Get|Set)(X|Y|Z) are no longer inline, because this would prevent
//   step-by-step compilation. Implementation could be moved to header file, though.
// Sep 15/2004 - Bruno de Oliveira Schneider
// - Files have been replaced with the docs files (including DoxyGen comments)
// - Code typing has been cleaned (tabs replaced by spaces, excessive spaces
// - removed, etc.) in the headers file because tabs and spaces were mixed together.
// April 18/2001 Isabel Harb Manssour
// - Change of attributes.
// August 10/2000 Isabel Harb Manssour
// - Classes and new methods declaration.
// January 20/2000 Isabel Harb Manssour
// - New overload methods declaration.


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

float VPPoint3D::vpGetX() const {
	return x;
}

float VPPoint3D::vpGetY() const {
	return y;
}

float VPPoint3D::vpGetZ() const {
	return z;
}

void VPPoint3D::vpGetXY(float &xx, float &yy) const {
	xx = x;
	yy = y;
}

void VPPoint3D::vpGetXZ(float &xx, float &zz) const {
	xx = x;
	zz = z;
}

void VPPoint3D::vpGetYZ(float &yy, float &zz) const {
	yy = y;
	zz = z;
}

void VPPoint3D::vpGetXYZ(float &xx, float &yy, float &zz) const {
	xx = x;	
	yy = y;
	zz = z;
}

VPPoint3D VPPoint3D::vpGetPoint3D(void) const {
	return *this;
}

float* VPPoint3D::vpGetPointAsArray( void ) const
{
	float* array = new float[3]; 
	array[0] = x; 
	array[1] = y; 
	array[2] = z; 
	return array;
}

void VPPoint3D::vpGetPoint(float v[]) const
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void VPPoint3D::vpSetX(float xx) {
	x = xx;
}
	
void VPPoint3D::vpSetY(float yy) {
	y = yy;
}

void VPPoint3D::vpSetZ(float zz) {
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

VPPoint3D VPPoint3D::operator- (const VPPoint3D& p) const {
 VPPoint3D vv(x-p.x, y-p.y, z-p.z);
 return ( vv ) ;
} 

VPPoint3D VPPoint3D::operator+ (const VPPoint3D& p) const {
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

bool VPPoint3D::operator!= (const VPPoint3D& p) const
{
	if ( (x!=p.x) || (y!=p.y) || (z!=p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator== (const VPPoint3D& p) const
{
	if ( (x==p.x) && (y==p.y) && (z==p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator<= (const VPPoint3D& p) const
{
	if ( (x<=p.x) && (y<=p.y) && (z<=p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator< (const VPPoint3D& p) const
{
	if ( (x<p.x) && (y<p.y) && (z<p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator>= (const VPPoint3D& p) const
{
	if ( (x>=p.x) && (y>=p.y) && (z>=p.z) )
		return true;
	else
		return false;
}

bool VPPoint3D::operator> (const VPPoint3D& p) const {
	if ( (x>p.x) && (y>p.y) && (z>p.z) )
		return true;
	else
		return false;
}

float VPPoint3D::vpDistance(VPPoint3D p1) const {
	float d;
	d = (float)sqrt((x-p1.x)*(x-p1.x) +
			 (y-p1.y)*(y-p1.y) +
			 (z-p1.z)*(z-p1.z));
	return d;
}
