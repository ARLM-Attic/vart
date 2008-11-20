/// \file vpinterpolator.h
/// \brief Header file for VPInterpolator VPAT class.
/// \version $Revision: 1.2 $

// There is no associated implementation file (no vpinterpolator.cpp).

// ChangeLog
// Apr 05, 2006 - Bruno de Oliveira Schneider
// - GetValue is now "const".
// Mar 27, 2006 - Bruno de Oliveira Schneider
// - File created.

#ifndef __VPINTERPOLATOR_H
#define __VPINTERPOLATOR_H

/// \class VPInterpolator vpinterpolator.h
/// \brief Interpolator representation
///
/// A interpolator is a function whose domain is [0..infinite) and range is [0..1].
class VPInterpolator
{
    public:
        VPInterpolator() {}
        virtual ~VPInterpolator() {}
        virtual float GetValue(float n) const = 0;
};

#endif
