//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vptable.h
//  DESCRIPTION.: Contain the VPTable class declarations,
//				  that is responsible for color and opacity
//				  mapping. 
//
//  AUTHOR......: Isabel Harb Manssour
//  LAST UPDATE.: May/08/2000
//  DESCRIPTION.: Class declaration.
//
//  AUTHOR......: Isabel Harb Manssour
//  LAST UPDATE.: June/05/2001
//  DESCRIPTION.: New methods and attributes declaration.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPTABLE_H	
#define __VPTABLE_H	


#include <vpvector3d.h>
#include <vpcolor.h>
#include <vector>	//STL include
using namespace std;


// Opacity and color array dimension
#define DIM 256



//-----------------------------------------------------------------
// T A B L E    
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPTable
// Superclass: -
// Subclasses: -
///\deprecated

class VPTable {
	private:
		int minVolumeDensity;
		int maxVolumeDensity;
		VPVector3D maxGradient;
		VPColor colors[DIM];			// Color table
		float	opacity[DIM];			// Opacity table
		int		numControlPoints;		// Number of table control points
		int		controlPoints[DIM];		// Control points density
		VPColor colorControlPoints[DIM];// Control points color
	public:
		VPTable();
		void  vpSetMinVolumeDensity(int d);
		void  vpSetMaxVolumeDensity(int d);
		void  vpProcessMaxGradient();
		void  vpSetMaxGradient(VPVector3D g);
		float vpGetLinearOpacityValue(float color); 
		float vpGetTableOpacityValue(float color); 
		float vpGetGradientOpacityValue(int color, VPVector3D g); 
		void  vpSetOpacityTable (float t[]);
		float * vpGetOpacityTable();
		void  vpSetColorTable (VPColor c[]);
		VPColor * vpGetColorTable();
		void  vpSetControlPoints (int cp[], int n);
		void  vpGetControlPoints(int cp[], int &n);
		void  vpSetControlPoints (int cp[], VPColor c[], int n);
		void  vpGetControlPoints(int cp[], VPColor c[], int &n);
		VPColor vpGetColor(float color);

};




#endif	// __VPTABLE_H
