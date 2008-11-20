// main.cpp - vigilance camera application (biaxial joint example)

// VPAT example: Biaxial Joint

// This application shows/controls a vigilance camera that has two degrees of freedom.
// It is intended to be an example of how to use a biaxial joint.

// When running the application use 'q' to quit. Use 'i', 'j', 'k', 'l' to move
// the vigilance camera's degrees of freedom.

// Note: VPViewerGlutOGL should be found in the "contrib" directory, because it is
// too specific to go into VPAT core.

// Changelog
// Mar 17, 2006 - Bruno de Oliveira Schneider
// - This example may use visual joints now.
// Jun 07, 2005 - Bruno de Oliveira Schneider
// - Code updated to comply with the new VPViewerGlutOGL version.
// Oct 01, 2004 - Bruno de Oliveira Schneider
// - Application created.

#include <vpscene.h>
#include <vpcylinder.h>
#include <vplight.h>
#include <vppoint4d.h>
#include <vpsphere.h>
#include "vpviewerglutogl.h"
#include "vigilancecamera.h"

#include <iostream>

using namespace std;

// Define the keyboard handler
class KeyboardHandler : public VPViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler(clVigilanceCamera* cam) {
            ptrCam = cam;
        }
        virtual void HandleKey(unsigned char key) {
            switch (key)
            {
                case 'j':
                    ptrCam->Flex(-0.005);
                    viewerPtr->PostRedisplay();
                    break;
                case 'l':
                    ptrCam->Flex(0.005);
                    viewerPtr->PostRedisplay();
                    break;
                case 'i':
                    ptrCam->Adduct(0.01);
                    viewerPtr->PostRedisplay();
                    break;
                case 'k':
                    ptrCam->Adduct(-0.01);
                    viewerPtr->PostRedisplay();
                    break;
            }
        }
    private:
        clVigilanceCamera* ptrCam;
};

// The application itself:
int main(int argc, char* argv[])
{
    VPViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    VPScene scene; // create a scene
    // create a camera (scene observer)
    VPCamera camera(VPPoint4D(-5,0,3.5,1),VPPoint4D(0,0,1.5,1),VPPoint4D(0,0,-1,0));
    clVigilanceCamera vgCamera; // create a vigilance camera (scene object)
    VPViewerGlutOGL viewer; // create a viewer (application window)

    // Build up the scene
    scene.AddObject(&vgCamera);
    scene.AddLight(VPLight::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    #ifdef VISUAL_JOINTS
    VPDof::axisSize = 2.3f;
    #endif

    // Set up the viewer
    viewer.SetScene(scene); // attach the scene
    KeyboardHandler kbh(&vgCamera); // create a keyboard handler
    viewer.SetKbHandler(&kbh); // attach the keyboard handler

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VPViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
