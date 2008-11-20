
#include	<vpbezier.h>
#include	<vppoint3d.h>
#include	<vpvertex3d.h>

#include	<stdio.h>


//VPBezier::~VPBezier(){
	// Default destructor
	//vpSetControlPoints( NULL );
//}

VPBezier::VPBezier(){
	
	VPVertex3D *pts = new VPVertex3D[4];
	pts[0].vpSetXYZ( 0, 0, 0 );
	pts[1].vpSetXYZ( 0, 0, 0 );
	pts[2].vpSetXYZ( 0, 0, 0 );
	pts[3].vpSetXYZ( 0, 0, 0 );
	vpSetControlPoints( pts );
}

VPBezier::VPBezier( VPVertex3D	*pts){

	vpSetControlPoints( pts );
}
		
VPVertex3D
VPBezier::vpGetControlPoint( int i ){

	if( i >3 ){
		i = 3;
	}
	VPVertex3D *v = vpGetControlPoints();
	return v[i];
}

void
VPBezier::vpSetControlPoint( int i, VPVertex3D *n ){

	if( i >3 ){
		i = 3;
	}
	VPVertex3D *v = vpGetControlPoints();
	v[i] = *n;
}

VPPoint3D
VPBezier::vpGetPointAsPoint( float t ){

	VPPoint3D	*p = new VPPoint3D();
	VPVertex3D p0 = vpGetControlPoint(0);
	VPVertex3D p1 = vpGetControlPoint(1);
	VPVertex3D p2 = vpGetControlPoint(2);
	VPVertex3D p3 = vpGetControlPoint(3);
	
	p->vpSetX( p0.vpGetX()*(1-t)*(1-t)*(1-t) + p1.vpGetX()*3*t*(1-t)*(1-t) +
						 p2.vpGetX()*3*t*t*(1-t) + p3.vpGetX()*t*t*t );
	p->vpSetY( p0.vpGetY()*(1-t)*(1-t)*(1-t) + p1.vpGetY()*3*t*(1-t)*(1-t) +
						 p2.vpGetY()*3*t*t*(1-t) + p3.vpGetY()*t*t*t );
	p->vpSetZ( p0.vpGetZ()*(1-t)*(1-t)*(1-t) + p1.vpGetZ()*3*t*(1-t)*(1-t) +
						 p2.vpGetZ()*3*t*t*(1-t) + p3.vpGetZ()*t*t*t );
	
	return *p;
}
		
VPVertex3D
VPBezier::vpGetPointAsVertex( float t ){
	
	VPPoint3D	p = vpGetPointAsPoint(t);
	VPVertex3D	*v = new VPVertex3D();
	v->vpSetX( p.vpGetX() );
	v->vpSetY( p.vpGetY() );
	v->vpSetZ( p.vpGetZ() );
	return *v;
}