//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpslice.h
//  DESCRIPTION.: Contain the VPSlice class declarations. 
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: May/30/2000
//  DESCRIPTION.: Class declaration.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: January/05/2000
//  DESCRIPTION.: Parameter changes.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPSLICE_H	
#define __VPSLICE_H	


#include <vptable.h>
#include <vpscene.h>
#include <vpgraphicobj.h>




//-----------------------------------------------------------------
// S L I C E
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPSlice
// Superclass: -
// Subclasses: -
///\deprecated

class VPSlice {
	protected:
		VPPoint2D endOfSBand;
		VPPoint2D endOfTBand;
		VPPoint2D *lineSBand;
		VPPoint2D *lineTBand;
		int numTotalPoints;
	public:
		VPSlice();
		~VPSlice();
		void  vpTopSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image);
		void  vpSideSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image);
		void  vpFrontSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image);
		void  vpLineTopSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image);
		void  vpLineSideSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image);
		void  vpLineFrontSliceRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int **image);
		void  vpTopInnerStructRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int ***image);
		void  vpSideInnerStructRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int ***image);
		void  vpFrontInnerStructRender(VPCamera *c, VPGraphicObj *v, int sliceNumber, unsigned int ***image);
		void  vpFindPointInsideAxialSlice(VPPoint3D &nextPoint, VPVector3D scanDirection, int xDimension, int zDimension);
		void  vpFindPntInsideSagittalSlice(VPPoint3D &nextPoint, VPVector3D scanDirection, int yDimension, int zDimension);
		void  vpFindPntInsideFrontalSlice(VPPoint3D &nextPoint, VPVector3D scanDirection, int xDimension, int yDimension);
		void  vpOrder(int arraySize, int numTotalPoints, int aux);
		float vpTrilinearInterpolation(int i, int j, int k,  
									   VPGraphicObj *volume, VPPoint3D p);
		VPPoint2D vpGetViewSBandPoint();
		VPPoint2D vpGetViewTBandPoint();
		void vpGetLineSBand(VPPoint2D **l, int &n);
		void vpGetLineTBand(VPPoint2D **l, int &n);
};




#endif	// __VPSLICE_H
