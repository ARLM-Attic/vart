///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpbiaxialjoint.h
//  DESCRIPTION.: Contain the VPBiaxialJoint class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: January/15/2001
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef	__VPBIAXIALJOINT_H
#define	__VPBIAXIALJOINT_H


#include	<vpjoint.h>

/////////////////////////////////////////////////////////////////////////
// Class Name: VPBiaxialJoint
// Superclass: VPJoint
// Subclass: VPCondylarJoint, VPSaddleJoint, VPEllipsoidJoint

class	VPBiaxialJoint : public VPJoint{

	private:

	public:
		VPBiaxialJoint( void );
		VPBiaxialJoint( VPJoint *parent );
		VPBiaxialJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2 );
		void	vpSetFlexionTo( float value );
		void	vpSetAductionTo( float value );

};

/////////////////////////////////////////////////////////////////////////
// Class Name: VPCondylarJoint
// Superclass: VPBiaxialJoint
// Subclass: none

class	VPCondylarJoint : public VPBiaxialJoint{

};

/////////////////////////////////////////////////////////////////////////
// Class Name: VPSaddleJoint
// Superclass: VPBiaxialJoint
// Subclass: none

class	VPSaddlelJoint : public VPBiaxialJoint{

};

/////////////////////////////////////////////////////////////////////////
// Class Name: VPEllipsoidJoint
// Superclass: VPBiaxialJoint
// Subclass: none

class	VPEllipsoidJoint : public VPBiaxialJoint{

};

#endif