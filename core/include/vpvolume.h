//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpvolume.h
//  DESCRIPTION.: Contain the VPVolume class hierarchy declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: December/04/2000
//  DESCRIPTION.: Class declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: June/04/2001
//  DESCRIPTION.: New constants, attributes (view dimension, 
//				  activeCamera) and methods inclusion.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/09/2001
//  DESCRIPTION.: New structs, attributes and methods inclusion,
//				  due to Gouraud shading implementation.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPVOLUME_H
#define __VPVOLUME_H


#include	<vpimage.h>
#include	<vpslice.h>
#include	<vptable.h>
#include	<vpraycasting.h>


// To foresee several "includes"
#ifndef __GL_STUFF
#define __GL_STUFF	
#include <FL/Fl_Gl_Window.h> 
#include <FL/gl.h>
#include <GL/glu.h>
#endif	// __GL_STUFF


// File contents constants
#define		NONE			1001
#define		ARQCT			1002
#define		ARQMRI			1003
#define		ARQPET			1004
#define		ARQSPECT		1005


// Type of opacity computation
#define		LINEAROPAC		100
#define		TABLEOPAC		101
#define		GRADIENTOPAC	102


// Shading methods
#define		LOCALSHADING	200
#define		GOURAUDSHADING	201
#define		PHONGSHADING	202


// Auxiliary constants
#define		red			0
#define		green		1
#define		blue		2


// Another constants and structs to read .vol volumes
#define C_VR_MAGIC_NUMBER_OLD           192837465
#define C_VR_MAGIC_NUMBER               192837466
#define C_MANUFACTURER_STRING_LENGTH    20
#define C_ORIG_FILENAME_STRING_LENGTH   128
#define	C_UNSIGNED_BYTE					1001
#define	C_UNSIGNED_SHORT				1002
#define	C_INTENSITY8					2001
#define	C_INTENSITY16					2002
#define	C_INDEX4_INTENSITY12			2003
#define	C_INDEX8_INTENSITY8				2004

typedef struct {
	int x;
	int y;
	int z;
}xyz;
typedef unsigned short int	sword;


// Structs to store volume gradients
typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char z;
}xyzc;


//-----------------------------------------------------------------
// V P V O L U M E
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPVolume
// Superclass: VPImage
// Subclasses: -
///\deprecated

class VPVolume : public VPImage{

	private:
		int visualizationType;
		int opacityComputation;
		int fileType;
		int zDimension;
		int fileBits;		// number of bits read from the file
		int topSlice;		// axial slice
		int sideSlice;		// sagittal slice
		int frontSlice;		// coronal slice
		unsigned int **grayImage;	// \ load gray level image that will be
									// / generated by the visualization algorithm
		unsigned int ***colorImage;	// \ load color level image that will be
									// / generated by the visualization algorithm
		unsigned int maxVolumeColor;	// highest volume density value
		unsigned int minVolumeColor;	// smallest volume density value
		unsigned int ***volumeMatrix;	// to store volume data
		unsigned int ***volumeMatrix2;	// to store volume data
		unsigned char ***volumeLightedMatrix;	// to store light volume information
		VPPoint3D scale;				// Volume scale (e.g. for slices adjustment)
		VPPoint3D centerFocalPoint;		// Volume center focal point
		VPTable controlTables;			// Opacity and Color Table
		VPCamera *activeCamera;			// to set VPScene active camera, which is determined by the view
		VPRayCasting rayCasting;		// volume visualization algorithms
		VPSlice slice;					// slice visualization algorithms
		bool allocMemory();				// allocates the volume data matrix
		bool allocMemory2();				// allocates the volume data matrix
		VPPoint3D cameraLocationForInnerStructure;
	public:
		VPVolume(); 
		VPVolume (const char *n, int t, int dh, int dv, int s, int b);
		~VPVolume();
		void  vpReadRawFile(const char *n);
		void  vpReadIFile(const char *n, char *p);
		void  vpReadVolFile(const char *n);
		float vpGetLightValue(int i, int j, int k);
		unsigned int vpGetValue(int i, int j, int k);
		inline int   vpGetZDimension() {	return zDimension; }
		void  vpRender();
		void  vpAllocateGrayImage(int h, int w);
		void  vpAllocateColorImage(int h, int w);
		void  vpProcessLightedVolume(VPVector3D ld, float al);
		VPVector3D   vpGetSobelGradient(int i, int j, int k);
		VPVector3D   vpGetLinearGradientEstimation(int i, int j, int k);
		VPVector3D   vpGetCentralDifferenceGradient(int i, int j, int k);
		void  vpSetSliceTopCamera();
		void  vpSetSliceFrontCamera();
		void  vpSetSliceSideCamera();
		void  vpDisplayGray();
		void  vpDisplayColor();
		void  vpRotate();
		void  vpTranslate(VPPoint3D t);
		void  vpScale(VPPoint3D s);
		void  vpSetVisualizationType(int vp);
		int   vpGetVisualizationType();
		void  vpSetTopSlice(int v);
		int   vpGetTopSlice();
		void  vpSetSideSlice(int v);
		int   vpGetSideSlice();
		void  vpSetFrontSlice(int v);
		int   vpGetFrontSlice();
		void  vpSetActiveCamera(VPCamera *c);
		void  vpSetRayCastingSBand(VPPoint3D sb, int vt);
		int	  vpGetRayCastingSBand();
		void  vpSetRayCastingTBand(VPPoint3D tb, int vt);
		int	  vpGetRayCastingTBand();
		void  vpSetRayCastingSampleStep(float s);
		float vpGetRayCastingSampleStep();
		void  vpSetRayCastingAmbientLight(float a);
		float vpGetRayCastingAmbientLight();
		void  vpSetRayCastingDiffuseLight(float d);
		float vpGetRayCastingDiffuseLight();
		void  vpSetRayCastingSpecularExponent(int s);
		int   vpGetRayCastingSpecularExponent();
		void  vpSetRayCastingSpecular(bool s);
		bool  vpGetRayCastingSpecular();
		void  vpSetRayCastingShadingMethod(int sm);
		int   vpGetRayCastingShadingMethod();
		void  vpSetRayCastingTypeOfCuttingTool(int t);
		int   vpGetRayCastingTypeOfCuttingTool();
		void  vpSetOpacityComputation(int oc);
		int   vpGetOpacityComputation();
		void  vpSetOpacityTable(float t[]);
		float * vpGetOpacityTable();
		void  vpSetColorTable(VPColor c[]);
		VPColor		  * vpGetColorTable();
		void  vpSetControlPoints (int cp[], int n);
		void  vpGetControlPoints(int cp[], int &n);
		void  vpSetControlPoints (int cp[], VPColor c[], int n);
		void  vpGetControlPoints(int cp[], VPColor c[],int &n);
		void  vpProcessHistogram();
		void  vpProcessCenterFocalPoint();
		VPPoint3D	 vpGetCenterFocalPoint();
		unsigned int vpGetMinVolumeColor();
		unsigned int vpGetMaxVolumeColor();
		VPTable		 vpGetControlTables();
		GLvoid		 vpSwapBytes(GLint *intPtr, int sizeBytes);
		void	  vpAdjustScale(int x, int y, int z);
		VPPoint3D vpGetScale();
		VPPoint2D vpGetSliceViewSBandPoint();
		VPPoint2D vpGetSliceViewTBandPoint();
		int       vpGetFileType();
		void	  vpSetCameraLocationForInnerStructure(VPPoint3D c);
		VPPoint3D vpGetCameraLocationForInnerStructure();
};



#endif  // __VPVOLUME_H
