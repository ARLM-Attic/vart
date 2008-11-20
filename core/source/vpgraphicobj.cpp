/// \file vpgraphicobj.cpp
/// \brief Implementation file for VPGraphicObj VPAT class.
/// \version $Revision: 1.9 $

// ChangeLog
// May 18, 2006 - Bruno de Oliveira Schneider
// - ComputeRecursiveBoundingBox now enters transforms.
// - Removed RecursiveBoundingBox. GetRecursiveBoundingBox should be used instead.
// - Added ToggleRecBBoxVisibility.
// - Removed bBoxShow because VPBoundingBox already has a visibility flag.
// May 08, 2006 - Dalton Reis
// - Added a RecursiveBoundingBox method
// Feb 17, 2006 - Andreia Schneider
// - Removed const of the VPSceneNode in method ComputeRecursiveBoundingBox.
// - Added bBoxShow (attribute that indicates if the bounding box is visible, or not).
// Jan 25, 2006 - Bruno de Oliveira Schneider
// - Added polygon display modes.
// Apr 27, 2005 - Bruno de Oliveira Schneider
// - Added ToggleVisibility() and IsVisible().
// Dec 03, 2004 - Bruno de Oliveira Schneider
// - Added bBox, recBBox and related methods.
// Oct 13, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Removed "using namespace std" from header file.
// - Removed "vp" prefix from every method name.
// - Removed IVPATH declaration.
// - VPGraphicObj is now derived from VPSceneNode. AS A RESULT, MOST METHODS AND
//   ATRIBUTES WERE REMOVED.
// May 04, 2004 - Bruno de Oliveira Schneider
// - Commented out IVPATH declaration.
// Jul 25, 2000 - Anderson Maciel
// - Classes declaration.

#include "vpgraphicobj.h"
#include "vptransform.h"
#include <cassert>
#include <list>

using namespace std;

VPGraphicObj::VPGraphicObj() {
    show = true;
    howToShow = FILLED;
}

void VPGraphicObj::Show() {
    show = true;
}

void VPGraphicObj::Hide() {
    show = false;
}

void VPGraphicObj::ToggleVisibility() {
    show = !show;
}

void VPGraphicObj::ToggleBBoxVisibility() {
    bBox.visible = !bBox.visible;
}

void VPGraphicObj::ToggleRecBBoxVisibility() {
    recBBox.visible = !recBBox.visible;
}

bool VPGraphicObj::IsVisible() {
    return show;
}

void VPGraphicObj::ComputeRecursiveBoundingBox() {
    VPBoundingBox box;
    list<VPSceneNode*>::const_iterator iter;
    VPGraphicObj* objPtr;
    VPTransform* transPtr;

    recBBox.CopyGeometryFrom(bBox); // start with its own bounding box

    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        objPtr = dynamic_cast<VPGraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ComputeRecursiveBoundingBox();
            recBBox.MergeWith(objPtr->GetRecursiveBoundingBox());
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VPTransform*>(*iter);
            if (transPtr) { // object is a transform
                if (transPtr->RecursiveBoundingBox(&box)) {
                    recBBox.MergeWith(box);
                }
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
    recBBox.ProcessCenter();
}
