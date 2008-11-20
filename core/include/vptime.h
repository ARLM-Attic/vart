/// \file vptime.h
/// \brief Header file for VPTime VPAT class.
/// \version $Revision: 1.1 $

// ChangeLog is at the implementation file.

#ifndef __VPTIME_H
#define __VPTIME_H

#include <iostream>

/// \class VPTime vptime.h
/// \brief Elapsed time representation
///
/// VPTime represents the length of a time interval.

class VPTime
{
    friend std::ostream& operator<<(std::ostream& output, const VPTime& t);
    public:
    // PUBLIC TYPES
    // PUBLIC METHODS
        /// \brief Creates an unitialized object.
        VPTime();
        /// \brief Cast constructor from double.
        VPTime(double newSeconds);
        /// \brief Set to current time.
        ///
        /// Makes the active object hold the elapsed time since "the epoch". In sloppy terms,
        /// it could be said that this method set the active object to hold current time. "Epoch"
        /// is some fixed point in time determined by the operating system (usually
        /// 0h of Jan 1, 1970).
        void Set();
        /// \brief Returns the number of whole microseconds in elapsed time.
        unsigned int WholeMicroseconds();
        /// \brief Returns the number of whole miliseconds in elapsed time.
        unsigned int WholeMilliseconds();
        /// \brief Returns the number of whole seconds in elapsed time.
        unsigned int WholeSeconds();
        /// \brief Returns elapsed time as float.
        float AsFloat() const { return static_cast<float>(seconds); }
    // PUBLIC OPERATORS
        VPTime operator-(const VPTime& initialTime);
        bool operator>=(const VPTime& t) { return seconds >= t.seconds; }
        bool operator<=(const VPTime& t) { return seconds <= t.seconds; }
        bool operator>(const VPTime& t) { return seconds > t.seconds; }
        bool operator<(const VPTime& t) { return seconds < t.seconds; }
    // PUBLIC STATIC METHODS
        /// \brief Returns current time.
        ///
        /// Since VPTime represents elapsed time, NOW actually is the elapsed time since
        /// Epoch. See Set().
        static const VPTime& NOW();
    protected:
        /// \brief Number of seconds in elapsed time.
        double seconds;
    private:
};

#endif
