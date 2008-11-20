///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcamera.h
//  DESCRIPTION.: Contain the VPCamera class declarations. 
//
//  AUTHOR......: Andre Rubin Santos
//  DATE........: June/09/2000
//  DESCRIPTION.: Attributes and classes declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: June/13/2000
//  DESCRIPTION.: Redefinition of attributes and classes.
//
///////////////////////////////////////////////////////////////////

// To foresee several "includes"
#ifndef __VPCAMERA_H	
#define __VPCAMERA_H


#include	<vppoint2d.h>
#include	<vppoint3d.h>
#include	<vpvector3d.h>
#include	<string>	//STL include
//using namespace std;

#define ORTHOGRAPHIC	0
#define PERSPECTIVE		1



///////////////////////////////////////////////////////////////////
// Class Name: VPCamera
// Superclass: -
// Subclasses: -


class VPCamera {
	
	private:
	
		std::string	description;
		int projectionType;
		float nearPlane;
		float farPlane;
		float fovY; //Vertical angle of frustum.
		VPPoint2D winBottomLeft;
		VPPoint2D winTopRight;
		VPPoint3D location;
		VPPoint3D target;
		VPVector3D up;
	public:
		VPCamera();

		std::string vpGetDescription();

		void vpSetDescription(std::string descriptionValue);
		int vpGetProjectionType();
		void vpSetProjectionType(int projectionTypeValue);
		float vpGetNearPlane();
		void vpSetNearPlane(float nearValue);
		float vpGetFarPlane();
		void vpSetFarPlane(float farValue);
		VPPoint2D vpGetWinBottomLeft();
		void vpSetWinBottomLeft(VPPoint2D winBottomLeftValue);
		VPPoint2D vpGetWinTopRight();
		void vpSetWinTopRight(VPPoint2D winTopRightValue);
		VPPoint2D vpGetWinLeftRight();
		void vpSetWinLeftRight(VPPoint2D winLeftRightValue);
		VPPoint2D vpGetWinBottomTop();
		void vpSetWinBottomTop(VPPoint2D winTopRightValue);
		VPPoint3D vpGetLocation();
		void vpSetLocation(VPPoint3D locationValue);
		VPPoint3D vpGetTarget();
		void vpSetTarget(VPPoint3D targetValue);
		VPVector3D vpGetUp();
		void vpSetUp(VPVector3D upValue);
		float vpGetFovY();
		void vpSetFovY(float f);
		
};




#endif	// __VPCAMERA_H
