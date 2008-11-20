/// \file vpmesh.cpp
/// \brief Implementation file for VPMesh VPAT class.
/// \version 1.3

// ChangeLog
// Jun 09, 2005 - Bruno de Oliveira Schneider
// - Added a function to output mesh types.
// Dec 15, 2004 - Bruno de Oliveira Schneider
// - Added GetOglType() and IncrementIndices.
// Oct 26, 2004 - Bruno de Oliveira Schneider
// - Rename VPMeshType to MeshType, since its internal to a VP class, the prefix
//   seems redundant. Declared MeshType following OpenGL's primitives.
// - Added copy constructor, operator=, GetOglType and DrawInstanceOGL.
// Sep 23, 2004 - Bruno de Oliveira Schneider 
// - The class has been recreated from scratch after discussion with
//   Carla Freitas and Eduardo Costa.
// Aug 24, 2000 - Anderson Maciel
// - Class and methods declaration.
// Aug 22, 2000 - Anderson Maciel
// - Class standarization.
// Jun 07, 2000 - Julierme Kruger Gaviao
// - Class and methods declaration.

#include <vpmesh.h>

using namespace std;

VPMesh::VPMesh() {
}

VPMesh::VPMesh(const VPMesh& mesh) {
    indexVec = mesh.indexVec;
    normIndVec = mesh.normIndVec;
    type = mesh.type;
    material = mesh.material;
}

//~ VPMesh::VPMesh(MeshType type) {
//~ }

VPMesh& VPMesh::operator=(const VPMesh& mesh) {
    indexVec = mesh.indexVec;
    normIndVec = mesh.normIndVec;
    type = mesh.type;
    material = mesh.material;
    return *this;
}

void VPMesh::IncrementIndices(unsigned int increment) {
    for (unsigned int i=0; i < indexVec.size(); ++i)
        indexVec[i] += increment;
}

#ifdef VP_OGL
GLenum VPMesh::GetOglType(MeshType type) {
    switch (type) {
        case POINTS:
            return GL_POINTS;
        case LINES:
            return GL_LINES;
        case LINE_STRIP:
            return GL_LINE_STRIP;
        case LINE_LOOP:
            return GL_LINE_LOOP;
        case TRIANGLES:
            return GL_TRIANGLES;
        case TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case QUADS:
            return GL_QUADS;
        case QUAD_STRIP:
            return GL_QUAD_STRIP;
        default:
            return GL_POLYGON;
    }
}
#endif


bool VPMesh::DrawInstanceOGL() const {
#ifdef VP_OGL
    bool result = material.DrawOGL();
    glDrawElements(GetOglType(type), indexVec.size(), GL_UNSIGNED_INT, &indexVec[0]);
    return result;
#else
    return false;
#endif
}

//~ bool VPMesh::DrawOglUnoptimized(const vector<VPPoint4D>& vertVec) const {
//~ #ifdef VP_OGL
    //~ bool result = material.DrawOGL();
    //~ glBegin(GetOglType(type));
    //~ for (unsigned int i=0; i < indexVec.size(); ++i)
        //~ //glNormal3dv(normVec
        //~ glVertex4dv(vertVec[indexVec[i]].VetXYZW());
    //~ glEnd();
    //~ return result;
//~ #else
    //~ return false;
//~ #endif
//~ }

ostream& operator<<(ostream& output, VPMesh::MeshType mt)
{
    switch (mt)
    {
        case VPMesh::POINTS:
            output << "POINTS";
            break;
        case VPMesh::LINES:
            output << "LINES";
            break;
        case VPMesh::LINE_STRIP:
            output << "LINE_STRIP";
            break;
        case VPMesh::LINE_LOOP:
            output << "LINE_LOOP";
            break;
        case VPMesh::TRIANGLES:
            output << "TRIANGLES";
            break;
        case VPMesh::TRIANGLE_STRIP:
            output << "TRIANGLE_STRIP";
            break;
        case VPMesh::TRIANGLE_FAN:
            output << "TRIANGLE_FAN";
            break;
        case VPMesh::QUADS:
            output << "QUADS";
            break;
        case VPMesh::QUAD_STRIP:
            output << "QUAD_STRIP";
            break;
        case VPMesh::POLYGON:
            output << "POLYGON";
            break;
        default:
            output << "NO MESH TYPE";
    }
    return output;
}
