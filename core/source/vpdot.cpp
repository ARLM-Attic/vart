/// \file vpdot.cpp
/// \brief Implementation file for VPDot VPAT class.
/// \version 1.1

// ChangeLog
// Dec 19, 2005 - Bruno de Oliveira Schneider 
// - File created.

#include "vpdot.h"
#ifdef VP_OGL
    #include <GL/gl.h>
#endif
//#include <iostream>

//using namespace std;

VPDot::VPDot()
{
    size = 3.0f;
    show = true;
}

VPDot::VPDot(const VPPoint4D& location)
{
    size = 3.0f;
    show = true;
    position = location;
}

void VPDot::ComputeBoundingBox()
{
    bBox.SetBoundingBox(position.GetX(), position.GetY(), position.GetZ(),
                        position.GetX(), position.GetY(), position.GetZ());
}

bool VPDot::DrawInstanceOGL() const
{
#ifdef VP_OGL
    static float fColor[4];
    color.Get(fColor);
    glPointSize(size); // FixMe: remove when size is turned into class attribute
    glDisable(GL_LIGHTING); // FixMe: check if lighting is enabled
    glBegin(GL_POINTS);
        glColor4fv(fColor);
        glVertex4dv(position.VetXYZW());
    glEnd();
    glEnable(GL_LIGHTING);
    return true;
#else
    return false;
#endif

}
