/// \file vpxmlaction.h
/// \brief Header file for VPXml VPAT class.
///  \version $Revision: 1.1 $

#ifndef __VPXMLACTION_H
#define __VPXMLACTION_H

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <sax/SAXParseException.hpp>

#include <iostream>
#include <list>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include <vpscene.h>
#include <vppoint4d.h>
#include <vpdof.h>
#include <vpgraphicobj.h>
#include <vpcylinder.h>
#include <vpjoint.h>
#include <vplinearinterpolator.h>
#include <vpsineinterpolator.h>
#include <vprangesineinterpolator.h>
#include <vpcosinterpolator.h>
#include <vpdofmover.h>
#include <vpjointmover.h>
#include <vpscenenode.h>
#include <vpcolor.h>
#include <vpsphere.h>
#include <vpmeshobject.h>
#include <vpaction.h>
#include <vpxmlscene.h>

/// \class VPXmlAction vpxmlaction.h
/// \brief Parser and loader for xml files that describe an action

class VPXmlAction : public VPAction
{
    public:
        class ErrorHandler : public XERCES_CPP_NAMESPACE::ErrorHandler
        {
            friend class VPXmlAction;
            public:
                ErrorHandler();
                ~ErrorHandler();
                bool getSawErrors() const;
                void warning(const XERCES_CPP_NAMESPACE::SAXParseException& e);
                void error(const XERCES_CPP_NAMESPACE::SAXParseException& e);
                void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& e);
                void resetErrors();

            private :
                ErrorHandler(const ErrorHandler&);
                void operator=(const ErrorHandler&);
                //This is set if we get any errors, and is queryable via a getter
                //method. Its used by the main code to suppress output if there are
                //errors.
                bool fSawErrors;
        };

        class StrX
        {
            friend class VPXmlAction;
            public :
                StrX(const XMLCh* const toTranscode)
                {
                    // Call the private transcoding method
                    fLocalForm = XERCES_CPP_NAMESPACE::XMLString::transcode(toTranscode);
                }

                ~StrX()
                {
                    delete [] fLocalForm;
                }

                const char* localForm() const
                {
                    return fLocalForm;
                }

            private :
                //This is the local code page form of the string.
                char*   fLocalForm;
        };

        VPXmlAction();
        ~VPXmlAction();
        bool LoadFromFile(char* filename, VPXmlScene* scene);
        bool ParserFile(char* filename, XERCES_CPP_NAMESPACE::XercesDOMParser *parser);
        void LoadAnimation(XERCES_CPP_NAMESPACE::DOMDocument* doc, VPXmlScene* scene);
    private:
};

inline std::ostream& operator<<(std::ostream& target, const VPXmlAction::StrX& toDump)
{
    target << toDump.localForm();
    return target;
}
#endif
