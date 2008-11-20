/// \file vpmaterial.h
/// \brief Header file for VPMaterial VPAT class.
/// \version 1.2

// ChangeLog is at the implementation file.

#ifndef __VPMATERIAL_H
#define __VPMATERIAL_H

#include "vptexture.h"
#include "vpcolor.h"

/// \class VPMaterial vpmaterial.h
/// \brief Material properties for graphical objects.
///
/// FixMe: Put detailed description here.

// FixMe: Texture still needs to be implemented.

class VPMaterial
{
    public:
    // Public Methods
        VPMaterial();
        VPMaterial(const VPMaterial& m);
        /// Creates plastic-looking material of given color.
        VPMaterial(const VPColor& c);
        VPMaterial(const VPTexture& t);
        /// Sets all colors to be shades of one single color.
        VPMaterial(const VPColor& c, float spc, float amb, float ems, float shi);
        VPMaterial& operator=(const VPMaterial& m);
        /// Sets the diffuse color (main color) of the material.
        void SetDiffuseColor(const VPColor& c) { color = c; }
        /// Returns the diffuse color.
        const VPColor& GetDiffuseColor() const { return color; }
        /// Sets the specular color (highlight color) of the material.
        void SetSpecularColor(const VPColor& c) { specular = c; }
        /// Sets the ambient color of the material.
        void SetAmbientColor(const VPColor& c) { ambient = c; }
        /// Sets the emissive color of the material.
        void SetEmissiveColor(const VPColor& c) { emissive = c; }

        /// \brief Set the shininess of the material.

        /// According to the "OpenGL programming guide" (red book), shininess should be
        /// in the range of 0.0 (no shininess) to 128.0 (very shiny).
        void SetShininess(float newValue) { shininess = newValue; }

        /// \brief Draws the material using OpenGL engine.
        /// \return false if VPAT has not been compiled with OpenGL support.
        bool DrawOGL() const;
    // Public Static Methods
        static const VPMaterial& LIGHT_PLASTIC_GRAY();
        static const VPMaterial& DARK_PLASTIC_GRAY();
        static const VPMaterial& PLASTIC_RED();
        static const VPMaterial& PLASTIC_GREEN();
        static const VPMaterial& PLASTIC_BLUE();
private:
        /// color for diffuse reflection
        VPColor color;
        /// color for light emission
        VPColor emissive;
        /// color for ambient light reflection
        VPColor ambient;
        /// color for specular light reflection
        VPColor specular;
        VPTexture texture;
        /// shininess coeficient
        float shininess;
};

#endif
