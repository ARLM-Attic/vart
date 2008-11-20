/// \file vpmeshobject.h
/// \brief Header file for VPMeshObject VPAT class.
/// \version $Revision: 1.12 $

// ChangeLog is at the implementation file.

#ifndef __VPMESHOBJECT_H
#define __VPMESHOBJECT_H

#include "vpmesh.h"
#include "vppoint4d.h"
#include "vpgraphicobj.h"
#include "vptransform.h"
#include <vector>
#include <list>
#include <map>

/// \class VPMeshObject vpmeshobject.h
/// \brief Graphical object made of polygon meshes.
///
/// A mesh object, unlike other graphical objects, maintains information about
/// every vertex, normals, texture coordinates and edges. This is the class of
/// choice for creating objects from files or objects whose vertex are to be edited.
/// A mesh object may be optimized for display, and in that case vertex editing will
/// be no longer possible.
class VPMeshObject : public VPGraphicObj {
    public:
    // PUBLIC METHODS
        VPMeshObject();
        VPMeshObject(const VPMeshObject& obj);
        VPMeshObject& operator=(const VPMeshObject& obj);

        /// \brief Assigns a material to all meshes of the mesh object.
        void SetMaterial(const VPMaterial& mat);

        /// \brief Creates a box aligned with the 3 reference planes (XY, XZ and YZ).
        void MakeBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);
    
        /// \brief Computes the projection of all vertices along the Y axis.
        /// \return a list of projected points.
        /// \param resultPtr [in,out] A pointer to a list that will be filled with
        ///        resulting vertices.
        /// \param height [in] Y coordinate to use in the projection (defaults to 0).
        void GetYProjection(std::list<VPPoint4D>* resultPtr, double height=0) const;

        /// \brief Optimize object for display.
        /// \bug Not implemented yet.
        ///
        /// This method creates an internal representation that is optimized for
        /// display (currently aimed at OpenGL - optimized representation may not
        /// be usefull for other renderers such as Direct3D). After being optimized,
        /// old data is discarded and the object can no longer be edited.
        void Optimize();

        /// \brief Erases internal structures.
        ///
        /// Erases previous vertices, indices, normals, texture coordinates, etc.
        /// Use before a complete change in the object.
        void Clear();

        /// \brief Sets the vector of vertices.
        /// \param vertexVec [in] The new vector of vertices.
        ///
        /// All vertices in vertexVec are copied into the MeshObject's internal
        /// representation. The MeshObject becomes unoptimized. The meshList is
        /// cleared.
        void SetVertices(const std::vector<VPPoint4D>& vertexVec);

        /// \brief Sets the vector of vertices.
        /// \param vertexStr [in] The set of vertices (a C-string of point
        /// coordinates).
        ///
        /// Parses all values in vertexStr and stores them into the MeshObject's
        /// internal representation. Parameter vertexStr should be set of points
        /// separated by commas, point coordinates should be separated by spaces,
        /// point may have 3 or 4 coordinates - i.e.: "1.2 0.2 3.1, 2 3 4 1, 1 2 3"
        /// The MeshObject becomes unoptimized. The meshList is cleared.
        void SetVertices(const char* vertexStr);

        /// \brief Changes one vertex.
        /// \param index [in] Index of the vertex to be changed.
        /// \param newValue [in] New value to be copied into the vertex.
        void SetVertex(unsigned int index, const VPPoint4D& newValue);

        /// \brief Returns the coordinates of the vertices in the object.
        const std::vector<double>& GetVerticesCoordinates() { return vertCoordVec; }

        /// \brief Adds a face (a mesh of a single polygon) based on previously
        /// set vertices.
        /// \param indexStr [in] The set of indices of the face (a C-string of
        /// numbers separated by spaces) - i.e.: "0 1 2 3".
        /// \sa SetVertices(const char*), SetVertices(const std::vector<VPPoint4D>&).
        ///
        /// Creates a new Mesh of a single polygon, based on the indices found
        /// in indexStr. Indices start at zero. The vertices referenced should be
        /// in counter-clockwise order. Vertices should be referenced in
        /// counter-clockwise order. No checking is done.
        void AddFace(const char* indexStr);

        /// Computes the bounding box.
        virtual void ComputeBoundingBox();

        virtual TypeID GetID() const { return MESH_OBJECT; }

        /// \brief Merges one mesh object with another.
        ///
        /// All vertices, normals, meshes, etc. are copied inside the active object.
        void MergeWith(const VPMeshObject& obj);

        /// \brief Apply Transformation to all vertices.
        void Transform(const VPTransform& trans);

        //~ /// \brief Splits the object along some plane parallel to XZ.
        //~ /// \param y [in] Position of the cutting plane.
        //~ /// \param ptObjAbove [out] Parts that lie below the cutting plane.
        //~ /// \param ptObjBelow [out] Parts that lie above the cutting plane.
        //~ ///
        //~ /// Splits an object into two, along a cutting plane. The two resulting objects
        //~ /// are returned by filling the objects pointed by ptObjAbove and ptObjBelow, which
        //~ /// must not contain previous geometry (use Clear()). If the cutting plane does not
        //~ /// intersect the object, one of the resulting objects will be empty (check with
        //~ /// IsEmpty()).
        //~ void YSplit(double y, VPMeshObject* ptObjBelow, VPMeshObject* ptObjAbove) const;

        /// \brief Checks whether the object contains geometry data.
        ///
        /// Returns true if the object contains no geometry data. Actually it checks whether
        /// the mesh list is empty, which should be enough for consistent mesh objects.
        bool IsEmpty() { return meshList.empty(); }

        /// \brief Returns a copy of a vertex.
        VPPoint4D GetVertex(unsigned int pos);
		
		/// \brief Return the Aritmethic mean of vertexes values
		VPPoint4D GetVertexMedia();

    // STATIC PUBLIC METHODS
        /// \brief Computes the normal of a triangle.
        /// \param v1 [in] 1st triangle vertex
        /// \param v2 [in] 2nd triangle vertex
        /// \param v3 [in] 4rd triangle vertex
        /// \param resultPtr [out] Triangle normal (unit length).
        ///
        /// V1, V2 and V3 should be in counter clockwise order.
        static void ComputeTriangleNormal(const VPPoint4D& v1, const VPPoint4D& v2,
                                          const VPPoint4D& v3, VPPoint4D* resultPtr);

        /// \brief Read VPMeshObjects from a Wavefront OBJ file
        /// \bug Currently reads only triangle meshes.
        /// \param filename [in] The file (Wavefront OBJ file) name.
        /// \param resultPtr [out] The address of an empty list of pointers to mesh objects
        /// to be filled with the addresses of newly allocated mesh objects.
        ///
        /// This method creates mesh objects marked as auto-delete, ie, they will be
        /// automatically deleted if attached to scene. If not, the application programmer
        /// should delete them.
        static void ReadFromOBJ(const std::string& filename, std::list<VPMeshObject*>* resultPtr);

    // STATIC PUBLIC ATTRIBUTES
        /// Size of normals for rendering (in world coordinates).
        static float sizeOfNormals;

    protected:
    // Protected Methods
        virtual bool DrawInstanceOGL() const;
        static void ReadMaterialTable(const std::string& filename,
                                      std::map<std::string,VPMaterial>* matMapPtr);
    // Protected Attributes
        /// \brief Vector of pointers to all vertices.
        ///
        /// This is the unoptimized storage of vertices. It must be a vector (not
        /// a list) because the internal meshes identifies vertices by integers.
        std::vector<VPPoint4D> vertVec;

        /// \brief Vector of all vertices (their coordinates in sequence).
        ///
        /// This is the optimized storage of vertices. It must be a vector (not
        /// a list) because OpenGL will see it as a C array. Every 3 sucessive
        /// coordinates in the array describe a vertex in the object.
        std::vector<double> vertCoordVec;

        /// \brief Vector of pointers to all normals.
        ///
        /// This is the unoptimized storage of normals. It must be a vector (not
        /// a list) because the internal meshes identifies vertices by integers.
        std::vector<VPPoint4D> normVec;

        /// \brief Vector of all normals (their coordinates in sequence).
        ///
        /// This is the optimized storage of normals. It must be a vector (not
        /// a list) because OpenGL will see it as a C array.
        std::vector<double> normCoordVec;

        /// \brief Vector of all texture coordinates.
        ///
        /// This is both the optimized and the unoptimized storage of texture coordinates.
        /// It must be a vector (not a list) because OpenGL will see it as a C array.
        std::vector<float> textCoordVec;

        /// \brief List of Meshes
        std::list<VPMesh> meshList;
};

#endif
