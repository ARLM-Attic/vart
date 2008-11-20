
#include	<vpcurve.h>
#include	<vppoint.h>
#include 	<stdio.h>


VPCurve::~VPCurve(){
	
	if( controlPoints ){
		delete controlPoints;
		controlPoints = NULL;
	}
}

VPCurve::VPCurve(){
	
	controlPoints = NULL;
}

VPCurve::VPCurve( VPVertex3D	*pts){

	controlPoints = pts;
}
		
void
VPCurve::vpSetControlPoints( VPVertex3D	*p ){
	controlPoints = p;
}

VPVertex3D*
VPCurve::vpGetControlPoints( void ){
	return controlPoints;
}

VPPoint3D
VPCurve::vpGetPointAsPoint( float t ){

	VPPoint3D	*p = new VPPoint3D();
/*	VPVertex3D p0 = vpGetControlPoint(0);
	VPVertex3D p1 = vpGetControlPoint(1);
	VPVertex3D p2 = vpGetControlPoint(2);
	VPVertex3D p3 = vpGetControlPoint(3);
	
	p->vpSetX( p0.vpGetX()*(1-t)*(1-t)*(1-t) + p1.vpGetX()*3*t*(1-t)*(1-t) +
						 p2.vpGetX()*3*t*t*(1-t) + p3.vpGetX()*t*t*t );
	p->vpSetY( p0.vpGetY()*(1-t)*(1-t)*(1-t) + p1.vpGetY()*3*t*(1-t)*(1-t) +
						 p2.vpGetY()*3*t*t*(1-t) + p3.vpGetY()*t*t*t );
	p->vpSetZ( p0.vpGetZ()*(1-t)*(1-t)*(1-t) + p1.vpGetZ()*3*t*(1-t)*(1-t) +
						 p2.vpGetZ()*3*t*t*(1-t) + p3.vpGetZ()*t*t*t );
*/	
	return *p;
}

void
VPCurve::vpRender( void ){
	// meanwhile do nothing
}