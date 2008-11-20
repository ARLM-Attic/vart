/// \file vplight.h
/// \brief Header file for VPLight VPAT class.
/// \version $Revision: 1.6 $

// ChangeLog is at the implementation file

#ifndef __VPLIGHT_H
#define __VPLIGHT_H

#include "vppoint3d.h"
#include "vppoint4d.h"
#include "vpvector3d.h"
#include "vpcolor.h"
#include "vpscenenode.h"
#include <string>

class VPBoundingBox;

/// \class VPLight vplight.h
/// \brief Represents a light source.
///
/// This class represents simple light sources, i.e.: directional lights or point
/// lights with no attenuation. See the derived classes for more complex light sources.
/// Lights can have children, but they should not.
class VPLight : public VPSceneNode {
    public:
        VPLight();
        VPLight(const VPLight& light);
        VPLight(float i, float ai, VPColor c, bool o);
        VPLight(const std::string& newDescription, float newIntensity,
                float newAmbientIntensity, const VPColor& newColor,
                const VPPoint4D& newLocation);
        virtual ~VPLight();
        VPLight& operator=(const VPLight& light);
        void SetIntensity(float i);
        float GetIntensity() const;
        void SetAmbientIntensity(float ai);
        float GetAmbientIntensity() const;
        void SetColor(const VPColor& c);
        VPColor GetColor() const;
        /// Turns a light on or off.
        void Turn(bool on_off);
        bool IsOn() const;
        
        /// \brief Always returns false, therefore recursive bbox does not exist.
        virtual bool RecursiveBoundingBox(VPBoundingBox* bBox);

        /// \brief Sets the location of the light.

        /// newLocation should be a point for positional lights or a direction for
        /// directional lights.
        void SetLocation(const VPPoint4D& newLocation);

        VPPoint4D GetLocation() const { return location; }
        /// \brief Draws (sets up) a light using OpenGL commands.
        /// \return false if VPAT was not compiled with OpenGL support.
        /// \param oglLightID [in] OpenGL ID for light in the range 0..7.
        bool DrawOGL(unsigned int oglLightID) const;
    // CLASS CONSTANT INITIALIZERS
        /// White directional light towards negative Y. Small ambient intensity.
        static const VPLight& SUN();
        /// Strong, white ambient light.
        static const VPLight& BRIGHT_AMBIENT();
    private:
        float intensity;
        float ambientIntensity;
        VPColor color;
        bool on;
        VPPoint4D location; //unified representation of location/direction.
};

#endif  // __VPLIGHT_H
