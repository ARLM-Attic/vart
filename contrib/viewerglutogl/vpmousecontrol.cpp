/// \file vpmousecontrol.cpp
/// \brief Implementation file for VPMouseControl VPAT class.
/// \version 1.3

// ChangeLog
// Dec 21, 2005 - Bruno de Oliveira Schneider
// - Added viewerPtr to ClickHandler and MotionHandler.
// - Removed SetController from ClickHandler because the controller needs only to be
//   set at VPMouseControl::SetClickHandler.
// Oct 06, 2005 - Bruno de Oliveira Schneider
// - Added viewerPtr and SetOwner.
// - Added mouseCtrlPtr and viewerPtr to DragHandler.
// - Added virtual destructors.
// May 27, 2005 - Bruno de Oliveira Schneider 
// - File created.

#include "vpmousecontrol.h"

#include <iostream>
using namespace std;

VPMouseControl::VPMouseControl()
{
    viewerPtr = NULL;
    clickHndPtr = NULL;
    dragHndPtr = NULL;
    motionHndPtr = NULL;
}

void VPMouseControl::OnClick()
{
    //~ cout << "in VPMouseControl::OnClick" << endl;
    if (clickHndPtr)
        clickHndPtr->OnClick();
}

void VPMouseControl::SetModifierState(ModifierID mdValue, bool state)
{
    //~ cout << "in VPMouseControl::SetModifierState" << endl;
    modifiers.set(mdValue,state);
    //~ cout << "out VPMouseControl::SetModifierState" << endl;
}

void VPMouseControl::SetDragHandler(DragHandler* handlerPtr)
{
    dragHndPtr = handlerPtr;
    handlerPtr->mouseCtrlPtr = this;
    handlerPtr->viewerPtr = viewerPtr;
}

void VPMouseControl::SetClickHandler(ClickHandler* handlerPtr)
{
    clickHndPtr = handlerPtr;
    handlerPtr->mouseCtrlPtr = this;
    handlerPtr->viewerPtr = viewerPtr;
}

void VPMouseControl::SetMotionHandler(MotionHandler* handlerPtr)
{
    motionHndPtr = handlerPtr;
    handlerPtr->mouseCtrlPtr = this;
    handlerPtr->viewerPtr = viewerPtr;
}

void VPMouseControl::NewEvent(ButtonID btn, StateID newState)
{
    if (newState == DOWN)
        buttonsState.set(btn);
    else
        buttonsState.reset(btn);
    button = btn;
    state = newState;
}
