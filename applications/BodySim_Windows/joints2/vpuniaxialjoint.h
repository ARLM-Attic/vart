///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpuniaxialjoint.h
//  DESCRIPTION.: Contain the VPUniaxial Joint class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: August/10/2000
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef	__VPUNIAXIALJOINT_H
#define	__VPUNIAXIALJOINT_H


#include	<vpjoint.h>

/////////////////////////////////////////////////////////////////////////
// Class Name: VPUniaxialJoint
// Superclass: VPJoint
// Subclass: VPPivotJoint, VPHingeJoint

class	VPUniaxialJoint : public VPJoint{

	private:

	public:
		VPUniaxialJoint( void );
		VPUniaxialJoint( VPJoint *parent );
		VPUniaxialJoint( VPJoint *parent, VPDof *dof1 );
		void	vpSetFlexionTo( float value );
};

/////////////////////////////////////////////////////////////////////////
// Class Name: VPPivotJoint
// Superclass: VPUniaxialJoint
// Subclass: none

class	VPPivotJoint : public VPUniaxialJoint{

};

/////////////////////////////////////////////////////////////////////////
// Class Name: VPHingeJoint
// Superclass: VPUniaxialJoint
// Subclass: none

class	VPHingeJoint : public VPUniaxialJoint{

	
};

#endif