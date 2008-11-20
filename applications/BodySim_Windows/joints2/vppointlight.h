///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vppointlight.h
//  DESCRIPTION.: Contain the VPPointLight class hierarchy declarations.
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
//  DATE........: August/10/2000
//  DESCRIPTION.: Classes declaration and methods addition.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPPOINTLIGHT_H	
#define __VPPOINTLIGHT_H	

#include	<vplight.h>

///////////////////////////////////////////////////////////////////
// Class Name: VPPointLight
// Superclass: VPLight
// Subclasses: -


class VPPointLight : public VPLight {

	private:
		VPVector3D attenuation;
		VPPoint3D location;

	public:
		VPPointLight(VPVector3D a, VPPoint3D l);
		//eu//~VPPointLight();
		void vpSetAttenuation( VPVector3D a );
		VPVector3D vpGetAttenuation( void );
		void vpSetLocation( VPPoint3D l );
		VPPoint3D vpGetLocation( void );

};


#endif