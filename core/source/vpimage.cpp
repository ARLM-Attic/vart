//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpimage.cpp
//  DESCRIPTION.: Contain the VPImage class implementation, which
//				  is the superclass of VPVolume and VPImage2D classes.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/14/2000
//  DESCRIPTION.: Some methods declarations.
//
///////////////////////////////////////////////////////////////////


#include <vpimage.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPImage" constructor without parameter. 
// Parameters.: -
// Return.....: -

VPImage::VPImage () {
		xDimension = 0;
		yDimension = 0;
}

