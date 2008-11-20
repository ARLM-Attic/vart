///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpview.cpp
//  DESCRIPTION.: Contain the VPView class implementation that 
//                manage the viewport.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: November/30/2000
//  DESCRIPTION.: Methods definition.
//
///////////////////////////////////////////////////////////////////


#include <vpview.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPView" constructor withou parameters
// Parameters.: -
// Return.....: -

VPView::VPView() {
	visualizationType = DEFAULTVIS;
	scene = NULL;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPView" constructor with parameters
// Parameters.: -
// Return.....: -

VPView::VPView(VPScene *s, int v) {
	visualizationType = v;
	scene = s;
	
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetVisualizationType" returns the value 
//				of "visualizationType" attribute.
// Parameters.: -
// Return.....: int visualizationType (attribute content)

int VPView::vpGetVisualizationType() {
	return(visualizationType);
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetVisualizationType" sends a new value 
//				to the "visualizationType" attribute.
// Parameters.: int vp (new visualization type)
// Return.....: -

void VPView::vpSetVisualizationType(int vp) {
	visualizationType = vp;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetScene" returns the value 
//				of "scene" attribute.
// Parameters.: -
// Return.....: VPScene* scene (attribute content)

VPScene* VPView::vpGetScene() {
	return scene;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetScene" sends a new value 
//				to the "scene" attribute.
// Parameters.: VPScene* s (new scene)
// Return.....: -

void VPView::vpSetScene(VPScene* s) {
	scene = s;
}

