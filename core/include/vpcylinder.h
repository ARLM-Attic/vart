/// \file vpcylinder.h
/// \brief Header file for VPCylinder VPAT class.
/// \version $Revision: 1.8 $

// ChangeLog is at the implementation file

#ifndef __VPCYLINDER_H
#define __VPCYLINDER_H

#include "vpgraphicobj.h"
#include "vpmaterial.h"

/// \class VPCylinder vpcylinder.h
/// \brief A cylinder or cone.

/// Cylinders are created along the z axis, with bottom at zero and top at height.
/// Cones are considered a special case of cylinder, where the top or bottom has zero radius.
// It is up to the Draw methods to make the above statement true.
class VPCylinder : public VPGraphicObj {
    public:
        /// \brief Bitmask for cylinder parts.
        ///
        /// The bit meanings from right (lower) to left are: bottom disc, sides and top disc.
        /// IDs may combied using operators '&' and '|'.
        enum PartsID { NONE, BOTTOM, SIDES, NO_TOP, TOP, NO_SIDES, NO_BOTTOM, ALL };

        /// \brief Creates an uninitialized cylinder.
        VPCylinder();
        VPCylinder( float fHi, float fRad );
        VPCylinder( float fHi, float fRad, bool bS, bool bT, bool bB );

        /// Computes the bounding box.
        virtual void ComputeBoundingBox();

        void SetHeight(float h);
        /// Sets the value of top and bottom radius
        void SetRadius( float r );
        /// Sets the value of the top radius
        void SetTopRadius(float r) { topRadius = r; }
        /// Sets the value of the bottom radius
        void SetBottomRadius(float r){ btRadius = r; }

        /// Material assignment.
        void SetMaterial(const VPMaterial& mat) { material = mat; }

        /// \brief Sets which parts are visible.
        ///
        /// All three bits present in the bitmask are copied to internal visibility state.
        void SetPartsVisibility(PartsID parts);

        /// \brief Toogle visibility of marked parts.
        ///
        /// Toggle visibility of all parts whose bits are set in given bitmask.
        void TogglePartsVisibilty(PartsID parts);

        /// \brief Returns internal visibility state as PartsID.
        PartsID GetPartsVisibility();

        /// \deprecated Use SetPartsVisibility or TogglePartsVisibilty instead.
        void    ShowSide( bool yesno );
        /// \deprecated Use SetPartsVisibility or TogglePartsVisibilty instead.
        void    ShowTop( bool yesno );
        /// \deprecated Use SetPartsVisibility or TogglePartsVisibilty instead.
        void    ShowBottom( bool yesno );
        float   GetHeight();
        /// Returns the top radius
        float GetTopRadius();
        /// Returns the bottom radius
        float GetBottomRadius();
        /// Returns of copy of the cylinder's material.
        VPMaterial GetMaterial() const { return material; }
        /// \deprecated Use GetPartsVisibility instead.
        bool    ShowSide();
        /// \deprecated Use GetPartsVisibility instead.
        bool    ShowTop();
        /// \deprecated Use GetPartsVisibility instead.
        bool    ShowBottom();
    private:
    // PRIVATE METHODS
        /// \brief Draws the cylinder using OpenGL engine.
        /// \return false if VPAT has not been compiled with OpenGL support.
        bool DrawInstanceOGL() const;
    // PRIVATE ATTRIBUTES
        float height;
        float topRadius;
        float btRadius;
        bool sidesVisible;
        bool topVisible;
        bool bottomVisible;
        VPMaterial material;
};

// OPERATORS FOR PartsID
inline VPCylinder::PartsID operator &(VPCylinder::PartsID a, VPCylinder::PartsID b) {
    return VPCylinder::PartsID(static_cast<int>(a) & static_cast<int>(b));
}

inline VPCylinder::PartsID operator |(VPCylinder::PartsID a, VPCylinder::PartsID b) {
    return VPCylinder::PartsID(static_cast<int>(a) | static_cast<int>(b));
}

#endif
