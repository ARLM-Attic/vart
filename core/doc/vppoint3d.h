///////////////////////////////////////////////////////////////////
///  \file               vppoint3d.h
///  \brief              Contain the VPPoint3D class declarations.	
///  \version            0.1
///////////////////////////////////////////////////////////////////
//  Change log:
//  August 10/2000 Isabel Harb Manssour <isabel @ provedor.com>
//  - Classes and new methods declaration.
//  January 20/2000 Isabel Harb Manssour
//  - New overload methods declaration.
//  April 18/2001 Isabel Harb Manssour
//  - Change of attributes.


#ifndef __VPPOINT3D_H	
#define __VPPOINT3D_H	

#include	<vppoint.h>



///////////////////////////////////////////////////////////////////
/// \class VPPoint3D 
/// \brief Storage and manipulation of 3D points
///
/// Contain the VPPoint3D class implementation that 
/// is responsible for the 3D point operations.<br>
/// Superclass: VPPoint
////////////////////////////////////////////////////////////////////
class VPPoint3D : public VPPoint {
    public:
		/// Point's X coordinate
		float x;
		float y;
		float z;
        /// \brief Initiate a point 0,0,0.
		VPPoint3D();
		/// \brief Class "VPPoint3D" constructor with parameters.
		/// \param xi initial x
		/// \param yi initial y
		/// \param zi initial z
		VPPoint3D(float xi, float yi, float zi);

		float vpGetX();
		float vpGetY();
		float vpGetZ();
		void vpGetXY(float &xx, float &yy);
		void vpGetXZ(float &xx, float &zz);
		void vpGetYZ(float &yy, float &zz);
		void vpGetXYZ(float &xx, float &yy, float &zz);
		/// \brief Returns a copy of itself
		/// \return A copy of itself
		VPPoint3D vpGetPoint3D(void);
        /// \brief Get the point value.     
        /// \return pointer to the array
		float* vpGetPointAsArray(void);
        /// \brief Put the point value in an array     
        /// \param float v	
		void vpGetPoint(float v[]);
		void vpSetX(float xx);
		void vpSetY(float yy);
		void vpSetZ(float zz);
		void vpSetXY(float xx, float yy);
		void vpSetXZ(float xx, float zz);
		void vpSetYZ(float yy, float zz);
		void vpSetXYZ(float xx, float yy, float zz);
        /// \brief Implements the overload of = operator
        /// \param VPPoint3D p (object that has the new value for the
        ///					       attributes of the class)
        /// \param const VPPoint3D& p
        /// \return VPPoint3D (current object)
		VPPoint3D operator= (VPPoint3D p);
		/// \brief Implements the overload of - operator        
        /// \param VPPoint3D p (object that has the value for the
        ///					         operation)
        /// \param const VPPoint3D& p
        /// \return VPPoint3D (result from "-" operation)
		VPPoint3D operator- (const VPPoint3D&);
        /// \brief Implements the overload of + operator
        /// \param VPPoint3D p (object that has the value for the
        ///					         operation)
        /// \param const VPPoint3D& p
        /// \return VPPoint3D (result from "+" operation)
		VPPoint3D operator+ (const VPPoint3D&);		
        /// \brief Implements the overload of * operator
        /// \param float num (number for multiplication)
        /// \return VPPoint3D (current object)
		VPPoint3D operator* (float num);		
        /// \brief Implements the overload of -= operator
        /// \param int num (number for subtraction)
        /// \return VPPoint3D (current object)
		VPPoint3D operator-= (int num);		
        /// \brief Implements the overload of != operator
        /// \param const VPPoint3D& p (object to be compared)
        /// \return true or false
		bool operator!= (const VPPoint3D&);		
        /// \brief Implements the overload of == operator
        /// \param const VPPoint3D p (object to be compared)
        /// \return true or false
		bool operator== (const VPPoint3D&);		
        /// \brief Compares two points, returns true if the points are equal or the point on the left is smaller
		/// \param const VPPoint3D p (object to be compared)
        /// \return true or false
		bool operator<= (const VPPoint3D&);
		/// \brief Compares two points, returns true if the point on the left is smaller
		/// \param const VPPoint3D p (object to be compared)
        /// \return true or false
		bool operator< (const VPPoint3D&);
        /// \brief Compares two points, returns true if the points are equal or the point on the left is bigger
		/// \param const VPPoint3D p (object to be compared)
        /// \return true or false 
		bool operator>= (const VPPoint3D&);
		/// \brief Compares two points, returns true if the point on the left is bigger
		/// \param const VPPoint3D p (object to be compared)
        /// \return true or false
		bool operator> (const VPPoint3D&);	
        /// \brief Calculates the distance between two points
        /// \param VPPoint3D p 
        /// \return float distance
		float vpDistance(VPPoint3D p1);
};

#endif