/// \file viewerglutogl.cpp
/// \brief Implementation file for V-ART class "ViewerGlutOGL".
/// \version $Revision: 1.3 $

#include "vart/contrib/viewerglutogl.h"
#include <cassert>
#include <GL/glut.h>

#include <iostream> // FixMe: Remove me if possible.
#include <cstdlib>

using namespace std;

// INITIALIZATION OF STATIC ATRIBUTES
int VART::ViewerGlutOGL::glutIDVec[6] = { 0, 0, 0, 0, 0, 0 };
VART::ViewerGlutOGL* VART::ViewerGlutOGL::instancePtrVec[6] = { NULL, NULL, NULL, NULL, NULL, NULL };

// FixMe: Add other constructors to avoid SetPosition.

VART::ViewerGlutOGL::ViewerGlutOGL()
{
    glutInitWindowSize(300,300);
    width = 300;
    height = 300;
    CommonConstructor();
}

VART::ViewerGlutOGL::ViewerGlutOGL(int newWidth, int newHeight)
{
    glutInitWindowSize(newWidth, newHeight);
    width = newWidth;
    height = newHeight;
    CommonConstructor();
}

void VART::ViewerGlutOGL::CommonConstructor()
{
    glutInitWindowPosition(0,0);
    glutID = glutCreateWindow("V-ART Glut OpenGL Viewer");
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

VART::ViewerGlutOGL::~ViewerGlutOGL()
{
    int i = 0;
    while (glutIDVec[i] != glutID)
        ++i;
    glutIDVec[i] = 0;
    instancePtrVec[i] = NULL;
}

void VART::ViewerGlutOGL::SetIdleHandler(IdleHandler* newIHPtr)
{
    idleHndPtr = newIHPtr;
    if (newIHPtr)
        newIHPtr->viewerPtr = this;
}

void VART::ViewerGlutOGL::SetPosition(int x, int y)
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutPositionWindow(x,y);
    glutSetWindow(currentWindow);
}

void VART::ViewerGlutOGL::SetSize(int newWidth, int newHeight)
{
    int currentWindow = glutGetWindow(); // save current window
    if (autoChangeCameraAspect && ptScene)
    {
        ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
        float horScale = static_cast<float>(newWidth)/width;
        float verScale = static_cast<float>(newHeight)/height;
        ptScene->ChangeAllCamerasViewVolume(horScale,verScale);
    }
    width = newWidth;
    height = newHeight;
    glutSetWindow(glutID);
    glutReshapeWindow(newWidth, newHeight);
    glutSetWindow(currentWindow); // restore current window
    //~ glViewport(0,0,newWidth,newHeight);
}

void VART::ViewerGlutOGL::TurnIntoCurrentWindow()
{
    glutSetWindow(glutID);
}

void VART::ViewerGlutOGL::SetDrawHandler(DrawHandler* newDHPtr)
{
    drawHandlerPtr = newDHPtr;
    if (newDHPtr)
        newDHPtr->viewerPtr = this;
}

void VART::ViewerGlutOGL::SetScene(VART::Scene& scene)
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

void VART::ViewerGlutOGL::ClearOGLBuffers()
{
    static float bgColor[4];
    ptScene->GetBackgroundColor().GetScaled(1.0f, bgColor); // convert color components to float
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void VART::ViewerGlutOGL::MainLoop()
// static method
{
    glutIdleFunc(IdleMngr);
    glutMainLoop();
}

void VART::ViewerGlutOGL::Init(int* argcPtr, char* argv[])
// static method
{
    glutInit(argcPtr, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
}

void VART::ViewerGlutOGL::DrawCB0()
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
        cerr << "Fatal error: VART::ViewerGlutOGL tried to draw a scene pointed by NULL!" << endl;
        exit(1);
    }
    instancePtrVec[0]->ClearOGLBuffers();
    if (!(instancePtrVec[0]->ptScene->DrawOGL()))
    {
        cerr << "Fatal error: VART::Scene::DrawOGL returned false!\n\a"
             << "This means that V-ART has been compiled with no underlying rendering "
             << "engine, please check your project options and re-compile V-ART." << endl;
        exit(1);
    }
#endif
    if (instancePtrVec[0]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[0]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VART::ViewerGlutOGL::DrawCB1()
{
    //~ assert(glutGetWindow() == instancePtrVec[1]->glutID);
    instancePtrVec[1]->ClearOGLBuffers();
    instancePtrVec[1]->ptScene->DrawOGL();
    if (instancePtrVec[1]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[1]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VART::ViewerGlutOGL::DrawCB2()
{
    //~ assert(glutGetWindow() == instancePtrVec[2]->glutID);
    instancePtrVec[2]->ClearOGLBuffers();
    instancePtrVec[2]->ptScene->DrawOGL();
    if (instancePtrVec[2]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[2]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VART::ViewerGlutOGL::DrawCB3()
{
    instancePtrVec[3]->ClearOGLBuffers();
    instancePtrVec[3]->ptScene->DrawOGL();
    if (instancePtrVec[3]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[3]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VART::ViewerGlutOGL::DrawCB4()
{
    instancePtrVec[4]->ClearOGLBuffers();
    instancePtrVec[4]->ptScene->DrawOGL();
    if (instancePtrVec[4]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[4]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VART::ViewerGlutOGL::DrawCB5()
{
    instancePtrVec[5]->ClearOGLBuffers();
    instancePtrVec[5]->ptScene->DrawOGL();
    if (instancePtrVec[5]->drawHandlerPtr) // if (drawHandlerPtr != NULL)
        instancePtrVec[5]->drawHandlerPtr->OnDraw();
    glutSwapBuffers();
}

void VART::ViewerGlutOGL::MouseCB0(int button, int state, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[0];
    int keyModifier = glutGetModifiers();
    VART::MouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VART::MouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VART::MouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VART::MouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VART::MouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VART::MouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VART::ViewerGlutOGL::MouseCB1(int button, int state, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[1];
    int keyModifier = glutGetModifiers();
    VART::MouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VART::MouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VART::MouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VART::MouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VART::MouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VART::MouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VART::ViewerGlutOGL::MouseCB2(int button, int state, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[2];
    int keyModifier = glutGetModifiers();
    VART::MouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VART::MouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VART::MouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VART::MouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VART::MouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VART::MouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VART::ViewerGlutOGL::MouseCB3(int button, int state, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[3];
    int keyModifier = glutGetModifiers();
    VART::MouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VART::MouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VART::MouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VART::MouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VART::MouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VART::MouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VART::ViewerGlutOGL::MouseCB4(int button, int state, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[4];
    int keyModifier = glutGetModifiers();
    VART::MouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VART::MouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VART::MouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VART::MouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VART::MouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VART::MouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VART::ViewerGlutOGL::MouseCB5(int button, int state, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[5];
    int keyModifier = glutGetModifiers();
    VART::MouseControl::ButtonID vpButton;

    switch (button)
    {
        //FixMe: There may be a faster way...
        case GLUT_LEFT_BUTTON:
            vpButton = VART::MouseControl::LEFT;
            break;
        case GLUT_MIDDLE_BUTTON:
            vpButton = VART::MouseControl::MIDDLE;
            break;
        case GLUT_RIGHT_BUTTON:
            vpButton = VART::MouseControl::RIGHT;
            break;
        case 3: // FixMe: use the GLUT constant!
            vpButton = VART::MouseControl::WHEEL_UP;
            break;
        default:
            vpButton = VART::MouseControl::WHEEL_DOWN;
    }
    if (state == GLUT_DOWN)
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::DOWN);
        viewerPtr->mouseController.SetClickPosition(x, y);
    }
    else
    {
        viewerPtr->mouseController.NewEvent(vpButton, VART::MouseControl::UP);
        viewerPtr->mouseController.SetReleasedPosition(x, y);
    }
    if (keyModifier & GLUT_ACTIVE_SHIFT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::SHIFT, false);
    if (keyModifier & GLUT_ACTIVE_CTRL)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::CONTROL, false);
    if (keyModifier & GLUT_ACTIVE_ALT)
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, true);
    else
        viewerPtr->mouseController.SetModifierState(VART::MouseControl::ALT, false);
    viewerPtr->mouseController.SetCurrentPosititon(x,y);
    viewerPtr->mouseController.OnClick();
}

void VART::ViewerGlutOGL::DragMouseCB0(int x, int y)
{
    instancePtrVec[0]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[0]->mouseController.OnDrag();
}

void VART::ViewerGlutOGL::DragMouseCB1(int x, int y)
{
    instancePtrVec[1]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[1]->mouseController.OnDrag();
}

void VART::ViewerGlutOGL::DragMouseCB2(int x, int y)
{
    instancePtrVec[2]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[2]->mouseController.OnDrag();
}

void VART::ViewerGlutOGL::DragMouseCB3(int x, int y)
{
    instancePtrVec[3]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[3]->mouseController.OnDrag();
}

void VART::ViewerGlutOGL::DragMouseCB4(int x, int y)
{
    instancePtrVec[4]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[4]->mouseController.OnDrag();
}

void VART::ViewerGlutOGL::DragMouseCB5(int x, int y)
{
    instancePtrVec[5]->mouseController.SetCurrentPosititon(x,y);
    instancePtrVec[5]->mouseController.OnDrag();
}

void VART::ViewerGlutOGL::HandleKey(unsigned char key)
{
    VART::Camera* cameraPtr = ptScene->GetCurrentCamera();
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

void VART::ViewerGlutOGL::KeyboardCB0(unsigned char key, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[0]; // to ease copy/paste among keyboard CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VART::ViewerGlutOGL::KeyboardCB1(unsigned char key, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[1]; // to ease copy/paste among keyboard CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VART::ViewerGlutOGL::KeyboardCB2(unsigned char key, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[2]; // to ease copy/paste among keyboard CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VART::ViewerGlutOGL::KeyboardCB3(unsigned char key, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[3]; // to ease copy/paste among keyboard CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VART::ViewerGlutOGL::KeyboardCB4(unsigned char key, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[4]; // to ease copy/paste among keyboard CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VART::ViewerGlutOGL::KeyboardCB5(unsigned char key, int x, int y)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[5]; // to ease copy/paste among keyboard CBs
    // Call keyboard handler
    if (viewerPtr->kbHandlerPtr)
        viewerPtr->kbHandlerPtr->HandleKey(key);
    // Viewer's built-in keyboard handling
    if (viewerPtr->autoRespondKeys)
        viewerPtr->HandleKey(key);
}

void VART::ViewerGlutOGL::ResizeCB0(int newWidth, int newHeight)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[0]; // to ease copy/paste among resize CBs
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
        float horScale = static_cast<float>(newWidth)/viewerPtr->width;
        float verScale = static_cast<float>(newHeight)/viewerPtr->height;
        viewerPtr->ptScene->ChangeAllCamerasViewVolume(horScale,verScale);
    }
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
}

void VART::ViewerGlutOGL::ResizeCB1(int newWidth, int newHeight)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[1]; // to ease copy/paste among resize CBs
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
        float horScale = static_cast<float>(newWidth)/viewerPtr->width;
        float verScale = static_cast<float>(newHeight)/viewerPtr->height;
        viewerPtr->ptScene->ChangeAllCamerasViewVolume(horScale,verScale);
    }
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
}

void VART::ViewerGlutOGL::ResizeCB2(int newWidth, int newHeight)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[2]; // to ease copy/paste among resize CBs
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
        float horScale = static_cast<float>(newWidth)/viewerPtr->width;
        float verScale = static_cast<float>(newHeight)/viewerPtr->height;
        viewerPtr->ptScene->ChangeAllCamerasViewVolume(horScale,verScale);
    }
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
}

void VART::ViewerGlutOGL::ResizeCB3(int newWidth, int newHeight)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[3]; // to ease copy/paste among resize CBs
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
        float horScale = static_cast<float>(newWidth)/viewerPtr->width;
        float verScale = static_cast<float>(newHeight)/viewerPtr->height;
        viewerPtr->ptScene->ChangeAllCamerasViewVolume(horScale,verScale);
    }
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
}

void VART::ViewerGlutOGL::ResizeCB4(int newWidth, int newHeight)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[4]; // to ease copy/paste among resize CBs
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
        float horScale = static_cast<float>(newWidth)/viewerPtr->width;
        float verScale = static_cast<float>(newHeight)/viewerPtr->height;
        viewerPtr->ptScene->ChangeAllCamerasViewVolume(horScale,verScale);
    }
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
}

void VART::ViewerGlutOGL::ResizeCB5(int newWidth, int newHeight)
{
    VART::ViewerGlutOGL* viewerPtr = instancePtrVec[5]; // to ease copy/paste among resize CBs
    if (viewerPtr->autoChangeCameraAspect) {
        assert(viewerPtr->ptScene != NULL);
        viewerPtr->ptScene->SetAllCamerasAspectRatio(static_cast<float>(newWidth)/newHeight);
        float horScale = static_cast<float>(newWidth)/viewerPtr->width;
        float verScale = static_cast<float>(newHeight)/viewerPtr->height;
        viewerPtr->ptScene->ChangeAllCamerasViewVolume(horScale,verScale);
    }
    viewerPtr->width = newWidth;
    viewerPtr->height = newHeight;
    glViewport(0,0,newWidth,newHeight);
}

void VART::ViewerGlutOGL::Idle()
// Handles basic camera movment:
// LEFT: pitch and yaw around target.
// RIGHT: walk and yaw.
// RIGHT + CTRL: slide.
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    if (mouseController.AnyButtonIsDown() && autoNavigationEnabled)
    {
        VART::Camera* ptCam = ptScene->GetCurrentCamera();
        if (mouseController.ButtonIsDown(VART::MouseControl::LEFT))
        {
            double pitchAngle = mouseController.GetYDrag() * 0.00001;
            double yawAngle = mouseController.GetXDrag() * -0.00001;
            ptCam->PitchAroundTarget(pitchAngle);
            ptCam->YawAroundTarget(yawAngle);
        }
        else
        { // Consider RIGHT button is down
            if (mouseController.ModifierIsActive(VART::MouseControl::CONTROL))
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

void VART::ViewerGlutOGL::IdleMngr()
{
    for (int i = 0; i < 6; ++i)
        if (instancePtrVec[i]) // if instancePtrVec[i] != NULL ...
            instancePtrVec[i]->Idle();
}

void VART::ViewerGlutOGL::RegisterCallbacks()
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

void VART::ViewerGlutOGL::Hide()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutHideWindow();
    glutSetWindow(currentWindow);
}

void VART::ViewerGlutOGL::Show()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutShowWindow();
    glutSetWindow(currentWindow);
}

void VART::ViewerGlutOGL::SetTitle(const std::string& newTitle)
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutSetWindowTitle(newTitle.c_str());
    glutSetIconTitle(newTitle.c_str());
    glutSetWindow(currentWindow);
}

void VART::ViewerGlutOGL::Iconify()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutIconifyWindow();
    glutSetWindow(currentWindow);
}

void VART::ViewerGlutOGL::PostRedisplay()
{
    int currentWindow = glutGetWindow();
    glutSetWindow(glutID);
    glutPostRedisplay();
    glutSetWindow(currentWindow);
}

void VART::ViewerGlutOGL::SetKbHandler(KbHandler* ptrKbH)
{
    kbHandlerPtr = ptrKbH;
    ptrKbH->viewerPtr = this;
}

void VART::ViewerGlutOGL::SetClickHandler(VART::MouseControl::ClickHandler* newCHPtr)
{
    mouseController.SetClickHandler(newCHPtr);
}

void VART::ViewerGlutOGL::SetDragHandler(VART::MouseControl::DragHandler* newDHPtr)
{
    mouseController.SetDragHandler(newDHPtr);
}

void VART::ViewerGlutOGL::SetMotionHandler(VART::MouseControl::MotionHandler* newMHPtr)
{
    mouseController.SetMotionHandler(newMHPtr);
}
