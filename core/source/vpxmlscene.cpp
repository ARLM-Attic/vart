/// \file vpxmlscene.cpp
/// \brief Implementation file for VPXmlScene VPAT class.
/// \version $Revision: 1.5 $

// ChangeLog

//Mar 24, 2006 - Andreia Schneider
// - Added rest position of dofs.
//Mar 16, 2006 - Andreia Schneider
// - Deleted unused VPMeshObject pointers.
// - Implemented method called LoadMeshFromFile to avoid loading a file many times.
// - Implemented method LoadFromFile, that verifies if the xml parser is valid and calls the method LoadScene.
// - Implemented method called ParserFile, that parsers the xml file.
// - Class name has changed to VPXmlScene and now it is child of VPScene class.
// - Now, error messagens are shown in case of parser errors.
// - DOMcountErrorHandler was renamed to ErrorHandler and was nested into VPXML.
// - StrX was nested into VPXML.
//Feb 20, 2006 - Andreia Schneider
// - Reimplemented the part of load transformation, in method LoadSceneNode.
// - Class Reimplemented acording to nem DTD format.
// Aug 10, 2000 - Anderson Maciel
// - Classes declaration.
// Jun 28, 2000 - Anderson Maciel
// - Methods atualization.

#include "vpxmlscene.h"

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

VPXmlScene::VPXmlScene()
{
}

bool VPXmlScene::LoadFromFile(char* filename)
{
    meshMap::iterator iterMesh;
    meshObjMap::iterator iterMeshObj;

    bool bDoNamespaces          = false;
    bool bDoSchema              = false;
    bool bSchemaFullChecking    = false;
    bool bDoCreate              = false;
    
    
    static XercesDOMParser::ValSchemes ValScheme = XercesDOMParser::Val_Auto; //Val_Never Val_Always


    // Initialize the XML4C2 system...
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch(const XMLException &toCatch)
    {
        XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
            << "  Exception message:"
            << VPXmlScene::StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
        return false;
    }

    XercesDOMParser *parser = new XercesDOMParser;
    parser->setValidationScheme(ValScheme);
    parser->setDoNamespaces(bDoNamespaces);
    parser->setDoSchema(bDoSchema);
    parser->setValidationSchemaFullChecking(bSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(bDoCreate);

    if(ParserFile(filename, parser))
    { //XML parser sucessfull
        istringstream stream;
        // Extract the DOM tree, get the list of all the elements,
        DOMDocument* doc = parser->getDocument();
        //...load the elements of the scene,
        LoadScene(doc);
        //...delete the parser itself (Must be done prior to calling Terminate, below),
        delete parser;
        //...call the termination method
        XMLPlatformUtils::Terminate();
        // ...delete unused mesh objects and clear the maps.
        for(iterMesh = mapMesh.begin(); iterMesh != mapMesh.end(); iterMesh++)
        {
            for(iterMeshObj = mapMeshObj.begin(); iterMeshObj != mapMeshObj.end(); iterMeshObj++)
            {
                delete iterMeshObj->second;
            }
            mapMeshObj.clear();
        }
        mapMesh.clear();
        return true;
    }

    else
    {   //the parser has failled!
        //Delete the parser itself.  Must be done prior to calling Terminate, below.
        delete parser;
        //Call the termination method
        XMLPlatformUtils::Terminate();
        return false;
    }
}

bool VPXmlScene::ParserFile(char* filename, XercesDOMParser *parser)
{

    // Create the error handler and install it
    ErrorHandler* errorH = new ErrorHandler;
    parser->setErrorHandler(errorH);
    //  Parse the XML file, catching any XML exceptions that might propogate
    //  out of it.
    try
    {
        parser->parse( filename );
    }
    catch (const OutOfMemoryException&)
    {
        XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
        return false;
    }
    catch (const XMLException& e)
    {
        XERCES_STD_QUALIFIER cerr << "An error occurred during parsing\n   Message: "
             << VPXmlScene::StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
        return false;
    }
    catch (const DOMException& e)
    {
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << filename << "'\n"
             << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;
        if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
             XERCES_STD_QUALIFIER cerr << "Message is: " << VPXmlScene::StrX(errText) << XERCES_STD_QUALIFIER endl;
        return false;
    }
    catch (...)
    {
        XERCES_STD_QUALIFIER cerr << "An error occurred during parsing\n " << XERCES_STD_QUALIFIER endl;
        return false;
    }
    if (errorH->getSawErrors())
    {
        //  Clean up the error handler. The parser does not adopt handlers
        //  since they could be many objects or one object installed for multiple
        //  handlers.
        delete errorH;
        return false;
    }
    else
    {
        //...clean up the error handler. The parser does not adopt handlers
        //  since they could be many objects or one object installed for multiple
        //  handlers.
        delete errorH;
        return true;
    }
}

void VPXmlScene::LoadScene(DOMDocument* doc)
{
    VPSceneNode* sceneObject;
    
    unsigned int i;
    unsigned int j;
    
    DOMNodeList* sceneList = doc->getElementsByTagName(XMLString::transcode("scene"));

    //The scene can have cameras and sceneNodes (lights and other objects).
    for (i = 0; i<sceneList->getLength(); i++)
    {
        DOMNodeList* nodesL = sceneList->item(i)->getChildNodes();
        for(j = 0; j<nodesL->getLength(); j++)
        {
            if(strcmp(XMLString::transcode(nodesL->item(j)->getNodeName()), "camera")== 0)
            { ///fixMe: a vpLoadCamera method must be implemented
            }

            else if(strcmp(XMLString::transcode(nodesL->item(j)->getNodeName()), "node")== 0)
            {
                VPLight* lightPtr;
                lightPtr = new VPLight;
                lightPtr->autoDelete=true;
                sceneObject = LoadSceneNode(nodesL->item(j));
                lightPtr = dynamic_cast<VPLight*>(sceneObject);
                if (lightPtr) /// is it a light?
                {
                    AddLight(lightPtr);
                }
                else
                {
                    AddObject(sceneObject);
                }
            }
        }
    }
}

VPMeshObject* VPXmlScene::LoadMeshFromFile(string filen, string type, string meshName)
{
    meshMap::iterator iterMesh;
    meshObjMap::iterator iterMeshObj;
    list<VPMeshObject*> meshObjectList;
    list<VPMeshObject*>::iterator iter;
    VPMeshObject* finalMesh;
    VPMeshObject* result;
    finalMesh = new VPMeshObject;
    finalMesh->autoDelete = true;
    iterMesh = mapMesh.find(filen);
    int i=0;

    if(iterMesh != mapMesh.end())
    {//The file has been loaded
        iterMeshObj = mapMeshObj.find(meshName);
        if(iterMeshObj != mapMeshObj.end())
        {//The mesh has been loaded
            i++;
            finalMesh = iterMeshObj->second;
            mapMeshObj.erase(iterMeshObj);
            return finalMesh;
        }

        else
        {
            cerr<<"Error! Mesh "<<meshName<<" not found in file "<<filen<<endl;
            return NULL;
        }
    }

    else
    {
        //The file hasn't been loaded
        if(type == "obj")
        {
            meshObjectList.clear();
            VPMeshObject::ReadFromOBJ(filen, &meshObjectList);
            for (iter = meshObjectList.begin(); iter != meshObjectList.end(); ++iter)
            {
                if ((*iter)->GetDescription() == meshName)
                {
                    result=*iter;
                }
                else
                {
                    finalMesh = *iter;
                    mapMeshObj[finalMesh->GetDescription()] = finalMesh;
                }
            }
            mapMesh[filen] = mapMeshObj;
            //result->bBoxShow = true;
            return result;
        }
        else
        {
            //FixMe. Must load others types of file.
        }
    //result->bBoxShow = true;
    return result;
    }
}

VPSceneNode* VPXmlScene::LoadSceneNode(DOMNode* sceneList)
{
    istringstream stream;

    char *charAux;
    int numChildren;
    int currentChild = 1;
    VPSceneNode* result;

    DOMNodeList* listNode = sceneList->getChildNodes();
    numChildren = listNode->getLength();
    
    //The sceneNodes can be (joints, tranformations, geometry, lights, etc.)

    if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "bezier")== 0)
    { ///fixMe: this part must be implemented
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "sphere")== 0)
    {
        VPSphere* spherePtr = new VPSphere;
        spherePtr->autoDelete=true;
        float radius;
        unsigned int i;
        
        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        //The sphere is defined by a radius and a material.
        for(i = 0; i<childNodes->getLength(); i++)
        {
            if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "radius")== 0)
            {
                
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> radius;
                spherePtr->SetRadius(radius);
            }
            else if (strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "material")== 0)
            {
                int rTemp, gTemp, bTemp;
                unsigned char r, g, b;

                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("r"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> rTemp;
                r = static_cast<unsigned char>(rTemp);

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("g"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> gTemp;
                g = static_cast<unsigned char>(gTemp);

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("b"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> bTemp;
                b = static_cast<unsigned char>(bTemp);

                VPMaterial mat(VPColor(r, g, b));
                spherePtr->SetMaterial(mat);
            }
        }
        DOMNamedNodeMap* attrAux = listNode->item(1)->getAttributes();
        spherePtr->SetDescription(XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
        result =  spherePtr;
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "cylinder")== 0)
    {
        //The cylinder is defined by a radius, a height and a material.
        VPCylinder* cylinderPtr = new VPCylinder;
        cylinderPtr->autoDelete=true;
        float radius;
        float height;
        unsigned int i;
        

        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        
        for(i = 0; i<childNodes->getLength(); i++)
        {
            if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "radius")== 0)
            {
                
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> radius;
                cylinderPtr->SetRadius(radius);
            }
            else if ( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "height")== 0)
            {
                
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> height;
                cylinderPtr->SetHeight(height);
            }
            else if ( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "material")== 0)
            {
                unsigned char r, g, b;
                int rTemp, gTemp, bTemp;

                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("r"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> rTemp;
                r = static_cast<unsigned char> (rTemp);

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("g"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> gTemp;
                g = static_cast<unsigned char> (gTemp);

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("b"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> bTemp;
                b = static_cast<unsigned char> (bTemp);

                VPMaterial mat(VPColor(r,g,b));
                cylinderPtr->SetMaterial(mat);
            }
        }
        DOMNamedNodeMap* attrAux = listNode->item(1)->getAttributes();
        cylinderPtr->SetDescription(XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
        result =  cylinderPtr;
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "meshobject")== 0)
    {
        //the mesh object can have a material associated and is defined by a file (for example...
        // a Wavefront file).
        list<VPMeshObject*> meshObjectList;
        list<VPMeshObject*>::iterator iter;
        
        string file_name;
        string obj_name;
        string typeName;
        unsigned int i;
        unsigned int j; ///used to traversal in map of node attributes
        
        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrAux = listNode->item(1)->getAttributes();

        for(i = 0; i<childNodes->getLength(); i++)
        {
            if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "material")== 0)
            { ///fixMe: this must be implemented
            }
        }
        for (j = 0; j<attrAux->getLength(); j++)
        {
            if(strcmp(XMLString::transcode(attrAux->item(j)->getNodeName()), "filename")== 0)
            {
                charAux = XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("filename"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> file_name;
            }

            else if(strcmp(XMLString::transcode(attrAux->item(j)->getNodeName()), "description")== 0)
            {
                charAux = XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("description"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> obj_name;
            }

            else if (strcmp(XMLString::transcode(attrAux->item(j)->getNodeName()), "type")== 0)
            {
                charAux = XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("type"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> typeName;
            }
        }
        result = LoadMeshFromFile(file_name, typeName, obj_name);
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "directionallight")== 0)
    {
        VPLight* directionallightPtr = new VPLight;
        directionallightPtr->autoDelete=true;
        VPColor* color;
        color = new VPColor;
        VPPoint4D* location;
        location = new VPPoint4D;
        
        unsigned int i;
        float intensity;
        float ambientIntensity;
        float xPos;
        float yPos;
        float zPos;
        bool enabled;
        ///Color attributes
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
        istringstream stream;

        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();

        for(i = 0; i<childNodes->getLength(); i++)
        {
            if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "intensity")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> intensity;
                directionallightPtr->SetIntensity(intensity);
            }

            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "ambientIntensity")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> ambientIntensity;
                directionallightPtr->SetAmbientIntensity(ambientIntensity);
            }

            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "color")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("red"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> r;
                color->SetR(r);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("green"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> g;
                color->SetG(g);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("blue"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> b;
                color->SetB(b);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("alpha"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> a;
                color->SetA(a);
                
                directionallightPtr->SetColor(*color);
            }

            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "enabled")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> enabled;
                directionallightPtr->Turn(enabled);
            }

            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "position")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("x"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> xPos;
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("y"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> yPos;
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("z"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> zPos;
                location->SetXYZW(xPos, yPos, zPos, 1.0);
                directionallightPtr->SetLocation(*location);
            }
        }
        directionallightPtr->SetDescription(XMLString::transcode(attrTemp->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
        result =  directionallightPtr;
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "spotlight")== 0)
    {
        ///FixMe: The spotlight must have an attenuation attribute
        VPLight* spotlightPtr = new VPLight;
        spotlightPtr->autoDelete=true;
        VPColor* color;
        color = new VPColor;
        VPPoint4D* location;
        location = new VPPoint4D;
        
        unsigned int i;
        float intensity;
        float ambientIntensity;
        float xPos;
        float yPos;
        float zPos;
        bool enabled;
        ///Color attributes
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
        istringstream stream;

        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();

        for(i = 0; i<childNodes->getLength(); i++)
        {
            if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "intensity")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> intensity;
                spotlightPtr->SetIntensity(intensity);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "ambientIntensity")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> ambientIntensity;
                spotlightPtr->SetAmbientIntensity(ambientIntensity);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "color")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("red"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> r;
                color->SetR(r);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("green"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> g;
                color->SetG(g);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("blue"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> b;
                color->SetB(b);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("alpha"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> a;
                color->SetA(a);
                
                spotlightPtr->SetColor(*color);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "enabled")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> enabled;
                spotlightPtr->Turn(enabled);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "position")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("x"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> xPos;
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("y"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> yPos;
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("z"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> zPos;
                location->SetXYZW(xPos, yPos, zPos, 1.0);
                spotlightPtr->SetLocation(*location);
            }
        }
        spotlightPtr->SetDescription(XMLString::transcode(attrTemp->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
        result =  spotlightPtr;
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "pointlight")== 0)
    {///FixMe: The pointlight must have an attenuation attribute
        VPLight* pointlightPtr = new VPLight;
        pointlightPtr->autoDelete=true;
        VPColor* color;
        color = new VPColor;
        VPPoint4D* location;
        location = new VPPoint4D;
        
        unsigned int i;
        float intensity;
        float ambientIntensity;
        float xPos;
        float yPos;
        float zPos;
        bool enabled;
        ///Color attributes
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
        istringstream stream;

        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();

        for(i = 0; i<childNodes->getLength(); i++)
        {
            if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "intensity")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> intensity;
                pointlightPtr->SetIntensity(intensity);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "ambientIntensity")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> ambientIntensity;
                pointlightPtr->SetAmbientIntensity(ambientIntensity);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "color")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("red"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> r;
                color->SetR(r);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("green"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> g;
                color->SetG(g);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("blue"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> b;
                color->SetB(b);
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("alpha"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> a;
                color->SetA(a);
                
                pointlightPtr->SetColor(*color);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "enabled")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("value"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> enabled;
                pointlightPtr->Turn(enabled);
            }
            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "position")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("x"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> xPos;
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("y"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> yPos;
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("z"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> zPos;
                location->SetXYZW(xPos, yPos, zPos, 1.0);
                pointlightPtr->SetLocation(*location);
            }
        }
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();
        pointlightPtr->SetDescription(XMLString::transcode(attrTemp->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
        result =  pointlightPtr;
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "transform")== 0)
    {
        //The transform can be defined by a matrix that defines the translation and 
        //rotations of the object, or a scale, or only a rotation, or only a translation.
        
        
        list<VPTransform>::const_iterator iter;
        list<VPTransform> listTrans;
        istringstream stream;
        VPTransform* trans;
        trans = new VPTransform();
        trans->autoDelete=true;
        unsigned int i;
        char *charAux;
        trans->MakeIdentity();
        
        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();

        for(i = 0; i<childNodes->getLength(); i++)
        {
            
            if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "matrix")== 0)
            {
                double data[16];
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m00"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[0];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m01"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[1];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m02"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[2];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m03"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[3];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m10"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[4];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m11"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[5];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m12"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[6];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m13"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[7];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m20"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[8];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m21"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[9];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m22"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[10];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m23"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[11];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m30"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[12];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m31"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[13];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m32"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[14];

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("m33"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> data[15];

                trans->SetData(data);
                trans->SetDescription(XMLString::transcode(attrTemp->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
                result =  trans;
            }

            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "translation")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                VPPoint4D transVector;
                float x, y, z;

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("x"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> x;

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("y"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> y;

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("z"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> z;
                transVector = VPPoint4D (x,y,z);
                trans-> MakeTranslation(transVector);
                trans->SetDescription(XMLString::transcode(attrTemp->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
                result =  trans;
            }

            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "scale")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                float x, y, z;

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("x"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> x;

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("y"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> y;

                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("z"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> z;
                trans-> MakeScale(x, y, z);
                trans->SetDescription(XMLString::transcode(attrTemp->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
                result =  trans;
            }

            else if( strcmp(XMLString::transcode(childNodes->item(i)->getNodeName()), "rotation")== 0)
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                char* axis;
                double radians;

                axis = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("axis"))->getNodeValue());
                charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("radians"))->getNodeValue());

                if (*axis == 'x')
                    trans->MakeXRotation(radians);
                else if (*axis == 'y')
                    trans->MakeYRotation(radians);
                else if (*axis == 'z')
                    trans->MakeZRotation(radians);
                trans->SetDescription(XMLString::transcode(attrTemp->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
                result =  trans;
            }
        }
    }

    else if( strcmp(XMLString::transcode(listNode->item(1)->getNodeName()), "joint")== 0)
    {
        list<VPDof*> *dofs;
        list<VPDof*>::const_iterator iter;

        /// Create a Joint and its dependencies
        dofs = new list<VPDof*>;
        
        DOMNamedNodeMap* attrNode = listNode->item(1)->getAttributes();
        char *name = XMLString::transcode(attrNode->getNamedItem(XMLString::transcode("type"))->getNodeValue());
        if (strcmp(name, "biaxial")==0)
        {
            VPBiaxialJoint* newJointB;
            newJointB = new VPBiaxialJoint();
            newJointB->autoDelete = true;
            loadDofs (listNode->item(1), dofs);
            for (iter = dofs->begin(); iter!= dofs->end(); iter++)
            {
                newJointB->AddDof(*iter);
            }
            DOMNamedNodeMap* attrAux = listNode->item(1)->getAttributes();
            newJointB->SetDescription(XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
            result =  newJointB;
        }

        else if ( strcmp(name, "polyaxial")==0)
        {
            VPPolyaxialJoint* newJointB;
            newJointB = new VPPolyaxialJoint();
            newJointB->autoDelete = true;
            loadDofs (listNode->item(1), dofs);
            for (iter = dofs->begin(); iter!=dofs->end(); ++iter)
                newJointB->AddDof(*iter);
            DOMNamedNodeMap* attrAux = listNode->item(1)->getAttributes();
            newJointB->SetDescription(XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
            result = newJointB;
        }

        else if (strcmp(name, "uniaxial")==0)
        {
            VPUniaxialJoint* newJointB;
            newJointB = new VPUniaxialJoint();
            newJointB->autoDelete = true;
            DOMNamedNodeMap* attrAux = listNode->item(1)->getAttributes();
            newJointB->SetDescription(XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
            loadDofs (listNode->item(1), dofs);
            for (iter = dofs->begin(); iter!=dofs->end(); ++iter)
            {
                newJointB->AddDof(*iter);
            }
            result = newJointB;
            
        }
            delete(dofs);
    }

    // Next XML node
    currentChild += 2;
    while(currentChild < numChildren)
    {
        // FixMe: Needs to treat lights in a proper way.
        result->AddChild(*(LoadSceneNode(listNode->item(currentChild))));
        currentChild += 2;
    }
    return result;
}

void VPXmlScene::loadDofs( DOMNode* node, list<VPDof*>* dofs)
{
    list<VPDof*>::iterator iter;
    
    unsigned int i;
    unsigned int j;

    istringstream stream;

    float xPos;
    float yPos;
    float zPos;

    VPPoint4D axis;
    VPPoint4D position;

    DOMNodeList* dof = node->getChildNodes();
    
    
    char* charAux;
    for (i = 0; i< dof->getLength();i++)
    {
        
        if( strcmp(XMLString::transcode(dof->item(i)->getNodeName()), "dof")==0 )
        {
            VPDof* d = new VPDof;
            d->autoDelete=true;
            DOMNodeList* dofAux = dof->item(i)->getChildNodes();
            
            for( j = 0; j <dofAux->getLength(); j++ )
            {
                if( strcmp(XMLString::transcode(dofAux->item(j)->getNodeName()), "position")==0 )
                {
                    DOMNamedNodeMap* attrPos = dofAux->item(j)->getAttributes();
                    
                    charAux = XMLString::transcode(attrPos->getNamedItem(XMLString::transcode("x"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> xPos;
                    
                    charAux = XMLString::transcode(attrPos->getNamedItem(XMLString::transcode("y"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> yPos;
                    
                    charAux = XMLString::transcode(attrPos->getNamedItem(XMLString::transcode("z"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> zPos;
                    position = VPPoint4D (xPos,yPos,zPos);
                } 
                
                else if ( strcmp(XMLString::transcode(dofAux->item(j)->getNodeName()), "axis")==0 )
                {
                    DOMNamedNodeMap* attrAxis = dofAux->item(j)->getAttributes();
                    
                    charAux = XMLString::transcode(attrAxis->getNamedItem(XMLString::transcode("x"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> xPos;
                    
                    charAux = XMLString::transcode(attrAxis->getNamedItem(XMLString::transcode("y"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> yPos;
                    
                    charAux = XMLString::transcode(attrAxis->getNamedItem(XMLString::transcode("z"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> zPos;
                    
                    axis = VPPoint4D( xPos, yPos, zPos);
                }
                else if( strcmp(XMLString::transcode(dofAux->item(j)->getNodeName()), "range")==0 )
                {
                    DOMNamedNodeMap *attrRange = dofAux->item(j)->getAttributes();
                    
                    charAux = XMLString::transcode(attrRange->getNamedItem(XMLString::transcode("min"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> xPos;
                    
                    charAux = XMLString::transcode(attrRange->getNamedItem(XMLString::transcode("max"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> yPos;
                    
                    charAux = XMLString::transcode(attrRange->getNamedItem(XMLString::transcode("rest"))->getNodeValue());
                    stream.clear();
                    stream.str(charAux);
                    stream >> zPos;
                }
            }
            d->Set(axis, position, xPos, yPos);
            DOMNamedNodeMap* attrAux = dof->item(i)->getAttributes();
            d->SetDescription(XMLString::transcode(attrAux->getNamedItem(XMLString::transcode("description"))->getNodeValue()));
            d->SetRest(zPos);
            dofs->push_back(d);
        }
    }
}

VPXmlScene::~VPXmlScene()
{
    //cout<<"VPXmlScene::~VPXmlScene()"<<endl;
}

//---------------------------Error methods---------------------------

VPXmlScene::ErrorHandler::ErrorHandler() :

    fSawErrors(false)
{
}

VPXmlScene::ErrorHandler::~ErrorHandler()
{
}

//Overrides of the SAX ErrorHandler interface
void VPXmlScene::ErrorHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nError at file " << VPXmlScene::StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << VPXmlScene::StrX(e.getMessage()) << endl;
}

void VPXmlScene::ErrorHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nFatal Error at file " << VPXmlScene::StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << VPXmlScene::StrX(e.getMessage()) << endl;
}

void VPXmlScene::ErrorHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    cerr << "\nWarning at file '" << VPXmlScene::StrX(e.getSystemId())
         << "', line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << VPXmlScene::StrX(e.getMessage()) << endl;
}

void VPXmlScene::ErrorHandler::resetErrors()
{
    fSawErrors = false;
}

bool VPXmlScene::ErrorHandler::getSawErrors() const
{
    return fSawErrors;
}
