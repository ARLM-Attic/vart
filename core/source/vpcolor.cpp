/// \file vpcolor.cpp
/// \brief Implementation file for VPColor VPAT class.
/// \version $Revision: 1.3 $

// ChangeLog
// Sep 22, 2005 - Bruno de Oliveira Schneider
// - Added MAGENTA, CYAN and YELLOW static methods.
// Dec 02, 2004 - Bruno de Oliveira Schneider
// - Added RANDOM().
// Sep 27, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Removed "vp" prefix from every method name.
// - Optimized code for operators == and != .
// - Added BLACK, RED, GREEN, BLUE and WHITE static methods.
// - Renamed "I" component to "A" (along with related methods).
// - Changed internal representation from "unsigned int" to "unsigned char".
// - "rgba" is now a public atribute. A few Get/Set methods were removed.
// - Added "const" modifier wherever possible.
// Jun 13, 2000 - Fernando Sola Pereira
// - Implementation of the class methods.
// - Attributes and methods declaration.

#include "vpcolor.h"
#include <cassert>
#include <cstdlib>

//?
#include <iostream>
using namespace std;

typedef unsigned char uchar;

VPColor::VPColor() {
    rgba[0] = 0;
    rgba[1] = 0;
    rgba[2] = 0;
    rgba[3] = 255;
}

VPColor::VPColor(uchar r, uchar g, uchar b, uchar a) {
// a defaults to 255 (see header)
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

VPColor::VPColor(const VPColor& color) {
    rgba[0] = color.rgba[0];
    rgba[1] = color.rgba[1];
    rgba[2] = color.rgba[2];
    rgba[3] = color.rgba[3];
}

const VPColor& VPColor::BLACK() {
    static const VPColor black(0,0,0,255);
    return black;
}

const VPColor& VPColor::RED() {
    static const VPColor red(255,0,0,255);
    return red;
}

const VPColor& VPColor::GREEN() {
    static const VPColor green(0,255,0,255);
    return green;
}

const VPColor& VPColor::BLUE() {
    static const VPColor blue(0,0,255,255);
    return blue;
}

const VPColor& VPColor::MAGENTA() {
    static const VPColor magenta(255,0,255,255);
    return magenta;
}

const VPColor& VPColor::CYAN() {
    static const VPColor cyan(0,255,255,255);
    return cyan;
}

const VPColor& VPColor::YELLOW() {
    static const VPColor yellow(255,255,0,255);
    return yellow;
}

const VPColor& VPColor::WHITE() {
    static const VPColor white(255,255,255,255);
    return white;
}

VPColor VPColor::RANDOM() {
    VPColor color;
    unsigned char byte;
    byte = static_cast<unsigned char>(rand() % 256);
    color.SetR(byte);
    byte = static_cast<unsigned char>(rand() % 256);
    color.SetG(byte);
    byte = static_cast<unsigned char>(rand() % 256);
    color.SetB(byte);
    color.SetA(255);
    return color;
}

void VPColor::SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

void VPColor::GetRGBA(unsigned char* r, unsigned char* g,
                      unsigned char* b, unsigned char* a) const {
    *r = rgba[0];
    *g = rgba[1];
    *b = rgba[2];
    *a = rgba[3];
}

void VPColor::Get(float rgbaVec[4]) const {
    rgbaVec[0] = rgba[0]/255.0f;
    rgbaVec[1] = rgba[1]/255.0f;
    rgbaVec[2] = rgba[2]/255.0f;
    rgbaVec[3] = rgba[3]/255.0f;
}
VPColor& VPColor::operator=(const VPColor& color) {
    rgba[0] = color.rgba[0];
    rgba[1] = color.rgba[1];
    rgba[2] = color.rgba[2];
    rgba[3] = color.rgba[3];
    return *this;
}

VPColor VPColor::operator+(const VPColor& color) const {
    VPColor c(rgba[0]+color.rgba[0], rgba[1]+color.rgba[1],
              rgba[2]+color.rgba[2], rgba[3]+color.rgba[3]);
    return c;
}

VPColor VPColor::operator-(const VPColor& color) const {
    VPColor c(rgba[0]-color.rgba[0], rgba[1]-color.rgba[1],
              rgba[2]-color.rgba[2], rgba[3]-color.rgba[3]);
    return c;
}

bool VPColor::operator==(const VPColor& color) const {
    return ( rgba[0] == color.rgba[0] &&
             rgba[1] == color.rgba[1] &&
             rgba[2] == color.rgba[2] &&
             rgba[3] == color.rgba[3] );
}

bool VPColor::operator!=(const VPColor& color) const {
    return ( rgba[0] != color.rgba[0] ||
             rgba[1] != color.rgba[1] ||
             rgba[2] != color.rgba[2] ||
             rgba[3] != color.rgba[3] );
}

void VPColor::GetScaled(float escalar, float result[4]) const {
    assert(escalar >= 0);
    assert(escalar <= 1);
    result[0] = rgba[0]/255.0f * escalar;
    result[1] = rgba[1]/255.0f * escalar;
    result[2] = rgba[2]/255.0f * escalar;
    result[3] = rgba[3]/255.0f;
}

void VPColor::GetScaled(float escalar, VPColor* result) const {
    assert(escalar >= 0);
    assert(escalar <= 1);
    result->rgba[0] = static_cast<unsigned char>(rgba[0] * escalar);
    result->rgba[1] = static_cast<unsigned char>(rgba[1] * escalar);
    result->rgba[2] = static_cast<unsigned char>(rgba[2] * escalar);
    result->rgba[3] = rgba[3];
}

// FixMe: Remove this later...
ostream& operator<<(ostream& output, const VPColor& c)
{
    output << "{" << static_cast<int>(c.rgba[0]) << " "
           << static_cast<int>(c.rgba[1]) << " " << static_cast<int>(c.rgba[2])
           << " " << static_cast<int>(c.rgba[3]) << "}";
    return output;
}
