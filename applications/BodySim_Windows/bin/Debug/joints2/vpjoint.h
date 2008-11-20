///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpjoint.h
//  DESCRIPTION.: Contain the VPJoint class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: August/10/2000
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef	__VPJOINT_H
#define	__VPJOINT_H


#ifndef __VPDOF_H
class VPDof;
#endif
#include	<vpmatrix.h>
#include	<vpgraphicobj.h>
#include	<vppoint3d.h>
#include	<vpvector3d.h>
#include	<list>
#include	<string>
//using namespace std;

#define MAXDOF 6


//-----------------------------------------------------------------------
// V P J O I N T
//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
// Class Name: VPJoint
// Superclass: none
// Subclass: VPPlaneJoint, VPUniaxialJoint, VPBiaxialJoint, VPPoliaxialJoint

class VPJoint{

    private:
		std::string	description;
		unsigned short int	numDofs;
		bool changed;
        VPMatrix	lim;
        VPMatrix	invLim;
        VPJoint*	parentJoint;
		std::list<VPGraphicObj*>	shapeList;
        VPDof** dofList;

    protected:
        void    vpSetInvLim( VPMatrix );

    public:
        VPJoint();
        VPJoint( VPJoint * );
        VPJoint( VPVector3D * );
        //~VPJoint();

		std::list<VPJoint*>			childList;

        unsigned short int	vpGetNumDofs( void );
        void	vpSetChanged( bool yesno );
				bool	vpIsChanged( void );
				void	vpSetDescription( char* );
				void	vpSetDescription( std::string );
				std :: string	vpGetDescription();
        VPVector3D	vpGetVectorX( void );
        VPVector3D	vpGetVectorY( void );
        VPVector3D	vpGetVectorZ( void );
        VPJoint*		vpGetParent( void );
		std::list<VPJoint*>		vpGetChildList( void );
		std::list<VPGraphicObj*>	vpGetShapeList( void );
        VPDof**		vpGetDofList( void );
        VPMatrix		vpGetLim( void );
        VPMatrix		vpGetInvLim( void );
        void	vpSetNumDofs( unsigned short int );
        void	vpSetParent( VPJoint* );
        void	vpSetChildList( std::list<VPJoint*> );
        void	vpSetLim( VPMatrix );
        void	vpRefreshInvLim( void );
        void	vpAddShape( VPGraphicObj *shape );
        void	vpAddChild( VPJoint *child );
        void	vpAddDof( VPDof *dof );
        void	vpMakeLim( void );
        void	vpPrintLim( void );
        void	vpMakeGims( VPMatrix *composite );
        //// Motion methods
        //void	vpSetFlexionTo( float value ){ };
        //virtual void	vpSetAductionTo( float value ){ };
        //virtual void	vpSetTwistTo( float value ){ };

};



#endif
