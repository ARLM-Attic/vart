/// \file vppolyaxialjoint.cpp
/// \brief Implementation file for VPPolyaxialJoint VPAT class.
/// \version $Revision: 1.8 $

// ChangeLog
// Apr 05, 2006 - Bruno de Oliveira Schneider
// - Added an empty virtual destructor.
// Mar 06, 2006 - Andreia Schneider
// - Added methods to flex, twist and adduct a joint.
// Jun 22, 2005 - Bruno de Oliveira Schneider
// - Fixed documentation: file and version commands were wrong and were confusing doxygen.
// - Replaced tabs with spaces for proper indentation.
// - Fixed date on changelog entry below. Fixed assert include (from C style to C++).
// Jun 09, 2005 - Marta Becker Villamil
// - Code typing has been cleaned. Doxygen documentation started.
// - Removed 'vp' preffix of every method name.
// - Reimplemented "AddDof(VPDof*)" according to changes in VPDof and VPJoint.
// - Commented out old constructors because they leak memory and because of changes
//   in VPJoint and VPDof.
// Jan 15, 2001 - Anderson Maciel
// - Class and new methods declaration.
// Jan 16, 2000 - Anderson Maciel
// - Methods implementation.

#include "vppolyaxialjoint.h"
#include <cassert>

using namespace std;

VPPolyaxialJoint::VPPolyaxialJoint()
{
}

void VPPolyaxialJoint::SetFlexionTo(float value) {
    VPDof* ptrFirstDof = dofList.front();
    ptrFirstDof->MoveTo(value);
}

void VPPolyaxialJoint::SetAductionTo(float value) {
    list<VPDof*>::iterator iter = dofList.begin();
    ++iter;
    VPDof* ptrSecondDof = *iter;
    ptrSecondDof->MoveTo(value);
}

void VPPolyaxialJoint::SetTwistTo(float value) {
    list<VPDof*>::iterator iter = dofList.begin();
    ++iter;
    ++iter;
    VPDof* ptrThirdDof = *iter;
    ptrThirdDof->MoveTo(value);
}

void VPPolyaxialJoint::Adduct(float variance)
{
    assert(GetNumDofs() == 3);
    list<VPDof*>::iterator iter = dofList.begin();
    ++iter;
    VPDof* ptrSecondDof = *iter;
    ptrSecondDof->Move(variance);
}

void VPPolyaxialJoint::Twist(float variance)
{
    assert(GetNumDofs() == 3);
    list<VPDof*>::iterator iter = dofList.begin();
    ++iter;
    ++iter;
    VPDof* ptrSecondDof = *iter;
    ptrSecondDof->Move(variance);
}

void VPPolyaxialJoint::AddDof(VPDof* dof) {
    assert (GetNumDofs() < 3);
    VPJoint::AddDof(dof);
    // If the third DOF was added, the joint should be made ready for use.
    if (GetNumDofs() == 3)
        MakeLim();
}
