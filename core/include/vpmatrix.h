//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpmatrix.h
//  DESCRIPTION.: Contain the VPMatrix class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 09/June/2000
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef __VPMATRIX_H
#define __VPMATRIX_H


#include	<vppoint3d.h>

//-----------------------------------------------------------------------
// V P M A T R I X
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPMatrix
// Superclass: -
// Subclass: -
// Describes a 4x4 homogeneous matrix for most CG transformations

typedef float mat44 [4][4];
///\deprecated

class VPMatrix{


	private:
		float	data[4][4];

	protected:
		VPMatrix*	vpGetAdjoint( void );
		float		vpGetDet33( float, float, float, float, float, float, float, float, float );
		float		vpGetDet22( float, float, float, float );

	public:
		VPMatrix();
		VPMatrix( float[] );
		VPMatrix( float[][4] );
		~VPMatrix();

		float		vpGetValueAt( int, int );
		void		vpSetValueAt( int, int, float );
		//mat44*		vpGetMatrixF( void );
		void		vpGetMatrixF( float ** );
		float*	vpGetMatrixVF( void );

		VPMatrix*	vpGetInverse( void );
		float		vpGetDeterminent( void );

		VPMatrix*	vpMultiplyScalar( float );
		VPMatrix*	vpMultiply( VPMatrix );
		VPMatrix*	vpSubtract( VPMatrix );
		VPMatrix*	vpAdd( VPMatrix );
		VPPoint3D*	vpMultiply ( VPPoint3D );

};

#endif
