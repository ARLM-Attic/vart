/// \file vpxmlaction.cpp
/// \brief Implementation file for VPXml VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog

//Apr 4, 2006 - Andreia Schneider
// - Methods declaration.


#include "vpxmlaction.h"

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

VPXmlAction::VPXmlAction()
{
}

bool VPXmlAction::LoadFromFile(char* filename, VPXmlScene* scene)
{
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
            << VPXmlAction::StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
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
        LoadAnimation(doc, scene);
        //...delete the parser itself (Must be done prior to calling Terminate, below),
        delete parser;
        //...call the termination method
        XMLPlatformUtils::Terminate();
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

bool VPXmlAction::ParserFile(char* filename, XercesDOMParser *parser)
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
             << VPXmlAction::StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
        return false;
    }
    catch (const DOMException& e)
    {
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << filename << "'\n"
             << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;
        if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
             XERCES_STD_QUALIFIER cerr << "Message is: " << VPXmlAction::StrX(errText) << XERCES_STD_QUALIFIER endl;
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

void VPXmlAction::LoadAnimation(DOMDocument* doc, VPXmlScene* scene)
{
    istringstream stream;
    char* charAux;
    unsigned int i, j, k;
    float inTime, fTime, fPos, dur, speed;
    string dofIdentifier, jName, interp;
    bool cycle;

    DOMNodeList* actionList = doc->getElementsByTagName(XMLString::transcode("action"));

    DOMElement* rootNode = doc-> getDocumentElement ();

    DOMNamedNodeMap* attrAc = rootNode->getAttributes();
    
    charAux = XMLString::transcode(attrAc->getNamedItem(XMLString::transcode("speed"))->getNodeValue());
    stream.clear();
    stream.str(charAux);
    stream >> speed;
    
    if (strcmp(XMLString::transcode(attrAc->getNamedItem(XMLString::transcode("cycle"))->getNodeValue()), "true")== 0)
        cycle = true;
    else
        cycle = false;
    
    Set(speed, 0, cycle); //Priority default is 0.
    for (i = 0; i<actionList->getLength(); i++)
    {
        DOMNodeList* jointsL = actionList->item(i)->getChildNodes();

        for(j = 0; j<jointsL->getLength(); j++)
        {
            if(strcmp(XMLString::transcode(jointsL->item(j)->getNodeName()), "joint_movement")== 0)
            {

                DOMNamedNodeMap* attrLt = jointsL->item(j)->getAttributes();
                
                charAux = XMLString::transcode(attrLt->getNamedItem(XMLString::transcode("duration"))->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> dur;

                jName = XMLString::transcode(attrLt->getNamedItem(XMLString::transcode("joint_name"))->getNodeValue());

                VPJoint* jointPtr = dynamic_cast<VPJoint*>(scene->GetObjectRec(jName));
                interp = XMLString::transcode(attrLt->getNamedItem(XMLString::transcode("interpolation"))->getNodeValue());
                VPInterpolator* interpPtr;
                if (interp == "seno padrao")
                {
                    interpPtr = new VPSineInterpolator;
                }
                else 
                {
                    if (interp == "linear")
                        interpPtr = new VPLinearInterpolator;
                    else
                    {
                        if (interp == "seno com limites")
                        {
                            VPRangeSineInterpolator *intRangePtr;
                            intRangePtr = new VPRangeSineInterpolator;
                            interpPtr = dynamic_cast<VPInterpolator*> (intRangePtr);
                        }
                    }
                }


                VPJointMover* jointMPtr;
                jointMPtr = AddJointMover(jointPtr, dur, *interpPtr);

                DOMNodeList* dofsL = jointsL->item(j)->getChildNodes();

                for(k = 0; k<dofsL->getLength(); k++)
                {
                    if(strcmp(XMLString::transcode(dofsL->item(k)->getNodeName()), "dof_movement")== 0)
                    {
                        DOMNamedNodeMap* attrPt = dofsL->item(k)->getAttributes();

                        charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("initialTime"))->getNodeValue());
                        stream.clear();
                        stream.str(charAux);
                        stream >> inTime;

                        charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("finalTime"))->getNodeValue());
                        stream.clear();
                        stream.str(charAux);
                        stream >> fTime;

                        charAux = XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("finalPosition"))->getNodeValue());
                        stream.clear();
                        stream.str(charAux);
                        stream >> fPos;
                        if (strcmp((XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("dofID"))->getNodeValue())), "FLEX")== 0)
                        {
                            jointMPtr->AddDofMover(VPJoint::FLEXION, inTime, fTime, fPos);
                        }
                        else if (strcmp((XMLString::transcode(attrPt->getNamedItem(XMLString::transcode("dofID"))->getNodeValue())), "ADDUCT")== 0)
                        {
                            jointMPtr->AddDofMover(VPJoint::ADDUCTION, inTime, fTime, fPos);
                        }
                        else
                        {
                            jointMPtr->AddDofMover(VPJoint::TWIST, inTime, fTime, fPos);
                        }
                    }
                }
            }
        }
    }
}

VPXmlAction::~VPXmlAction()
{
}

//---------------------------Error methods---------------------------

VPXmlAction::ErrorHandler::ErrorHandler() :

    fSawErrors(false)
{
}

VPXmlAction::ErrorHandler::~ErrorHandler()
{
}

//Overrides of the SAX ErrorHandler interface
void VPXmlAction::ErrorHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nError at file " << VPXmlAction::StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << VPXmlAction::StrX(e.getMessage()) << endl;
}

void VPXmlAction::ErrorHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nFatal Error at file " << VPXmlAction::StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << VPXmlAction::StrX(e.getMessage()) << endl;
}

void VPXmlAction::ErrorHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    cerr << "\nWarning at file '" << VPXmlAction::StrX(e.getSystemId())
         << "', line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << VPXmlAction::StrX(e.getMessage()) << endl;
}

void VPXmlAction::ErrorHandler::resetErrors()
{
    fSawErrors = false;
}

bool VPXmlAction::ErrorHandler::getSawErrors() const
{
    return fSawErrors;
}
