//! \file vppoint.h
//! \brief Header file for VPPoint VPAT class.
//! \version 1.2

// ChangeLog
// Sep 27, 2005 - Bruno de Oliveira Schneider
// - Added virtual destructor.
// Sep 15, 2004 - Bruno de Oliveira Schneider
// - Removed the "vp" prefix from every method.
// - Code typing cleaned.
// - Added W related "get"s and "set"s.
// - Added DoxyGen documentation.
// Jul 10, 2000 - Isabel Harb Manssour
// - Classes and new methods declaration.

#ifndef __VPPOINT_H
#define __VPPOINT_H

///////////////////////////////////////////////////////////////////
// Class Name: VPPoint (abstract class)
// Superclass: -
// Subclasses: VPPoint2D, VPPoint3D

//! \class VPPoint vppoint.h
//! \brief Abstract class that provides a common base for point and vector classes.
class VPPoint {
    public:
        virtual ~VPPoint() {};
        virtual float GetX() {return 0;};
        virtual float GetY() {return 0;};
        virtual float GetZ() {return 0;};
        virtual float GetW() {return 0;};
        virtual void GetXY(float &x, float &y) {};
        virtual void GetXZ(float &x, float &z) {};
        virtual void GetYZ(float &y, float &z) {};
        virtual void GetXYZ(float &x, float &y, float &z) {};
        virtual void GetXYZW(float &x, float &y, float &z, float &w) {};
        virtual void SetX(float x) {};
        virtual void SetY(float y) {};
        virtual void SetZ(float z) {};
        virtual void SetW(float z) {};
        virtual void SetXY(float x, float y) {};
        virtual void SetXZ(float x, float z) {};
        virtual void SetYZ(float y, float z) {};
        virtual void SetXYZ(float x, float y, float z) {};
        virtual void SetXYZW(float x, float y, float z, float w) {};
};

#endif  // __VPPOINT_H
