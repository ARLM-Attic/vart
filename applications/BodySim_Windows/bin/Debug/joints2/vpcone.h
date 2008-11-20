///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcone.h
//  DESCRIPTION.: Contain the VPCone class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 10/August/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPCONE_H
#define __VPCONE_H

#include	<vpgraphicobj.h>


///////////////////////////////////////////////////////////////////
// Class Name: VPCone
// Superclass: VPGraphicObj
// Subclasses: none


class VPCone : public VPGraphicObj{

	private:
		float  bottomRadius;
		float height;
		bool side;
		bool bottom;
		
	public:
};

#endif