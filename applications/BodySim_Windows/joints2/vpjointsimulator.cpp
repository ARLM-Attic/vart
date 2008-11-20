#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include	<iostream>
#include	<stdlib.h>
#include	<stdio.h>

#include	<vppoint3d.h>
#include	<vpdof.h>
#include	<vpbody.h>
#include	<vpgraphicobj.h>
#include	<vpcylinder.h>
#include	<vpuniaxialjoint.h>
#include	<vpbiaxialjoint.h>
#include	<vpvertex3d.h>
#include	<vpbezier.h>
#include	<vpmatrix.h>
#include	<vpmodifier.h>
#include	<vpinventorview.h>

#include <vpjointsimulator.h>

#include	<vpxml.h>


VPBody	*vpatBody;

//Launch and start the application
//////////////////////////////////
int
main( int argc, char *argv[] ){
		new VPJointSimulator();	
		return 1;
}

//Initialize and create a Joint Simulator with a scene and a view
/////////////////////////////////////////////////////////////////
VPJointSimulator::VPJointSimulator( void ){
		VPXml doc = VPXml();
		printf("Type XML file name: ");
		char sFname[64];// = "hebe1.xml";
		scanf("%s", sFname );
		doc.vpLoadBodyFile( sFname );
		vpatScene = doc.vpGetXmlScene();
		vpatBody = doc.vpGetXmlBody();
		vpatView = new VPInventorView( vpatScene, vpatBody  );	
}

