/// \file vpdot.h
/// \brief Header file for VPDot VPAT class.
/// \version 1.1

// ChangeLog is at the implementation file.

#ifndef __VPDOT_H
#define __VPDOT_H

#include "vpgraphicobj.h"
#include "vpcolor.h"
#include "vppoint4d.h"

/// \class VPDot vpdot.h
/// \brief A graphical point.
///
/// A dot is a point that can rendered on screen as part of a scene graph.
class VPDot : public VPGraphicObj
{
    public:
    // PUBLIC METHODS
        /// \brief Creates an initialized point.
        VPDot();

        /// \brief Creates a point at a specific location.
        VPDot(const VPPoint4D& location);

        /// \brief Computes the bounding box.
        virtual void ComputeBoundingBox();

    // PUBLIC ATTRIBUTES
        /// \brief The point's position.
        VPPoint4D position;

        /// \brief The size (in pixels) of the point on screen.
        float size; // FixMe: better turn it into a class attribute

        /// \brief The point's color.
        VPColor color;

    protected:
        /// Draws the point.
        virtual bool DrawInstanceOGL() const;

};

#endif
