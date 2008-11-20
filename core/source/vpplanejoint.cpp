//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpplanejoint.cpp
//  DESCRIPTION.: Contain the VPPlaneJoint class definitions.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: May/16/2001
//  DESCRIPTION.: Methods implementation.
//
///////////////////////////////////////////////////////////////////

#include	<stdio.h>
#include	<vpplanejoint.h>
#include	<vpdof.h>

VPPlaneJoint::VPPlaneJoint( void ):VPJoint(){

	vpSetParent( NULL );
	VPDof *d1 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	VPDof *d2 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d3 = new VPDof( VPVector3D( 0, 1, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d4 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d5 = new VPDof( VPVector3D( 0, 1, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d6 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	d1->vpSetOwnerJoint( this );
	d2->vpSetOwnerJoint( this );
	d3->vpSetOwnerJoint( this );
	d4->vpSetOwnerJoint( this );
	d5->vpSetOwnerJoint( this );
	d6->vpSetOwnerJoint( this );
	vpAddDof( d1 );
	vpAddDof( d2 );
	vpAddDof( d3 );
	vpAddDof( d4 );
	vpAddDof( d5 );
	vpAddDof( d6 );
	vpMakeLim();
}

VPPlaneJoint::VPPlaneJoint( VPJoint *parent ):VPJoint(){

	vpSetParent( parent );
	VPDof *d1 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	VPDof *d2 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d3 = new VPDof( VPVector3D( 0, 1, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d4 = new VPDof( VPVector3D( 1, 0, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d5 = new VPDof( VPVector3D( 0, 1, 0 ), VPPoint3D(), new VPBezier() );
	VPDof *d6 = new VPDof( VPVector3D( 0, 0, 1 ), VPPoint3D(), new VPBezier() );
	d1->vpSetOwnerJoint( this );
	d2->vpSetOwnerJoint( this );
	d3->vpSetOwnerJoint( this );
	d4->vpSetOwnerJoint( this );
	d5->vpSetOwnerJoint( this );
	d6->vpSetOwnerJoint( this );
	vpAddDof( d1 );
	vpAddDof( d2 );
	vpAddDof( d3 );
	vpAddDof( d4 );
	vpAddDof( d5 );
	vpAddDof( d6 );
	vpMakeLim();
}

VPPlaneJoint::VPPlaneJoint( VPJoint *parent, VPDof *dof1, VPDof *dof2, VPDof *dof3,
																						 VPDof *dof4, VPDof *dof5, VPDof *dof6 ):VPJoint(){

	vpSetParent( parent );
	dof1->vpSetOwnerJoint( this );
	dof2->vpSetOwnerJoint( this );
	dof3->vpSetOwnerJoint( this );
	dof4->vpSetOwnerJoint( this );
	dof5->vpSetOwnerJoint( this );
	dof6->vpSetOwnerJoint( this );
	vpAddDof( dof1 );
	vpAddDof( dof2 );
	vpAddDof( dof3 );
	vpAddDof( dof4 );
	vpAddDof( dof5 );
	vpAddDof( dof6 );
	vpMakeLim();
}

void
VPPlaneJoint::vpSetFlexionTo( float value ){

	vpGetDofList()[0]->vpMoveTo( value );
	vpMakeLim();
}

void
VPPlaneJoint::vpSetAdductionTo( float value ){

	vpGetDofList()[1]->vpMoveTo( value );
	vpMakeLim();
}

void
VPPlaneJoint::vpSetTwistTo( float value ){

	vpGetDofList()[2]->vpMoveTo( value );
	vpMakeLim();
}

void
VPPlaneJoint::vpSetXSlideTo( float value ){

	vpGetDofList()[3]->vpMoveTo( value );
	vpMakeLim();
}

void
VPPlaneJoint::vpSetYSlideTo( float value ){

	vpGetDofList()[4]->vpMoveTo( value );
	vpMakeLim();
}

void
VPPlaneJoint::vpSetZSlideTo( float value ){

	vpGetDofList()[5]->vpMoveTo( value );
	vpMakeLim();
}
