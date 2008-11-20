///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpmotion.h
//  DESCRIPTION.: Contain the VPMotion class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: June/06/2001
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef	__VPJOINTMOTION_H
#define	__VPJOINTMOTION_H


#include	<string>
//using namespace std;

#define FLEX    0
#define ADDUCT  1
#define TWIST   2

//-----------------------------------------------------------------------
// V P J O I N T M O T I O N
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPMovement
// Superclass: none
// Subclass: none

class VPJointMotion {

  private:
	std::string  jointName;
    int     motionType;
    float   t0;
    float   tf;
    float   parameter;
    float   deltaS;

  public:
	  VPJointMotion();
	  VPJointMotion( std::string, int, float, float, float );
	  ~VPJointMotion();
	
	  std::string  vpGetJointName();
	  int     vpGetMotionType();
    float   vpGetTimeIni();
    float   vpGetTimeFin();
    float   vpGetParameter();
    float   vpGetDeltaS();

    void    vpSetJointName( std::string );
    void    vpSetMotionType( int );
    void    vpSetTimeIni( float );
    void    vpSetTimeFin( float );
    void    vpSetParameter( float );
    void    vpSetDeltaS( float );
	
};

#endif
