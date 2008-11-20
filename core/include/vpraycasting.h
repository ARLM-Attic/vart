//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpraycasting.h
//  DESCRIPTION.: Contain the VPRayCasting class declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: June/26/2000
//  DESCRIPTION.: Class declaration.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: July/10/2001
//  DESCRIPTION.: New methods declaration.
//
///////////////////////////////////////////////////////////////////


#include <vpgraphicobj.h>
#include <vptable.h>
#include <vpview.h>
#include <vpraysegment.h>


// To foresee several "includes"
#ifndef __VPRAYCASTING_H	
#define __VPRAYCASTING_H	


#include <vpscene.h>


#define NOTIO		 0
#define PIN			 1
#define POUT		 2
#define PFRONT		 0
#define	PBACK		 1
#define	PTOP		 2
#define	PBOTTOM		 3
#define	PLEFT		 4
#define	PRIGHT		 5

#define NOCUTTINGTOOL		10
#define CVP					11
#define OBLIQUESLICE		12
#define ONECUTPLANE			13
#define TWOCUTPLANES		14
#define CUBEBYINCLUSION		15 
#define CUBEBYEXCLUSION		16
#define SPHEREBYINCLUSION	17
#define SPHEREBYEXCLUSION	18
#define TWOCUTPLANESINCLUSIONOPAC	19

#define LINEAROPACITY		50
#define LEVOYSURFACEOPACITY	51

#define EPSILON 0.0001




//-----------------------------------------------------------------
// R A Y    C A S T I N G    
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: vpRayCasting
// Superclass: -
// Subclasses: -
///\deprecated

class VPRayCasting {
	private:
		VPRaySegment raySegment;
		int	  virtualYDimension;// Virtual "y" dimension, because the number of slices
							    // is often smaller than XZ dimension
		int   endOfSBand;		// The end of S-Band area (z point in volume space - x,y of centerFocalPoint) used in the inner structures visualization algorithm
		int   endOfTBand;		// The end of T-Band area (z point in volume space - x,y of centerFocalPoint) used in the inner structures visualization algorithm
		float dFront,			// "D" components of orthogonal planes equation...
			  dTop,
			  dLeft,
			  dBack,
			  dBottom,
			  dRight;
		float sampleStep;
		float ambientLight;
		float diffuseLight;
		int   shadingMethod;
		int   specularExponent;
		bool  specular;
		VPLight *light;
		VPPoint3D volumeScale;
		bool  lightVolumeComputation;
		int   negativeLightContribution;
		int   integrationType;			// Types of integration: volumes same size, volumes different size 
		int   firstSlice, lastSlice;	// For the integration of volumes with different size
		int	  valueCVP;					// Density value used for CVP technique

		// Livro...
		float			attenuate;        // Result of the shading.
		float			luminance;        // Grayscale result of classification
		float			alpha;            // The opacity of sample point
		float           lum,a,at;         // Temporary compositing variables
		float			gm;               // The gradient magnitude
		int				ivalue;           // Final value to be put in image buffer
		float			levWidth, levThreshold; // Values used in the classification
		unsigned char	alphaTable[65536];
		unsigned char	luminanceTable[65536];
		unsigned char	RGBtable[3*65536];

		// To load the information read from the "parameters file"
		int typeOfCuttingTool;
		int typeOfOpacityFunction;
		int backCuttingPlane;
		int distanceObliquePlane;
		int initialValueForLinearOpacity;
		VPPoint3D P1,P2,P3;	// to define the front cutting plane
		VPPoint3D backP1,backP2,backP3;	// to define the back cutting plane
		VPPoint3D cameraLocation;
		VPPoint3D p1Parallelepiped, p2Parallelepiped; 


	public:
		VPRayCasting();
		void  vpMakeRegistration();
		void  vpSetDefaultParameters (VPGraphicObj *volume, VPCamera *camera);
		void  vpRenderMonoGray(VPScene *s, VPCamera *c, int visualizationType, int opacityComputation, unsigned int **image);
		void  vpRenderLivroMonoGray(VPScene *s, VPCamera *c, int visualizationType, int opacityComputation, unsigned int **image);
		void  vpRenderMultiModal(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image);
		void  vpRenderLivroMultiModal(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image);
		void  vpRenderMonoColor(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image);
		void  vpRenderLivroMonoColor(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image);
		void  vpRenderNoLightMonoGray(VPScene *s, VPCamera *c, unsigned int **image);
		void  vpRenderLivroMultiInnerStructures(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image);
		void  vpRenderLivroInnerStructures(VPScene *s, VPCamera *c,  int opacityComputation, unsigned int **image); 
		void  vpRenderLivroMonoColorInnerStructures(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image); 		
		void  vpRenderLivro3VolMultiModal(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image);
		void  vpRenderMIP(VPScene *s, VPCamera *c, unsigned int **image);
		void  vpRenderLivroMIP(VPScene *s, VPCamera *c, unsigned int **image);
		void  vpRenderSubVolume();
		void  vpRenderMultiModal();
		void  vpRenderSideBySide();
		void  vpSetVariables(VPCamera *camera, VPVector3D &projectionDirection,
							VPPoint3D &minPlaneProjection, VPPoint3D &volumeDimension, 
							short int planes[], int &finalLineValue, int &finalColumnValue,
							VPVector3D &deltaX, VPVector3D &deltaY, VPVector3D &deltaZ, 
							VPGraphicObj *volume);
		void  vpSetCameraDefault(VPCamera *camera, VPGraphicObj *volume);
		void  vpSetLightDefault(VPLight *light, VPCamera *camera);
		void  vpFindIntersectionCandidatePlanes(short int planes[], VPVector3D dir);
		bool  vpFindIntersectionPoints(short int planes[], VPPoint3D maxB, VPPoint3D origin, 
									  VPVector3D dir, VPPoint3D &in, VPPoint3D &out,
									  float &tIn, float &tOut);
		bool  vpFindSphereIntersectionPoints(VPPoint3D origin, VPPoint3D center, float ray,
										     VPVector3D dir, VPPoint3D &point);
		void  vpBAKA(VPVector3D &gradient, int i, int j, int k);
		float vpTrilinearInterpolation(int i, int j, int k,  
									   VPGraphicObj *volume, VPPoint3D p);
		void  vpTriLinearGouraudInterpolation(int i, int j, int k,  
									   VPGraphicObj *volume, VPPoint3D p, 
									   float &color, float &light, bool &interpolate);
		void  vpTriLinearInterpolationAndGradientComputation(int i, int j, int k,  
											VPGraphicObj *volume, VPPoint3D p, 
											float &color, VPVector3D &g, bool &interpolate);
		void  vpOtherSampleColorAndShading(int i, int j, int k,  
									 VPGraphicObj *volume, 
									 VPPoint3D p, float previousIlight,
									 VPLight *l, float &color, float &Ilight);
		void vpGradientComputation(int i, int j, int k, VPGraphicObj *volume, 
									 VPPoint3D p, VPVector3D &g, bool &interpolate);
		void vpGradient(int i, int j, int k, VPGraphicObj *volume, 
									 VPVector3D &g);

		float vpProcessILight(VPVector3D g, VPLight *l);
		float vpProcessSpecularILight(VPVector3D g, VPLight *l, VPVector3D o);
		void  vpSetEndSBand(VPPoint3D sb, VPGraphicObj *volume, int vt);
		int   vpGetEndSBand();
		void  vpSetEndTBand(VPPoint3D tb, VPGraphicObj *volume, int vt);
		int   vpGetEndTBand();
		void  vpSetSampleStep(float s);
		float vpGetSampleStep();
		void  vpSetAmbientLight(float a);
		float vpGetAmbientLight();
		void  vpSetDiffuseLight(float d);
		float vpGetDiffuseLight();
		void  vpSetSpecularExponent(int s);
		int   vpGetSpecularExponent();
		void  vpSetSpecular(bool s);
		bool  vpGetSpecular();
		void  vpSetShadingMethod(int sm);
		int   vpGetShadingMethod();
		void  vpSetLightDirection(VPVector3D ld);
		VPVector3D vpGetLightDirection();
		void  vpSetTypeOfCuttingTool(int t);
		int   vpGetTypeOfCuttingTool();

		// To set "a" and "b"
		void vpDefineA(VPPoint3D &pIn, VPPoint3D pOut, VPPoint3D p1, VPVector3D deltaZ, float yCorrection, VPVector3D plane1Normal, VPGraphicObj *volume, VPPoint3D volumeDimension, float d);
		void vpDefineB(int i, int j, int k, float &alpha, VPGraphicObj *volume, VPPoint3D &pIn, int depth, int &ivalue);

		// Livro...
		void	Classify (float	*intensity, float *gm, float *luminance, float *alpha);
		void	ClassifyColor (float *intensity, float *gm, float *r, float	*g, float *b, float	*alpha);
		void	SetClassificationTable (void);
		void	SetClassificationTable2 (void);
		void	SetClassificationTable3 (void);
		float	Shade (VPVector3D gradient);
		float	ShadeSpecular (VPVector3D gradient, VPVector3D obsPoint);
		void	Gradient (int i, int j, int k, VPVector3D &gradient, float *gm, VPGraphicObj *volume);
		void	GradientSobel (int i, int j, int k, VPVector3D &gradient, float *gm, VPGraphicObj *volume);
		void	GradientBaka (int i, int j, int k, VPVector3D &gradient, float *gm, VPGraphicObj *volume);
};


#endif	// __VPRAYCASTING_H
