///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: Light.h
//  DESCRIPTION.: Contain the vpLight class hierarchy declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: May/22/2000
//  DESCRIPTION.: Classes declaration.
//
//  AUTHOR......: Luciana Porcher Nedel
//  DATE........: June/08/2000
//  DESCRIPTION.: Classes declaration.
//
//  AUTHOR......: Rodrigo Berggevist Martins
//  DATE........: June/19/2000
//  DESCRIPTION.: Classes declaration.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: June/28/2000
//  DESCRIPTION.: Classes declaration and methods addition.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPLIGHT_H	
#define __VPLIGHT_H	


#include <vppoint3d.h>
#include <vpvector3d.h>
#include <vpcolor.h>
#include <string>	//STL include

//using namespace std;


#ifndef ON
#define ON	true	//1
#define OFF	false	//0
#endif


///////////////////////////////////////////////////////////////////
// Class Name: VPLight
// Superclass: -
// Subclasses: VPDirectionalLight, VPPointLight, VPSpotLight

class VPLight {

	private:
		std::string	description;
		float intensity;
		float ambientIntensity;
		VPColor color;
		bool on;

	public:
		VPLight();
		VPLight(float i, float ai, VPColor c, bool o);
		~VPLight();
		void vpSetDescription( std::string desc );
		std::string vpGetDescription( void );
		void vpSetIntensity ( float i );
		float vpGetIntensity ( void );
		void vpSetAmbientIntensity( float ai );
		float vpGetAmbientIntensity( void );
		void vpSetColor( VPColor c );
		VPColor vpGetColor( void );
		void vpTurn( bool on_off );
		bool vpIsOn( void );

};

#endif	// __VPLIGHT_H

