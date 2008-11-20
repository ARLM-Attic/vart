/// \file vpcurve.cpp
/// \brief Implementation file for VPCurve VPAT class.
/// \version 1.1.1.2

// ChangeLog
// Oct 08, 2004 - Bruno de Oliveira Schneider
// - Code typing has been cleaned. DoxyGen documentation started.
// - Changed the type of control points from "VPVertex3D" to "VPPoint4D" in order
//   to compute geometrical transformations easier.
// - Changed the type of "controlPoints" from pointer (C vector) to STL vector in
//   order to enclose memory management control.
// - Changed "controlPoints" visibility from "private" to "protected", so that
//   derived classes may allocate space freely.
// - Removed constructors and the destructor (this is an abstract class).
// - Removed "Render" method which was not implemented.
// - Removed "vp" prefix from every method name.
// - Removed "#include <stdio.h>" from implementation file.
// Aug 10, 2000 - Anderson Maciel
// - Classes declaration.

#include <vpcurve.h>

void VPCurve::Transform(const VPTransform& t)
{
    unsigned int numCtrlPoints = controlPoints.size();
    for (unsigned int i=0; i < numCtrlPoints; ++i)
        t.ApplyTo(&controlPoints[i]);
}
