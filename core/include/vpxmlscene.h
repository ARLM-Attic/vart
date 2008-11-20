/// \file vpxmlscene.h
/// \brief Header file for VPXml VPAT class.
/// \version $Revision: 1.3 $

#ifndef __VPXML_H
#define __VPXML_H

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
#include <vpuniaxialjoint.h>
#include <vpbiaxialjoint.h>
#include <vppolyaxialjoint.h>
#include <vptransform.h>
#include <vpscenenode.h>
#include <vpcolor.h>
#include <vpsphere.h>
#include <vpmeshobject.h>

typedef std::map<std::string, VPMeshObject*> meshObjMap;
typedef std::map<std::string, meshObjMap> meshMap;

class VPXmlScene:public VPScene
{
    public:
        class ErrorHandler : public XERCES_CPP_NAMESPACE::ErrorHandler
        {
            friend class VPXmlScene;
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
                bool    fSawErrors;
        };

        class StrX
        {
            friend class VPXmlScene;
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

        VPXmlScene();
        ~VPXmlScene();
        /// \Parses the xml file. If it dont't have errors, load scene.
        bool LoadFromFile(char* filename);
        bool ParserFile(char* filename, XERCES_CPP_NAMESPACE::XercesDOMParser *parser);
        /// \Load the scene based in xml archieve.
        void LoadScene(XERCES_CPP_NAMESPACE::DOMDocument* doc);
        /// \Load the nodes (transformations, geometry, etc.) of the scene.
        VPSceneNode* LoadSceneNode(XERCES_CPP_NAMESPACE::DOMNode* sceneList);
        /// \Load MeshObjects from file.
        VPMeshObject* LoadMeshFromFile(std::string filen, std::string type, std::string meshName);
        /// \Load the dofs of the joint.
        void loadDofs( XERCES_CPP_NAMESPACE::DOMNode* node, std::list<VPDof*>* dofs);

    private:
        meshObjMap mapMeshObj;
        meshMap mapMesh;
};

inline std::ostream& operator<<(std::ostream& target, const VPXmlScene::StrX& toDump)
{
    target << toDump.localForm();
    return target;
}


#endif
