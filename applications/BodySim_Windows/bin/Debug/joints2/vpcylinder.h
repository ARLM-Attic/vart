///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcylinder.h
//  DESCRIPTION.: Contain the VPCylinder class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 10/August/2000
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPCYLINDER_H
#define __VPCYLINDER_H

#include	<vpgraphicobj.h>


///////////////////////////////////////////////////////////////////
// Class Name: VPCylinder
// Superclass: VPGraphicObj
// Subclasses: none


class VPCylinder : public VPGraphicObj{

	private:
		float height;
		float radius;
		bool side;
		bool top;
		bool bottom;

	public:
		VPCylinder();
		VPCylinder( float fHi, float fRad );
		VPCylinder( float fHi, float fRad, bool bS, bool bT, bool bB );
		void	vpSetHeight( float h );
		void	vpSetRadius( float r );
		void	vpShowSide( bool yesno );
		void	vpShowTop( bool yesno );
		void	vpShowBottom( bool yesno );
		float	vpGetHeight( void );
		float	vpGetRadius( void );
		bool	vpShowSide( void );
		bool	vpShowTop( void );
		bool	vpShowBottom( void );
		void	vpRender();
};

#endif