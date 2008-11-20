///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpspotlight.h
//  DESCRIPTION.: Contain the VPSpotlight class hierarchy declarations.
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
#ifndef __VPSPOTLIGHT_H	
#define __VPSPOTLIGHT_H	

#include	<vplight.h>


///////////////////////////////////////////////////////////////////

// Class Name: VPSpotLight
// Superclass: VPLight
// Subclasses: -

class VPSpotLight : public VPLight {

	private:
		VPVector3D attenuation;
		float beamWidth;
		float cutOffAngle;
		VPPoint3D location;
		VPVector3D direction;
		float radius;

	public:
		VPSpotLight(VPVector3D a, float bw, float coa,
		VPPoint3D l, VPVector3D d, float r);
		//eu//	~VPSpotLight();
		void vpSetAttenuation( VPVector3D a );
		VPVector3D vpGetAttenuation( void );
		void vpSetBeamWidth( float bw );
		float vpGetBeamWidth( void );
		void vpSetCutOffAngle( float coa );
		float vpGetCutOffAngle( void );
		void vpSetLocation( VPPoint3D l );
		VPPoint3D vpGetLocation( void );
		void vpSetDirection( VPVector3D d );
		VPVector3D vpGetDirection( void );
		void vpSetRadius( float r );
		float vpGetRadius( void );

};

#endif