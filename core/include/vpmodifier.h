/// \file vpmodifier.h
/// \brief Header file for VPModifier VPAT class.
/// \version $Revision: 1.4 $

// ChangeLog is at the implementation file

#ifndef __VPMODIFIER_H
#define __VPMODIFIER_H

#ifndef __VPDOF_H
class VPDof;
#endif

#include    <vpbezier.h>

#define MINANG -7;  // 2*(-PI) rounded
#define MAXANG 7;       // 2*PI rounded

/// \class VPModifier vpmodifier.h
/// \brief A modifier controls how one DOF influences other DOF
///
/// FixMe: Put a detailed description here.
class VPModifier {
    public:
        VPModifier();
        VPModifier( VPDof **dofs, VPCurve *mins, VPCurve *maxs, int numD );
        ~VPModifier();
        void    SetMaxList( VPCurve *list );
        void    SetMinList( VPCurve *list );
        void    SetDofList( VPDof **list );
        VPCurve *GetMinPonderatorList();
        VPCurve *GetMaxPonderatorList();
        float   GetMin();
        float   GetMax();
    private:
        VPCurve *maxPonderatorList;
        VPCurve *minPonderatorList;
        VPDof       **dofList;
        int         numDofs;
};

#endif
