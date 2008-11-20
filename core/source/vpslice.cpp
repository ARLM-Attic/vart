///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpslice.cpp
//  DESCRIPTION.: Contain the VPSlice class implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: October/04/2000
//  DESCRIPTION.: Constructors definition and methods implementation.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: June/04/2001
//  DESCRIPTION.: Methods and parameters changes.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: October/25/2001
//  DESCRIPTION.: Update for inner structure visualization.
//
///////////////////////////////////////////////////////////////////


#include <vpslice.h>
#include <vpimage.h>
#include <vpvolume.h>
#include <vpplane.h>
#include <cmath>
#include <list>
using namespace std;


///////////////////////////////////////////////////////////////////
// Description: Class "VPSlice" constructor without parameter. 
// Parameters.: -
// Return.....: -

VPSlice::VPSlice () {
	lineSBand = NULL;
	lineTBand = NULL;
	numTotalPoints = 0;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPSlice" destructor. 
// Parameters.: -
// Return.....: -

VPSlice::~VPSlice () {
	if (lineSBand != NULL) {
		delete [] lineSBand;
		delete [] lineTBand;
	}
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpTopSliceRender" implement the algorithm 
//				to visualize one volume slice from the top.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image);
// Return.....: -

void VPSlice::vpTopSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 imageXdelta=0, imageYdelta=0, whiteLine=0, whiteColumn=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, vectorH, vectorV, deltaX, deltaY, up;
 VPPoint3D p1, minPlaneProjection; 

 // Set pointer to the volume object
 VPGraphicObj *volume = v;

 // Set a pointer to the right camera
 VPCamera *camera = c;

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 // Set "h" and "v" vectors 
 up = camera->vpGetUp();
 vectorH = up.vpCrossProduct(projectionDirection);
 vectorV = vectorH.vpCrossProduct(projectionDirection);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
 if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = (int) (p1.vpGetX()); //(column);
		j = sliceNumber;		 //(line);
		k = (int) (p1.vpGetZ()); //(depth);

		p1.vpSetY(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		image[line+imageYdelta][column+imageXdelta] = (unsigned int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSideSliceRender" implement the algorithm 
//				to visualize one volume slice from the side.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image);
// Return.....: -

void VPSlice::vpSideSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 imageXdelta=0, imageYdelta=0, virtualYDimension=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0, yCorrection=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, vectorH, vectorV, deltaX, deltaY, up;
 VPPoint3D p1, minPlaneProjection; 

 // Set pointer to the volume object
 VPGraphicObj *volume = v;

 // Set a pointer to the right camera
 VPCamera *camera = c;

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 // Set "h" and "v" vectors 
 up = camera->vpGetUp();
 vectorH = up.vpCrossProduct(projectionDirection);
 vectorV = vectorH.vpCrossProduct(projectionDirection);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
 if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // To verify the smallest dimension and set the virtualVolumeDimension
 if ( ((VPImage *)volume)->vpGetXDimension() < ((VPVolume *)volume)->vpGetZDimension() )
	virtualYDimension = ((VPVolume *)volume)->vpGetZDimension();
 else 
	virtualYDimension = ((VPImage *)volume)->vpGetXDimension();	
 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 
	 p1.y = p1.y*yCorrection;

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = sliceNumber;		//(column);
		j = (int) (p1.vpGetY());//(line);
		k = (int) (p1.vpGetZ());//(depth);

		p1.vpSetX(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		image[line+imageYdelta][column+imageXdelta] = (unsigned int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFrontSliceRender" implement the algorithm 
//				to visualize one volume slice from the front.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image);
// Return.....: -

void VPSlice::vpFrontSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 imageXdelta=0, imageYdelta=0, virtualYDimension=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0, yCorrection=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, vectorH, vectorV, deltaX, deltaY, up;
 VPPoint3D p1, minPlaneProjection; 

 // Set pointer to the volume object
 VPGraphicObj *volume = v;

 // Set a pointer to the right camera
 VPCamera *camera = c;

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();

 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 // Set "h" and "v" vectors 
 up = camera->vpGetUp();
 vectorH = projectionDirection.vpCrossProduct(up);
 vectorV = projectionDirection.vpCrossProduct(vectorH);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
  if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // To verify the smallest dimension and set the virtualVolumeDimension
 if ( ((VPImage *)volume)->vpGetXDimension() < ((VPVolume *)volume)->vpGetZDimension() )
	virtualYDimension = ((VPVolume *)volume)->vpGetZDimension();
 else 
	virtualYDimension = ((VPImage *)volume)->vpGetXDimension();	
 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 // Window/Viewport ratio
 ratioX = winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 
	 p1.y = p1.y*yCorrection;

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = (int) (p1.vpGetX());//(column);
		j = (int) (p1.vpGetY());//(line);
		k = sliceNumber;		//(depth);

		p1.vpSetZ(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		image[line+imageYdelta][column+imageXdelta] = (unsigned int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpLineTopSliceRender" implement the algorithm 
//				to visualize one volume slice from the top. Lines
//				indicating the position of the other two orthogonal
//				planes are draw in the image.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image);
// Return.....: -

void VPSlice::vpLineTopSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 imageXdelta=0, imageYdelta=0, whiteLine=0, whiteColumn=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, vectorH, vectorV, deltaX, deltaY, up;
 VPPoint3D p1, minPlaneProjection; 

 // Set pointer to the volume object
 VPGraphicObj *volume = v;

 // Set a pointer to the right camera
 VPCamera *camera = c;

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 // Set "h" and "v" vectors 
 up = camera->vpGetUp();
 vectorH = up.vpCrossProduct(projectionDirection);
 vectorV = vectorH.vpCrossProduct(projectionDirection);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
 if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();

 // Other orthogonal planes visualization values
 // (goal: put a line in the right position)
 int side = ((VPVolume *)volume)->vpGetSideSlice();
 int front = ((VPVolume *)volume)->vpGetFrontSlice();
 bool sideFlag = true, frontFlag = true;

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = (int) (p1.vpGetX()); //(column);
		j = sliceNumber;		 //(line);
		k = (int) (p1.vpGetZ()); //(depth);

		p1.vpSetY(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		if ( ((k<=front+1)&&(k>=front-1)) && frontFlag ) {
			frontFlag = false;
			front = line+imageYdelta;
		}
		else if ( ((i<=side+1)&&(i>=side-1)) && sideFlag ) {
			sideFlag = false;
			side = column+imageXdelta;
		}
		else
			image[line+imageYdelta][column+imageXdelta] = (unsigned int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)


  // To draw the line of the other orthogonal planes visualization
  finalLineValue = camera->vpGetViewHeight();
  for (line=0; line<finalLineValue; line++)
	image[line][side] = (unsigned int) 255;

  finalColumnValue = camera->vpGetViewWidth();
  for (column=0; column<finalColumnValue; column++)
	image[front][column] = (unsigned int) 255;

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpLineSideSliceRender" implement the algorithm 
//				to visualize one volume slice from the side. Lines
//				indicating the position of the other two orthogonal
//				planes are draw in the image.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image);
// Return.....: -

void VPSlice::vpLineSideSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 imageXdelta=0, imageYdelta=0, virtualYDimension=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0, yCorrection=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, vectorH, vectorV, deltaX, deltaY, up;
 VPPoint3D p1, minPlaneProjection; 

 // Set pointer to the volume object
 VPGraphicObj *volume = v;

 // Set a pointer to the right camera
 VPCamera *camera = c;

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 // Set "h" and "v" vectors 
 up = camera->vpGetUp();
 vectorH = up.vpCrossProduct(projectionDirection);
 vectorV = vectorH.vpCrossProduct(projectionDirection);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
 if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // To verify the smallest dimension and set the virtualVolumeDimension
 if ( ((VPImage *)volume)->vpGetXDimension() < ((VPVolume *)volume)->vpGetZDimension() )
	virtualYDimension = ((VPVolume *)volume)->vpGetZDimension();
 else 
	virtualYDimension = ((VPImage *)volume)->vpGetXDimension();	
 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();

 // Other orthogonal planes visualization values
 // (goal: put a line in the right position)
 int top = ((VPVolume *)volume)->vpGetTopSlice();
 int front = ((VPVolume *)volume)->vpGetFrontSlice();
 bool topFlag = true, frontFlag = true;

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 
	 p1.y = p1.y*yCorrection;

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = sliceNumber;		//(column);
		j = (int) (p1.vpGetY());//(line);
		k = (int) (p1.vpGetZ());//(depth);

		p1.vpSetX(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		if ( ((j<=top+1)&&(j>=top-1)) && topFlag ) {
			topFlag = false;
			top = line+imageYdelta;
		}
		else if ( ((k<=front+1)&&(k>=front-1)) && frontFlag ) {
			frontFlag = false;
			front = column+imageXdelta;
		}
		else
			image[line+imageYdelta][column+imageXdelta] = (unsigned int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)


  // To draw the line of the other orthogonal planes visualization
  finalLineValue = camera->vpGetViewHeight();
  for (line=0; line<finalLineValue; line++)
	image[line][front] = (unsigned int) 255;

  finalColumnValue = camera->vpGetViewWidth();
  for (column=0; column<finalColumnValue; column++)
	image[top][column] = (unsigned int) 255;

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpLineFrontSliceRender" implement the algorithm 
//				to visualize one volume slice from the front. Lines
//				indicating the position of the other two orthogonal
//				planes are draw in the image.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image);
// Return.....: -

void VPSlice::vpLineFrontSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 imageXdelta=0, imageYdelta=0, virtualYDimension=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0, yCorrection=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, vectorH, vectorV, deltaX, deltaY, up;
 VPPoint3D p1, minPlaneProjection; 

 // Set pointer to the volume object
 VPGraphicObj *volume = v;

 // Set a pointer to the right camera
 VPCamera *camera = c;

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();

 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 // Set "h" and "v" vectors 
 up = camera->vpGetUp();
 vectorH = projectionDirection.vpCrossProduct(up);
 vectorV = projectionDirection.vpCrossProduct(vectorH);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
  if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // To verify the smallest dimension and set the virtualVolumeDimension
 if ( ((VPImage *)volume)->vpGetXDimension() < ((VPVolume *)volume)->vpGetZDimension() )
	virtualYDimension = ((VPVolume *)volume)->vpGetZDimension();
 else 
	virtualYDimension = ((VPImage *)volume)->vpGetXDimension();	
 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 // Window/Viewport ratio
 ratioX = winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();

 // Other orthogonal planes visualization values
 // (goal: put a line in the right position)
 int top = ((VPVolume *)volume)->vpGetTopSlice();
 int side = ((VPVolume *)volume)->vpGetSideSlice();
 bool topFlag = true, sideFlag = true;

  // Scan view line for the ray tracing 
  for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 
	 p1.y = p1.y*yCorrection;

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = (int) (p1.vpGetX());//(column);
		j = (int) (p1.vpGetY());//(line);
		k = sliceNumber;		//(depth);

		p1.vpSetZ(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		if ( ((j<=top+1)&&(j>=top-1)) && topFlag ) {
			topFlag = false;
			top = line+imageYdelta;
		}
		else if ( ((i<=side+1)&&(i>=side-1)) && sideFlag ) {
			sideFlag = false;
			side = column+imageXdelta;
		}
		else
			image[line+imageYdelta][column+imageXdelta] = (unsigned int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)


  // To draw the line of the other orthogonal planes visualization
  finalLineValue = camera->vpGetViewHeight();
  for (line=0; line<finalLineValue; line++)
	image[line][side] = (unsigned int) 255;

  finalColumnValue = camera->vpGetViewWidth();
  for (column=0; column<finalColumnValue; column++)
	image[top][column] = (unsigned int) 255;

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpTopInnerStructRender" implement the 
//				algorithm to visualize one volume slice from the 
//				top. Point indicating the position of the end of 
//				S-Band and end of T-Band (used for setting the 
//				inner structures parameters visualization) are draw 
//				in the image.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image with colors);
// Return.....: -

void VPSlice::vpTopInnerStructRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int ***image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0,
	 imageXdelta=0, imageYdelta=0, whiteLine=0, whiteColumn=0, cont=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0, auxFloat=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, scanDirection, lineScan, vectorH, vectorV, 
			deltaX, deltaY, up(0,1,0), auxUp;
 VPPoint3D p1, minPlaneProjection, auxTarget, auxPoint, nextPoint, nextLinePoint; 
 
 VPGraphicObj *volume = v;  // Set pointer to the volume object

 VPPoint3D volumeCameraLocation = ((VPVolume *)volume)->vpGetCameraLocationForInnerStructure();
 
 VPCamera *camera = c;  // Set a pointer to the right camera

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // Set scan window direction and line scan (for SBand and TBand lines processing)
 auxTarget = target;
 scanDirection.vpSetVector3D(auxTarget - volumeCameraLocation);
 scanDirection.vpNormalize();
 lineScan = scanDirection.vpCrossProduct(up);
 scanDirection.y = lineScan.y = 0;

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 auxUp = camera->vpGetUp();
 vectorH = auxUp.vpCrossProduct(projectionDirection);  // Set "h" vector
 vectorV = vectorH.vpCrossProduct(projectionDirection);  // Set "v" vector

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
 if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();

 
 //////////////////////////////////////////////////////////////
 // Inner structures visualization parameters
 // Goal: exhibition of two points (end of S-Band and T-Band), 
 //       a line that join them, and a line that identify
 //       the ROI (where data will be sampled)

 // Necessary variables to set the S-Band and T-Band position in the image
 VPPoint3D SBand, TBand, center = ((VPVolume *)volume)->vpGetCenterFocalPoint();  
 bool findPoint;
 int aux=0, arrayDimension, edgeComparison,
	 xDimension = ((VPVolume *)volume)->vpGetXDimension(),
	 zDimension = ((VPVolume *)volume)->vpGetZDimension();
 int edge=0, SBandSteps=0, TBandSteps=0;

 // To set arrayDimension (slice diagonal)
  arrayDimension = (int) ( sqrt( (xDimension*xDimension) + (zDimension*zDimension) ) / 2.0 + 1.0 );

 // round value
 SBand.x = (int) (center.x+0.5);
 SBand.y = (int) (center.y+0.5);
 SBand.z = ((VPVolume *)volume)->vpGetRayCastingSBand();
 TBand.x = (int) (center.x+0.5);
 TBand.y = (int) (center.y+0.5);
 TBand.z = ((VPVolume *)volume)->vpGetRayCastingTBand();

 // Process the steps between the edge and SBand and between
 // SBand and TBand (using the "middle slice")
 edge = 0;
 while (((VPVolume *)volume)->vpGetValue(SBand.x,SBand.y,edge) <= 40) 
	 edge++;
 SBandSteps = SBand.z - edge;
 TBandSteps = TBand.z - SBand.z;		

 //////////////////////////////////////////////////
 // First: find the lineSBand and lineTBand points
 numTotalPoints=0;
 if (lineSBand != NULL) {
	delete [] lineSBand;
	delete [] lineTBand;
 }
 lineSBand = new VPPoint2D [sizeof(VPPoint2D) * arrayDimension];
 lineTBand = new VPPoint2D [sizeof(VPPoint2D) * arrayDimension];

 // Find object edge at "SBand column" 
 edge = 0;
 edgeComparison = 0.8 * zDimension; // 80%
 while ( (((VPVolume *)volume)->vpGetValue(SBand.x,sliceNumber,edge) <= 40) && (edge<=edgeComparison) )
	edge++;
 if ( edge < edgeComparison )
	edgeComparison = 1;
 else
	edgeComparison = 0;
 
 // For the case of a very dark slice (top or bottom slice)
 if ( edgeComparison )
 { 
	 // New SBand and TBand in accordance with scan direction
	 nextPoint = volumeCameraLocation;
	 nextPoint.y = sliceNumber;
	 vpFindPointInsideAxialSlice(nextPoint, scanDirection, xDimension, zDimension);
	 while ( ((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y,nextPoint.z) <= 40 )
		nextPoint = nextPoint + scanDirection;
	 
	 SBand = nextPoint;

	 for (i=0; i<SBandSteps; i++)
		 SBand = SBand + scanDirection;
		
	 TBand = SBand;

	 for (i=0; i<TBandSteps; i++)
		 TBand = TBand + scanDirection;
		
	 // round value
	 SBand.x = (int) (SBand.x+0.5);
	 SBand.y = (int) (SBand.y+0.5);
	 SBand.z = (int) (SBand.z+0.5);
	 TBand.x = (int) (TBand.x+0.5);
	 TBand.y = (int) (TBand.y+0.5);
	 TBand.z = (int) (TBand.z+0.5);

	 // lineScan to find the positions at scanDirection
	 // (from the center to left, and from the center to right)
	 lineSBand[numTotalPoints].x = endOfSBand.x = SBand.x;
	 lineSBand[numTotalPoints].y = endOfSBand.y = SBand.z;
	 lineTBand[numTotalPoints].x = endOfTBand.x = TBand.x;			
	 lineTBand[numTotalPoints].y = endOfTBand.y = TBand.z;
	 findPoint = true;
	 numTotalPoints++;

	 nextLinePoint = volumeCameraLocation;
	 nextLinePoint.y = sliceNumber;

	 // First, process "right" points
	 while (findPoint) {
		nextLinePoint = nextLinePoint + lineScan + lineScan; // next point after SBand in line scan
		nextPoint = nextLinePoint;
		vpFindPointInsideAxialSlice(nextPoint, scanDirection, xDimension, zDimension);
		edge = 0;
		edgeComparison = 0.8 * zDimension; // 80%
		while ( (((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y,nextPoint.z) <= 40) && (edge<=edgeComparison) ) 
		{
			nextPoint = nextPoint + scanDirection;
			edge++;
			if ( (nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) )
				break;
		}
		if ((nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension))
				edgeComparison = 0;
		else	if ( edge <= edgeComparison )
					edgeComparison = 1;
				else 
					edgeComparison = 0;

		if ( edgeComparison ) { // if the column "hit the volume"
			// nextPoint is 3D, but y is the selected slice 
			// lineSBand is 2D; since y is always the same, x=nextPoint.x and y=nextPoint.z (the point in the "slice plane")
			lineSBand[numTotalPoints].x = nextPoint.x + (SBandSteps * scanDirection.x);
			lineSBand[numTotalPoints].y = nextPoint.z + (SBandSteps * scanDirection.z);	
			lineTBand[numTotalPoints].x = lineSBand[numTotalPoints].x + (TBandSteps * scanDirection.x);
			lineTBand[numTotalPoints].y = lineSBand[numTotalPoints].y + (TBandSteps * scanDirection.z);	
			
			numTotalPoints++;
		}
		else { // the column doesn't "hit the volume"
			findPoint = false;	
			aux = numTotalPoints;
		}
	 } // while
	 findPoint = true;
	 nextLinePoint = volumeCameraLocation;
	 nextLinePoint.y = sliceNumber;
	 nextLinePoint = nextLinePoint - lineScan - lineScan; // "next point" before SBand in line scan
	 nextPoint = nextLinePoint;
	 vpFindPointInsideAxialSlice(nextPoint, scanDirection, xDimension, zDimension);

	 // Second, process "left" points
	 while (findPoint) {
		edge = 0;
		edgeComparison = 0.8 * zDimension; // 80%

		while ( (((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y,nextPoint.z) <= 40) && (edge<=edgeComparison) ) 
		{
			nextPoint = nextPoint + scanDirection;
			edge++;
			if ( (nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) )
				break;
		}
		if ((nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension))
				edgeComparison = 0;
		else	if ( edge <= edgeComparison )
					edgeComparison = 1;
				else 
					edgeComparison = 0;

		if ( edgeComparison ) { // the column "hit the volume"

			lineSBand[numTotalPoints].x = nextPoint.x + (SBandSteps * scanDirection.x);
			lineSBand[numTotalPoints].y = nextPoint.z + (SBandSteps * scanDirection.z);	
			lineTBand[numTotalPoints].x = lineSBand[numTotalPoints].x + (TBandSteps * scanDirection.x);
			lineTBand[numTotalPoints].y = lineSBand[numTotalPoints].y + (TBandSteps * scanDirection.z);	
			nextLinePoint = nextLinePoint - lineScan - lineScan; // "next point" before SBand in line scan
		    nextPoint = nextLinePoint;
			vpFindPointInsideAxialSlice(nextPoint, scanDirection, xDimension, zDimension);
			numTotalPoints++;
		}
		else // the column doesn't "hit the volume"
			findPoint = false;	
	 }
 }

 vpOrder(arrayDimension, numTotalPoints, aux);

 //////////////////////////////////////
 // Scan view line for the ray casting 

 for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = (int) (p1.x); //(column);
		j = sliceNumber;  //(line);
		k = (int) (p1.z); //(depth);

		p1.vpSetY(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		//i = (int) (p1.x+0.5);

		image[line+imageYdelta][column+imageXdelta][red] = (unsigned short int) voxelColor;
		image[line+imageYdelta][column+imageXdelta][green] = (unsigned short int) voxelColor;
		image[line+imageYdelta][column+imageXdelta][blue] = (unsigned short int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

 // Mapping endOfSBand and endOfTBand from the window to the viewport
 auxFloat = endOfSBand.x;
 endOfSBand.x = (finalLineValue*(endOfSBand.y-zDimension)) / zDimension + finalLineValue;
 endOfSBand.y = ( (finalColumnValue*auxFloat) / xDimension );
 endOfSBand.x += imageYdelta;
 endOfSBand.y += imageXdelta;
 auxFloat = endOfTBand.x;
 endOfTBand.x = (finalLineValue*(endOfTBand.y-zDimension)) / zDimension + finalLineValue;
 endOfTBand.y = ( (finalColumnValue*auxFloat) / xDimension );
 endOfTBand.x += imageYdelta;
 endOfTBand.y += imageXdelta;

 // Mapping lineTBand and lineSBand points from the window to the viewport
 for (cont=0; cont<numTotalPoints; cont++) {
	auxFloat = lineTBand[cont].x;
	lineTBand[cont].x = (finalLineValue*(lineTBand[cont].y-zDimension)) / zDimension + finalLineValue;
	lineTBand[cont].y = ( (finalColumnValue*auxFloat) / xDimension );
	lineTBand[cont].x += imageYdelta;
	lineTBand[cont].y += imageXdelta;

	auxFloat = lineSBand[cont].x;
	lineSBand[cont].x = (finalLineValue*(lineSBand[cont].y-zDimension)) / zDimension + finalLineValue;
	lineSBand[cont].y = ( (finalColumnValue*auxFloat) / xDimension );
	lineSBand[cont].x += imageYdelta;
	lineSBand[cont].y += imageXdelta;
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSideInnerStructRender" implement the 
//				algorithm to visualize one volume slice from the 
//				side. Point indicating the position of the end of 
//				S-Band and end of T-Band (used for setting the 
//				inner structures parameters visualization) are draw 
//				in the image.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image with colors);
// Return.....: -

void VPSlice::vpSideInnerStructRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int ***image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0, virtualYDimension=0,
	 imageXdelta=0, imageYdelta=0, whiteLine=0, whiteColumn=0, cont=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0, auxFloat=0, yCorrection=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, scanDirection, lineScan, vectorH, vectorV, 
			deltaX, deltaY, xAxis(1,0,0), up;
 VPPoint3D p1, minPlaneProjection, auxTarget, auxPoint, nextPoint, nextLinePoint; 
 
 VPGraphicObj *volume = v;  // Set pointer to the volume object

 VPPoint3D volumeCameraLocation = ((VPVolume *)volume)->vpGetCameraLocationForInnerStructure();
 
 VPCamera *camera = c;  // Set a pointer to the right camera

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // Set scan window direction and line scan (for SBand and TBand lines processing)
 auxTarget = target;
 scanDirection.vpSetVector3D(auxTarget - volumeCameraLocation);
 scanDirection.vpNormalize();
 lineScan = scanDirection.vpCrossProduct(xAxis);
 scanDirection.x = lineScan.x = 0;

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 up = camera->vpGetUp();
 vectorH = up.vpCrossProduct(projectionDirection);
 vectorV = vectorH.vpCrossProduct(projectionDirection);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
 if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // To verify the smallest dimension and set the virtualVolumeDimension
 if ( ((VPImage *)volume)->vpGetXDimension() < ((VPVolume *)volume)->vpGetZDimension() )
	virtualYDimension = ((VPVolume *)volume)->vpGetZDimension();
 else 
	virtualYDimension = ((VPImage *)volume)->vpGetXDimension();	
 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();
 
 //////////////////////////////////////////////////////////////
 // Inner structures visualization parameters
 // Goal: exhibition of two points (end of S-Band and T-Band), 
 //       a line that join them, and a line that identify
 //       the ROI (where data will be sampled)

 // Necessary variables to set the S-Band and T-Band position in the image
 VPPoint3D SBand, TBand, center = ((VPVolume *)volume)->vpGetCenterFocalPoint();  
 bool findPoint;
 int aux=0, arrayDimension, edgeComparison, matrixValue,
	 yDimension = ((VPVolume *)volume)->vpGetYDimension(),
	 zDimension = ((VPVolume *)volume)->vpGetZDimension();
 int edge=0, SBandSteps=0, TBandSteps=0;

 // To set arrayDimension (slice diagonal)
  arrayDimension = (int) ( sqrt( (yDimension*yDimension) + (zDimension*zDimension) ) / 2.0 + 1.0 );

   // round value
 SBand.x = (int) (center.x+0.5);
 SBand.y = (int) (center.y+0.5);
 SBand.z = ((VPVolume *)volume)->vpGetRayCastingSBand();
 TBand.x = (int) (center.x+0.5);
 TBand.y = (int) (center.y+0.5);
 TBand.z = ((VPVolume *)volume)->vpGetRayCastingTBand();

 // Process the steps between the edge and SBand and between
 // SBand and TBand (using the "middle slice")
 edge = 0;
 while (((VPVolume *)volume)->vpGetValue(SBand.x,SBand.y,edge) <= 40) 
	 edge++;
 SBandSteps = SBand.z - edge;
 TBandSteps = TBand.z - SBand.z;

 //////////////////////////////////////////////////
 // First: find the lineSBand and lineTBand points
 numTotalPoints=0;
 if (lineSBand != NULL) {
	delete [] lineSBand;
	delete [] lineTBand;
 }
 lineSBand = new VPPoint2D [sizeof(VPPoint2D) * arrayDimension];
 lineTBand = new VPPoint2D [sizeof(VPPoint2D) * arrayDimension];

 // Find object edge at "SBand column"
 edge = 0;
 edgeComparison = 0.8 * zDimension; // 80%
 while ( (((VPVolume *)volume)->vpGetValue(sliceNumber,SBand.y,edge) <= 40) && (edge<=edgeComparison) )
	edge++;
 if ( edge < edgeComparison )
	edgeComparison = 1;
 else
	edgeComparison = 0;
 
 // For the case of a very dark slice (top or bottom slice)
 if ( edgeComparison )
 { 
	 // New SBand and TBand in accordance with scan direction
	 nextPoint = volumeCameraLocation;
	 nextPoint.x = sliceNumber;
	 vpFindPntInsideSagittalSlice(nextPoint, scanDirection, virtualYDimension, zDimension); // @@ YDimension
	 while ( ((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y*yCorrection,nextPoint.z) <= 40 ) // @@ Tirar *yCorrection
		nextPoint = nextPoint + scanDirection;
	 
	 SBand = nextPoint;

	 for (i=0; i<SBandSteps; i++)
		 SBand = SBand + scanDirection;
		
	 TBand = SBand;

	 for (i=0; i<TBandSteps; i++)
		 TBand = TBand + scanDirection;
		
	 // round value
	 SBand.x = (int) (SBand.x+0.5);
	 SBand.y = (int) (SBand.y+0.5);
	 SBand.z = (int) (SBand.z+0.5);
	 TBand.x = (int) (TBand.x+0.5);
	 TBand.y = (int) (TBand.y+0.5);
	 TBand.z = (int) (TBand.z+0.5);

	 // lineScan to find the positions at scanDirection
	 // (from the center to left, and from the center to right)
	 lineSBand[numTotalPoints].x = endOfSBand.x = SBand.z;
	 lineSBand[numTotalPoints].y = endOfSBand.y = SBand.y*yCorrection; // @@ tirar *yCorrection
	 lineTBand[numTotalPoints].x = endOfTBand.x = TBand.z;			
	 lineTBand[numTotalPoints].y = endOfTBand.y = TBand.y*yCorrection; // @@ tirar *yCorrection
	 findPoint = true;
	 numTotalPoints++;

	 nextLinePoint = volumeCameraLocation;
	 nextLinePoint.x = sliceNumber;

	 // First, process "right" points
	 while (findPoint) {
		nextLinePoint = nextLinePoint + lineScan + lineScan; // next point after SBand in line scan
		nextPoint = nextLinePoint;
		edge = 0;
		edgeComparison = 0.8 * zDimension; // 80%
		while ( (nextPoint.y<0) || (nextPoint.y>=virtualYDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) )
		{
			nextPoint = nextPoint + scanDirection;
			edge++; // to verify if the point is out of the volume (edge must be <= edgeComparison)
			if (edge > edgeComparison)
				break;
		}
		if (edge <= edgeComparison)
		{
			edge = 0;
			matrixValue = ((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y*yCorrection,nextPoint.z);
		}
		else
			matrixValue = 0;
		while ( (matrixValue <= 40) && (edge<=edgeComparison) )  // @@ Tirar *yCorrection
		{
			nextPoint = nextPoint + scanDirection;
			edge++;
			if ( (nextPoint.y<0) || (nextPoint.y>=virtualYDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) ) // @@ yDimension 
				break;
			matrixValue = ((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y*yCorrection,nextPoint.z);
		}
		if ((nextPoint.y<0) || (nextPoint.y>=virtualYDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension)) // @@ yDimension 
				edgeComparison = 0;
		else	if ( edge <= edgeComparison )
					edgeComparison = 1;
				else 
					edgeComparison = 0;

		if ( edgeComparison ) { // if the column "hit the volume"
			// nextPoint is 3D, but x is the selected slice 
			// lineSBand is 2D; since x is always the same, x=nextPoint.z and y=nextPoint.y (the point in the "slice plane")
			lineSBand[numTotalPoints].x = nextPoint.z + (SBandSteps * scanDirection.z);
			lineSBand[numTotalPoints].y = nextPoint.y*yCorrection + (SBandSteps * scanDirection.y);	// @@ Tirar *yCorrection
			lineTBand[numTotalPoints].x = lineSBand[numTotalPoints].x + (TBandSteps * scanDirection.z);
			lineTBand[numTotalPoints].y = lineSBand[numTotalPoints].y + (TBandSteps * scanDirection.y);	
			
			numTotalPoints++;
		}
		else { // the column doesn't "hit the volume"
			findPoint = false;	
			aux = numTotalPoints;
		}
	 } // while
	 findPoint = true;
	 nextLinePoint = volumeCameraLocation;
	 nextLinePoint.x = sliceNumber;
	 nextLinePoint = nextLinePoint - lineScan - lineScan; // "next point" before SBand in line scan
	 nextPoint = nextLinePoint;
	 vpFindPntInsideSagittalSlice(nextPoint, scanDirection, virtualYDimension, zDimension); //// @@ yDimension 

	 // Second, process "left" points
	 while (findPoint) {
		edge = 0;
		edgeComparison = 0.8 * zDimension; // 80%

		while ( (((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y*yCorrection,nextPoint.z) <= 40) && (edge<=edgeComparison) ) 
		{
			nextPoint = nextPoint + scanDirection;
			edge++;
			if ( (nextPoint.y<0) || (nextPoint.y>=virtualYDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) )
				break;
		}
		if ((nextPoint.y<0) || (nextPoint.y>=virtualYDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension))
				edgeComparison = 0;
		else	if ( edge <= edgeComparison )
					edgeComparison = 1;
				else 
					edgeComparison = 0;

		if ( edgeComparison ) { // the column "hit the volume"
			lineSBand[numTotalPoints].x = nextPoint.z + (SBandSteps * scanDirection.z);
			lineSBand[numTotalPoints].y = nextPoint.y*yCorrection + (SBandSteps * scanDirection.y);	
			lineTBand[numTotalPoints].x = lineSBand[numTotalPoints].x + (TBandSteps * scanDirection.z);
			lineTBand[numTotalPoints].y = lineSBand[numTotalPoints].y + (TBandSteps * scanDirection.y);	
			nextLinePoint = nextLinePoint - lineScan - lineScan; // "next point" before SBand in line scan
		    nextPoint = nextLinePoint;
			numTotalPoints++;

			// Guarantee that the next point is inside the volume...
			edge = 0;
			edgeComparison = 0.8 * zDimension; // 80%
			while ( (nextPoint.y<0) || (nextPoint.y>=virtualYDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) )	{
				nextPoint = nextPoint + scanDirection;
				edge++; 
				if (edge > edgeComparison)// to verify if the point is out of the volume (edge must be <= edgeComparison)
					break;
			}
			if (edge > edgeComparison)
				findPoint = false;
		}
		else // the column doesn't "hit the volume"
			findPoint = false;	
	 }
 }

 vpOrder(arrayDimension, numTotalPoints, aux);

 //////////////////////////////////////
 // Scan view line for the ray casting 

 for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 
	 p1.y = p1.y*yCorrection;

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = sliceNumber;		//(column);
		j = (int) (p1.vpGetY());//(line);
		k = (int) (p1.vpGetZ());//(depth);

		p1.vpSetX(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		image[line+imageYdelta][column+imageXdelta][red] = (unsigned short int) voxelColor;
		image[line+imageYdelta][column+imageXdelta][green] = (unsigned short int) voxelColor;
		image[line+imageYdelta][column+imageXdelta][blue] = (unsigned short int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

 // Mapping endOfSBand and endOfTBand from the window to the viewport
 auxFloat = endOfSBand.x;
 endOfSBand.x = ( (-finalLineValue)*endOfSBand.y ) / yDimension + finalLineValue;
 endOfSBand.y = ( (finalColumnValue*(auxFloat-zDimension)) / -zDimension );
 endOfSBand.x += imageYdelta;
 endOfSBand.y += imageXdelta;
 auxFloat = endOfTBand.x;
 endOfTBand.x = ( (-finalLineValue)*endOfTBand.y ) / yDimension + finalLineValue;
 endOfTBand.y = ( (finalColumnValue*(auxFloat-zDimension)) / -zDimension );
 endOfTBand.x += imageYdelta;
 endOfTBand.y += imageXdelta;



 // Mapping lineTBand and lineSBand points from the window to the viewport
 for (cont=0; cont<numTotalPoints; cont++) {
	auxFloat = lineTBand[cont].x;
	lineTBand[cont].x = ( (-finalLineValue)*lineTBand[cont].y ) / yDimension + finalLineValue;
	lineTBand[cont].y = ( (finalColumnValue*(auxFloat-zDimension)) / -zDimension );
	lineTBand[cont].x += imageYdelta;
	lineTBand[cont].y += imageXdelta;

	auxFloat = lineSBand[cont].x;
	lineSBand[cont].x = ( (-finalLineValue)*lineSBand[cont].y ) / yDimension + finalLineValue;
	lineSBand[cont].y = ( (finalColumnValue*(auxFloat-zDimension)) / -zDimension );
	lineSBand[cont].x += imageYdelta;
	lineSBand[cont].y += imageXdelta;
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFrontInnerStructRender" implement the 
//				algorithm to visualize one volume slice from the 
//				front. Point indicating the position of the end of 
//				S-Band and end of T-Band (used for setting the 
//				inner structures parameters visualization) are draw 
//				in the image.
// Parameters.: VPCamera *c (active camera), VPGraphicObj *v, 
//				int sliceNumber, unsigned int image[][256] (pointer 
//				to the image with colors);
// Return.....: -

void VPSlice::vpFrontInnerStructRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int ***image) {
 int line=0, column=0, i=0, j=0, k=0, initialLineValue=0, 
	 finalLineValue=0, initialColumnValue=0, finalColumnValue=0, virtualYDimension=0,
	 imageXdelta=0, imageYdelta=0, whiteLine=0, whiteColumn=0, cont=0;
 float voxelColor=0, ratioX=0, ratioY=0, deltaZ=0, auxRatio=0, auxFloat=0, yCorrection=0;

 VPPoint2D winTopRightValue;
 VPVector3D projectionDirection, scanDirection, lineScan, vectorH, vectorV, 
			deltaX, deltaY, zAxis(0,0,1), up;
 VPPoint3D p1, minPlaneProjection, auxTarget, auxPoint, nextPoint, nextLinePoint; 
 
 VPGraphicObj *volume = v;  // Set pointer to the volume object

 VPPoint3D volumeCameraLocation = ((VPVolume *)volume)->vpGetCameraLocationForInnerStructure();
 
 VPCamera *camera = c;  // Set a pointer to the right camera

 VPPoint3D location = camera->vpGetLocation();
 VPPoint3D target = camera->vpGetTarget();
				
 // Set projection direction by the vector
 projectionDirection.vpSetVector3D(target - location);
 projectionDirection.vpNormalize();

 // Set scan window direction and line scan (for SBand and TBand lines processing)
 auxTarget = target;
 scanDirection.vpSetVector3D(auxTarget - volumeCameraLocation);
 scanDirection.vpNormalize();
 lineScan = scanDirection.vpCrossProduct(zAxis);
 scanDirection.z = lineScan.z = 0;

 // winTopRightValue has the projection plane size (x,y)
 winTopRightValue = camera->vpGetWinTopRight();

 up = camera->vpGetUp();
 vectorH = up.vpCrossProduct(projectionDirection);
 vectorV = vectorH.vpCrossProduct(projectionDirection);

 // Set scan window corner (where the scan begin)
 minPlaneProjection = (location - vectorH*(winTopRightValue.vpGetX()/2));
 minPlaneProjection = (minPlaneProjection - vectorV*(winTopRightValue.vpGetY()/2));

 // Set variables to determine the end of the view scan
 finalLineValue = camera->vpGetViewHeight();
 finalColumnValue = camera->vpGetViewWidth();

 // To verify the ratio between the window and the view
 // greater window dimension / view dimension (where viewx=viewy)
 if (winTopRightValue.vpGetX() > winTopRightValue.vpGetY()) {
	auxRatio = winTopRightValue.vpGetX() / finalColumnValue;
	finalLineValue = winTopRightValue.vpGetY() / auxRatio;
	imageYdelta = (camera->vpGetViewHeight() - finalLineValue) / 2;
 }
 else {
	auxRatio = winTopRightValue.vpGetY() / finalLineValue;
	finalColumnValue = winTopRightValue.vpGetX() / auxRatio;
 	imageXdelta = (camera->vpGetViewWidth() - finalColumnValue) / 2;
 }

 // To verify the smallest dimension and set the virtualVolumeDimension
 if ( ((VPImage *)volume)->vpGetXDimension() < ((VPVolume *)volume)->vpGetZDimension() )
	virtualYDimension = ((VPVolume *)volume)->vpGetZDimension();
 else 
	virtualYDimension = ((VPImage *)volume)->vpGetXDimension();	
 yCorrection = ((float) ((VPImage *)volume)->vpGetYDimension()) / ((float) virtualYDimension);

 // Window/Viewport ratio
 ratioX =  winTopRightValue.vpGetX()/finalColumnValue;
 ratioY = winTopRightValue.vpGetY()/finalLineValue;

 // Line, column and sample step 
 deltaX = vectorH * ratioX;
 deltaY = vectorV * ratioY;
 deltaZ = projectionDirection.vpModule();
 
 //////////////////////////////////////////////////////////////
 // Inner structures visualization parameters
 // Goal: exhibition of two points (end of S-Band and T-Band), 
 //       a line that join them, and a line that identify
 //       the ROI (where data will be sampled)

 // Necessary variables to set the S-Band and T-Band position in the image
 VPPoint3D SBand, TBand, center = ((VPVolume *)volume)->vpGetCenterFocalPoint();  
 bool findPoint;
 int aux=0, arrayDimension, edgeComparison,
	 xDimension = ((VPVolume *)volume)->vpGetXDimension(),
	 yDimension = ((VPVolume *)volume)->vpGetYDimension();
 int edge=0, SBandSteps=0, TBandSteps=0;

 // To set arrayDimension (slice diagonal)
  arrayDimension = (int) ( sqrt( (xDimension*xDimension) + (yDimension*yDimension) ) / 2.0 + 1.0 );

 // round value
 SBand.x = (int) (center.x+0.5);
 SBand.y = (int) (center.y+0.5);
 SBand.z = ((VPVolume *)volume)->vpGetRayCastingSBand();
 TBand.x = (int) (center.x+0.5);
 TBand.y = (int) (center.y+0.5);
 TBand.z = ((VPVolume *)volume)->vpGetRayCastingTBand();

 // Process the steps between the edge and SBand and between
 // SBand and TBand (using the "middle slice")
 edge = 0;
 while (((VPVolume *)volume)->vpGetValue(SBand.x,SBand.y,edge) <= 40) 
	 edge++;
 SBandSteps = SBand.z - edge;
 TBandSteps = TBand.z - SBand.z;

 //////////////////////////////////////////////////
 // First: find the lineSBand and lineTBand points
 numTotalPoints=0;
 if (lineSBand != NULL) {
	delete [] lineSBand;
	delete [] lineTBand;
 }
 lineSBand = new VPPoint2D [sizeof(VPPoint2D) * arrayDimension];
 lineTBand = new VPPoint2D [sizeof(VPPoint2D) * arrayDimension];

 // Find object edge at "SBand column"
 edge = 0;
 edgeComparison = 0.8 * yDimension; // 80%
 while ( (((VPVolume *)volume)->vpGetValue(SBand.x,edge,sliceNumber) <= 40) && (edge<=edgeComparison) )
	edge++;
 if ( edge < edgeComparison )
	edgeComparison = 1;
 else
	edgeComparison = 0;
 
 // For the case of a very dark slice (top or bottom slice)
 if ( edgeComparison )
 { 
	 // New SBand and TBand in accordance with scan direction
	 nextPoint = volumeCameraLocation;
	 nextPoint.z = sliceNumber;
	 vpFindPntInsideFrontalSlice(nextPoint, scanDirection, xDimension, yDimension);
	 if ( ((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y,nextPoint.z) >= 10 )
	 { // if the point is already inside the volume, it's necessary to find "the limit"
		 while ( (nextPoint.x>1) && (nextPoint.x<xDimension-1) && (nextPoint.y>1) && (nextPoint.y<yDimension-1) )
			nextPoint = nextPoint - scanDirection;
	 }
	 while ( ((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y,nextPoint.z) <= 40 )
		nextPoint = nextPoint + scanDirection;
	 
	 SBand = nextPoint;

	 for (i=0; i<SBandSteps; i++)
		 SBand = SBand + scanDirection;
		
	 TBand = SBand;

	 for (i=0; i<TBandSteps; i++)
		 TBand = TBand + scanDirection;
		
	 // round value
	 SBand.x = (int) (SBand.x+0.5);
	 SBand.y = (int) (SBand.y+0.5);
	 SBand.z = (int) (SBand.z+0.5);
	 TBand.x = (int) (TBand.x+0.5);
	 TBand.y = (int) (TBand.y+0.5);
	 TBand.z = (int) (TBand.z+0.5);

	 // lineScan to find the positions at scanDirection
	 // (from the center to left, and from the center to right)
	 lineSBand[numTotalPoints].x = endOfSBand.x = SBand.x;
	 lineSBand[numTotalPoints].y = endOfSBand.y = SBand.y;
	 lineTBand[numTotalPoints].x = endOfTBand.x = TBand.x;			
	 lineTBand[numTotalPoints].y = endOfTBand.y = TBand.y;
	 findPoint = true;
	 numTotalPoints++;

	 nextLinePoint = volumeCameraLocation;
	 nextLinePoint.z = sliceNumber;

	 // First, process "right" points
	 while (findPoint) {
		nextLinePoint = nextLinePoint + lineScan + lineScan; // next point after SBand in line scan
		nextPoint = nextLinePoint;
		vpFindPntInsideFrontalSlice(nextPoint, scanDirection, xDimension, yDimension);
		
		// if the point is already inside the volume, it's necessary to find "the limit"
		while ( (nextPoint.x>1) && (nextPoint.x<xDimension-1) && (nextPoint.y>1) && (nextPoint.y<yDimension-1) )
			nextPoint = nextPoint - scanDirection;

		edge = 0;
		edgeComparison = 0.8 * yDimension; // 80%
		while ( (((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y,nextPoint.z) <= 40) && (edge<=edgeComparison) ) 
		{
			nextPoint = nextPoint + scanDirection;
			edge++;
			if ( (nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.y<0) || (nextPoint.y>=yDimension) )
				break;
		}
		if ((nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.y<0) || (nextPoint.y>=yDimension))
				edgeComparison = 0;
		else	if ( edge <= edgeComparison )
					edgeComparison = 1;
				else 
					edgeComparison = 0;

		if ( edgeComparison ) { // if the column "hit the volume"
			// nextPoint is 3D, but z is the selected slice 
			// lineSBand is 2D; since z is always the same, x=nextPoint.x and y=nextPoint.y (the point in the "slice plane")
			lineSBand[numTotalPoints].x = nextPoint.x + (SBandSteps * scanDirection.x);
			lineSBand[numTotalPoints].y = nextPoint.y + (SBandSteps * scanDirection.y);	
			lineTBand[numTotalPoints].x = lineSBand[numTotalPoints].x + (TBandSteps * scanDirection.x);
			lineTBand[numTotalPoints].y = lineSBand[numTotalPoints].y + (TBandSteps * scanDirection.y);	
			
			numTotalPoints++;
		}
		else { // the column doesn't "hit the volume"
			findPoint = false;	
			aux = numTotalPoints;
		}
	 } // while
	 findPoint = true;
	 nextLinePoint = volumeCameraLocation;
	 nextLinePoint.z = sliceNumber;
	 nextLinePoint = nextLinePoint - lineScan - lineScan; // "next point" before SBand in line scan
	 nextPoint = nextLinePoint;
	 vpFindPntInsideFrontalSlice(nextPoint, scanDirection, xDimension, yDimension);

	 // Second, process "left" points
	 while (findPoint) {
		edge = 0;
		edgeComparison = 0.8 * yDimension; // 80%

		// if the point is already inside the volume, it's necessary to find "the limit"
		while ( (nextPoint.x>1) && (nextPoint.x<xDimension-1) && (nextPoint.y>1) && (nextPoint.y<yDimension-1) )
			nextPoint = nextPoint - scanDirection;
		
		while ( (((VPVolume *)volume)->vpGetValue(nextPoint.x,nextPoint.y,nextPoint.z) <= 40) && (edge<=edgeComparison) ) 
		{
			nextPoint = nextPoint + scanDirection;
			edge++;
			if ( (nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.y<0) || (nextPoint.y>=yDimension) )
				break;
		}
		if ((nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.y<0) || (nextPoint.y>=yDimension))
				edgeComparison = 0;
		else	if ( edge <= edgeComparison )
					edgeComparison = 1;
				else 
					edgeComparison = 0;

		if ( edgeComparison ) { // the column "hit the volume"

			lineSBand[numTotalPoints].x = nextPoint.x + (SBandSteps * scanDirection.x);
			lineSBand[numTotalPoints].y = nextPoint.y + (SBandSteps * scanDirection.y);	
			lineTBand[numTotalPoints].x = lineSBand[numTotalPoints].x + (TBandSteps * scanDirection.x);
			lineTBand[numTotalPoints].y = lineSBand[numTotalPoints].y + (TBandSteps * scanDirection.y);	
			nextLinePoint = nextLinePoint - lineScan - lineScan; // "next point" before SBand in line scan
		    nextPoint = nextLinePoint;
		    vpFindPntInsideFrontalSlice(nextPoint, scanDirection, xDimension, yDimension);

			// if the point is already inside the volume, it's necessary to find "the limit"
			while ( (nextPoint.x>1) && (nextPoint.x<xDimension-1) && (nextPoint.y>1) && (nextPoint.y<yDimension-1) )
				nextPoint = nextPoint - scanDirection;
			
			numTotalPoints++;
		}
		else // the column doesn't "hit the volume"
			findPoint = false;	
	 }
 }

 vpOrder(arrayDimension, numTotalPoints, aux);

 //////////////////////////////////////
 // Scan view line for the ray casting 

 for (line=0; line<finalLineValue; line++) { 

     // Plane point computation
	 p1 = minPlaneProjection;
	 p1 = p1 + deltaY * line; 
	 p1.y = p1.y*yCorrection;

	 // Scan view column
	 for (column=0; column<finalColumnValue; column++) {

		i = (int) (p1.vpGetX()); //(column);
		j = (int) (p1.vpGetY()); //(line);
		k = sliceNumber; //(depth);

		p1.vpSetZ(sliceNumber);
		voxelColor = vpTrilinearInterpolation(i,j,k,volume,p1);

		image[line+imageYdelta][column+imageXdelta][red] = (unsigned short int) voxelColor;
		image[line+imageYdelta][column+imageXdelta][green] = (unsigned short int) voxelColor;
		image[line+imageYdelta][column+imageXdelta][blue] = (unsigned short int) voxelColor;

		// Next plane point computation
		p1 = p1 + deltaX; 

	} // for (column)

 } // for (line)

 // Mapping endOfSBand and endOfTBand from the window to the viewport
 auxFloat = endOfSBand.x;
 endOfSBand.x = (finalLineValue*(endOfSBand.y-yDimension)) / yDimension + finalLineValue;
 endOfSBand.y = ( (finalColumnValue*(auxFloat-xDimension)) / -xDimension );
 endOfSBand.x += imageYdelta;
 endOfSBand.y += imageXdelta;
 auxFloat = endOfTBand.x;
 endOfTBand.x = (finalLineValue*(endOfTBand.y-yDimension)) / yDimension + finalLineValue;
 endOfTBand.y = ( (finalColumnValue*(auxFloat-xDimension)) / -xDimension );
 endOfTBand.x += imageYdelta;
 endOfTBand.y += imageXdelta;

 // Mapping lineTBand and lineSBand points from the window to the viewport
 for (cont=0; cont<numTotalPoints; cont++) {
	auxFloat = lineTBand[cont].x;
	lineTBand[cont].x = (finalLineValue*(lineTBand[cont].y-yDimension)) / yDimension + finalLineValue;
	lineTBand[cont].y = ( (finalColumnValue*(auxFloat-xDimension)) / -xDimension );
	lineTBand[cont].x += imageYdelta;
	lineTBand[cont].y += imageXdelta;

	auxFloat = lineSBand[cont].x;
	lineSBand[cont].x = (finalLineValue*(lineSBand[cont].y-yDimension)) / yDimension + finalLineValue;
	lineSBand[cont].y = ( (finalColumnValue*(auxFloat-xDimension)) / -xDimension );
	lineSBand[cont].x += imageYdelta;
	lineSBand[cont].y += imageXdelta;
 }

}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFindPointInsideAxialSlice" guarante that 
//				the point is inside a axial slice in the volume.
// Parameters.: VPPoint3D nextPoint (point inside the volume)
//				VPVector3D scanDirection (direction to find the point)
//				int xDimension (x volume dimension)
//				int zDimension (z volume dimension)
// Return.....: -

void VPSlice::vpFindPointInsideAxialSlice(VPPoint3D &nextPoint, VPVector3D scanDirection, int xDimension, int zDimension) {
	while ( (nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) )
			nextPoint = nextPoint + scanDirection;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFindPntInsideSagittalSlice" guarante that 
//				the point is inside a sagittal slice in the volume.
// Parameters.: VPPoint3D nextPoint (point inside the volume)
//				VPVector3D scanDirection (direction to find the point)
//				int yDimension (y volume dimension)
//				int zDimension (z volume dimension)
// Return.....: -

void VPSlice::vpFindPntInsideSagittalSlice(VPPoint3D &nextPoint, VPVector3D scanDirection, int yDimension, int zDimension) {
	while ( (nextPoint.y<0) || (nextPoint.y>=yDimension) || (nextPoint.z<0) || (nextPoint.z>=zDimension) )
			nextPoint = nextPoint + scanDirection;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpFindPntInsideFrontalSlice" guarante that 
//				the point is inside a frontal slice in the volume.
// Parameters.: VPPoint3D nextPoint (point inside the volume)
//				VPVector3D scanDirection (direction to find the point)
//				int xDimension (x volume dimension)
//				int yDimension (y volume dimension)
// Return.....: -

void VPSlice::vpFindPntInsideFrontalSlice(VPPoint3D &nextPoint, VPVector3D scanDirection, int xDimension, int yDimension) {
	while ( (nextPoint.x<0) || (nextPoint.x>=xDimension) || (nextPoint.y<0) || (nextPoint.y>=yDimension) )
			nextPoint = nextPoint + scanDirection;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpOrder" "put in order" the points of the
//				lineSBand and lineTBand arrays.
// Parameters.: int arraySize, to alloc a temporary array (same 
//							   size as lineTBand and lineSBand)
//				int numTotalPoints (arrays total points) 
//				int aux (middle point, separate left and right
//						 points from SBand and TBand) 
// Return.....: -

void VPSlice::vpOrder(int arraySize, int numTotalPoints, int aux)
{
 VPPoint2D *auxLine, *auxPointer; 
 int i, j;

 auxLine = new VPPoint2D [sizeof(VPPoint2D) * arraySize];

 for (i=numTotalPoints-1, j=0; i>=aux; i--, j++) {
	auxLine[j].x = lineSBand[i].x;
	auxLine[j].y = lineSBand[i].y;
 }
 auxLine[j].x = lineSBand[0].x;
 auxLine[j].y = lineSBand[0].y;
 j++;
 for (i=1; i<aux; i++, j++) {
	auxLine[j].x = lineSBand[i].x;
	auxLine[j].y = lineSBand[i].y;
 }

 auxPointer = lineSBand;
 lineSBand = auxLine;
 auxLine = auxPointer;

 for (i=numTotalPoints-1, j=0; i>=aux; i--, j++) {
	auxLine[j].x = lineTBand[i].x;
	auxLine[j].y = lineTBand[i].y;
 }
 auxLine[j].x = lineTBand[0].x;
 auxLine[j].y = lineTBand[0].y;
 j++;
 for (i=1; i<aux; i++, j++) {
	auxLine[j].x = lineTBand[i].x;
	auxLine[j].y = lineTBand[i].y;
 }

 auxPointer = lineTBand;
 lineTBand = auxLine;

 delete [] auxPointer;
}
	

///////////////////////////////////////////////////////////////////
// Description: Method "vpTrilinearInterpolation" process the voxel 
//				color (gray) using trilinear interpolation.
// Parameters.: int i, int j, int k, VPGraphicObj *volume,
//				float increment (i,j,k=volume matrix index; 
//				depth=ray position; volume=object volume that has all 
//				information about the volume; increment=increment step
//				in the ray) 
// Return.....: float color (voxel color)

float VPSlice::vpTrilinearInterpolation(int i, int j, int k,  
											 VPGraphicObj *volume, VPPoint3D p) {

 int C1, C2, C3, C4, C5, C6, C7, C8, ii, jj, kk;
 float i1, i2, i3, i4, i5, i6, i7,	// valores interpolados
	   dx, dy, dz;					// distância (delta)

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
 dx = p.vpGetX()-i;
 dy = p.vpGetY()-j;
 dz = p.vpGetZ()-k;

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


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetViewSBandPoint" returns the value of 
//				endOfSBand attribute.
// Parameters.: -
// Return.....: VPPoint2D endOfSBand (attribute value)

VPPoint2D VPSlice::vpGetViewSBandPoint() {
	return endOfSBand; // endOfSBand;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetViewTBandPoint" returns the value of 
//				endOfTBand attribute.
// Parameters.: -
// Return.....: VPPoint2D endOfTBand (attribute value)

VPPoint2D VPSlice::vpGetViewTBandPoint() {
	return endOfTBand; // endOfTBand;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLineSBand" returns the value of 
//				lineSBand attribute and how many points it has.
// Parameters.: VPPoint2D *l, int &n
// Return.....: -

void VPSlice::vpGetLineSBand(VPPoint2D **l, int &n) {
	*l = lineSBand;
	n = numTotalPoints;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetLineTBand" returns the value of 
//				lineTBand attribute and how many points it has.
// Parameters.: VPPoint2D *l, int n
// Return.....: -

void VPSlice::vpGetLineTBand(VPPoint2D **l, int &n) {
	*l = lineTBand;
	n = numTotalPoints;
}


