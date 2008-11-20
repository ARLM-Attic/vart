///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpscene.h
//  DESCRIPTION.: Contain the vpSceneParameters class declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: 30/May/2000
//  DESCRIPTION.: Class declaration.
//
//  AUTHOR......: Luciana Porcher Nedel
//  DATE........: 08/June/2000
//  DESCRIPTION.: Class declaration.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 27/September/2000
//  DESCRIPTION.: New methods and attributes addition.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPSCENE_H
#define __VPSCENE_H


#ifndef __VPGRAPHICOBJ_H
class VPGraphicObj;
#endif

#include	<vplight.h>
#include	<vpcamera.h>
#include	<vpcolor.h>
#include	<string>	//STL include
#include	<list>		//STL include
//using namespace std;



///////////////////////////////////////////////////////////////////
// Class Name: VPScene
// Superclass: -
// Subclasses: -

class VPScene {
	
	private:
		std::list<VPLight*>		lights;
		std::list<VPCamera*>		cameras;
		std::list<VPGraphicObj*>	objects;
		VPColor 		background;
		bool			lightChanged;
		bool			cameraChanged;
		bool			objectChanged;
		std::list<VPLight*>		lightListChanged;
		std::list<VPCamera*>		cameraListChanged;
		std::list<VPGraphicObj*>	objectListChanged;
		
	public:
		VPScene();
		~VPScene();

		std::list<VPLight*> vpGetLights( void );
		std::list<VPCamera*> vpGetCameras( void );
		std::list<VPGraphicObj*> vpGetObjects( void );
		void vpAddLight( VPLight* newLight );
		void vpAddCamera( VPCamera* newCamera );
		void vpAddObject( VPGraphicObj* newObject );
		VPLight* vpGetLight( std::string lightName );
		VPCamera* vpGetCamera( std::string cameraName );
		VPGraphicObj* vpGetObject( std::string objectName );
		VPColor vpGetBackgroundColor( void );
		void vpSetBackgroundColor( VPColor color );
		void vpSetChanged( bool yesno, VPLight *ltChg );
		void vpSetChanged( bool yesno, VPCamera *camChg );
		void vpSetChanged( bool yesno, VPGraphicObj *objChg );

};




#endif	// __VPSCENE_H
