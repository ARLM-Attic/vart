
#include <vpinventorview.h>

#include <Inventor/SbLinear.h>
#include <Inventor/QT/SoQt.h>
#include <Inventor/QT/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include	<vpgraphicobj.h>
#include	<vpcylinder.h>
#include	<vppoint3d.h>
#include	<vpplanejoint.h>
#include	<vpuniaxialjoint.h>
#include	<vpbiaxialjoint.h>
#include	<vppolyaxialjoint.h>
#include	<vpvertex3d.h>
#include	<vpbezier.h>
#include	<vpdof.h>
#include	<vpmatrix.h>
#include	<vpmodifier.h>

//# include <strstream>
//# include <fstream>
//# include <stdio>
//# include <stdlib.h>


using namespace std;  

static int controle = 48;
//static float pos;
//static float sinal;
static bool liberado = false;
static SoTransform *trans[200]; // It isn't a magic number, it's the maximum amount joints in a body

static SoQtExaminerViewer *viewer = NULL;
static SoSeparator * sep;
static SoPerspectiveCamera *cam = NULL;

static VPScene *vpatScene;
static VPBody	*vpatBody;
static VPMovement	*simulator;

// Load an iv file into a separator
///////////////////////////////////
SoSeparator*
VPInventorView::loadFile( const char *fileName ){
	
	SoInput in;
	SoSeparator *node;
  if ( !in.openFile( fileName ) ){
  	printf ("Could not load file %s\n", fileName );
  	return 0;
  }
  node = SoDB::readAll(&in);
  in.closeFile();
  return node;
}

// Update transformations (is called every timesensor loop)
//////////////////////////////////////////////////////////
void
WorkProc ( void *, SoSensor * ){

  if( liberado ){ 		
  	if( ( simulator->vpGetTime() >= simulator->vpGetTimeFinal()-simulator->vpGetTime_Err() ) &&
  	    ( simulator->vpGetTime() <= simulator->vpGetTimeFinal()+simulator->vpGetTime_Err() ) &&
  	    ( simulator->vpGetCycle() ) ){
  	        simulator->vpSetTime( 0.0 );
  	}
  	
  	for( int i = 0; i < simulator->vpGetQtdMotion(); i++ ){
  	
  	  VPJoint *jointAux = vpatBody->vpGetJointByName( simulator->vpGetTimeline()[i].vpGetJointName() );
 	
  	  if( ( simulator->vpGetTimeline()[i].vpGetTimeIni() >= simulator->vpGetTime()-simulator->vpGetTime_Err() ) &&
    	    ( simulator->vpGetTimeline()[i].vpGetTimeIni() <= simulator->vpGetTime()+simulator->vpGetTime_Err() ) ){
  	        //calculate deltaS
  	        float gds = 0;
  	        switch( simulator->vpGetTimeline()[i].vpGetMotionType() ){
             case FLEX   : gds = simulator->vpGetTimeline()[i].vpGetParameter() - (((VPPlaneJoint*)jointAux )->vpGetDofList()[FLEX])->vpGetCurrent();
                           break;
             case ADDUCT : gds = simulator->vpGetTimeline()[i].vpGetParameter() - (((VPPlaneJoint*)jointAux )->vpGetDofList()[ADDUCT])->vpGetCurrent();
                           break;
             case TWIST  : gds = simulator->vpGetTimeline()[i].vpGetParameter() - (((VPPlaneJoint*)jointAux )->vpGetDofList()[TWIST])->vpGetCurrent();
                           break;
            }
  	        float gdt = ( simulator->vpGetTimeline()[i].vpGetTimeFin() - simulator->vpGetTimeline()[i].vpGetTimeIni() ) / simulator->vpGetDeltaT();
  	        simulator->vpGetTimeline()[i].vpSetDeltaS( gds / gdt );
  	  }
  	
      // move to new position	
      float newPos = 0.0;
      switch( simulator->vpGetTimeline()[i].vpGetMotionType() ){
        case FLEX   : newPos = (((VPPlaneJoint*)jointAux )->vpGetDofList()[FLEX])->vpGetCurrent() + simulator->vpGetTimeline()[i].vpGetDeltaS();
                      ((VPPlaneJoint*)jointAux )->vpSetFlexionTo( newPos );
                      //printf( "FLEX:%f\n", newPos );
                      break;
        case ADDUCT : newPos = (((VPPlaneJoint*)jointAux )->vpGetDofList()[ADDUCT])->vpGetCurrent() + simulator->vpGetTimeline()[i].vpGetDeltaS();
                      ((VPPlaneJoint*)jointAux )->vpSetAdductionTo( newPos );
                      //printf( "ADDUCT:%f\n", newPos );
                      break;
        case TWIST  : newPos = (((VPPlaneJoint*)jointAux )->vpGetDofList()[TWIST])->vpGetCurrent() + simulator->vpGetTimeline()[i].vpGetDeltaS();
                      ((VPPlaneJoint*)jointAux )->vpSetTwistTo( newPos );
                      //printf( "TWIST:%f\n", newPos );
                      break;
      }


      if( ( simulator->vpGetTimeline()[i].vpGetTimeFin() >= simulator->vpGetTime()-simulator->vpGetTime_Err() ) &&
    	    ( simulator->vpGetTimeline()[i].vpGetTimeFin() <= simulator->vpGetTime()+simulator->vpGetTime_Err() ) ){
  	        //reset deltaS
  	        simulator->vpGetTimeline()[i].vpSetDeltaS( 0.0000000000 );
  	  }
  	}
  	
    vpatBody->vpUpdate();

     //getchar();
    /*printf("A controle %d \n", controle );
    if( controle == 48 ){
      controle = (int)getchar();
      getchar();
    } else {
      controle--;
    }
    printf("D controle %d \n", controle );
    */
    	
		// Update Inventor model
		list<VPGraphicObj*> objects = vpatScene->vpGetObjects();
		list<VPGraphicObj*>::const_iterator iter;
		int iTrans = 0;
		for (iter = objects.begin(); !(iter == objects.end()); iter++){
      	
    	SbMatrix gim;

    	float aa[4][4];
  		VPMatrix *vpGim = ((*iter)->vpGetGlobalInstanceMatrix());

  		for( int l = 0; l < 4; l++ ){
 				for( int c = 0; c < 4; c++ ){
   				aa[l][c] = vpGim->vpGetValueAt( l, c );
 				}
 			}
   			   			
 			float f1 = vpGim->vpGetValueAt( 0, 0 );
 			float f2 = vpGim->vpGetValueAt( 0, 1 );
 			float f3 = vpGim->vpGetValueAt( 0, 2 );
 			float f4 = vpGim->vpGetValueAt( 0, 3 );
 			float f5 = vpGim->vpGetValueAt( 1, 0 );
 			float f6 = vpGim->vpGetValueAt( 1, 1 );
 			float f7 = vpGim->vpGetValueAt( 1, 2 );
 			float f8 = vpGim->vpGetValueAt( 1, 3 );
 			float f9 = vpGim->vpGetValueAt( 2, 0 );
 			float f10 = vpGim->vpGetValueAt( 2, 1 );
 			float f11 = vpGim->vpGetValueAt( 2, 2 );
 			float f12 = vpGim->vpGetValueAt( 2, 3 );
 			float f13 = vpGim->vpGetValueAt( 3, 0 );
 			float f14 = vpGim->vpGetValueAt( 3, 1 );
 			float f15 = vpGim->vpGetValueAt( 3, 2 );
 			float f16 = vpGim->vpGetValueAt( 3, 3 );
      		
  		//gim = SbMatrix( aa );
  		gim = SbMatrix( f1, f2, f3, f4, f5, f6, f7, f8,
  										f9, f10, f11, f12, f13, f14, f15, f16 );
  		trans[iTrans++]->setMatrix( gim );
 		}
	

    // Update time
    simulator->vpSetTime( simulator->vpGetTime()+simulator->vpGetDeltaT() );
    char *sza;
    char szt[10] = "";
    int dec, sign;
    sza = fcvt( simulator->vpGetTime(), 2, &dec, &sign);
    if( sign == 1 ) strcat( szt, "-" );
    strncat( szt, sza, dec );
    char *sz2 = &sza[dec];
    if( sign == 1 ) szt[dec+1] = '\0';
    else szt[dec] = '\0';
    strcat( szt, "." );
    strcat( szt, sz2 );
    char saida[30] = "BodySim 3D - (";
    strcat( saida, szt );
    strcat( saida, " seg)" );
    viewer->setTitle( saida );
  }
}

// Inventor main loop, runs auto-iteratively
///////////////////////////////////////////////
void
VPInventorView::MainLoop (){

   SoTimerSensor *timer = new SoTimerSensor(WorkProc, NULL);
   timer->setInterval(SbTime( simulation.vpGetDeltaT() ));//0.01));
   timer->schedule();

   SoQt::mainLoop();
}

// Constructor for viewer
/////////////////////////
VPInventorView::VPInventorView(VPScene *mainScene, VPBody *mainBody ){

	// Window initialization -----------------------------------------------------
  QWidget *myWindow = SoQt::init( "Joints Viewer 0.1");

  // Set up viewer -------------------------------------------------------------
  viewer = new SoQtExaminerViewer(myWindow, "Juventude", true, SoQtFullViewer::BUILD_ALL, SoQtViewer::EDITOR ); //new SoXtEximinerViewer(myWindow);
  viewer->setDecoration ( true );
  viewer->setBorder     ( true );
  //SbColor bkgndColor = SbColor( 1, 1, 1 );
  //viewer->setBackgroundColor( bkgndColor );
  viewer->setTitle( "BodySim 3D" );
  viewer->show();

 // Set a scene -------------------------------------------------------------
  sep = new SoSeparator;
  cam = new SoPerspectiveCamera;
  sep->addChild ( cam );
	
	vpatScene = mainScene;
	vpatBody = mainBody;
		
  sep->addChild( loadVPScene( vpatScene ) );
	
  viewer->setSceneGraph ( sep );
  cam->viewAll ( sep, viewer->getViewportRegion() );

	
  // Initialize animation engine -----------------------------------------------
  simulation = VPMovement();
	simulation.vpLoadMotionFile( (char*)vpatBody->vpGetFileName().c_str() );
  simulator = &simulation;
  simulator->vpSetTime( -1.0 );
	
  // Main loop -----------------------------------------------------------------
  liberado = true;
  SoQt::show(myWindow);

  controle = 48;
  //getchar();

  printf( "Main loop started.\n" );
  MainLoop();
}


//Load a VPat Scene
///////////////////
SoSeparator*
VPInventorView::loadVPScene( VPScene *vpScn ){
	
	SoSeparator	*root = new SoSeparator;
	list<VPGraphicObj*> objects = vpScn->vpGetObjects();
	list<VPGraphicObj*>::const_iterator iter;
	int iTrans = 0;
  for (iter = objects.begin(); !(iter == objects.end()); iter++){
  	
  	SoSeparator	*aux = new SoSeparator;
   	SbMatrix gim;
   	//if( !( iter == objects.begin() ) ){//
   	//	SoSeparator *aux1 = new SoSeparator;//
   	//	aux->addChild( aux1 );//
   	//	aux = aux1;//
   	//}//
 	
   	// Add transformation to separator
   	SoTransform *t = new SoTransform;
   	float aa[4][4];
 		VPMatrix *vpGim = ((*iter)->vpGetGlobalInstanceMatrix());

 		for( int l = 0; l < 4; l++ ){
			for( int c = 0; c < 4; c++ ){
  			aa[l][c] = vpGim->vpGetValueAt( l, c );
			}
		}
				
 		float f1 = vpGim->vpGetValueAt( 0, 0 );
 		float f2 = vpGim->vpGetValueAt( 0, 1 );
 		float f3 = vpGim->vpGetValueAt( 0, 2 );
 		float f4 = vpGim->vpGetValueAt( 0, 3 );
 		float f5 = vpGim->vpGetValueAt( 1, 0 );
 		float f6 = vpGim->vpGetValueAt( 1, 1 );
 		float f7 = vpGim->vpGetValueAt( 1, 2 );
 		float f8 = vpGim->vpGetValueAt( 1, 3 );
 		float f9 = vpGim->vpGetValueAt( 2, 0 );
 		float f10 = vpGim->vpGetValueAt( 2, 1 );
 		float f11 = vpGim->vpGetValueAt( 2, 2 );
 		float f12 = vpGim->vpGetValueAt( 2, 3 );
 		float f13 = vpGim->vpGetValueAt( 3, 0 );
 		float f14 = vpGim->vpGetValueAt( 3, 1 );
 		float f15 = vpGim->vpGetValueAt( 3, 2 );
 		float f16 = vpGim->vpGetValueAt( 3, 3 );
		gim = SbMatrix( f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16 );
		  							
		t->setMatrix( gim );
		aux->addChild( t );
   	trans[iTrans++] = t;
		
   	// Add shape to separator
   	//SoCylinder *cil = new SoCylinder;
   	//cil->height = 2;//((VPCylinder*)(*iter))->vpGetHeight();
   	//cil->radius = 0.1;//((VPCylinder*)(*iter))->vpGetRadius();
   	//cil->removePart( SoCylinder::TOP );
   	//aux->addChild( cil );
   	
   	
   	aux->addChild( loadFile( ( (*iter)->vpGetFileName() ).c_str() ) );
   	root->addChild( aux );
   	printf( "Added object from %s.\n", ( (*iter)->vpGetFileName() ).c_str() );
  }

	return root;
}




void
VPInventorView::vpDraw(){


}
		
void
VPInventorView::vpRefresh(){


}
