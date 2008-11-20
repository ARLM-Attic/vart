// main.cpp - Action Builder application

// Changelog
// Oct 30, 2008 - Bruno de Oliveira Schneider
// - Added new cameras.
// - Added experimental features for joint modifiers.
// - Ported actions to the new action classes.
// - Added feature: edit DOF.
// Jul 23, 2007 - Bruno de Oliveira Schneider
// - Added support for frame buffer export.
// Feb 23, 2007 - Bruno de Oliveira Schneider
// - Fixed aspected ratio bug. Added command to toggle background color. Added
//   support for printing more information. Added activation with timeout.
//   Added support for multiple cameras. More flexible with action argument.
// Feb 01, 2007 - Leonardo Garcia Fischer
// - Added the option to use the keys '[' and ']' instead of use WHELL_UP
//   and WHELL_DOWN
// Jan 23, 2007 - Leonardo Garcia Fischer
// - Added definition of NOMINMAX, for compilation under Windows environment
// Jan 19, 2007 - Bruno de Oliveira Schneider
// - Recovered from CVS version of 2006-11-23, because of data loss.
// Oct 09, 2006 - Bruno de Oliveira Schneider
// - Application created (from V-ART Poser application).

#ifdef WIN32
#define NOMINMAX
#endif
#include <vart/dof.h>
#include <vart/xmlscene.h>
#include <vart/light.h>
#include <vart/sgpath.h>
#include <vart/jointmover.h>
#include <vart/xmljointaction.h>
#include <vart/xmlprinter.h>
#include <vart/meshobject.h>
#include <vart/sineinterpolator.h>
#include <vart/graphicobj.h>
#include <vart/collector.h>
#include <vart/addresslocator.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/contrib/human.h>
#include <vart/contrib/pnmexporter.h>
#include "offsetmodifier.h"

#include <iostream>
#include <iterator> // for ostream_iterator
#include <limits> // STL numeric limits
#include <fstream>

using namespace std;

namespace global
{
    // AddKeyPose will add a joint mover for each changed joint
    list<VART::Joint*> changedJoints;
    list<VART::JointMover*> jointMovers;
    VART::XmlJointAction* actionPtr1 = NULL;
    VART::SineInterpolator interpolator;
    VART::Joint* selectedJointPtr = NULL;
    ofstream outFile;
    string fileName;
    VART::Collector<VART::Joint> jointCollector;
    VART::Human model1;
    VART::OffsetModifier tiredModifier;
    bool editingDOF = false; // true is the user is editing a DOF
    bool editingKeyPoses = false;
    VART::Point4D dofRotationAxis = VART::Point4D::X();
}

class IdleHandlerClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        IdleHandlerClass() {
        }
        virtual ~IdleHandlerClass() {};
        virtual void OnIdle() {
            if (VART::BaseAction::MoveAllActive() == 0) {
                viewerPtr->SetIdleHandler(NULL);
                viewerPtr->redrawOnIdle = false;
            }
            //~ cout << "+";
        }
};

// Define the click handler
class ClickHandlerClass : public VART::MouseControl::ClickHandler
{
    public:
        ClickHandlerClass() {
            selectedObjectPtr = NULL;
            selectedDof = VART::Joint::FLEXION;
        }
        virtual ~ClickHandlerClass() {};
        virtual void OnClick() {
            // left button down -> select joint
            if (mouseCtrlPtr->LastClickIsDown()) {
                VART::MouseControl::ButtonID lastButton = mouseCtrlPtr->GetLastClickButton();

                if (lastButton == VART::MouseControl::LEFT) {
                    scenePtr->Pick(mouseCtrlPtr->GetClickXPosition(),
                                   mouseCtrlPtr->GetClickYPosition(), &pickList);
                    if (!pickList.empty()) {
                        VART::GraphicObj* nearestMeshPtr = NULL;
                        VART::GraphicObj* tempMeshPtr;
                        double nearestMeshDistance = numeric_limits<double>::max();
                        double tempMeshDistance;
                        VART::Point4D tempBBoxCenter;

                        // Find selected meshes
                        //~ cout << "Matching objects are: ";
                        while (!pickList.empty()) {
                            tempMeshPtr = pickList.front();
                            //~ cout << "[" << tempMeshPtr->GetDescription() << "] ";
                            if (dynamic_cast<VART::Human*>(tempMeshPtr) == NULL)
                            { // the human as whole should be ignored
                                VART::AddressLocator locator(tempMeshPtr);
                                articulatedBodyPtr->LocateDepthFirst(&locator);
                                // choose from articulated body parts ONLY!
                                if (locator.Finished())
                                // if selected mesh is part of the human...
                                {
                                    //cout << tempMeshPtr->GetDescription() << " ";
                                    tempBBoxCenter = tempMeshPtr->GetBoundingBox().GetCenter();
                                    tempMeshDistance = (tempBBoxCenter - cameraPtr->GetLocation()).Length();
                                    if (tempMeshDistance < nearestMeshDistance) {
                                        nearestMeshDistance = tempMeshDistance;
                                        nearestMeshPtr = tempMeshPtr;
                                        pathToSelectedMesh = locator.Path();
                                    }
                                }
                            }
                            pickList.pop_front();
                        }
                        //~ cout << endl;
                        // At this point, nearestMeshPtr point to the nearest mesh that is part of
                        // the human (ignores the ground) or is NULL.
                        if (nearestMeshPtr) {
                            if (selectedObjectPtr) // on first selection, nothing to deselect
                                selectedObjectPtr->ToggleRecBBoxVisibility(); // deselect previous
                            selectedObjectPtr = nearestMeshPtr;
                            selectedObjectPtr->ToggleRecBBoxVisibility();
                            //~ cout << "Selected object is: " << selectedObjectPtr->GetDescription() << endl;
                            //~ cout << "Path to selected object is: " << flush << pathToSelectedMesh << endl;
                            global::selectedJointPtr = pathToSelectedMesh.PointerToLastJoint();
                            if (global::selectedJointPtr) {
                                cout << "Selected joint: " << global::selectedJointPtr->GetDescription() 
                                     << " (" << global::selectedJointPtr->GetNumDofs() 
                                     << " DOFs). Change selected DOF thru keys [1], [2] and [3]."
                                     << endl;
                            }
                            viewerPtr->PostRedisplay();
                        }
                    }
                }
            }
            else
            { // last click is UP
                // treat mouse wheel
                if ((global::selectedJointPtr) && 
                    (global::selectedJointPtr->HasDof(selectedDof)))
                {
                    if (global::editingDOF)
                    { // user is editing a DOF, use mouse wheel to change axis
                        if (mouseCtrlPtr->LastClickButtonIs(VART::MouseControl::WHEEL_DOWN))
                            RotateDofAxis(-0.1);
                        if (mouseCtrlPtr->LastClickButtonIs(VART::MouseControl::WHEEL_UP))
                            RotateDofAxis(0.1);
                    }
                    else
                    { // user is not editing a DOF, use mouse wheel to move it
                        float wheelRange = 0.002;
                        if (mouseCtrlPtr->ModifierIsActive(VART::MouseControl::SHIFT))
                            wheelRange = 0.00002;

                        bool needToEditDofMover = false;
                        if (mouseCtrlPtr->LastClickButtonIs(VART::MouseControl::WHEEL_DOWN)) {
                            global::selectedJointPtr->MoveDof(selectedDof, -wheelRange);
                            viewerPtr->PostRedisplay();
                            MarkJointAsChanged();
                            needToEditDofMover = global::editingKeyPoses;
                        }
                        if (mouseCtrlPtr->LastClickButtonIs(VART::MouseControl::WHEEL_UP)) {
                            global::selectedJointPtr->MoveDof(selectedDof, wheelRange);
                            viewerPtr->PostRedisplay();
                            MarkJointAsChanged();
                            needToEditDofMover = global::editingKeyPoses;
                        }
                        if (needToEditDofMover)
                        {
                            global::actionPtr1->SearchDofMover(global::selectedJointPtr, selectedDof);
                        }
                    }
                }
            }
        }
        void MarkJointAsChanged() {
            list<VART::Joint*>::iterator iter = global::changedJoints.begin();
            bool jointIsNotInList = true;
            while ((iter != global::changedJoints.end()) && jointIsNotInList) {
                if ((*iter) == global::selectedJointPtr)
                    jointIsNotInList = false;
                else
                    ++iter;
            }
            if (jointIsNotInList) {
                global::changedJoints.push_back(global::selectedJointPtr);
                cout << "Joint "<<global::selectedJointPtr->GetDescription()<<" marked as changed."<<endl;
            }
        }
        list<VART::GraphicObj*> pickList;
        VART::Scene* scenePtr;
        VART::GraphicObj* selectedObjectPtr;
        VART::Camera* cameraPtr;
        VART::SGPath pathToSelectedMesh;
        VART::SceneNode* articulatedBodyPtr;
        VART::Joint::DofID selectedDof;
    private:
        void RotateDofAxis(float radians)
        { // selected joint and DOF must be valid
            VART::Point4D axis = global::selectedJointPtr->GetDof(selectedDof).GetAxis();
            VART::Transform rotation;
            rotation.MakeRotation(global::dofRotationAxis, radians);
            VART::Dof& targetDof = const_cast<VART::Dof&>(global::selectedJointPtr->GetDof(selectedDof));
            targetDof.SetAxis(rotation * axis);
            viewerPtr->PostRedisplay();
        }
};

// Define the keyboard handler
class KeyboardHandler : public VART::ViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler() {
            keyPoses = 0;
        }
        virtual void HandleKey(unsigned char key) {
            VART::Camera* cameraPtr = NULL;
            bool cyclicStatus;
            static bool fileNotSaved = true;
            list<const VART::Joint*>::iterator iter;

            switch (key)
            {
                case '|': // vertically align camera
                    cameraPtr = scenePtr->GetCurrentCamera();
                    if (cameraPtr) {
                        VART::Point4D leftVec;
                        cameraPtr->LeftVector(&leftVec);
                        VART::Point4D desiredLeftVec = leftVec;
                        desiredLeftVec.SetZ(0);
                        desiredLeftVec.Normalize();
                        VART::Point4D crossVec = leftVec.CrossProduct(desiredLeftVec);
                        //cout << "left: " << leftVec << endl;
                        VART::Point4D frontVec;
                        cameraPtr->FrontVector(&frontVec);
                        if (crossVec.DotProduct(frontVec) > 0.0)
                            cameraPtr->Roll(0.005);
                        else
                            cameraPtr->Roll(-0.005);
                    }
                    //viewerPtr->PostRedisplay();
                    break;
                case ',':
                    cameraPtr = scenePtr->GetCurrentCamera();
                    if (cameraPtr)
                        cameraPtr->Roll(0.003);
                    viewerPtr->PostRedisplay();
                    break;
                case '.':
                    cameraPtr = scenePtr->GetCurrentCamera();
                    if (cameraPtr)
                        cameraPtr->Roll(-0.003);
                    viewerPtr->PostRedisplay();
                    break;
                case '\\': // change Dof rotation axis
                    if (global::dofRotationAxis == VART::Point4D::X()) {
                        global::dofRotationAxis = VART::Point4D::Y();
                        cout << "Now rotating around Y axis\n";
                    }
                    else {
                        if (global::dofRotationAxis == VART::Point4D::Y()) {
                            global::dofRotationAxis = VART::Point4D::Z();
                            cout << "Now rotating around Z axis\n";
                        }
                        else {
                            global::dofRotationAxis = VART::Point4D::X();
                            cout << "Now rotating around X axis\n";
                        }
                    }
                    break;
                case ' ': // reset current camera to default position
                    cameraPtr = scenePtr->GetCurrentCamera();
                    if (cameraPtr) {
                        defaultCamera.SetAspectRatio(cameraPtr->GetAspectRatio());
                        *cameraPtr = defaultCamera;
                        cout << "Camera reverted to default position." << endl;
                    }
                    viewerPtr->PostRedisplay();
                    break;
                case '1': // select FLEXION
                    clickHPtr->selectedDof = VART::Joint::FLEXION;
                    cout << "Now moving: FLEXION" << endl;
                    break;
                case '2':
                    if (global::selectedJointPtr->HasDof(VART::Joint::ADDUCTION))
                    {
                        clickHPtr->selectedDof = VART::Joint::ADDUCTION;
                        cout << "Now moving: ADDUCTION" << endl;
                    }
                    else
                        cout << "No such DOF: ADDUCTION" << endl;
                    break;
                case '3':
                    if (global::selectedJointPtr->HasDof(VART::Joint::TWIST))
                    {
                        clickHPtr->selectedDof = VART::Joint::TWIST;
                        cout << "Now moving: TWIST" << endl;
                    }
                    else
                        cout << "No such DOF: TWIST" << endl;
                    break;
                case '8': // Set window size as 800x600
                    viewerPtr->SetSize(800, 600);
                    viewerPtr->PostRedisplay();
                    break;
                case 'a': // activate/deactivate action
                    if (global::actionPtr1->IsActive())
                    {
                        global::actionPtr1->Deactivate();
                    }
                    else
                    {
                        global::actionPtr1->Activate();
                        viewerPtr->SetIdleHandler(idleHdlPtr);
                        viewerPtr->redrawOnIdle = true;
                    }
                    break;
                case 'b': // change background color
                    if (scenePtr->GetBackgroundColor() == VART::Color::BLACK())
                        scenePtr->SetBackgroundColor(VART::Color::WHITE());
                    else
                        scenePtr->SetBackgroundColor(VART::Color::BLACK());
                    viewerPtr->PostRedisplay();
                    break;
                case 'c': // change cyclic status
                    cyclicStatus = !global::actionPtr1->IsCyclic();
                    global::actionPtr1->SetCyclic(cyclicStatus);
                    cout << "Action '" << global::actionPtr1->description << "' new cyclic status: ";
                    if (cyclicStatus)
                        cout << "true" << endl;
                    else
                        cout << "false" << endl;
                    break;
                case 'd': // print out dofs
                    if (global::selectedJointPtr)
                    {
                        cout << "FLEXION: "
                             << global::selectedJointPtr->GetDof(VART::Joint::FLEXION).GetCurrent();
                        if (global::selectedJointPtr->HasDof(VART::Joint::ADDUCTION))
                        {
                            cout << " ADDUCTION: "
                                 << global::selectedJointPtr->GetDof(VART::Joint::ADDUCTION).GetCurrent();
                            if (global::selectedJointPtr->HasDof(VART::Joint::TWIST))
                                cout << " TWIST: "
                                     << global::selectedJointPtr->GetDof(VART::Joint::TWIST).GetCurrent();
                        }
                        cout << endl;
                    }
                    break;
                case 'e': // tired modifier
                    if (true)
                    {
                        cout << "Enter tired weight (float, [0..1]): ";
                        float weight;
                        cin >> weight;
                        if (weight < 0)
                            global::tiredModifier.weight = 0;
                        else
                        {
                            if (weight > 1)
                                global::tiredModifier.weight = 1;
                            else
                                global::tiredModifier.weight = weight;
                        }
                        global::model1.ModifyActions(global::tiredModifier);
                        viewerPtr->PostRedisplay();
                    }
                    break;
                case 'f': // save action to file
                    global::outFile.open(global::fileName.c_str());
                    global::outFile << *global::actionPtr1;
                    fileNotSaved = false;
                    cout << "File '" << global::fileName << "' saved." << endl;
                    break;
                case 'g': // activate with timeout
                    if (global::actionPtr1->IsActive())
                        cerr << "Action is already active." << endl;
                    else
                    {
                        cout << "Enter timeout (float, in seconds): ";
                        cin >> timeout;
                        global::actionPtr1->Activate();
                        global::actionPtr1->Deactivate(timeout);
                        viewerPtr->SetIdleHandler(idleHdlPtr);
                        viewerPtr->redrawOnIdle = true;
                    }
                    break;
                case 'h': // print out help
                    cout << "1) select FLEXION\n"
                         << "2) select ADDUCTION\n"
                         << "3) select TWIST\n"
                         << "<SPACE>) reset camera\n"
                         << ",.) rool camera\n"
                         << "\\) change rotation for selected DOF\n"
                         << "a) Activate/deactivate action\n"
                         << "b) change Background color (black/white)\n"
                         << "c) change Cyclic status\n"
                         << "d) print out Dofs\n"
                         << "e) set tirEd modifier\n"
                         << "f) save File\n"
                         << "g) Goto time (activate with time out)\n"
                         << "i) save 'rest state' fIle\n"
                         << "j) print out selected Joint\n"
                         << "k) add Key pose\n"
                         << "l) Look at selected joint\n"
                         << "m) print out caMera\n"
                         << "M) set caMera\n"
                         << "n) use Next camera\n"
                         << "o) toogle dof axis editing\n"
                         << "p) Print out action\n"
                         << "q) Quit\n"
                         << "r) Reset joint\n"
                         << "s) reSt all joints\n"
                         << "t) Recompute Times\n"
                         << "u) Stick right foot\n"
                         << "x) Toggle frame buffer eXport (fake animation time)\n"
                         << "y) Stick left foot\n";
                    break;
                case 'i': // save 'rest state' file
                    if (true) // test if file not saved?
                    {
                        ofstream outFile("rest.new.xml");
                        VART::XmlJointAction restAction;
                        VART::JointMover* jointMoverPtr;
                        restAction.description = "Rest";
                        restAction.SetSpeed(1.0f);
                        iter = global::jointCollector.begin();
                        for (; iter != global::jointCollector.end(); ++iter)
                        {
                            VART::Joint* jointPtr = const_cast<VART::Joint*>(*iter);
                            jointMoverPtr = restAction.AddJointMover(jointPtr, 3.0f, global::interpolator);
                            // for each dof
                            unsigned int dof = 0;
                            while (dof < (*iter)->GetNumDofs())
                            {
                                VART::Joint::DofID dofID = static_cast<VART::Joint::DofID>(dof);
                                float goalPosition = (*iter)->GetDof(dofID).GetRest();
                                jointMoverPtr->AddDofMover(dofID, 0, 1, goalPosition);
                                ++dof;
                            }
                        }
                        outFile << restAction;
                        cout << "Rest action saved on 'rest.new.xml'\n";
                    }
                    break;
                case 'j': // print out selected joint
                    if (global::selectedJointPtr)
                    {
                        bool oldValue = VART::SceneNode::recursivePrinting;
                        VART::SceneNode::recursivePrinting = false;
                        global::selectedJointPtr->XmlPrintOn(cout, 0);
                        VART::SceneNode::recursivePrinting = oldValue;
                    }
                    break;
                case 'k': // add key pose to action
                    ++keyPoses;
                    AddKeyPose();
                    global::changedJoints.clear();
                    cout << "Added pose number " << keyPoses << ". Type t to normalize times."<<endl;
                    break;
                case 'l': // look at selected joint
                    cameraPtr = scenePtr->GetCurrentCamera();
                    if (cameraPtr && global::selectedJointPtr)
                    {
                        const VART::Dof& tempDOF = global::selectedJointPtr->GetDof(VART::Joint::FLEXION);
                        VART::Point4D position = tempDOF.GetPosition();
                        VART::SGPath pathToJoint;
                        global::model1.FindPathTo(global::selectedJointPtr, &pathToJoint);
                        VART::Transform trans;
                        pathToJoint.GetTransform(&trans);
                        trans.ApplyTo(&position);
                        cameraPtr->SetTarget(position);
                    }
                    viewerPtr->PostRedisplay();
                    break;
                case 'm': // print out camera
                    cameraPtr = scenePtr->GetCurrentCamera();
                    cout << *cameraPtr << endl;
                    break;
                case 'M': // set camera
                    cameraPtr = scenePtr->GetCurrentCamera();
                    if (true)
                    {
                        cout << "Enter position (x, y, z): ";
                        float x, y, z;
                        cin >> x >> y >> z;
                        cameraPtr->SetLocation(VART::Point4D(x,y,z));
                        cout << "Enter focus (x, y, z): ";
                        cin >> x >> y >> z;
                        cameraPtr->SetTarget(VART::Point4D(x,y,z));
                        cout << "Enter UP (x, y, z): ";
                        cin >> x >> y >> z;
                        cameraPtr->SetUp(VART::Point4D(x,y,z,0));
                    }
                    viewerPtr->PostRedisplay();
                    break;
                case 'n': // use next camera
                    viewerPtr->UseNextCamera();
                    viewerPtr->PostRedisplay();
                    break;
                case 'o': // toogle dof axis editing
                    if (global::editingDOF)
                        cout << "No longer editing dof axis\n";
                    else
                        cout << "Now editing dof axis\n";
                    global::editingDOF = !global::editingDOF;
                    break;
                case 'p': // print out action
                    cout << *global::actionPtr1 << endl;
                    break;
                case 'q':
                    exit(0);
                    break;
                case 'r': // reset joint
                    global::selectedJointPtr->SetAtRest();
                    viewerPtr->PostRedisplay();
                    break;
                case 's': // rest all joints
                    iter = global::jointCollector.begin();
                    for (; iter != global::jointCollector.end(); ++iter)
                        const_cast<VART::Joint*>(*iter)->SetAtRest();
                    viewerPtr->PostRedisplay();
                    break;
                case 't': // Set times for every key pose
                    SetTimes();
                    cout<<"Times have been normalized. Type a to activate."<<endl;
                    break;
                case 'u': // stick right foot
                    {
                        VART::Point4D pos;
                        global::model1.StickRightFoot();
                        global::model1.ResetPosition();
                        global::model1.ComputeStickPosition(&pos);
                        cout << "Right foot is at " << pos << endl;
                        break;
                    }
                //~ case 'x': // Toggle frame buffer export
                    //~ if (true)
                    //~ {
                        //~ static bool exporting = false;
                        //~ if (exporting)
                        //~ {
                            //~ exporting = false;
                            //~ viewerPtr->SetDrawHandler(NULL);
                            //~ cout << "Frame exporting DISABLED." << endl;
                        //~ }
                        //~ else
                        //~ {
                            //~ exporting = true;
                            //~ viewerPtr->SetDrawHandler(&exporter);
                            //~ VART::Action::frameFrequency = 0.033333333;
                            //~ //VART::Action::frameFrequency = 0.008333333;
                            //~ exporter.SetBufferSize(viewerPtr->GetWidth(), viewerPtr->GetHeight());
                            //~ cout << "Frame exporting ENABLED (30fps - fake time)." << endl;
                        //~ }
                    //~ }
                    //~ break;
                case 'y': // stick left foot
                    {
                        VART::Point4D pos;
                        global::model1.StickLeftFoot();
                        global::model1.ResetPosition();
                        global::model1.ComputeStickPosition(&pos);
                        cout << "Left foot is at " << pos << endl;
                        break;
                    }
                case '[': //this is the same funcionality of mouseWhellUp
                    if ((global::selectedJointPtr) && (global::selectedJointPtr->HasDof(clickHPtr->selectedDof)))
                    {
                        global::selectedJointPtr->MoveDof(clickHPtr->selectedDof, -0.002);
                        viewerPtr->PostRedisplay();
                        clickHPtr->MarkJointAsChanged();
                    }
                    break;
                case ']'://this is the same funcionality of mouseWhellDown
                    if ((global::selectedJointPtr) && (global::selectedJointPtr->HasDof(clickHPtr->selectedDof)))
                    {
                        global::selectedJointPtr->MoveDof(clickHPtr->selectedDof, 0.002);
                        viewerPtr->PostRedisplay();
                        clickHPtr->MarkJointAsChanged();
                    }
                    break;
                default:
                    cerr << "Key " << key << " not handled!" << endl;
            }
        }
        void AddKeyPose() {
            list<VART::Joint*>::iterator iter = global::changedJoints.begin();
            VART::JointMover* jointMoverPtr;
            unsigned int dof;
            float goalPosition;
            VART::Joint::DofID dofID;

            // for each changed joint
            while (iter != global::changedJoints.end()) {
                jointMoverPtr = global::actionPtr1->AddJointMover(*iter, 2.0f, global::interpolator);
                cout << "Added joint mover " << jointMoverPtr << endl;
                global::jointMovers.push_back(jointMoverPtr);
                dof = 0;
                // for each dof
                while (dof < (*iter)->GetNumDofs()) {
                    dofID = static_cast<VART::Joint::DofID>(dof);
                    goalPosition = (*iter)->GetDof(dofID).GetCurrent();
                    jointMoverPtr->AddDofMover(dofID, keyPoses-1, keyPoses, goalPosition);
                    cout << dofID << " from: " << keyPoses-1 << " to: " << keyPoses << " goal: " << goalPosition << endl;
                    ++dof;
                }
                ++iter;
            }
        }
        void ModifyKeyPose()
        {
            // Which joint movers should change?
            list<VART::Joint*>::iterator iter = global::changedJoints.begin();
            while (iter != global::changedJoints.end())
            {
                ++iter;
            }
        }
        void SetTimes() { // Sets times for jointmovers and dof movers so that there exists 2s
                          // between every key pose
            list<VART::JointMover*>::iterator iter = global::jointMovers.begin();
            // for each joint mover...
            while (iter != global::jointMovers.end()) {
                (*iter)->SetDuration(2.0f);
                (*iter)->MultiplyTimes(1.0f/keyPoses);
                ++iter;
            }
        }
        void SetNumberOfKeyPoses(unsigned int n) {
            keyPoses = n;
        }
        VART::Scene* scenePtr;
        VART::Camera defaultCamera;
        ClickHandlerClass* clickHPtr;
        IdleHandlerClass* idleHdlPtr;
        unsigned int keyPoses;
        float timeout;
        VART::PNMExporter exporter;
};

bool FileExists(const string& fileName)
{
    ifstream file(fileName.c_str());
    if (!file)
        return false;
    else
        return true;
}

bool ReadActionFromFile(VART::XmlJointAction* actionPtr, const VART::SceneNode& node)
// VART::XmlJointAction.LoadFromFile() is too noisy, try a silent test first...
{
    if (FileExists(global::fileName))
        return actionPtr->LoadFromFile(global::fileName.c_str(), node);
    else
        return false;
}

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::XmlScene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    KeyboardHandler kbh; // create a keyboard handler
    ClickHandlerClass clickHandler;
    IdleHandlerClass idleHdl;
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,18,18),VART::Point4D(0,8,0),VART::Point4D::Y());
    VART::Camera camera2(VART::Point4D(6,18,17.3),VART::Point4D(-1.1,8,-0.6),VART::Point4D::Y());
    VART::Camera camera3(VART::Point4D(-10,9,0),VART::Point4D(0,9,0),VART::Point4D::Y());
    // Create some objects
    VART::MeshObject floor;

    // Initialize scene objects
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " model action" << endl;
        return 1;
    }
    if (global::model1.LoadFromFile(argv[1]))
    {
        global::model1.SetDescription("human");
        // Create a list of joints from the given (SceneNode) list
        global::model1.TraverseDepthFirst(&global::jointCollector);

        global::fileName = argv[2];

        // Add '.xml' at the end of action name if not already there
        string::size_type xml_rpos = global::fileName.rfind(".xml");
        if ((xml_rpos == string::npos) || ((global::fileName.length() - xml_rpos) != 4))
            global::fileName += ".xml";

        //if (ReadActionFromFile(&global::action1, global::model1))
        if (FileExists(global::fileName))
            global::actionPtr1 = global::model1.LoadAction(global::fileName);
        if (global::actionPtr1)
        { // action could be read from a file
            list<float> finalTimes;
            global::actionPtr1->GetFinalTimes(&finalTimes);
            cout << "Estimated " << finalTimes.size() << " key frames." << endl;
            kbh.SetNumberOfKeyPoses(finalTimes.size());
            // nao ta guardando o nome da acao
            global::editingKeyPoses = true;
            // mark action related joints as changed
            global::actionPtr1->GetJoints(&global::changedJoints);
        }
        else
        { // could not read action from file
            cout << "Could not read action from '" << global::fileName << "'" << endl;
            global::actionPtr1 = new VART::XmlJointAction;
            global::actionPtr1->description = argv[2];
        }

        global::tiredModifier.LoadFromFile("tired.4b.offset");

        clickHandler.scenePtr = &scene;
        clickHandler.cameraPtr = &camera;
        clickHandler.articulatedBodyPtr = &global::model1;
        //~ cout << "Articulated Body root node: " << clickHandler.articulatedBodyPtr->GetDescription() << endl;

        floor.MakeBox(-10,10,-0.1,0,-10,10);
        floor.SetMaterial(VART::Material::PLASTIC_GREEN());
        floor.SetDescription("floor");

        // Build the scene graph
        scene.AddObject(&global::model1);
        scene.AddObject(&floor);

        // Add lights and cameras to the scene
        scene.AddLight(VART::Light::BRIGHT_AMBIENT());
        scene.AddCamera(&camera);
        scene.AddCamera(&camera2);
        scene.AddCamera(&camera3);

        // Set up the viewer
        viewer.SetTitle("V-ART Action Builder");
        viewer.SetScene(scene); // attach the scene
        viewer.SetKbHandler(&kbh); // attach the keyboard handler
        viewer.SetClickHandler(&clickHandler);
        viewer.SetSize(400,800);
        viewer.SetPosition(700,20);
        camera.SetAspectRatio(0.5);
        camera3.SetProjectionType(VART::Camera::ORTHOGRAPHIC);
        camera3.SetVisibleVolumeHLimits(-5,5);
        camera3.SetVisibleVolumeVLimits(10,-10);
        //~ viewer.redrawOnIdle = true;
        viewer.autoRespondKeys = false;
        kbh.scenePtr = &scene;
        kbh.defaultCamera = camera;
        kbh.clickHPtr = &clickHandler;
        kbh.idleHdlPtr = &idleHdl;

        // Mark adduction, R_forearmJoint for debug
        // remove me
        //~ VART::Joint* dbgJtPtr = dynamic_cast<VART::Joint*>(scene.GetObjectRec("headJoint"));
        //~ if (dbgJtPtr)
        //~ {
            //~ VART::Dof* dbgDofPtr = const_cast<VART::Dof*>(&(dbgJtPtr->GetDof(VART::Joint::ADDUCTION)));
            //~ dbgDofPtr->debug = true;
        //~ }

        // Run application
        scene.DrawLightsOGL(); // Set OpenGL's lights' state
        cout << "Hit [h] for help!" << endl;
        VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
        return 0;
    }
}
