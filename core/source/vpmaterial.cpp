/// \file vpmaterial.cpp
/// \brief Implementation file for VPMaterial VPAT class.
/// \version 1.2

// ChangeLog
// Dec 15, 2004 - Bruno de Oliveira Schneider 
// - Added PLASTIC_RED(), PLASTIC_GREEN(), PLASTIC_BLUE(), LIGHT_PLASTIC_GRAY()
//   and DARK_PLASTIC_GRAY().
// - Added SetDiffuseColor, SetSpecularColor, SetShininess, GetDiffuseColor, 
//   SetAmbientColor and SetEmissiveColor.
// Sep 24, 2004 - Bruno de Oliveira Schneider 
// - File created.

#include "vpmaterial.h"

#ifdef VP_OGL
#include <GL/gl.h>
#endif

//#include <iostream>
//using namespace std;

VPMaterial::VPMaterial()
{
}

VPMaterial::VPMaterial(const VPMaterial& m)
{
    color = m.color;
    emissive = m.emissive;
    ambient = m.ambient;
    specular = m.specular;
    shininess = m.shininess;
}

VPMaterial::VPMaterial(const VPColor& c)
{
    color = c;
    c.GetScaled(0.0001f, &emissive);
    c.GetScaled(0.3f, &ambient);
    c.GetScaled(0.1f, &specular);
    shininess = 0.01f;
}

VPMaterial::VPMaterial(const VPTexture& t)
{
}

VPMaterial::VPMaterial(const VPColor& c, float spc, float amb, float ems, float shi)
{
    color = c;
    c.GetScaled(ems, &emissive);
    c.GetScaled(amb, &ambient);
    c.GetScaled(spc, &specular);
    shininess = shi;
}

VPMaterial& VPMaterial::operator=(const VPMaterial& m)
{
    color = m.color;
    emissive = m.emissive;
    ambient = m.ambient;
    specular = m.specular;
    shininess = m.shininess;
    return *this;
}

const VPMaterial& VPMaterial::LIGHT_PLASTIC_GRAY()
{
    static const VPMaterial lpGray(VPColor(204,204,220));
    return lpGray;
}

const VPMaterial& VPMaterial::DARK_PLASTIC_GRAY()
{
    static const VPMaterial dpGray(VPColor(127,127,127));
    return dpGray;
}

const VPMaterial& VPMaterial::PLASTIC_RED()
{
    static const VPMaterial pRed(VPColor::RED());
    return pRed;
}

const VPMaterial& VPMaterial::PLASTIC_GREEN()
{
    static const VPMaterial pGreen(VPColor::GREEN());
    return pGreen;
}

const VPMaterial& VPMaterial::PLASTIC_BLUE()
{
    static const VPMaterial pBlue(VPColor::BLUE());
    return pBlue;
}

bool VPMaterial::DrawOGL() const
{
#ifdef VP_OGL
    float fVec[4];
    color.Get(fVec);
    glColor4fv(fVec);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, fVec);
    ambient.Get(fVec);
    glMaterialfv(GL_FRONT, GL_AMBIENT, fVec);
    specular.Get(fVec);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fVec);
    emissive.Get(fVec);
    glMaterialfv(GL_FRONT, GL_EMISSION, fVec);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    return true;
#else
    return false;
#endif
}
