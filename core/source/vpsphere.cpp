/// \file vpsphere.cpp
/// \brief Implementation file for VPSphere VPAT class.
/// \version $Revision: 1.7 $

// ChangeLog
// May 17, 2006 - Bruno de Oliveira Schneider
// - Removed automatic activation of ComputeRecursiveBoundingBox() everywhere.
// - Removed reference to bBoxShow.
// May 08, 2006 - Dalton Reis
// - Use of the ComputeRecursiveBoundingBox method instead of attributing bbox current.
// - It verifies if bounding box is visible. 
// Jun 16, 2005 - Andreia Schneider
// - Bounding box computation is now automatic
// Dec 07, 2004 - Bruno de Oliveira Schneider
// - Added ComputeBoundingBox().
// Sep 30, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started. Indentation fixed.
// - Removed "vp" prefix from every method name.
// - Removed (empty) vpRender method.
// - Added DrawInstanceOGL method.
// - Added code for initializing transform matrices in constructors.
// - Added "material" attribute and related Get/Set methods.
// Oct 10, 2000 - Anderson Maciel
// - Methods atualization.
// Aug 10, 2000 - Anderson Maciel
// - Classes declaration.

#ifdef VP_OGL
#include <GL/glu.h>
#endif
#include "vpsphere.h"
#include <iostream>

using namespace std;

VPSphere::VPSphere() : VPGraphicObj() {
    radius = 1.0;
    ComputeBoundingBox();
}

VPSphere::VPSphere( float fRad ) : VPGraphicObj() {
    radius = fRad;
    ComputeBoundingBox();
}

void VPSphere::SetRadius( float r ) {
    radius = r;
    ComputeBoundingBox();
}

float VPSphere::GetRadius( void ) {
    return radius;
}

void VPSphere::ComputeBoundingBox() {
    bBox.SetSmallerX(-radius);
    bBox.SetSmallerY(-radius);
    bBox.SetSmallerZ(-radius);
    bBox.SetGreaterX(radius);
    bBox.SetGreaterY(radius);
    bBox.SetGreaterZ(radius);
    //oobBox=VPOOBoundingBox(bBox);
}

bool VPSphere::DrawInstanceOGL() const {
#ifdef VP_OGL
    GLUquadricObj* qObj = gluNewQuadric();
    bool result = material.DrawOGL();

    gluQuadricDrawStyle(qObj, GLU_FILL);
    gluQuadricNormals(qObj, GLU_SMOOTH);
    gluSphere(qObj, radius, 15, 15);
    gluDeleteQuadric(qObj);
    if (bBox.visible)
        bBox.DrawInstanceOGL();
    return result;
#else
    return false;
#endif
}
