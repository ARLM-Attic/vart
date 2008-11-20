//#include	<stdio.h>
#include	<math.h>

#include	<vpdof.h>
#include	<vpjoint.h>
#include	<vpmodifier.h>

VPDof::VPDof( void ){

	axis = VPVector3D( 0, 0, 1 );
	evoluta = new VPBezier();
	position = VPPoint3D( 0, 0, 0 );
	lim = VPMatrix();
	minAngle = 0;
  maxAngle = 0;
  currentPosition = 0;
  restPosition = 0;
  rangeModifier = NULL;
}

VPDof::VPDof( VPVector3D za, VPPoint3D pos, VPBezier *evol ){

	axis = za;
	evoluta = evol;
	position = pos;
	lim = VPMatrix();
	minAngle = 0;
  maxAngle = 0;
  currentPosition = 0;
  restPosition = 0;
  rangeModifier = NULL;
}

VPDof::VPDof( VPVector3D za, VPPoint3D pos, VPCurve *evol, float min, float max, float cur, float rest ){
	
	axis = za;
	evoluta = (VPBezier *)evol;
	position = pos;
	lim = VPMatrix();
	minAngle = min;
  maxAngle = max;
  currentPosition = cur;
  restPosition = rest;
  rangeModifier = NULL;
}

VPDof::VPDof( VPVector3D za, VPPoint3D pos, VPBezier *evol, float min, float max, float cur, float rest, VPModifier *rm ){
	
	axis = za;
	evoluta = evol;
	position = pos;
	lim = VPMatrix();
	minAngle = min;
  maxAngle = max;
  currentPosition = cur;
  restPosition = rest;
  rangeModifier = rm;
}

void
VPDof::vpSetDescription( char* szDesc ){
	description = szDesc;
}

void
VPDof::vpSetDescription( std::string sDesc ){
	description = sDesc;
}

std::string
VPDof::vpGetDescription(){
	return description;
}

VPVector3D
VPDof::vpGetAxis( void ){
	
	return axis;
}

VPPoint3D
VPDof::vpGetOrigin( void ){
	
	VPPoint3D *p = new VPPoint3D();
	p = lim.vpMultiply(*p);
	return *p;
}

VPCurve
VPDof::vpGetEvoluta( void ){
	
	return *evoluta;
}

VPMatrix
VPDof::vpGetLim( void ){
	
	return lim;
}

VPMatrix
VPDof::vpGetBim( void ){
	
	
	/// Build BIM.
	axis.vpNormalize();
	VPVector3D vx = VPVector3D( 1, 0, 0 );
	VPVector3D vy = axis.vpCrossProduct(vx);
	// Verify linearity
	if( vy == VPVector3D( 0, 0, 0 ) ){
		vx = VPVector3D( 0, 0, 1 );
		vy = axis.vpCrossProduct(vx);
	}
	vy.vpNormalize();
	vx = vy.vpCrossProduct(axis);
	vx.vpNormalize();
	float			fm[4][4];

 	fm[0][0] = vx.vpGetX();
	fm[1][0] = vx.vpGetY();
	fm[2][0] = vx.vpGetZ();
	fm[3][0] = 0;
	
	fm[0][1] = vy.vpGetX();
	fm[1][1] = vy.vpGetY();
	fm[2][1] = vy.vpGetZ();
	fm[3][1] = 0;
	
	fm[0][2] = axis.vpGetX();
	fm[1][2] = axis.vpGetY();
	fm[2][2] = axis.vpGetZ();
	fm[3][2] = 0;
	
	fm[0][3] = 0;
 	fm[1][3] = 0;
 	fm[2][3] = 0;
 	fm[3][3] = 1;
 	
	VPMatrix lBim = VPMatrix( fm );
	return lBim;
}

float
VPDof::vpGetMin( void ){
	
	return minAngle;
}

float
VPDof::vpGetMax( void ){
	
	return maxAngle;
}

float
VPDof::vpGetCurrentMin( void ){
	
	float minModif = minAngle;
	if( rangeModifier ){
		minModif = rangeModifier->vpGetMin();
	}
	if( minAngle < minModif ){
		return minModif;
	}
	return minAngle;
}

float
VPDof::vpGetCurrentMax( void ){
	
	float maxModif = maxAngle;
	if( rangeModifier ){
		maxModif = rangeModifier->vpGetMax();
	}
	if( maxAngle > maxModif ){
		return maxModif;
	}
	return maxAngle;
}

float
VPDof::vpGetCurrent( void ){
	
	return currentPosition;
}

float
VPDof::vpGetRest( void ){
	
	return restPosition;
}

VPJoint*
VPDof::vpGetOwnerJoint( void ){
	
	return ownerJoint;
}

void
VPDof::vpSetAxis( VPVector3D za ){
	
	axis = za;
}

void
VPDof::vpSetEvoluta( VPBezier *evol ){
	
	evoluta = evol;
}

void
VPDof::vpSetLim( VPMatrix m ){
	
	lim = m;
}

void
VPDof::vpSetMin( float min ){
	
	minAngle = min;
}

void
VPDof::vpSetMax( float max ){
	
	maxAngle = max;
}

void
VPDof::vpMoveTo( float pos ){
	
	// Avoid values out of range
	if( pos > 1.0 ) pos = 1.0;
	if( pos < 0.0 ) pos = 0.0;
	
	// Get resources (obtain angles and points by interpolation)
	VPPoint3D tgtpos = evoluta->vpGetPointAsPoint( pos ); //target position
	float tgtang = vpGetCurrentMin() + ( ( vpGetCurrentMax() - vpGetCurrentMin() ) * pos ); //target angle
	
	VPPoint3D p = position + VPPoint3D( tgtpos );
		
	// Build transformation matrix
	float r[4][4];
	r[0][0] = cos( tgtang );
	r[0][1] = sin( tgtang );
	r[1][0] = -sin( tgtang );
	r[1][1] = cos( tgtang );
	r[3][0] = p.vpGetX();
	r[3][1] = p.vpGetY();
	r[3][2] = p.vpGetZ();
	r[2][2] = r[3][3] = 1;
	r[0][2] = r[1][2] = r[2][0] = r[2][1] = r[0][3] = r[1][3] = r[2][3] = 0;
	VPMatrix mt = VPMatrix( r );
	
	// update to new position
	// lim = bim * T * inv(bim)
	vpSetLim(*( (vpGetBim().vpMultiply( mt ))->vpMultiply( *(vpGetBim().vpGetTransposed()) ) ) );
	currentPosition = pos;
}


void
VPDof::vpSetOwnerJoint( VPJoint *ow ){
	ownerJoint = ow;
}

void
VPDof::vpSetRest( float rest ){
	
	restPosition = rest;
}

void
VPDof::vpRest( void ){
	
	vpMoveTo( restPosition );
}

void
VPDof::vpSetRangeModifier( VPModifier *m ){
	rangeModifier = m;
}

VPModifier*
VPDof::vpGetRangeModifier( void ){
	return rangeModifier;
}


void
VPDof::vpPrintLim( void ){
	printf( "%f %f %f %f \n",vpGetLim().vpGetValueAt(0,0), vpGetLim().vpGetValueAt(0,1),
  												vpGetLim().vpGetValueAt(0,2), vpGetLim().vpGetValueAt(0,3) );
  printf( "%f %f %f %f \n",vpGetLim().vpGetValueAt(1,0), vpGetLim().vpGetValueAt(1,1),
  												vpGetLim().vpGetValueAt(1,2), vpGetLim().vpGetValueAt(1,3) );
	printf( "%f %f %f %f \n",vpGetLim().vpGetValueAt(2,0), vpGetLim().vpGetValueAt(2,1),
  												vpGetLim().vpGetValueAt(2,2), vpGetLim().vpGetValueAt(2,3) );
	printf( "%f %f %f %f \n\n",vpGetLim().vpGetValueAt(3,0), vpGetLim().vpGetValueAt(3,1),
  												vpGetLim().vpGetValueAt(3,2), vpGetLim().vpGetValueAt(3,3) );
}