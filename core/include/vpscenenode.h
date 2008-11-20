/// \file vpscenenode.h
/// \brief Header file for VPSceneNode VPAT class.
/// \version 1.8

// ChangeLog is at the implementation file.

#ifndef __VPSCENENODE_H
#define __VPSCENENODE_H

#include "vpmemoryobj.h"
#include <list>
#include <string>

/// \class VPSceneNode vpscenenode.h
/// \brief Base class for objects that compose a scene graph
///
/// A SceneNode is something directly related to the rendering engine such as shapes
/// (VPGraphicObj), transformations (VPTransform) and lights (VPLight). Several
/// nodes together create an environment (VPScene) that is draw every rendering
/// cicle. SceneNodes have childs to allow creating a hierarchy of objects. This class
/// should be considered abstract.
class VPSceneNode : public VPMemoryObj
{
    public:
    // PUBLIC TYPES
        enum TypeID { NONE, GRAPHIC_OBJ, BOX, CONE, CURVE, BEZIER,
                      CYLINDER, IMAGE, IMAGE_2D, VOLUME, MESH_OBJECT,
                      SPHERE, JOINT, BIAXIAL_JOINT, CONDYLAR_JOINT,
                      ELLIPSOID_JOINT, SADDLE_JOINT, PLANE_JOINT,
                      POLYAXIAL_JOINT, UNIAXIAL_JOINT, HINGE_JOINT,
                      PIVOT_JOINT, TRANSFORM };
    // PUBLIC METHODS
        /// Creates an uninitialized scene node
        VPSceneNode();
        VPSceneNode(VPSceneNode& node);
        virtual ~VPSceneNode();
        VPSceneNode& operator=(VPSceneNode& node);
        /// Returns a copy of the object's description
        const std::string& GetDescription() const { return description; }
        /// Changes the object's description
        void SetDescription(const std::string& desc) { description = desc; }
        /// Add a child at the end of child list
        void AddChild(VPSceneNode& child);
        /// \brief Recursive drawing using OpenGL commands
        /// \return false if VPAT is was not compiled with OpenGL support
        virtual bool DrawOGL() const;
        virtual TypeID GetID() const { return NONE; }
        /// Deletes (dealocate memory) recursively all children marked as 'autoDelete'.
        void AutoDeleteChildren() const;
        /// Recusively searches its children for a given name
		VPSceneNode* FindChildByName(const std::string& name) const;

		std::list<VPSceneNode*> GetChilds(){ return childList; }
		
		/// \brief Find the name of nodes until 'target' node. 
		/// \return Number of Nodes until 'target'. If 0 = target not found. 'nodeList' stores nodes.
		int GetPathToNodeName( char target[], std::list<VPSceneNode*>& nodeList );
		/// \brief Find all the nodes of with typeID 'type'
		/// \return total Number of Nodes. 'nodeList' stores nodes.
		int GetNodeTypeList( TypeID type, std::list<VPSceneNode*>& nodeList );

    protected:
    // PROTECTED METHODS
        /// Non-recursive drawing - should be overriden by every derived clas
        virtual bool DrawInstanceOGL() const { return false; };
    // PROTECTED ATTRIBUTES
        /// Child list
        std::list<VPSceneNode*> childList;
        /// Textual identification
        std::string description;
};

#endif
