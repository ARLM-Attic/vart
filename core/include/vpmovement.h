//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpmovment.h
//  DESCRIPTION.: Contain the VPMovement class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: June/06/2001
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////
// Registro de Alteracoes
// 04/MAI/04 - Bruno Schneider
// - alterei os includes


#ifndef	__VPMOVEMENT_H
#define	__VPMOVEMENT_H

#include <vpxml.h>
#include <vpjoint.h>
#include <vpdof.h>
#include <vpjointmotion.h>

#include <dom/deprecated/DOM_DOMException.hpp>
#include <dom/deprecated/DOMParser.hpp>
#include <dom/deprecated/DOM_NamedNodeMap.hpp>

#include <string>
using namespace std;

#define INIT_TIME -0.1


//-----------------------------------------------------------------------
// V P M O V E M E N T
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPMovement
// Superclass: none
// Subclass: none
///\deprecated

class VPMovement {

  private:
    string  bodyName;
    bool    cycle;
    float   deltaT;
    float   time;
    float   tFinal;
    float   time_err;
    int     qtdMotion;

    VPJointMotion *motionTimeline;

  public:
    VPMovement();
	  VPMovement( string, bool, float, float );
	  ~VPMovement();
	
	  float         vpGetDeltaT();
	  float         vpGetTime();
	  float         vpGetTimeFinal();
	  float         vpGetTime_Err();
	  bool          vpGetCycle();
	  int           vpGetQtdMotion();
	  VPJointMotion *vpGetTimeline();
	
	  void  vpSetDeltaT( float );
	  void  vpSetTime( float );
	  void  vpSetTimeFinal( float );
	  void  vpSetTime_Err( float );
	  void  vpSetCycle( bool );
	  void  vpSetQtdMotion( int );
	
	  void  vpLoadMotionFile( char *fileName );
};

#endif
