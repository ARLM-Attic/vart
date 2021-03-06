///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpdirectionallight.h
//  DESCRIPTION.: Contain the VPDirectionalLight class hierarchy declarations.
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
#ifndef __VPDIRECTIONALLIGHT_H	
#define __VPDIRECTIONALLIGHT_H	

#include	<vplight.h>



///////////////////////////////////////////////////////////////////
// Class Name: VPDirectionalLight
// Superclass: VPLight
// Subclasses: -


class VPDirectionalLight : public VPLight {

	private:
		VPVector3D direction;

	public:
		VPDirectionalLight( VPVector3D d );
		//eu//~VPDirectionalLight();
		void vpSetDirection( VPVector3D d );
		VPVector3D vpGetDirection( void );

};

#endif