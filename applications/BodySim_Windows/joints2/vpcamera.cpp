///////////////////////////////////////////////////////////////////

//

//  PROJECT.....: vpat - Creating Virtual Patients

//  RESPONSIBLE.: Carla Freitas e Luciana Nedel

//

//  FILE........: vpcamera.cpp

//  DESCRIPTION.: Contain the VPCamera class declarations.

//

//  AUTHOR......: Andre Rubin Santos

//  DATE........: June/09/2000

//  DESCRIPTION.: Fixing Gets and Sets implementation.

//

//  AUTHOR......: Isabel Harb Manssour

//  DATE........: June/13/2000

//  DESCRIPTION.: Gets and Sets update.

//

//  AUTHOR......: Julierme Krüger Gavião

//  DATE........: September/01/2000

//  DESCRIPTION.: Gets and Sets of fovY attibute and ortho attributes modification.

//







#include <vpcamera.h>





///////////////////////////////////////////////////////////////////

// Description: Constructor of VPCamera class.

// Parameters.: -

// Return.....: VPCamera.



VPCamera::VPCamera() {

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



std::string VPCamera::vpGetDescription(){

	return description;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetDescription(std::string desc){

	description = desc;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



int VPCamera::vpGetProjectionType(){

	return projectionType;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetProjectionType(int projectionTypeValue){

	projectionType = projectionTypeValue;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



float VPCamera::vpGetNearPlane(){

	return nearPlane;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetNearPlane(float nearValue){

	nearPlane = nearValue;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



float VPCamera::vpGetFarPlane(){

	return farPlane;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetFarPlane(float farValue){

	farPlane = farValue;

}



///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



VPPoint2D VPCamera::vpGetWinBottomLeft(){

	return winBottomLeft;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetWinBottomLeft(VPPoint2D winBottomLeftValue){

	winBottomLeft = winBottomLeftValue;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



VPPoint2D VPCamera::vpGetWinTopRight(){

	return winTopRight;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetWinTopRight(VPPoint2D winTopRightValue){

	winTopRight = winTopRightValue;

}



///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



VPPoint2D VPCamera::vpGetWinLeftRight(){

	VPPoint2D point(winBottomLeft.vpGetX(), winTopRight.vpGetY());

	return point;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetWinLeftRight(VPPoint2D winLeftRightValue){

	winBottomLeft.vpSetY(winLeftRightValue.vpGetX());

	winTopRight.vpSetY(winLeftRightValue.vpGetY());

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



VPPoint2D VPCamera::vpGetWinBottomTop(){

	VPPoint2D point(winBottomLeft.vpGetX(), winTopRight.vpGetX());

	return point;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetWinBottomTop(VPPoint2D winBottomTopValue){

	winBottomLeft.vpSetX(winBottomTopValue.vpGetX());

	winTopRight.vpSetX(winBottomTopValue.vpGetY());

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



VPPoint3D VPCamera::vpGetLocation(){

	return location;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetLocation(VPPoint3D locationValue){

	location = locationValue;

}





///////////////////////////////////////////////////////////////////

// Description: Method "vpGetTarget" returns the value of target

//				attribute.

// Parameters.: -

// Return.....: target (value of target attribute)



VPPoint3D VPCamera::vpGetTarget(){

	return target;

}





///////////////////////////////////////////////////////////////////

// Description:

// Parameters.: -

// Return.....: -



void VPCamera::vpSetTarget(VPPoint3D targetValue){

	target = targetValue;

}





///////////////////////////////////////////////////////////////////

// Description: Method "vpGetUp" returns the value of up attribute.

// Parameters.: -

// Return.....: up (value of up attribute)



VPVector3D VPCamera::vpGetUp(){

	return up;

}





///////////////////////////////////////////////////////////////////

// Description: Method "vpSetUp" sends a new value to the up attribute.

// Parameters.: VPVector3D upValue (contains a new value for up attribute)

// Return.....: -



void VPCamera::vpSetUp(VPVector3D upValue){

	up = upValue;

}



///////////////////////////////////////////////////////////////////

// Description: Method "vpGetFovY" returns the value of fovY attribute.

// Parameters.: -

// Return.....: fovY (value of fovY attribute)



float VPCamera::vpGetFovY() {

	return fovY;

}



///////////////////////////////////////////////////////////////////

// Description: Method "vpSetFovY" sends a new value to the fovY attribute.

// Parameters.: float f (contains a new value for fovY attribute)

// Return.....: -



void VPCamera::vpSetFovY(float f) {

	fovY = f;

}

