///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: VPmodifier.h
//  DESCRIPTION.: Contain the VPModifier class hierarchy declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: 08/February/2001
//  DESCRIPTION.: Classes declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef __VPMODIFIER_H
#define __VPMODIFIER_H

#ifndef __VPDOF_H
class VPDof;
#endif

//#include	<vpcurve.h>
#include	<vpbezier.h>


#define MINANG -7;	// 2*(-PI) rounded
#define MAXANG 7;		// 2*PI rounded

class VPModifier {

	private:
		VPCurve	*maxPonderatorList;
		VPCurve	*minPonderatorList;
		VPDof		**dofList;
		int			numDofs;
		
	public:
		VPModifier();
		VPModifier( VPDof **dofs, VPCurve *mins, VPCurve *maxs, int numD );
		~VPModifier();
		void	vpSetMaxList( VPCurve *list );
		void	vpSetMinList( VPCurve *list );
		void	vpSetDofList( VPDof **list );
		VPCurve	*vpGetMinPonderatorList();
		VPCurve	*vpGetMaxPonderatorList();
		float	vpGetMin();
		float	vpGetMax();
};

#endif
