/// \file vpboundingbox.h
/// \brief Header file for VPBoundingBox VPAT class.
/// \version $Revision: 1.11 $

// ChangeLog is at the implementation file

#ifndef __VPBOUNDINGBOX_H
#define __VPBOUNDINGBOX_H

#include "vppoint4d.h"
#include "vpcolor.h"
#include <GL/glut.h>
#include <iostream>

class VPTransform;

/// \class VPBoundingBox vpboundingbox.h
/// \brief Axis aligned bounding box.
///
/// FixMe: A detailed class description is needed here.
class VPBoundingBox {
    /// Output operator
    // FixMe: Perhaps this will cause trouble on an application with no console...
    friend std::ostream& operator<<(std::ostream& output, const VPBoundingBox& box);
    public:
        /// Creates an uninitialized bounding box.
        VPBoundingBox();
        VPBoundingBox(double minX, double minY, double minZ,
                      double maxX, double maxY, double maxZ);
        VPBoundingBox(const VPBoundingBox& box);

        VPBoundingBox& operator=(const VPBoundingBox& box);
        void ProcessCenter();

        /// \brief Applies transformation to a BoundingBox.
        /// \param trans [in] transformation matrix to be applied in bbox
        void Transform(const VPTransform& trans);

        double GetGreaterX() const;
        double GetGreaterY() const;
        double GetGreaterZ() const;

        /// \brief Returns the length of the greater edge.
        double GetGreaterEdge() const;
        /// \brief Returns the length of the smaller edge.
        double GetSmallerEdge() const;
        /// \brief Returns the length of the X edge.
        double GetEdgeX() const    { return greaterX-smallerX; }
        /// \brief Returns the length of the Y edge.
        double GetEdgeY() const    { return greaterY-smallerY; }
        /// \brief Returns the length of the Z edge.
        double GetEdgeZ() const    { return greaterZ-smallerZ; }

        void SetGreaterX(double v);
        void SetGreaterY(double v);
        void SetGreaterZ(double v);

        double GetSmallerX() const;
        double GetSmallerY() const;
        double GetSmallerZ() const;

        void SetSmallerX(double v);
        void SetSmallerY(double v);
        void SetSmallerZ(double v);

        void SetBoundingBox(double minX, double minY, double minZ,
                            double maxX, double maxY, double maxZ);

        const VPPoint4D& GetCenter() const;

        /// Updates the bounding box if any given coordinate is outside it.
        void ConditionalUpdate(double x, double y, double z);
        /// Updates the bounding box if any given coordinate is outside it.
        void ConditionalUpdate(const VPPoint4D& point);

        /// Merges a bounding with another, expanding it.
        void MergeWith(const VPBoundingBox& box);
        /// Draws a bounding box.
        bool DrawInstanceOGL() const;
        /// \brief Sets the bounding box visibility.
        void SetVisibility(const bool value)    { visible = value; };
        /// Toggles the visibility.
        void ToggleVisibility();
        /// Test intersection among AABBs
        bool testAABBAABB(VPBoundingBox &b);
        /// Indicates wether the bounding box is visible.
        bool visible;
        /// \brief Sets the bounding box color.
        void SetColor(const VPColor& value)     { color = value; }
        /// Copies geometry data from other bounding box.
        void CopyGeometryFrom(const VPBoundingBox& box);
    private:
        double greaterX, greaterY, greaterZ;
        double smallerX, smallerY, smallerZ;
        VPPoint4D center;
        VPColor color;
};

#endif  // __VPBOUNDINGBOX_H
