/// \file vpscene.cpp
/// \brief Implementation file for VPScene VPAT class.
/// \version $Revision: 1.16 $

// ChangeLog
// May 29, 2006 - Bruno de Oliveira Schneider
// - GetObjectRec and GetObject are now const methods.
// - ComputeBoundingBox now checks the type of its children to call the right
//   bounding box method. Other ComputeBoundingBox's bugs fixed.
// - Removed bBoxShow because VPBoundingBox already has a visibility flag.
// - Some documentation added.
// May 17, 2006 - Dalton Reis
// - Added ProcessCenter in ComputeBoundingBox and changed for bool.
// May 08, 2006 - Dalton Reis
// - Added ComputeBoundingBox method.
// - It verifies if bounding box is visible.
// Apr 20, 2006 - Cleber Souza Ughini
// - GetObjectTranformMatrix created.
// Mar 16, 2006 - Bruno de Oliveira Schneider
// - GetBackgroundColor now returns a constant reference.
// - DrawOGL no longer clears OpenGL buffers. This should be done by viewer classes.
// Feb 17, 2006 - Andreia Schneider
// - Removed const from pointers in "objects" because when creating objects from
//   VPXml::LoadScene, one may need to search the scene graph for pointers.
// - Added method GetObjectRec.
// Jun 07, 2005 - Bruno de Oliveira Schneider
// - DrawOGL now checks for existence of a current camera.
// - Added description attribute and related Get/Set methods.
// Dec 15, 2004 - Bruno de Oliveira Schneider
// - DrawOGL is now virtual, allowing use of derived classes in the viewers.
// - The destructor now deletes objects and lights marked as "auto delete".
// - Added SetAllCamerasAspectRatio, ComputeBoundingBox and GetBoundingBox.
// - Removed lines of old commented coded to control scene changes.
// - AddLight(const VPLight&) now makes a copy of its parameter.
// - Added GetNumLights and MakeCameraViewAll.
// Oct 04, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started. Indentation fixed.
// - Moved "using namespace std" from header file to implementation file.
// - Removed "vp" prefix from every method name.
// - Changed the prototypes of GetLight, GetCamera and GetObject to avoid passing
//   a string by copy.
// - Added "AddLight(const VPLight&)" method because it was not possible to add a
//   constant light to a scene. Changed the lists "lights" and "objects" so
//   that they now keep const pointers.
// - Changed cameras, lights and objects to "protected" because derived classes need
//   access to them in order to call draw routines.
// - Added "DrawOGL" method.
// - Added "currentCamera" atribute. Added "GetCurrentCamera", "UseNextCamera"
//   and "UsePreviousCamera" methods.
// - Changed the type of objects from VPGraphicObj to VPSceneNode.
// - Removed methods and attributes to mark changed children.
// January 04, 2001 - Isabel Harb Manssour
// - Addition of new methods, constants and the "typeOfChange" attribute.
// - New methods implementation.
// September 27, 2000 - Anderson Maciel
// - New methods and attributes addition.
// June 22, 2000 - Anderson Maciel
// - Methods implementation.
// June 08, 2000 - Luciana Porcher Nedel
// - Class declaration.
// May 30, 2000 - Isabel Harb Manssour
// - Class declaration.

#include "vpscene.h"
#include "vpgraphicobj.h"
#include "vptransform.h"

#include <cassert>

#ifdef VP_OGL
#include <GL/gl.h>
#endif

using namespace std;

VPScene::VPScene() {
    background = VPColor::BLACK();
    currentCamera = cameras.end();
    bBox.SetColor(VPColor::WHITE());
}

VPScene::~VPScene() {
    list<VPSceneNode*>::const_iterator objItr;
    list<const VPLight*>::const_iterator lightItr;
    //~ cout << "VPScene::~VPScene()" << endl;
    for (objItr = objects.begin(); objItr != objects.end(); ++objItr)
    {
        (*objItr)->AutoDeleteChildren();
        if ((*objItr)->autoDelete)
            delete (*objItr);
    }
    for (lightItr = lights.begin(); lightItr != lights.end(); ++lightItr)
    {
        if ((*lightItr)->autoDelete)
            delete (*lightItr);
    }
}

list<const VPLight*> VPScene::GetLights() {
    return lights;
}

list<VPCamera*> VPScene::GetCameras() {
    return cameras;
}

list<VPSceneNode*> VPScene::GetObjects() {
    return objects;
}

void VPScene::AddLight(VPLight* newLight) {
    lights.push_back(newLight);
}

void VPScene::AddLight(const VPLight& newLight) {
    VPLight* lightPtr = new VPLight(newLight);
    lightPtr->autoDelete = true;
    lights.push_back(lightPtr);
}

void VPScene::AddCamera(VPCamera* newCamera) {
    cameras.push_back( newCamera );
    if (currentCamera == cameras.end())
        currentCamera = cameras.begin();
}

void VPScene::AddObject( VPSceneNode* newObjectPtr ) {
    //~ cout << "VPScene::AddObject(" << newObjectPtr->GetDescription() << ")" << endl;
    objects.push_back( newObjectPtr );
}

const VPLight* VPScene::GetLight(const string& lightName) {
    list<const VPLight*>::const_iterator iter;
    for (iter = lights.begin(); iter != lights.end(); ++iter) {
        if( (*iter)->GetDescription() == lightName ) {
            return (*iter);
        }
    }
    return NULL;
}

const VPCamera* VPScene::GetCamera(const string& cameraName) {
    list<VPCamera*>::const_iterator iter;
    for (iter = cameras.begin(); iter != cameras.end(); ++iter) {
        if( (*iter)->GetDescription() == cameraName ) {
            return (*iter);
        }
    }
    return NULL;
}

VPSceneNode* VPScene::GetObject(const string& objectName) const {
    list<VPSceneNode*>::const_iterator iter;
    for (iter = objects.begin(); iter != objects.end(); ++iter) {
        if( (*iter)->GetDescription() == objectName ) {
            return (*iter);
        }
    }
    return NULL;
}

VPSceneNode* VPScene::GetObjectRec(const string& objectName) const {
    VPSceneNode* result;
    list<VPSceneNode*>::const_iterator iter;
    for (iter = objects.begin(); iter != objects.end(); ++iter) {
        if( (*iter)->GetDescription() == objectName )
            return (*iter);
        else
        {
            result = (*iter)->FindChildByName(objectName);
            if (result)
                return result;
        }
    }
    return NULL;
}

const VPColor& VPScene::GetBackgroundColor() {
    return background;
}

void VPScene::SetBackgroundColor(VPColor color) {
    background = color;
}

bool VPScene::DrawOGL() const {
#ifdef VP_OGL
    assert(*currentCamera); // make sure there is a current camera
    // Lights need not be drawn every rendering cicle. They are should be drawn
    // by a different method.
    DrawLightsOGL();

    // LookAT
    (*currentCamera)->DrawOGL();

    // Draw graphical objects
    list<VPSceneNode*>::const_iterator iter;
    for (iter = objects.begin(); iter != objects.end(); ++iter)
        (*iter)->DrawOGL();
    if (bBox.visible)
        bBox.DrawInstanceOGL();
    return true;
#else
    return false;
#endif
}
//}

bool VPScene::DrawLightsOGL() const {
#ifdef VP_OGL
    unsigned int lightID = 0;
    list<const VPLight*>::const_iterator iter;
    for (iter = lights.begin(); iter != lights.end(); ++iter)
    {
        (*iter)->DrawOGL(lightID);
        ++lightID;
    }
    return true;
#else
    return false;
#endif
}

VPCamera* VPScene::GetCurrentCamera() const {
    if (currentCamera == cameras.end())
        return NULL;
    else
        return *currentCamera;
}

void VPScene::UseNextCamera() {
    ++currentCamera;
    if (currentCamera == cameras.end())
        currentCamera = cameras.begin();
}

void VPScene::UsePreviousCamera() {
    --currentCamera;
    if (currentCamera == cameras.end())
        --currentCamera;
}

void VPScene::SetAllCamerasAspectRatio(float newAspectRatio) {
    list<VPCamera*>::const_iterator iter;
    for (iter = cameras.begin(); iter != cameras.end(); ++iter) {
        (*iter)->SetAspectRatio(newAspectRatio);
    }
}

bool VPScene::ComputeBoundingBox() {
    VPBoundingBox box;
    bool initBBox = false;
    list<VPSceneNode*>::const_iterator iter;
    VPGraphicObj* objPtr;
    VPTransform* transPtr;

    for (iter = objects.begin(); iter != objects.end(); ++iter) {
        objPtr = dynamic_cast<VPGraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ComputeRecursiveBoundingBox();
            if (initBBox)
                bBox.MergeWith(objPtr->GetRecursiveBoundingBox());
            else {
                bBox.CopyGeometryFrom(objPtr->GetRecursiveBoundingBox());
                initBBox = true;
            }
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VPTransform*>(*iter);
            if (transPtr) { // object is a transform
                if (transPtr->RecursiveBoundingBox(&box)) {
                    if (initBBox)
                        bBox.MergeWith(box);
                    else {
                        bBox.CopyGeometryFrom(box);
                        initBBox = true;
                    }
                }
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
    bBox.ProcessCenter();
    return initBBox;
}

//~ void VPScene::ComputeBoundingBox() { //old version
    //~ list<VPSceneNode*>::const_iterator iter = objects.begin();
    //~ const VPGraphicObj* objPtr;
    //~ bool notInitialized = true;

    //~ // Initialize the bounding box
    //~ for (; (iter != objects.end()) && notInitialized; ++iter) {
        //~ objPtr = dynamic_cast<const VPGraphicObj*>(*iter);
        //~ if (objPtr) // objPtr != NULL
        //~ {
            //~ bbox = objPtr->GetRecursiveBoundingBox();
            //~ notInitialized = false;
        //~ }
    //~ }

    //~ // Merge the bounding box with other graphical objects
    //~ for (; iter != objects.end(); ++iter) {
        //~ objPtr = dynamic_cast<const VPGraphicObj*>(*iter);
        //~ if (objPtr) // objPtr != NULL
            //~ bbox.MergeWith(objPtr->GetRecursiveBoundingBox());
    //~ }
//~ }

void VPScene::MakeCameraViewAll()
{
    assert(cameras.size() > 0);
    VPPoint4D bBoxCenter;
    double distance, cameraDistance;

    bBox.ProcessCenter();
    bBoxCenter = bBox.GetCenter();
    (*currentCamera)->SetTarget(bBoxCenter);
    cameraDistance = bBox.GetGreaterX()-bBox.GetSmallerX();
    distance = bBox.GetGreaterY()-bBox.GetSmallerY();
    if (distance > cameraDistance)
        cameraDistance = distance;
    distance = bBox.GetGreaterZ()-bBox.GetSmallerZ();
    if (distance > cameraDistance)
        cameraDistance = distance;

    (*currentCamera)->SetLocation(bBoxCenter+VPPoint4D(0,0,cameraDistance*1.2,0));
    (*currentCamera)->SetUp(VPPoint4D::Y());
    // FixMe: Not sure what to do with near plane distance
    //~ (*currentCamera)->SetNearPlaneDistance();
    (*currentCamera)->SetFarPlaneDistance(cameraDistance*2.4);
}


bool VPScene::GetObjectTranformMatrix( char objName[], double m[])
{
#ifdef VP_OGL
    list<VPSceneNode*> nodeList;
    list<VPSceneNode*>::const_iterator iter;
    VPTransform* mTrans;

    int pathsize=0;

    for (iter = objects.begin(); iter != objects.end() && pathsize == 0; ++iter)
        pathsize = (*iter)->GetPathToNodeName( objName, nodeList);

    if( pathsize == 0 )
        return false;

    //here we use the openGL matrix due performance issues
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for( iter = nodeList.begin(); iter != nodeList.end(); ++iter)
    {
        mTrans = dynamic_cast <VPTransform*>(*iter);
        if ( mTrans != NULL )
            glMultMatrixd( mTrans->GetData());
    }
    glGetDoublev( GL_MODELVIEW_MATRIX, m);
    glPopMatrix();

    return true;
#else
    return false;
#endif
}

std::list<VPSceneNode*> VPScene::GetAllSceneJoints()
{
    list<VPSceneNode*> nodeList;
    list<VPSceneNode*>::const_iterator iter;

    for (iter = objects.begin(); iter != objects.end(); ++iter)
        (*iter)->GetNodeTypeList( VPSceneNode::JOINT, nodeList);

    return nodeList;
}

std::list<VPSceneNode*> VPScene::GetAllSceneTypeObject( VPSceneNode::TypeID type)
{
    list<VPSceneNode*> nodeList;
    list<VPSceneNode*>::const_iterator iter;

    for (iter = objects.begin(); iter != objects.end(); ++iter)
        (*iter)->GetNodeTypeList( type, nodeList);

    return nodeList;
}
