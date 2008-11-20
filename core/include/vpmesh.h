/// \file vpmesh.h
/// \brief Header file for VPMesh VPAT class.
/// \version 1.3

// ChangeLog is at the implementation file

#ifndef __VPMESH_H
#define __VPMESH_H

#include "vppoint4d.h"
#include "vpmaterial.h"
#include <vector>
#ifdef VP_OGL
#include <GL/gl.h>
#endif

/// \class VPMesh vpmesh.h
/// \brief A mesh is part of an graphical object, in which faces are made of
/// vertices according to some rule of formation.
///
/// The mesh is the basic building block of an mesh object (VPMeshObject). It has a single
/// material, and indexes to vertices that define faces according to a rule (on
/// a triangle strip, for instance, the first 3 indexes from a face, and then each
/// new index from a new face together with the last 2 indexes).
/// A mesh degenerates to a single polygon if the mesh object has no pratical
/// rule of formation.
class VPMesh {
    public:
    // PUBLIC TYPES
        enum MeshType { POINTS, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, TRIANGLE_STRIP,
                        TRIANGLE_FAN, QUADS, QUAD_STRIP, POLYGON };
        friend std::ostream& operator<<(std::ostream& output, MeshType mt);
    // PUBLIC METHODS
        /// Creates an uninitialized mesh.
        VPMesh();
        VPMesh(const VPMesh& mesh);
        //~ VPMesh(MeshType type);
        VPMesh& operator=(const VPMesh& mesh);

        /// \brief Draws the mesh assuming that its MeshObject is optimized.
        /// \return false if VPAT was not compiled with OpenGL support.
        bool DrawInstanceOGL() const;

        // \brief Draws the mesh assuming that its MeshObject is unoptimized.
        // \param vertVec [in] The vector of vertices from the parent MeshObject.
        // \return false if VPAT was not compiled with OpenGL support.
        //~ bool DrawOglUnoptimized(const std::vector<VPPoint4D>& vertVec) const;

        /// Returns the mesh type as OpenGL enum.
        #ifdef VP_OGL
        GLenum GetOglType() const { return GetOglType(type); }
        #endif

        void IncrementIndices(unsigned int increment);

    // PUBLIC ATTRIBUTES
        std::vector<unsigned int> indexVec; // indexes of the vertices (start at 0)
        std::vector<unsigned int> normIndVec; // indexes of the normals (for unoptimized meshes)
        VPMaterial material;
        MeshType type;
    private:
        #ifdef VP_OGL
        /// \brief Converts from VPAT MeshType to OpenGL GLenum (for mesh types).
        static GLenum GetOglType(MeshType type);
        #endif
};

#endif
