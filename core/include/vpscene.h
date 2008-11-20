/// \file vpscene.h
/// \brief Header file for VPScene VPAT class.
/// \version $Revision: 1.15 $

// ChangeLog is at the implementation file

#ifndef __VPSCENE_H
#define __VPSCENE_H

#include "vplight.h"
#include "vpcamera.h"
#include "vpcolor.h" // for background color
#include "vpscenenode.h"
#include "vpboundingbox.h"
#include <string> //STL include
#include <list>   //STL include

/// \class VPScene vpscene.h
/// \brief A set of objects, lights and cameras.
///
/// A Scene is a complete viewable object with objects, lights, cameras and
/// background color. Instances of this class should be declared as static objects inside
/// the "main" function (see VPMemoryObj and ~VPScene()).
class VPScene {
    public:
        VPScene();
        /// \brief Destructor.
        ///
        /// The scene desctructor is of key importance in VPAT applications because it
        /// dealocates memory for every scene node marked as "auto delete" (see VPMemoryObj).
        /// The application programmer should create scenes as late as possible to ensure
        /// its destructor will be called before static members of classes are destroyed. This
        /// usually means that a scene should be declared inside the "main" function. It should
        /// also be declared as an static object.
        virtual ~VPScene();
        std::list<const VPLight*> GetLights();
        std::list<VPCamera*> GetCameras();
        std::list<VPSceneNode*> GetObjects();

        /// \brief Adds a light to the list of lights.
        ///
        /// This version only copies the light pointer to the internal list. The
        /// application programmer should keep the memory space untouched while
        /// the scene uses it.
        void AddLight(VPLight* newLight);

        /// \brief Adds a light to the list of lights.
        ///
        /// This version creates an internal copy of the light, so that the application
        /// programmer may free the light's memory space.
        void AddLight(const VPLight& newLight);

        /// \brief Add a camera to the list of cameras.
        ///
        /// The first added camera is made the current camera. Other cameras are
        /// added to end of the list of cameras. Currently, it is not a good idea
        /// to share a camera between different scenes, because viewers change the
        /// aspect ratio of their scenes.
        void AddCamera(VPCamera* newCamera);

        /// \brief Add a object to the scene.
        ///
        /// The object's address is added to the scene's list. The object will be
        /// dealocated withing the scene destructor if marked as "auto delete". If
        /// not, the application programmer is responsible for freeing memory at 
        /// the end of scene's existence.
        void AddObject(VPSceneNode* newObjectPtr);

        const VPLight* GetLight(const std::string& lightName);
        const VPCamera* GetCamera(const std::string& cameraName);
        /// \brief Finds an object by its description.
        VPSceneNode* GetObject(const std::string& objectName) const;
        /// \brief Recursive GetObject.
        VPSceneNode* GetObjectRec(const std::string& objectName) const;

        /// \brief Returns the background color.
        ///
        /// Returns a constant reference to the background color.
        const VPColor& GetBackgroundColor();
        void SetBackgroundColor(VPColor color);

        /// \brief Sets projection and draws graphics objects using OpenGL commands.
        ///
        /// This method is intended to be executed at every rendering cicle. It
        /// does not draw lights (from the "lights" list), because they need not be
        /// drawn at every rendering cicle.
        /// \return false if VPAT was not compiled with OpenGL support.
        virtual bool DrawOGL() const;

        /// \brief Set lights using OpenGL commands.
        ///
        /// Lights may be drawn apart from other scene components because they need
        /// not be drawn at every rendering cicle.
        /// \return false if VPAT was not compiled with OpenGL support.
        bool DrawLightsOGL() const;

        /// Returns the current camera in the scene or NULL if no camera exists.
        VPCamera* GetCurrentCamera() const;

        /// Sets the current camera to the next camera in list of cameras.
        void UseNextCamera();

        /// Sets the current camera to the previous camera in the list of cameras.
        void UsePreviousCamera();

        /// Set the aspect ratio of all cameras
        void SetAllCamerasAspectRatio(float newAspectRatio);

        /// \brief Computes the axis aligned bounding box of all objects.
        bool ComputeBoundingBox();

        /// \brief Returns the scene bounding box.
        /// Attention: uses the ComputeBoundingBox method to refresh value of bounding box.
        const VPBoundingBox& GetBoundingBox() const { return bBox; }

        /// \brief Toggles the bounding box visibility.
        void ToggleBBoxVisibility() { bBox.visible = !bBox.visible; }

        /// Returns the number of light in the scene.
        unsigned int GetNumLights() const { return lights.size(); }

        /// \brief Fits the whole scene to drawing region.
        ///
        /// Changes current camera's position and clipping planes so that the whole scene
        /// fits the viewer window.
        void MakeCameraViewAll();

        /// Sets the scene description.
        void SetDescription(const std::string& newDescr) { description = newDescr; }

        /// Returns the scene description.
        const std::string& GetDescription() { return description; }

		/// \brief Get the Final Transform Matrix on drawing 'objName' object. Stores at m[16].
		/// \return false if no object 'objName' found. If true, m holds transform matrix.
		bool GetObjectTranformMatrix( char objName[], double m[]);

		/// \brief Get all scenenodes of type 'joint' in the scene.
		/// \return a list of scene nodes of type joint.
		std::list<VPSceneNode*> GetAllSceneJoints();

		/// \brief Get all scenenodes of type 'type' in the scene.
		/// \return a list of scene nodes of type 'type'
		std::list<VPSceneNode*> GetAllSceneTypeObject( VPSceneNode::TypeID type);

    protected:
        /// Separate lights (not treated as common objects)
        std::list<const VPLight*> lights;
        // FixMe: Viewers should manipulate a local copy of the current camera, allowing
        // pointers to be const.
        std::list<VPCamera*> cameras;
        std::list<VPSceneNode*> objects;
        VPColor background;
    private:
        // FixMe: Viewers should manipulate a local copy of the current camera, a different
        // method to switch between cameras should be developed.
        std::list<VPCamera*>::const_iterator currentCamera;
        VPBoundingBox bBox;
        std::string description;
};

#endif  // __VPSCENE_H
