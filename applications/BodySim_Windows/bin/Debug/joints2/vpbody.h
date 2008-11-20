///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpbody.h
//  DESCRIPTION.: Contain the VPBody class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 09/June/2000
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPBODY_H
#define __VPBODY_H


#include	<vpjoint.h>
#include	<string>	//STL include
//using namespace std;

//-----------------------------------------------------------------------
// V P B O D Y
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPBody
// Superclass: none
// Subclass: none

class VPBody{

	private:
		std::string		fileName;
		std::string		description;
		bool			changed;
		VPJoint*	rootJoint;

	public:

		VPBody();
		VPBody( std::string desc );
		VPBody( std::string fname, std::string desc, VPJoint *jointsTree );
		//~VPBody();

		void		vpSetFileName( char* );
		void		vpSetFileName( std::string );
		void		vpSetDescription( char* );
		void		vpSetDescription( std::string );
		void		vpSetChanged( bool yesno );
		bool		vpIsChanged( void );
		void		vpUpdate( void );
		std::string		vpGetFileName();
		std::string		vpGetDescription();
		VPJoint*	vpGetRootJoint();
		VPJoint*  vpGetJointByName( std::string );
		VPJoint*  vpSearchJoint( VPJoint*, std::string );
		void		vpSetRootJoint( VPJoint* );
		void		vpMakeShapeGims( void );

};

#endif
