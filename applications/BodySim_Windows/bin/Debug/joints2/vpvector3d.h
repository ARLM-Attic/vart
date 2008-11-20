
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpvector3d.h
//  DESCRIPTION.: Contain the VPVector3D class declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: August/10/2000
//  DESCRIPTION.: Classes and new methods declaration.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: January/16/2001
//  DESCRIPTION.: New methods declaration.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: April/18/2001
//  DESCRIPTION.: Class update in accordance with the changes in the 
//				  VPPoint3D attributes.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPVECTOR3D_H	
#define __VPVECTOR3D_H	

#include	<vppoint3d.h>


///////////////////////////////////////////////////////////////////
// Class Name: VPVector3D (storage and manipulation of 3D vector)
// Superclass: VPPoint3D
// Subclasses: -

class VPVector3D:public VPPoint3D {
  public:
	  VPVector3D();
	  VPVector3D(float xi, float yi, float zi);
	  VPVector3D(const VPVector3D&);
 	  VPVector3D& operator=(const VPVector3D&);
	  //  Comparison
      int operator==(const VPVector3D&);
      int operator!=(const VPVector3D&);
	  //  Sum, Difference, Scalar Product
      VPPoint3D operator+(VPPoint3D);
      VPPoint3D operator-(VPPoint3D);
      VPVector3D operator+(const VPVector3D&);
      VPVector3D operator-(const VPVector3D&);
      VPVector3D operator-();
      VPVector3D operator*(const float&);
      VPVector3D operator/(const float&);
      VPVector3D operator*(const VPVector3D&);
      //  Immediate Sum, Difference, Scalar Product
      VPVector3D& operator+=(const VPVector3D&);
      VPVector3D& operator-=(const VPVector3D&);
      VPVector3D& operator*=(const float&);
      VPVector3D& operator/=(const float&);
      //  Dot Product
      float vpDotProduct(const VPVector3D&);
      //  Cross Product
      VPVector3D vpCrossProduct(const VPVector3D&);
	  //  Vector module
	  float vpModule();
      //  Vector normalization
      void vpNormalize();
	  //  Set, Get
	  void vpSetVector3D(VPPoint3D p);
	  void vpSetVector3D(float xx,float yy,float zz);
	  void vpGetVector3D(float &xx,float &yy,float &zz);
	  VPVector3D vpGetVector3D(void);
	  float vpGetVector3DX(void);
	  float vpGetVector3DY(void);
	  float vpGetVector3DZ(void);
};



//  The global constant VPVector3Ds
const VPVector3D VZERO ( 0.0, 0.0, 0.0 );
const VPVector3D VX    ( 1.0, 0.0, 0.0 );
const VPVector3D VY    ( 0.0, 1.0, 0.0 );
const VPVector3D VZ    ( 0.0, 0.0, 1.0 );

#endif