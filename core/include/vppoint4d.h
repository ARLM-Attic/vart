/// \file vppoint4d.h
/// \brief Header file for VPPoint4D VPAT class.
/// \version 1.5

// ChangeLog is at the implementation file

#ifndef __VPVPPOINT4D_H
#define __VPVPPOINT4D_H

#include "vppoint.h"
#include "vppoint3d.h" // Temporarily needed. VPPoint3D could be replaced by VPPoint4D.
#include "vpvertex3d.h" // Temporarily needed. VPVertex3D could be replaced by VPPoint4D.
#include "vpvector3d.h" // Temporarily needed. VPVector3D could be replaced by VPPoint4D.
#include <iostream>

/// \class VPPoint4D vppoint4d.h
/// \brief Points and vectors using homogeneous coordinates.
///
/// The VPPoint4D class provides a unified way of representing point and vectors objects,
/// easing operations between these "two" kinds of entities along with integration
/// with the VPTransform class. A 4D is homogeneous point represented by a vector (x,y,z,w).
/// W coordinate is 0 for vectors and 1 for normalized points.
/// This class has been created long after the VPPoint3D and VPVector3D classes,
/// wich are used by (at this point) all other classes. It is recommended that this
/// class slowly substitutes the other two.
class VPPoint4D : public VPPoint
{
    /// Output operator
    // FixMe: Perhaps this will cause trouble on an application with no console...
    friend std::ostream& operator<<(std::ostream& output, const VPPoint4D& p);

    /// Type for comparison operators
    typedef bool (VPPoint4D::*tCompareOper)(const VPPoint4D&) const;
    public:
    // PUBLIC METHODS
        /// Creates the (0,0,0,1) point.
        VPPoint4D();
        VPPoint4D(const VPPoint4D& point);
        VPPoint4D(const VPVertex3D& v);
        VPPoint4D(const VPVector3D& v);
        VPPoint4D(const VPPoint3D& p);
        VPPoint4D(double x, double y, double z, double w = 1.0);

        VPPoint4D operator-() const;
        bool operator==(const VPPoint4D& point) const;
        bool operator!=(const VPPoint4D& point) const;
        void operator=(const VPPoint4D& point);
        void operator=(const VPVertex3D& v);
        void operator=(const VPVector3D& v);
        void operator=(const VPPoint3D& p);

        /// \brief Checks whether a point is "less then" other.

        /// The idea of ordering in 4D point comes from operatorLess public
        /// attribute.
        bool operator<(const VPPoint4D& p) const;

        /// \brief Verifies whether a point is less than other by looking at
        /// coordinates in lexicographical (x, y, z then w) order.
        bool LexicographicalLess(const VPPoint4D& p) const;

        /// \brief Verifies whether a point is less then other by looking at
        /// coordinates X, then Z only.
        bool XThenZLess(const VPPoint4D& p) const;
        /// \brief Addition for point and vector (or vice versa).

        /// Both point and vector should be normalized.
        /// \return a (translated) point.
        VPPoint4D operator+(const VPPoint4D& vector) const;
        void operator+=(const VPPoint4D& vector);

        /// \brief Subtraction of two points.

        /// Both points should be normalized.
        /// \return a vector (from "point" to "this").
        VPPoint4D operator-(const VPPoint4D& point) const;
        VPPoint4D operator*(double escalar) const;
        void operator*=(double escalar);

		VPPoint4D operator/( double escalar ) const;

        double GetX() const { return vetCoord[0]; }
        double GetY() const { return vetCoord[1]; }
        double GetZ() const { return vetCoord[2]; }
        double GetW() const { return vetCoord[3]; }

        /// \brief Returns vertex coordinates as a pointer to 4 doubles.

        /// This method is usefull to pass to glVertex4dv().
        const double* VetXYZW() const { return vetCoord; }

        /// \brief Computes the dot product between "this" and "p".
        double DotProduct(const VPPoint4D& p) const;

        /// \brief Computes the angle up to p.

        /// Requires both vectors to be normalized.
        /// \return the angle in radians of the smaller arc between "this" and "p".
        double AngleTo(const VPPoint4D& p) const;

        /// \brief Computes the cross product between "this" and "p".
        VPPoint4D CrossProduct(const VPPoint4D& p) const;

        void SetX(double x) { vetCoord[0] = x; }
        void SetY(double y) { vetCoord[1] = y; }
        void SetZ(double z) { vetCoord[2] = z; }
        void SetW(double w) { vetCoord[3] = w; }
        void SetXYZW(double x, double y, double z, double w);

        /// \brief Normalizes the point/vector.

        /// Points are normalized by dividing coordinates by W. Vectors are
        /// normalized by having their length changed to 1. Warning: The homogeneous
        /// point (0,0,0,0) is neither a point nor a vector and will cause a division
        /// by zero!
        void Normalize();
        double Length() const;
    // CLASS CONSTANT INITIALIZERS
        /// The (0,-1,0,0) vector.
        static const VPPoint4D& DOWN();
        /// The (0,0,0,1) point.
        static const VPPoint4D& ORIGIN();
        /// The X vector (1,0,0,0).
        static const VPPoint4D& X();
        /// The Y vector (0,1,0,0).
        static const VPPoint4D& Y();
        /// The Z vector (0,0,1,0).
        static const VPPoint4D& Z();
    // PUBLIC ATTRIBUTES
        /// \brief Points to the operator to use within operator<.

        /// The application programmer should change this pointer to the desired
        /// comparison function. Default value is LexicographicalLess. To change its
        /// value, use something like: VPPoint4D::operatorLess = &VPPoint4D::LexicographicalLess .
        /// Currently, possible values are: LexicographicalLess and XThenZLess.
        static tCompareOper operatorLess;
    private:
        double vetCoord[4];
};

#endif
