/// \file vprangesineinterpolator.h
/// \brief Header file for VPSineInterpolator VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog is at the implementation file.

#ifndef __VPRANGESINEINTERPOLATOR_H
#define __VPRANGESINEINTERPOLATOR_H

#include "vpsineinterpolator.h"

/// \class VPRangeSineInterpolator vprangesineinterpolator.h
/// \brief Smooth (sine function) interpolator with range
///
/// This is a smooth (following sine function) interpolator with period of 2 units,
///with a range that defines the amplitute. The values of the range must be among 0 and 1.
class VPRangeSineInterpolator : public VPSineInterpolator
{
    public:
        VPRangeSineInterpolator();
        void SetMinimunLimit(float minLim);
        void SetMaximunLimit(float maxLimit);
        virtual float GetValue(float n) const;
    private:
        float minLimit;
        float maxLimit;
};

#endif
