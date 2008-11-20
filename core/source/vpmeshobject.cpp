/// \file vpmeshobject.cpp
/// \brief Implementation file for VPMeshObject VPAT class.
/// \version 1.10

// ChangeLog
// May 18, 2006 - Bruno de Oliveira Schneider
// - DrawInstanceOGL now checks if it should draw the recursive bounding box.
// - Removed references to bBoxShow.
// Feb 16, 2006 - Andreia Schneider
// - DrawInstanceOGL modified to allow the bounding box to be drawn.
// Jan 27, 2006 - Bruno de Oliveira Schneider
// - Fixed some bugs in ReadFromOBJ.
// - Added sizeOfNormals class attribute.
// - Added GetVerticesCoordinates(), GetVertex() and SetVertex().
// Jun 22, 2005 - Bruno de Oliveira Schneider
// - Fixed and enhanced documentation.
// Jun 13, 2005 - Bruno de Oliveira Schneider
// - Moved code to enable vertex arrays from constructor into vpviewerglutogl.
// - Added IsEmpty.
// - DrawInstanceOGL now respects the "show" attribute.
// Jan 31, 2005 - Bruno de Oliveira Schneider
// - Added faceCounter to ReadFromOBJ.
// Dec 16, 2004 - Bruno de Oliveira Schneider
// - Added Clear, MakeBox, GetYProjection, SetMaterial, DrawInstanceOGL and GetID.
// - Attribute vertVec was a vector of pointer to points and is now a vector of
//   points. Work with the ConvexHull class showed that it would be better if a
//   MeshObject could manage its own storage. Accordingly, normVec had its type
//   changed.
// - Fixed DrawInstanceOGL(), so that it now works with unoptimized meshes.
// - Added SetVertices, AddFace and ComputeBoundingBox.
// - Added Transform, MergeWith, ReadFromOBJ and ReadMaterialTable.
// - Added a copy constructor and operator=.
// Sep 22, 2004 - Bruno de Oliveira Schneider
// - File created.

#include "vpmeshobject.h"
#include <sstream>
#include <cassert>
#include <fstream>

using namespace std;

float VPMeshObject::sizeOfNormals = 0.1f;

VPMeshObject::VPMeshObject()
{
    howToShow = FILLED;
}

VPMeshObject::VPMeshObject(const VPMeshObject& obj)
{
    vertVec = obj.vertVec;
    vertCoordVec = obj.vertCoordVec;
    normVec = obj.normVec;
    normCoordVec = obj.normCoordVec;
    textCoordVec = obj.textCoordVec;
    meshList = obj.meshList;
}

VPMeshObject& VPMeshObject::operator=(const VPMeshObject& obj)
{
    vertVec = obj.vertVec;
    vertCoordVec = obj.vertCoordVec;
    normVec = obj.normVec;
    normCoordVec = obj.normCoordVec;
    textCoordVec = obj.textCoordVec;
    meshList = obj.meshList;
    return *this;
}

void VPMeshObject::Clear()
{
    vertVec.clear();
    vertCoordVec.clear();
    normVec.clear();
    normCoordVec.clear();
    textCoordVec.clear();
    meshList.clear();
}

void VPMeshObject::SetMaterial(const VPMaterial& mat)
{
    list<VPMesh>::iterator iter;
    for (iter = meshList.begin(); iter != meshList.end(); ++iter)
        iter->material = mat;
}

void VPMeshObject::SetVertices(const std::vector<VPPoint4D>& vertexVec)
{
    vertVec = vertexVec;
    meshList.clear();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VPMeshObject::SetVertices(const char* vertexStr)
{
    string valStr = vertexStr;
    istringstream iss(valStr);
    double x,y,z,w;
    bool notFinished = true;
    VPPoint4D point;

    vertVec.clear();
    do {
        if (!(iss >> x >> y >> z)) // Try to read 3 values
            notFinished = false; // signal end of parsing
        else
        {
            if (!(iss >> w)) // try to read 4th value
            {
                w = 1.0; // use default value
                iss.clear(); // erase error flags
            }
            point.SetXYZW(x,y,z,w);
            vertVec.push_back(point);
            iss >> ws; // skip possible white space before comma
            iss.get(); // skip the comma
        }
    } while (notFinished);
    meshList.clear();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VPMeshObject::SetVertex(unsigned int index, const VPPoint4D& newValue)
{
    if (vertVec.empty())
    {
        unsigned int newIndex = index*3;
        vertCoordVec[newIndex] = newValue.GetX();
        vertCoordVec[newIndex+1] = newValue.GetY();
        vertCoordVec[newIndex+2] = newValue.GetZ();
    }
    else
    { // vertVec is not empty
        vertVec[index] = newValue;
    }
}

VPPoint4D VPMeshObject::GetVertex(unsigned int pos)
{
    if (vertVec.empty())
    {
        unsigned int newPos = pos*3;
        return VPPoint4D(vertCoordVec[newPos],vertCoordVec[newPos+1],vertCoordVec[newPos+2]);
    }
    else
    { // vertVec is not empty
        return vertVec[pos];

    }
}

VPPoint4D VPMeshObject::GetVertexMedia()
{
    VPPoint4D mean = VPPoint4D(0,0,0,1);
    double size = 0;

    size = vertCoordVec.size();

    for( int i=0; i < size ; i+=3)
    {
        mean += VPPoint4D( vertCoordVec[i], vertCoordVec[i+1], vertCoordVec[i+2]);
    }

    if( size == 0 )
        return VPPoint4D(0,0,0,1);
    else
        mean = mean / (size/3);

    return mean;
}

void VPMeshObject::AddFace(const char* indexStr)
{
    string valStr = indexStr;
    istringstream iss(valStr);
    unsigned int value;
    unsigned int thisFacesNormalIndex = normVec.size();
    VPMesh mesh;

    mesh.type = VPMesh::POLYGON;
    while (iss >> value)
    {
        mesh.indexVec.push_back(value);
        mesh.normIndVec.push_back(thisFacesNormalIndex);
    }
    meshList.push_back(mesh);

    // Auto computation of face normal
    // FixMe: It should be possible to disable auto computation
    VPPoint4D v1 = vertVec[mesh.indexVec[1]] - vertVec[mesh.indexVec[0]];
    VPPoint4D v2 = vertVec[mesh.indexVec[2]] - vertVec[mesh.indexVec[1]];
    v1.Normalize();
    v2.Normalize();
    VPPoint4D normal = v1.CrossProduct(v2);
    normVec.push_back(normal);
}

void VPMeshObject::MakeBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
    assert((minX <= maxX) && (minY <= maxY) && (minZ <= maxZ));
    // each vertex must repeat 3 times because there must be a vertex/normal correspondence
    // of 1:1
    double coordinateArray[] = { minX,minY,minZ, //0
                                 minX,maxY,minZ, //1
                                 maxX,maxY,minZ, //2
                                 maxX,minY,minZ, //3
                                 minX,minY,maxZ, //4
                                 minX,maxY,maxZ, //5
                                 maxX,maxY,maxZ, //6
                                 maxX,minY,maxZ, //7
                                 minX,minY,minZ,
                                 minX,maxY,minZ,
                                 maxX,maxY,minZ,
                                 maxX,minY,minZ,
                                 minX,minY,maxZ,
                                 minX,maxY,maxZ,
                                 maxX,maxY,maxZ,
                                 maxX,minY,maxZ,
                                 minX,minY,minZ,
                                 minX,maxY,minZ,
                                 maxX,maxY,minZ,
                                 maxX,minY,minZ,
                                 minX,minY,maxZ,
                                 minX,maxY,maxZ,
                                 maxX,maxY,maxZ,
                                 maxX,minY,maxZ };
    double* endOfCoordinateArray = coordinateArray + sizeof(coordinateArray)/sizeof(double);
    unsigned int indexArray[] = { 0,1,2,3, // back face
                                  4,7,6,5, // front face
                                  11,10,14,15, // right face
                                  13,9,8,12, // left face
                                  22,18,17,21, // top face
                                  23,20,16,19 }; // bottom face
    double normalArray[] = { 0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1,
                             0,0,1, 0,0,1, 0,0,1, 0,0,1,
                             -1,0,0, -1,0,0, 1,0,0, 1,0,0,
                             -1,0,0, -1,0,0, 1,0,0, 1,0,0,
                             0,-1,0, 0,1,0, 0,1,0, 0,-1,0,
                             0,-1,0, 0,1,0, 0,1,0, 0,-1,0 };
    double* endOfNormalArray = normalArray + sizeof(normalArray)/sizeof(double);
    unsigned int* endOfIndexArray = indexArray + sizeof(indexArray)/sizeof(int);
    VPMesh mesh;

    normCoordVec.clear();
    textCoordVec.clear();
    meshList.clear();
    vertCoordVec.assign(coordinateArray,endOfCoordinateArray);
    normCoordVec.assign(normalArray,endOfNormalArray);
    mesh.type = VPMesh::QUADS;
    mesh.indexVec.assign(indexArray,endOfIndexArray);
    mesh.material = VPMaterial::DARK_PLASTIC_GRAY(); // default material
    meshList.push_back(mesh);
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VPMeshObject::GetYProjection(std::list<VPPoint4D>* resultPtr, double height) const
// height defaults to 0 (see headers).
{
    VPPoint4D point;
    resultPtr->clear();
    if (vertCoordVec.size() > 6)
    { // Found optimized data - use it
        unsigned int vertexVetEnd = vertCoordVec.size();
        for (unsigned int i=0; i < vertexVetEnd; i+=3)
        {
            point.SetXYZW(vertCoordVec[i],height,vertCoordVec[i+2],1);
            resultPtr->push_back(point);
        }
    }
    else
    { // Use unoptimized data
        unsigned int vertexVetEnd = vertVec.size();
        for (unsigned int i=0; i < vertexVetEnd; ++i)
        {
            point.SetXYZW(vertVec[i].GetX(), height, vertVec[i].GetZ(), 1);
            resultPtr->push_back(point);
        }
    }
}

void VPMeshObject::Optimize()
{
    // Create optmized structures from unoptimized ones
    // Erase unoptimized data
}

void VPMeshObject::ComputeBoundingBox() {
    if (vertCoordVec.size() > 0)
    { // Optimized structure found - use it!
        // Initialize
        bBox.SetBoundingBox(vertCoordVec[0], vertCoordVec[1], vertCoordVec[2],
                            vertCoordVec[0], vertCoordVec[1], vertCoordVec[2]);
        // Check against the others
        for (unsigned int i=3; i < vertCoordVec.size(); i+=3)
            bBox.ConditionalUpdate(vertCoordVec[i], vertCoordVec[i+1], vertCoordVec[i+2]);
    }
    else
    { // No optmized structure found - use vertVec
        // Initialize
        bBox.SetBoundingBox(vertVec[0].GetX(), vertVec[0].GetY(), vertVec[0].GetZ(),
                            vertVec[0].GetX(), vertVec[0].GetY(), vertVec[0].GetZ());
        // Check against the others
        for (unsigned int i=1; i < vertVec.size(); ++i)
            bBox.ConditionalUpdate(vertVec[i]);
    }
}

void VPMeshObject::MergeWith(const VPMeshObject& obj) {
// both meshObjects must be optimized or the both must be unoptimized
    bool bothOptimized = vertVec.empty() && obj.vertVec.empty();
    list<VPMesh>::const_iterator iter = obj.meshList.begin();
    VPMesh mesh;
    unsigned int prevNumVertices;
    assert (bothOptimized || (vertCoordVec.empty() && obj.vertCoordVec.empty()));

    prevNumVertices = (bothOptimized? (vertCoordVec.size()/3) : vertVec.size());
    for (; iter != obj.meshList.end(); ++iter)
    {
        mesh = *iter;
        mesh.IncrementIndices(prevNumVertices);
        meshList.push_back(mesh);
    }

    vertVec.insert(vertVec.end(), obj.vertVec.begin(), obj.vertVec.end());
    vertCoordVec.insert(vertCoordVec.end(), obj.vertCoordVec.begin(), obj.vertCoordVec.end());
    normVec.insert(normVec.end(), obj.normVec.begin(), obj.normVec.end());
    normCoordVec.insert(normCoordVec.end(), obj.normCoordVec.begin(), obj.normCoordVec.end());
    textCoordVec.insert(textCoordVec.end(), obj.textCoordVec.begin(), obj.textCoordVec.end());
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VPMeshObject::Transform(const VPTransform& trans) {
    unsigned int i = 0;
    unsigned int size;

    if (vertCoordVec.empty())
    {
        for (size = vertVec.size(); i < size; ++i)
        {
            trans.ApplyTo(&(vertVec[i]));
        }
    }
    else
    {
        VPPoint4D vertex;
        for (size = vertCoordVec.size(); i < size; i+=3)
        {
            vertex.SetXYZW(vertCoordVec[i],vertCoordVec[i+1],vertCoordVec[i+2],1);
            trans.ApplyTo(&vertex);
            vertCoordVec[i] = vertex.GetX();
            vertCoordVec[i+1] = vertex.GetY();
            vertCoordVec[i+2] = vertex.GetZ();
        }
    }
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VPMeshObject::ComputeTriangleNormal(const VPPoint4D& v1, const VPPoint4D& v2,
                                         const VPPoint4D& v3, VPPoint4D* resultPtr) {
    VPPoint4D edge1 = v2 - v1;
    VPPoint4D edge2 = v3 - v2;
    // FixMe: Tent normals seem to be inverted (note minus operator below)
    // investigate and solve...
    *resultPtr = -edge1.CrossProduct(edge2);
    resultPtr->Normalize();
}

bool VPMeshObject::DrawInstanceOGL() const {
// Note that vertex arrays must be enabled to allow drawing of optimized meshes. See
// VPViewerGlutOGL.
#ifdef VP_OGL
    bool result = true;
    list<VPMesh>::const_iterator iter;
    if (show) // if visible...
    {         // FixMe: no need to keep this old name; rename "show" to "visible".
        switch (howToShow)
        {
            case LINES:
            case LINES_AND_NORMALS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case POINTS:
            case POINTS_AND_NORMALS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                break;
            default:
                glPolygonMode(GL_FRONT, GL_FILL);
                break;
        }
        if (vertCoordVec.size() > 0)
        { // Optimized structure found - draw it!
            glVertexPointer(3, GL_DOUBLE, 0, &vertCoordVec[0]);
            glNormalPointer(GL_DOUBLE, 0, &normCoordVec[0]);
            assert(vertCoordVec.size() == normCoordVec.size());
            if ((howToShow == LINES_AND_NORMALS) || (howToShow == POINTS_AND_NORMALS))
            { // Draw normals
                unsigned int numVertices = vertCoordVec.size() / 3;
                unsigned int index = 0;
                glBegin(GL_LINES);
                for (unsigned int i=0; i < numVertices; ++i)
                {
                    glVertex3dv(&vertCoordVec[index]);
                    glVertex3d(vertCoordVec[index] + sizeOfNormals*normCoordVec[index],
                               vertCoordVec[index+1]+sizeOfNormals*normCoordVec[index+1],
                               vertCoordVec[index+2]+sizeOfNormals*normCoordVec[index+2]);
                    index += 3;
                }
                glEnd();
            }
            for (iter = meshList.begin(); iter != meshList.end(); ++iter)
            { // for each mesh:
                result &= iter->DrawInstanceOGL();
            }
        }
        else
        { // No optmized structure found - draw vertices from vertVec
            unsigned int meshSize;
            unsigned int i;
            for (iter = meshList.begin(); iter != meshList.end(); ++iter)
            { // for each mesh:
                iter->material.DrawOGL();
                glBegin(iter->GetOglType());
                meshSize = iter->indexVec.size();
                assert(meshSize == iter->normIndVec.size());
                for (i = 0; i < meshSize; ++i)
                {
                    glNormal3dv(normVec[iter->normIndVec[i]].VetXYZW());
                    glVertex4dv(vertVec[iter->indexVec[i]].VetXYZW());
                }
                glEnd();
            }
        }
        if(bBox.visible)
            bBox.DrawInstanceOGL();
        if(recBBox.visible)
            recBBox.DrawInstanceOGL();
    }
    return result;
#else
    return false;
#endif
}

void VPMeshObject::ReadFromOBJ(const string& filename, list<VPMeshObject*>* resultPtr)
// passing garbage on *resultPtr makes the method crash. Remember to clean it before calling.
// FixMe: This method currently handles only triangle meshes!
{
    ifstream file(filename.c_str());
    istringstream iss;
    string line;
    string lineID;
    string name;
    string materialName;
    double x,y,z;
    map<string,VPMaterial> materialMap;
    vector<double> vertCoordTempVec;
    VPMeshObject* meshObjectPtr = NULL;
    VPMesh mesh;
    char slash;
    bool noVertexMemoryAllocated = true;
    unsigned int vi, ti, ni; // vertex, texture and normal indices
    unsigned int lastNormIndex = 1;
    unsigned int lastVertIndex = 1;
    unsigned int coordIndex, tempIndex;
    unsigned int faceCounter = 0; // counts the number of faces in the file
    unsigned int lineNumber = 0;

    cout << "Loading " << filename << "..." << flush;
    mesh.type = VPMesh::TRIANGLES;
    while (getline(file,line))
    {
        iss.clear();
        iss.str(line); // iss <- line
        iss >> lineID;
        ++lineNumber;
        if (lineID == "v") // vertex
        {
            iss >> x >> y >> z;
            vertCoordTempVec.push_back(x);
            vertCoordTempVec.push_back(y);
            vertCoordTempVec.push_back(z);
        }
        else if (lineID == "vn") // vertex normal
        {
            iss >> x >> y >> z;
            meshObjectPtr->normCoordVec.push_back(x);
            meshObjectPtr->normCoordVec.push_back(y);
            meshObjectPtr->normCoordVec.push_back(z);
        }
        else if (lineID == "f") // face
        {
            if (noVertexMemoryAllocated)
            {
                noVertexMemoryAllocated = false;
                meshObjectPtr->vertCoordVec.assign(meshObjectPtr->normCoordVec.size(),0);
            }
            while (iss >> vi >> slash >> ti >> slash >> ni)
            {
                // find where in vertCoordTempVec are the vertex coordinates
                coordIndex = 3 * (ni - lastNormIndex);
                tempIndex = 3 * (vi - lastVertIndex);
                // copy coordinates to the mesh object
                meshObjectPtr->vertCoordVec[coordIndex++] = vertCoordTempVec[tempIndex];
                ++tempIndex;
                meshObjectPtr->vertCoordVec[coordIndex++] = vertCoordTempVec[tempIndex];
                ++tempIndex;
                meshObjectPtr->vertCoordVec[coordIndex]   = vertCoordTempVec[tempIndex];
                mesh.indexVec.push_back(ni - lastNormIndex);
            }
            ++faceCounter;
        }
        else if (lineID == "vt") // texture coordinates
        {
        }
        else if (lineID == "mtllib") // material library
        {
            iss >> ws >> name;
            ReadMaterialTable(name, &materialMap);
        }
        else if (lineID == "o") // object delimiter
        {
            // Add last mesh to last meshObject
            if (mesh.indexVec.size() > 0)
            {
                meshObjectPtr->meshList.push_back(mesh);
                lastNormIndex += (meshObjectPtr->normCoordVec.size()/3);
                lastVertIndex += (vertCoordTempVec.size()/3);
                mesh.indexVec.clear();
                vertCoordTempVec.clear(); // it seems that clear() does not free memory
                                          // therefore this should not slow down the method.
                noVertexMemoryAllocated = true;
            }

            iss >> ws >> name;
            meshObjectPtr = new VPMeshObject;
            meshObjectPtr->autoDelete = true;
            meshObjectPtr->SetDescription(name);
            resultPtr->push_back(meshObjectPtr);
        }
        else if (lineID == "usemtl") // material assignment
        { // start new mesh
            // add old mesh to meshObject
            if (mesh.indexVec.size() > 0)
            {
                meshObjectPtr->meshList.push_back(mesh);
                mesh.indexVec.clear();
                mesh.normIndVec.clear();
            }

            iss >> ws >> materialName;
            mesh.material = materialMap[materialName];
        }
        else if (lineID[0] == '#')
        { // ignore this line (comment)
        }
        else if (lineID == "g") // group names
        { // ignore this line (no groups in VPAT yet!)
        }
        else
            cerr << "Error in '" << filename << "', line " << lineNumber << ": unknown ID '"
                 << lineID << "'" << endl;
    }
    // Finished. Add last mesh to last meshObject
    if (mesh.indexVec.size() > 0)
    {
        meshObjectPtr->meshList.push_back(mesh);
    }
    // Compute bounding boxes
    list<VPMeshObject*>::iterator iter;
    for (iter = resultPtr->begin(); iter != resultPtr->end(); ++iter)
    {
        (*iter)->ComputeBoundingBox();
        (*iter)->ComputeRecursiveBoundingBox();
    }
    cout << " (" << faceCounter << " polygons)" << endl; // for "loading filename..."
}

//~ void VPMeshObject::YSplit(double y, VPMeshObject* ptObjBelow, VPMeshObject* ptObjAbove) const
//~ {
    //~ list<VPMesh>::iterator iter;
    //~ for (iter = meshList.begin(); iter != meshList.end(); ++iter)
    //~ {
        //~ switch (iter->type)
        //~ { // FixMe: implement more mesh types!
            //~ case POINTS:
                //~ break;
            //~ case LINES:
                //~ break;
            //~ case LINE_STRIP:
                //~ break;
            //~ case LINE_LOOP:
                //~ break;
            //~ case TRIANGLES:
                //~ break;
            //~ case TRIANGLE_STRIP:
                //~ break;
            //~ case TRIANGLE_FAN:
                //~ break;
            //~ case QUADS:
                //~ break;
            //~ case QUAD_STRIP:
                //~ break;
            //~ case POLYGON:
                //~ break;
            //~ default:
                //~ assert (0 && "VPMeshObject::YSplit: mesh type not implemented!");
        //~ }
    //~ }
//~ }

void VPMeshObject::ReadMaterialTable(const string& filename, map<string,VPMaterial>* matMapPtr)
// Reads a Wavefront material table (.mtl file)
{
    ifstream file(filename.c_str());
    istringstream iss;
    string line;
    string lineID;
    string materialName;
    VPMaterial material;
    float r,g,b;
    float value;
    unsigned int type;
    unsigned int lineNumber = 0;

    material.SetEmissiveColor(VPColor::BLACK());
    while (getline(file,line))
    {
        iss.str(line); // iss <- line
        iss >> lineID;
        ++lineNumber;
        if (lineID == "newmtl")
        {
            if (materialName.size() > 0)
                matMapPtr->insert(make_pair(materialName,material));
            iss >> ws >> materialName;
        }
        else if (lineID == "Ns")
        {
            iss >> value;
            // shininess values appear to be in the range 0..1000, so they should be
            // multiplied by 0.128
            material.SetShininess(value*0.128);
        }
        else if (lineID == "Kd")
        {
            iss >> r >> g >> b;
            material.SetDiffuseColor(VPColor(static_cast<unsigned char>(r*255),
                                             static_cast<unsigned char>(g*255),
                                             static_cast<unsigned char>(b*255)));
        }
        else if (lineID == "Ka")
        {
            iss >> r >> g >> b;
            material.SetAmbientColor(VPColor(static_cast<unsigned char>(r*255),
                                             static_cast<unsigned char>(g*255),
                                             static_cast<unsigned char>(b*255)));
        }
        else if (lineID == "Ks")
        {
            iss >> r >> g >> b;
            material.SetSpecularColor(VPColor(static_cast<unsigned char>(r*255),
                                              static_cast<unsigned char>(g*255),
                                              static_cast<unsigned char>(b*255)));
        }
        else if (lineID == "Ke")
        {
            iss >> r >> g >> b;
            material.SetEmissiveColor(VPColor(static_cast<unsigned char>(r*255),
                                              static_cast<unsigned char>(g*255),
                                              static_cast<unsigned char>(b*255)));
        }
        else if ((lineID == "d") || (lineID == "Tr"))
        {
            iss >> value; // transparency value
            // FixMe: use the value
        }
        else if (lineID == "illum")
        {
            iss >> type;
            if (type == 1) // no specular color for this material
                material.SetSpecularColor(VPColor::BLACK());
                //~ material.SetSpecularColor(material.GetDiffuseColor());
        }
        else if (lineID == "#")
        { // comment, ignore this line
        }
        else
        {
            cerr << "Error in '" << filename << "', line " << lineNumber << ": unknown ID '"
                 << lineID << "'" << endl;
        }
        iss.clear();
    }
    // Add last material
    if (materialName.size() > 0)
        matMapPtr->insert(make_pair(materialName,material));
}
