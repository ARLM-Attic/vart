///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpscene.cpp
//  DESCRIPTION.: Contain the vpScene methods implementation.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 22/June/2000
//  DESCRIPTION.: Methods implementation.
//
///////////////////////////////////////////////////////////////////

#include	<vpscene.h>
#include	<vpgraphicobj.h>

using std::string;

VPScene::VPScene(){

        background = VPColor( 0, 0, 0, 0 );
        lightChanged = false;
        cameraChanged = false;
        objectChanged = false;
}

VPScene::~VPScene(){
	// do nothing
}

std::list<VPLight*>
VPScene::vpGetLights( void ){

	return lights;
}

std::list<VPCamera*>
VPScene::vpGetCameras( void ){

	return cameras;
}

std::list<VPGraphicObj*>
VPScene::vpGetObjects( void ){

	return objects;
}

void
VPScene::vpAddLight( VPLight* newLight ){

	lights.push_back( newLight );
}

void
VPScene::vpAddCamera( VPCamera* newCamera ){

	cameras.push_back( newCamera );
}

void
VPScene::vpAddObject( VPGraphicObj* newObject ){

	objects.push_back( newObject );
}

VPLight*
VPScene::vpGetLight( std::string lightName ){

	std::list<VPLight*>::const_iterator iter;
        for (iter = lights.begin(); !(iter == lights.end()); iter++){
        	if( (*iter)->vpGetDescription() == lightName ){
                	return (*iter);
                }
        }
        return NULL;
}

VPCamera*
VPScene::vpGetCamera( string cameraName ){

	std::list<VPCamera*>::const_iterator iter;
        for (iter = cameras.begin(); !(iter == cameras.end()); iter++){
        	if( (*iter)->vpGetDescription() == cameraName ){
                	return (*iter);
                }
        }
        return NULL;
}

VPGraphicObj*
VPScene::vpGetObject( string objectName ){

	std::list<VPGraphicObj*>::const_iterator iter;
        for (iter = objects.begin(); !(iter == objects.end()); iter++){
        	if( (*iter)->vpGetDescription() == objectName ){
                	return (*iter);
                }
        }
        return NULL;
}

VPColor
VPScene::vpGetBackgroundColor( void ){

	return background;
}

void
VPScene::vpSetBackgroundColor( VPColor color ){

	background = color;
}

void
VPScene::vpSetChanged( bool yesno, VPLight *ltChg ){

	lightListChanged.push_back( ltChg );
        lightChanged = yesno;
}

void
VPScene::vpSetChanged( bool yesno, VPCamera *camChg ){

	cameraListChanged.push_back( camChg );
        cameraChanged = yesno;

}

void
VPScene::vpSetChanged( bool yesno, VPGraphicObj *objChg ){

	objectListChanged.push_back( objChg );
        objectChanged = yesno;

}

