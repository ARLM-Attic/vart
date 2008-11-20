/// \file vpcylinder.cpp
/// \brief Implementation file for VPCylinder VPAT class.
/// \version $Revision: 1.10 $

// ChangeLog
// May 17, 2006 - Bruno de Oliveira Schneider
// - ComputeRecursiveBoundingBox is no longer called automatically.
// - DrawInstanceOGL now checks if it should draw the recursive bounding box.
// May 08, 2006 - Dalton Reis
// - Use of the ComputeRecursiveBoundingBox method instead of attributing bbox current.
// Mar 16, 2006 - Bruno de Oliveira Schneider
// - Changed the way how visibility of cylinder parts are handled. Added SetPartsVisibility,
//   TogglePartsVisibility and GetPartsVisibility. Made ShowSide, ShowTop, ShowBottom deprecated.
//   Renamed side, top and bottom to sidesVisible, topVosible and bottomVisible.
// Feb 16, 2006 - Andreia Schneider
// - DrawInstanceOGL modified to allow the bounding box to be drawn.
// Jan 24, 2006 - Bruno de Oliveira Schneider
// - DrawInstanceOGL now respects show and howToShow attributes.
// - Splitted radius attribute into topRadius and btRadius so that this class may be used
//   to create cones.
// - The VPCylinder(float fHi, float fRad) constructor now sets all parts to visible.
// - The default constructor now creates uninitialized cylinders.
// - Fixed ComputeBoundingBox.
// - Added bounding computation to methods that change geometry.
// Jun 01, 2005 - Andreia Schneider
// - Fixed bottom orientation.
// Mar 21, 2005 - Bruno de Oliveira Schneider
// - Added ComputeBoundingBox.
// Oct 13, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Removed "vp" prefix from every method name.
// - Removed (empty) vpRender method.
// - Added DrawInstanceOGL method.
// - Added code for initializing transform matrices in constructors.
// - Added "material" attribute and related Get/Set methods.
// Aug 10, 2000 - Anderson Maciel
// - Classes declaration.
// Jun 28, 2000 - Anderson Maciel
// - Methods atualization.

#include "vpcylinder.h"
#ifdef VP_OGL
#include <GL/glu.h>
#endif

//?
#include <iostream>
using namespace std;


VPCylinder::VPCylinder() : VPGraphicObj()
{
}

VPCylinder::VPCylinder(float fHi, float fRad) : VPGraphicObj()
{
    height = fHi;
    topRadius = btRadius = fRad;
    sidesVisible = topVisible = bottomVisible = true;
    bBox.SetBoundingBox(-fRad, -fRad, 0, fRad, fRad, fHi);
    recBBox = bBox;
}

VPCylinder::VPCylinder( float fHi, float fRad, bool bS, bool bT, bool bB )
           : VPGraphicObj()
{
    height = fHi;
    topRadius = btRadius = fRad;
    sidesVisible = bS;
    topVisible = bT;
    bottomVisible = bB;
    bBox.SetBoundingBox(-fRad, -fRad, 0, fRad, fRad, fHi);
    recBBox = bBox;
}

void VPCylinder::ComputeBoundingBox()
{
    float maxRadius = (topRadius > btRadius)? topRadius : btRadius;
    bBox.SetBoundingBox(-maxRadius, -maxRadius, 0, maxRadius, maxRadius, height);
}

void VPCylinder::SetHeight(float h)
{
    float maxRadius = (topRadius > btRadius)? topRadius : btRadius;
    height = h;
    bBox.SetBoundingBox(-maxRadius, -maxRadius, 0, maxRadius, maxRadius, h);
    ComputeRecursiveBoundingBox();
}

void VPCylinder::SetRadius(float r)
{
    topRadius = btRadius = r;
    bBox.SetBoundingBox(-r, -r, 0, r, r, height);
    ComputeRecursiveBoundingBox();
}

void VPCylinder::SetPartsVisibility(PartsID parts)
{
    bottomVisible = static_cast<bool>(parts & BOTTOM);
    sidesVisible = static_cast<bool>(parts & SIDES);
    topVisible = static_cast<bool>(parts & TOP);
}

void VPCylinder::TogglePartsVisibilty(PartsID parts)
{
    if (parts & BOTTOM) bottomVisible = !bottomVisible;
    if (parts & SIDES) sidesVisible = !sidesVisible;
    if (parts & TOP) topVisible = !topVisible;
}

VPCylinder::PartsID VPCylinder::GetPartsVisibility()
{
    PartsID result = NONE;
    if (bottomVisible) result = result & BOTTOM;
    if (sidesVisible) result = result & SIDES;
    if (topVisible) result = result & TOP;
    return result;
}

void VPCylinder::ShowSide(bool yesno)
{
    sidesVisible = yesno;
    cerr << "Warning: VPCylinder::ShowSide is deprecated." << endl;
}

void VPCylinder::ShowTop(bool yesno)
{
    topVisible = yesno;
    cerr << "Warning: VPCylinder::ShowTop is deprecated." << endl;
}

void VPCylinder::ShowBottom(bool yesno)
{
    bottomVisible = yesno;
    cerr << "Warning: VPCylinder::ShowBottom is deprecated." << endl;
}

float VPCylinder::GetHeight(void)
{
    return height;
}

float VPCylinder::GetTopRadius(void)
{
    return topRadius;
}

float VPCylinder::GetBottomRadius(void)
{
    return btRadius;
}

bool VPCylinder::ShowSide(void)
{
    cerr << "Warning: VPCylinder::ShowSide is deprecated." << endl;
    return sidesVisible;
}

bool VPCylinder::ShowTop(void)
{
    cerr << "Warning: VPCylinder::ShowTop is deprecated." << endl;
    return topVisible;
}

bool VPCylinder::ShowBottom(void)
{
    cerr << "Warning: VPCylinder::ShowBottom is deprecated." << endl;
    return bottomVisible;
}

bool VPCylinder::DrawInstanceOGL() const 
{
#ifdef VP_OGL
    GLUquadricObj* qObj = gluNewQuadric();
    bool result = true;

    if (show)
    {
        switch (howToShow)
        {
            case LINES:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case POINTS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                break;
            default:
                glPolygonMode(GL_FRONT, GL_FILL);
                break;
        }
        result = material.DrawOGL();
        // Render sides
        if (sidesVisible) {
            gluQuadricDrawStyle(qObj, GLU_FILL);
            gluQuadricNormals(qObj, GLU_SMOOTH);
            gluCylinder(qObj, btRadius, topRadius, height, 15, 1);
        }
        // Render bottom
        if (bottomVisible) {
            gluQuadricOrientation(qObj,GLU_INSIDE);
            gluDisk(qObj, 0.0, btRadius, 15, 1);
        }
        // Render top
        if (topVisible) {
            glPushMatrix();
            gluQuadricOrientation(qObj,GLU_OUTSIDE);
            glTranslatef(0.0f, 0.0f, height);
            gluDisk(qObj, 0.0, topRadius, 15, 1);
            glPopMatrix();
        }
        gluDeleteQuadric(qObj);
        if (bBox.visible) // Is the bounding box visible?
            bBox.DrawInstanceOGL();
        if (recBBox.visible)
            recBBox.DrawInstanceOGL();
    }
    return result;
#else
    return false;
#endif
}
