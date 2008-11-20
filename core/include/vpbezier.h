/// \file vpbezier.h
/// \brief Header file for VPBezier VPAT class.
/// \version 1.1.1.3

// ChangeLog is at the implementation file

#ifndef __VPBEZIER_H
#define __VPBEZIER_H

#include "vpcurve.h"
#include "vppoint4d.h"

/// \class VPBezier vpbezier.h
/// \brief A Bezier curve.
///
/// A Bezier curve defined by 4 control points.
class VPBezier : public VPCurve {
    public:
        VPBezier();
        /// \brief Returns a copy a control point.
        VPPoint4D GetControlPoint(int i);

        /// \brief Assigns a (copy of a) control point.
        void SetControlPoint(int i, const VPPoint4D& point);

        /// \brief Returns a point of the curve.
        /// \param t [in] curve parameter describing the desired point.
        /// \param result [out] the point at position t.
        virtual void GetPoint(double t, VPPoint4D* result);
};

#endif
