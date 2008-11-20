///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpplanejoint.h
//  DESCRIPTION.: Contain the VPPlaneJoint class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: January/15/2001
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef	__VPPLANEJOINT_H
#define	__VPPLANEJOINT_H


#include	<vpjoint.h>

/////////////////////////////////////////////////////////////////////////
// Class Name: VPPlaneJoint
// Superclass: VPJoint
// Subclass: none

class	VPPlaneJoint : public VPJoint{

	private:

	public:
		VPPlaneJoint( void );
		VPPlaneJoint( VPJoint *parent );
		VPPlaneJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2, VPDof *dof3, VPDof *dof4, VPDof *dof5, VPDof *dof6 );
		void	vpSetFlexionTo( float value );
		void	vpSetAdductionTo( float value );
		void	vpSetTwistTo( float value );
		void	vpSetXSlideTo( float value );
		void	vpSetYSlideTo( float value );
		void	vpSetZSlideTo( float value );
};


#endif