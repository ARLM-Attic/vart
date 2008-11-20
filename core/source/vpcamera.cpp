/// \file vpcamera.cpp
/// \brief Implementation file for VPCamera VPAT class.
/// \version $Revision: 1.4 $

// ChangeLog
// Dec 21, 2005 - Bruno de Oliveira Schneider
// - Attributes projectionType, fovY, aspectRatio, nearPlaneDistance, farPlaneDistance,
//   vvLeft, vvRight, vvTop, vvBottom, location, target and up changed from private to protected.
// - Removed commented old methods and attributes.
// May 20, 2005 - Bruno de Oliveira Schneider
// - Fixed vvBottom initialization (was 1, now is -1).
// - GetDescription is now inline and returns by reference.
// Dec 16, 2004 - Bruno de Oliveira Schneider
// - Changed DrawOGL, so that it now sets the projection matrix.
// - The projection type were changed from C preprocessor symbols to a public type
//   ProjectionType. Changed GetProjectionType and SetProjectionType.
// - Added aspectRatio attribute and related Get/Set methods.
// - Renamed nearPlane and farPlane to nearPlaneDistance and farPlaneDistance, along
//   with related Get/Set methods.
// - Added vvLeft, vvRight, vvTop and vvBottom to allow any orthographic projection.
//   Added related Get/Set methods.
// - Commented out viewHeight, viewWidth, winBottomLeft, winTopRight and related
//   Get/Set methods. It seems that they served to same purpose as vvLeft, vvRight, etc.
// - Added an output operator.
// - Added MoveSideways and MoveUp.
// Sep 24, 2004 - Bruno de Oliveira Schneider
// - Removed 'vp' preffix of every method name.
// - Added methods 'YawAroundTarget', 'PitchAroundTarget', 'MoveForward' and 'Yaw'.
// - Removed 'using namespace std;' from header file.
// - Code typing has been cleaned (tabs replaced by spaces, excessive spaces
//   removed, etc.) in the headers file because tabs and spaces were mixed together.
// - Changed the class of "location", "target" and "up" to VPPoint4D.
// - Added "const" to identify constant methods and to avoid copy large parameters
//   (except to those that would require changing other classes).
// - Added a copy constructor and "operator=" because this class is not static.
// - Added "(position, focus, upVec)" constructor.
// - Added "DrawOGL" method.
// April/25/2001 - Isabel Harb Manssour
// - New methods inclusion.
// September/01/2000 - Julierme Krüger Gavião
// - Gets and Sets of fovY attibute and ortho attributes modification.
// June/13/2000 - Isabel Harb Manssour
// - Gets and Sets update.
// June 09, 2000 - Andre Rubin Santos
// - Fixing Gets and Sets implementation.

#include "vpcamera.h"
#include "vptransform.h"
#ifdef VP_OGL
#include <GL/glu.h>
#endif

//?
#include <iostream>

using namespace std;

VPCamera::VPCamera() {
    projectionType = PERSPECTIVE;
    fovY = 60;
    aspectRatio = 1.0f;
    vvLeft = -1.0;
    vvRight = 1.0;
    vvTop = 1.0;
    vvBottom = -1.0;
    nearPlaneDistance = 0.5f;
    farPlaneDistance = 100.0f;

    //~ viewHeight = 0;
    //~ viewWidth = 0;
}

VPCamera::VPCamera(const VPPoint4D& position, const VPPoint4D& focus, const VPPoint4D& upVec)
{
    projectionType = PERSPECTIVE;
    fovY = 60;
    vvLeft = -1.0;
    vvRight = 1.0;
    vvTop = 1.0;
    vvBottom = -1.0;
    nearPlaneDistance = 0.5f;
    farPlaneDistance = 100.0f;
    aspectRatio = 1.0f;
    location = position;
    target = focus;
    up = upVec;
    // Make sure "up" is orthogonal to "front" and normalized
    VPPoint4D front = focus - position;
    up.Normalize();
    front.Normalize();
    VPPoint4D left = up.CrossProduct(front);
    up = front.CrossProduct(left);
}

VPCamera::VPCamera(const VPCamera& cam)
{
    description = cam.description;
    projectionType = cam.projectionType;
    vvLeft = cam.vvLeft;
    vvRight = cam.vvRight;
    vvTop = cam.vvTop;
    vvBottom = cam.vvBottom;
    nearPlaneDistance = cam.nearPlaneDistance;
    farPlaneDistance = cam.farPlaneDistance;
    fovY = cam.fovY;
    location = cam.location;
    target = cam.target;
    up = cam.up;
    aspectRatio = cam.aspectRatio;
    //~ viewHeight = cam.viewHeight;
    //~ viewWidth = cam.viewWidth;
    //~ winBottomLeft = cam.winBottomLeft;
    //~ winTopRight = cam.winTopRight;
}

VPCamera& VPCamera::operator=(const VPCamera& cam)
{
    description = cam.description;
    projectionType = cam.projectionType;
    vvLeft = cam.vvLeft;
    vvRight = cam.vvRight;
    vvTop = cam.vvTop;
    vvBottom = cam.vvBottom;
    nearPlaneDistance = cam.nearPlaneDistance;
    farPlaneDistance = cam.farPlaneDistance;
    fovY = cam.fovY;
    //~ viewHeight = cam.viewHeight;
    //~ viewWidth = cam.viewWidth;
    //~ winBottomLeft = cam.winBottomLeft;
    //~ winTopRight = cam.winTopRight;
    location = cam.location;
    target = cam.target;
    up = cam.up;
    aspectRatio = cam.aspectRatio;
    return (*this);
}

void VPCamera::SetDescription(const string& descriptionValue){
    description = descriptionValue;
}

//~ VPPoint2D VPCamera::GetWinBottomLeft() const {
    //~ return winBottomLeft;
//~ }

//~ void VPCamera::SetWinBottomLeft(VPPoint2D winBottomLeftValue){
    //~ winBottomLeft = winBottomLeftValue;
//~ }

// FixMe: This method should be const
//~ void VPCamera::GetWinTopRight(float &x, float &y) {
    //~ x = winTopRight.vpGetX();
    //~ y = winTopRight.vpGetY();
//~ }

//~ void VPCamera::SetWinTopRight(float x, float y) {
    //~ winTopRight.vpSetX(x);
    //~ winTopRight.vpSetY(y);
//~ }

//~ VPPoint2D VPCamera::GetWinTopRight() const {
    //~ return winTopRight;
//~ }

//~ void VPCamera::SetWinTopRight(VPPoint2D winTopRightValue){
    //~ winTopRight = winTopRightValue;
//~ }

// FixMe: This method should be const
//~ VPPoint2D VPCamera::GetWinLeftRight() {
    //~ VPPoint2D point(winBottomLeft.vpGetX(), winTopRight.vpGetY());
    //~ return point;
//~ }

//~ void VPCamera::SetWinLeftRight(VPPoint2D winLeftRightValue){
    //~ winBottomLeft.vpSetY(winLeftRightValue.vpGetX());
    //~ winTopRight.vpSetY(winLeftRightValue.vpGetY());
//~ }

// FixMe: This method should be const
//~ VPPoint2D VPCamera::GetWinBottomTop() {
    //~ VPPoint2D point(winBottomLeft.vpGetX(), winTopRight.vpGetX());
    //~ return point;
//~ }

//~ void VPCamera::SetWinBottomTop(VPPoint2D winBottomTopValue){
    //~ winBottomLeft.vpSetX(winBottomTopValue.vpGetX());
    //~ winTopRight.vpSetX(winBottomTopValue.vpGetY());
//~ }

VPPoint4D VPCamera::GetLocation() const {
    return location;
}

void VPCamera::SetLocation(const VPPoint4D& locationValue){
    location = locationValue;
}

VPPoint4D VPCamera::GetTarget() const {
    return target;
}

void VPCamera::SetTarget(const VPPoint4D& targetValue){
    target = targetValue;
}

VPPoint4D VPCamera::GetUp() const {
    return up;
}

void VPCamera::SetUp(const VPPoint4D& upValue){
    up = upValue;
}

float VPCamera::GetFovY() const {
    return fovY;
}

void VPCamera::SetFovY(float f) {
    fovY = f;
}

void VPCamera::SetVisibleVolumeHeight(double newValue) {
    double halfHeight = newValue / 2;
    double halfWidth = halfHeight * aspectRatio;
    vvTop = halfHeight;
    vvBottom = -halfHeight;
    vvLeft = -halfWidth;
    vvRight = halfWidth;
}

//~ void VPCamera::SetViewDimension(int h, int w) {
    //~ viewHeight = h;
    //~ viewWidth = w;
//~ }

//~ void VPCamera::GetViewDimension(int &h, int &w) const {
    //~ h = viewHeight;
    //~ w = viewWidth;
//~ }

//~ int VPCamera::GetViewWidth() const {
    //~ return viewWidth;
//~ }

//~ int VPCamera::GetViewHeight() const {
    //~ return viewHeight;
//~ }

void VPCamera::YawAroundTarget(double radians) {
    VPTransform trans;
    trans.MakeRotation(target, up, radians);
    trans.ApplyTo(&location);
}

void VPCamera::Yaw(double radians) {
    VPTransform trans;
    trans.MakeRotation(location, up, radians);
    trans.ApplyTo(&target);
}

void VPCamera::PitchAroundTarget(double radians) {
    VPTransform trans;
    VPPoint4D left;
    VPPoint4D front = target - location;
    front.Normalize();
    left = up.CrossProduct(front);
    trans.MakeRotation(target, left, radians);
    trans.ApplyTo(&location);
    trans.ApplyTo(&up);
}

void VPCamera::MoveForward(double distance) {
    VPTransform trans;
    VPPoint4D front = target - location;
    front.Normalize();
    front *= distance;
    trans.MakeTranslation(front);
    trans.ApplyTo(&location);
    trans.ApplyTo(&target);
}

void VPCamera::MoveSideways(double distance) {
    VPTransform trans;
    VPPoint4D right;
    
    trans.MakeRotation(up,-1.5707963267948966192313216916398);
    right = trans * (target - location);

    right.Normalize();
    right *= distance;
    trans.MakeTranslation(right);
    trans.ApplyTo(&location);
    trans.ApplyTo(&target);
}

void VPCamera::MoveUp(double distance) {
    VPTransform trans;
    VPPoint4D translation(up);

    translation.Normalize();
    translation *= distance;
    trans.MakeTranslation(translation);
    trans.ApplyTo(&location);
    trans.ApplyTo(&target);
}

bool VPCamera::DrawOGL() const {
#ifdef VP_OGL
    // It seems that setting the projection matrix at every rendering cycle is a
    // good idea because an application may have multiple cameras and multiple viewers.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projectionType == PERSPECTIVE)
        gluPerspective(fovY, aspectRatio, nearPlaneDistance, farPlaneDistance);
    else
        glOrtho(vvLeft, vvRight, vvBottom, vvTop, nearPlaneDistance, farPlaneDistance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(location.GetX(),location.GetY(),location.GetZ(),
              target.GetX(), target.GetY(), target.GetZ(),
              up.GetX(), up.GetY(), up.GetZ());
    return true;
#else
    return false;
#endif
}

ostream& operator<<(ostream& output, const VPCamera& cam)
{
    output << "Camera(" << cam.description << ": pos"<< cam.location << " target" << cam.target
           << " up" << cam.up << " near(" << cam.nearPlaneDistance << ") far(" << cam.farPlaneDistance
           << "))";
    return output;
}
