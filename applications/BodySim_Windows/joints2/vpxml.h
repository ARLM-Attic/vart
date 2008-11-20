///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpxml.h
//  DESCRIPTION.: Contain the VPXml class declarations.
//
//  AUTHOR......: Anderson Maciel
//  DATE........: March/01/2001
//  DESCRIPTION.: Class and new methods declaration.
//
///////////////////////////////////////////////////////////////////

#ifndef __VPXML_H
#define __VPXML_H

#include <DOM.hpp>
//#include <xercesc/dom/DOM_Element.hpp>
#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>
#include <sax/SAXParseException.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_DOMException.hpp>


#include	<vpscene.h>
#include	<vpbody.h>

//using namespace XERCES_CPP_NAMESPACE;
using XERCES_CPP_NAMESPACE::DOM_NodeList;

class VPXml {

	private:
		VPScene			*scene;
		VPBody			*body;
		XERCES_CPP_NAMESPACE::DOM_Element	     root;
		VPDof				**auxVec; /// Temporary vector of DOF pointers to every dof in the tree
		int					iIndDofAux; /// Index for the above vector.

	protected:		
		VPJoint*			loadJoint(XERCES_CPP_NAMESPACE::DOM_Node node, VPJoint *parent );
		VPDof*				loadDofs(DOM_NodeList list );
		VPGraphicObj*	loadShapes(DOM_NodeList list, int *num );
		void					loadModifiers( DOM_NodeList joints );
		
	public:
		VPXml();
		~VPXml();
		
		void					vpLoadBodyFile( char *fileName );		
		VPScene*			vpGetXmlScene( void );
		VPBody*				vpGetXmlBody( void );
};


#include <sax/ErrorHandler.hpp>
#include <iostream.h>

class SAXParseException;


// ---------------------------------------------------------------------------
//  Simple error handler deriviative to install on parser
// ---------------------------------------------------------------------------
class DOMCountErrorHandler : public XERCES_CPP_NAMESPACE::ErrorHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DOMCountErrorHandler();
    ~DOMCountErrorHandler();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const;


    // -----------------------------------------------------------------------
    //  Implementation of the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const XERCES_CPP_NAMESPACE::SAXParseException& e);
    void error(const XERCES_CPP_NAMESPACE::SAXParseException& e);
    void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& e);
    void resetErrors();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMCountErrorHandler(const DOMCountErrorHandler&);
    void operator=(const DOMCountErrorHandler&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};


// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
    {
        // Call the private transcoding method
        fLocalForm = XERCES_CPP_NAMESPACE::XMLString::transcode(toTranscode);
    }

    ~StrX()
    {
        delete [] fLocalForm;
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* localForm() const
    {
        return fLocalForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocalForm
    //      This is the local code page form of the string.
    // -----------------------------------------------------------------------
    char*   fLocalForm;
};

inline ostream& operator<<(ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

inline bool DOMCountErrorHandler::getSawErrors() const
{
    return fSawErrors;
}


#endif
