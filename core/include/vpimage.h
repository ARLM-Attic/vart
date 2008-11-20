//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpimage.h
//  DESCRIPTION.: Contain the VPImage class hierarchy declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: June/26/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPIMAGE_H	
#define __VPIMAGE_H	


#include <vpgraphicobj.h>
#include <vppoint3d.h>




//-----------------------------------------------------------------
// I M A G E    
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPImage
// Superclass: VPGraphicObj
// Subclasses: VPImage2D, VPVolume
///\deprecated

class VPImage : public VPGraphicObj {
	protected:
		int xDimension;
		int yDimension;
	public:
		VPImage();
		inline int vpGetXDimension() { return xDimension; }
		inline int vpGetYDimension() { return yDimension; }
};




#endif	// __VPIMAGE_H
