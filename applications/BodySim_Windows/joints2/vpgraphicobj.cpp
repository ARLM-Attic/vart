#include	<vpgraphicobj.h>
#include	<vpscene.h>


VPGraphicObj::VPGraphicObj(){

	scene = NULL;
	show = true;
}

VPGraphicObj::VPGraphicObj( VPScene *sc ){

	scene = sc;
	show = true;
}

VPGraphicObj::~VPGraphicObj(){
	//do nothing
}

std::string
VPGraphicObj::vpGetFileName(){
	return fileName;
}

void
VPGraphicObj::vpSetFileName( std::string fName ){
	fileName = fName;
}

std::string
VPGraphicObj::vpGetDescription(){
	return description;
}

void
VPGraphicObj::vpSetDescription( std::string desc ){
	description = desc;
}

VPMatrix*
VPGraphicObj::vpGetLocalInstanceMatrix(){
	return &localInstanceMatrix;
}

void
VPGraphicObj::vpSetLocalInstanceMatrix( VPMatrix* nMtx ){
	localInstanceMatrix = *nMtx;
}

VPMatrix*
VPGraphicObj::vpGetGlobalInstanceMatrix(){
        return &globalInstanceMatrix;
}

void
VPGraphicObj::vpSetGlobalInstanceMatrix( VPMatrix* nMtx ){
        globalInstanceMatrix = *nMtx;
}

VPScene*
VPGraphicObj::vpGetScene(){
	return scene;
}

void
VPGraphicObj::vpSetScene( VPScene* sc ){
	scene = sc;
}

void
VPGraphicObj::vpShow( void ){
	show = true;
}
		
void
VPGraphicObj::vpHide( void ){
	show = false;
}