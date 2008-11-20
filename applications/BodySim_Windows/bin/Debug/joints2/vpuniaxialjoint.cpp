///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpuniaxialjoint.cpp
//  DESCRIPTION.: Contain the VPUniaxialJoint class definitions.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: August/10/2000
//  DESCRIPTION.: Methods implementation.
//
///////////////////////////////////////////////////////////////////

//#include	<stdio.h>
#include	<vpuniaxialjoint.h>
#include	<vpdof.h>

VPUniaxialJoint::VPUniaxialJoint( void ):VPJoint(){

	vpSetParent( NULL );
	VPDof *d = new VPDof();
	d->vpSetOwnerJoint( this );
	vpAddDof( d );
	vpMakeLim();
}

VPUniaxialJoint::VPUniaxialJoint( VPJoint *parent ):VPJoint(){

	vpSetParent( parent );
	VPDof *d = new VPDof();
	d->vpSetOwnerJoint( this );
	vpAddDof( d );
	vpMakeLim();
}

VPUniaxialJoint::VPUniaxialJoint( VPJoint *parent, VPDof *dof1 ):VPJoint(){

	vpSetParent( parent );
	dof1->vpSetOwnerJoint( this );
	vpAddDof( dof1 );
	vpMakeLim();
}

void
VPUniaxialJoint::vpSetFlexionTo( float value ){

	vpGetDofList()[0]->vpMoveTo( value );
	vpMakeLim();
}
