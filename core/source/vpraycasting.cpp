//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpraycasting.cpp
//  DESCRIPTION.: Contain the VPRayCasting class implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: August/08/2000
//  DESCRIPTION.: Constructors definition and methods implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: October/28/2002
//  DESCRIPTION.: New methods implementation. Code "adjust".
//
///////////////////////////////////////////////////////////////////


#include <vpraycasting.h>
#include <vpimage.h>
#include <vpvolume.h>
#include <vpplane.h>
#include <vpvector3d.h>
#include <vpdirectionallight.h>
#include <stdio.h>
#include <cmath>
#include <list>
using namespace std;


///////////////////////////////////////////////////////////////////
// Description: Class "VPRayCasting" constructor without parameter, 
//				that set the default ray casting visualization 
//				values.
// Parameters.: -
// Return.....: -

VPRayCasting::VPRayCasting () {
 char nomearq[30], aux[50];
 int tmp;
 FILE *fp;

 virtualYDimension = 0;
 dFront = dTop = dLeft = dBack = dBottom = dRight = 0.0;
 shadingMethod = LOCALSHADING; // GOURAUDSHADING;
 endOfSBand = 10;	
 endOfTBand = 18;
 light = NULL;
 lightVolumeComputation = true;

 printf("\nDigite o nome do arquivo: ");
 gets(nomearq);

 fp = fopen(nomearq, "r"); 

 if(fp == NULL) {
	printf("\n Erro na abertura do arquivo!");
	return;
 }

 fscanf(fp, "%s\n", aux); //Sample Step
 fscanf(fp, "%f\n", &sampleStep);

 fscanf(fp, "%s\n", aux); //TypeOfCuttingTool
 fscanf(fp, "%d\n", &typeOfCuttingTool);

 fscanf(fp, "%s\n", aux); //ThreePointsOfTheFrontCuttingPlane
 fscanf(fp, "%f%f%f\n", &P1.x,&P1.y,&P1.z);
 fscanf(fp, "%f%f%f\n", &P2.x,&P2.y,&P2.z);
 fscanf(fp, "%f%f%f\n", &P3.x,&P3.y,&P3.z);

 fscanf(fp, "%s\n", aux); //OneVolumePointToSimulateABackCuttingPlane
 fscanf(fp, "%d\n", &backCuttingPlane);

 fscanf(fp, "%s\n", aux); //ThreePointsOfTheBackCuttingPlane
 fscanf(fp, "%f%f%f\n", &backP1.x,&backP1.y,&backP1.z);
 fscanf(fp, "%f%f%f\n", &backP2.x,&backP2.y,&backP2.z);
 fscanf(fp, "%f%f%f\n", &backP3.x,&backP3.y,&backP3.z);

 fscanf(fp, "%s\n", aux); //TwoPointsToDefineTheParallelepipedCuttingTool
 fscanf(fp, "%f%f%f\n", &p1Parallelepiped.x,&p1Parallelepiped.y,&p1Parallelepiped.z);
 fscanf(fp, "%f%f%f\n", &p2Parallelepiped.x,&p2Parallelepiped.y,&p2Parallelepiped.z);

 fscanf(fp, "%s\n", aux); //OneVolumePointToSimulateADistanceForAnObliquePlane
 fscanf(fp, "%d\n", &distanceObliquePlane);

 fscanf(fp, "%s\n", aux); //LevThreshold_LevWidth_VAluesForClassification
 fscanf(fp, "%f%f\n", &levThreshold, &levWidth);
 
 fscanf(fp, "%s\n", aux); //KindOfOpacityFunction
 fscanf(fp, "%d\n", &typeOfOpacityFunction);

 fscanf(fp, "%s\n", aux); //InitialValueForLinearOpacity
 fscanf(fp, "%d\n", &initialValueForLinearOpacity);

 fscanf(fp, "%s\n", aux); //OptionalCameraLocation
 fscanf(fp, "%f%f%f\n", &cameraLocation.x,&cameraLocation.y,&cameraLocation.z);

 fscanf(fp, "%s\n", aux); //AmbientLight
 fscanf(fp, "%f\n", &ambientLight);

 fscanf(fp, "%s\n", aux); //DiffuseLight
 fscanf(fp, "%f\n", &diffuseLight);

 fscanf(fp, "%s\n", aux); //SpecularExponent
 fscanf(fp, "%d\n", &specularExponent);

 fscanf(fp, "%s\n", aux); //UseSpecular?
 fscanf(fp, "%d\n", &tmp);
 if (tmp)
	 specular = true;
 else
	 specular = false;

 fscanf(fp, "%s\n", aux); //negativeLightContribution?
 fscanf(fp, "%d\n", &negativeLightContribution);

 fscanf(fp, "%s\n", aux); //KindOfIntegration(1-VolumesWithTheSameSize,2-FunctionalVolumeSmall)
 fscanf(fp, "%d\n", &integrationType);

 fscanf(fp, "%s\n", aux); //RangeOfSlices(ForIntegration=2)
 fscanf(fp, "%d%d\n", &firstSlice,&lastSlice);

 fscanf(fp, "%s\n", aux); //Density value used in CVP technique
 fscanf(fp, "%d%d\n", &valueCVP);

 fclose(fp);

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetDefaultParameters" set some attributes
//				default values for volume visualization.
// Parameters.: VPGraphicObj *volume, VPCamera *camera
// Return.....: -

void VPRayCasting::vpSetDefaultParameters (VPGraphicObj *volume, VPCamera *camera) {

	VPPoint3D point;
	// To verify the smallest dimension and set the virtualVolumeDimension
	if ( ((VPImage *)volume)->vpGetXDimension() < ((VPVolume *)volume)->vpGetZDimension() )
	{
		if ( ((VPVolume *)volume)->vpGetYDimension() < (((VPVolume *)volume)->vpGetZDimension()/3) ) // if the number os slices is very small...
			virtualYDimension = ((VPVolume *)volume)->vpGetZDimension() / 2;
		else
			virtualYDimension = ((VPVolume *)volume)->vpGetZDimension();
	}
	else 
	{
		if ( ((VPVolume *)volume)->vpGetYDimension() < (((VPVolume *)volume)->vpGetXDimension()/3) ) // if the number os slices is very small...
			virtualYDimension = ((VPImage *)volume)->vpGetXDimension() / 2;
		else
			virtualYDimension = ((VPImage *)volume)->vpGetXDimension();
	}
	
	((VPVolume *)volume)->vpAdjustScale(((VPImage *)volume)->vpGetXDimension(), ((VPImage *)volume)->vpGetYDimension(), ((VPVolume *)volume)->vpGetZDimension());
	volumeScale = ((VPVolume *)volume)->vpGetScale();
	vpSetCameraDefault(camera, volume);
	vpSetLightDefault(light, camera);

	// Initialization of inner structures visualization parameters
	point = ((VPVolume *)volume)->vpGetCenterFocalPoint(); // the center of the "object"
	point.z = 0;
	while ( ((VPVolume *)volume)->vpGetValue(point.x,point.y,point.z) <= 40 )
			point.z++;
	endOfSBand = point.z + ( ((VPVolume *)volume)->vpGetZDimension() * 0.06 ); // 6% 	
	endOfTBand = endOfSBand + ( ((VPVolume *)volume)->vpGetZDimension() * 0.06 );
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderLivroMonoGray" implement the brute  
//				force ray casting visualization algorithm for gray 
//				levels, available in "Introduction to Volume 
//				Rendering" book.
// Parameters.: VPScene *s, VPCamera *c (active camera), 
//				int visualizationType, unsigned int **image 
//				(pointer to the image);
// Return.....: -

void VPRayCasting::vpRenderLivroMonoGray(VPScene *s, VPCamera *c, int visualizationType, 
								  int opacityComputation, unsigned int **image) {
 int line=0, column=0, depth=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0, flagCVP=0;
 short int planes[6];
 float yCorrection=0, sIn=0, sOut=0, sampleColor, antColor;
 VPVector3D projectionDirection, gradient, deltaX, deltaY, deltaZ, 
			vAux, correctProjectionDirection, antGradient;
 VPPoint3D p1, pIn, pOut, pAux, minPlaneProjection, volumeDimension, 
	       origin, temp; 
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();

 // To get a pointer to the volume object
 VPGraphicObj *volume = *objects.begin();

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // Get color and opacity tables
 VPTable controlTables = ((VPVolume *)volume)->vpGetControlTables();

 // Verify if it is the DEFAULTVIS visualization type to set the default parameters
 if(visualizationType==DEFAULTVIS) {
	 vpSetDefaultParameters (volume, camera);
 	 visualizationType = MONOGRAYVIS;
 }

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume);
 
 // For specular processing
 VPPoint3D observer = camera->vpGetLocation();	 
 VPVector3D obsPoint;							

 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 switch(typeOfOpacityFunction)
 {
	case LEVOYSURFACEOPACITY: SetClassificationTable();
						break;
	case LINEAROPACITY: SetClassificationTable2();
						break;
 }

 float magnitudeMax = sqrt( (255*sqrt(3)) * (255*sqrt(3)) + (255*sqrt(3)) * (255*sqrt(3)) + (255*sqrt(3)) * (255*sqrt(3)) );

 // To set the light with the same direction of the observer...
 correctProjectionDirection = projectionDirection;
 correctProjectionDirection.x = -correctProjectionDirection.x; 
 correctProjectionDirection.y = -correctProjectionDirection.y; 
 correctProjectionDirection.z = -correctProjectionDirection.z; 
 // To set light position
 //correctProjectionDirection.x = 60; 
 //correctProjectionDirection.y = -40; 
 //correctProjectionDirection.z = -30;
 //correctProjectionDirection.vpNormalize();
 vpSetLightDirection(correctProjectionDirection);


 // 2- Cutting technique: one cut plane for the sphere example
 // plane1 = P1(0,40,0) P2(99,40,0) P3(0,80,99)
 // vet1 (P1->P2) = (99,0,0) 
 // vet2 (P1->P3) = (0,40,99) 
 // 2- Cutting technique: one cut plane for the head.i example
 // plane1 = P1(0,15,0) P2(112,15,0) P3(0,70,127)
 // vet1 (P1->P2) = (112,0,0) 
 // vet2 (P1->P3) = (0,70,127) 
  VPVector3D vet1(P2.x-P1.x,P2.y-P1.y,P2.z-P1.z), vet2(P3.x-P1.x,P3.y-P1.y,P3.z-P1.z), plane1Normal; // Resultado bom se "if(f>0&&g<0) pIn=pInNew;"
  vet1.vpNormalize(); vet2.vpNormalize(); 
  plane1Normal = vet1.vpCrossProduct(vet2);
  float d = - (plane1Normal.x*0 + plane1Normal.y*P1.y + plane1Normal.z*0); 


  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 
     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples

			// variables initialization
			gradient.x = gradient.y = gradient.z = 0;
			lum   = (float) 0.0;
			a     = (float) 0.0;
			sampleColor   = (float) 0.0;
			depth = ivalue = flagCVP = 0;
			antColor = antGradient.x = antGradient.y = antGradient.z = 0;

			// BAKA corte: a=conteudo(valor densidade) b=r2
			bool FLAG=false;

			pIn.y = pIn.y*yCorrection;

			vpDefineA(pIn, pOut, p1, deltaZ, yCorrection, plane1Normal, volume, volumeDimension, d);

			// 7- Cutting technique: value range (a!=b: conteúdo) 
			//	  Cutting by inclusion: a=first>=(Rp,150)  and b=first>=(Rp,250) 
			//while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) && sampleColor<135) { //200, 130
			//	i = (int) (pIn.x); //(column);
			//	j = (int) (pIn.y); //(line);
			//	k = (int) (pIn.z); //(depth);
			//	sampleColor = ((VPVolume *)volume)->vpGetValue(i,j,k);
			//	pIn = pIn + deltaZ; // projectionDirection; 
			//}
			//while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) && sampleColor>70) { //180, 120
			//	i = (int) (pIn.x); //(column);
			//	j = (int) (pIn.y); //(line);
			//	k = (int) (pIn.z); //(depth);
			//	sampleColor = ((VPVolume *)volume)->vpGetValue(i,j,k);
			//	pIn = pIn + deltaZ; // projectionDirection; 
			//}


			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension())) { 
				if (typeOfCuttingTool==TWOCUTPLANESINCLUSIONOPAC) 
					depth++;

				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);


				// Sample processing in accordance with the shading method (LOCALSHADING)
				GradientSobel (i, j, k, gradient, &gm, volume); 

				sampleColor = vpTrilinearInterpolation(i,j,k,volume,pIn);


				// BAKA corte: a=conteudo(valor densidade) b=r2
				//if ( FLAG==false )			// processa as amostras somente depois de encontrar
				//	if ( sampleColor < 160 ) {	// a primeira amostra com valor 230
				//		 pIn = pIn + deltaZ; continue;
				//	}
				//	else
				//		FLAG = true;

				//if ( FLAG==false )				// processa as amostras somente depois de encontrar
				//	if ( sampleColor < 100 ) {	// a primeira amostra com valor 230
				//		 pIn = pIn + deltaZ; continue;
				//	}
				//	else 
				//		FLAG = true;			// pára de processar as amostras depois
				//else if ( sampleColor > 250)	// que encontra o valor 230
				//			break;
				
				// BAKA corte: a=conteudo(valor densidade) b=conteudo(valor densidade) 
				//if ( FLAG==false )			// processa as amostras com densidade entre 200 e 250 						
				//	if ( sampleColor < 110 ) {	
				//		 pIn = pIn + deltaZ; continue;
				//	}
				//	else {
				//		if ( sampleColor > 130 ) 
				//			break;
				//		else
				//			FLAG = true;
				//	}

				// 7- BAKA for value range (a!=b: conteúdo)
				// if ( sampleColor > 250 ) break;

				// 1- BAKA for just one slice (line arround the slice)
				if (OBLIQUESLICE==typeOfCuttingTool) {
					if ( ( (i<4) || (i>=(volumeDimension.x-4)) ) || ( (j<4) || (j>=(((VPImage *)volume)->vpGetYDimension()-4)) ) || ( (k<4) || (k>=(volumeDimension.z-4)) ) )
						sampleColor = 255;
					ivalue = (int) sampleColor;
				}
				if (sampleColor>255 || sampleColor<0)
					printf("i=%d j=%d k=%d",i,j,k);
				Classify (&sampleColor, &gm, &luminance, &alpha);

				alpha = 1 - pow(1-alpha, (1 / (1/sampleStep)) );
				if (specular) {
					obsPoint.x = i - observer.x;
					obsPoint.y = j - observer.y;
					obsPoint.z = k - observer.z;
					obsPoint.vpNormalize();
					attenuate = ShadeSpecular(gradient,obsPoint);
				}
				else 
					attenuate = Shade (gradient); 
				luminance = luminance * attenuate;

				if (typeOfCuttingTool == TWOCUTPLANESINCLUSIONOPAC) { // proccess the sample IN two cutting planes (needs a different "while" implementation)
					if ( (alpha && depth<=1) || (alpha && ( (pIn.y<(backCuttingPlane+1)) && (pIn.y>(backCuttingPlane-1)) )) ) {
							ivalue = (int) vpTrilinearInterpolation(i,j,k,volume,pIn);
							break; 
					}
				}
				else
					vpDefineB(i, j, k, alpha, volume, pIn, depth, ivalue);

				//*----- Front-to-back alpha blending compositing
				if (alpha > 0.0 && typeOfCuttingTool != TWOCUTPLANESINCLUSIONOPAC)	{
						at = alpha * (1.0f - a);
						lum = lum + (luminance * at);
						a = a + at;
						//*----- do early ray termination 
						if (a > 0.97)
							break;
				}

				// Next sample
				pIn = pIn + deltaZ; // projectionDirection; 

				if(typeOfCuttingTool == CVP) {
						if (flagCVP == 0 && sampleColor > valueCVP)
							flagCVP = sampleColor;
						else if (flagCVP > valueCVP && sampleColor < valueCVP)
							break;
				}

				if (typeOfCuttingTool==OBLIQUESLICE)
					pIn.x = pIn.y = pIn.z = -10;

			} // while (depth)

			if ( (typeOfCuttingTool != TWOCUTPLANESINCLUSIONOPAC) && (typeOfCuttingTool != OBLIQUESLICE))
				ivalue = (int) lum;
			if (a < 0)
					image[line][column] = (unsigned int) (255<<24 | 102<<16 | 179<<8 | 230);
			else
					image[line][column] = (unsigned int) (255<<24 | ivalue<<16 | ivalue<<8 | ivalue);
	
		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderLivroMultiModal" implement the brute  
//				force ray casting visualization algorithm that 
//				works with two volumes.
// Parameters.: VPScene *s, VPCamera *c (active camera), 
//				int visualizationType, unsigned int ***image 
//				(pointer to the image);
// Return.....: -

void VPRayCasting::vpRenderLivroMultiModal(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, ivalue1, ivalue2, ivalue3, depth,
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0, i2=0, j2=0, k2=0, flagCVP=0;
 short int planes[6];
 float previousIlight=0, Ilight=0, sampleColor1=0, sampleColor2=0, 
	   sIn=0, sOut=0, rayColorRed=0, rayColorGreen=0, rayColorBlue=0, 
	   rayOpacity=0, sampleOpacity1=0, sampleOpacity2=0, yCorrection=0, 
	   toRaio=0, auxLight=0, r, g, b;
 VPVector3D projectionDirection, gradient,
			deltaX, deltaY, deltaZ, vAux, correctProjectionDirection;
 VPPoint3D p1, pIn, pIn2, pOut, pAux, minPlaneProjection, volumeDimension, 
	       volume2Dimension, origin, temp, point; 
 VPColor color1, color2;
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();
 list<VPGraphicObj*>::const_iterator iter;

 // To get a pointer to the first volume object
 VPGraphicObj *volume1 = *objects.begin();
 iter = objects.begin();
 iter++;
 // To get a pointer to the second volume object
 VPGraphicObj *volume2 = *iter;

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // For specular processing
 VPPoint3D observer = camera->vpGetLocation();	 
 VPVector3D obsPoint;	

 // Get color and opacity tables 
 VPTable controlTables1 = ((VPVolume *)volume1)->vpGetControlTables();
 VPTable controlTables2 = ((VPVolume *)volume2)->vpGetControlTables();

 // Verify if it is the first call for the visualization algorithm to set the default parameters
 if(virtualYDimension==0) {
	// To verify the smallest dimension and set the virtualVolumeDimension
	if ( ((VPImage *)volume1)->vpGetXDimension() < ((VPVolume *)volume1)->vpGetZDimension() )
		virtualYDimension = ((VPVolume *)volume1)->vpGetZDimension();
	else 
		virtualYDimension = ((VPImage *)volume1)->vpGetXDimension();	

	((VPVolume *)volume1)->vpAdjustScale(((VPImage *)volume1)->vpGetXDimension(), ((VPImage *)volume1)->vpGetYDimension(), ((VPVolume *)volume1)->vpGetZDimension());
	((VPVolume *)volume2)->vpAdjustScale(((VPImage *)volume2)->vpGetXDimension(), ((VPImage *)volume2)->vpGetYDimension(), ((VPVolume *)volume2)->vpGetZDimension());
	vpSetCameraDefault(camera, volume1);
	vpSetLightDefault(light, camera);

	// Initialization of inner structures visualization parameters 
	point = ((VPVolume *)volume1)->vpGetCenterFocalPoint(); // the center of the "object"
	point.z = 0;
	while ( ((VPVolume *)volume1)->vpGetValue(point.x,point.y,point.z) <= 40 )
			point.z++;
	endOfSBand = point.z + ( ((VPVolume *)volume1)->vpGetZDimension() * 0.06 ); // 6% 	
	endOfTBand = endOfSBand + ( ((VPVolume *)volume1)->vpGetZDimension() * 0.06 );
 }

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume1);
 volume2Dimension.x = ((VPImage *)volume2)->vpGetXDimension(); 
 volume2Dimension.y = ((VPImage *)volume2)->vpGetYDimension(); 
 volume2Dimension.z = ((VPVolume *)volume2)->vpGetZDimension(); 
 volume2Dimension -= 1; // because of the volume matrix index

 yCorrection = ((float) ((VPImage *)volume1)->vpGetYDimension()) / ((float) virtualYDimension);

 switch(typeOfOpacityFunction)
 {
	case LEVOYSURFACEOPACITY: SetClassificationTable();
						break;
	case LINEAROPACITY: SetClassificationTable2();
						break;
 }

 // To set the light with the same direction of the observer...
 correctProjectionDirection = projectionDirection;
 correctProjectionDirection.x = -correctProjectionDirection.x; 
 correctProjectionDirection.y = -correctProjectionDirection.y; 
 correctProjectionDirection.z = -correctProjectionDirection.z; 
 // To set a different light position
 //correctProjectionDirection.x = 0;
 //correctProjectionDirection.y = -40;
 //correctProjectionDirection.z = -30;
 //correctProjectionDirection.vpNormalize();
 vpSetLightDirection(correctProjectionDirection);

 // 2- Cutting technique: one cut plane 
  VPVector3D vet1(P2.x-P1.x,P2.y-P1.y,P2.z-P1.z), vet2(P3.x-P1.x,P3.y-P1.y,P3.z-P1.z), plane1Normal; // Resultado bom se "if(f>0&&g<0) pIn=pInNew;"
  vet1.vpNormalize(); vet2.vpNormalize(); 
  plane1Normal = vet1.vpCrossProduct(vet2);
  float d = - (plane1Normal.x*0 + plane1Normal.y*P1.y + plane1Normal.z*0); 

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			sampleColor1 = sampleColor2 = sampleOpacity1 = sampleOpacity2 = (float) 0.0;
			rayColorRed = rayColorGreen = rayColorBlue = rayOpacity = (float) 0.0;
			gradient.x = gradient.y = gradient.z = (float) 0.0;
			lum = a = (float) 0.0;
			depth = ivalue = ivalue1 = ivalue2 = ivalue3 = flagCVP = 0;

			// BAKA corte: a=conteudo(valor densidade) b=r2
			bool FLAG=false;
			int intFLAG=0;

			pIn.y = pIn.y*yCorrection;

			vpDefineA(pIn, pOut, p1, deltaZ, yCorrection, plane1Normal, volume1, volumeDimension, d);

			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume1)->vpGetYDimension())) { 

				if (typeOfCuttingTool==TWOCUTPLANESINCLUSIONOPAC) 
					depth++;

				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);

				// Sample processing in accordance with the shading method (LOCALSHADING)
				GradientSobel (i, j, k, gradient, &gm, volume1); 
				sampleColor1 = vpTrilinearInterpolation(i,j,k,volume1,pIn);

				if ( ( volumeDimension.x != volume2Dimension.x ) ||
					 ( ((VPImage *)volume1)->vpGetYDimension() != volume2Dimension.y ) ||
					 ( volumeDimension.z != volume2Dimension.z ) ) {
					switch(integrationType)
					{
						case 1:
								i2 = (volume2Dimension.x*i) / volumeDimension.x;
								k2 = (volume2Dimension.z*k) / volumeDimension.z;
								pIn2.x = (volume2Dimension.x*pIn.x) / volumeDimension.x;
								pIn2.y = (volume2Dimension.y*pIn.y) / ( ((VPImage *)volume1)->vpGetYDimension() - 1 );
								j2 = (int)(pIn2.y); // por causa do problema de arredondamento...
								pIn2.z = (volume2Dimension.z*pIn.z) / volumeDimension.z;
								sampleColor2 = vpTrilinearInterpolation(i2,j2,k2,volume2,pIn2);
								break;
						case 2:
								if ( (j>=firstSlice) && (j<=lastSlice) )
								{
									i2 = (volume2Dimension.x*i) / volumeDimension.x;
									k2 = (volume2Dimension.z*k) / volumeDimension.z;
									pIn2.x = (volume2Dimension.x*pIn.x) / volumeDimension.x;
									pIn2.y = ( volume2Dimension.y * (pIn.y-(float)(firstSlice))) / ((float)(lastSlice-firstSlice+1));
									j2 = (int)(pIn2.y); // por problemas de precisão foi a melhor solução para o cálculo de j2
									pIn2.z = (volume2Dimension.z*pIn.z) / volumeDimension.z;
									sampleColor2 = vpTrilinearInterpolation(i2,j2,k2,volume2,pIn2);
								}
								break;
					}

				}
				else
					sampleColor2 = vpTrilinearInterpolation(i,j,k,volume2,pIn);

				// BAKA corte: a=conteudo(valor densidade) b=r2
				//if ( FLAG==false )			// processa as amostras somente depois de encontrar
				//	if ( sampleColor2 < 150 ) {	// a primeira amostra com valor 100 no volume2 (funcional)
				//		 pIn = pIn + deltaZ; continue;
				//	}
				//	else
				//		FLAG = true;

				// BAKA corte: a=conteudo(valor densidade) b=r2
				//if ( FLAG==false )			// processa as amostras somente depois de encontrar
				//	if ( sampleColor1 < 230 ) {	// a primeira amostra com valor 230 no volume1 (anatomia)
				//		 pIn = pIn + deltaZ; continue;
				//	}
				//	else
				//		FLAG = true;

				// BAKA corte: a=conteudo(valor densidade) b=conteudo(valor densidade) 
				//if ( FLAG==false )			// processa as amostras com densidade entre 100 e 170 
				//								// (considerando o volume que tem a anatomia - sampleColor1)
				//	if ( sampleColor1 < 100 ) {	
				//		 pIn = pIn + deltaZ; continue;
				//}
				//	else {
				//		if ( sampleColor1 > 170 ) 
				//			break;
				//		else
				//			FLAG = true;
				//	}

				// BAKA corte: a=conteudo(valor densidade) b=conteudo(valor densidade) 
				//if ( intFLAG == 0 )			// não processa as amostras com densidade entre 100 e 170
				//{								// (considerando o volume que tem a anatomia - sampleColor1)
				//	if ( sampleColor1 > 100 ) {	
				//		 intFLAG = 1;
				//		 pIn = pIn + deltaZ; continue;
				//	}
				//}
				//else {
				//		if ( intFLAG == 1 ) 
				//		{
				//			if (sampleColor1 != 255) {
				//				pIn = pIn + deltaZ; 
				//				continue;
				//			}
				//			else
				//				intFLAG = 2;
				//		}
				//}

				// BAKA corte: a=geometria(r1) b=conteúdo
				//if ( sampleColor2 >= 200 )	// pára de processar as amostras quando encontra
				//	break;					// a primeira amostra com valor 200 no volume2 ("funcional")

				// BAKA corte: a=geometria(r1) b=conteúdo
				//if ( FLAG == false)
				//	if ( sampleColor2 < 200 ) 	// começa a processar as amostras quando encontra
				//	{	pIn = pIn + deltaZ;		// a primeira amostra com valor 200 no volume2 ("funcional")
				//		continue;
				//	}
				//	else FLAG = TRUE;

				color2 = controlTables2.vpGetColor(sampleColor2); //

				ClassifyColor (&sampleColor1, &gm, &r, &g, &b, &alpha);
				alpha = 1 - pow(1-alpha, (1 / (1/sampleStep)) );
				if (specular) {
					obsPoint.x = i - observer.x;
					obsPoint.y = j - observer.y;
					obsPoint.z = k - observer.z;
					obsPoint.vpNormalize();
					attenuate = ShadeSpecular(gradient,obsPoint);
				}
				else 
					attenuate = Shade (gradient); 

				if(integrationType==1)
				{
					r = color2.vpGetR() * attenuate; //r * attenuate;
					g = color2.vpGetG() * attenuate; //g * attenuate;
					b = color2.vpGetB() * attenuate; //b * attenuate;
				}
				else
				{
					if ( (j>=firstSlice) && (j<=lastSlice) )
					{
						r = color2.vpGetR() * attenuate; //r * attenuate;
						g = color2.vpGetG() * attenuate; //g * attenuate;
						b = color2.vpGetB() * attenuate; //b * attenuate;
					}
					else
					{
						Classify (&sampleColor1, &gm, &luminance, &alpha);
						r = luminance * attenuate; //r * attenuate;
						g = luminance * attenuate; //g * attenuate;
						b = luminance * attenuate; //b * attenuate;
					}
				}

				if (typeOfCuttingTool == TWOCUTPLANESINCLUSIONOPAC) { // proccess the sample IN two cutting planes (needs a different "while" implementation)
					if ( (alpha && depth<=1) || (alpha && ( (pIn.y<(backCuttingPlane+1)) && (pIn.y>(backCuttingPlane-1)) )) ) {
							ivalue1 = (int) r; //vpTrilinearInterpolation(i2,j2,k2,volume2,pIn2);
							ivalue2 = (int) g;
							ivalue3 = (int) b;
							break; 
					}
				}
				else 
					vpDefineB(i, j, k, alpha, volume1, pIn, depth, ivalue);

				//*----- Front-to-back alpha blending compositing
				if (alpha > 0.0)
				{
						at = alpha * (1.0f - a);
						rayColorRed   = rayColorRed   + (r * at);
						rayColorGreen = rayColorGreen + (g * at);
						rayColorBlue  = rayColorBlue  + (b * at);
						a = a + at;

						//*----- do early ray termination 
						if (a > 0.97)
							break;
				}

				// Next sample
				pIn = pIn + deltaZ; // projectionDirection; 

				if(typeOfCuttingTool == CVP) {
						if (flagCVP == 0 && sampleColor1 > valueCVP)
							flagCVP = sampleColor1;
						else if (flagCVP > valueCVP && sampleColor1 < valueCVP)
							break;
				}
				
				if (typeOfCuttingTool == OBLIQUESLICE) {
					if ( ( (i<4) || (i>=(volumeDimension.x-4)) ) && ( (j<4) || (j>=(((VPImage *)volume1)->vpGetYDimension()-4)) ) && ( (k<4) || (k>=(volumeDimension.z-4)) ) )
						ivalue1 = ivalue2 = ivalue3 = 0;
					else {
						if (alpha > 0.0) {
							ivalue1 = (int) r; 
							ivalue2 = (int) g;
							ivalue3 = (int) b;
						}
					}
					pIn.x = pIn.y = pIn.z = -10;
				}

			} // while (depth)

			if ( (typeOfCuttingTool != TWOCUTPLANESINCLUSIONOPAC) && (typeOfCuttingTool != OBLIQUESLICE))
			{
				ivalue1 = (int) rayColorRed;
				ivalue2 = (int) rayColorGreen;
				ivalue3 = (int) rayColorBlue;
			}


			if (a < 0) {
					image[line][column][red] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][green] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][blue] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
			}
			else {
					image[line][column][red] = (unsigned short int) (255<<24 | ivalue1<<16 | ivalue1<<8 | ivalue1);
					image[line][column][green] = (unsigned short int) (255<<24 | ivalue2<<16 | ivalue2<<8 | ivalue2);
					image[line][column][blue] = (unsigned short int) (255<<24 | ivalue3<<16 | ivalue3<<8 | ivalue3);
			}

		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}	


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderLivroMonoColor" implement the brute  
//				force ray casting visualization algorithm that 
//				works with colors.
// Parameters.: VPScene *s, VPCamera *c (active camera), 
//				int visualizationType, unsigned int ***image 
//				(pointer to the image);
// Return.....: -

void VPRayCasting::vpRenderLivroMonoColor(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, ivalue1, ivalue2, ivalue3,
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0, depth, flagCVP=0;
 short int planes[6];
 float sampleColor=0, sIn=0, sOut=0, tmp,
	   rayColorRed=0, rayColorGreen=0, rayColorBlue=0, rayOpacity=0, 
	   sampleOpacity=0, yCorrection=0, toRaio=0, auxLight=0, r, g, b;

 VPVector3D projectionDirection, gradient,
			deltaX, deltaY, deltaZ, vAux, correctProjectionDirection;
 VPPoint3D p1, pIn, pOut, pAux, minPlaneProjection, volumeDimension, origin, temp; 
 VPColor color;
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();

 // To get a pointer to the volume object
 VPGraphicObj *volume = *objects.begin();

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // For specular processing
 VPPoint3D observer = camera->vpGetLocation();	 
 VPVector3D obsPoint;							

 // Get color and opacity tables
 VPTable controlTables = ((VPVolume *)volume)->vpGetControlTables();

 // Verify if it is the first call for the visualization algorithm to set the default parameters
 if(virtualYDimension==0) 
	 vpSetDefaultParameters (volume, camera);

 if (lightVolumeComputation) {
    ((VPVolume *)volume)->vpProcessLightedVolume(((VPDirectionalLight *)light)->vpGetDirection(), ambientLight);
	lightVolumeComputation = false;
 }

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume);

 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 switch(typeOfOpacityFunction)
 {
	case LEVOYSURFACEOPACITY: SetClassificationTable();
						break;
	case LINEAROPACITY: SetClassificationTable2();
						break;
 }

 // To set the light with the same direction of the observer...
 correctProjectionDirection = projectionDirection;
 correctProjectionDirection.x = -correctProjectionDirection.x; 
 correctProjectionDirection.y = -correctProjectionDirection.y; 
 correctProjectionDirection.z = -correctProjectionDirection.z; 
 // To set light position
 //correctProjectionDirection.x = 60; 
 //correctProjectionDirection.y = -40; 
 //correctProjectionDirection.z = -30;
 correctProjectionDirection.vpNormalize();

 vpSetLightDirection(correctProjectionDirection);

 // 2- Cutting technique: one cut plane 
  VPVector3D vet1(P2.x-P1.x,P2.y-P1.y,P2.z-P1.z), vet2(P3.x-P1.x,P3.y-P1.y,P3.z-P1.z), plane1Normal; // Resultado bom se "if(f>0&&g<0) pIn=pInNew;"
  vet1.vpNormalize(); vet2.vpNormalize(); 
  plane1Normal = vet1.vpCrossProduct(vet2);
  float d = - (plane1Normal.x*0 + plane1Normal.y*P1.y + plane1Normal.z*0); 

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			gradient.x = gradient.y = gradient.z = 0;
			lum   = (float) 0.0;
			a     = (float) 0.0;
			sampleColor = sampleOpacity = (float) 0.0;
			rayColorRed = rayColorGreen = rayColorBlue = rayOpacity = 0.0;
			depth = ivalue = ivalue1 = ivalue2 = ivalue3 = flagCVP = 0;

			pIn.y = pIn.y*yCorrection;

			vpDefineA(pIn, pOut, p1, deltaZ, yCorrection, plane1Normal, volume, volumeDimension, d);

			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension())) { 
				if (typeOfCuttingTool==TWOCUTPLANESINCLUSIONOPAC) 
					depth++;

				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);

				// Sample processing in accordance with the shading method (LOCALSHADING)
				GradientSobel (i, j, k, gradient, &gm, volume); 
				tmp = vpTrilinearInterpolation(i,j,k,volume,pIn);
				color = controlTables.vpGetColor(tmp); //

				ClassifyColor (&tmp, &gm, &r, &g, &b, &alpha);
				alpha = 1 - pow(1-alpha, (1 / (1/sampleStep)) );

				if (specular) {
					obsPoint.x = i - observer.x;
					obsPoint.y = j - observer.y;
					obsPoint.z = k - observer.z;
					obsPoint.vpNormalize();
					attenuate = ShadeSpecular(gradient,obsPoint);
				}
				else 
					attenuate = Shade (gradient); 
				r = color.vpGetR() * attenuate; //r * attenuate;
				g = color.vpGetG() * attenuate; //g * attenuate;
				b = color.vpGetB() * attenuate; //b * attenuate;

				if (typeOfCuttingTool == TWOCUTPLANESINCLUSIONOPAC) { // proccess the sample IN two cutting planes (needs a different "while" implementation)
					if ( (alpha && depth<=1) || (alpha && ( (pIn.y<(backCuttingPlane+1)) && (pIn.y>(backCuttingPlane-1)) )) ) {
							ivalue1 = (int) r; //vpTrilinearInterpolation(i2,j2,k2,volume2,pIn2);
							ivalue2 = (int) g;
							ivalue3 = (int) b;
							break; 
					}
				}
				else
					vpDefineB(i, j, k, alpha, volume, pIn, depth, ivalue);

				//*----- Front-to-back alpha blending compositing
				if (alpha > 0.0)
				{
						at = alpha * (1.0f - a);
						rayColorRed   = rayColorRed   + (r * at);
						rayColorGreen = rayColorGreen + (g * at);
						rayColorBlue  = rayColorBlue  + (b * at);
						a = a + at;

						//*----- do early ray termination 
						if (a > 0.97)
							break;
				}

				// Next sample
				pIn = pIn + deltaZ; // projectionDirection; 

				if(typeOfCuttingTool == CVP) {
						if (flagCVP == 0 && tmp > valueCVP)
							flagCVP = tmp;
						else if (flagCVP > valueCVP && tmp < valueCVP)
							break;
				}

				if (typeOfCuttingTool==OBLIQUESLICE) {
					if ( ( (i<4) || (i>=(volumeDimension.x-4)) ) && ( (j<4) || (j>=(((VPImage *)volume)->vpGetYDimension()-4)) ) && ( (k<4) || (k>=(volumeDimension.z-4)) ) )
						ivalue1 = ivalue2 = ivalue3 = 0;
					else {
						if (alpha > 0.0) {
							ivalue1 = (int) r; 
							ivalue2 = (int) g;
							ivalue3 = (int) b;
						}
					}
					pIn.x = pIn.y = pIn.z = -10;
				}

			} // while (depth)

			if ( (typeOfCuttingTool != TWOCUTPLANESINCLUSIONOPAC) && (typeOfCuttingTool != OBLIQUESLICE))
			{
				ivalue1 = (int) rayColorRed;
				ivalue2 = (int) rayColorGreen;
				ivalue3 = (int) rayColorBlue;
			}
			if (a < 0) {
					image[line][column][red] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][green] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][blue] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
			}
			else {
					image[line][column][red] = (unsigned short int) (255<<24 | ivalue1<<16 | ivalue1<<8 | ivalue1);
					image[line][column][green] = (unsigned short int) (255<<24 | ivalue2<<16 | ivalue2<<8 | ivalue2);
					image[line][column][blue] = (unsigned short int) (255<<24 | ivalue3<<16 | ivalue3<<8 | ivalue3);
			}
	
		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderNoLightMonoGray" implement the brute  
//				force ray casting visualization algorithm for gray 
//				levels and whithout light.
// Parameters.: VPScene *s, VPCamera *c (active camera), 
//				unsigned int **image (pointer to the image);
// Return.....: -

void VPRayCasting::vpRenderNoLightMonoGray(VPScene *s, VPCamera *c, unsigned int **image) {
 int line=0, column=0, depth=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0;
 short int planes[6];
 float previousIlight=0, Ilight=0, sampleColor=0, rayColor=0, rayOpacity=0, 
	   sampleOpacity=0, yCorrection=0, sIn=0, sOut=0;

 VPVector3D projectionDirection, deltaX, deltaY, deltaZ, vAux;
 VPPoint3D p1, pIn, pOut, pAux, minPlaneProjection, volumeDimension, origin, temp; 
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();

 // To get a pointer to the volume object
 VPGraphicObj *volume = *objects.begin();

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // Get color and opacity tables
 VPTable controlTables = ((VPVolume *)volume)->vpGetControlTables();

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume);

 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			sampleColor = 0;
			rayColor = rayOpacity = 0.0;
			previousIlight = Ilight = 0.0;

			pIn.y = pIn.y*yCorrection;
		
			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension())) { 

				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);
	
				// Just interpolation...
				sampleColor = vpTrilinearInterpolation(i,j,k,volume,pIn);
				if (sampleColor > 255) 
					sampleColor = 255;
				sampleOpacity = controlTables.vpGetLinearOpacityValue( (unsigned int) sampleColor );

				rayColor += (sampleColor * sampleOpacity);
				rayOpacity += sampleOpacity;
			
				if (sampleOpacity == 1) break;

				// Next sample
				pIn = pIn + deltaZ; // projectionDirection; 
			} // while (depth)
            
			if ( (rayColor != 0.0) && (rayOpacity != 0.0) ) {
				sampleColor = rayColor / rayOpacity;
				// store pixel color
				image[line][column] = (unsigned int) sampleColor;
			}

		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderLivroMultiInnerStructures" implement the 
//				inner structures visualization algorithm (based in  
//				the brute force ray casting algorithm).
// Parameters.: VPScene *s (scene object), 
//				VPCamera *c (active camera), 
//				int opacityComputation (opacity computation function),
//				unsigned int ***image (pointer to the image)
// Return.....: -

void VPRayCasting::vpRenderLivroMultiInnerStructures(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image) {
 int line=0, column=0, i=0, j=0, k=0, i2=0, j2=0, k2=0, initialLineValue=0, finalLineValue=0, 
	 depth=0, initialColumnValue=0, finalColumnValue=0, sampleNumberSBand=0, sampleNumberTBand=0, 
	 ivalue1, ivalue2, ivalue3;
 short int planes[6];
 float sampleColor1=0, sampleColor2=0, r, g, b, 
	   sIn=0, sOut=0, rayColorRed=0, rayColorGreen=0, rayColorBlue=0, 
	   rayOpacity=0, sampleOpacity1=0, sampleOpacity2=0, yCorrection=0, 
	   toRaio=0, auxLight=0, distance1=0, distance2=0;

 VPVector3D projectionDirection, gradient, auxProjectionDirection, auxDelta,
			deltaX, deltaY, deltaZ, vAux, correctProjectionDirection;
 VPPoint3D p1, pIn, pIn2, pOut, pAux, minPlaneProjection, volumeDimension, 
	       volume2Dimension, origin, temp, centerFocalPoint, auxSBand, auxTBand; 
 VPColor color1, color2;
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();
 list<VPGraphicObj*>::const_iterator iter;

 // To get a pointer to the first volume object
 VPGraphicObj *volume1 = *objects.begin();
 iter = objects.begin();
 iter++;
 // To get a pointer to the second volume object
 VPGraphicObj *volume2 = *iter;

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // For specular processing
 VPPoint3D observer = camera->vpGetLocation();	 
 VPVector3D obsPoint;							

 VPTable controlTables1 = ((VPVolume *)volume1)->vpGetControlTables();
 VPTable controlTables2 = ((VPVolume *)volume2)->vpGetControlTables();

 // Verify if it is the first call for the visualization algorithm to set the default parameters
 if(virtualYDimension==0) {
	// To verify the smallest dimension and set the virtualVolumeDimension
	if ( ((VPImage *)volume1)->vpGetXDimension() < ((VPVolume *)volume1)->vpGetZDimension() )
		virtualYDimension = ((VPVolume *)volume1)->vpGetZDimension();
	else 
		virtualYDimension = ((VPImage *)volume1)->vpGetXDimension();	

	((VPVolume *)volume1)->vpAdjustScale(((VPImage *)volume1)->vpGetXDimension(), ((VPImage *)volume1)->vpGetYDimension(), ((VPVolume *)volume1)->vpGetZDimension());
	((VPVolume *)volume2)->vpAdjustScale(((VPImage *)volume2)->vpGetXDimension(), ((VPImage *)volume2)->vpGetYDimension(), ((VPVolume *)volume2)->vpGetZDimension());
	vpSetCameraDefault(camera, volume1);
	vpSetLightDefault(light, camera);

	// Initialization of inner structures visualization parameters 
	pAux = ((VPVolume *)volume1)->vpGetCenterFocalPoint(); // the center of the "object"
	pAux.z = 0;
	while ( ((VPVolume *)volume1)->vpGetValue(pAux.x,pAux.y,pAux.z) <= 40 )
			pAux.z++;
	endOfSBand = pAux.z + ( ((VPVolume *)volume1)->vpGetZDimension() * 0.06 ); // 6% 	
	endOfTBand = endOfSBand + ( ((VPVolume *)volume1)->vpGetZDimension() * 0.06 );
 }

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume1);
 volume2Dimension.x = ((VPImage *)volume2)->vpGetXDimension(); 
 volume2Dimension.y = ((VPImage *)volume2)->vpGetYDimension(); 
 volume2Dimension.z = ((VPVolume *)volume2)->vpGetZDimension(); 
 volume2Dimension -= 1; // because of the volume matrix index

 yCorrection = ((float) ((VPImage *)volume1)->vpGetYDimension()) / ((float) virtualYDimension);


 //////////////////////////////////////////////////////////////////////////
 /// To know how many samples we need in T-Band	and S-Band				///

  // To get the volume center focal point
 auxSBand = auxTBand = centerFocalPoint = ((VPVolume *)volume1)->vpGetCenterFocalPoint();

 // First, get the direction from endOfTBand to endOfSBand
 auxSBand.z = endOfSBand;
 auxTBand.z = endOfTBand;
 auxProjectionDirection.x = auxSBand.x - auxTBand.x;
 auxProjectionDirection.y = auxSBand.y - auxTBand.y;
 auxProjectionDirection.z = auxSBand.z - auxTBand.z;
 distance1 = auxProjectionDirection.vpModule();
 auxProjectionDirection.vpNormalize();

 // After, process the "delta" used to "walk" along the "ray"
 auxDelta = auxProjectionDirection * sampleStep;

 // The first point before "walking" throught the volume
 p1 = auxTBand;

 // Loop to go from the endOfTBand (internal) to the endOfSBand (external)
 while (distance2 < distance1) {
	 sampleNumberTBand++;
	 p1 = p1 + auxDelta;
	 vAux.x = p1.x - auxTBand.x;
	 vAux.y = p1.y - auxTBand.y;
	 vAux.z = p1.z - auxTBand.z;
	 distance2 = vAux.vpModule();
 }
 // To find the limit of the volume 
 while ((p1<=volumeDimension) && (p1>=temp) && (p1.y<((VPImage *)volume1)->vpGetYDimension())) {
	 p1 = p1 + auxDelta;
 }

 // p1 "adjustment"
 p1 = p1 - auxDelta;

 // Loop to find the object "edge" 
 while (sampleColor1<=40) { 
	i = (int) (p1.x); //(column);
	j = (int) (p1.y); //(line);
	k = (int) (p1.z); //(depth);
	sampleColor1 = ((VPVolume *)volume1)->vpGetValue(i,j,k);
	p1 = p1 - auxDelta; // inverse projectionDirection; 
 }

 // Loop to go from "the edge" to the enfOfSband
 do {
	 sampleNumberSBand++;
	 p1 = p1 - auxDelta;
	 vAux.x = p1.x - auxSBand.x;
	 vAux.y = p1.y - auxSBand.y;
	 vAux.z = p1.z - auxSBand.z;
	 distance2 = vAux.vpModule();
 } while (distance2 > sampleStep);

 /// End of the instructions to know how many samples we need			///
 //////////////////////////////////////////////////////////////////////////

 switch(typeOfOpacityFunction)
 {
	case LEVOYSURFACEOPACITY: SetClassificationTable();
						break;
	case LINEAROPACITY: SetClassificationTable2();
						break;
 }

 // To set the light with the same direction of the observer...
 correctProjectionDirection = projectionDirection;
 correctProjectionDirection.x = -correctProjectionDirection.x; 
 correctProjectionDirection.y = -correctProjectionDirection.y; 
 correctProjectionDirection.z = -correctProjectionDirection.z; 
 vpSetLightDirection(correctProjectionDirection);

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			sampleColor1 = sampleColor2 = sampleOpacity1 = sampleOpacity2 = 0;
			rayColorRed = rayColorGreen = rayColorBlue = rayOpacity = 0.0;
			a=0;

			pIn.y = pIn.y*yCorrection;

			// while don't "hit" the organ (beginning of S-Band)
			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume1)->vpGetYDimension()) && (sampleColor1<=40)) { 
				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);
				sampleColor1 = ((VPVolume *)volume1)->vpGetValue(i,j,k);
				pIn = pIn + deltaZ; // Next sample 
			}

			// If still inside the volume (because the ray can not "hit" the object)
			if ( (pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume1)->vpGetYDimension()) )
			{
				// End of S-Band and beginning of T-Band
				for (depth=0; depth<=sampleNumberSBand; depth++) { 
					pIn = pIn + deltaZ; 
				} 
            
				// While inside the region of interest (until the end of T-Band)
				depth=0;
				while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume1)->vpGetYDimension()) && (depth<=sampleNumberTBand)) {
				    depth++;
					i = (int) (pIn.x); //(column);
					j = (int) (pIn.y); //(line);
					k = (int) (pIn.z); //(depth);
					
					GradientSobel (i, j, k, gradient, &gm, volume1); 
					sampleColor1 = vpTrilinearInterpolation(i,j,k,volume1,pIn);


					if ( ( volumeDimension.x != volume2Dimension.x ) ||
						 ( ((VPImage *)volume1)->vpGetYDimension() != volume2Dimension.y ) ||
						 ( volumeDimension.z != volume2Dimension.z ) ) {
						switch(integrationType)
						{
							case 1:
									i2 = (volume2Dimension.x*i) / volumeDimension.x;
									j2 = (volume2Dimension.y*j) / ( ((VPImage *)volume1)->vpGetYDimension() - 1 );
									k2 = (volume2Dimension.z*k) / volumeDimension.z;
									pIn2.x = (volume2Dimension.x*pIn.x) / volumeDimension.x;
									pIn2.y = (volume2Dimension.y*pIn.y) / ( ((VPImage *)volume1)->vpGetYDimension() - 1 );
									pIn2.z = (volume2Dimension.z*pIn.z) / volumeDimension.z;
									sampleColor2 = vpTrilinearInterpolation(i2,j2,k2,volume2,pIn2);
									break;
							case 2:
									if ( (j>=firstSlice) && (j<=lastSlice) )
									{
										i2 = (volume2Dimension.x*i) / volumeDimension.x;
										k2 = (volume2Dimension.z*k) / volumeDimension.z;
										pIn2.x = (volume2Dimension.x*pIn.x) / volumeDimension.x;
										pIn2.y = ( volume2Dimension.y * (pIn.y-(float)(firstSlice))) / ((float)(lastSlice-firstSlice+1));
										j2 = (int)(pIn2.y); // por problemas de precisão foi a melhor solução para o cálculo de j2
										pIn2.z = (volume2Dimension.z*pIn.z) / volumeDimension.z;
										sampleColor2 = vpTrilinearInterpolation(i2,j2,k2,volume2,pIn2);
									}
									break;
						}

					}
					else
						sampleColor2 = vpTrilinearInterpolation(i,j,k,volume2,pIn);

					color2 = controlTables2.vpGetColor(sampleColor2); //

					ClassifyColor (&sampleColor1, &gm, &r, &g, &b, &alpha);
					alpha = 1 - pow(1-alpha, (1 / (1/sampleStep)) );
					if (specular) {
						obsPoint.x = i - observer.x;
						obsPoint.y = j - observer.y;
						obsPoint.z = k - observer.z;
						obsPoint.vpNormalize();
						attenuate = ShadeSpecular(gradient,obsPoint);
					}
					else 
						attenuate = Shade (gradient); 

					if(integrationType==1)
					{
						r = color2.vpGetR() * attenuate; //r * attenuate;
						g = color2.vpGetG() * attenuate; //g * attenuate;
						b = color2.vpGetB() * attenuate; //b * attenuate;
					}
					else
					{
						if ( (j>=firstSlice) && (j<=lastSlice) )
						{
							r = color2.vpGetR() * attenuate; //r * attenuate;
							g = color2.vpGetG() * attenuate; //g * attenuate;
							b = color2.vpGetB() * attenuate; //b * attenuate;
						}
						else
						{
							Classify (&sampleColor1, &gm, &luminance, &alpha);
							r = luminance * attenuate; //r * attenuate;
							g = luminance * attenuate; //g * attenuate;
							b = luminance * attenuate; //b * attenuate;
						}
					}

					//*----- Front-to-back alpha blending compositing
					if (alpha > 0.0)
					{
							at = alpha * (1.0f - a);
							rayColorRed   = rayColorRed   + (r * at);
							rayColorGreen = rayColorGreen + (g * at);
							rayColorBlue  = rayColorBlue  + (b * at);
							a = a + at;

							//*----- do early ray termination 
							if (a > 0.97)
								break;
					}

					// Next sample
					pIn = pIn + deltaZ; // projectionDirection; 

				} // while (depth)

			} // if

			ivalue1 = (int) rayColorRed;
			ivalue2 = (int) rayColorGreen;
			ivalue3 = (int) rayColorBlue;
			if (a < 0) {
					image[line][column][red] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][green] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][blue] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
			}
			else {
					image[line][column][red] = (unsigned short int) (255<<24 | ivalue1<<16 | ivalue1<<8 | ivalue1);
					image[line][column][green] = (unsigned short int) (255<<24 | ivalue2<<16 | ivalue2<<8 | ivalue2);
					image[line][column][blue] = (unsigned short int) (255<<24 | ivalue3<<16 | ivalue3<<8 | ivalue3);
			}
	
		} // if find intersection points

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderLivroInnerStructures" implement 
//				the inner structures visualization algorithm   
//				(based in the brute force ray casting algorithm).
// Parameters.: VPScene *s (scene object), 
//				VPCamera *c (active camera), 
//				int opacityComputation (opacity computation function),
//				unsigned int **image (pointer to the image)
// Return.....: -

void VPRayCasting::vpRenderLivroInnerStructures(VPScene *s, VPCamera *c,  int opacityComputation, unsigned int **image) {
 int line=0, column=0, depth=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 sampleNumberTBand=0, sampleNumberSBand=0;
 short int planes[6];
 float previousIlight=0, Ilight=0, sampleColor=0, rayColor=0, rayOpacity=0, tmp,
	   sampleOpacity=0, yCorrection=0, sIn=0, sOut=0, distance1=0, distance2=0;

 VPVector3D projectionDirection, gradient, deltaX, deltaY, deltaZ, 
			vAux, correctProjectionDirection, auxProjectionDirection, auxDelta;
 VPPoint3D p1, pIn, pOut, pAux, minPlaneProjection, volumeDimension, 
	       origin, temp, auxSBand, auxTBand; 
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();

 // To get a pointer to the volume object
 VPGraphicObj *volume = *objects.begin();

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // For specular processing
 VPPoint3D observer = camera->vpGetLocation();	 
 VPVector3D obsPoint;							

 // Get color and opacity tables
 VPTable controlTables = ((VPVolume *)volume)->vpGetControlTables();

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume);

 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);


 //////////////////////////////////////////////////////////////////////////
 /// To know how many samples we need in T-Band	and S-Band				///

  // To get the volume center focal point
 auxSBand = auxTBand = ((VPVolume *)volume)->vpGetCenterFocalPoint();

 // First, get the direction from endOfTBand to endOfSBand
 auxSBand.z = endOfSBand;
 auxTBand.z = endOfTBand;
 auxProjectionDirection.x = auxSBand.x - auxTBand.x;
 auxProjectionDirection.y = auxSBand.y - auxTBand.y;
 auxProjectionDirection.z = auxSBand.z - auxTBand.z;
 distance1 = auxProjectionDirection.vpModule();
 auxProjectionDirection.vpNormalize();

 // After, process the "delta" used to "walk" along the "ray"
 auxDelta = auxProjectionDirection * sampleStep;

 // The first point before "walking" throught the volume
 p1 = auxTBand;

 // Loop to go from the endOfTBand (internal) to the endOfSBand (external)
 while (distance2 < distance1) {
	 sampleNumberTBand++;
	 p1 = p1 + auxDelta;
	 vAux.x = p1.x - auxTBand.x;
	 vAux.y = p1.y - auxTBand.y;
	 vAux.z = p1.z - auxTBand.z;
	 distance2 = vAux.vpModule();
 }
 // To find the limit of the volume 
 while ((p1<=volumeDimension) && (p1>=temp) && (p1.y<((VPImage *)volume)->vpGetYDimension())) {
	 p1 = p1 + auxDelta;
 }

 // p1 "adjustment"
 p1 = p1 - auxDelta;

 // Loop to find the object "edge" 
 while (sampleColor<=40) { 
	i = (int) (p1.x); //(column);
	j = (int) (p1.y); //(line);
	k = (int) (p1.z); //(depth);
	sampleColor = ((VPVolume *)volume)->vpGetValue(i,j,k);
	p1 = p1 - auxDelta; // inverse projectionDirection; 
 }

 // Loop to go from "the edge" to the enfOfSband
 do {
	 sampleNumberSBand++;
	 p1 = p1 - auxDelta;
	 vAux.x = p1.x - auxSBand.x;
	 vAux.y = p1.y - auxSBand.y;
	 vAux.z = p1.z - auxSBand.z;
	 distance2 = vAux.vpModule();
 } while (distance2 > sampleStep);

 /// End of the instructions to know how many samples we need			///
 //////////////////////////////////////////////////////////////////////////

 switch(typeOfOpacityFunction)
 {
	case LEVOYSURFACEOPACITY: SetClassificationTable();
						break;
	case LINEAROPACITY: SetClassificationTable2();
						break;
 }

 // To set the light with the same direction of the observer...
 correctProjectionDirection = projectionDirection;
 correctProjectionDirection.x = -correctProjectionDirection.x; 
 correctProjectionDirection.y = -correctProjectionDirection.y; 
 correctProjectionDirection.z = -correctProjectionDirection.z; 
 vpSetLightDirection(correctProjectionDirection);

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			gradient.x = gradient.y = gradient.z = 0;
			lum   = (float) 0.0;
			a     = (float) 0.0;
			sampleColor = rayColor = rayOpacity = 0.0;

			pIn.y = pIn.y*yCorrection;

			// while don't "hit" the organ (beginning of S-Band)
			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) && (sampleColor<=40)) { 
				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);
				sampleColor = ((VPVolume *)volume)->vpGetValue(i,j,k);
				pIn = pIn + deltaZ; // Next sample 
			}

			// If still inside the volume (because the ray can not "hit" the object)
			if ( (pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) )
			{
				// End of S-Band and beginning of T-Band
				for (depth=0; depth<sampleNumberSBand; depth++) { 
					pIn = pIn + deltaZ; 
				} 
            
				// While inside the region of interest (until the end of T-Band
				depth=0;
				while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) && (depth<sampleNumberTBand)) {
				    depth++;
					i = (int) (pIn.x); //(column);
					j = (int) (pIn.y); //(line);
					k = (int) (pIn.z); //(depth);
		
					// Sample processing in accordance with the shading method (LOCALSHADING)
					GradientSobel (i, j, k, gradient, &gm, volume); 
					tmp = vpTrilinearInterpolation(i,j,k,volume,pIn);
					Classify (&tmp, &gm, &luminance, &alpha);
					alpha = 1 - pow(1-alpha, (1 / (1/sampleStep)) );
					if (specular) {
						obsPoint.x = i - observer.x;
						obsPoint.y = j - observer.y;
						obsPoint.z = k - observer.z;
						obsPoint.vpNormalize();
						attenuate = ShadeSpecular(gradient,obsPoint);
					}
					else 
						attenuate = Shade (gradient); 
					luminance = luminance * attenuate;

					//*----- Front-to-back alpha blending compositing
					if (alpha > 0.0) 
					{
						at = alpha * (1.0f - a);
						lum = lum + (luminance * at);
						a = a + at;

						if (a > 0.97) break;
					}

					// Next sample
					pIn = pIn + deltaZ; // projectionDirection; 
				} // while (depth)
			} // if

			ivalue = (int) lum;
			if (a < 0)
					image[line][column] = (unsigned int) (255<<24 | 102<<16 | 179<<8 | 230);
			else
					image[line][column] = (unsigned int) (255<<24 | ivalue<<16 | ivalue<<8 | ivalue);
		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderLivroMonoColorInnerStructures"  
//				implement the inner structures visualization 
//				algorithm using colors (based in the brute force 
//				ray casting algorithm).
// Parameters.: VPScene *s (scene object), 
//				VPCamera *c (active camera), 
//				int opacityComputation (opacity computation function),
//				unsigned int ***image (pointer to the image)
// Return.....: -

void  VPRayCasting::vpRenderLivroMonoColorInnerStructures(VPScene *s, VPCamera *c, int opacityComputation, unsigned int ***image) {
 int line=0, column=0, depth=0, i=0, j=0, k=0, initialLineValue=0, ivalue1, ivalue2, ivalue3, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 sampleNumberTBand=0, sampleNumberSBand=0;
 short int planes[6];
 float previousIlight=0, Ilight=0, sampleColor=0, rayColor=0, rayOpacity=0, r, g, b,
	   sampleOpacity=0, yCorrection=0, sIn=0, sOut=0, distance1=0, distance2=0,
	   rayColorRed=0, rayColorGreen=0, rayColorBlue=0, tmp;

 VPVector3D projectionDirection, gradient, deltaX, deltaY, deltaZ, 
			vAux, correctProjectionDirection, auxProjectionDirection, auxDelta;
 VPPoint3D p1, pIn, pOut, pAux, minPlaneProjection, volumeDimension, 
	       origin, temp, auxSBand, auxTBand; 
 VPColor color;
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();

 // To get a pointer to the volume object
 VPGraphicObj *volume = *objects.begin();

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // For specular processing
 VPPoint3D observer = camera->vpGetLocation();	 
 VPVector3D obsPoint;							

 // Get color and opacity tables
 VPTable controlTables = ((VPVolume *)volume)->vpGetControlTables();

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume);

 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 //////////////////////////////////////////////////////////////////////////
 /// To know how many samples we need in T-Band	and S-Band				///

  // To get the volume center focal point
 auxSBand = auxTBand = ((VPVolume *)volume)->vpGetCenterFocalPoint();

 // First, get the direction from endOfTBand to endOfSBand
 auxSBand.z = endOfSBand;
 auxTBand.z = endOfTBand;
 auxProjectionDirection.x = auxSBand.x - auxTBand.x;
 auxProjectionDirection.y = auxSBand.y - auxTBand.y;
 auxProjectionDirection.z = auxSBand.z - auxTBand.z;
 distance1 = auxProjectionDirection.vpModule();
 auxProjectionDirection.vpNormalize();

 // After, process the "delta" used to "walk" along the "ray"
 auxDelta = auxProjectionDirection * sampleStep;

 // The first point before "walking" throught the volume
 p1 = auxTBand;

 // Loop to go from the endOfTBand (internal) to the endOfSBand (external)
 while (distance2 < distance1) {
	 sampleNumberTBand++;
	 p1 = p1 + auxDelta;
	 vAux.x = p1.x - auxTBand.x;
	 vAux.y = p1.y - auxTBand.y;
	 vAux.z = p1.z - auxTBand.z;
	 distance2 = vAux.vpModule();
 }
 // To find the limit of the volume 
 while ((p1<=volumeDimension) && (p1>=temp) && (p1.y<((VPImage *)volume)->vpGetYDimension())) {
	 p1 = p1 + auxDelta;
 }

 // p1 "adjustment"
 p1 = p1 - auxDelta;

 // Loop to find the object "edge" 
 while (sampleColor<=40) { 
	i = (int) (p1.x); //(column);
	j = (int) (p1.y); //(line);
	k = (int) (p1.z); //(depth);
	sampleColor = ((VPVolume *)volume)->vpGetValue(i,j,k);
	p1 = p1 - auxDelta; // inverse projectionDirection; 
 }

 // Loop to go from "the edge" to the enfOfSband
 do {
	 sampleNumberSBand++;
	 p1 = p1 - auxDelta;
	 vAux.x = p1.x - auxSBand.x;
	 vAux.y = p1.y - auxSBand.y;
	 vAux.z = p1.z - auxSBand.z;
	 distance2 = vAux.vpModule();
 } while (distance2 > sampleStep);

 /// End of the instructions to know how many samples we need			///
 //////////////////////////////////////////////////////////////////////////

 switch(typeOfOpacityFunction)
 {
	case LEVOYSURFACEOPACITY: SetClassificationTable();
						break;
	case LINEAROPACITY: SetClassificationTable2();
						break;
 }

 // To set the light with the same direction of the observer...
 correctProjectionDirection = projectionDirection;
 correctProjectionDirection.x = -correctProjectionDirection.x; 
 correctProjectionDirection.y = -correctProjectionDirection.y; 
 correctProjectionDirection.z = -correctProjectionDirection.z; 
 vpSetLightDirection(correctProjectionDirection);

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			gradient.x = gradient.y = gradient.z = 0;
			lum   = (float) 0.0;
			a     = (float) 0.0;
			sampleColor = rayColor = rayOpacity = r = g = b = 0.0;
			rayColorRed = rayColorGreen = rayColorBlue = 0.0;

			pIn.y = pIn.y*yCorrection;
			
			// while don't "hit" the organ (beginning of S-Band)
			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) && (sampleColor<=40)) { 
				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);
				sampleColor = ((VPVolume *)volume)->vpGetValue(i,j,k);
				pIn = pIn + deltaZ; // Next sample 
			}

			// If still inside the volume (because the ray can not "hit" the object)
			if ( (pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) )
			{
				// End of S-Band and beginning of T-Band
				for (depth=0; depth<sampleNumberSBand; depth++) { 
					pIn = pIn + deltaZ; 
				} 
            
				// While inside the region of interest (until the end of T-Band
				depth=0;
				while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension()) && (depth<sampleNumberTBand)) {
				    depth++;
					i = (int) (pIn.x); //(column);
					j = (int) (pIn.y); //(line);
					k = (int) (pIn.z); //(depth);
		
					// Sample processing in accordance with the shading method (LOCALSHADING)
					GradientSobel (i, j, k, gradient, &gm, volume); 
					tmp = vpTrilinearInterpolation(i,j,k,volume,pIn);
					color = controlTables.vpGetColor(tmp); //

					ClassifyColor (&tmp, &gm, &r, &g, &b, &alpha);
					alpha = 1 - pow(1-alpha, (1 / (1/sampleStep)) );
					if (specular) {
						obsPoint.x = i - observer.x;
						obsPoint.y = j - observer.y;
						obsPoint.z = k - observer.z;
						obsPoint.vpNormalize();
						attenuate = ShadeSpecular(gradient,obsPoint);
					}
					else 
						attenuate = Shade (gradient); 
					r = color.vpGetR() * attenuate; //r * attenuate;
					g = color.vpGetG() * attenuate; //g * attenuate;
					b = color.vpGetB() * attenuate; //b * attenuate;

					//*----- Front-to-back alpha blending compositing
					if (alpha > 0.0) 
					{
						at = alpha * (1.0f - a);
						rayColorRed   = rayColorRed   + (r * at);
						rayColorGreen = rayColorGreen + (g * at);
						rayColorBlue  = rayColorBlue  + (b * at);
						a = a + at;

						if (a > 0.97) break;
					}

					// Next sample
					pIn = pIn + deltaZ; // projectionDirection; 
				} // while (depth)
			} // if

			ivalue1 = (int) rayColorRed;
			ivalue2 = (int) rayColorGreen;
			ivalue3 = (int) rayColorBlue;
			if (a < 0) {
					image[line][column][red] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][green] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
					image[line][column][blue] = (unsigned short int) (255<<24 | 102<<16 | 179<<8 | 230);
			}
			else {
					image[line][column][red] = (unsigned short int) (255<<24 | ivalue1<<16 | ivalue1<<8 | ivalue1);
					image[line][column][green] = (unsigned short int) (255<<24 | ivalue2<<16 | ivalue2<<8 | ivalue2);
					image[line][column][blue] = (unsigned short int) (255<<24 | ivalue3<<16 | ivalue3<<8 | ivalue3);
			}
		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderMIP" implement the MIP visualization  
//				type using the brute force ray casting algorithm for  
//				gray levels.
// Parameters.: VPScene *s, VPCamera *c (active camera), 
//				unsigned int image[][256] (pointer to the image);
// Return.....: -

void VPRayCasting::vpRenderMIP(VPScene *s, VPCamera *c, unsigned int **image) {
 int line=0, column=0, depth=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0;
 short int planes[6];
 float previousIlight=0, Ilight=0, sampleColor=0, rayColor=0, rayOpacity=0, 
	   sampleOpacity=0, yCorrection=0, sIn=0, sOut=0,
	   MIPcolor=0, MIPlight=0, MIPopacity=0;

 VPVector3D projectionDirection, deltaX, deltaY, deltaZ, vAux;
 VPPoint3D p1, pIn, pOut, pAux, minPlaneProjection, volumeDimension, origin, temp; 
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();

 // To get a pointer to the volume object
 VPGraphicObj *volume = *objects.begin();

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // Get color and opacity tables
 VPTable controlTables = ((VPVolume *)volume)->vpGetControlTables();

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume);

 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			sampleColor = 0;
			rayColor = rayOpacity = 0.0;
			previousIlight = Ilight = 0.0;
			MIPcolor = -1.0;

			pIn.y = pIn.y*yCorrection;

			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension())) { 

				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);


				// Interpolation 
				sampleColor = vpTrilinearInterpolation(i,j,k,volume,pIn);

				if (sampleColor > 255) 
					sampleColor = 255;

				if (MIPcolor < sampleColor)
					MIPcolor = sampleColor;

				// Next sample
				pIn = pIn + deltaZ; // projectionDirection; 
			} // while (depth)
            
			sampleOpacity = controlTables.vpGetLinearOpacityValue( MIPcolor );
			
			// store pixel color
			image[line][column] = (unsigned int) MIPcolor*sampleOpacity;

		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpRenderLivroMIP" implement the MIP   
//				visualization type using the brute force ray  
//				casting algorithm for gray levels.
// Parameters.: VPScene *s, VPCamera *c (active camera), 
//				unsigned int image[][256] (pointer to the image);
// Return.....: -

void VPRayCasting::vpRenderLivroMIP(VPScene *s, VPCamera *c, unsigned int **image) {
 int line=0, column=0, depth=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0;
 short int planes[6];
 float sampleOpacity=0, yCorrection=0, sIn=0, sOut=0, MIPcolor=0, MIPlight=0, 
	 MIPopacity=0, MIPgm, tmp;

 VPVector3D projectionDirection, deltaX, deltaY, deltaZ, vAux, gradient, 
	        MIPgradient, correctProjectionDirection;
 VPPoint3D p1, pIn, pOut, pAux, minPlaneProjection, volumeDimension, origin, temp; 
 VPScene *scene = s;
 list<VPLight*> lights = scene->vpGetLights();
 list<VPGraphicObj*> objects = scene->vpGetObjects();

 // To get a pointer to the volume object
 VPGraphicObj *volume = *objects.begin();

 // Pointer to the right camera 
 VPCamera *camera = c; 

 // Get a pointer to the first light of the list 
 light = *lights.begin();

 // Get color and opacity tables
 VPTable controlTables = ((VPVolume *)volume)->vpGetControlTables();

 vpSetVariables(camera, projectionDirection, minPlaneProjection, 
				volumeDimension, planes, finalLineValue, finalColumnValue,
				deltaX, deltaY, deltaZ, volume);

 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 switch(typeOfOpacityFunction)
 {
	case LEVOYSURFACEOPACITY: SetClassificationTable();
						break;
	case LINEAROPACITY: SetClassificationTable2();
						break;
 }

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		// Volume and radius intersection point computation
		if ( vpFindIntersectionPoints(planes, volumeDimension, p1, 
									  projectionDirection, pIn, pOut, sIn, sOut) )
		{ 
			// if the ray intersect the volume then we have to process the samples
			gradient.x = gradient.y = gradient.z = (float) 0.0;
			MIPgradient = gradient;
			MIPgm = (float) 0.0;
			lum   = (float) 0.0;
			a     = (float) 0.0;
			MIPcolor = (float) -1.0;

			pIn.y = pIn.y*yCorrection;
		
			while ((pIn<=volumeDimension) && (pIn>=temp) && (pIn.y<((VPImage *)volume)->vpGetYDimension())) { 

				i = (int) (pIn.x); //(column);
				j = (int) (pIn.y); //(line);
				k = (int) (pIn.z); //(depth);

				Gradient (i, j, k, gradient, &gm, volume); 
				tmp = vpTrilinearInterpolation(i,j,k,volume,pIn);

				if (MIPcolor < tmp) {
					MIPcolor = tmp;
					MIPgm = gm;
					MIPgradient = gradient;
				}

				// Next sample
				pIn = pIn + deltaZ; // projectionDirection; 
			} // while (depth)

			// To set the light with the same direction of the observer...
			correctProjectionDirection = projectionDirection;
			correctProjectionDirection.x = -correctProjectionDirection.x; 
			correctProjectionDirection.y = -correctProjectionDirection.y; 
			correctProjectionDirection.z = -correctProjectionDirection.z; 
			vpSetLightDirection(correctProjectionDirection);
            
			attenuate = Shade (MIPgradient);
			luminance = MIPcolor * attenuate;

			// store pixel color
			ivalue = (int) luminance;
			if (a < 0)
					image[line][column] = (unsigned int) (255<<24 | 102<<16 | 179<<8 | 230);
			else
					image[line][column] = (unsigned int) (255<<24 | ivalue<<16 | ivalue<<8 | ivalue);

		} // if

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetVariables" set the values of some
//				ray casting variables.
// Parameters.: VPCamera *camera, a pointer to the camera object 
//				VPVector3D &projectionDirection
//				VPPoint3D &minPlaneProjection, point where the scan begin
//				VPPoint3D &volumeDimension
//				short int planes[], intersection candidate planes
//				int &finalLineValue, screen width
//				int &finalColumnValue, screen hight
//				VPVector3D &deltaX, x step
//				VPVector3D &deltaY, y step
//				VPVector3D &deltaZ, z step
//				VPGraphicObj *volume, volume that will be visualized
// Return.....: -

void VPRayCasting::vpSetVariables(VPCamera *camera, VPVector3D &projectionDirection,
								  VPPoint3D &minPlaneProjection, VPPoint3D &volumeDimension, 
								  short int planes[], int &finalLineValue, int &finalColumnValue,
								  VPVector3D &deltaX, VPVector3D &deltaY, VPVector3D &deltaZ,
								  VPGraphicObj *volume) 
{
 float ratioX=0, ratioY=0;
 VPPoint2D winTopRightValue;
 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
 VPVector3D vectorH, vectorV, auxUp(0,1,0);
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 if ((projectionDirection==camera->vpGetUp()) || (projectionDirection==auxUp)) {
	VPVector3D v(0,0,-1);
 	camera->vpSetUp(v);
 }

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 // volumeDimension has the volume dimension
 volumeDimension.x = ((VPImage *)volume)->vpGetXDimension();
 volumeDimension.y = virtualYDimension;
 volumeDimension.z = ((VPVolume *)volume)->vpGetZDimension();
 volumeDimension -= 1; // because of the volume matrix index
 
 // "D" component of orthogonal planes equation...
 dFront = 0;					// Normal(0,0,-1)  Point(0,0,0)
 dTop = 0;						// Normal(0,-1,0)   Point(0,0,0)
 dLeft = 0;						// Normal(-1,0,0)  Point(0,0,0)
 dBack = -volumeDimension.z;	// Normal(0,0,1)   Point = volumeDimension
 dBottom = -volumeDimension.y;	// Normal(0,1,0)  Point = volumeDimension
 dRight = -volumeDimension.x;	// Normal(1,0,0)   Point = volumeDimension

 // Set "h" and "v" vectors 
 VPVector3D BAKA = camera->vpGetUp();
 vectorH = BAKA.vpCrossProduct(projectionDirection);
 vectorV = vectorH.vpCrossProduct(projectionDirection);

 //vectorH = projectionDirection.vpCrossProduct(camera->vpGetUp());
 //vectorV = projectionDirection.vpCrossProduct(vectorH);


 // Possible intersection planes computation
 vpFindIntersectionCandidatePlanes(planes, projectionDirection);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

// Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection * sampleStep;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetCameraDefault" set the initial camera
//				attributes values, in accordance with the volume.
// Parameters.: *camera, a pointer to the camera object 
//				*volume, a pointer to the volume object that will 
//						 be visualized.
// Return.....: -

void VPRayCasting::vpSetCameraDefault(VPCamera *camera, VPGraphicObj *volume) {

 float h=0, w=0;
 VPPoint3D center = ((VPVolume *)volume)->vpGetCenterFocalPoint();

 // Middle of the object (gives camera direction)
 VPPoint3D target((float) center.x, 
				  (float) virtualYDimension/2,
				  (float) center.z);

 // Camera location
 VPPoint3D location ((float) center.x, 
					 (float) virtualYDimension/2,
					 (float) 0 );

 VPVector3D up(0.0,1.0,0.0);

 if (cameraLocation.x==0 && cameraLocation.y==0 && cameraLocation.z==0)
	camera->vpSetLocation(location);
 else
	camera->vpSetLocation(cameraLocation);
 camera->vpSetTarget(target);
 camera->vpSetProjectionType(ORTHOGRAPHIC);
 camera->vpSetUp(up);

 // Set the parallelpiped or frustum view, in agreement with the
 // selected projection (winTopRight is initialized before)
 VPPoint2D winBottomLeftValue(0,0);
 
 camera->vpSetNearPlane(0);
 if ( ((VPImage *)volume)->vpGetYDimension() > ((VPVolume *)volume)->vpGetZDimension() )
	camera->vpSetFarPlane(((VPImage *)volume)->vpGetYDimension()); 
 else
	camera->vpSetFarPlane(((VPVolume *)volume)->vpGetZDimension());

 camera->vpSetWinBottomLeft(winBottomLeftValue);

 ((VPVolume *)volume)->vpSetCameraLocationForInnerStructure(location);

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetLightDefault" set the initial light
//				attributes values, in accordance with the observer
//				position.
// Parameters.: *light, a pointer to the light object  
//				*camera, a pointer to the camera object
// Return.....: -

void VPRayCasting::vpSetLightDefault(VPLight *light, VPCamera *camera) {
 VPVector3D direction;
 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();

 //location.x += 15;
 //location.y += 15;
 direction.vpSetVector3D(location-target); 
 direction.vpNormalize();

 ((VPDirectionalLight *)light)->vpSetDirection(direction);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFindIntersectionCandidatePlanes" process   
//				the possible planes intersection.
// Parameters.: short int planes[](information about the possibles 
//								   planes intersection)
//				VPVector3D dir (observer-target direction ray) 
// Return.....: -

void VPRayCasting::vpFindIntersectionCandidatePlanes(short int planes[], 
													 VPVector3D dir) {
 // Identification of possible intersection planes
 if ( dir.x != 0 ) { // If it could intersect YZ plane...
	if ( dir.x > 0) {
		planes[PLEFT] = PIN;
		planes[PRIGHT] = POUT;
	}
	else {
		planes[PLEFT] = POUT;
		planes[PRIGHT] = PIN;
	}
 }
 else {
	planes[PLEFT] = planes[PRIGHT] = NOTIO;
 }

 if ( dir.y != 0 ) { // If it could intersect XZ plane...
	if ( dir.y > 0) { 
		planes[PTOP] = PIN;
		planes[PBOTTOM] = POUT;
	}
	else {
		planes[PTOP] = POUT;
		planes[PBOTTOM] = PIN;
	}
 }
 else {
	planes[PTOP] = planes[PBOTTOM] = NOTIO;
 }

 if ( dir.z != 0 ) { // If it could intersect XY plane...
	if ( dir.z > 0) { 
		planes[PFRONT] = PIN;
		planes[PBACK] = POUT;
	}
	else {
		planes[PFRONT] = POUT;
		planes[PBACK] = PIN;
	}
 }
 else {
	planes[PFRONT] = planes[PBACK] = NOTIO;
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFindIntersectionPoints" process the  
//				intersection between a line and a volume.
// Parameters.: VPPoint3D maxB (volume dimension) 
//				VPPoint3D origin (ray origin) 
//				VPVector3D dir (ray direction) 
//				VPPoint3D &in (intersection point from in plane)
//				VPPoint3D &out (intersection point from out plane)
// Return.....: true (line intersect the volume)
//				false (line doesn't intersect the volume)

bool VPRayCasting::vpFindIntersectionPoints(short int planes[], VPPoint3D maxB, 
											VPPoint3D origin, VPVector3D dir, 
											VPPoint3D &in, VPPoint3D &out, 
											float &tIn, float &tOut) {
 bool noIn=true, noOut=true;
 short int planeIn=0, planeOut=0;
 float d=0, t[6]={0,0,0,0,0,0};
 VPPoint3D minB;

 // Equations to find the intersection between a line and a plane
 // f = originX - endX, g = originY - endY, h = originZ - endZ
 // d=A*f+B*g+C*h  T=-(A*x2+B*y2+C*z2+D)/d
 // origin - end = dir, so (f,g,h)=dir - it's not necessary to process "f,g,h"

 // "T" processing for all planes (distance between the ray origin and the plane)
 if (planes[PFRONT] != NOTIO) { // N(0,0,-1)
	d = -dir.z;
	t[PFRONT] = origin.z / d;
 }
 if (planes[PTOP] != NOTIO) { // N(0,-1,0)
	d = -dir.y;
	t[PTOP] = origin.y / d;
 }
 if (planes[PLEFT] != NOTIO) { // N(-1,0,0)
	d = -dir.x;
	t[PLEFT] = origin.x / d;
 }
 if (planes[PBACK] != NOTIO) { // N(0,0,1)
	d = dir.z;
	t[PBACK] = -(origin.z + dBack) / d;
 }
 if (planes[PBOTTOM] != NOTIO) { // N(0,1,0)
	d = dir.y;
	t[PBOTTOM] = -(origin.y + dBottom) / d;
 }
 if (planes[PRIGHT] != NOTIO) { // N(1,0,0)
	d = dir.x;
	t[PRIGHT] = -(origin.x + dRight) / d;
 }

 // Verification of the min "T" for "in plane" and "out plane" candidates
 for (int i=0; i<6; i++) {
	 if (planes[i] == NOTIO) continue;
	 if (planes[i] == PIN) {
		 if (noIn) {
			tIn = t[i];
			planeIn = i;
			noIn = false;
		 }
		 else
			if (t[i] > tIn) {
				 tIn=t[i];
				 planeIn = i;
			}
	 }
	 else { // planes[i] == POUT
		 if (noOut) {
			tOut = t[i];
			planeOut = i;
			noOut = false;
		 }
		 else
			if (t[i] < tOut) {
				 tOut=t[i];
				 planeOut = i;
			}
	 }
 }
 
 // Intersection point
 in = origin + (dir * tIn);
 out = origin + (dir * tOut);

 // Round error adjust
 switch (planeIn) {
	case PFRONT:in.z = 0;
				break;
	case PBACK: in.z = maxB.z;
				break;
	case PTOP:  in.y = 0;
				break;
	case PBOTTOM:in.y = maxB.y;
				break;
	case PLEFT: in.x = 0;
				break;
	case PRIGHT: in.x = maxB.x;
				break;
 }
 switch (planeOut) {
	case PFRONT:out.z = 0;
				break;
	case PBACK: out.z = maxB.z;
				//if (tOut > maxB.z) 
				//	tOut = maxB.z;
				break;
	case PTOP:  out.y = 0;
				break;
	case PBOTTOM:out.y = maxB.y;
	case PLEFT: out.x = 0;
				break;
	case PRIGHT: out.x = maxB.x;
				break;
 }

 if ( ((in<=maxB) && (in>=minB)) && ((out<=maxB) && (out>=minB)) )
	 return true;
 else 
	 return false;

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFindSphereIntersectionPoints" process the  
//				intersection between a line and sphere.
// Parameters.: VPPoint3D origin (origin of the ray) 
//				VPPoint3D center (center of the sphere) 
//				float ray (sphere ray) 
//				VPVector3D dir (direction of the ray) 
//				VPPoint3D &point (intersection point from in plane)
// Return.....: true (ray intersect the sphere)
//				false (ray doesn't intersect the sphere)

bool VPRayCasting::vpFindSphereIntersectionPoints(VPPoint3D origin, VPPoint3D center, float ray,
												  VPVector3D dir, VPPoint3D &point) {
 VPVector3D v1;
 float v, disc;

 // v1 = vector from the origin to the center of the sphere
 v1.x = center.x - origin.x;
 v1.y = center.y - origin.y;
 v1.z = center.z - origin.z;
 
 // dot product among v1 and ray direction
 v = v1.vpDotProduct(dir);

 // "E" is the starting point of the ray and "O" is the center of the sphere
 // "EO" is the vector from E to O (thus, EO = O - E)
 // disc = r^2 - ((EO · EO) - v^2);
 disc = ray * ray - ( (v1.vpDotProduct(v1)) - (v*v) );

 if (disc < 0)
	 return false; // no intersection
 else {
	 disc = sqrt(disc);
	 point.x = origin.x + (v - disc) * dir.x;
	 point.y = origin.y + (v - disc) * dir.y;
	 point.z = origin.z + (v - disc) * dir.z;
	 return true;
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpTrilinearInterpolation" process the voxel 
//				color (gray) using trilinear interpolation.
// Parameters.: int i, int j, int k, VPGraphicObj *volume,
//				VPPoint3D p (i,j,k=volume matrix index; 
//				depth=ray position; volume=object volume that has all 
//				information about the volume; p="in plane" intersection
//				point) 
// Return.....: float color (voxel color)

float VPRayCasting::vpTrilinearInterpolation(int i, int j, int k,  
											 VPGraphicObj *volume, VPPoint3D p) {

 int C1, C2, C3, C4, C5, C6, C7, C8, ii, jj, kk;
 float i1, i2, i3, i4, i5, i6, i7,	// interpolated values
	   dx, dy, dz;					// distance (delta)

 // First, voxels color initialization
 ii = ((i+1) < ((VPImage *)volume)->vpGetXDimension()) ? i+1 : i;
 jj = ((j+1) < ((VPImage *)volume)->vpGetYDimension()) ? j+1 : j;
 kk = ((k+1) < ((VPVolume *)volume)->vpGetZDimension()) ? k+1 : k;
 C1 = ((VPVolume *)volume)->vpGetValue(i,j,k);
 C2 = ((VPVolume *)volume)->vpGetValue(ii,j,k);
 C3 = ((VPVolume *)volume)->vpGetValue(ii,jj,k);
 C4 = ((VPVolume *)volume)->vpGetValue(i,jj,k);
 C5 = ((VPVolume *)volume)->vpGetValue(i,j,kk);
 C6 = ((VPVolume *)volume)->vpGetValue(ii,j,kk);
 C7 = ((VPVolume *)volume)->vpGetValue(ii,jj,kk);
 C8 = ((VPVolume *)volume)->vpGetValue(i,jj,kk);
 
 // If all voxels have the same color, it is not necessary to interpolate  
 if ((C1==C2) && (C2==C3) && (C3==C4) && (C4==C5) && (C5==C6) && (C6==C7) && (C7==C8))
	return( (float)(C1) );

 // Trilinear interpolation (seven linear interpolations)
 //					
 //       5-----------------6		
 //      /|          	   /|		
 //     i1-------i5-------i2|		
 //    /  |      |       /  |		
 //   1-----------------2   |		
 //   |   |      |      |   |		
 //   |   |	     i7     |   |		
 //   |   |      |      |   |		
 //   |   8------|i3----|---7		
 //   |  /       |/     |  /		
 //   | /	     i6     | /		
 //   |/        /       |/		
 //   4--------i4-------3		
 //					

 // Distance from the voxel 1
 dx = p.x-(float)i;
 dy = p.y-(float)j;
 dz = p.z-(float)k;

 // Interpolation values
 i1 = ( (C5-C1) * dz + C1);
 i2 = ( (C6-C2) * dz + C2);
 i3 = ( (C7-C8) * dx + C8);
 i4 = ( (C3-C4) * dx + C4);
 i5 = ( (i2-i1) * dx + i1);
 i6 = ( (i3-i4) * dz + i4);
 i7 = ( (i6-i5) * dy + i5);

 return(i7);
}


//////////////////////////////////////////////////////////////////
// Description: Method "vpTriLinearGouraudInterpolation" process 
//				the voxel color (gray) and the light value using 
//				trilinear interpolation.
// Parameters.: int i, int j, int k = volume matrix index;
//				VPGraphicObj *volume = object volume that has all 
//									   information about the volume;
//				VPPoint3D p = "in plane" intersection point
//				float &color = voxel color processed 
//				float &light = light processed 
//				bool  &interpolate = if all voxels don't have the 
//									same value,interpolation occurs 
//									and i=TRUE, else i=FALSE)
// Return.....: -

void VPRayCasting::vpTriLinearGouraudInterpolation(int i, int j, int k,  
									   VPGraphicObj *volume, VPPoint3D p, 
									   float &color, float &light, bool &interpolate) {
 int C1, C2, C3, C4, C5, C6, C7, C8, ii, jj, kk;
 float l1, l2, l3, l4, l5, l6, l7, l8,
	   i1, i2, i3, i4, i5, i6, 	// interpolated values 
	   dx, dy, dz;				// distance (delta)

 // First, voxels color initialization
 ii = ((i+1) < ((VPImage *)volume)->vpGetXDimension()) ? i+1 : i;
 jj = ((j+1) < ((VPImage *)volume)->vpGetYDimension()) ? j+1 : j;
 kk = ((k+1) < ((VPVolume *)volume)->vpGetZDimension()) ? k+1 : k;

 C1 = ((VPVolume *)volume)->vpGetValue(i,j,k);
 C2 = ((VPVolume *)volume)->vpGetValue(ii,j,k);
 C3 = ((VPVolume *)volume)->vpGetValue(ii,jj,k);
 C4 = ((VPVolume *)volume)->vpGetValue(i,jj,k);
 C5 = ((VPVolume *)volume)->vpGetValue(i,j,kk);
 C6 = ((VPVolume *)volume)->vpGetValue(ii,j,kk);
 C7 = ((VPVolume *)volume)->vpGetValue(ii,jj,kk);
 C8 = ((VPVolume *)volume)->vpGetValue(i,jj,kk);
 

 // Trilinear interpolation for color and light
 // (seven linear interpolations)
 //					
 //       5-----------------6		
 //      /|          	   /|		
 //     i1-------i5-------i2|		
 //    /  |      |       /  |		
 //   1-----------------2   |		
 //   |   |      |      |   |		
 //   |   |	   color    |   |		
 //   |   |      |      |   |		
 //   |   8------|i3----|---7		
 //   |  /       |/     |  /		
 //   | /	     i6     | /		
 //   |/        /       |/		
 //   4--------i4-------3		
 //					

 // Distance from the voxel 1
 dx = p.x-i;
 dy = p.y-j;
 dz = p.z-k;


 // If all voxels have the same color, it is not necessary to interpolate  
 // FUTURAMENTE VERIFICAR BIT DE FLAG ???
 if ((C1==C2) && (C2==C3) && (C3==C4) && (C4==C5) && (C5==C6) && (C6==C7) && (C7==C8)) {
	color = (float)(C1);
	interpolate = false;
 }
 else {
	 // Voxel interpolation values
	i1 = ( (C5-C1) * dz + C1);
	i2 = ( (C6-C2) * dz + C2);
	i3 = ( (C7-C8) * dx + C8);
	i4 = ( (C3-C4) * dx + C4);
	i5 = ( (i2-i1) * dx + i1);
	i6 = ( (i3-i4) * dz + i4);
	color = ( (i6-i5) * dy + i5);
	interpolate = true;
 }

  // Second, light color initialization
 l1 = ((VPVolume *)volume)->vpGetLightValue(i,j,k);
 l2 = ((VPVolume *)volume)->vpGetLightValue(ii,j,k);
 l3 = ((VPVolume *)volume)->vpGetLightValue(ii,jj,k);
 l4 = ((VPVolume *)volume)->vpGetLightValue(i,jj,k);
 l5 = ((VPVolume *)volume)->vpGetLightValue(i,j,kk);
 l6 = ((VPVolume *)volume)->vpGetLightValue(ii,j,kk);
 l7 = ((VPVolume *)volume)->vpGetLightValue(ii,jj,kk);
 l8 = ((VPVolume *)volume)->vpGetLightValue(i,jj,kk);

 // Light interpolation values
 i1 = ( (l5-l1) * dz + l1);
 i2 = ( (l6-l2) * dz + l2);
 i3 = ( (l7-l8) * dx + l8);
 i4 = ( (l3-l4) * dx + l4);
 i5 = ( (i2-i1) * dx + i1);
 i6 = ( (i3-i4) * dz + i4);
 light = ( (i6-i5) * dy + i5);

 return;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpTriLinearInterpolationAndGradientComputation" 
//				process the voxel color (gray) using trilinear 
//				interpolation and the gradient approximation.
//				The gradient calculation technique used
//				is "interpolated points difference"
// Parameters.: int i, int j, int k, VPGraphicObj *volume,
//				VPPoint3D p, float previousIlight, VPLight *l 
//				(i,j,k=volume matrix index; 
//				depth=ray position; volume=object volume that has all 
//				information about the volume; p="in plane" intersection
//				point; float &color (voxel color), VPVector3D &g 
//				(gradient vector approximation) and bool interpolate (if all voxels
//				don't have the same value, interpolation occurs and i=TRUE,
//				else i=FALSE)
// Return.....: -

void VPRayCasting::vpTriLinearInterpolationAndGradientComputation(int i, int j, int k,  
											VPGraphicObj *volume, VPPoint3D p, 
											float &color, VPVector3D &g, bool &interpolate) {

 int C1, C2, C3, C4, C5, C6, C7, C8, ii, jj, kk;
 float i1, i2, i3, i4, i5, i6, i7, i8,	// interpolated values 
	   PX0, PX1, PY0, PY1, PZ0, PZ1,	// interpolated values 
	   dx, dy, dz;						// distance (delta)
 
 // First, voxels color initialization
 ii = ((i+1) < ((VPImage *)volume)->vpGetXDimension()) ? i+1 : i;
 jj = ((j+1) < ((VPImage *)volume)->vpGetYDimension()) ? j+1 : j;
 kk = ((k+1) < ((VPVolume *)volume)->vpGetZDimension()) ? k+1 : k;
 C1 = ((VPVolume *)volume)->vpGetValue(i,j,k);
 C2 = ((VPVolume *)volume)->vpGetValue(ii,j,k);
 C3 = ((VPVolume *)volume)->vpGetValue(ii,jj,k);
 C4 = ((VPVolume *)volume)->vpGetValue(i,jj,k);
 C5 = ((VPVolume *)volume)->vpGetValue(i,j,kk);
 C6 = ((VPVolume *)volume)->vpGetValue(ii,j,kk);
 C7 = ((VPVolume *)volume)->vpGetValue(ii,jj,kk);
 C8 = ((VPVolume *)volume)->vpGetValue(i,jj,kk);

 // If all voxels have the same color, it is not necessary to interpolate  
 if ((C1==C2) && (C2==C3) && (C3==C4) && (C4==C5) && (C5==C6) && (C6==C7) && (C7==C8)) {
	color = (float)(C1);
	interpolate = false;
 }
 else {
	// Trilinear interpolation (seven linear interpolations)
	//					
	//       5-----------------6		
	//      /|          	  /|		
	//     i1------PY0-------i2|		
	//    /  |      |       /  |		
	//   1-----------------2   |		
	//   |   |      |      |   |		
	//   |   |	  color    |   |		
	//   |   |      |      |   |		
	//   |   8------|i3----|---7		
	//   |  /       |/     |  /		
	//   | /	   PY1     | /		
	//   |/        /       |/		
	//   4--------i4-------3		
	//					

	// Distance from the voxel 1
	dx = p.x-i;
	dy = p.y-j;
	dz = p.z-k;

	// Interpolation values
	i1  = ( (C5-C1) * dz + C1);
	i2  = ( (C6-C2) * dz + C2);
	i3  = ( (C7-C8) * dx + C8);
	i4  = ( (C3-C4) * dx + C4);
	PY0 = ( (i2-i1) * dx + i1);
	PY1 = ( (i3-i4) * dz + i4);
	color  = ( (PY1-PY0) * dy + PY0); // final voxel color
	interpolate = true;


	// More interpolations for gradient approximation
	// Gradient calculation technique: interpolated points difference
	// 
	//       5-------i7--------6		
	//      /|          	  /|		
	//     i1------PY0-------i2|		
	//    /| |      |       /| |		
	//   1--------i8-------2 | |		
	//   | | |      |      | | |		
	//   |PX0-------G------|PX1|		
	//   | | |      |      | | |		
	//   | | 8------|i3----|-|-7    -> PZ1		
	//   | |/       |/     | |/		
	//   | i5	   PY1     | i6		
	//   |/        /       |/		
	//   4--------i4-------3    ->PZ0	
	//					

	VPPoint3D scale = ((VPVolume *)volume)->vpGetScale();

	// Interpolation values 
	i5  = ( (C8-C4) * dz + C4); 
	i6  = ( (C7-C3) * dz + C3); 
	i7  = ( (C6-C5) * dx + C5); 
	i8  = ( (C2-C1) * dx + C1); 
	PX0 = ( (i5-i1) * dy + i1); 
	PX1 = ( (i6-i2) * dy + i2); 
	PZ0 = ( (i4-i8) * dy + i8); 
	PZ1 = ( (i3-i7) * dy + i7); 
	g.x = (PX0-PX1) * scale.x; 
	g.y = (PY0-PY1) * scale.y; 
	g.z = (PZ0-PZ1) * scale.z; // gradient approximation
	
	g.vpNormalize();

 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpOtherSampleColorAndShading" process the voxel 
//				color (gray) using trilinear interpolation, the
//				the gradient approximation and the voxel light 
//				intensity. The gradient calculation technique used
//				is "centra difference"
// Parameters.: int i, int j, int k, VPGraphicObj *volume,
//				VPPoint3D p, float previousIlight, VPLight *l 
//				(i,j,k=volume matrix index; 
//				depth=ray position; volume=object volume that has all 
//				information about the volume; p="in plane" intersection
//				point; previousIlight=previous voxel light intensity;
//				l=pointer to the point light) 
//				float &color (voxel color) and float &Ilight (voxel 
//				light intensity)
// Return.....: -

void VPRayCasting::vpOtherSampleColorAndShading(int i, int j, int k,  
											VPGraphicObj *volume, 
											VPPoint3D p, float previousIlight,
											VPLight *l, float &color, float &Ilight) {

 int C1, C2, C3, C4, C5, C6, C7, C8, ii, jj, kk, Caux;
 float i1, i2, i3, i4, PY0, PY1,		// interpolated values 
	   dx, dy, dz;						// distance (delta)
 VPVector3D gradient;					// gradient vector approximation

 // First, voxels color initialization
 ii = ((i+1) < ((VPImage *)volume)->vpGetXDimension()) ? i+1 : i;
 jj = ((j+1) < ((VPImage *)volume)->vpGetYDimension()) ? j+1 : j;
 kk = ((k+1) < ((VPVolume *)volume)->vpGetZDimension()) ? k+1 : k;
 C1 = ((VPVolume *)volume)->vpGetValue(i,j,k);
 C2 = ((VPVolume *)volume)->vpGetValue(ii,j,k);
 C3 = ((VPVolume *)volume)->vpGetValue(ii,jj,k);
 C4 = ((VPVolume *)volume)->vpGetValue(i,jj,k);
 C5 = ((VPVolume *)volume)->vpGetValue(i,j,kk);
 C6 = ((VPVolume *)volume)->vpGetValue(ii,j,kk);
 C7 = ((VPVolume *)volume)->vpGetValue(ii,jj,kk);
 C8 = ((VPVolume *)volume)->vpGetValue(i,jj,kk);
 
 // If all voxels have the same color, it is not necessary to interpolate  
 if ((C1==C2) && (C2==C3) && (C3==C4) && (C4==C5) && (C5==C6) && (C6==C7) && (C7==C8)) {
	color = (float)(C1);
	Ilight = previousIlight;
 }
 else {
	// Trilinear interpolation (seven linear interpolations)
	//					
	//       5-----------------6		
	//      /|          	  /|		
	//     i1------PY0-------i2|		
	//    /  |      |       /  |		
	//   1-----------------2   |		
	//   |   |      |      |   |		
	//   |   |	  color    |   |		
	//   |   |      |      |   |		
	//   |   8------|i3----|---7		
	//   |  /       |/     |  /		
	//   | /	   PY1     | /		
	//   |/        /       |/		
	//   4--------i4-------3		
	//					

	// Distance from the voxel 1
	dx = p.x-i;
	dy = p.y-j;
	dz = p.z-k;

	// Interpolation values
	i1  = ( (C5-C1) * dz + C1);
	i2  = ( (C6-C2) * dz + C2);
	i3  = ( (C7-C8) * dx + C8);
	i4  = ( (C3-C4) * dx + C4);
	PY0 = ( (i2-i1) * dx + i1);
	PY1 = ( (i3-i4) * dz + i4);
	color = ( (PY1-PY0) * dy + PY0 ); // final voxel color


	// More processing for gradient approximation
	// Technique: Central Difference Gradient Estimator
	// 
	//        5-----------------6		
	//       /|           	   /|		
	//      / |               / |		
	//     /  |              /  |		
	// C(x,y,z)-------------2   |		
	//    |   |             |   |		
	//    |   |             |   |		
	//    |   |             |   |		
	//    |   8-------------|---C(x+1,y+1,z+1)     		
	//    |  /              |  /		
	//    | / 	            | /		
	//    |/                |/		
	//    4-----------------3     	
	//					
	// Gx = C(x+1,y,z) - C(x-1,y,z)
	// Gy = C(x,y+1,z) - C(x,y-1,z)
	// Gz = C(x,y,z+1) - C(x,y,z-1)

	if ( (i-1) >= 0  ) {
		Caux = ((VPVolume *)volume)->vpGetValue(i-1,j,k);
		gradient.x = C2-Caux; // gradient approximation
	}
	else {
		gradient.x = C2-C1; // gradient approximation
	}

	if ( (j-1) >= 0  ) {
		Caux = ((VPVolume *)volume)->vpGetValue(i,j-1,k);
		gradient.y = C4-Caux; // gradient approximation
	}
	else {
		gradient.y = C4-C1; // gradient approximation
	}
	
	if ( (k-1) >= 0  ) {
		Caux = ((VPVolume *)volume)->vpGetValue(i,j,k-1);
		gradient.z = C5-Caux; // gradient approximation
	}
	else {
		gradient.z = C5-C1; // gradient approximation
	}

	gradient.vpNormalize();
	Ilight = vpProcessILight(gradient, l);
 
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGradientComputation" process the gradient
//				approximation. The gradient calculation technique 
//				used is "interpolated points difference"
// Parameters.: (i,j,k=volume matrix index; 
//				volume=object volume that has all information about
//				the volume; p="in plane" intersection point;
//				VPVector3D &g (gradient vector approximation)
//				and bool interpolate (if all voxels don't have the  
//				same value, interpolation occurs and i=TRUE,
//				else i=FALSE)
// Return.....: -

void VPRayCasting::vpGradientComputation(int i, int j, int k,  
										VPGraphicObj *volume, VPPoint3D p, 
										VPVector3D &g, bool &interpolate) {

 int C1, C2, C3, C4, C5, C6, C7, C8, ii, jj, kk;
 float i1, i2, i3, i4, i5, i6, i7, i8,	// interpolated values 
	   PX0, PX1, PY0, PY1, PZ0, PZ1,	// interpolated values 
	   dx, dy, dz;						// distance (delta)
 
 // First, voxels color initialization
 ii = ((i+1) < ((VPImage *)volume)->vpGetXDimension()) ? i+1 : i;
 jj = ((j+1) < ((VPImage *)volume)->vpGetYDimension()) ? j+1 : j;
 kk = ((k+1) < ((VPVolume *)volume)->vpGetZDimension()) ? k+1 : k;
 C1 = ((VPVolume *)volume)->vpGetValue(i,j,k);
 C2 = ((VPVolume *)volume)->vpGetValue(ii,j,k);
 C3 = ((VPVolume *)volume)->vpGetValue(ii,jj,k);
 C4 = ((VPVolume *)volume)->vpGetValue(i,jj,k);
 C5 = ((VPVolume *)volume)->vpGetValue(i,j,kk);
 C6 = ((VPVolume *)volume)->vpGetValue(ii,j,kk);
 C7 = ((VPVolume *)volume)->vpGetValue(ii,jj,kk);
 C8 = ((VPVolume *)volume)->vpGetValue(i,jj,kk);

 // If all voxels have the same color, it is not necessary to interpolate  
 if ((C1==C2) && (C2==C3) && (C3==C4) && (C4==C5) && (C5==C6) && (C6==C7) && (C7==C8)) {
	interpolate = false;
 }
 else {
	// More interpolations for gradient approximation
	// Gradient calculation technique: interpolated points difference
	// 
	//       5-------i7--------6		
	//      /|          	  /|		
	//     i1------PY0-------i2|		
	//    /| |      |       /| |		
	//   1--------i8-------2 | |		
	//   | | |      |      | | |		
	//   |PX0-------G------|PX1|		
	//   | | |      |      | | |		
	//   | | 8------|i3----|-|-7    -> PZ1		
	//   | |/       |/     | |/		
	//   | i5	   PY1     | i6		
	//   |/        /       |/		
	//   4--------i4-------3    ->PZ0	
	//					

	// Distance from the voxel 1
	dx = p.x-i;
	dy = p.y-j;
	dz = p.z-k;
	VPPoint3D scale = ((VPVolume *)volume)->vpGetScale();

	// Interpolation values
	i1  = ( (C5-C1) * dz + C1);
	i2  = ( (C6-C2) * dz + C2);
	i3  = ( (C7-C8) * dx + C8);
	i4  = ( (C3-C4) * dx + C4);
	i5  = ( (C8-C4) * dz + C4); 
	i6  = ( (C7-C3) * dz + C3); 
	i7  = ( (C6-C5) * dx + C5); 
	i8  = ( (C2-C1) * dx + C1); 
	PY0 = ( (i2-i1) * dx + i1);
	PY1 = ( (i3-i4) * dz + i4);
	PX0 = ( (i5-i1) * dy + i1); 
	PX1 = ( (i6-i2) * dy + i2); 
	PZ0 = ( (i4-i8) * dy + i8); 
	PZ1 = ( (i3-i7) * dy + i7); 
	g.x = (PX0-PX1) * scale.x; 
	g.y = (PY0-PY1) * scale.y; 
	g.z = (PZ0-PZ1) * scale.z; // gradient approximation
	
	g.vpNormalize();
	interpolate = true;
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpProcessILight" process the light 
//				intensity.
// Parameters.: VPVector3D g (gradient approximation)
//				VPLight *l (pointer to the point light)
// Return.....: float Ilight (voxel light intensity)

float VPRayCasting::vpProcessILight(VPVector3D g, VPLight *light) {
	float Ilight=0, M=0, dot=0;
	VPVector3D l(1,0,0);

	// De acordo com a fórmula do livro...
	l = ((VPDirectionalLight *)light)->vpGetDirection();
	dot = g.vpDotProduct(l); 
          
	if (dot < 0.0)
		dot = -dot;
		//dot = (float) 0.0;	// no negative contributions

	Ilight = ambientLight + (diffuseLight * dot); 

	if (Ilight > 1) 
		Ilight = 1;

	return Ilight;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpProcessSpecularILight" process the light 
//				intensity.
// Parameters.: VPVector3D g (gradient approximation)
//				VPPoint3D o (observer and light position)
// Return.....: float Ilight (voxel light intensity)

float VPRayCasting::vpProcessSpecularILight(VPVector3D g, VPLight *light, VPVector3D o) {
	float Ilight=0, dot1=0, dot2=0, dot3=0;

	// De acordo com a fórmula do livro...
	VPVector3D l = ((VPDirectionalLight *)light)->vpGetDirection();
	dot1 = g.vpDotProduct(l); // g . l
	dot2 = g.vpDotProduct(o); // g . o
	dot3 = o.vpDotProduct(l); // o . l
         
	dot1 = fabs(dot1);
	dot2 = fabs(dot2);
	dot3 = fabs(dot3);

	Ilight = ambientLight + (diffuseLight * dot1) + (float) ( pow( ((2 * dot1 * dot2) - dot3), 50 ) ); 

	if (Ilight > 1) 
		Ilight = 1;

	return Ilight;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetEndSBand" sends a new value for the 
//				endOfS-Band attribute (used for inner structures 
//				visualization).
// Parameters.: VPPoint3D sb (view proportional variation for x and z)
//				VPGraphicObj *volume,
//				int vt (visualization type)
// Return.....: -

void VPRayCasting::vpSetEndSBand(VPPoint3D sb, VPGraphicObj *volume, int vt) {

	VPVector3D scanDirection;
	VPPoint3D volumeCameraLocation = ((VPVolume *)volume)->vpGetCameraLocationForInnerStructure();

	// "Assuming" that the target is always the central focal point
	VPPoint3D target = ((VPVolume *)volume)->vpGetCenterFocalPoint();
	target.y = (float) virtualYDimension/2;	

	 if (vt == INNERSTRTOPSLICE) {
			// To verify scan direction
			scanDirection.vpSetVector3D(target - volumeCameraLocation);
			scanDirection.vpNormalize();
			
			if (scanDirection.x==0 && scanDirection.z<0)
			{
				if ( (sb.x==0 && sb.z<0) || (sb.x>0 && sb.z<0) || (sb.x<0 && sb.z<0) )
					endOfSBand += abs(sb.z);
				else if ( (sb.x==0 && sb.z>0) || (sb.x<0 && sb.z>0) || (sb.x>0 && sb.z>0)  )
					endOfSBand -= abs(sb.z);
			}
			else if (scanDirection.x==0 && scanDirection.z>0)
			{
				if ( (sb.x==0 && sb.z<0) || (sb.x>0 && sb.z<0) || (sb.x<0 && sb.z<0) )
					endOfSBand -= abs(sb.z);
				else if ( (sb.x==0 && sb.z>0) || (sb.x<0 && sb.z>0) || (sb.x>0 && sb.z>0)  )
					endOfSBand += abs(sb.z);
			}
			else if (scanDirection.x<0 && scanDirection.z==0)
			{
				if ( (sb.x<0 && sb.z==0) || (sb.x<0 && sb.z>0) || (sb.x<0 && sb.z<0) )
					endOfSBand -= abs(sb.x);
				else if ( (sb.x>0 && sb.z==0) || (sb.x>0 && sb.z>0) || (sb.x>0 && sb.z<0)  )
					endOfSBand += abs(sb.x);
			}
			else if (scanDirection.x>0 && scanDirection.z==0)
			{
				if ( (sb.x<0 && sb.z==0) || (sb.x<0 && sb.z>0) || (sb.x<0 && sb.z<0) )
					endOfSBand += abs(sb.x);
				else if ( (sb.x>0 && sb.z==0) || (sb.x>0 && sb.z>0) || (sb.x>0 && sb.z<0)  )
					endOfSBand -= abs(sb.x);
			}
			else if (scanDirection.x<0 && scanDirection.z<0)
			{
				if ( (sb.x==0 && sb.z<0) || (sb.x>0 && sb.z<0) || (sb.x>0 && sb.z==0) || (sb.x<0 && sb.z<0) )
					endOfSBand += (abs(sb.z)+abs(sb.x)) / 2.0;
				else if ( (sb.x==0 && sb.z>0) || (sb.x<0 && sb.z>0) || (sb.x<0 && sb.z==0) || (sb.x>0 && sb.z>0)  )
					endOfSBand -= (abs(sb.z)+abs(sb.x)) / 2.0;
			}
			else if (scanDirection.x>0 && scanDirection.z>0)
			{
				if ( (sb.x==0 && sb.z<0) || (sb.x>0 && sb.z<0) || (sb.x>0 && sb.z==0) || (sb.x<0 && sb.z<0) )
					endOfSBand -= (abs(sb.z)+abs(sb.x)) / 2.0;
				else if ( (sb.x==0 && sb.z>0) || (sb.x<0 && sb.z>0) || (sb.x<0 && sb.z==0) || (sb.x>0 && sb.z>0)  )
					endOfSBand += (abs(sb.z)+abs(sb.x)) / 2.0;
			}
			else if (scanDirection.x<0 && scanDirection.z>0)
			{
				if ( (sb.x==0 && sb.z<0) || (sb.x>0 && sb.z<0) || (sb.x<0 && sb.z==0) || (sb.x<0 && sb.z<0) )
					endOfSBand -= (abs(sb.z)+abs(sb.x)) / 2.0;
				else if ( (sb.x==0 && sb.z>0) || (sb.x<0 && sb.z>0) || (sb.x>0 && sb.z==0) || (sb.x>0 && sb.z>0)  )
					endOfSBand += (abs(sb.z)+abs(sb.x)) / 2.0;
			}
			else if (scanDirection.x>0 && scanDirection.z<0)
			{
				if ( (sb.x==0 && sb.z<0) || (sb.x>0 && sb.z<0) || (sb.x<0 && sb.z==0) || (sb.x<0 && sb.z<0) )
					endOfSBand += (abs(sb.z)+abs(sb.x)) / 2.0;
				else if ( (sb.x==0 && sb.z>0) || (sb.x<0 && sb.z>0) || (sb.x>0 && sb.z==0) || (sb.x>0 && sb.z>0)  )
					endOfSBand -= (abs(sb.z)+abs(sb.x)) / 2.0;
			}
						
			// Obs: The point is always in the middle from bottom to top
	 }
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetEndSBand" get the value of the 
//				endOfSBand attribute (used for inner structures 
//				visualization).
// Parameters.: -
// Return.....: int endOfSBand 

int VPRayCasting::vpGetEndSBand() { 
	return endOfSBand;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetEndTBand" sends a new value for the 
//				endOfTBand attribute (used for inner structures 
//				visualization).
// Parameters.: VPPoint3D tb (view proportional variation for x and z)
//				VPGraphicObj *volume,
//				int vt (visualization type)
// Return.....: -

void VPRayCasting::vpSetEndTBand(VPPoint3D tb, VPGraphicObj *volume, int vt) { 

 VPVector3D scanDirection;
 VPPoint3D volumeCameraLocation = ((VPVolume *)volume)->vpGetCameraLocationForInnerStructure();

 // "Assuming" that the target is always the central focal point
 VPPoint3D target = ((VPVolume *)volume)->vpGetCenterFocalPoint();
 target.y = (float) virtualYDimension/2;	

 if (vt == INNERSTRTOPSLICE) {
		// To verify scan direction
		scanDirection.vpSetVector3D(target - volumeCameraLocation);
		scanDirection.vpNormalize();

		if (scanDirection.x==0 && scanDirection.z<0)
		{
			if ( (tb.x==0 && tb.z<0) || (tb.x>0 && tb.z<0) || (tb.x<0 && tb.z<0) )
				endOfTBand += abs(tb.z);
			else if ( (tb.x==0 && tb.z>0) || (tb.x<0 && tb.z>0) || (tb.x>0 && tb.z>0)  )
				endOfTBand -= abs(tb.z);
		}
		else if (scanDirection.x==0 && scanDirection.z>0)
		{
			if ( (tb.x==0 && tb.z<0) || (tb.x>0 && tb.z<0) || (tb.x<0 && tb.z<0) )
				endOfTBand -= abs(tb.z);
			else if ( (tb.x==0 && tb.z>0) || (tb.x<0 && tb.z>0) || (tb.x>0 && tb.z>0)  )
				endOfTBand += abs(tb.z);
		}
		else if (scanDirection.x<0 && scanDirection.z==0)
		{
			if ( (tb.x<0 && tb.z==0) || (tb.x<0 && tb.z>0) || (tb.x<0 && tb.z<0) )
				endOfTBand -= abs(tb.x);
			else if ( (tb.x>0 && tb.z==0) || (tb.x>0 && tb.z>0) || (tb.x>0 && tb.z<0)  )
				endOfTBand += abs(tb.x);
		}
		else if (scanDirection.x>0 && scanDirection.z==0)
		{
			if ( (tb.x<0 && tb.z==0) || (tb.x<0 && tb.z>0) || (tb.x<0 && tb.z<0) )
				endOfTBand += abs(tb.x);
			else if ( (tb.x>0 && tb.z==0) || (tb.x>0 && tb.z>0) || (tb.x>0 && tb.z<0)  )
				endOfTBand -= abs(tb.x);
		}
		else if (scanDirection.x<0 && scanDirection.z<0)
		{
			if ( (tb.x==0 && tb.z<0) || (tb.x>0 && tb.z<0) || (tb.x>0 && tb.z==0) || (tb.x<0 && tb.z<0) )
				endOfTBand += (abs(tb.z)+abs(tb.x)) / 2.0;
			else if ( (tb.x==0 && tb.z>0) || (tb.x<0 && tb.z>0) || (tb.x<0 && tb.z==0) || (tb.x>0 && tb.z>0)  )
				endOfTBand -= (abs(tb.z)+abs(tb.x)) / 2.0;
		}
		else if (scanDirection.x>0 && scanDirection.z>0)
		{
			if ( (tb.x==0 && tb.z<0) || (tb.x>0 && tb.z<0) || (tb.x>0 && tb.z==0) || (tb.x<0 && tb.z<0) )
				endOfTBand -= (abs(tb.z)+abs(tb.x)) / 2.0;
			else if ( (tb.x==0 && tb.z>0) || (tb.x<0 && tb.z>0) || (tb.x<0 && tb.z==0) || (tb.x>0 && tb.z>0)  )
				endOfTBand += (abs(tb.z)+abs(tb.x)) / 2.0;
		}
		else if (scanDirection.x<0 && scanDirection.z>0)
		{
			if ( (tb.x==0 && tb.z<0) || (tb.x>0 && tb.z<0) || (tb.x<0 && tb.z==0) || (tb.x<0 && tb.z<0) )
				endOfTBand -= (abs(tb.z)+abs(tb.x)) / 2.0;
			else if ( (tb.x==0 && tb.z>0) || (tb.x<0 && tb.z>0) || (tb.x>0 && tb.z==0) || (tb.x>0 && tb.z>0)  )
				endOfTBand += (abs(tb.z)+abs(tb.x)) / 2.0;
		}
		else if (scanDirection.x>0 && scanDirection.z<0)
		{
			if ( (tb.x==0 && tb.z<0) || (tb.x>0 && tb.z<0) || (tb.x<0 && tb.z==0) || (tb.x<0 && tb.z<0) )
				endOfTBand += (abs(tb.z)+abs(tb.x)) / 2.0;
			else if ( (tb.x==0 && tb.z>0) || (tb.x<0 && tb.z>0) || (tb.x>0 && tb.z==0) || (tb.x>0 && tb.z>0)  )
				endOfTBand -= (abs(tb.z)+abs(tb.x)) / 2.0;
		}
		
		// Obs: The point is always in the middle from bottom to top
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetEndTBand" get the value of the 
//				endOfTBand attribute (used for inner structures 
//				visualization).
// Parameters.: -
// Return.....: int endOfTBand

int VPRayCasting::vpGetEndTBand() {
	return endOfTBand;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetRayCastingSampleStep" sends a new value 
//				to the sampleStep attribute.
// Parameters.: float s (new value)
// Return.....: -

void VPRayCasting::vpSetSampleStep(float s) {
	sampleStep = s;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetRayCastingSampleStep" returns the  
//				sampleStep attribute value.
// Parameters.: -
// Return.....: sampleStep attribute value

float VPRayCasting::vpGetSampleStep() {
	return sampleStep;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetAmbientLight" sends a new value 
//				to the ambientLight attribute.
// Parameters.: float a (new value)
// Return.....: -

void VPRayCasting::vpSetAmbientLight(float a) {
	ambientLight = a;
	lightVolumeComputation = true;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetAmbientLight" returns the ambientLight
//				attribute value.
// Parameters.: -
// Return.....: ambientLight attribute value

float VPRayCasting::vpGetAmbientLight() {
	return ambientLight;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetDiffuseLight" sends a new value 
//				to the diffuseLight attribute.
// Parameters.: float d (new value)
// Return.....: -

void VPRayCasting::vpSetDiffuseLight(float d) {
	diffuseLight = d;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetDiffuseLight" returns the diffuseLight
//				attribute value.
// Parameters.: -
// Return.....: diffuseLight attribute value

float VPRayCasting::vpGetDiffuseLight() {
	return diffuseLight;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetSpecularExponent" sends a new value 
//				to the specularExponent attribute.
// Parameters.: int s (new value)
// Return.....: -

void  VPRayCasting::vpSetSpecularExponent(int s) {
	specularExponent = s;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetSpecularExponent" returns the 
//				specularExponent attribute value.
// Parameters.: -
// Return.....: specularExponent attribute value

int   VPRayCasting::vpGetSpecularExponent() {
	return specularExponent;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetSpecular" sends a new value 
//				to the specular attribute.
// Parameters.: bool s (new value)
// Return.....: -

void  VPRayCasting::vpSetSpecular(bool s) {
	specular = s;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetSpecular" returns the 
//				specular attribute value.
// Parameters.: -
// Return.....: specular attribute value

bool  VPRayCasting::vpGetSpecular() {
	return specular;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetShadingMethod" sends a new value 
//				to the shadingMethod attribute.
// Parameters.: float sm (new value)
// Return.....: -

void VPRayCasting::vpSetShadingMethod(int sm) {
	shadingMethod = sm;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetShadingMethod" returns the 
//				shadingMethod attribute value.
// Parameters.: -
// Return.....: shadingMethod attribute value

int VPRayCasting::vpGetShadingMethod() {
	return shadingMethod;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetLightDirection" sends a new value 
//				to the light direction. For actualization, the
//				light attribute is used.
// Parameters.: VPVector3D ld (new value)
// Return.....: -

void VPRayCasting::vpSetLightDirection(VPVector3D ld) {
	lightVolumeComputation = true;
	((VPDirectionalLight *)light)->vpSetDirection(ld);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLightDirection" returns the light
//				direction in accordance with the light attribute 
//				value.
// Parameters.: -
// Return.....: VPVector3D ld (light direction)

VPVector3D VPRayCasting::vpGetLightDirection() {
	return ((VPDirectionalLight *)light)->vpGetDirection();
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetTypeOfCuttingTool" sends a new value 
//				to the typeOfCuttingTool attribute.
// Parameters.: int t (new value)
// Return.....: -

void VPRayCasting::vpSetTypeOfCuttingTool(int t) {
	typeOfCuttingTool = t;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetTypeOfCuttingTool" returns the 
//				typeOfCuttingTool attribute value.
// Parameters.: -
// Return.....: int typeOfCuttingTool

int VPRayCasting::vpGetTypeOfCuttingTool() {
	return typeOfCuttingTool;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpDefineA" proccess the "a" variable when
//				some cutting tool are being used.
// Parameters.: -
// Return.....: -

void VPRayCasting::vpDefineA(VPPoint3D &pIn, VPPoint3D pOut, VPPoint3D p1, VPVector3D deltaZ, float yCorrection, VPVector3D plane1Normal, VPGraphicObj *volume, VPPoint3D volumeDimension, float d) {
 int i;
 float denom, f, g, h;
 VPPoint3D pInNew, pInAux, temp;

 switch(typeOfCuttingTool)
 {
	case OBLIQUESLICE:	// Cutting technique: just one slice (orthogonal or oblique, according to camera position)
						pIn = p1;
						pIn.y = pIn.y*yCorrection;
						for(i=0; i<(sampleStep*distanceObliquePlane); i++) //sampleStep*80 ou *53
							pIn = pIn + deltaZ; // projectionDirection; 
						pIn = pIn + deltaZ;
						break;
	case TWOCUTPLANES:  
	case ONECUTPLANE:	// Cutting technique: one cut plane (from the cut plane INTERSECTION 
						//					to the end of the volume (back) ) - plane1Normal, d, ray=pIn-pOut 
						f = pIn.x - pOut.x;
						g = pIn.y - pOut.y;
						h = pIn.z - pOut.z;
						denom = plane1Normal.x*f + plane1Normal.y*g + plane1Normal.z*h;
						denom = - ( (plane1Normal.x*pOut.x + plane1Normal.y*pOut.y + plane1Normal.z*pOut.z + d) / denom );
						pInNew.x = pOut.x + f*denom;
						pInNew.y = pOut.y + g*denom;
						pInNew.z = pOut.z + h*denom; // pInNew = intersection point
						pInAux = pInNew + deltaZ;
						f = plane1Normal.x*pInAux.x + plane1Normal.y*pInAux.y + plane1Normal.z*pInAux.z + d;
						g = plane1Normal.x*pIn.x    + plane1Normal.y*pIn.y    + plane1Normal.z*pIn.z    + d;
						if ( f < 0 && g > 0 )
							pIn = pInNew;
						break;
	case TWOCUTPLANESINCLUSIONOPAC: // Cutting technique: just if the samples are IN the cut planes 
									//						(plane1Normal, d, ray=pIn-pOut)
						f = pIn.x - pOut.x;
						g = pIn.y - pOut.y;
						h = pIn.z - pOut.z;
						denom = plane1Normal.x*f + plane1Normal.y*g + plane1Normal.z*h;
						denom = - ( (plane1Normal.x*pOut.x + plane1Normal.y*pOut.y + plane1Normal.z*pOut.z + d) / denom );
						pInNew.x = pOut.x + f*denom;
						pInNew.y = pOut.y + g*denom;
						pInNew.z = pOut.z + h*denom; // pInNew = intersection point
						pInAux = pInNew + deltaZ;
						f = plane1Normal.x*pInAux.x + plane1Normal.y*pInAux.y + plane1Normal.z*pInAux.z + d;
						g = plane1Normal.x*pIn.x    + plane1Normal.y*pIn.y    + plane1Normal.z*pIn.z    + d;
						if ( f < 0 && g > 0 )
							pIn = pInNew;
						break;
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpDefineB" proccess the "b" variable when
//				some cutting tool are being used.
// Parameters.: -
// Return.....: -

void VPRayCasting::vpDefineB(int i, int j, int k, float &alpha, VPGraphicObj *volume, VPPoint3D &pIn, int depth, int &ivalue) {

 switch(typeOfCuttingTool)
 {
	case TWOCUTPLANES:  // BAKA para simular um plano de corte traseiro 
						if (j > backCuttingPlane)
							alpha = 0.0;
						break;
	case CUBEBYINCLUSION: // Cutting technique: a parallelepiped is used as selection tool 
						  // Do not consider everything that is outside the parallelepiped
						if ( ( (i>p2Parallelepiped.x) || (i<p1Parallelepiped.x) ) ||
							 ( (j>p2Parallelepiped.y) || (j<p1Parallelepiped.y) ) || 
							 ( (k>p2Parallelepiped.z) || (k<p1Parallelepiped.z) ) )
							alpha = 0.0;
						break;
	case CUBEBYEXCLUSION: // Cutting technique: with a parallelepiped as a cut volume
						if ( ( (i<p2Parallelepiped.x) && (i>p1Parallelepiped.x) ) &&
							 ( (j<p2Parallelepiped.y) && (j>p1Parallelepiped.y) ) && 
							 ( (k<p2Parallelepiped.z) && (k>p1Parallelepiped.z) ) )
							alpha = 0.0;
							//if (alpha != 0) alpha = 0.05;
						break;
	case SPHEREBYINCLUSION:
						break;
	case SPHEREBYEXCLUSION:
						break;

 }

}


/*+-------------------------------------------------------------------*/
/*+--------------+
  |   Classify   |
  +--------------+*/

/* This routine performs the actual classification */

void	VPRayCasting::Classify
(
	float	*intensity,
	float	*gm,
	float	*luminance,
	float	*alpha
)

{
	int		iGm;
	int		composite;

    /* 442 is maximum gm value. SQRT(255^2 + 255^2 + 255^2), since we only
	   work on 8 bit data. The gradient magnitude is converted into six
	   bits.
	*/

	iGm = (int) (*gm / 442.0f * 64.0f);	
	composite = (iGm<<8) | (unsigned char) *intensity;

	*alpha = (float) alphaTable[composite] / 255.0f;
	*luminance = (float) luminanceTable[composite];
}


/*+-------------------------------------------------------------------*/
/*+-------------------+
  |   ClassifyColor   |
  +-------------------+*/

void	VPRayCasting::ClassifyColor
(
	float	*intensity,
	float	*gm,
	float	*r,
	float	*g,
	float	*b,
	float	*alpha
)

{
	int		iGm;
	int		composite;

    /* 442 is maximum gm value. SQRT(255^2 + 255^2 + 255^2), since we only
	   work on 8 bit data. The gradient magnitude is converted into six
	   bits.
	*/

	iGm = (int) (*gm / 442.0f * 64.0f);	
	composite = (iGm<<8) | (unsigned char) *intensity;

	*alpha = (float) alphaTable[composite] / 255.0f;
	*r = (float) RGBtable[3*composite];
	*g = (float) RGBtable[3*composite+1];
	*b = (float) RGBtable[3*composite+2];

}


/*+-------------------------------------------------------------------*/
/*+----------------------------+
  |   SetClassificationTable   |
  +----------------------------+*/

void	VPRayCasting::SetClassificationTable (void)

/* Precompute the opacity lookup table and the color lookup table */

{
		float			alpha;
		float           denominator;
		int				index, magnitude, intensity, ind;


/* The luminance table is only used when the final image should be
   a grayscale. */

/* The format that is assumed here is 2 bits for index information,
   6 bits for gradient magnitude and 8 bits for intensity values,
   in that order */
  
	/* The levThreshold and levWidth variable determine the position
	   of the classification function. Experiment with different 
	   values to see their effect. The current settings are reasonable
	   for both the ramp classification function and the levoy 'tent'
	   classification function. But they are dataset dependent, and
	   also will differ depending on what you want to make translucent
	   in a dataset.

  		Now, these values are read from the parameters file and the 
		following comands are not necessary...
		levThreshold = (float) 180.0; // 128.0 para a engine; 88.0 para cthead.vol
		levWidth = (float) 3.0; //2.0 para a engine; 4.0 para cthead.vol

	 */

	for (index = 0; index < 4; index++)
	{
		for (magnitude = 0; magnitude < 64; magnitude++)
		{
			for (intensity = 0; intensity < 256; intensity++)
			{
				ind = intensity + (magnitude << 8) + (index << 14);

			/*----- Simple ramp classification function -----*/

#if 0
				if ((intensity >= levThreshold) &&
					(intensity < (levThreshold + levWidth)))
					alpha = ((float) intensity - levThreshold) / levWidth;
				else if (intensity < levThreshold)
					alpha= (float) 0.0;
				else
					alpha= (float) 1.0;

#endif

			/*----- Slight variation on Levoy's 'tent' classification function -----*/

				denominator = levWidth * (float) (magnitude<<2);
                if (denominator > EPSILON)
                    alpha = (float) (1.0 -
                        (double) fabs ((double) ((levThreshold-intensity) /
                        denominator)));
                else
                    alpha = (float) 0.0;


			/*----- Encode alpha value into unsigned char table -----*/

				if (alpha>0.0)
					alphaTable[ind] = (unsigned char) (255.0 * alpha);
				else
					alphaTable[ind] = (unsigned char) 0;

			/*----- Set luminance value in unsigned char table -----*/

				luminanceTable[ind] = 255;

			/*----- For index values other than zero, set a color -----*/

				if (index==0)
				{
					RGBtable[3*ind  ] = 255;
					RGBtable[3*ind+1] = 255;
					RGBtable[3*ind+2] = 255;
				}
				else if (index==2)
				{
					RGBtable[3*ind  ] = 243;
					RGBtable[3*ind+1] = 208;
					RGBtable[3*ind+2] = 190;
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
				else if (index==1)
				{
					RGBtable[3*ind  ] = (unsigned char) (255.0 *0.95);
					RGBtable[3*ind+1] = (unsigned char) (255.0 *0.10);
					RGBtable[3*ind+2] = (unsigned char) (255.0 *0.10);
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
				else if (index==3)
				{
					RGBtable[3*ind  ] = (unsigned char) (255.0 *0.38);
					RGBtable[3*ind+1] = (unsigned char) (255.0 *0.50);
					RGBtable[3*ind+2] = (unsigned char) (255.0 *0.90);
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
			}
		}
	}
}


/*+-------------------------------------------------------------------*/
/*+-----------------------------+
  |   SetClassificationTable2   |
  +-----------------------------+*/

void	VPRayCasting::SetClassificationTable2 (void)

/* Precompute the opacity lookup table and the color lookup table */

{
		float			alpha;
		int				index, magnitude, intensity, ind;

/* The luminance table is only used when the final image should be
   a grayscale. */

/* The format that is assumed here is 2 bits for index information,
   6 bits for gradient magnitude and 8 bits for intensity values,
   in that order */
  
	/* The levThreshold and levWidth variable determine the position
	   of the classification function. Experiment with different 
	   values to see their effect. The current settings are reasonable
	   for both the ramp classification function and the levoy 'tent'
	   classification function. But they are dataset dependent, and
	   also will differ depending on what you want to make translucent
	   in a dataset.

		Now, these values are read from the parameters file and the 
		following comands are not necessary...
		levThreshold = (float) 252.0; // 128.0 para a engine
		levWidth = (float) 4.0; //2.0
	 */


	for (index = 0; index < 4; index++)
	{
		for (magnitude = 0; magnitude < 64; magnitude++)
		{
			for (intensity = 0; intensity < 256; intensity++)
			{
				ind = intensity + (magnitude << 8) + (index << 14);

			/*----- Simple ramp classification function -----*/

#if 0
				if ((intensity >= levThreshold) &&
					(intensity < (levThreshold + levWidth)))
					alpha = ((float) intensity - levThreshold) / levWidth;
				else if (intensity < levThreshold)
					alpha= (float) 0.0;
				else
					alpha= (float) 1.0;

#endif

			/*----- Função linear (despreza o que for menor que 40 ou 8) -----*/
				if ( intensity < initialValueForLinearOpacity)
					alpha = ( (float) 0);
				else
					alpha = ((float) (intensity-initialValueForLinearOpacity) / (float)(255-initialValueForLinearOpacity) );

			/*----- Função linear (bloco: 80=0.06, 240-255=1) -----*/
			//if (intensity == 80)
			//	alpha = ( (float) 0.06 );
			//else if ( intensity >=240 )
			//	alpha = ( (float) 1.0 );
			//else
			//	alpha = ( (float) 0.0 );

			/*----- Função linear (rampa) -----*/
			//float ic = (float)255.0 / (float)2.0;
			//if (intensity > ic)
			//	alpha =  ( ((float)225.0) - intensity ) / ic;
			//else
			//	alpha =  intensity / ic;

			/*----- Função linear (exponencial) NÃO FUNCIONOU!!! -----*/
			//float aux = ( (9.0f * intensity) / 255.0f ) + 1.0f;
			//aux = (float) log(aux);

			/*----- Encode alpha value into unsigned char table -----*/

				if (alpha>0.0)
					alphaTable[ind] = (unsigned char) (255.0 * alpha);
				else
					alphaTable[ind] = (unsigned char) 0;

			/*----- Set luminance value in unsigned char table -----*/

				luminanceTable[ind] = 255;

			/*----- For index values other than zero, set a color -----*/

				if (index==0)
				{
					RGBtable[3*ind  ] = 255;
					RGBtable[3*ind+1] = 255;
					RGBtable[3*ind+2] = 255;
				}
				else if (index==2)
				{
					RGBtable[3*ind  ] = 243;
					RGBtable[3*ind+1] = 208;
					RGBtable[3*ind+2] = 190;
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
				else if (index==1)
				{
					RGBtable[3*ind  ] = (unsigned char) (255.0 *0.95);
					RGBtable[3*ind+1] = (unsigned char) (255.0 *0.10);
					RGBtable[3*ind+2] = (unsigned char) (255.0 *0.10);
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
				else if (index==3)
				{
					RGBtable[3*ind  ] = (unsigned char) (255.0 *0.38);
					RGBtable[3*ind+1] = (unsigned char) (255.0 *0.50);
					RGBtable[3*ind+2] = (unsigned char) (255.0 *0.90);
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
			}
		}
	}
}


/*+-------------------------------------------------------------------*/
/*+-----------------------------+
  |   SetClassificationTable3   |
  +-----------------------------+*/

void	VPRayCasting::SetClassificationTable3 (void)

/* Precompute the opacity lookup table and the color lookup table */

{
		float			alpha;
		int				index, magnitude, intensity, ind;

/* The luminance table is only used when the final image should be
   a grayscale. */

/* The format that is assumed here is 2 bits for index information,
   6 bits for gradient magnitude and 8 bits for intensity values,
   in that order */
  
	/* The levThreshold and levWidth variable determine the position
	   of the classification function. Experiment with different 
	   values to see their effect. The current settings are reasonable
	   for both the ramp classification function and the levoy 'tent'
	   classification function. But they are dataset dependent, and
	   also will differ depending on what you want to make translucent
	   in a dataset.

  	   Now, these values are read from the parameters file and the 
	   following comands are not necessary...
	   levThreshold = (float) 252.0; // 128.0 para a engine
	   levWidth = (float) 4.0; //2.0
	 */

	for (index = 0; index < 4; index++)
	{
		for (magnitude = 0; magnitude < 64; magnitude++)
		{
			for (intensity = 0; intensity < 256; intensity++)
			{
				ind = intensity + (magnitude << 8) + (index << 14);

			/*----- Simple ramp classification function -----*/

#if 0
				if ((intensity >= levThreshold) &&
					(intensity < (levThreshold + levWidth)))
					alpha = ((float) intensity - levThreshold) / levWidth;
				else if (intensity < levThreshold)
					alpha= (float) 0.0;
				else
					alpha= (float) 1.0;

#endif

			/*----- Tabela de opacidade do tipo bloco (79-81->0.1, 240-255->1.0, 0 caso contrário) -----*/
				if ( intensity >= 79 && intensity <= 81 ) 
					alpha = ( (float) 0.05);
				else if ( intensity >= 240 )
					alpha = ((float) 1 );
				else
					alpha = ((float) 0 );

			/*----- Encode alpha value into unsigned char table -----*/

				if (alpha>0.0)
					alphaTable[ind] = (unsigned char) (255.0 * alpha);
				else
					alphaTable[ind] = (unsigned char) 0;

			/*----- Set luminance value in unsigned char table -----*/

				luminanceTable[ind] = 255;

			/*----- For index values other than zero, set a color -----*/

				if (index==0)
				{
					RGBtable[3*ind  ] = 255;
					RGBtable[3*ind+1] = 255;
					RGBtable[3*ind+2] = 255;
				}
				else if (index==2)
				{
					RGBtable[3*ind  ] = 243;
					RGBtable[3*ind+1] = 208;
					RGBtable[3*ind+2] = 190;
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
				else if (index==1)
				{
					RGBtable[3*ind  ] = (unsigned char) (255.0 *0.95);
					RGBtable[3*ind+1] = (unsigned char) (255.0 *0.10);
					RGBtable[3*ind+2] = (unsigned char) (255.0 *0.10);
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
				else if (index==3)
				{
					RGBtable[3*ind  ] = (unsigned char) (255.0 *0.38);
					RGBtable[3*ind+1] = (unsigned char) (255.0 *0.50);
					RGBtable[3*ind+2] = (unsigned char) (255.0 *0.90);
					alphaTable[ind]   = (unsigned char) (255.0 *0.5);
				}
			}
		}
	}
}


/*+-------------------------------------------------------------------*/
/*+-----------+
  |   Shade   |
  +-----------+*/

float	VPRayCasting::Shade
(
	VPVector3D gradient
)

{
	VPVector3D 	light;
	float		dot;
	float		intensity;	// returned attenuation factor

	//                        // frente head; // esfera;  // engine
	//light.x = (float)     0;  // -1.0;      //  0.0;    //  2.0
	//light.y = (float)     0;  // -0.25;     //  0.25;   // -1.25
	//light.z = (float)  -1.0;  //  2.0;      // -4/2.0;  //  2.0

	light = vpGetLightDirection();

	light.vpNormalize();

	dot = gradient.x * light.x +
		  gradient.y * light.y +
		  gradient.z * light.z;
          
	if (dot < 0.0) {
		if (negativeLightContribution)
			dot = -dot; 
		else
			dot = (float) 0.0;	// no negative contributions
	}

	intensity = ambientLight + (diffuseLight * dot);

	if (intensity > 1)
		intensity = 1;

	return (intensity);
}


/*+-------------------------------------------------------------------*/
/*+-------------------+
  |   ShadeSpecular   |
  +-------------------+*/

float	VPRayCasting::ShadeSpecular
(
	VPVector3D gradient,
	VPVector3D obsPoint
)

{
	VPVector3D 	light;
	float		dot1, dot2, dot3;
	float		intensity;	// returned attenuation factor

	light = vpGetLightDirection();

	light.vpNormalize();

	dot1 = gradient.vpDotProduct(light);	// g . l
	dot2 = gradient.vpDotProduct(obsPoint); // g . o
	dot3 = obsPoint.vpDotProduct(light);	// o . l
         
	dot1 = fabs(dot1);
	dot2 = fabs(dot2);
	dot3 = fabs(dot3);
          
	intensity = ambientLight + (diffuseLight * dot1) + (float) ( pow( ((2 * dot1 * dot2) - dot3), specularExponent ) ); //50-30

	if (intensity > 1)
		intensity = 1;

	return (intensity);
}


/*+-------------------------------------------------------------------*/
/*+--------------+
  |   Gradient   |
  +--------------+*/

void    VPRayCasting::Gradient (int i, int j, int k,      // The location where to compute the gradient
				  VPVector3D &gradient,    // The gradient
				  float *gm,              // The gradient magnitude
				  VPGraphicObj *volume
				  )

{
 int ii,jj,kk,i_,j_,k_;

/*----- Note that the gradient being computed is the nearest
		neighbor for the sample point.  This is being estimated
		to provide a straight forward example.  The x, y and z
		gradients should be interpolated for the sample point
		using the eight neighboring gradients.  It would be
		very time consuming to calculate eight gradients for
		each x, y and z direction and then interpolate amongst
		them.  Please see the chapter that discusses system
		design and trade-offs and then adapt those ideas to
		create your own, high-performance, high-quality renderer. -----*/

	 ii = ((i+1) < ((VPImage *)volume)->vpGetXDimension()) ? i+1 : i;
	 jj = ((j+1) < ((VPImage *)volume)->vpGetYDimension()) ? j+1 : j;
	 kk = ((k+1) < ((VPVolume *)volume)->vpGetZDimension()) ? k+1 : k;
	 i_ = ((i-1) < 0) ? i : i-1;
	 j_ = ((j-1) < 0) ? j : j-1;
	 k_ = ((k-1) < 0) ? k : k-1;

	gradient.x = (float) ((VPVolume *)volume)->vpGetValue(i_,j,k) - (float) ((VPVolume *)volume)->vpGetValue(ii,j,k);
	gradient.y = (float) ((VPVolume *)volume)->vpGetValue(i,j_,k) -
		(float) ((VPVolume *)volume)->vpGetValue(i,jj,k);
	gradient.z = (float) ((VPVolume *)volume)->vpGetValue(i,j,k_) -
		(float) ((VPVolume *)volume)->vpGetValue(i,j,kk);

	*gm = ((gradient.x * gradient.x) + (gradient.y * gradient.y) + (gradient.z * gradient.z));
	*gm = (float) sqrt ((double) *gm);

	gradient.vpNormalize();
}


/*+-------------------------------------------------------------------*/
/*+-------------------+
  |   GradientSobel   |
  +-------------------+*/

void    VPRayCasting::GradientSobel (int i, int j, int k,      // The location where to compute the gradient
				  VPVector3D &gradient,    // The gradient
				  float *gm,              // The gradient magnitude
				  VPGraphicObj *volume
				  )
{
    gradient = ((VPVolume *)volume)->vpGetSobelGradient(i, j, k);

	*gm = ((gradient.x * gradient.x) + (gradient.y * gradient.y) + (gradient.z * gradient.z));
	*gm = (float) sqrt ((double) *gm);

	gradient.vpNormalize();
}


