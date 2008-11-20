//XML action file generator

// Feb 23, 2007 - Leonardo Garcia Fischer
//- Added the 'system("pause")', for compilation under windows.
// Jan 19, 2007 - Leonardo Garcia Fischer
//- Added include files uniaxialjoint.h, biaxialjoint.h, polyaxialjoint.h
//- Initialized nameAct, to avoid errors in glui
//- Some of callbacks are registered with glui now, due to recommendations of glui manual
// Oct 20, 2006 - Leonardo Garcia Fischer
//- Added definition of NOMINMAX and include file windows.h, for compilation under Windows environment
// Sep 18, 2006 - Andreia Schneider
//- Added mouse function. Now joints are selected by mouse click.
// Mar 31, 2006 - Andreia Schneider
//- Application implemented

#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <glui.h>
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <limits>

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/xmlscene.h>
#include <vart/sgpath.h>
#include <vart/uniaxialjoint.h>
#include <vart/biaxialjoint.h>
#include <vart/polyaxialjoint.h>

#include "doftime.h"
#include "jointtime.h"

using namespace std;
using namespace VART;

namespace global
{
    bool openFile = false;
    ofstream* outXml = NULL;
}

//These are the live variables passed into GLUI
int main_window;
int dofId = 0;
int jointID = 0;
int cycleID = 0;
int interID = 0;
int j;

static char nameAct[sizeof (GLUI_String)] = "";

float nameSpeed;
float dur;
string dofType;
string fileName;

//Glui interface Objects
GLUI* glui = NULL; 

GLUI_Translation* flexControl= NULL; 
GLUI_Translation* adductControl= NULL; 
GLUI_Translation* twistControl= NULL; 

GLUI_EditText* finFlex= NULL;
GLUI_EditText* finAdduct= NULL;
GLUI_EditText* finTwist= NULL;
GLUI_EditText* action_name= NULL;
GLUI_EditText* speed= NULL;
GLUI_EditText* duration= NULL;
GLUI_EditText* initialTimeFlex= NULL;
GLUI_EditText* finalTimeFlex= NULL;
GLUI_EditText* initialTimeAdduct= NULL;
GLUI_EditText* finalTimeAdduct= NULL;
GLUI_EditText* initialTimeTwist= NULL;
GLUI_EditText* finalTimeTwist= NULL;
GLUI_StaticText* jointSel;
GLUI_StaticText* textMouse;
GLUI_Listbox* interpolation= NULL;
GLUI_Listbox* cycle= NULL;

GLUI_Button* restJointB= NULL;
GLUI_Button* restBodyB= NULL;
GLUI_Button* finishB= NULL;
GLUI_Button* resetCam= NULL;

//Glui interface Variables
float cameraspinner[2] = { 0,0 };
float cameraTransXY[2] = { 0,0 };
float cameraTransZ[1] = { 0 };

float flexTrans[1] = { 0 };
float adductTrans[1] = { 0 };
float twistTrans[1] = { 0 };
float tIFlex;
float tFFlex;
float tIAdd;
float tFAdd;
float tITw;
float tFTw;

bool jointExist = false;
bool dofFlexExist = false;
bool dofAdductExist = false;
bool dofTwistExist = false;
bool jointSelected = false;

vector<string> statusCycleV(3);
vector<string> statusInterp;


JointTime* jPtr = NULL;
DofTime* dPtrFlex = NULL;
DofTime* dPtrAdduc = NULL;
DofTime* dPtrTwist = NULL;
//list<Joint*>    jointL;
list<DofTime*>   dofTimeL;
list<JointTime*> jointXml;
list<JointTime*>::iterator iterJ;
list<DofTime*>::iterator iterD;

//Vpat Scenario variables
Camera camera(Point4D(2.5,-17,5,1),Point4D(0,0,0.05,1),Point4D(0,0,1,0));
XmlScene* scenePtr;
SceneNode* articulatedBodyPtr;
GraphicObj* selectedObjectPtr = NULL;
Joint* selectedJointPtr= NULL;
SceneNode*      node;
UniaxialJoint*  unijoint  = NULL;
BiaxialJoint*   bijoint   = NULL;
PolyaxialJoint* polyjoint = NULL;
Joint*          jointRest = NULL;

//Open the xml file to write
void xmlCreator()
{
    if (!global::outXml)
    {
        cerr<<"Error. Couldn't open file"<<endl;
        exit(1);
    }
    else
    {
        //xml header
        global::outXml->setf(ios::fixed);
        global::outXml->precision(4);
        (*global::outXml)<<"<?xml version="<<'"'<<"1.0"<<'"'<<"?>"<<'\n';
        (*global::outXml)<<"<!DOCTYPE action SYSTEM "<<'"'<<"vpatAnimation.dtd"<<'"'<<'>'<<'\n';
        global::openFile = true;
    };
}

void myGlutIdle( void )
{
    if ( glutGetWindow() != main_window ) 
        glutSetWindow(main_window);  

    glutPostRedisplay();
}

void myReshape( int x, int y )
{
    int tx, ty, tw, th;

    float xy_aspect;

    xy_aspect = (float)x / (float)y;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    scenePtr->SetAllCamerasAspectRatio(static_cast<float>(tw)/th);
    GLUI_Master.auto_set_viewport();

    glutPostRedisplay();
}

void myDisplay( void )
{
    //glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scenePtr->DrawOGL(); 
    glutSwapBuffers();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    switch(key)     
    {
        case 'q':exit(EXIT_SUCCESS);   
        break;
    }
    glutPostRedisplay();
}


void ControleGlui(int call)
{
    static float x1=0, y1=0;
    static float x2=0, y2=0;
    static float z2=0;
    static float lastFlex=0, lastAdduct=0, lastTwist=0;
    
    jointExist = false;
    dofFlexExist = false;
    dofAdductExist = false;
    dofTwistExist = false;

    Dof d;

    Camera* cameraPtr = scenePtr->GetCurrentCamera();

    list<SceneNode*> nodeList = scenePtr->GetAllSceneJoints();
    list<SceneNode*>::const_iterator iter;

    switch( call )
    {
        case 1: //Camera Control
            cameraPtr->YawAroundTarget( (x1-cameraspinner[0])/100);
            cameraPtr->PitchAroundTarget( (y1-cameraspinner[1])/100);
            x1 = cameraspinner[0];
            y1 = cameraspinner[1];
            cout<<"Camera"<<*cameraPtr<<endl;
        break; 

        case 2: //Camera Control
            cameraPtr->MoveSideways( (x2 - cameraTransXY[0])/10);
            cameraPtr->MoveUp((y2 - cameraTransXY[1])/10);
            x2 = cameraTransXY[0];
            y2 = cameraTransXY[1];
            cout<<"Camera"<<*cameraPtr<<endl;
        break; 

        case 3: //Camera Control
            cameraPtr->MoveForward( (z2 - cameraTransZ[0])/10);
            z2 = cameraTransZ[0];
            cout<<"Camera"<<*cameraPtr<<endl;
        break; 

        case 4: //set the joint which will move
            cycle->disable();
            duration->enable();
            //jointL.push_back(selectedJointPtr);
            flexControl->disable();
            adductControl->disable();
            twistControl->disable();
            finFlex->disable();
            finAdduct->disable();
            finTwist->disable();
            initialTimeAdduct->disable();
            initialTimeFlex->disable();
            initialTimeTwist->disable();
            finalTimeAdduct->disable();
            finalTimeFlex->disable();
            finalTimeTwist->disable();
            break;

        case 5: //move flex dof
            switch( selectedJointPtr->GetNumDofs() ){
                case 1:
                    unijoint->Flex((flexTrans[0]-lastFlex)/1000);
                    break;
                case 2:
                    bijoint->Flex((flexTrans[0]-lastFlex)/1000);
                    break;
                case 3:
                    polyjoint->Flex((flexTrans[0]-lastFlex)/1000);
                    break;
            }
            d = selectedJointPtr->GetDof( Joint::FLEXION );
            //set final position of xml dof.
            finFlex->set_float_val( d.GetCurrent());
            lastFlex = flexTrans[0];
            break;

        case 6://move adduct dof
            switch( selectedJointPtr->GetNumDofs() ){
                case 2:
                    bijoint->Adduct((adductTrans[0]-lastAdduct)/1000);
                    break;
                case 3:
                    polyjoint->Adduct((adductTrans[0]-lastAdduct)/1000);
                    break;
            }
            d = selectedJointPtr->GetDof( Joint::ADDUCTION );
            
            //set final position of xml dof.
            finAdduct->set_float_val( d.GetCurrent());
            lastAdduct = adductTrans[0];
        break;

        case 7://Move twist dof
            polyjoint->Twist((twistTrans[0]-lastTwist)/1000);
            d = selectedJointPtr->GetDof( Joint::TWIST );

            //set final position of xml dof.
            finTwist->set_float_val( d.GetCurrent());
            lastTwist = twistTrans[0];
        break;

        case 8: //Put a given joint in a rest position
            selectedJointPtr->SetAtRest();
            switch( selectedJointPtr->GetNumDofs() )
            {
                case 1: //If flex
                    d = selectedJointPtr->GetDof( Joint::FLEXION );
                    finFlex->set_float_val( d.GetCurrent());
                break;
                
                case 2: //If Adduct
                    d = selectedJointPtr->GetDof( Joint::FLEXION );
                    finFlex->set_float_val( d.GetCurrent());
                    d = selectedJointPtr->GetDof( Joint::ADDUCTION );
                    finAdduct->set_float_val( d.GetCurrent());
                break;
                
                case 3: //If twist
                    d = selectedJointPtr->GetDof( Joint::FLEXION );
                    finFlex->set_float_val( d.GetCurrent());
                    d = selectedJointPtr->GetDof( Joint::ADDUCTION );
                    finAdduct->set_float_val( d.GetCurrent());
                    d = selectedJointPtr->GetDof( Joint::TWIST );
                    finTwist->set_float_val( d.GetCurrent());
                break;
            }
        break;

        case 9: //Put the body in a rest position
            iter = nodeList.begin();
            while (iter != nodeList.end())
            {
                node = scenePtr->GetObjectRec((*iter)->GetDescription());
                jointRest = dynamic_cast <Joint*>(node);
                ++iter;
                jointRest->SetAtRest();
            }
            //jointL.clear();
            dofTimeL.clear();
            jointXml.clear();
            jointSelected = true;
        break;

        case 10: //Create the xml filename and enable speed box
            fileName = nameAct;
            fileName+=".xml";
            speed->enable();

        break;

        case 11: //add speed in XML file and enable cycle box
            action_name->disable();
            cycle->enable();
            jointSelected = true;
            
        break;

        case 12: //enable list of joints
            speed->disable();
            statusCycleV[0] = "--status--";
            statusCycleV[1] = "true";
            statusCycleV[2] = "false";
            textMouse->set_text("Click the mouse on the model to select a joint");
        break;

        case 13: //enable list of interpolations
            interpolation->delete_item(0);
            interpolation->delete_item(1);
            interpolation->delete_item(2);
            interpolation->delete_item(3);
        
            interpolation->add_item(0, "--status--");
            interpolation->add_item(1, "ease-in_ease-out");
            interpolation->add_item(2, "linear");
            interpolation->add_item(3,"range_sine");
            interpolation->enable();
        break;
        
        //Manage the interpolation, disable duration and call a method...
        //...that enable the dofs controls
        case 14:
            duration->disable();
            statusInterp.push_back("status");
            statusInterp.push_back("ease-in_ease-out");
            statusInterp.push_back("linear");
            statusInterp.push_back("range_sine");
            if (interID != 0)
            {
                for(iterJ = jointXml.begin(); iterJ != jointXml.end(); ++iterJ)
                {
                    if(selectedJointPtr->GetDescription() == (*iterJ)->GetJoint()->GetDescription())
                    {
                        (*iterJ)->SetDuration(duration->get_float_val());
                        (*iterJ)->SetInterpolation(statusInterp[interID]);
                        jointExist = true;
                    }
                }
                if (jointExist == false)
                {
                    jPtr = new JointTime(duration->get_float_val(), statusInterp[interID], selectedJointPtr);
                    jointXml.push_back(jPtr);
                }
            }
            ControleGlui(16);
        break;

        case 15: //initialize the application with only one box enabled.

        cycle->add_item(0, "--cycle--");
        cycle->add_item(1, "true");
        cycle->add_item(2, "false");
        
            flexControl->disable(); 
            adductControl->disable(); 
            twistControl->disable(); 
            finFlex->disable(); 
            finAdduct->disable(); 
            finTwist->disable(); 
            speed->disable();
            cycle->disable();
            duration->disable();
            interpolation->disable();
            initialTimeFlex->disable();
            finalTimeFlex->disable();
            initialTimeAdduct->disable();
            finalTimeAdduct->disable();
            initialTimeTwist->disable();
            finalTimeTwist->disable();
            restBodyB->disable();
            finishB->disable();
        break;

        case 16://Set final DOFs
            
            interpolation->disable();

            restBodyB->enable();

            //Enable dofs control buttons
            switch( selectedJointPtr->GetNumDofs() )
            {
                case 1: //If flex
                    unijoint = dynamic_cast <UniaxialJoint*>(selectedJointPtr);
                    flexControl->enable();
                    d = selectedJointPtr->GetDof( Joint::FLEXION );
                    finFlex->enable();
                    finFlex->set_float_val( d.GetCurrent());
                    initialTimeFlex->enable();
                break;
                
                case 2: //If Adduct
                    bijoint = dynamic_cast <BiaxialJoint*>(selectedJointPtr);
                    flexControl->enable();
                    adductControl->enable();
                    d = selectedJointPtr->GetDof( Joint::FLEXION );
                    finFlex->enable();
                    finFlex->set_float_val( d.GetCurrent());
                    initialTimeFlex->enable();
                    d = selectedJointPtr->GetDof( Joint::ADDUCTION );
                    finAdduct->enable();
                    initialTimeAdduct->enable();
                    finAdduct->set_float_val( d.GetCurrent());
                break;
                
                case 3: //If twist
                    polyjoint = dynamic_cast <PolyaxialJoint*>(selectedJointPtr);
                    flexControl->enable();
                    adductControl->enable();
                    twistControl->enable();
                    d = selectedJointPtr->GetDof( Joint::FLEXION );
                    finFlex->enable();
                    finFlex->set_float_val( d.GetCurrent());
                    initialTimeFlex->enable();
                    initialTimeFlex->set_float_val(0.0);
                    d = selectedJointPtr->GetDof( Joint::ADDUCTION);
                    finAdduct->enable();
                    finAdduct->set_float_val( d.GetCurrent());
                    initialTimeAdduct->enable();
                    d = selectedJointPtr->GetDof( Joint::TWIST );
                    finTwist->enable();
                    finTwist->set_float_val( d.GetCurrent());
                    initialTimeTwist->enable();
                break;
            }
        break;

        case 17:
            finalTimeFlex->enable();
            //jointSelected = true;
        break;
        
        case 18:
            initialTimeFlex->disable();
            for (iterD = dofTimeL.begin(); iterD != dofTimeL.end(); ++iterD)
            {
                if (selectedJointPtr->GetDescription() == (*iterD)->GetJoint()->GetDescription() && (*iterD)->GetDofType() == "FLEX")
                {
                    (*iterD)->SetInitialTime(tIFlex);
                    (*iterD)->SetFinalTime(tFFlex);
                    dofFlexExist = true;
                }
            }
            if (dofFlexExist == false)
            {
                dPtrFlex = new DofTime(tIFlex, tFFlex, "FLEX", selectedJointPtr);
                dofTimeL.push_back(dPtrFlex);
            }
            finFlex->disable();
            finishB->enable();
            textMouse->set_text("You may use the mouse to select another joint");
            jointSelected = true;
        break;
        
        case 19:
            finalTimeAdduct->enable();
        break;
        
        case 20:
            initialTimeAdduct->disable();
            for (iterD = dofTimeL.begin(); iterD != dofTimeL.end(); ++iterD)
            {
                if (selectedJointPtr->GetDescription() == (*iterD)->GetJoint()->GetDescription() && (*iterD)->GetDofType() == "ADDUCT")
                {
                    (*iterD)->SetInitialTime(tIAdd);
                    (*iterD)->SetFinalTime(tFAdd);
                    dofAdductExist = true;
                }
            }
            if (dofAdductExist == false)
            {
                dPtrAdduc = new DofTime(tIAdd, tFAdd, "ADDUCT", selectedJointPtr);
                dofTimeL.push_back(dPtrAdduc);
            }
            finAdduct->disable();
            finishB->enable();
            textMouse->set_text("You may use the mouse to select another joint");
            jointSelected = true;
        break;
        
        case 21:
            finalTimeTwist->enable();
        break;
        
        case 22:
            initialTimeTwist->disable();
            for (iterD = dofTimeL.begin(); iterD != dofTimeL.end(); ++iterD)
            {
                if (selectedJointPtr->GetDescription() == (*iterD)->GetJoint()->GetDescription() && (*iterD)->GetDofType() == "TWIST")
                {
                    (*iterD)->SetInitialTime(tITw);
                    (*iterD)->SetFinalTime(tFTw);
                    dofTwistExist = true;
                }
            }
            if (dofTwistExist == false)
            {
                dPtrTwist = new DofTime(tITw, tFTw, "TWIST", selectedJointPtr);
                dofTimeL.push_back(dPtrTwist);
            }
            finTwist->disable();
            finishB->enable();
            textMouse->set_text("You may use the mouse to select another joint");
            jointSelected = true;
        break;

        case 23: //Finish XML file
            global::outXml = new ofstream (fileName.c_str());
            if(global::openFile == false)
                xmlCreator();
            (*global::outXml)<<"<action action_name= "<<'"'<<nameAct<<'"'<<' ';
            (*global::outXml)<<"speed = "<<'"'<<speed->get_float_val()<<'"'<<' ';
            (*global::outXml)<<"cycle = "<<'"'<<statusCycleV[cycleID]<<'"'<<">"<<'\n';
            for(iterJ = jointXml.begin(); iterJ != jointXml.end(); ++iterJ)
            {
                (*global::outXml)<<"<joint_movement joint_name = "<<'"'<<(*iterJ)->GetJoint()->GetDescription()<<'"';
                (*global::outXml)<<" duration = "<<'"'<<(*iterJ)->GetDuration()<<'"'<<">"<<'\n';
                (*global::outXml)<<"<interpolation type= "<<'"'<<(*iterJ)->GetInterpolation()<<'"'<<"/>"<<'\n';
                for (iterD = dofTimeL.begin(); iterD != dofTimeL.end(); ++iterD)
                {
                    if((*iterJ)->GetJoint()->GetDescription() == (*iterD)->GetJoint()->GetDescription())
                    {
                        (*global::outXml)<<"<dof_movement dofID = "<<'"'<<(*iterD)->GetDofType()<<'"';
                        (*global::outXml)<<" initialTime = "<<'"'<<(*iterD)->GetInitialTime()<<'"';
                        (*global::outXml)<<" finalTime = "<<'"'<<(*iterD)->GetFinalTime()<<'"';
                        if((*iterD)->GetDofType() == "FLEX")
                            (*global::outXml)<<" finalPosition = "<<'"'<< (*iterJ)->GetJoint()->GetDof(Joint::FLEXION).GetCurrent()<<'"'<<"/>"<<'\n';
                        else
                        {
                            if ((*iterD)->GetDofType() == "ADDUCT")
                                (*global::outXml)<<" finalPosition = "<<'"'<< (*iterJ)->GetJoint()->GetDof(Joint::ADDUCTION).GetCurrent()<<'"'<<"/>"<<'\n';
                            else
                                (*global::outXml)<<" finalPosition = "<<'"'<< (*iterJ)->GetJoint()->GetDof(Joint::TWIST).GetCurrent()<<'"'<<"/>"<<'\n';
                        }
                    }
                }
                (*global::outXml)<<"</joint_movement>"<<"\n";
            }
            (*global::outXml)<<"</action>"<<'\n';
            global::outXml->close();
            if (jPtr != NULL)
                delete jPtr;
            if (dPtrFlex != NULL)
                delete dPtrFlex;
            if (dPtrAdduc != NULL)
                delete dPtrAdduc;
            if (dPtrTwist != NULL)
                delete dPtrTwist;
            delete global::outXml;
            exit(0);
        break;
        
        case 24:
            cameraPtr->SetTarget(Point4D (0,0,0.05,1));
            cameraPtr->SetLocation(Point4D(2.5,-17,5,1));
            cameraPtr->SetUp(Point4D(0,0,1,0));
        break;
    }
    glutSetWindow( main_window );
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
    list<VART::GraphicObj*> pickList;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && jointSelected)
    {
        jointSelected = false;
        textMouse->set_text("Joint selected!");
        scenePtr->Pick(x,y, &pickList);
        if (!pickList.empty())
        {
            VART::GraphicObj* nearestMeshPtr = NULL;
            VART::GraphicObj* tempMeshPtr;
            double nearestMeshDistance = numeric_limits<double>::max();
            double tempMeshDistance;
            VART::Point4D tempBBoxCenter;
            VART::SGPath pathToSelectedMesh;
            
            // Find selected meshes
            while (!pickList.empty()) 
            {
                tempMeshPtr = pickList.front();
                // choose from articulated body parts ONLY!
                if (articulatedBodyPtr->FindPathTo(tempMeshPtr, &pathToSelectedMesh))
                {
                    tempBBoxCenter = tempMeshPtr->GetBoundingBox().GetCenter();
                    tempMeshDistance = (tempBBoxCenter - camera.GetLocation()).Length();
                    if (tempMeshDistance < nearestMeshDistance) {
                        nearestMeshDistance = tempMeshDistance;
                        nearestMeshPtr = tempMeshPtr;
                    }
                }
                pickList.pop_front();
            }
            if (nearestMeshPtr) 
            {
                if (selectedObjectPtr) // on first selection, nothing to deselect
                    selectedObjectPtr->ToggleRecBBoxVisibility(); // deselect previous
                selectedObjectPtr = nearestMeshPtr;
                selectedObjectPtr->ToggleRecBBoxVisibility();
                // Find path again (may be lost because of farther objetcs)
                articulatedBodyPtr->FindPathTo(selectedObjectPtr, &pathToSelectedMesh);
                selectedJointPtr = pathToSelectedMesh.PointerToLastJoint();
                if (selectedJointPtr) 
                {
                    char* jointChar;
                    jointChar = const_cast <char*>(selectedJointPtr->GetDescription().c_str());
                    jointSel->set_text(jointChar);
                    ControleGlui(4);
                }
            }
        }
    }
}

/**************************************** main() ********************/
int main(int argc, char* argv[])
{
    static XmlScene scene;
    scenePtr = &scene;
    
    glutInit(&argc, argv);
    if(argc<=1)
    {
        cout<<"O arquivo xml deve ser passado!"<<endl;
#ifdef WIN32
        system("pause");
#endif
        return 1;
    }

    if (scene.LoadFromFile(argv[1]))
    {
        articulatedBodyPtr = scene.GetObjects().front();
        scene.AddLight(Light::BRIGHT_AMBIENT());
        scene.AddCamera(&camera);
        scene.DrawLightsOGL();

        //~ /****************************************/
        //~ /*   Initialize GLUT and create window  */
        //~ /****************************************/
    
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowPosition( 0, 50 );
        glutInitWindowSize( 1000, 600 );
        main_window = glutCreateWindow( "XML action file generator" );
    
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glutDisplayFunc(myDisplay);
    
        /****************************************/
        /*         Here's the GLUI code         */
        /****************************************/

        GLUI_Master.set_glutKeyboardFunc( KeyboardFunc );
        GLUI_Master.set_glutSpecialFunc( NULL );
        GLUI_Master.set_glutMouseFunc( MouseFunc );
        GLUI_Master.set_glutReshapeFunc( myReshape );
        GLUI_Master.set_glutIdleFunc( NULL );
    
        glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT );
        glui->set_main_gfx_window( main_window );
    
        //Controles de Navegação no universo
        GLUI_Panel* cameraPanel = glui->add_panel("Camera Controls");
        glui->add_translation_to_panel(cameraPanel, 
            "Rotations", GLUI_TRANSLATION_XY, cameraspinner, 1, ControleGlui);
        glui->add_column_to_panel(cameraPanel, false);
        glui->add_translation_to_panel( cameraPanel,
            "Up/Down and Sides",GLUI_TRANSLATION_XY, cameraTransXY, 2, ControleGlui);  
        glui->add_column_to_panel(cameraPanel, false);
        glui->add_translation_to_panel( cameraPanel,
            "Forward/Backward",GLUI_TRANSLATION_Z, cameraTransZ, 3, ControleGlui);
        glui->add_column_to_panel(cameraPanel, false);
        resetCam = glui->add_button_to_panel(cameraPanel,"reset Camera", 24, ControleGlui);

        //Variaveis da animacao
        GLUI_Panel* animPanel = glui->GLUI::add_panel("Animation Controls");
        GLUI_Panel* animcontrols =glui->add_panel_to_panel(animPanel,"animcontrols", GLUI_PANEL_NONE);
        action_name = glui->add_edittext_to_panel( animcontrols,"action_name:", GLUI_EDITTEXT_TEXT,nameAct,10, ControleGlui);
        glui->add_column_to_panel(animcontrols, false);
        speed = glui->add_edittext_to_panel( animcontrols,"speed:", GLUI_EDITTEXT_FLOAT, &nameSpeed, 11, ControleGlui);
        speed->set_float_val(1.0);
        glui->add_column_to_panel(animcontrols, false);
        cycle = glui->add_listbox_to_panel( animcontrols,"cycle:", &cycleID, 12, ControleGlui);
        GLUI_Panel* jointText =glui->add_panel_to_panel(animPanel,"", GLUI_PANEL_NONE);
        textMouse = glui->add_statictext_to_panel(jointText, " ");
        //joint motion controls
        GLUI_Panel* jointPanel = glui->add_panel_to_panel(animPanel, "Joint Controls");

        GLUI_Panel* jointcontrols = glui->add_panel_to_panel(jointPanel,"jointcontrols", GLUI_PANEL_NONE);

        flexControl   = glui->add_translation_to_panel( jointcontrols, 
                        "Flex"  , GLUI_TRANSLATION_X, flexTrans  , 5, ControleGlui);
        GLUI_StaticText* stext = glui->add_statictext_to_panel(jointcontrols,"");
        
        adductControl = glui->add_translation_to_panel( jointcontrols,
                        "Adduct", GLUI_TRANSLATION_X, adductTrans, 6, ControleGlui);
        
        twistControl  = glui->add_translation_to_panel( jointcontrols,
                        "Twist" , GLUI_TRANSLATION_X, twistTrans , 7, ControleGlui);

        glui->add_column_to_panel(jointcontrols, false);
        stext = glui->add_statictext_to_panel(jointcontrols,"");
        stext->set_h(25);
        finFlex = glui->add_edittext_to_panel(jointcontrols,"Final: ", GLUI_EDITTEXT_FLOAT);
        finFlex->set_w( 1 );

        stext =  glui->add_statictext_to_panel(jointcontrols,"");
        stext->set_h(65);

        finAdduct = glui->add_edittext_to_panel(jointcontrols,"Final: ", GLUI_EDITTEXT_FLOAT);
        finAdduct->set_w( 1 );

        stext = glui->add_statictext_to_panel(jointcontrols,"");
        stext->set_h(80);

        finTwist = glui->add_edittext_to_panel(jointcontrols,"Final: ", GLUI_EDITTEXT_FLOAT);
        finTwist->set_w( 1 );

        glui->add_column_to_panel(jointcontrols, false);
        stext = glui->add_statictext_to_panel(jointcontrols,"");
        stext->set_h(15);
        
        initialTimeFlex = glui->add_edittext_to_panel(jointcontrols,"initial time: ", GLUI_EDITTEXT_FLOAT, &tIFlex, 17, ControleGlui);
        finalTimeFlex = glui->add_edittext_to_panel(jointcontrols,"final time: ", GLUI_EDITTEXT_FLOAT, &tFFlex, 18, ControleGlui);

        stext = glui->add_statictext_to_panel(jointcontrols,"");
        stext->set_h(42);
        
        initialTimeAdduct = glui->add_edittext_to_panel(jointcontrols,"initial time: ", GLUI_EDITTEXT_FLOAT, &tIAdd, 19, ControleGlui);
        finalTimeAdduct = glui->add_edittext_to_panel(jointcontrols,"final time: ", GLUI_EDITTEXT_FLOAT, &tFAdd, 20, ControleGlui);

        stext = glui->add_statictext_to_panel(jointcontrols,"");
        stext->set_h(55);

        initialTimeTwist = glui->add_edittext_to_panel(jointcontrols,"initial time: ", GLUI_EDITTEXT_FLOAT, &tITw, 21, ControleGlui);
        finalTimeTwist = glui->add_edittext_to_panel(jointcontrols,"final time: ", GLUI_EDITTEXT_FLOAT, &tFTw, 22, ControleGlui);

        GLUI_Panel* listcontrols = glui->add_panel_to_panel(jointPanel,"listcontrols", GLUI_PANEL_NONE);
        glui->add_statictext_to_panel( jointPanel,"");
        duration = glui->add_edittext_to_panel( listcontrols,"duration (s):", GLUI_EDITTEXT_FLOAT, &dur, 13, ControleGlui);
        glui->add_column_to_panel(listcontrols, false);
        interpolation = glui->add_listbox_to_panel( listcontrols, "interpolation", &interID, 14, ControleGlui);
        glui->add_column_to_panel(listcontrols, false);
        stext = glui->add_statictext_to_panel(listcontrols,"Selected Joint:");
        jointSel = glui->add_statictext_to_panel(listcontrols,"None");

        GLUI_Panel* buttoncontrols = glui->add_panel_to_panel(jointPanel,"buttoncontrols", GLUI_PANEL_NONE);
        restJointB = glui->add_button_to_panel( buttoncontrols, "Reset Joint", 8, ControleGlui );
        glui->add_column_to_panel(buttoncontrols, false);

        finishB = glui->add_button_to_panel( buttoncontrols,"Finish XML file", 23, ControleGlui );
        glui->add_column_to_panel(buttoncontrols, false);
        restBodyB = glui->add_button_to_panel( buttoncontrols,"Reset Body", 9, ControleGlui );
        glui->set_main_gfx_window( main_window );
        //We register the idle callback with GLUI, *not* with GLUT
        GLUI_Master.set_glutIdleFunc( myGlutIdle ); 
        
        ControleGlui(15);
        
        interpolation->add_item(0, "--status--");
        interpolation->add_item(1, "ease-in_ease-out");
        interpolation->add_item(2, "linear");
        interpolation->add_item(3,"range_sine");

        glutMainLoop();
        return 0;
    }
#ifdef WIN32
    else
        system("pause");
#endif
}
