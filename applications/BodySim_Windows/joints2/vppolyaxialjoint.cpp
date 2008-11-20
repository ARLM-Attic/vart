///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppolyaxialjoint.cpp
//  DESCRIPTION.: Contain the VPPolyaxialJoint class definitions.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: May/16/2001
//  DESCRIPTION.: Methods implementation.
//
///////////////////////////////////////////////////////////////////

//#include	<stdio.h>
#include	<vppolyaxialjoint.h>
#include	<vpdof.h>

VPPolyaxialJoint::VPPolyaxialJoint( void ):VPJoint(){

	vpSetParent( NULL );
	VPDof *d1 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	VPDof *d2 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d3 = new VPDof( VPVector3D( 0, 1, 0 ), VPPoint3D(), new VPBezier() );
	d1->vpSetOwnerJoint( this );
	d2->vpSetOwnerJoint( this );
	d3->vpSetOwnerJoint( this );
	vpAddDof( d1 );
	vpAddDof( d2 );
	vpAddDof( d3 );
	vpMakeLim();
}

VPPolyaxialJoint::VPPolyaxialJoint( VPJoint *parent ):VPJoint(){

	vpSetParent( parent );
	VPDof *d1 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	VPDof *d2 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d3 = new VPDof( VPVector3D( 0, 1, 0 ), VPPoint3D(), new VPBezier() );
	d1->vpSetOwnerJoint( this );
	d2->vpSetOwnerJoint( this );
	d3->vpSetOwnerJoint( this );
	vpAddDof( d1 );
	vpAddDof( d2 );
	vpAddDof( d3 );
	vpMakeLim();
}

VPPolyaxialJoint::VPPolyaxialJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2, VPDof *dof3 ):VPJoint(){

	vpSetParent( parent );
	dof1->vpSetOwnerJoint( this );
	dof2->vpSetOwnerJoint( this );
	dof3->vpSetOwnerJoint( this );
	vpAddDof( dof1 );
	vpAddDof( dof2 );
	vpAddDof( dof3 );
	vpMakeLim();
}

void
VPPolyaxialJoint::vpSetFlexionTo( float value ){

	vpGetDofList()[0]->vpMoveTo( value );
	vpMakeLim();
}

void
VPPolyaxialJoint::vpSetAductionTo( float value ){

	vpGetDofList()[1]->vpMoveTo( value );
	vpMakeLim();
}

void
VPPolyaxialJoint::vpSetTwistTo( float value ){

	vpGetDofList()[2]->vpMoveTo( value );
	vpMakeLim();
}
