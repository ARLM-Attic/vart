/// \file vpcolor.h
/// \brief Header file for VPColor VPAT class.
/// \version $Revision: 1.5 $

// ChangeLog is at the implementation file

#ifndef __VPCOLOR_H
#define __VPCOLOR_H

#include <iostream>

/// \class VPColor vpcolor.h
/// \brief RGBA color representation.

/// Colors are quadruplets of Red (R), Green (G), Blue (B) and Alpha (A) components
/// where A is the opacity of a color. Note that OpenGL requires that blending is
/// enabled in order to make actual use of the alpha value - this is NOT done automatically
/// by VPAT to produce faster drawings when blending is not needed. To enable blending
/// the programmer should do it in the application's main routine, with code like
/// "glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);".
/// Enabling blending may not be enough in some cases: OpenGL uses traditional
/// frame-buffer pixel blending which requires that farther fragments be drawn first in
/// order to achieve correct blending results. In other words, translucent objects may
/// appear opaque if drawn before the objects behind them.

class VPColor {
    friend std::ostream& operator<<(std::ostream& output, const VPColor& c);
    public:
        /// Creates a black, opaque color.
        VPColor();
        VPColor(const VPColor& color);
        VPColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
        void SetR(unsigned char r) { rgba[0] = r; }
        void SetG(unsigned char g) { rgba[1] = g; }
        void SetB(unsigned char b) { rgba[2] = b; }
        void SetA(unsigned char a) { rgba[3] = a; }
        void SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        unsigned char GetR() const { return rgba[0]; }
        unsigned char GetG() const { return rgba[1]; }
        unsigned char GetB() const { return rgba[2]; }
        unsigned char GetA() const { return rgba[3]; }
        void GetRGBA(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a) const;
        /// Returns RGBA componts as a vector of floats.
        void Get(float rgbaVec[4]) const;
        VPColor& operator=(const VPColor& color);
        VPColor operator+(const VPColor& color) const;
        VPColor operator-(const VPColor& color) const;
        bool operator==(const VPColor& color) const;
        bool operator!=(const VPColor& color) const;

        /// \brief Multiplies RGB components, keeps A component.

        /// Use this method to make a color darker, by multipling it by a number in
        /// the range 0..1.
        /// \param escalar [in] Number to multiply against RGB components. Should be
        /// in the range 0..1 in order to garantee that the result is in the range
        /// 0..1. No checking on the range is done.
        /// \param result [out] Result of multiplication as RGBA float vector.
        /// Components should be in the range 0..1, but no clamping is done.
        void GetScaled(float escalar, float result[4]) const;

        /// \brief Multiplies RGB components, keeps A component.

        /// Use this method to make a color darker, by multipling it by a number in
        /// the range 0..1.
        /// \param escalar [in] Number to multiply against RGB components. Should be
        /// in the range 0..1 in order to garantee that the result is in the range
        /// 0..1. No checking on the range is done.
        /// \param result [out] Result of multiplication as VPColor.
        /// Components should be in the range 0..1, but no clamping is done.
        void GetScaled(float escalar, VPColor* result) const;
    // PUBLIC STATIC METHODS
        /// Black opaque color
        static const VPColor& BLACK();
        /// Red opaque color
        static const VPColor& RED();
        /// Green opaque color
        static const VPColor& GREEN();
        /// Blue opaque color
        static const VPColor& BLUE();
        /// Magenta opaque color
        static const VPColor& MAGENTA();
        /// Cyan opaque color
        static const VPColor& CYAN();
        /// Yellow opaque color
        static const VPColor& YELLOW();
        /// White opaque color
        static const VPColor& WHITE();

        /// \brief Returns a randomly chosen color.

        /// You may want to initialize the random number generator with something like:
        /// srand(static_cast<unsigned int>(time(NULL))) before generating random colors.
        static VPColor RANDOM();
    private:
    // PRIVATE ATTRIBUTES
        /// Internal representation.
        unsigned char rgba[4];
        // Note: OpenGL's glLight... and glMaterial... only have int/float versions.
    // PRIVATE STATIC METHODS
};

#endif // _VPCOLOR_H
