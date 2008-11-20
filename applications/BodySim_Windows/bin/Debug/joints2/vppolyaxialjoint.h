///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppolyaxialjoint.h
//  DESCRIPTION.: Contain the VPPolyaxialJoint class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: January/15/2001
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef	__VPPOLYAXIALJOINT_H
#define	__VPPOLYAXIALJOINT_H


#include	<vpjoint.h>

/////////////////////////////////////////////////////////////////////////
// Class Name: VPPolyaxialJoint
// Superclass: VPJoint
// Subclass: none

class	VPPolyaxialJoint : public VPJoint{

	private:

	public:
		VPPolyaxialJoint( void );
		VPPolyaxialJoint( VPJoint *parent );
		VPPolyaxialJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2, VPDof *dof3 );
		void	vpSetFlexionTo( float value );
		void	vpSetAductionTo( float value );
		void	vpSetTwistTo( float value );
};

#endif