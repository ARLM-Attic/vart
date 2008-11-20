/// \file vptransform.h
/// \brief Header file for VPTransform VPAT class.
/// \version $Revision: 1.9 $

// ChangeLog is at the implementation file
#ifndef __VPTRANSFORM_H
#define __VPTRANSFORM_H

#include "vppoint4d.h"
#include "vpscenenode.h"
#include <iostream>
#include <iomanip>
#include <cmath>

class VPBoundingBox;

/// \class VPTransform vptransform.h
/// \brief Geometric transformations.
///
/// A transformation matrix is represented by a 4x4 pre-multiplication matrix,
/// i.e., to apply T1, then T2, then T3, one must multiply T3*T2*T1. Translation
/// values are at the rightmost column.
class VPTransform : public VPSceneNode
{
    /// 'ostream' class extension to output a transformation on the console.
    friend std::ostream& operator<<(std::ostream& output, const VPTransform& t);
    public:
        /// \brief Creates an uninitialized transform.
        VPTransform();

        /// \brief Copy constructor.
        /// \param trans [in] Source transform
        VPTransform(const VPTransform& trans);

        /// \brief Turns transform into identity.
        void MakeIdentity();

        /// \brief Applies transformation to a point.
        /// \param point [in] Point to apply transformation
        /// \return The transformed point.
        VPPoint4D operator*(const VPPoint4D& point) const;

        /// \brief Applies transformation to another transform.
        /// \param t [in] Transform to apply transformation
        /// \return The combined transform.
        VPTransform operator*(const VPTransform& t) const;

        /// \brief Copies data from anoter transform.
        VPTransform& operator=(const VPTransform& t);

        /// \brief Applies tranformation to a point.
        /// \param ptPoint [in,out] Point to be transformed
        void ApplyTo(VPPoint4D* ptPoint) const;

        /// \brief Turns transform into a translation.
        ///
        /// MakeTranslation expects a vector but actualy ignores the W coordinate.
        /// \param translationVector [in] Vector representation of the translation
        void MakeTranslation(const VPPoint4D& translationVector);

        /// \brief Turns transform into a translation.
        /// \param tx [in] X component of translation
        /// \param ty [in] Y component of translation
        /// \param tz [in] Z component of translation
        void MakeTranslation(double tx, double ty, double tz)
            { MakeTranslation(VPPoint4D(tx,ty,tz,0)); }

        /// \brief Turns transform into a rotation around the X axis.
        /// \param radians [in] Angle of rotation
        void MakeXRotation(double radians);

        /// \brief Turns transform into a rotation around the Y axis.
        /// \param radians [in] Angle of rotation
        void MakeYRotation(double radians);

        /// \brief Turns transform into a rotation around the Z axis.
        /// \param radians [in] Angle of rotation
        void MakeZRotation(double radians);

        /// \brief Turns transform into a rotation (around some reference vetor).
        /// \param refVec [in] Reference vector
        /// \param radians [in] Angle of rotation
        void MakeRotation(const VPPoint4D& refVec, const float radians);

        /// \brief Turns transform into a rotation (around some reference axis).
        /// \param refPoint [in] Reference point (defines an axis together with refVec)
        /// \param refVec [in] Reference vector (defines an axis together with refPoint)
        /// \param radians [in] Angle of rotation
        void MakeRotation(const VPPoint4D& refPoint, const VPPoint4D& refVec, const float radians);

        /// \brief Turns transform into a scale.
        void MakeScale(double sX, double sY, double sZ);

        /// \brief Turns transform into a shear.
        void MakeShear(double shX, double shY);

        /// \brief Set all data in the transform.
        void SetData(double* data);

        /// \brief Returns the address of transformation matrix.
        ///
        /// Use this method to get an OpenGl like transformation matrix, compatible
        /// with methods such as "glLoadMatrixd" and "glMultMatrixd".
        const double* GetData() const { return matrix; }

        /// \brief Returns the X vector of the transform.
        ///
        /// A rigid transformation matrix may be interpreted as a coordinate system.
        /// This method returns the 1st column of the matrix which may be interpreted
        /// as the X vector of such coordinate system.
        void GetVectorX(VPPoint4D* result) const;

        /// \brief Returns the Y vector of the transform.
        ///
        /// A rigid transformation matrix may be interpreted as a coordinate system.
        /// This method returns the 2nd column of the matrix which may be interpreted
        /// as the Y vector of such coordinate system.
        void GetVectorY(VPPoint4D* result) const;

        /// \brief Returns the Z vector of the transform.
        ///
        /// A rigid transformation matrix may be interpreted as a coordinate system.
        /// This method returns the 3rd column of the matrix which may be interpreted
        /// as the Z vector of such coordinate system.
        void GetVectorZ(VPPoint4D* result) const;

        /// \brief Returns the translation part of the transform.
        void GetTranslation(VPPoint4D* result) const;

        /// \brief Check for NaNs inside the matrix.
        ///
        /// This is for debugging purposes only.
        #ifndef NDEBUG
        bool HasNaN() const;
        #endif

        /// \brief Apply transform to rendering engine.
        /// \return false if VPAT was not compiled with OpenGL support.
        virtual bool DrawOGL() const;

        virtual TypeID GetID() const { return TRANSFORM; }

        /// \brief Returns the recursive bounding box.
        /// \param bBox [out] recursive bounding box.
        /// \return true if the is a return value exists.
        virtual bool RecursiveBoundingBox(VPBoundingBox* bBox);
    protected:
    // PROTECTED ATTRIBUTES
        double matrix[16];
    private:
    // PRIVATE METHODS
        bool Zero(const double& n) { return (fabs(n) < 0.0000001); }
};

#endif
