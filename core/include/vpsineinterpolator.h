/// \file vpsineinterpolator.h
/// \brief Header file for VPSineInterpolator VPAT class.
/// \version $Revision: 1.2 $

// ChangeLog is at the implementation file.

#ifndef __VPSINEINTERPOLATOR_H
#define __VPSINEINTERPOLATOR_H

#include "vpinterpolator.h"

/// \class VPSineInterpolator vpsineinterpolator.h
/// \brief Smooth (sine function) interpolator
///
/// This is a smooth (following sine function) interpolator with period of 2 units.
class VPSineInterpolator : public VPInterpolator
{
    public:
        VPSineInterpolator();
        virtual float GetValue(float n) const;
    private:
};

#endif
