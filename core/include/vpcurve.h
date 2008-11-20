/// \file vpcurve.h
/// \brief Header file for VPCurve VPAT class.
/// \version $Revision: 1.4 $

// ChangeLog is at the implementation file

#ifndef __VPCURVE_H
#define __VPCURVE_H

#include "vpgraphicobj.h"
#include "vppoint4d.h"
#include "vptransform.h"
#include <vector>

/// \class VPCurve vpcurve.h
/// \brief A paramteric curve in 3D/4D space.
///
/// FixMe: Put detailed description here.
class VPCurve : public VPGraphicObj {
    public:
        /// \brief Returns a point of the curve.
        /// \param t [in] curve parameter describing the desired point.
        /// \param result [out] the point at position t.
        virtual void GetPoint(double t, VPPoint4D* result)=0;

        /// \brief Applies a transformation to all controlPoints
        void Transform(const VPTransform& t);

        // FixMe: Implement DrawInstanceOGL?
    protected:
        std::vector<VPPoint4D> controlPoints;
};

#endif
