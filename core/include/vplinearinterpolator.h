/// \file vplinearinterpolator.h
/// \brief Header file for VPLinearInterpolator VPAT class.
/// \version $Revision: 1.2 $

// ChangeLog is at the implementation file.

#ifndef __VPLINEARINTERPOLATOR_H
#define __VPLINEARINTERPOLATOR_H

#include "vpinterpolator.h"

/// \class VPLinearInterpolator vplinearinterpolator.h
/// \brief Linear interpolation
///
/// This interpolation function is linear and has period of 2 units.
class VPLinearInterpolator : public VPInterpolator
{
    public:
        VPLinearInterpolator();
        virtual float GetValue(float n) const;
    private:
};

#endif
