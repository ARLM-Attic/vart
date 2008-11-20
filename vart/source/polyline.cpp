/// \file PolyLine.cpp
/// \brief Implementation file for VART class "PolyLine".
/// \version $Revision: 1.2 $

#include "vart/polyline.h"

using namespace std;
using namespace VART;

VART::PolyLine::PolyLine(void)
: width( 1 )
{
}

VART::PolyLine::PolyLine(const VART::PolyLine& polyLine)
{
    this->operator =(polyLine);
}

VART::PolyLine::~PolyLine(void)
{
    Clear();
}

VART::PolyLine& VART::PolyLine::operator=(const VART::PolyLine& polyLine)
{
    width = polyLine.width;
    vertexVec = polyLine.vertexVec;
    material = polyLine.material;
    return *this;
}

VART::SceneNode* VART::PolyLine::Copy()
{
    return new VART::PolyLine(*this);
}

void VART::PolyLine::Clear()
{
    vertexVec.clear();
}

void VART::PolyLine::ComputeBoundingBox() 
{
    if (vertexVec.size() > 1)
    {
        bBox.SetBoundingBox(vertexVec[0].GetX(), vertexVec[0].GetY(), vertexVec[0].GetZ(),
                            vertexVec[1].GetX(), vertexVec[1].GetY(), vertexVec[1].GetZ());
         // Check against the others
        for (unsigned int i=2; i < vertexVec.size(); i++)
             bBox.ConditionalUpdate(vertexVec[i].GetX(), vertexVec[i].GetY(), vertexVec[i].GetZ());
    }
}

bool VART::PolyLine::DrawOGL() const
{
#ifdef VART_OGL
    bool result = true;
    if (show)
    {
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( width );
        result = material.DrawOGL();
        glBegin(GL_LINES);
        for(unsigned int i = 0; i < vertexVec.size(); ++i)
        {
            glVertex3dv( vertexVec[i].VetXYZW() );
        }
        glEnd();
        glPopAttrib();
    }
    if (bBox.visible)
        bBox.DrawInstanceOGL();
    return result;
#else
    return false;
#endif
}
