#include	<vpbody.h>

using std::string;

VPBody::VPBody(){

	rootJoint = NULL;
}

VPBody::VPBody( std::string desc ){
	
	description = desc;
	rootJoint = NULL;
}

VPBody::VPBody( std::string fname, std::string desc, VPJoint *jointsTree ){
	
	fileName = fname;
	description = desc;
	rootJoint = jointsTree;
	vpSetChanged( true );
	vpUpdate();
}
		
//VPBody::~VPBody(){

//}


void
VPBody::vpSetFileName( char* szName ){
	fileName = szName;
}

void
VPBody::vpSetFileName( std::string sName ){
	fileName = sName;
}

void
VPBody::vpSetDescription( char* szDesc ){
	description = szDesc;
}

void
VPBody::vpSetDescription( std::string sDesc ){
	description = sDesc;
}

std::string
VPBody::vpGetFileName(){
	return fileName;
}

std::string
VPBody::vpGetDescription(){
	return description;
}

void
VPBody::vpSetRootJoint( VPJoint* newRoot){
	rootJoint = newRoot;
}

void
VPBody::vpSetChanged( bool yesno ){
	changed = yesno;
}
		
bool
VPBody::vpIsChanged( void ){
	return changed;
}

void
VPBody::vpUpdate( void ){
	if( vpIsChanged() ){
		vpMakeShapeGims();
	}
}
		
VPJoint*
VPBody::vpGetRootJoint(){
	return rootJoint;
}

VPJoint*
VPBody::vpGetJointByName( string target ){

  return vpSearchJoint( rootJoint, target );
}

VPJoint*
VPBody::vpSearchJoint( VPJoint* currJoint, string target ){

  if( currJoint->vpGetDescription() == target ){
    return currJoint;
  } else {
	  std::list<VPJoint*>::iterator jointIter;
/*
	  int aaa = currJoint->vpGetChildList().size();
    for( jointIter = currJoint->vpGetChildList().begin(); jointIter != currJoint->vpGetChildList().end(); jointIter++ ){
	    VPJoint* ccc = (*jointIter);
		VPJoint* found = vpSearchJoint( ccc, target );
	    if( found != NULL ){
	      return found;
	    }
	}
  }
*/
    for( jointIter = currJoint->childList.begin(); jointIter != currJoint->childList.end(); jointIter++ ){
	    VPJoint* ccc = (*jointIter);
		VPJoint* found = vpSearchJoint( ccc, target );
	    if( found != NULL ){
	      return found;
	    }
	}
  }
  return NULL;
}


void
VPBody::vpMakeShapeGims( void ){
	/// Update recursively the GIMs of all shapes in the hierarchy
	rootJoint->vpMakeGims( new VPMatrix() );
}

