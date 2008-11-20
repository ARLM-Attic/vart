/// \file vpbiaxialjoint.cpp
/// \brief Implementation file for VPBiaxialJoint VPAT class.
/// \version $Revision: 1.3 $

// ChangeLog
// May 20, 2005 - Bruno de Oliveira Schneider
// - Added some documentation.
// - SetAductionTo is now SetAdductionTo (fixed spelling, kept old name for compatibility).
// Jan 10, 2005 - Bruno de Oliveira Schneider
// - Fixed DoxyGen class description.
// - Removed previously commented constructors.
// Oct 07, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. Doxygen documentation started.
// - Removed "#include <stdio.h>" from implementation file.
// - Removed 'vp' preffix of every method name.
// - Reimplemented "AddDof(VPDof*)" according to changes in VPDof and VPJoint.
// - Commented out old constructors because they leak memory and because of changes
//   in VPJoint and VPDof.
// Jan 15, 2001 - Anderson Maciel
// - Class and new methods declaration.
// Jan 16, 2000 - Anderson Maciel
// - Methods implementation.

#include "vpbiaxialjoint.h"
#include <cassert>

using namespace std;

VPBiaxialJoint::VPBiaxialJoint()
{
}

VPBiaxialJoint::~VPBiaxialJoint()
{
}

void VPBiaxialJoint::SetFlexionTo(float value)
{
    VPDof* ptrFirstDof = dofList.front();
    ptrFirstDof->MoveTo(value);
}

void VPBiaxialJoint::SetAductionTo(float value)
{
    cerr << "Warning: VPBiaxialJoint::SetAductionTo is deprecated.\n";
    SetAdductionTo(value); 
}

void VPBiaxialJoint::SetAdductionTo(float value)
{
    assert(GetNumDofs() == 2);
    list<VPDof*>::iterator iter = dofList.begin();
    ++iter;
    VPDof* ptrSecondDof = *iter;
    ptrSecondDof->MoveTo(value);
}

void VPBiaxialJoint::Adduct(float variance)
{
    assert(GetNumDofs() == 2);
    list<VPDof*>::iterator iter = dofList.begin();
    ++iter;
    VPDof* ptrSecondDof = *iter;
    ptrSecondDof->Move(variance);
}

void VPBiaxialJoint::AddDof(VPDof* dof)
{
    assert(GetNumDofs() < 2);
    VPJoint::AddDof(dof);
    // If the second DOF was added, the joint should be made ready for use.
    if (GetNumDofs() == 2) MakeLim();
}
