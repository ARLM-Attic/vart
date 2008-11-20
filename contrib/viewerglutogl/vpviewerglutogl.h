/// \file vpviewerglutogl.h
/// \brief Header file for VPGlutOglViewer VPAT class.
/// \version $Revision: 1.10 $

// ChangeLog is at the implementation file.

#ifndef __VPVIEWERGLUTOGL_H
#define __VPVIEWERGLUTOGL_H

#include "vpscene.h"
#include "vpmousecontrol.h"
#include <list>

/// \class VPViewerGlutOGL vpviewerglutogl.h
/// \brief VPAT Viewer that uses GLUT/OpenGL.
///
/// A ViewerGlutOGL is a window which can display a VPAT scene (VPScene) using GLUT
/// and OpenGL. The window also has basic behavior such as mouse navigation and 
/// keyboard response. At most 6 instances of this class can be created.

class VPViewerGlutOGL
{
    public:
    // PUBLIC NESTED CLASSES
        // These classes are nested because other types of viewers may have different
        // needs for their handlers.

        /// Keyboard handler
        class KbHandler
        {
            friend class VPViewerGlutOGL;
            public:
                virtual ~KbHandler() {};
                /// Called when a key is pressed.
                virtual void HandleKey(unsigned char key)=0;
            protected:
                /// Points to the viewer on which the handler was attached.
                VPViewerGlutOGL* viewerPtr;
        };
        /// Idle time handler
        class IdleHandler
        {
            friend class VPViewerGlutOGL;
            public:
                virtual ~IdleHandler() {};
                /// Called when the application is idle.
                virtual void OnIdle()=0;
            protected:
                /// Points to the viewer on which the handler was attached.
                VPViewerGlutOGL* viewerPtr;
        };
        /// \brief Draw handler
        ///
        /// A draw handler allows the application programmer to add custom behavior associated
        /// with the window drawing. To use a draw handler, declare a derived class that
        /// implements the OnDraw method, then link it to the viewer using SetDrawHandler.
        /// The OnDraw method will be called after the window is drawn at every rendering cycle.
        class DrawHandler
        {
            friend class VPViewerGlutOGL;
            public:
                virtual ~DrawHandler() {};
                /// \brief Called when the window is redrawn.
                ///
                /// When a draw handler is associated to a viewer, the OnDraw method will
                /// be called at every rendering cycle, after the scene has been drawn.
                virtual void OnDraw()=0;
            protected:
                /// Points to the viewer on which the handler was attached.
                VPViewerGlutOGL* viewerPtr;
        };
    // PUBLIC METHODS
        /// \brief Creates an empty, unusable viewer positioned at (0,0).

        /// Before using the viewer, a scene (with at least one object, one camera
        /// and one light) should be attached to it.
        VPViewerGlutOGL();

        /// \brief Creates an empty, unusable viewer positioned at (0,0) of given size.

        /// Before using the viewer, a scene (with at least one object, one camera
        /// and one light) should be attached to it.
        VPViewerGlutOGL(int newWidth, int newHeight);

        ~VPViewerGlutOGL();

        /// \brief Attaches a scene to the viewer.

        /// Attachment is done by storing a pointer to the scene. The viewer
        /// itself will not change the scene, but it should handle a changing scene.
        /// Lighting is enabled if there are lights attached to the scene.
        void SetScene(VPScene& scene);

        /// \brief Changes the position of a viewer.
        /// \param x [in] New left coordinate.
        /// \param y [in] New top coordinate.
        void SetPosition(int x, int y);

        /// \brief Add a keyboard handler to the viewer.
        void SetKbHandler(KbHandler* ptrKbH);

        /// \brief Sets the idle time handler.
        void SetIdleHandler(IdleHandler* newIHPtr);

        /// \brief Sets the drawing handler.
        void SetDrawHandler(DrawHandler* newDHPtr);

        /// \brief Sets the mouse click handler.
        ///
        /// Define a "click handler" if you want your application to receive
        /// mouse button events (button down/up).
        void SetClickHandler(VPMouseControl::ClickHandler* newCHPtr);

        /// \brief Sets the mouse drag handler.
        void SetDragHandler(VPMouseControl::DragHandler* newDHPtr);

        /// \brief Sets the mouse motion handler.
        void SetMotionHandler(VPMouseControl::MotionHandler* newMHPtr);

        /// \brief Idle Management.
        ///
        /// Currently, handles mouse movment, updating the camera.
        void Idle();

        /// Sets the window size of the viewer.
        void SetSize(int newWidth, int newHeight);
        /// Returns current window width.
        int GetWidth() { return width; }
        /// Returns current window height.
        int GetHeight() { return height; }
        /// Returns the current camera in the scene or NULL if no camera exists.
        const VPCamera* GetCurrentCamera() const { return ptScene->GetCurrentCamera(); }

        /// Hides the viewer.
        void Hide();

        /// Shows the viewer.
        void Show();

        /// Make the viewer become the current window.
        void TurnIntoCurrentWindow();

        /// Changes window title.
        void SetTitle(const std::string& newTitle);

        /// Iconifies (minimizes) the viewer window.
        void Iconify();

        /// Marks the viewer window for redisplay.
        void PostRedisplay();

        /// Tells the scene to use next camera in its list.
        void UseNextCamera() { ptScene->UseNextCamera(); }

    // PUBLIC STATIC METHODS

        /// \brief Enters main rendering loop.
        ///
        /// Draws (set them up) all lights in the scene and enters GLUT main loop.
        /// Never returns.
        static void MainLoop();

        /// \brief Initializes GLUT Library.
        ///
        /// Call this method before creation of the first viewer, passing the
        /// application arguments.
        static void Init(int* argcPtr, char* argv[]);
    // PUBLIC ATTRIBUTES
        /// Sets whether the aspect ratio of the viewer window should be passed
        /// along to its cameras.
        bool autoChangeCameraAspect;

        /// \brief Sets whether the viewer should redraw at every Idle call.
        ///
        /// This may usefull for simulations, where the scene is changing, to
        /// continuously redraw the scene.
        bool redrawOnIdle;

        /// How much to walk at each step (mouse movement).
        float walkStep;

        /// Sets whether the built-in navigation is enabled. This is set to "true" by default.
        bool autoNavigationEnabled;

        /// Sets whether the built-in keyboard responses are enabled.
        /// This is set to "true" by default.
        bool autoRespondKeys;

    protected:
        /// \brief Sets clear color and clears OpenGL buffers.
        void ClearOGLBuffers();

    private:
    // PRIVATE STATIC METHODS (GLUT CALLBACKS)
        // Drawing callbacks
        static void DrawCB0();
        static void DrawCB1();
        static void DrawCB2();
        static void DrawCB3();
        static void DrawCB4();
        static void DrawCB5();
        // Mouse click callbacks
        static void MouseCB0(int, int, int, int);
        static void MouseCB1(int, int, int, int);
        static void MouseCB2(int, int, int, int);
        static void MouseCB3(int, int, int, int);
        static void MouseCB4(int, int, int, int);
        static void MouseCB5(int, int, int, int);
        // Mouse drag callbacks
        static void DragMouseCB0(int x, int y);
        static void DragMouseCB1(int x, int y);
        static void DragMouseCB2(int x, int y);
        static void DragMouseCB3(int x, int y);
        static void DragMouseCB4(int x, int y);
        static void DragMouseCB5(int x, int y);
        // Keyboard callbacks
        static void KeyboardCB0(unsigned char key, int x, int y);
        static void KeyboardCB1(unsigned char key, int x, int y);
        static void KeyboardCB2(unsigned char key, int x, int y);
        static void KeyboardCB3(unsigned char key, int x, int y);
        static void KeyboardCB4(unsigned char key, int x, int y);
        static void KeyboardCB5(unsigned char key, int x, int y);
        // Resize callbacks
        static void ResizeCB0(int newWidth, int newHeight);
        static void ResizeCB1(int newWidth, int newHeight);
        static void ResizeCB2(int newWidth, int newHeight);
        static void ResizeCB3(int newWidth, int newHeight);
        static void ResizeCB4(int newWidth, int newHeight);
        static void ResizeCB5(int newWidth, int newHeight);
        // Idle callback
        static void IdleMngr();
    // PRIVATE STATIC ATRIBUTES
        static int glutIDVec[6]; // GLUT window IDs
        static VPViewerGlutOGL* instancePtrVec[6]; // pointers to class instances
    // PRIVATE METHODS
        void RegisterCallbacks();
        void CommonConstructor();
        void HandleKey(unsigned char key);
    // PRIVATE ATRIBUTES
        VPScene* ptScene; // objects, lights and cameras
        int glutID; // GLUT window id
        KbHandler* kbHandlerPtr;
        DrawHandler* drawHandlerPtr; // pointer to draw handler
        IdleHandler* idleHndPtr; // pointer to idle handler
        VPMouseControl mouseController;
        int height; // window height in pixels
        int width;  // window width in pixels
};

#endif
