//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpquantificationtools.h
//  DESCRIPTION.: Contain the VPQuantificationTools class hierarchy 
//				  declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: May/30/2000
//  DESCRIPTION.: Class declaration.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPQUANTIFICATIONTOOLS_H	
#define __VPQUANTIFICATIONTOOLS_H




//-----------------------------------------------------------------
// Q U A N T I F I C A T I O N   T O O L S    
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPQuantificationTools
// Superclass: -
// Subclasses: VPPointValue, VPDistanceValue, VPVolumeValue
///\deprecated

class VPQuantificationTools {
	public:
		VPQuantificationTools() {};
};


///////////////////////////////////////////////////////////////////
// Class Name: VPPointValue
// Superclass: VPQuantificationTools
// Subclasses: -
///\deprecated

class VPPointValue: public VPQuantificationTools {
	public:
		VPPointValue();
};


///////////////////////////////////////////////////////////////////
// Class Name: VPDistanceValue
// Superclass: VPQuantificationTools
// Subclasses: -
///\deprecated

class VPDistanceValue: public VPQuantificationTools {
	public:
		VPDistanceValue();
};


///////////////////////////////////////////////////////////////////
// Class Name: VPVolumeValue
// Superclass: VPQuantificationTools
// Subclasses: -
///\deprecated

class VPVolumeValue: public VPQuantificationTools {
	public:
		VPVolumeValue();
};



#endif	// __VPQUANTIFICATIONTOOLS_H
