/// \file vpviewerglutogl.cpp
/// \brief Implementation file for VPViewerGlutOGL VPAT class.
/// \version $Revision: 1.9 $

// ChangeLog
// Mar 16, 2006 - Bruno de Oliveira Schneider
// - Added ClearOGLBuffers() because VPScene no longer clears the buffers.
// Dec 21, 2005 - Bruno de Oliveira Schneider
// - Added UseNextCamera().
// - Added GetCurrentCamera().
// - Fixed ResizeCBs - they now update "width" and "height".
// Oct 06, 2005 - Bruno de Oliveira Schneider
// - Added HandleKey to make maintenance easier.
// - Added autoNavigationEnabled and autoRespondKeys attributes.
// - Added attributes width and height. Added GetWidth and GetHeight.
// - Added virtual destructors.
// - Fixed SetIdleHandler: now checks if the pointer is valid.
// - Draw handlers are now called after the scene is drawn. This fixes problems when
//   reading the frame buffer and z buffer.
// - Added constructor with size. Common parts of constructors were moved to CommonConstructor,
//   to make code easier to maintain.
// Jun 13, 2005 - Bruno de Oliveira Schneider
// - The constructor now enables vertex arrays, fixing a bug on showing mesh objects with
//   multiple viewers.
// - Fixed current window management. Most methods now save the current window, then make
//   glut calls, then restore the current window. -> The Idle method might need it too.
// - Fixed SetTitle (now changes the icon title as well).
// - Moved glutInitDisplayMode into Init.
// Jun 07, 2005 - Bruno de Oliveira Schneider
// - Renamed SetIdleHandlerPointer to SetIdleHandler.
// - Handlers now have pointers to the viewer they are attached to.
// - Added SetTitle, Iconify and PostRedisplay.
// - The viewer now supports mouse handlers and uses the new mouse control class.
// Apr 14, 2005 - Bruno de Oliveira Schneider
// - Changed DrawCB0 to check for errors in the drawing routine because
//   a lot of pleople were compiling VPAT without the rendering engine symbol. The
//   new code should give a good hint on what to look for.
// - Changed the list of keyboard handlers into a single handler, which required
//   changing AddKbHandler to SetKbHandler.
// - Changed the draw function callback into a handler object, turning SetDrawCallBack
//   into SetDrawHandler.
// Dec 15, 2004 - Bruno de Oliveira Schneider
// - Added Init. This static method should be called before creating instances.
//   The second number in the version id has been incremented to show that old
//   applications should be updated.
// - Added SetPosition and SetSize.
// - Fixed Idle function management. Now a global idle function calls every
//   instance's idle fuction.
// - Added autoChangeCameraAspect and resize callbacks.
// - Added SetDrawCallBack, onDraw and onDrawUserPtr.
// - Added user defined idle time handling (idleHndPtr, SetIdleHandlerPointer, etc.).
// - Added redrawOnIdle and walkStep.
// - Added modifiers (ALT, CTRL, SHIFT) to enhance built-in camera movement. MouseCB? were
//   changed, constants NO_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON, SHIFT_KEY,
//   CTRL_KEY, ALT_KEY and Idle().
// Sep 24, 2004 - Bruno de Oliveira Schneider
// - File created.

#include "vpviewerglutogl.h"
#include <cassert>
#include <GL/glut.h>

//?
#include <iostream>

using namespace std;

// INITIALIZATION OF STATIC ATRIBUTES
int VPViewerGlutOGL::glutIDVec[6] = { 0, 0, 0, 0, 0, 0 };
VPViewerGlutOGL* VPViewerGlutOGL::instancePtrVec[6] = { NULL, NULL, NULL, NULL, NULL, NULL };

// FixMe: Add other constructors to avoid SetPosition.

VPViewerGlutOGL::VPViewerGlutOGL()
{
    glutInitWindowSize(300,300);
    width = 300;
    height = 300;
    CommonConstructor();
}

VPViewerGlutOGL::VPViewerGlutOGL(int newWidth, int newHeight)
{
    glutInitWindowSize(newWidth, newHeight);
    width = newWidth;
    height = newHeight;
    CommonConstructor();
}

void VPViewerGlutOGL::CommonConstructor()
{
    glutInitWindowPosition(0,0);
    glutID = glutCreateWindow("VPAT Glut OpenGL Viewer");
    ptScene = NULL;
    idleHndPtr = NULL;
    kbHandlerPtr = NULL;
    drawHandlerPtr = NULL;
    mouseController.SetOwner(this);
    walkStep = 0.0001;
    redrawOnIdle = false;
    autoChangeCameraAspect = true;
    autoNavigationEnabled = true;
    autoRespondKeys = true;
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // Enable vertex arrays to allow drawing of optimized mesh objects
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    RegisterCallbacks();
}

VPViewerGlutOGL::~VPViewerGlutOGL()
{
    int i = 0;
    while (glutIDVec[i] != glutID)
        ++i;
    glutIDVec[i] = 0;
    instancePtrVec[i] = NULL;
}

void VPViewerGlutOGL::SetIdleHandler(IdleHandler* newIHPtr)
{
    idleHndPtr = newIHPtr;
    if (newIHPtr)
        newIHPtr->viewerPtr = this;
}

void VPViewerGlutOGL::SetPosition(int x, int y)
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutPositionWindow(x,y);
    glutSetWindow(currentWindow);
}

void VPViewerGlutOGL::SetSize(int newWidth, int newHeight)
{
    int currentWindow = glutGetWindow(); // save current window
    width = newWidth;
    height = newHeight;
    glutSetWindow(glutID);
    glutReshapeWindow(newWidth, newHeight);
    glutSetWindow(currentWindow); // restore current window
}

void VPViewerGlutOGL::TurnIntoCurrentWindow()
{
    glutSetWindow(glutID);
}

void VPViewerGlutOGL::SetDrawHandler(DrawHandler* newDHPtr)
{
    drawHandlerPtr = newDHPtr;
    newDHPtr->viewerPtr = this;
}

void VPViewerGlutOGL::SetScene(VPScene& scene)
{
    int currentWindow = glutGetWindow();

    ptScene = &scene;
    if (scene.GetNumLights() > 0)
    {
        glutSetWindow(glutID);
        glEnable(GL_LIGHTING);
        glutSetWindow(currentWindow);
    }
}

void VPViewerGlutOGL::ClearOGLBuffers()
{
    static float bgColor[4];
    ptScene->GetBackgroundColor().GetScaled(1.0f, bgColor); // convert color components to float
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void VPViewerGlutOGL::MainLoop()
// static method
{
    glutIdleFunc(IdleMngr);
    glutMainLoop();
}

void VPViewerGlutOGL::Init(int* argcPtr, char* argv[])
// static method
{
    glutInit(argcPtr, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
}

void VPViewerGlutOGL::DrawCB0()
// This is the only DrawCB that can check the result of ptScene->DrawOGL() because
// the first viewer is allways instance 0 and it doesn't matter which instance calls
// DrawOGL (all will fail for wrong compilation parameters). However, if one creates
// two viewers then deletes the first, this checking will be bypassed...
{
    //~ assert(glutGetWindow() == instancePtrVec[0]->glutID);
#ifdef NDEBUG
    // Release version
    instancePtrVec[0]->ClearOGLBuffers();
    instancePtrVec[0]->ptScene->DrawOGL();
#else
    // Debug version
    if (instancePtrVec[0]->ptScene == NULL)
    {
        cerr << "Fatal error: VPViewerGlutOGL tried to draw a scene pointed by NULL!" << endl;
        exit(1);
    }
    instancePtrVec[0]->ClearOGLBuffers();
    if (!(instancePtrVec[0]->ptScene->DrawOGL()))
    {
        cerr << "Fatal error: VPScene::DrawOGL returned false!\n\a"
             << "This means that VPAT has been compiled with no underlying rendering "
             << "engine, please check your project options and re-compile VPAT." << endl;
        exit(1);
    }
#endif
    if (instancePtrVec[0]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[0]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VPViewerGlutOGL::DrawCB1()
{
    //~ assert(glutGetWindow() == instancePtrVec[1]->glutID);
    instancePtrVec[1]->ClearOGLBuffers();
    instancePtrVec[1]->ptScene->DrawOGL();
    if (instancePtrVec[1]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[1]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VPViewerGlutOGL::DrawCB2()
{
    //~ assert(glutGetWindow() == instancePtrVec[2]->glutID);
    instancePtrVec[2]->ClearOGLBuffers();
    instancePtrVec[2]->ptScene->DrawOGL();
    if (instancePtrVec[2]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[2]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VPViewerGlutOGL::DrawCB3()
{
    instancePtrVec[3]->ClearOGLBuffers();
    instancePtrVec[3]->ptScene->DrawOGL();
    if (instancePtrVec[3]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[3]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VPViewerGlutOGL::DrawCB4()
{
    instancePtrVec[4]->ClearOGLBuffers();
    instancePtrVec[4]->ptScene->DrawOGL();
    if (instancePtrVec[4]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[4]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VPViewerGlutOGL::DrawCB5()
{
    instancePtrVec[5]->ClearOGLBuffers();
    instancePtrVec[5]->ptScene->DrawOGL();
    if (instancePtrVec[5]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[5]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VPViewerGlutOGL::MouseCB0(int button, int state, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[0];
    int keyModifier = glutGetModifiers();
    VPMouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VPMouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VPMouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VPMouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VPMouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VPMouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VPViewerGlutOGL::MouseCB1(int button, int state, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[1];
    int keyModifier = glutGetModifiers();
    VPMouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VPMouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VPMouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VPMouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VPMouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VPMouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VPViewerGlutOGL::MouseCB2(int button, int state, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[2];
    int keyModifier = glutGetModifiers();
    VPMouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VPMouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VPMouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VPMouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VPMouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VPMouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VPViewerGlutOGL::MouseCB3(int button, int state, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[3];
    int keyModifier = glutGetModifiers();
    VPMouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VPMouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VPMouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VPMouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VPMouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VPMouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VPViewerGlutOGL::MouseCB4(int button, int state, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[4];
    int keyModifier = glutGetModifiers();
    VPMouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VPMouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VPMouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VPMouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VPMouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VPMouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VPViewerGlutOGL::MouseCB5(int button, int state, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[5];
    int keyModifier = glutGetModifiers();
    VPMouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VPMouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VPMouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VPMouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VPMouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VPMouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VPMouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VPMouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VPViewerGlutOGL::DragMouseCB0(int x, int y)
{
    instancePtrVec[0]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[0]->mouseController.OnDrag();
}

void VPViewerGlutOGL::DragMouseCB1(int x, int y)
{
    instancePtrVec[1]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[1]->mouseController.OnDrag();
}

void VPViewerGlutOGL::DragMouseCB2(int x, int y)
{
    instancePtrVec[2]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[2]->mouseController.OnDrag();
}

void VPViewerGlutOGL::DragMouseCB3(int x, int y)
{
    instancePtrVec[3]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[3]->mouseController.OnDrag();
}

void VPViewerGlutOGL::DragMouseCB4(int x, int y)
{
    instancePtrVec[4]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[4]->mouseController.OnDrag();
}

void VPViewerGlutOGL::DragMouseCB5(int x, int y)
{
    instancePtrVec[5]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[5]->mouseController.OnDrag();
}

void VPViewerGlutOGL::HandleKey(unsigned char key)
{
    VPCamera* cameraPtr = ptScene->GetCurrentCamera();
    switch (key)
    {
        case 'a':
            cameraPtr->YawAroundTarget(0.01);
            break;
        case 'd':
            cameraPtr->YawAroundTarget(-0.01);
            break;
        case 'w':
            cameraPtr->PitchAroundTarget(0.01);
            break;
        case 's':
            cameraPtr->PitchAroundTarget(-0.01);
            break;
        case 'q':
            exit(0);
            break;
        default:
            return;
    }
    glutPostRedisplay();
}

void VPViewerGlutOGL::KeyboardCB0(unsigned char key, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[0]; // to easy copy/paste among resize CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VPViewerGlutOGL::KeyboardCB1(unsigned char key, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[1]; // to easy copy/paste among resize CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VPViewerGlutOGL::KeyboardCB2(unsigned char key, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[2]; // to easy copy/paste among resize CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VPViewerGlutOGL::KeyboardCB3(unsigned char key, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[3]; // to easy copy/paste among resize CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VPViewerGlutOGL::KeyboardCB4(unsigned char key, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[4]; // to easy copy/paste among resize CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VPViewerGlutOGL::KeyboardCB5(unsigned char key, int x, int y)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[5]; // to easy copy/paste among resize CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VPViewerGlutOGL::ResizeCB0(int newWidth, int newHeight)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[0]; // to easy copy/paste among resize CBs
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
    }
}

void VPViewerGlutOGL::ResizeCB1(int newWidth, int newHeight)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[1]; // to easy copy/paste among resize CBs
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
    }
}

void VPViewerGlutOGL::ResizeCB2(int newWidth, int newHeight)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[2]; // to easy copy/paste among resize CBs
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
    }
}

void VPViewerGlutOGL::ResizeCB3(int newWidth, int newHeight)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[3]; // to easy copy/paste among resize CBs
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
    }
}

void VPViewerGlutOGL::ResizeCB4(int newWidth, int newHeight)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[4]; // to easy copy/paste among resize CBs
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
    }
}

void VPViewerGlutOGL::ResizeCB5(int newWidth, int newHeight)
{
    VPViewerGlutOGL* viewerPtr = instancePtrVec[5]; // to easy copy/paste among resize CBs
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
    }
}

void VPViewerGlutOGL::Idle()
// Handles basic camera movment:
// LEFT: pitch and yaw around target.
// RIGHT: walk and yaw.
// RIGHT + CTRL: slide.
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    if (mouseController.AnyButtonIsDown() && autoNavigationEnabled)
    {
        VPCamera* ptCam = ptScene->GetCurrentCamera();
        if (mouseController.ButtonIsDown(VPMouseControl::LEFT))
        {
            double pitchAngle = mouseController.GetYDrag() * 0.00001;
            double yawAngle = mouseController.GetXDrag() * -0.00001;
            ptCam->PitchAroundTarget(pitchAngle);
            ptCam->YawAroundTarget(yawAngle);
        }
        else
        { // Consider RIGHT button is down
            if (mouseController.ModifierIsActive(VPMouseControl::CONTROL))
            {
                double Xdiff = mouseController.GetXDrag() * walkStep;
                double Ydiff = mouseController.GetYDrag() * (-walkStep);
                ptCam->MoveSideways(Xdiff);
                ptCam->MoveUp(Ydiff);
            }
            else
            {
                double Ydiff = mouseController.GetYDrag() * (-walkStep);
                double yawAngle = mouseController.GetXDrag() * -0.00001;
                ptCam->MoveForward(Ydiff);
                ptCam->Yaw(yawAngle);
            }
        }
        glutPostRedisplay();
    }
    if (idleHndPtr) { // if (idleHndPtr != NULL)
        idleHndPtr->OnIdle();
    }
    if (redrawOnIdle) {
        glutPostRedisplay();
    }
    glutSetWindow(currentWindow);
}

void VPViewerGlutOGL::IdleMngr()
{
    for (int i = 0; i < 6; ++i)
        if (instancePtrVec[i]) // if instancePtrVec[i] != NULL ...
            instancePtrVec[i]->Idle();
}

void VPViewerGlutOGL::RegisterCallbacks()
{
    int i = 0;
    while ((i<6) && (glutIDVec[i] != 0))
        ++i;
    if (i < 6)
    {
        glutIDVec[i] = glutID;
        instancePtrVec[i] = this;
        switch (i)
        {
            case 0:
                glutDisplayFunc(DrawCB0);
                glutMouseFunc(MouseCB0);
                glutMotionFunc(DragMouseCB0);
                glutKeyboardFunc(KeyboardCB0);
                glutReshapeFunc(ResizeCB0);
            break;
            case 1:
                glutDisplayFunc(DrawCB1);
                glutMouseFunc(MouseCB1);
                glutMotionFunc(DragMouseCB1);
                glutKeyboardFunc(KeyboardCB1);
                glutReshapeFunc(ResizeCB1);
            break;
            case 2:
                glutDisplayFunc(DrawCB2);
                glutMouseFunc(MouseCB2);
                glutMotionFunc(DragMouseCB2);
                glutKeyboardFunc(KeyboardCB2);
                glutReshapeFunc(ResizeCB2);
            break;
            case 3:
                glutDisplayFunc(DrawCB3);
                glutMouseFunc(MouseCB3);
                glutMotionFunc(DragMouseCB3);
                glutKeyboardFunc(KeyboardCB3);
                glutReshapeFunc(ResizeCB3);
            break;
            case 4:
                glutDisplayFunc(DrawCB4);
                glutMouseFunc(MouseCB4);
                glutMotionFunc(DragMouseCB4);
                glutKeyboardFunc(KeyboardCB4);
                glutReshapeFunc(ResizeCB4);
            break;
            default:
                glutDisplayFunc(DrawCB5);
                glutMouseFunc(MouseCB5);
                glutMotionFunc(DragMouseCB5);
                glutKeyboardFunc(KeyboardCB5);
                glutReshapeFunc(ResizeCB5);
            break;
        }
    }
}

void VPViewerGlutOGL::Hide()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutHideWindow();
    glutSetWindow(currentWindow);
}

void VPViewerGlutOGL::Show()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutShowWindow();
    glutSetWindow(currentWindow);
}

void VPViewerGlutOGL::SetTitle(const std::string& newTitle)
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutSetWindowTitle(newTitle.c_str());
    glutSetIconTitle(newTitle.c_str());
    glutSetWindow(currentWindow);
}

void VPViewerGlutOGL::Iconify()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutIconifyWindow();
    glutSetWindow(currentWindow);
}

void VPViewerGlutOGL::PostRedisplay()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutPostRedisplay();
    glutSetWindow(currentWindow);
}

void VPViewerGlutOGL::SetKbHandler(KbHandler* ptrKbH)
{
    kbHandlerPtr = ptrKbH;
    ptrKbH->viewerPtr = this;
}

void VPViewerGlutOGL::SetClickHandler(VPMouseControl::ClickHandler* newCHPtr)
{
    mouseController.SetClickHandler(newCHPtr);
}

void VPViewerGlutOGL::SetDragHandler(VPMouseControl::DragHandler* newDHPtr)
{
    mouseController.SetDragHandler(newDHPtr);
}

void VPViewerGlutOGL::SetMotionHandler(VPMouseControl::MotionHandler* newMHPtr)
{
    mouseController.SetMotionHandler(newMHPtr);
}
