/// \file vpboundingbox.cpp
/// \brief Implementation file for VPBoundingBox VPAT class.
/// \version $Revision: 1.12 $

// ChangeLog
// May 18, 2006 - Bruno de Oliveira Schneider
// - Added "CopyGeometryFrom".
// May 17, 2006 - Dalton Reis
// - Added SetVisibility
// May 17, 2006 - Dalton Reis
// - Added GetSmallerEdge, GetEdgeX, GetEdgeY and GetEdgeZ
// May 17, 2006 - Bruno de Oliveira Schneider
// - DrawInstanceOGL now conforms to standards (checks for VP_OGL, retuns bool).
// - Optimized DrawInstanceOGL for better speed.
// May 11, 2006 - Dalton Reis
// - Added attribute of the color and corrected errors in method Transform.
// May 08, 2006 - Dalton Reis
// - Added a transform method
// Jul 03, 2005 - Andreia Schneider
// - Added intersection test among AAbbs.
// - Added a draw method.
// - Added the atribute visible.
// - Added ToggleVisibility.
// Dec 15, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Attributes were changed from float to double.
// - Removed "vp" prefix from every method name.
// - Changed "center" type from VPPoint3D* to VPPoint4D, along with realted methods.
// - Added "const" modifier wherever possible.
// - Added a copy constructor, operator= and an ouput operator.
// - Added GetGreaterEdge, ConditionalUpdate and MergeWith.
// Mar 01, 2001 - Isabel Harb Manssour
// - File and variables rename.
// Jan 23, 2001 - Fernando Sola Pereira
// - Methods implementation.
// - Class declarations.

#include <vpboundingbox.h>
#include "vptransform.h"

using namespace std;

VPBoundingBox::VPBoundingBox() {
    visible = false;
    color = VPColor::RED();
}

VPBoundingBox::VPBoundingBox(double minX, double minY, double minZ,
                             double maxX, double maxY, double maxZ) {
    visible = false;
    smallerX = minX;
    smallerY = minY;
    smallerZ = minZ;
    greaterX = maxX;
    greaterY = maxY;
    greaterZ = maxZ;
    color = VPColor::RED();
    ProcessCenter();
}

VPBoundingBox::VPBoundingBox(const VPBoundingBox& box) {
    visible = box.visible;
    smallerX = box.smallerX;
    smallerY = box.smallerY;
    smallerZ = box.smallerZ;
    greaterX = box.greaterX;
    greaterY = box.greaterY;
    greaterZ = box.greaterZ;
    center   = box.center;
    color    = box.color;
}

VPBoundingBox& VPBoundingBox::operator=(const VPBoundingBox& box) {
    visible = box.visible;
    smallerX = box.smallerX;
    smallerY = box.smallerY;
    smallerZ = box.smallerZ;
    greaterX = box.greaterX;
    greaterY = box.greaterY;
    greaterZ = box.greaterZ;
    center   = box.center;
    color    = box.color;
    return *this;
}

void VPBoundingBox::CopyGeometryFrom(const VPBoundingBox& box) {
    smallerX = box.smallerX;
    smallerY = box.smallerY;
    smallerZ = box.smallerZ;
    greaterX = box.greaterX;
    greaterY = box.greaterY;
    greaterZ = box.greaterZ;
    center   = box.center;
}


void VPBoundingBox::ProcessCenter() {
    center.SetXYZW((greaterX + smallerX)/2,
                   (greaterY + smallerY)/2,
                   (greaterZ + smallerZ)/2, 1);
}

void VPBoundingBox::Transform(const VPTransform& trans) {
    VPBoundingBox box = *this;

    VPPoint4D ptoBBox = trans * VPPoint4D(box.GetSmallerX(),box.GetSmallerY(),box.GetSmallerZ());
    this->SetBoundingBox(ptoBBox.GetX(),ptoBBox.GetY(),ptoBBox.GetZ(),ptoBBox.GetX(),ptoBBox.GetY(),ptoBBox.GetZ());
    this->ConditionalUpdate(trans * VPPoint4D(box.GetSmallerX(), box.GetSmallerY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VPPoint4D(box.GetSmallerX(), box.GetGreaterY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VPPoint4D(box.GetSmallerX(), box.GetGreaterY(), box.GetSmallerZ()));
    this->ConditionalUpdate(trans * VPPoint4D(box.GetGreaterX(), box.GetSmallerY(), box.GetSmallerZ()));
    this->ConditionalUpdate(trans * VPPoint4D(box.GetGreaterX(), box.GetSmallerY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VPPoint4D(box.GetGreaterX(), box.GetGreaterY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VPPoint4D(box.GetGreaterX(), box.GetGreaterY(), box.GetSmallerZ()));
    ProcessCenter();
}

double VPBoundingBox::GetGreaterX() const {
    return greaterX;
}

double VPBoundingBox::GetGreaterY() const {
    return greaterY;
}

double VPBoundingBox::GetGreaterZ() const {
    return greaterZ;
}

void VPBoundingBox::SetGreaterX(double v) {
    greaterX=v;
}

void VPBoundingBox::SetGreaterY(double v) {
    greaterY=v;
}

void VPBoundingBox::SetGreaterZ(double v) {
    greaterZ=v;
}

double VPBoundingBox::GetSmallerX() const {
    return smallerX;
}

double VPBoundingBox::GetSmallerY() const {
    return smallerY;
}

double VPBoundingBox::GetSmallerZ() const {
    return smallerZ;
}

void VPBoundingBox::SetSmallerX(double v) {
    smallerX=v;
}

void VPBoundingBox::SetSmallerY(double v) {
    smallerY=v;
}

void VPBoundingBox::SetSmallerZ(double v) {
    smallerZ=v;
}

void VPBoundingBox::SetBoundingBox(double minX, double minY, double minZ,
                                   double maxX, double maxY, double maxZ) {
    smallerX = minX;
    smallerY = minY;
    smallerZ = minZ;
    greaterX = maxX;
    greaterY = maxY;
    greaterZ = maxZ;
    ProcessCenter();
}

const VPPoint4D& VPBoundingBox::GetCenter() const {
    return center;
}

void VPBoundingBox::ConditionalUpdate(double x, double y, double z) {
    if (x < smallerX)
        smallerX = x;
    else {
        if (x > greaterX) greaterX = x;
    }
    if (y < smallerY)
        smallerY = y;
    else {
        if (y > greaterY) greaterY = y;
    }
    if (z < smallerZ)
        smallerZ = z;
    else {
        if (z > greaterZ) greaterZ = z;
    }
}

void VPBoundingBox::ConditionalUpdate(const VPPoint4D& point) {
    ConditionalUpdate(point.GetX(), point.GetY(), point.GetZ());
}

void VPBoundingBox::MergeWith(const VPBoundingBox& box) {
    ConditionalUpdate(box.smallerX, box.smallerY, box.smallerZ);
    ConditionalUpdate(box.greaterX, box.greaterY, box.greaterZ);
}

double VPBoundingBox::GetGreaterEdge() const {
    double greater = greaterX - smallerX;
    double tmp = greaterY - smallerY;

    if (tmp > greater)
        greater = tmp;
    tmp = greaterZ - smallerZ;
    if (tmp > greater)
        greater = tmp;
    return greater;
}

double VPBoundingBox::GetSmallerEdge() const {
    double smaller = greaterX - smallerX;
    double tmp = greaterY - smallerY;

    if (tmp < smaller)
        smaller = tmp;
    tmp = greaterZ - smallerZ;
    if (tmp < smaller)
        smaller = tmp;
    return smaller;
}

void VPBoundingBox::ToggleVisibility() {
    visible = !visible;
}

// Should be called if the bounding box is visible
bool VPBoundingBox::DrawInstanceOGL() const {
#ifdef VP_OGL
    static float fVec[4];

    glDisable(GL_LIGHTING);
    color.Get(fVec);
    glColor4fv(fVec);
    glBegin (GL_LINE_LOOP);
        glVertex3d (smallerX, greaterY, smallerZ);
        glVertex3d (greaterX, greaterY, smallerZ);
        glVertex3d (greaterX, smallerY, smallerZ);
        glVertex3d (smallerX, smallerY, smallerZ);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d (smallerX, smallerY, smallerZ);
        glVertex3d (smallerX, smallerY, greaterZ);
        glVertex3d (smallerX, greaterY, greaterZ);
        glVertex3d (smallerX, greaterY, smallerZ);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d (greaterX, greaterY, greaterZ);
        glVertex3d (smallerX, greaterY, greaterZ);
        glVertex3d (smallerX, smallerY, greaterZ);
        glVertex3d (greaterX, smallerY, greaterZ);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d (greaterX, smallerY, smallerZ);
        glVertex3d (greaterX, greaterY, smallerZ);
        glVertex3d (greaterX, greaterY, greaterZ);
        glVertex3d (greaterX, smallerY, greaterZ);
    glEnd();
    glEnable(GL_LIGHTING);
    return true;
#else
    return false;
#endif
}

bool VPBoundingBox::testAABBAABB(VPBoundingBox &b)
{
    if (b.greaterX < smallerX)
        return false;
    if (b.smallerX > greaterX)
        return false;
    if (b.greaterZ < smallerZ)
        return false;
    if (b.smallerZ > greaterZ)
        return false;
    if (b.greaterY < smallerY)
        return false;
    if (b.smallerY > greaterY)
        return false;
    return true;
}
ostream& operator<<(ostream& output, const VPBoundingBox& box)
{
    output << "(Greaters: " << box.greaterX << "," << box.greaterY << "," 
           << box.greaterZ << " Smallers: " << box.smallerX << "," 
           << box.smallerY << "," << box.smallerZ << ")";
    return output;
}
