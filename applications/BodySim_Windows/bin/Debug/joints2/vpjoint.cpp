//#include        <stdio.h>

#include        <vpmatrix.h>
#include        <vpjoint.h>
#include        <vpdof.h>

VPJoint::VPJoint(){

        //position = VPPoint3D();
        vpRefreshInvLim();
        //shapeList = NULL;
        parentJoint = NULL;
        //childList = NULL;
        numDofs = 0;
        dofList = new VPDof*[MAXDOF];
				for( int i = 0; i < MAXDOF; dofList[i++] = NULL );
}

//VPJoint::~VPJoint(){

//}

void
VPJoint::vpSetDescription( char* szDesc ){
	description = szDesc;
}

void
VPJoint::vpSetDescription( std::string sDesc ){
	description = sDesc;
}

std::string
VPJoint::vpGetDescription(){
	return description;
}

unsigned short int
VPJoint::vpGetNumDofs(){
        return numDofs;
}

void
VPJoint::vpSetChanged( bool yesno ){
	changed = yesno;
}

bool
VPJoint::vpIsChanged( void ){
	return changed;
}
		

VPVector3D
VPJoint::vpGetVectorX(){

	VPVector3D *v = new VPVector3D( lim.vpGetValueAt(0,0),
																	lim.vpGetValueAt(1,0),
																	lim.vpGetValueAt(2,0) );
	return *v;
}

VPVector3D
VPJoint::vpGetVectorY(){

	VPVector3D *v = new VPVector3D( lim.vpGetValueAt(0,1),
																	lim.vpGetValueAt(1,1),
																	lim.vpGetValueAt(2,1) );
	return *v;
}

VPVector3D
VPJoint::vpGetVectorZ(){

	VPVector3D *v = new VPVector3D( lim.vpGetValueAt(0,2),
																	lim.vpGetValueAt(1,2),
																	lim.vpGetValueAt(2,2) );
	return *v;
}

VPJoint*
VPJoint::vpGetParent(){

        return parentJoint;
}

std::list<VPJoint*>
VPJoint::vpGetChildList(){

        return childList;
}

std::list<VPGraphicObj*>
VPJoint::vpGetShapeList(){

        return shapeList;
}

VPDof**
VPJoint::vpGetDofList(){

        return dofList;
}

VPMatrix
VPJoint::vpGetLim(){

        return lim;
}

VPMatrix
VPJoint::vpGetInvLim(){

        return invLim;
}

void
VPJoint::vpSetNumDofs( unsigned short int n ){
	numDofs = n;
}


void
VPJoint::vpSetParent( VPJoint* j ){

        parentJoint = j;
}

void
VPJoint::vpSetChildList( std::list<VPJoint*> j ){

        childList = j;
}

void
VPJoint::vpSetLim( VPMatrix m ){

        lim = m;
}

void
VPJoint::vpSetInvLim( VPMatrix m ){

        invLim = m;
}


void
VPJoint::vpRefreshInvLim( void ){


	vpSetInvLim( *vpGetLim().vpGetInverse() );
}

void
VPJoint::vpAddShape( VPGraphicObj *shape ){

	shapeList.push_back( shape );
}

void
VPJoint::vpAddChild( VPJoint *child ){

	childList.push_back( child );
}

void
VPJoint::vpAddDof( VPDof *dof ){

	if( !dofList ) dofList = new VPDof*[MAXDOF];
	int ind = -1;
	while( dofList[++ind] );
	if( ind <= MAXDOF ){
		dofList[ind] = dof;
		numDofs++;
	}
}

void
VPJoint::vpMakeLim( void ){
	
	/// build translation to position matrix
	VPMatrix *newLim = new VPMatrix();
		
	/// Remake joint's Lim compounding the dof's Lims
	for( int	curDof = 0; curDof < vpGetNumDofs(); curDof++ ){
		newLim = dofList[curDof]->vpGetLim().vpMultiply( *newLim );
	}
		
	vpSetLim( *newLim );
	vpRefreshInvLim();
	
	// Set joint as changed
	vpSetChanged( true );
}

void
VPJoint::vpMakeGims( VPMatrix *composite ){
	
	/// Make up the new composite matrix
	VPMatrix	*newMatrix = vpGetLim().vpMultiply( *composite );
	
	/// Make Gim for every shape of the joint
	std::list<VPGraphicObj*>::iterator shapeIter;
	for( shapeIter = shapeList.begin(); shapeIter != shapeList.end(); shapeIter++ ){
		(*shapeIter)->vpSetGlobalInstanceMatrix( ( (*shapeIter)->vpGetLocalInstanceMatrix() )->vpMultiply( *newMatrix ) );
	}
	
	/// Make Gim for every children
	std::list<VPJoint*>::iterator jointIter;
	for( jointIter = childList.begin(); jointIter != childList.end(); jointIter++ ){
		(*jointIter)->vpMakeGims( newMatrix );
	}
}

void
VPJoint::vpPrintLim( void ){
	printf( "%f %f %f %f \n",vpGetLim().vpGetValueAt(0,0), vpGetLim().vpGetValueAt(0,1),
  												vpGetLim().vpGetValueAt(0,2), vpGetLim().vpGetValueAt(0,3) );
  printf( "%f %f %f %f \n",vpGetLim().vpGetValueAt(1,0), vpGetLim().vpGetValueAt(1,1),
  												vpGetLim().vpGetValueAt(1,2), vpGetLim().vpGetValueAt(1,3) );
	printf( "%f %f %f %f \n",vpGetLim().vpGetValueAt(2,0), vpGetLim().vpGetValueAt(2,1),
  												vpGetLim().vpGetValueAt(2,2), vpGetLim().vpGetValueAt(2,3) );
	printf( "%f %f %f %f \n\n",vpGetLim().vpGetValueAt(3,0), vpGetLim().vpGetValueAt(3,1),
  												vpGetLim().vpGetValueAt(3,2), vpGetLim().vpGetValueAt(3,3) );
}