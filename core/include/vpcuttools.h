//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcuttools.h
//  DESCRIPTION.: Contain the vpCutTools class hierarchy declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: December/12/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPCUTTOOLS_H	
#define __VPCUTTOOLS_H


#include <vppoint3d.h>


//-----------------------------------------------------------------
// C U T   T O O L S    
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPCutTools
// Superclass: -
// Subclasses: VPCutVolume, VPCutPlane
///\deprecated

class VPCutTools {
	private:
		VPPoint3D left;
		VPPoint3D right;
	public:
		VPCutTools();
		VPPoint3D vpGetPointLeft(void);
		void vpSetPointLeft(VPPoint3D l);
		VPPoint3D vpGetPointRight(void);
		void vpSetPointRight(VPPoint3D r);
};


#endif	// __VPCUTTOOLS_H
