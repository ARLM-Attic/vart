///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpbiaxialjoint.cpp
//  DESCRIPTION.: Contain the VPBiaxialJoint class definitions.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: January/16/2000
//  DESCRIPTION.: Methods implementation.
//
///////////////////////////////////////////////////////////////////

//#include	<stdio.h>
#include	<vpbiaxialjoint.h>
#include	<vpdof.h>

VPBiaxialJoint::VPBiaxialJoint( void ):VPJoint(){

	vpSetParent( NULL );
	VPDof *d1 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	VPDof *d2 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	d1->vpSetOwnerJoint( this );
	d2->vpSetOwnerJoint( this );
	vpAddDof( d1 );
	vpAddDof( d2 );
	vpMakeLim();
}

VPBiaxialJoint::VPBiaxialJoint( VPJoint *parent ):VPJoint(){

	vpSetParent( parent );
	VPDof *d1 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	VPDof *d2 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	d1->vpSetOwnerJoint( this );
	d2->vpSetOwnerJoint( this );
	vpAddDof( d1 );
	vpAddDof( d2 );
	vpMakeLim();
}

VPBiaxialJoint::VPBiaxialJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2 ):VPJoint(){

	vpSetParent( parent );
	dof1->vpSetOwnerJoint( this );
	dof2->vpSetOwnerJoint( this );
	vpAddDof( dof1 );
	vpAddDof( dof2 );
	vpMakeLim();
}

void
VPBiaxialJoint::vpSetFlexionTo( float value ){

	vpGetDofList()[0]->vpMoveTo( value );
	vpMakeLim();
}

void
VPBiaxialJoint::vpSetAductionTo( float value ){

	vpGetDofList()[1]->vpMoveTo( value );
	vpMakeLim();
}