///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients

//deprecated
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpmovment.cpp
//  DESCRIPTION.: Contain the VPMovement class definitions.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: June/06/2001
//  DESCRIPTION.: Methods definition.
//
///////////////////////////////////////////////////////////////////
// Registro de Alteracoes
// 04/MAI/04 - Bruno Schneider
// - alterei os includes
#include <vpvector3d.h>
//#include <stdio.h>
#include <vpmovement.h>

#include <string>
using namespace std;


VPMovement::VPMovement(){

  bodyName = "";
  cycle = false;
  deltaT = 0;
  time = INIT_TIME;
  time_err = 0;
}

VPMovement::VPMovement( string bn, bool cy, float dt, float te ){
  bodyName = bn;
  cycle = cy;
  deltaT = dt;
  time = INIT_TIME;
  time_err = te;
}


VPMovement::~VPMovement(){
}

float
VPMovement::vpGetDeltaT(){
  return deltaT;
}

bool
VPMovement::vpGetCycle(){
  return cycle;
}

float
VPMovement::vpGetTime(){
  return time;
}

float
VPMovement::vpGetTimeFinal(){
  return tFinal;
}

float
VPMovement::vpGetTime_Err(){
  return time_err;
}

int
VPMovement::vpGetQtdMotion(){
  return qtdMotion;
}

VPJointMotion *
VPMovement::vpGetTimeline(){
  return motionTimeline;
}

void
VPMovement::vpSetDeltaT( float dt ){
  deltaT = dt;
}

void
VPMovement::vpSetCycle( bool c ){
  cycle = c;
}

void
VPMovement::vpSetTime( float t ){
  time = t;
}

void
VPMovement::vpSetTimeFinal( float tf ){
  tFinal = tf;
}

void
VPMovement::vpSetTime_Err( float te ){
  time_err = te;
}

void
VPMovement::vpSetQtdMotion( int qm ){
  qtdMotion = qm;
}

void
VPMovement::vpLoadMotionFile( char *fileName ){


    // Initialize the XML4C system
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         printf( "Error during initialization! : %s\n", toCatch.getMessage() );
         return;
    }

    DOMParser::ValSchemes    valScheme = DOMParser::Val_Auto; //Val_Never Val_Always
    bool                     doNamespaces    = false;

    // Instantiate the DOM parser.
    DOMParser parser;
    parser.setValidationScheme(valScheme);
    parser.setDoNamespaces(doNamespaces);

    // And create our error handler and install it
    DOMCountErrorHandler errorHandler;
    parser.setErrorHandler(&errorHandler);

    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;
    try
    {
        const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
        parser.parse(fileName);
        const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
        duration = endMillis - startMillis;
    }

    catch (const XMLException& toCatch)
    {
        cerr << "\nError during parsing: '" << fileName << "'\n"
             << "Exception message is:  \n"
             << StrX(toCatch.getMessage()) << "\n" << endl;
        return;
    }
    catch (const DOM_DOMException& toCatch)
    {
        cerr << "\nError during parsing: '" << fileName << "'\n"
             << "Exception message is:  \n"
             << toCatch.msg.transcode() << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return;
    }
    catch (...)
    {
       cerr << "\nUnexpected exception during parsing: '" << fileName << "'\n";
        XMLPlatformUtils::Terminate();
        return;
    }

    //
    //  Extract the DOM tree, get the list of all the elements and report the
    //  length as the count of elements.
    //
    if (errorHandler.getSawErrors())
    {
        cout << "\nErrors occured, no output available\n" << endl;
    }
     else
    {
        DOM_Document doc = parser.getDocument();
        unsigned int elementCount = doc.getElementsByTagName("*").getLength();
				
				DOM_Element root = doc.getDocumentElement();
								
				/// Find root joint
				DOM_NodeList move = root.getElementsByTagName("movement");

				for( int i = 0; i < 1/*move.getLength()*/; i++ ){ // specific for ONLY ONE Body
					DOM_NamedNodeMap attrmove = move.item(i).getAttributes();
					bodyName = attrmove.getNamedItem( DOMString("body_name") ).getNodeValue().transcode();
					if( attrmove.getNamedItem( DOMString("cycle") ).getNodeValue().equals( DOMString( "on" ) ) ){
					  cycle = true;
					} else {
					  cycle = false;
					}
					deltaT = atof( attrmove.getNamedItem( DOMString("delta_t") ).getNodeValue().transcode() );
					time_err = atof( attrmove.getNamedItem( DOMString("time_err") ).getNodeValue().transcode() );
					tFinal = 0.0;
					
					DOM_NodeList motion = move.item(i).getChildNodes();
					int numMotion = 0;
					for( int j = 0; j < motion.getLength(); j++ ){
						if( motion.item(j).getNodeName().equals( "motion" ) ) numMotion++;
					}
					motionTimeline = new VPJointMotion [ numMotion ];
					qtdMotion = 0;
					for( int j = 0; j < motion.getLength(); j++ ){
						if( motion.item(j).getNodeName().equals( "motion" ) ){
  						DOM_NamedNodeMap attrmotion = motion.item(j).getAttributes();    					
    					string jointName = attrmotion.getNamedItem( DOMString("joint_name") ).getNodeValue().transcode();
    					float t0 = atof( attrmotion.getNamedItem( DOMString("t0") ).getNodeValue().transcode() );
  					  float tf = atof( attrmotion.getNamedItem( DOMString("tf") ).getNodeValue().transcode() );
  					  float parameter = atof( attrmotion.getNamedItem( DOMString("parameter") ).getNodeValue().transcode() );
  					  int motionType = FLEX;
  					  if( attrmotion.getNamedItem( DOMString("type") ).getNodeValue().equals( DOMString( "ADDUCT" ) ) ){
  					      motionType = ADDUCT;
  					  } else if( attrmotion.getNamedItem( DOMString("type") ).getNodeValue().equals( DOMString( "TWIST" ) ) ){
  					    motionType = TWIST;
  					  }
  					  motionTimeline[qtdMotion++] = *( new VPJointMotion( jointName, motionType, t0, tf, parameter ) );
  					  if( tf > tFinal ) tFinal = tf;
  					}
					}
				}
				
				/// Create body		
				//body = new VPBody( fileName, "Joe", rootJoint );

        // Print out the stats that we collected and time taken.
        cout << fileName << ": " << duration << " ms ("
             << elementCount << " elems)." << endl;
    }

    // And call the termination method
    //XMLPlatformUtils::Terminate();

    return;
}
