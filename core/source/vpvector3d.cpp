//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpvector3D.cpp
//  DESCRIPTION.: Contain the VPVector3D class implementation.
//
//  AUTHOR......: Maurício Drehmer
//	DATE........: December/10/1999
//  DESCRIPTION.: Class declaration.
//
//  AUTHOR......: Fernando Sola Pereira
//  DATE........: May/21/2000
//  DESCRIPTION.: Change the overloading operators methods.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/10/2000
//  DESCRIPTION.: Change the classes names and implements new 
//				  methods.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: April/18/2001
//  DESCRIPTION.: Class update in accordance with the changes in the 
//				  VPPoint3D attributes.
//
///////////////////////////////////////////////////////////////////

#include <cmath>

#include <vpvector3d.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPVector3D" constructor without parameter.
// Parameters.: -
// Return.....: -

VPVector3D::VPVector3D(void) {
	x = 0;
    y = 0;
    z = 0;
}





///////////////////////////////////////////////////////////////////
// Description: Class "VPVector3D" constructor with parameters.
// Parameters.: float xi (initial x value), 
//				float yi (initial y value),
//				float zi (initial z value).
// Return.....: -

VPVector3D::VPVector3D(const float xi, const float yi, const float zi) {
	x = xi;
    y = yi;
    z = zi;
}



///////////////////////////////////////////////////////////////////
// Description: Class "VPVector3D" constructor with parameters.
// Parameters.: VPVector3D (object).
// Return.....: -

VPVector3D::VPVector3D(const VPVector3D &v) {
	x = v.x ;
    y = v.y ;
    z = v.z ;
}


///////////////////////////////////////////////////////////////////
// Description: Operator=
// Parameters.: VPVector3D (object).
// Return.....: VPVector3D (object).

VPVector3D& VPVector3D :: operator= ( const VPVector3D& v ) {
      if ( this == &v ) return ( *this ) ;
      x = v.x ;
      y = v.y ;
      z = v.z ;
      return ( *this ) ;
}




///////////////////////////////////////////////////////////////////
// Description: Operator==
// Parameters.: VPVector3D (object).
// Return.....: TRUE if the vectors are same,
//              FALSE if the vectors are not same.

int VPVector3D :: operator== ( const VPVector3D& v1 ) {
	if ( ( x == v1.x ) &&
         ( y == v1.y ) &&
         ( z == v1.z ) )
	    return true;
	 else
	    return false;
}




///////////////////////////////////////////////////////////////////
// Description: Operator!=
// Parameters.: VPVector3D (object).
// Return.....: TRUE if the vectors are not same,
//              FALSE if the vectors are same.

int VPVector3D :: operator!= ( const VPVector3D& v ) {
	if ( ( x != v.x ) ||
         ( y != v.y ) ||
         ( z != v.z ) )
	    return true;
	 else
	    return false;
}


///////////////////////////////////////////////////////////////////
// Description: Operator+
// Parameters.: VPPoint3D (object).
// Return.....: VPPoint3D (object).

VPPoint3D VPVector3D :: operator+ (VPPoint3D v) {
      VPPoint3D vv(x + v.vpGetX(), y + v.vpGetY(), z + v.vpGetZ());
      return ( vv );
}


///////////////////////////////////////////////////////////////////
// Description: Operator-
// Parameters.: VPPoint3D (object).
// Return.....: VPPoint3D (object).

VPPoint3D VPVector3D :: operator- (VPPoint3D v) {
      VPPoint3D vv(x - v.vpGetX(), y - v.vpGetY(), z - v.vpGetZ());
      return ( vv );
}


///////////////////////////////////////////////////////////////////
// Description: Operator+
// Parameters.: VPVector3D (object).
// Return.....: VPVector3D (object).

VPVector3D VPVector3D :: operator+ ( const VPVector3D& v ) {
      VPVector3D vv(x + v.x, y + v.y, z + v.z);
      return ( vv ) ;
}


///////////////////////////////////////////////////////////////////
// Description: Operator-
// Parameters.: VPVector3D (object).
// Return.....: VPVector3D (object).

VPVector3D VPVector3D :: operator- ( const VPVector3D& v ) {
      VPVector3D vv(x - v.x, y - v.y, z - v.z);
      return ( vv ) ;
}




///////////////////////////////////////////////////////////////////
// Description: Operator-
// Parameters.: -
// Return.....: VPVector3D (object).

VPVector3D VPVector3D :: operator- () {
      VPVector3D vv(- x, - y, - z);
      return ( vv ) ;
}


///////////////////////////////////////////////////////////////////
// Description: Operator*
// Parameters.: float c (multiplies the vector).
// Return.....: VPVector3D (object).

VPVector3D VPVector3D :: operator* ( const float& c ) {
      VPVector3D vv(c * x, c * y, c * z);
      return ( vv ) ;
}


///////////////////////////////////////////////////////////////////
// Description: Operator*
// Parameters.: VPVector3D (multiplies the vector).
// Return.....: VPVector3D (object).

VPVector3D VPVector3D :: operator* ( const VPVector3D& v ) {
  VPVector3D vv(y * v.z - (z * v.y),
 	z * v.x - (x * v.z),
	x * v.y - (y * v.x));
  return ( vv ) ;

}


///////////////////////////////////////////////////////////////////
// Description: Operator/
// Parameters.: float c (divides the vector).
// Return.....: VPVector3D (object).

VPVector3D VPVector3D :: operator/ ( const float& c ) {
      VPVector3D vv(x / c, y / c, z / c);
      return ( vv ) ;
}



///////////////////////////////////////////////////////////////////
// Description: Operator+=
// Parameters.: VPVector3D (object).
// Return.....: VPVector3D (object).

VPVector3D& VPVector3D :: operator+= ( const VPVector3D& v ) {
      x += v.x ;
      y += v.y ;
      z += v.z ;
      return *this ;
}



///////////////////////////////////////////////////////////////////
// Description: Operator-=
// Parameters.: VPVector3D (object).
// Return.....: VPVector3D (object).

VPVector3D& VPVector3D :: operator-= ( const VPVector3D& v ) {
      x -= v.x ;
      y -= v.y ;
      z -= v.z ;
      return *this ;
}


///////////////////////////////////////////////////////////////////
// Description: Operator*=
// Parameters.: float c (multiplies the vector).
// Return.....: VPVector3D (object).

VPVector3D& VPVector3D :: operator*= ( const float& c ) {
      x *= c ;
      y *= c ;
      z *= c ;
      return *this ;
}


///////////////////////////////////////////////////////////////////
// Description: Operator/=
// Parameters.: float c (divides the vector).
// Return.....: VPVector3D (object).

VPVector3D& VPVector3D :: operator/= ( const float& c ) {
      x /= c ;
      y /= c ;
      z /= c ;
      return *this ;
}


///////////////////////////////////////////////////////////////////
// Description: Vector module.
// Parameters.: -
// Return.....: size (module)

float VPVector3D :: vpModule () {
	float size = (float) sqrt( (double) (x*x + y*y + z*z) );
	return size;
}


///////////////////////////////////////////////////////////////////
// Description: Vector normalization
// Parameters.: -
// Return.....: -

void VPVector3D :: vpNormalize () {
      float l =  vpModule();
      if (l==0) return;
      x = x / l ;
      y = y / l ;
      z = z / l ;
}


///////////////////////////////////////////////////////////////////
// Description: Dot Product.
// Parameters.: VPVector3D& v
// Return.....: float d (dot product - angle cos * the two vectors module)

float VPVector3D :: vpDotProduct ( const VPVector3D& v ) {
      float	d ;
      d =  x * v.x + y * v.y + z * v.z;
	
      return ( d ) ;
}


///////////////////////////////////////////////////////////////////
// Description: Cross Product
// Parameters.: 
// Return.....: 

VPVector3D VPVector3D :: vpCrossProduct ( const VPVector3D& v ) {
  VPVector3D vv(y * v.z - (z * v.y), 
 			    z * v.x - (x * v.z), 
			    x * v.y - (y * v.x));
  return ( vv ) ;
}


///////////////////////////////////////////////////////////////////
// Description: Set the vector value.
// Parameters.: VPPoint3D p
// Return.....: -

void VPVector3D :: vpSetVector3D(VPPoint3D p) {
	x = p.vpGetX();
	y = p.vpGetY();
	z = p.vpGetZ();
}


///////////////////////////////////////////////////////////////////
// Description: Set the vector value.
// Parameters.: float x,
//              float y,
//		        float z.
// Return.....: -

void VPVector3D :: vpSetVector3D(float xx,float yy,float zz) {
	x = xx;
	y = yy;
	z = zz;
}



///////////////////////////////////////////////////////////////////
// Description: Get the vector value.
// Parameters.: float x,
//              float y,
//		        float z.
// Return.....: -

void VPVector3D :: vpGetVector3D(float &xx,float &yy,float &zz) {
	xx = x;
	yy = y;
	zz = z;
}


///////////////////////////////////////////////////////////////////
// Description: Get the vector value.
// Parameters.: -
// Return.....: VPVector3D (object)

VPVector3D VPVector3D :: vpGetVector3D(void) {

	return *this;

}


///////////////////////////////////////////////////////////////////
// Description: Get the X vector value. 
// Parameters.: -
// Return.....: float (x)

float VPVector3D :: vpGetVector3DX(void) {

	return x;

}


///////////////////////////////////////////////////////////////////
// Description: Get the Y vector value.
// Parameters.: -
// Return.....: float (y)

float VPVector3D :: vpGetVector3DY(void) {

	return y;

}



///////////////////////////////////////////////////////////////////
// Description: Get the Z vector value.
// Parameters.: -
// Return.....: float (z)

float VPVector3D :: vpGetVector3DZ(void) {

	return z;

}



