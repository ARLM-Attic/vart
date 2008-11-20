/// \file vpjoint.cpp
/// \brief Implementation file for VPJoint VPAT class.
/// \version $Revision: 1.10 $

// ChangeLog
// May 22, 2006 - Bruno de Oliveira Schneider
// - Added MoveDof.
// Mar 24, 2006 - Andreia Schneider
// - Added method to put the joint in a rest position.
// Mar 16, 2006 - Bruno de Oliveira Schneider
// - Added code to draw DOFs when symbol VISUAL_JOINTS is defined.
// - Removed TransformDofs(VPDof*, const VPTransform&).
// Feb 17, 2006 - Andreia Schneider
// - Included cassert library.
// Oct 27, 2005 - Bruno de Oliveira Schneider
// - Added GetDof.
// - Removed #include "vpmatrix.h".
// - Removed unused MAXDOF constant.
// May 18, 2005 - Bruno de Oliveira Schneider
// - Added destructor that deletes DOFs marked as autoDelete.
// Jan 10, 2005 - Bruno de Oliveira Schneider
// - Added "#include <algorithm>" because MS VC++ users reported compilation errors.
// Oct 08, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned (tabs replaced by spaces, excessive spaces
//   removed, etc.) because tabs and spaces were mixed together.
// - Removed "#include <stdio.h>" from implementation file. Commented out 
//   "PrintLim" method.
// - Moved "using namespace std" from header file to implementation.
// - Removed 'vp' preffix of every method name.
// - Turned VPJoint a subclass of VPTransform. AS A RESULT, MOST ATTRIBUTES AND
//   METHODS WERE REMOVED.
// - Reimplemented "MakeLim()".
// - Added "TransformDofs(VPDof*, const VPTransform&)" so that when one DOF changes
//   it may changed its siblings. This is temporary behaviour - how changes should
//   be carried to the siblings is still an open issue!
// Aug 10, 2000 - Anderson Maciel
// - Class and new methods declaration.

#include <algorithm>
#include <cassert>
#include "vpjoint.h"
#include "vpdof.h"

#include <iostream>
#ifdef VP_OGL
#include <GL/gl.h>
#endif

using namespace std;

VPJoint::VPJoint()
{
}

VPJoint::~VPJoint()
{
    list<VPDof*>::iterator iter;
    for (iter = dofList.begin(); iter != dofList.end(); ++iter)
    {
        if ((*iter)->autoDelete)
            delete *iter;
    }
}

unsigned short int VPJoint::GetNumDofs()
{
    return dofList.size();
}

void VPJoint::AddDof(VPDof* dof)
{
    dofList.push_back(dof);
    dof->SetOwnerJoint(this);
}

void VPJoint::MakeLim()
// In visual mode, the lim is not really used
{
// LIM = ...DOF3 * DOF2 * DOF1
    list<VPDof*>::reverse_iterator iter = dofList.rbegin();
    // Copy DOF1's matrix to this object
    (*iter)->GetLim(this);
    ++iter;
    while (iter != dofList.rend())
    {
        // this = dof * this
        (*iter)->ApplyTransformTo(this);
        ++iter;
    }
}

const VPDof& VPJoint::GetDof(DofID dof)
{
    assert(static_cast<int>(dofList.size()) > dof);
    list<VPDof*>::const_iterator iter = dofList.begin();
    int position = 0;
    while (position < dof)
    {
        ++position;
        ++iter;
    }
    return **iter;
}

void VPJoint::SetAtRest()
{
    list<VPDof*>::iterator iter;
    for(iter = dofList.begin(); iter != dofList.end(); ++iter)
        (*iter)->Rest();
}

bool VPJoint::MoveDof(DofID dof, float variance)
{
    unsigned int dofNum = static_cast<unsigned int>(dof);

    if (dofNum < dofList.size())
    {
        list<VPDof*>::iterator iter = dofList.begin();
        while (dofNum > 0)
        {
            ++iter;
            --dofNum;
        }
        (*iter)->Move(variance);
        return true;
    }
    else
        return false;
}

#ifdef VISUAL_JOINTS
bool VPJoint::DrawOGL() const
{
#ifdef VP_OGL
    bool result = true;
    list<VPSceneNode*>::const_iterator iter;
    list<VPDof*>::const_iterator dofIter;
    int i = 0;

    glPushMatrix();

    for (dofIter = dofList.begin(); dofIter != dofList.end(); ++dofIter)
    {
        glMultMatrixd((*dofIter)->GetLim().GetData());
        GetMaterial(i).DrawOGL();
        (*dofIter)->DrawInstanceOGL();
        ++i;
    }
    for (iter = childList.begin(); iter != childList.end(); ++iter)
        result &= (*iter)->DrawOGL();
    glPopMatrix();
    return result;
#else
    return false;
#endif // VP_OGL
}

const VPMaterial& VPJoint::GetMaterial(int num)
{
    static VPMaterial red(VPColor::RED());
    static VPMaterial green(VPColor::GREEN());
    static VPMaterial blue(VPColor::BLUE());
    switch (num)
    {
        case 0:
            return red;
            break;
        case 1:
            return green;
            break;
        default:
            return blue;
    }
}
#endif // VISUAL_JOINTS
