/// \file vpgraphicobj.h
/// \brief Header file for VPGraphicObj VPAT class.
/// \version $Revision: 1.8 $

// ChangeLog is at the implementation file

#ifndef __VPGRAPHICOBJ_H
#define __VPGRAPHICOBJ_H

#include "vpscenenode.h"
#include "vpboundingbox.h"

/// \class VPGraphicObj vpgraphicobj.h
/// \brief An scene node that is associated with a shape
///
/// Abstract class for graphical objects, that is, objects that have shapes to be
/// drawn. A graphical object may, or may not, be visible without affecting scene
/// graph traversal.
class VPGraphicObj : public VPSceneNode {
    public:
    // PUBLIC TYPES
        enum ShowType { LINES, LINES_AND_NORMALS, POINTS, POINTS_AND_NORMALS, FILLED };

    // PUBLIC METHODS
        VPGraphicObj();
        /// Makes the object visible.
        void Show();
        /// Makes the object invisible.
        void Hide();
        /// Toggles the object's visibility.
        void ToggleVisibility();
        /// Checks whether the object is visible.
        bool IsVisible();

        /// Computes the bounding box.
        virtual void ComputeBoundingBox() = 0;

        /// \brief Computes the recursive bounding box.
        ///
        /// This method requires a correct bounding box, therefore it should usually
        /// be called after ComputeBoundingBox.
        void ComputeRecursiveBoundingBox();

        /// Returns the bounding box.
        const VPBoundingBox& GetBoundingBox() const { return bBox; }

        /// Returns the recursive bounding box.
        const VPBoundingBox& GetRecursiveBoundingBox() const { return recBBox; }

        /// Toggles the bounding box visibility.
        void ToggleBBoxVisibility();

        /// Toggles the recursive bounding box visibility.
        void ToggleRecBBoxVisibility();

    // PUBLIC ATTRIBUTES
        /// \brief Defines how to show the object
        ShowType howToShow;

    protected:
        bool show;
        VPBoundingBox bBox;
        VPBoundingBox recBBox; // recursive bounding box
};

#endif
