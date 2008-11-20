/// \file vpscenenode.cpp
/// \brief Implementation file for VPSceneNode VPAT class.
/// \version $Revision: 1.13 $

// ChangeLog
// May 17, 2006 - Bruno de Oliveira Schneider
// - Method RecursiveBoundingBox has been removed.
// - Added default case to GetNodeTypeList in order to pacify the compiler.
// Apr 20, 2006 - Cleber Souza Ughini
// - GetPathToNodeName created.
// Mar 09, 2006 - Bruno de Oliveira Schneider
// - Fixed some previous changelog items.
// - Removed debuging msgs from VPSceneNode::AddChild.
// Feb 17, 2006 - Andreia Schneider
// - Removed const of the childList and others VPSceneNode.
// Feb 16, 2006 - Bruno de Oliveira Schneider
// - Added FindChildByName
// May 12, 2005 - Bruno de Oliveira Schneider
// - Removed the "ID" suffix of every TypeID, added some underscores to the IDs.
// - GetDescription now returns by reference.
// Dec 15, 2004 - Bruno de Oliveira Schneider
// - Uncommented TypeID enum, added GetID() to start using ids.
// - This class is now derived from VPMemoryObj.
// - Added AutoDeleteChildren.
// Oct 04, 2004 - Bruno de Oliveira Schneider
// - File created.

#include "vpscenenode.h"
#include "vpjoint.h"
#include "vpmeshobject.h"
#include "vptransform.h"

#include <iostream>
using namespace std;

VPSceneNode::VPSceneNode()
{
}

VPSceneNode::~VPSceneNode()
{
    //~ cout << "VPSceneNode::~VPSceneNode(): " << GetDescription() << endl;
}

VPSceneNode::VPSceneNode(VPSceneNode& node)
{
    childList = node.childList;
    description = node.description;
}

VPSceneNode& VPSceneNode::operator=(VPSceneNode& node)
{
    childList = node.childList;
    description = node.description;
    return *this;
}

void VPSceneNode::AddChild(VPSceneNode& child)
{
    childList.push_back(&child);
}

bool VPSceneNode::DrawOGL() const
{
    bool result;
    list<VPSceneNode*>::const_iterator iter;

    result = DrawInstanceOGL();
    for (iter = childList.begin(); iter != childList.end(); ++iter)
        result = (result && (*iter)->DrawOGL());
    return result;
}

void VPSceneNode::AutoDeleteChildren() const
{
    list<VPSceneNode*>::const_iterator iter;
    for (iter = childList.begin(); iter != childList.end(); ++iter)
    {
        (*iter)->AutoDeleteChildren();
        if ((*iter)->autoDelete)
            delete (*iter);
    }
}

VPSceneNode* VPSceneNode::FindChildByName(const std::string& name) const
{
    list<VPSceneNode*>::const_iterator iter;
    VPSceneNode* result;
    for (iter = childList.begin(); iter != childList.end(); ++iter)
    {
        if ((*iter)->GetDescription() == name)
            return *iter;
        else{
            result = (*iter)->FindChildByName(name);
            if (result) return result;
        }
    }
    return NULL;
}

int VPSceneNode::GetPathToNodeName( char target[], std::list<VPSceneNode*>& nodeList )
{
    list<VPSceneNode*>::const_iterator iter;
    int i;

    nodeList.push_back( this );

    if( strcmp( target, description.c_str()) == 0 )
        return 1;

    for (iter = childList.begin(); iter != childList.end(); ++iter)
    {
        i = (*iter)->GetPathToNodeName( target, nodeList);
        if( i > 0 )
            return i + 1;
    }
    nodeList.pop_back();
    return 0;
}

int VPSceneNode::GetNodeTypeList( TypeID type, std::list<VPSceneNode*>& nodeList )
{
    list<VPSceneNode*>::const_iterator iter;
    int i=0;

    switch ( type )
    {
        case JOINT:
            if ( dynamic_cast <VPJoint*>(this) != NULL )
            {
                nodeList.push_back( this );
                i++;
            }
            break;
        case TRANSFORM:
            if ( dynamic_cast <VPTransform*>(this) != NULL )
            {
                nodeList.push_back( this );
                i++;
            }
            break;
        case MESH_OBJECT:
            if ( dynamic_cast <VPMeshObject*>(this) != NULL )
            {
                nodeList.push_back( this );
                i++;
            }
            break;
        default: ; // avoid compiler complaints about cases not handled
    }

    for (iter = childList.begin(); iter != childList.end(); ++iter)
            i = i + (*iter)->GetNodeTypeList( type, nodeList);

    return i;
}
