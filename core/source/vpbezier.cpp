/// \file vpbezier.cpp
/// \brief Implementation file for VPBezier VPAT class.
/// \version 1.1.1.3

// ChangeLog
// Jan 10, 2005 - Bruno de Oliveira Schneider
// - Fixed DoxyGen class description.
// - Removed previously commented methods: GetPointAsPoint and GetPointAsVertex.
// Oct 08, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Removed "vp" prefix from every method name.
// - Removed "#include <stdio.h>" from implementation.
// - Added "dealocateControlPoints" attribute.
// - Created destructor (old one did not free memory and was commented out).
// - Added "GetPoint" method.
// - Most methods changed to reflect the changes in VPCurve.
// Aug 10, 2000 - Anderson Maciel
// - Classes declaration.

#include "vpbezier.h"
#include <cassert>

VPBezier::VPBezier() {
    // Allocate memory
    controlPoints.reserve(4);
    // Set the number of control points. It seems to exist no way of setting the
    // number of control points without initializing all of them.
    controlPoints.assign(4,VPPoint4D::ORIGIN());
}

VPPoint4D VPBezier::GetControlPoint(int i) {
    assert (i < 4);
    assert (i >= 0);
    return controlPoints[i];
}

void VPBezier::SetControlPoint(int i, const VPPoint4D& point) {
    assert (i < 4);
    assert (i >= 0);
    controlPoints[i] = point;
}

void VPBezier::GetPoint(double t, VPPoint4D* result) {
    double invT = 1-t; // Cache this common value
    double t2 = t*t; // Cache this common value
    VPPoint4D p0(controlPoints[0]);
    VPPoint4D p1(controlPoints[1]);
    VPPoint4D p2(controlPoints[2]);
    VPPoint4D p3(controlPoints[3]);

    p0 *= invT*invT*invT;
    p1 *= 3*t*invT*invT;
    p2 *= 3*t2*invT;
    p3 *= t2*t;
    *result = p0 + p1 + p2 + p3;
}
