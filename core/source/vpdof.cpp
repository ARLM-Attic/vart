/// \file vpdof.cpp
/// \brief Implementation file for VPDof VPAT class.
/// \version $Revision: 1.14 $

// ChangeLog
// May 25, 2006 - Dalton Reis
// Added #include <algorithm>, for the method find.
// May 17, 2006 - Bruno de Oliveira Schneider
// - Added some documentation.
// - Reverted Ughini's changes to destructor. The instance list must be updated.
// Abr 03, 2006 - Bruno de Oliveira Schneider
// - Constructors now update the instance list.
// - Added destructor.
// - Added MoveTo(float, unsigned int), ClearPriorities and instanceList.
// Mar 17, 2006 - Bruno de Oliveira Schneider
// - Now, axis is normalized when set.
// - DrawInstanceOGL now avoids drawing degenerated axes.
// Mar 16, 2006 - Bruno de Oliveira Schneider
// - Added code to draw DOFs if symbol VISUAL_JOINTS is defined.
// - Removed Transform(const VPTransform&).
// - Attributes position and axis changed from private to protected.
// - VPDof::GetLim() now returns a constant reference to lim.
// Mar 09, 2006 - Andreia Schneider
// - Fixed a bug in method MoveTo.
// Oct 31, 2005 - Bruno de Oliveira Schneider
// - Methods that do not change the DOF and do not return pointers are now declared const.
// Jun 02, 2005 - Bruno de Oliveira Schneider
// - VPDof(const VPPoint4D&, const VPPoint4D&, float, float) now computes LIM.
// - GetDescription is now inline and returns by reference.
// - DOFs are now derived from memory objects. The programmer no longer needs to keep track
//   of all DOFs in a body.
// - Added some documentation.
// Oct 08, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned (tabs replaced by spaces, excessive spaces
//   removed, etc.) because tabs and spaces were mixed together.
// - Removed "#include <stdio.h>". Removed "PrintLim" method.
// - Removed "vp" prefix from every method name.
// - Moved "using namespace std" from header file to implementation file.
// - Changed type of "lim" attribute from "VPMatrix" to "VPTransform".
// - Changed type of "axis" and "position" attributes to VPPoint4D.
// - Changed "SetDescription" to avoid passing a string by value and removed the
//   "char*" version because it is not needed.
// - Changed "GetEvoluta".
// - Commented out "bim" attribute which was never used.
// - Commented out "GetBim()".
// - Added "Move" (inline) and "Transform(const VPTransform&)" methods.
// - Added copy constructor and operator=.
// Feb 23, 2001 - Anderson Maciel
// - Class and new methods declaration.

#include "vpdof.h"
#include "vpjoint.h"
#include "vpmodifier.h"
#include <algorithm>

using namespace std;
#ifdef VISUAL_JOINTS
float VPDof::axisSize = 0.5;
#endif
list<VPDof*> VPDof::instanceList;

VPDof::VPDof()
{
    ownerJoint = NULL;
    evoluta = NULL;
    rangeModifier = NULL;

    // These initializations are needed in order to make a joint automatically compute
    // its transform when setting dofs. They could be removed if the application programmer
    // is forced to tell when a joint is ready to compute its transform.
    axis.SetXYZW(0,0,1,0);
    position.SetXYZW(0,0,0,1);
    lim.MakeIdentity();

    // FixMe: (by Bruno) Not sure if the folowing initializations are needed...
    minAngle = 0;
    maxAngle = 0;
    currentPosition = 0;
    restPosition = 0;
    instanceList.push_back(this);
}

VPDof::VPDof(const VPDof& dof)
{
    description = dof.description;
    position = dof.position;
    axis = dof.axis;
    evoluta = dof.evoluta; //FixMe: Maybe a new evoluta should be created
    lim = dof.lim;
    minAngle = dof.minAngle;
    maxAngle = dof.maxAngle;
    currentMinAngle = dof.currentMinAngle;
    currentMaxAngle = dof.currentMaxAngle;
    confortMinAngle = dof.confortMinAngle;
    confortMaxAngle = dof.confortMaxAngle;
    rangeModifier = dof.rangeModifier;
    currentPosition = dof.currentPosition;
    restPosition = dof.restPosition;
    ownerJoint = dof.ownerJoint;
    instanceList.push_back(this);
}

VPDof::VPDof(const VPPoint4D& vec, const VPPoint4D& pos, float min, float max)
{
    ownerJoint = NULL;
    evoluta = NULL;
    rangeModifier = NULL;
    axis = vec;
    position = pos;
    minAngle = min;
    currentMinAngle = min;
    maxAngle = max;
    currentMaxAngle = max;
    currentPosition = (0-min)/(max-min);
    axis.Normalize();
    ComputeLIM();
    instanceList.push_back(this);
}

VPDof::~VPDof()
{
    // remove itself from list of instances
    list<VPDof*>::iterator iter = find(instanceList.begin(), instanceList.end(), this);
    instanceList.erase(iter);
}

VPDof& VPDof::operator=(const VPDof& dof)
{
    description = dof.description;
    position = dof.position;
    axis = dof.axis;
    evoluta = dof.evoluta; //FixMe: Maybe a new evoluta should be created
    lim = dof.lim;
    minAngle = dof.minAngle;
    maxAngle = dof.maxAngle;
    currentMinAngle = dof.currentMinAngle;
    currentMaxAngle = dof.currentMaxAngle;
    confortMinAngle = dof.confortMinAngle;
    confortMaxAngle = dof.confortMaxAngle;
    rangeModifier = dof.rangeModifier;
    currentPosition = dof.currentPosition;
    restPosition = dof.restPosition;
    ownerJoint = dof.ownerJoint;
    return *this;
}

void VPDof::Set(const VPPoint4D& vec, const VPPoint4D& pos, float min, float max)
{
    axis = vec;
    position = pos;
    minAngle = min;
    currentMinAngle = min;
    maxAngle = max;
    currentMaxAngle = max;
    // Note (by Bruno): Although not stated anywhere, it seems that when DOFs are
    // created, their current position is that of zero rotation
    currentPosition = (0-min)/(max-min);
    axis.Normalize();
    // After been set, a dof should be ready to draw
    ComputeLIM();
}

void VPDof::SetDescription(const string& desc)
{
    description = desc;
}

VPPoint4D VPDof::GetAxis() const
{
    return axis;
}

VPPoint4D VPDof::GetOrigin() const
{
    VPPoint4D result;
    lim.GetTranslation(&result);
    return result;
}

const VPTransform& VPDof::GetLim() const
{
    return lim;
}

void VPDof::GetLim(VPTransform* ptrResult)
{
    *ptrResult = lim;
}

//~ VPMatrix
//~ VPDof::GetBim() {
    //~ /// Build BIM.
    //~ axis.vpNormalize();
    //~ VPVector3D vx = VPVector3D(1, 0, 0);
    //~ VPVector3D vy = axis.vpCrossProduct(vx);
    //~ // Verify linearity
    //~ if (vy == VPVector3D(0, 0, 0)) {
        //~ vx = VPVector3D(0, 0, 1);
        //~ vy = axis.vpCrossProduct(vx);
    //~ }
    //~ vy.vpNormalize();
    //~ vx = vy.vpCrossProduct(axis);
    //~ vx.vpNormalize();
    //~ float           fm[4][4];

    //~ fm[0][0] = vx.vpGetX();
    //~ fm[1][0] = vx.vpGetY();
    //~ fm[2][0] = vx.vpGetZ();
    //~ fm[3][0] = 0;

    //~ fm[0][1] = vy.vpGetX();
    //~ fm[1][1] = vy.vpGetY();
    //~ fm[2][1] = vy.vpGetZ();
    //~ fm[3][1] = 0;

    //~ fm[0][2] = axis.vpGetX();
    //~ fm[1][2] = axis.vpGetY();
    //~ fm[2][2] = axis.vpGetZ();
    //~ fm[3][2] = 0;

    //~ fm[0][3] = 0;
    //~ fm[1][3] = 0;
    //~ fm[2][3] = 0;
    //~ fm[3][3] = 1;

    //~ VPMatrix lBim = VPMatrix(fm);
    //~ return lBim;
//~ }

float VPDof::GetMin() const
{
    return minAngle;
}

float VPDof::GetMax() const
{
    return maxAngle;
}

float VPDof::GetCurrentMin() const
{
    float minModif = minAngle;
    if (rangeModifier)
        minModif = rangeModifier->GetMin();
    if (minAngle < minModif)
        return minModif;
    return minAngle;
}

float VPDof::GetCurrentMax() const
{
    float maxModif = maxAngle;
    if (rangeModifier)
        maxModif = rangeModifier->GetMax();
    if (maxAngle > maxModif)
        return maxModif;
    return maxAngle;
}

float VPDof::GetCurrent() const
{
    return currentPosition;
}

float VPDof::GetRest() const
{
    return restPosition;
}

VPJoint* VPDof::GetOwnerJoint()
{
    return ownerJoint;
}

void VPDof::SetAxis(VPVector3D za)
{
    axis = za;
}

void VPDof::SetEvoluta(VPBezier* evol)
{
    evoluta = evol;
}

// FixMe: There should not exist a "SetLim" because computing it is probably the
// main reason for the class to exist...
void VPDof::SetLim(const VPTransform& t)
{
    lim = t;
}

void VPDof::SetMin(float min)
{
    minAngle = min;
}

void VPDof::SetMax(float max)
{
    maxAngle = max;
}

void VPDof::MoveTo(float pos)
{
    // Avoid values out of range
    if (pos > 1.0) pos = 1.0;
    if (pos < 0.0) pos = 0.0;

    // Find angles by interpolation
    double newAngle = currentMinAngle + pos * (currentMaxAngle - currentMinAngle);
    //double currentAngle = currentMinAngle + currentPosition * (currentMaxAngle - currentMinAngle);

    // Find center of rotation
    VPPoint4D center = position;
    if (evoluta)
    {
        VPPoint4D bezierPoint;
        evoluta->GetPoint(currentPosition, &bezierPoint);
        bezierPoint.SetW(0); // Turn it into translation vector
        center = position + bezierPoint;
    }

    // Update Local Instance Matrix
    currentPosition = pos;
    lim.MakeRotation(center, axis, newAngle);

    // Update external (joint) state
    ownerJoint->MakeLim();
}

void VPDof::MoveTo(float pos, unsigned int newPriority)
{
    if (newPriority > priority)
    {
        priority = newPriority;
        MoveTo(pos);
    }
}

void VPDof::ComputeLIM()
{
    // Find angle by interpolation
    double angle = currentMinAngle + currentPosition * (currentMaxAngle - currentMinAngle);

    // Find center of rotation
    VPPoint4D center;
    if (evoluta)
    {
        VPPoint4D bezierPoint;
        evoluta->GetPoint(currentPosition, &bezierPoint);
        bezierPoint.SetW(0); // Turn it into translation vector
        center = position + bezierPoint;
    }
    else
        center = position;
    // Update Local Instance Matrix
    lim.MakeRotation(center, axis, angle);
}

void VPDof::SetOwnerJoint(VPJoint* ow)
{
    ownerJoint = ow;
}

void VPDof::SetRest(float rest)
{
    restPosition = rest;
}

void VPDof::Rest()
{
    MoveTo(restPosition);
}

void VPDof::SetRangeModifier(VPModifier* m)
{
    rangeModifier = m;
}

VPModifier* VPDof::GetRangeModifier()
{
    return rangeModifier;
}

void VPDof::ApplyTransformTo(VPTransform* ptrTrans) const
{
    *ptrTrans = lim * (*ptrTrans);
}

#ifdef VISUAL_JOINTS
bool VPDof::DrawInstanceOGL() const
{
#ifdef VP_OGL
    float width = axisSize * 0.03; // width of the axis line
    VPTransform axisRotation; // a rotation around the axis
    VPPoint4D scaledAxis; // a vector from base to tip
    VPPoint4D baseVector; // rotating this vector on the base, we get base vertices
    VPPoint4D vertexVector[4]; // vertices

    scaledAxis = axis * axisSize;
    if ((1 - axis.DotProduct(VPPoint4D::Y())) < 0.01)
        // axis is too close to Y
        baseVector = VPPoint4D::X() * width;
    else
        // axis is not close to Y
        baseVector = VPPoint4D::Y() * width;
    axisRotation.MakeRotation(axis, 1.57079632679489661923); // set the rotation
    vertexVector[0] = position + baseVector;
    vertexVector[1] = vertexVector[0] + scaledAxis;
    
    glBegin(GL_QUADS);
    for (int i = 1; i < 5; ++i)
    {
        axisRotation.ApplyTo(&baseVector);
        vertexVector[2] = (position + scaledAxis) + baseVector;
        vertexVector[3] = position + baseVector;
        
        glVertex4dv(vertexVector[3].VetXYZW());
        glVertex4dv(vertexVector[2].VetXYZW());
        glVertex4dv(vertexVector[1].VetXYZW());
        glVertex4dv(vertexVector[0].VetXYZW());
        
        vertexVector[0] = vertexVector[3];
        vertexVector[1] = vertexVector[2];
    }
    glEnd();
    return true;
#else
    return false;
#endif // VP_OGL
}
#endif // VISUAL_JOINTS

void VPDof::ClearPriorities()
// static method
{
    list<VPDof*>::iterator iter;
    for (iter = instanceList.begin(); iter != instanceList.end(); ++iter)
        (*iter)->priority = 0;
}
