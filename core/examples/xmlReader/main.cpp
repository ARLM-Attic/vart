// main.cpp - robot arm aplication (xml reader example)

// VPAT example: xml reader

// This application shows/controls a robot arm that has loaded using the class vpxml.
// When running the application use 'q' to quit. 

// Note: VPViewerGlutOGL should be found in the "contrib" directory, because it is
// too specific to go into VPAT core.

// Changelog
// Mar 16, 2006 - Andreia Schneider
// - vpxml methods and objects changed to vpxmlscene methods and objects.
// Feb 23, 2006 - Andreia Schneider
// - Application created.

#include <vpscene.h>
#include <vpcylinder.h>
#include <vplight.h>
#include <vppoint4d.h>
#include <vpsphere.h>
#include <vpxmlscene.h>
#include <vpgraphicobj.h>
#include <list>
#include "vpviewerglutogl.h"

#include <iostream>

using namespace std;

class KeyboardHandler : public VPViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler(VPXmlScene* sceneAux){
            scene = sceneAux;
        }
        virtual void HandleKey(unsigned char key) {
            switch (key)
            {
                case '1':
                {
                    VPSceneNode* node;
                    node = new VPSceneNode;
                    VPBiaxialJoint* joint;
                    joint = new VPBiaxialJoint;
                    joint->autoDelete=true;

                    node = scene->GetObjectRec("arm1Joint");
                    joint = dynamic_cast <VPBiaxialJoint*>(node);
                    joint->Flex(0.005);
                    glutPostRedisplay();
                }
                break;

                case '2':
                {
                    VPSceneNode* node;
                    node = new VPSceneNode;
                    VPBiaxialJoint* joint;
                    joint = new VPBiaxialJoint;
                    joint->autoDelete=true;

                    node = scene->GetObjectRec("arm1Joint");
                    joint = dynamic_cast <VPBiaxialJoint*>(node);
                    joint->Flex(-0.005);
                    glutPostRedisplay();
                }
                break;

                case '3':
                {
                    VPSceneNode* node;
                    node = new VPSceneNode;
                    VPBiaxialJoint* joint;
                    joint = new VPBiaxialJoint;
                    joint->autoDelete=true;

                    node = scene->GetObjectRec("arm1Joint");
                    joint = dynamic_cast <VPBiaxialJoint*>(node);
                    joint->Adduct(0.005);
                    glutPostRedisplay();
                }
                break;

                case '4':
                {
                    VPSceneNode* node;
                    node = new VPSceneNode;
                    VPBiaxialJoint* joint;
                    joint = new VPBiaxialJoint;
                    joint->autoDelete=true;

                    node = scene->GetObjectRec("arm1Joint");
                    joint = dynamic_cast <VPBiaxialJoint*>(node);
                    joint->Adduct(-0.005);
                    glutPostRedisplay();
                }
                break;

                case '5':
                {
                    VPSceneNode* node;
                    node = new VPSceneNode;
                    VPUniaxialJoint* joint;
                    joint = new VPUniaxialJoint;
                    joint->autoDelete=true;

                    node = scene->GetObjectRec("arm2Joint");
                    joint = dynamic_cast <VPUniaxialJoint*>(node);
                    joint->Flex(0.005);
                    glutPostRedisplay();
                }
                break;

                case '6':
                {
                    VPSceneNode* node;
                    node = new VPSceneNode;
                    VPUniaxialJoint* joint;
                    joint = new VPUniaxialJoint;
                    joint->autoDelete=true;

                    node = scene->GetObjectRec("arm2Joint");
                    joint = dynamic_cast <VPUniaxialJoint*>(node);
                    joint->Flex(-0.005);
                    glutPostRedisplay();
                }
                break;
            }
        }

        private:
        VPXmlScene *scene;
};

int main(int argc, char* argv[])
{
    VPViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    VPXmlScene scene;
    VPMeshObject box;
    VPCamera camera(VPPoint4D(2.5,-10,5,1),VPPoint4D(0,0,0.05,1),VPPoint4D(0,1,0,0));
    VPViewerGlutOGL viewer; 
    
    if(argc<=1)
    {
        cout<<"O arquivo xml deve ser passado!"<<endl;
        return 1;
    }

    if (scene.LoadFromFile(argv[1]))
    {
        viewer.SetSize(480,480);
        viewer.SetTitle("Xml Reader");
        scene.AddLight(VPLight::BRIGHT_AMBIENT());
        scene.AddCamera(&camera);
        // Set up the viewer
        viewer.SetScene(scene); // attach the scene
        viewer.autoNavigationEnabled = true;
        KeyboardHandler kbh(&scene); // create a keyboard handler
        viewer.SetKbHandler(&kbh); // attach the keyboard handler
    
        // Run application
        scene.DrawLightsOGL(); // Set OpenGL's lights' state
        VPViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
        return 0;
    }
    else
        return 1;
}
