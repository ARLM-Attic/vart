/// \file vpmodifier.cpp
/// \brief Implementation file for VPModifier VPAT class.
/// \version 1.1.1.2

// ChangeLog
// Oct 08, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Removed "vp" prefix from every method name.
// - Minor changes to "GetMin" and "GetMax" because of changes in VPCurve/VPBezier.
// Feb 28, 2001 - Anderson Maciel
// - Classes declaration.
// Feb 08, 2001 - Anderson Maciel
// - Classes declaration.

#include "vpmodifier.h"
#include "vpdof.h"

VPModifier::VPModifier() {
    dofList = NULL;
    maxPonderatorList = NULL;
    minPonderatorList = NULL;
    numDofs = 0;
}

VPModifier::VPModifier( VPDof **dofs, VPCurve *mins, VPCurve *maxs, int numD ) {
    dofList = dofs;
    maxPonderatorList = maxs;
    minPonderatorList = mins;
    numDofs = numD;
}

VPModifier::~VPModifier() {
    if( dofList != NULL ) delete [] dofList;
    if( minPonderatorList != NULL ) delete [] minPonderatorList;
    if( maxPonderatorList != NULL ) delete [] maxPonderatorList;
}

VPCurve*
VPModifier::GetMinPonderatorList() {
    return minPonderatorList;
}

VPCurve*
VPModifier::GetMaxPonderatorList() {
    return maxPonderatorList;
}

float
VPModifier::GetMin() {
    VPBezier* ptrMinPonderator;
    VPPoint4D ponderatorPoint;
    float aux;
    float min = MINANG;
    for( int ind = 0; ind < numDofs; ind++ ) {
        ptrMinPonderator = dynamic_cast<VPBezier*>(&(minPonderatorList[ind]));
        ptrMinPonderator->GetPoint(dofList[ind]->GetCurrent(), &ponderatorPoint);
        aux = ponderatorPoint.GetY();
        if( aux > min ) min = aux;
    }
    return min;
}

float
VPModifier::GetMax() {
    VPBezier* ptrMaxPonderator;
    VPPoint4D ponderatorPoint;
    float aux;
    float max = MAXANG;
    for( int ind = 0; ind < numDofs; ind++ ) {
        ptrMaxPonderator = dynamic_cast<VPBezier*>(&(maxPonderatorList[ind]));
        ptrMaxPonderator->GetPoint(dofList[ind]->GetCurrent(), &ponderatorPoint);
        aux = ponderatorPoint.GetY();
        if( aux < max ) max = aux;
    }
    return max;
}
