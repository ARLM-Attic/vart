//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpplane.h
//  DESCRIPTION.: Contain the VPPlane class declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/11/2000
//  DESCRIPTION.: Class declaration.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPPLANE_H	
#define __VPPLANE_H	


#include <vpvector3d.h>
#include <vppoint2d.h>
#include <vpcolor.h>




//-----------------------------------------------------------------
// P L A N E  
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPPlane 
// Superclass: -
// Subclasses: -
///\deprecated

class VPPlane {
	private:
		VPVector3D normal;
		VPVector3D point;
		VPPoint2D size;
		VPColor color;
	public:
		VPPlane();
		VPPlane(VPPlane *plane);
		void vpSetNormal(VPVector3D n);
		VPVector3D vpGetNormal(void);
		void vpSetPoint(VPVector3D p);
		VPVector3D vpGetPoint(void);
		void vpSetSize(VPPoint2D s);
		VPPoint2D vpGetSize(void);
		void vpSetColor(VPColor c);
		VPColor vpGetColor(void);
		float TestPoint(VPVector3D p);
		VPVector3D TestLine(VPVector3D p, VPVector3D dir);
};
/*
		Plane();
		Plane(Plane* _Plane);
		Plane(Plane& _Plane);
		Plane(XYZR _Point, XYZR _Normal, XYR _Size);
		Plane(XYZR _Point, FLOAT _Azimute, FLOAT _Elevacao, XYR _Size);

		void SetNormal(XYZR* _Normal);
		void SetNormal(FLOAT _Azimute, FLOAT _Elevacao);
		void GetNormal(FLOAT& _Azimute, FLOAT& _Elevacao);

		FLOAT TestPoint(XYZR _Point);
		XYZR TestLine(XYZR _Point, XYZR _Dir);

		void AdjustPlane(Plane& _PlaneC, FLOAT _ZVoxel);
		void AdjustPlaneZ(Plane& _PlaneC, FLOAT _ZVoxel);
		void AdjustPlaneX(Plane& _PlaneC, FLOAT _XVoxel);
*/

#endif	// __VPPLANE_H
