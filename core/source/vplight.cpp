/// \file vplight.cpp
/// \brief Implementation file for VPLight VPAT class.
/// \version 1.1.1.4

// ChangeLog
// May 08, 2006 - Dalton Reis
// - Added a RecursiveBoundingBox method
// Jan 10, 2005 - Bruno de Oliveira Schneider
// - Fixed DoxyGen class description.
// - Removed a few "std::" wich were not needed.
// - Changed attributes of the SUN to make it brighter.
// Oct 13, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Moved "using namespace std" from header file to implementation file.
// - Removed "vp" prefix from every method name.
// - Added "location" attribute. THIS MAKES THE VPDirectionalLight DECREPTED!
// - Added the methods: SetLocation, GetLocation.
// - Added "(const std::string&, float, float, const VPColor&, const VPPoint4D&)"
//   constructor, "SUN()" and "BRIGHT_AMBIENT()".
// - Added "const" modifier wherever possible.
// - Added a copy constructor and "operator=" because light is not static.
// - Added DrawOGL method.
// - Lights are now derived from VPSceneNode.
// - Removed constant symbols "ON" and "OFF".
// Jun 28, 2000 - Anderson Maciel
// - Classes declaration and methods addition.
// Jun 08, 2000 - Luciana Porcher Nedel
// - Classes declaration.
// Jun 19, 2000 - Rodrigo Berggevist Martins
// - Classes declaration.
// - Constructors and some methods implementation.
// May 30, 2000 - Isabel Harb Manssour
// - Constructors and some methods implementation.
// May 22, 2000 - Isabel Harb Manssour
// - Classes declaration.

#include "vplight.h"

#ifdef VP_OGL
#include <GL/gl.h>
#endif

using namespace std;

VPLight::VPLight() {
    // FixMe: As with most VPAT default constructors, an unitilized object could be
    // created here.
    color.SetRGBA(255,255,255,255);
    intensity = 1;
    ambientIntensity = 1;
    on = true;
}

VPLight::VPLight (float i, float ai, VPColor c, bool o) {
    intensity=i;
    ambientIntensity=ai;
    color=c;
    on=o;
}

VPLight::VPLight(const string& newDescription, float newIntensity,
                 float newAmbientIntensity, const VPColor& newColor,
                 const VPPoint4D& newLocation)
{
    description = newDescription;
    intensity = newIntensity;
    ambientIntensity = newAmbientIntensity;
    color = newColor;
    location = newLocation;
    on = true;
}

VPLight::VPLight(const VPLight& light) {
    description = light.description;
    intensity = light.intensity;
    ambientIntensity = light.ambientIntensity;
    color = light.color;
    location = light.location;
    on = light.on;
}

VPLight::~VPLight() {
}

VPLight& VPLight::operator=(const VPLight& light) {
    description = light.description;
    intensity = light.intensity;
    ambientIntensity = light.ambientIntensity;
    color = light.color;
    location = light.location;
    on = light.on;
    return (*this);
}

const VPLight& VPLight::SUN() {
    static const VPLight sun("TheSun", 0.7, 0.7, VPColor::WHITE(), VPPoint4D(0,-1,0,0));
    return sun;
}
const VPLight& VPLight::BRIGHT_AMBIENT() {
    static const VPLight ba("BrightAmbient", 0.3, 0.7, VPColor::WHITE(), VPPoint4D::ORIGIN());
    return ba;
}

void VPLight::SetIntensity(float i) {
    intensity = i;
}

float VPLight::GetIntensity() const {
    return(intensity);
}

void VPLight::SetAmbientIntensity(float ai){
    ambientIntensity = ai;
}

float VPLight::GetAmbientIntensity() const {
    return(ambientIntensity);
}

void VPLight::SetColor(const VPColor& c){
    color = c;
}

VPColor VPLight::GetColor() const {
    return(color);
}

void VPLight::Turn(bool on_off){
    on=on_off;
}

bool VPLight::IsOn() const {
    return on;
}

void VPLight::SetLocation(const VPPoint4D& newLocation){
    location = newLocation;
}

bool VPLight::DrawOGL(unsigned int oglLightID) const {
// The ID is needed because OpenGL calls require it.
#ifdef VP_OGL
    if (on) { // Is light enabled?
        float weightedColor[4];
        GLenum realID;

        switch (oglLightID) {
            case 0:
                realID = GL_LIGHT0;
                break;
            case 1:
                realID = GL_LIGHT1;
                break;
            case 2:
                realID = GL_LIGHT2;
                break;
            case 3:
                realID = GL_LIGHT3;
                break;
            case 4:
                realID = GL_LIGHT4;
                break;
            case 5:
                realID = GL_LIGHT5;
                break;
            case 6:
                realID = GL_LIGHT6;
                break;
            default:
                realID = GL_LIGHT7;
                break;
        }
        glEnable(realID);
        color.GetScaled(ambientIntensity, weightedColor);
        glLightfv(realID, GL_AMBIENT, weightedColor);
        color.GetScaled(intensity, weightedColor);
        glLightfv(realID, GL_DIFFUSE, weightedColor);
    }
    // FixMe: if light is turned off, it seems that glDisable should be called.
    return true;
#else
    return false;
#endif
}

bool VPLight::RecursiveBoundingBox(VPBoundingBox* bBox) {
// virtual method
    return false;
}
