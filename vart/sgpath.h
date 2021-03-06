/// \file sgpath.h
/// \brief Header file for V-ART class "SGPath".
/// \version $Revision: 1.2 $

#ifndef VART_SGPATH_H
#define VART_SGPATH_H

#include <list>
#include <iostream>

namespace VART {
    class Transform;
    class SceneNode;
    class Joint;
/// \class SGPath sgpath.h
/// \brief Scene Graph Path
///
/// A Scene Graph Path is a sequence of scene nodes (see SceneNode) in a scene graph.
    class SGPath {
        friend std::ostream& operator<<(std::ostream& output, const SGPath& path);
        public:
            SGPath();
            SGPath& operator=(const SGPath& path);
            /// Erases all elements.
            void Clear() { graphPath.clear(); }
            /// Adds a node to the path beginning
            void PushFront(SceneNode* nodePtr) { graphPath.push_front(nodePtr); }
            /// Combines and returns the multiplication of all transforms in a path.
            void GetTransform(Transform* resultPtr);
            /// Returns a pointer to the last joint in the path.
            Joint* PointerToLastJoint();
        protected:
            std::list<SceneNode*> graphPath;
    }; // end class declaration
} // end namespace

#endif  // VART_SGPATH_H
