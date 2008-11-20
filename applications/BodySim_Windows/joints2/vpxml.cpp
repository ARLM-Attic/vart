///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpxml.cpp
//  DESCRIPTION.: Contain the VPXml class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: March/01/2001
//  DESCRIPTION.: VPXml class methods definition.
//
///////////////////////////////////////////////////////////////////

#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>
#include <sax/SAXParseException.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_DOMException.hpp>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include	<vppoint3d.h>
#include	<vpdof.h>
#include	<vpbody.h>
#include	<vpgraphicobj.h>
#include	<vpcylinder.h>
#include	<vpuniaxialjoint.h>
#include	<vpbiaxialjoint.h>
#include	<vppolyaxialjoint.h>
#include	<vpplanejoint.h>
#include	<vpvertex3d.h>
#include	<vpbezier.h>
#include	<vpmatrix.h>
#include	<vpmodifier.h>

#include "vpxml.h"

using namespace XERCES_CPP_NAMESPACE;


VPXml::VPXml(){

	scene = new VPScene();
	body = NULL;
	auxVec = NULL;
}

void
VPXml::vpLoadBodyFile( char *fileName ){


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
				
				root = doc.getDocumentElement();
				/*
				printf( " Elemento raiz: %s \n", root.getTagName().transcode() );
				
				DOM_NodeList lista = root.getElementsByTagName("*");
				
				for( int i = 0; i < lista.getLength(); i++ ){
					cout << "Nodo: " << i << " - " << lista.item(i).getNodeName().transcode() << endl;
				}
				*/
								
				/// Find root joint
				DOM_NodeList joints = root.getElementsByTagName("joint");
				VPJoint *rootJoint;
				for( int i = 0; i < joints.getLength(); i++ ){
					DOM_NamedNodeMap attr = joints.item(i).getAttributes();
					if( attr.getNamedItem( DOMString("description") ).getNodeValue().equals("root") ){
						auxVec = new VPDof*[joints.getLength()*6];
						iIndDofAux = 0;
						rootJoint = loadJoint( joints.item(i), NULL );
						loadModifiers( joints );
						delete [] auxVec;
						break;
					}
				}
				
				/// Create body		
				body = new VPBody( fileName, "Joe", rootJoint );

        // Print out the stats that we collected and time taken.
        cout << fileName << ": " << duration << " ms ("
             << elementCount << " elems)." << endl;
    }

    // And call the termination method
    //XMLPlatformUtils::Terminate();

    return;
}

VPJoint*
VPXml::loadJoint( DOM_Node node, VPJoint *parent ){

	/// Create a Joint and its dependencies
	DOM_NamedNodeMap attrNode = node.getAttributes();
	DOM_NodeList dofs_shapes = node.getChildNodes();
	VPDof *dofAux = loadDofs( dofs_shapes );
	VPJoint *newJoint;
	
	if( attrNode.getNamedItem( DOMString("type") ).getNodeValue().equals("plane") ){
		newJoint = new VPPlaneJoint( parent, &(dofAux[0]), &(dofAux[1]), &(dofAux[2]), &(dofAux[3]), &(dofAux[4]), &(dofAux[5]) );
	} else if( attrNode.getNamedItem( DOMString("type") ).getNodeValue().equals("uniaxial") ){
		newJoint = new VPUniaxialJoint( parent, &(dofAux[0]) );
	} else if( attrNode.getNamedItem( DOMString("type") ).getNodeValue().equals("biaxial") ){
		newJoint = new VPBiaxialJoint( parent, &(dofAux[0]), &(dofAux[1]) );
	} else if( attrNode.getNamedItem( DOMString("type") ).getNodeValue().equals("polyaxial") ){
		newJoint = new VPPolyaxialJoint( parent, &(dofAux[0]), &(dofAux[1]), &(dofAux[2]) );
	}
	newJoint->vpSetDescription( attrNode.getNamedItem( DOMString("description") ).getNodeValue().transcode() );
	int numShapes = 0;
	VPGraphicObj *shapeAux = loadShapes( dofs_shapes, &numShapes );
	for( int i = 0; i < numShapes; i++ ){
		newJoint->vpAddShape( &(shapeAux[ i ]) );
		scene->vpAddObject( &(shapeAux[ i ]) );
	}
	
	/// Call loadJoint recursively to create the tree
	DOM_NodeList joints = root.getElementsByTagName("joint");
	for(i = 0; i < joints.getLength(); i++ ){
		DOM_NamedNodeMap attrAux = joints.item(i).getAttributes();
		if( attrAux.getNamedItem( DOMString("parent") ).getNodeValue().equals( attrNode.getNamedItem( DOMString("description") ).getNodeValue() ) ){
			VPJoint* auxJoint = loadJoint( joints.item(i), newJoint );
			newJoint->vpAddChild( auxJoint );
		}
	}
	return newJoint;
}

VPDof*
VPXml::loadDofs( DOM_NodeList list ){

	/// Scan dofs of the list
	VPDof* listDof = new VPDof[6];
	int idof = 0;
	for( int i = 0; i < list.getLength(); i++ ){
		if( list.item(i).getNodeName().equals( "dof") ){
			/// Get dof properties
			VPVector3D axis;
			VPPoint3D position;
			VPCurve* evoluta = new VPBezier();
			float min = 0.0, max = 0.0 , curr = 0.0, rest = 0.0;
			DOM_NodeList dof = list.item(i).getChildNodes();
			for( int j = 0; j < dof.getLength(); j++ ){
				if( dof.item(j).getNodeName().equals( "position") ){
						DOM_NamedNodeMap attrPos = dof.item(j).getAttributes();
						position = VPPoint3D( (float)atof( attrPos.getNamedItem( DOMString("x") ).getNodeValue().transcode() ),
																	(float)atof( attrPos.getNamedItem( DOMString("y") ).getNodeValue().transcode() ),
																	(float)atof( attrPos.getNamedItem( DOMString("z") ).getNodeValue().transcode() )
															 	);
				} else if( dof.item(j).getNodeName().equals( "axis") ){
						DOM_NamedNodeMap attrAxis = dof.item(j).getAttributes();
						axis = VPVector3D( (float)atof( attrAxis.getNamedItem( DOMString("x") ).getNodeValue().transcode() ),
															 (float)atof( attrAxis.getNamedItem( DOMString("y") ).getNodeValue().transcode() ),
															 (float)atof( attrAxis.getNamedItem( DOMString("z") ).getNodeValue().transcode() )
														 );
				} else if( dof.item(j).getNodeName().equals( "range") ){
						DOM_NamedNodeMap attrRange = dof.item(j).getAttributes();
						min = (float)atof( attrRange.getNamedItem( DOMString("min") ).getNodeValue().transcode() );
						max = (float)atof( attrRange.getNamedItem( DOMString("max") ).getNodeValue().transcode() );
				} else if( dof.item(j).getNodeName().equals( "comfort_range") ){
						DOM_NamedNodeMap attrComf = dof.item(j).getAttributes();
						rest = (float)atof( attrComf.getNamedItem( DOMString("rest") ).getNodeValue().transcode() );
				} else if( dof.item(j).getNodeName().equals( "evoluta") ){
						DOM_NodeList pts = dof.item(j).getChildNodes();
						VPVertex3D*	ctrlPts = new VPVertex3D[4];
						int cp = 0;
						for( int k = 0; k < pts.getLength(); k++ ){
							if( pts.item(k).getNodeName().equals( "ctrl_point") ){
  							DOM_NamedNodeMap attrPt = pts.item(k).getAttributes();
  							ctrlPts[cp++].vpSetXYZ( (float)atof( attrPt.getNamedItem( DOMString("x") ).getNodeValue().transcode() ),
  																	 		(float)atof( attrPt.getNamedItem( DOMString("y") ).getNodeValue().transcode() ),
  																	 		(float)atof( attrPt.getNamedItem( DOMString("z") ).getNodeValue().transcode() )
  															 			);
  						}
						}
						evoluta = new VPBezier( ctrlPts );
				}
			}
			/// Create DOF
			listDof[idof] = VPDof( axis, position, evoluta, min, max, curr, rest );
			DOM_NamedNodeMap attrAux = list.item(i).getAttributes();
			listDof[idof].vpSetDescription( attrAux.getNamedItem( DOMString("description") ).getNodeValue().transcode() );
			listDof[idof].vpRest();
			
			//put dof in the temporary list for modifiers
			auxVec[iIndDofAux++] = &(listDof[idof]);
			
			idof++;
		}
	}
	return listDof;
}


VPGraphicObj*
VPXml::loadShapes( DOM_NodeList list, int *num ){

	/// Count shapes
	(*num) = 0;
	for( int i = 0; i < list.getLength(); i++ ){
		if( list.item(i).getNodeName().equals( "shape") ){
			(*num)++;
		}
	}
	
	/// Create Shapes
	VPGraphicObj* listShape = new VPGraphicObj[*num];
	int iShape = 0;
	for(i = 0; i < list.getLength(); i++ ){
		if( list.item(i).getNodeName().equals( "shape") ){
			DOM_NodeList nums = list.item(i).getChildNodes();
			DOM_NamedNodeMap attrShape = list.item(i).getAttributes();
 			float	matrix[16];
 			for( int j = 0; j < nums.getLength(); j++ ){
 				if( nums.item(j).getNodeName().equals( "position") ){
 					DOM_NamedNodeMap attrPt = nums.item(j).getAttributes();
 					matrix[12] = (float)atof( attrPt.getNamedItem( DOMString("x") ).getNodeValue().transcode() );
 					matrix[13] = (float)atof( attrPt.getNamedItem( DOMString("y") ).getNodeValue().transcode() );
 					matrix[14] = (float)atof( attrPt.getNamedItem( DOMString("z") ).getNodeValue().transcode() );
 					matrix[15] = 1;
 				} else if( nums.item(j).getNodeName().equals( "iaxis") ){
 						DOM_NamedNodeMap attrPt = nums.item(j).getAttributes();
 						matrix[0] = (float)atof( attrPt.getNamedItem( DOMString("x") ).getNodeValue().transcode() );
 						matrix[4] = (float)atof( attrPt.getNamedItem( DOMString("y") ).getNodeValue().transcode() );
 						matrix[8] = (float)atof( attrPt.getNamedItem( DOMString("z") ).getNodeValue().transcode() );
 						matrix[3] = 0;
 				} else if( nums.item(j).getNodeName().equals( "jaxis") ){
 						DOM_NamedNodeMap attrPt = nums.item(j).getAttributes();
 						matrix[1] = (float)atof( attrPt.getNamedItem( DOMString("x") ).getNodeValue().transcode() );
 						matrix[5] = (float)atof( attrPt.getNamedItem( DOMString("y") ).getNodeValue().transcode() );
 						matrix[9] = (float)atof( attrPt.getNamedItem( DOMString("z") ).getNodeValue().transcode() );
 						matrix[7] = 0;
 				} else if( nums.item(j).getNodeName().equals( "kaxis") ){
 						DOM_NamedNodeMap attrPt = nums.item(j).getAttributes();
 						matrix[2] = (float)atof( attrPt.getNamedItem( DOMString("x") ).getNodeValue().transcode() );
 						matrix[6] = (float)atof( attrPt.getNamedItem( DOMString("y") ).getNodeValue().transcode() );
 						matrix[10] = (float)atof( attrPt.getNamedItem( DOMString("z") ).getNodeValue().transcode() );
 						matrix[11] = 0;
 				}
 			}
 			VPMatrix *objLim = new VPMatrix( matrix );
			listShape[ iShape ].vpSetLocalInstanceMatrix( objLim );
			char desct[50];
			printf(" description = %s\n",  attrShape.getNamedItem( DOMString("description") ).getNodeValue().transcode() );
			listShape[ iShape ].vpSetDescription( attrShape.getNamedItem( DOMString("description") ).getNodeValue().transcode() );
			char fName[256] = "";
			strcpy(fName, IVPATH);
			strcat( fName, attrShape.getNamedItem( DOMString("file_name") ).getNodeValue().transcode() );
			listShape[ iShape ].vpSetFileName( fName );
			iShape++;
			printf(" loading %s...\n", fName );
 		}
 	}
 	return listShape;
}


void
VPXml::loadModifiers( DOM_NodeList joints ){

	for( int i = 0; i < joints.getLength(); i++ ){
		DOM_NodeList xmlDofs = joints.item(i).getChildNodes();
		for( int j = 0; j < xmlDofs.getLength(); j++ ){
			if( xmlDofs.item(j).getNodeName().equals( "dof") ){
				DOM_NamedNodeMap attrDof = xmlDofs.item(j).getAttributes();
 				DOM_NodeList xmlModif = xmlDofs.item(j).getChildNodes();
 				for( int k = 0; k < xmlModif.getLength(); k++ ){
 					if( xmlModif.item(k).getNodeName().equals( "modifier") ){
 						DOM_NodeList xmlRelation = xmlModif.item(k).getChildNodes();
 						int numRelat = 0;
 						for( int x = 0; x <	xmlRelation.getLength(); x++ ){
   						if( xmlRelation.item(x).getNodeName().equals( "relation") ) numRelat++;
   					}
   					VPDof** modifierDof = new VPDof*[numRelat];
 						VPDof* modifiedDof = NULL;
 						VPCurve *min = new VPCurve[numRelat];
 						VPCurve *max = new VPCurve[numRelat];
 						
 						int ll = 0;
   					for( int l = 0; l <	xmlRelation.getLength(); l++ ){
   						while( !xmlRelation.item(l).getNodeName().equals( "relation") ){
   								l++;
   								if( l >=	xmlRelation.getLength() ) break;
   						}
   						if( l >=	xmlRelation.getLength() ) break;
   						DOM_NamedNodeMap attrRelation = xmlRelation.item(l).getAttributes();
   						int mm = 0;
    					for( int m = 0; m <	iIndDofAux; m++ ){
    						if( attrRelation.getNamedItem( DOMString("dof") ).getNodeValue().equals( auxVec[m]->vpGetDescription().c_str() ) ){
     							modifierDof[mm++] = auxVec[m];
    						}
    						if( attrDof.getNamedItem( DOMString("description") ).getNodeValue().equals( auxVec[m]->vpGetDescription().c_str() ) ){
     							modifiedDof = auxVec[m];
    						}
    					}
    					
    					// get the points of the relation curves
    					DOM_NodeList xmlPoints = xmlRelation.item(l).getChildNodes();
    					VPVertex3D*	Ptm = new VPVertex3D[4];
    					VPVertex3D*	PtM = new VPVertex3D[4];
    					int nn = 0;
      				for( int n = 0; n < xmlPoints.getLength(); n++ ){
      					if( xmlPoints.item(n).getNodeName().equals( "ctrl_point") ){
      						DOM_NamedNodeMap attrPt = xmlPoints.item(n).getAttributes();
        					if( nn < 4 ){
                		Ptm[nn].vpSetXYZ( (float)atof( attrPt.getNamedItem( DOMString("x") ).getNodeValue().transcode() ),
                											(float)atof( attrPt.getNamedItem( DOMString("y") ).getNodeValue().transcode() ),
                											(float)atof( attrPt.getNamedItem( DOMString("z") ).getNodeValue().transcode() ) );
                	}
                	if( nn >= 4 ){
                		PtM[nn-4].vpSetXYZ( (float)atof( attrPt.getNamedItem( DOMString("x") ).getNodeValue().transcode() ),
                												(float)atof( attrPt.getNamedItem( DOMString("y") ).getNodeValue().transcode() ),
                										 		(float)atof( attrPt.getNamedItem( DOMString("z") ).getNodeValue().transcode() ) );
                	}
                	nn++;
                }
              }

              min[ll].vpSetControlPoints(Ptm);
              max[ll].vpSetControlPoints(PtM);
    					ll++;
    				}
    				// Create and set the modifier
   					VPModifier *modif = new VPModifier( modifierDof, min, max, numRelat );
   					modifiedDof->vpSetRangeModifier( modif );
   				}
   			}
			}
		}
	}
}



VPXml::~VPXml(){
}


VPScene*
VPXml::vpGetXmlScene( void ){
	return scene;
}

VPBody*
VPXml::vpGetXmlBody( void ){
	return body;
}

//---------------------------Error methods---------------------------

DOMCountErrorHandler::DOMCountErrorHandler() :

    fSawErrors(false)
{
}

DOMCountErrorHandler::~DOMCountErrorHandler()
{
}


// ---------------------------------------------------------------------------
//  DOMCountHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void DOMCountErrorHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nError at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void DOMCountErrorHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nFatal Error at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void DOMCountErrorHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    cerr << "\nWarning at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void DOMCountErrorHandler::resetErrors()
{
}
//----------------------------------------------------


