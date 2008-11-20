//deprecated
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
using namespace std;

//-----------------------------------------------------------------------
// V P B O D Y
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPBody
// Superclass: none
// Subclass: none
///\deprecated
class VPBody{

	private:
		string		fileName;
		string		description;
		bool			changed;
		VPJoint*	rootJoint;

	public:

		VPBody();
		VPBody( string desc );
		VPBody( string fname, string desc, VPJoint *jointsTree );
		//~VPBody();

		void		vpSetFileName( char* );
		void		vpSetFileName( string );
		void		vpSetDescription( char* );
		void		vpSetDescription( string );
		void		vpSetChanged( bool yesno );
		bool		vpIsChanged( void );
		void		vpUpdate( void );
		string		vpGetFileName();
		string		vpGetDescription();
		VPJoint*	vpGetRootJoint();
		VPJoint*  vpGetJointByName( string );
		VPJoint*  vpSearchJoint( VPJoint*, string );
		void		vpSetRootJoint( VPJoint* );
		void		vpMakeShapeGims( void );

};

#endif
