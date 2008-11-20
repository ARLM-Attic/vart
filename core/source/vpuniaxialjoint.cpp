/// \file vpuniaxialjoint.cpp
/// \brief Implementation file for VPUniaxialJoint VPAT class.
/// \version $Revision: 1.7 $

// ChangeLog
// Fev 17, 2006 - Andreia Schneider
// - Implemented SetFlexionTo, Flex, reimplemented AddDof and removed older methods.
// Jan 15, 2001 - Anderson Maciel
// - Class and new methods declaration.
// Aug 10, 2000 - Anderson Maciel
// - Methods implementation.

#include "vpuniaxialjoint.h"
#include <cassert>

using namespace std;

VPUniaxialJoint::VPUniaxialJoint()
{
}

VPUniaxialJoint::~VPUniaxialJoint()
{
}

void VPUniaxialJoint::SetFlexionTo(float value)
{
    VPDof* ptrFirstDof = dofList.front();
    ptrFirstDof->MoveTo(value);
}

void VPUniaxialJoint::AddDof(VPDof* dof)
{
    assert(GetNumDofs() <=1);
    VPJoint::AddDof(dof);
    if (GetNumDofs() == 1) MakeLim();
}
