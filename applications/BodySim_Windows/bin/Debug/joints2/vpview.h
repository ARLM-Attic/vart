///////////////////////////////////////////////////////////////////

//

//  PROJECT.....: vpat - Creating Virtual Patients

//  RESPONSIBLE.: Carla Freitas e Luciana Nedel

//

//  FILE........: vpview.h

//  DESCRIPTION.: Contain the VPView class declarations. 

//

//  AUTHOR......: Isabel Harb Manssour

//  DATE........: November/30/2000

//  DESCRIPTION.: Class declaration.

//

///////////////////////////////////////////////////////////////////





#include <vpscene.h>

#include <vpgraphicobj.h>

#include <list>		//STL include

//using namespace std;





// To foresee several "includes"
#ifndef __VPVIEW_H	
#define __VPVIEW_H




// Visualization Types

#define		MIPVIS				1201

#define		MONOCOLORVIS		1202

#define		MONOGRAYVIS			1203

#define		MULTIMODALVIS		1204

#define		DEFAULTVIS			1205

#define		SLICETOP			1206

#define		SLICEFRONT			1207

#define		SLICESIDE			1208









//-----------------------------------------------------------------

// V I E W P O R T

//-----------------------------------------------------------------





///////////////////////////////////////////////////////////////////

// Class Name: VPView 

// Superclass: -

// Subclasses: -



class VPView {

	private:

		VPScene *scene;

		int visualizationType;



	public:

		VPView();

		VPView(VPScene *s, int v);

		int vpGetVisualizationType();

		void vpSetVisualizationType(int vp);

		VPScene* vpGetScene();

		void vpSetScene(VPScene *s);

		virtual void vpDraw() = 0;

		virtual void vpRefresh() = 0;

};





#endif	// __VPVIEW_H
