///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpdof.h
//  DESCRIPTION.: Contain the VPDof class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 23/February/2001
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef __VPDOF_H
#define __VPDOF_H

#ifndef __VPJOINT_H
class VPJoint;
#endif

#ifndef __VPMODIFIER_H
class VPModifier;
#endif

#include	<vppoint3d.h>
#include	<vpvector3d.h>
#include	<vpmatrix.h>
#include	<vpbezier.h>
#include	<string>

//~//using namespace std;



//-----------------------------------------------------------------------
// V P D O F
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPDof
// Superclass: none
// Subclass: none


class VPDof{

	private:
		std::string		description;// Name of the Dof; often related to the dof's type of motion
		VPPoint3D position;		// Position of the dof related to the parent reference system
		VPVector3D axis;			// Arbitrary axis of rotation
		VPBezier	*evoluta;		// 3D path related to the axis position along its rotation
		VPMatrix	lim;				// Local Instance Matrix
		VPMatrix	bim;				// Base Instance Matrix
		float		minAngle;			// Min base angle in rad.
		float		maxAngle;			// Max base angle in rad.
		float		currentMinAngle;			// Min angle in rad currently valid.
		float		currentMaxAngle;			// Max angle in rad currently valid.
		float		confortMinAngle;			// Min angle in rad for comfortable position.
		float		confortMaxAngle;			// Max angle in rad for comfortable position.
		VPModifier	*rangeModifier;		// Entity used to change the motion range along motion.
		float		currentPosition;	//A real number from 0 to 1
		float		restPosition;			//Another real number from 0 to 1
		VPJoint	*ownerJoint;			//Reference to the joint where this dof is set up
	
	protected:
		void				setAxis( VPVector3D ax );
		
	public:
		VPDof();
		VPDof( VPVector3D za, VPPoint3D pos, VPBezier *evol );
		VPDof( VPVector3D za, VPPoint3D pos, VPCurve *evol,
						float min, float max, float cur, float rest );
		VPDof( VPVector3D za, VPPoint3D pos, VPBezier *evol,
						float min, float max, float cur, float rest, VPModifier *rm );
		//~VPDof();	
		void	vpSetDescription( char* );
		void	vpSetDescription( std::string );
		std::string			vpGetDescription();
		VPVector3D	vpGetAxis( void );
		VPPoint3D		vpGetOrigin( void );
		VPCurve			vpGetEvoluta( void );
		VPMatrix		vpGetLim( void );
		VPMatrix		vpGetBim( void );
		float				vpGetMin( void );
		float				vpGetMax( void );
		float				vpGetCurrentMin( void );
		float				vpGetCurrentMax( void );
		float				vpGetCurrent( void );
		float				vpGetRest( void );
		VPJoint*		vpGetOwnerJoint( void );
		void				vpSetEvoluta( VPBezier *evol );
		void				vpSetLim( VPMatrix m );
		void				vpSetAxis( VPVector3D v );
		void				vpSetMin( float min );
		void				vpSetMax( float max );
		void				vpSetOwnerJoint( VPJoint *ow );
		void				vpMoveTo( float pos );
		void				vpSetRest( float rest );
		void				vpRest( void );
		void				vpSetRangeModifier( VPModifier *m );
		VPModifier*	vpGetRangeModifier( void );
		void				vpPrintLim( void );
};

#endif
