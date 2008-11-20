///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcolor.cpp
//  DESCRIPTION.: Contain the VPColor class implementation.
//
//  AUTHOR......: Fernando Sola Pereira
//  DATE........: June/13/2000
//  DESCRIPTION.: Implementation of the class methods.
//
///////////////////////////////////////////////////////////////////


#include <vpcolor.h>


///////////////////////////////////////////////////////////////////
// Description: Class "VPColor" constructor without parameter.
// Parameters.: -
// Return.....: -

VPColor :: VPColor() {
	rgbi[0] = 0;
	rgbi[1] = 0;
	rgbi[2] = 0;
	rgbi[3] = 0;
}


///////////////////////////////////////////////////////////////////
// Description: Class "VPColor" constructor with parameter.
// Parameters.: int r (initial r value)
//			    int g (initial g value)
//              int b (initial g value)
//              int i (initial i value)
// Return.....: -

VPColor :: VPColor( unsigned short int r,
			        unsigned short int g,
				    unsigned short int b,
				    unsigned short int i ) {
	rgbi[0] = r;
	rgbi[1] = g;
	rgbi[2] = b;
	rgbi[3] = i;
}

		
///////////////////////////////////////////////////////////////////
// Description: Method "vpGetR" returns the value of r attribute.
// Parameters.: -
// Return.....: rgbi[0] (value of r attribute)

unsigned short int VPColor:: vpGetR() {
	return rgbi[0];
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetG" returns the value of g attribute.
// Parameters.: -
// Return.....: rgbi[1] (value of g attribute)

unsigned short int VPColor :: vpGetG() {
	return rgbi[1];
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetB" returns the value of b attribute.
// Parameters.: -
// Return.....: rgbi[2] (value of b attribute)

unsigned short int VPColor :: vpGetB() {
	return rgbi[2];
}
	

///////////////////////////////////////////////////////////////////
// Description: Method "vpGetI" returns the value of i attribute.
// Parameters.: -
// Return.....: rgbi[3] (value of i attribute)

unsigned short int VPColor :: vpGetI() {
	return rgbi[3];
}		


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetR" sends a new value to the r attribute.
// Parameters.: int r (contains a new value for r attribute)
// Return.....: -

void VPColor :: vpSetR( unsigned short int r ) {
	rgbi[0] = r;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetG" sends a new value to the g attribute.
// Parameters.: int g (contains a new value for g attribute)
// Return.....: -

void VPColor :: vpSetG( unsigned short int g ) {
	rgbi[1] = g;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetB" sends a new value to the b attribute.
// Parameters.: int b (contains a new value for b attribute)
// Return.....: -

void VPColor :: vpSetB( unsigned short int b ) {
	rgbi[2] = b;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetI" sends a new value to the i attribute.
// Parameters.: int i (contains a new value for i attribute)
// Return.....: -

void VPColor :: vpSetI( unsigned short int i ) {
	rgbi[2] = i;
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetColor" gets the attributes values.
// Parameters.: VPColor &color
// Return.....: -

void VPColor :: vpGetColor( VPColor &color) {
	color.rgbi[0] = rgbi[0];
	color.rgbi[1] = rgbi[1];
	color.rgbi[2] = rgbi[2];
	color.rgbi[3] = rgbi[3];
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetColor" sends a new value to the attributes.
// Parameters.: VPColor color
// Return.....: -

void VPColor :: vpSetColor( VPColor color ) {
	rgbi[0] = color.rgbi[0];
	rgbi[1] = color.rgbi[1];
	rgbi[2] = color.rgbi[2];
	rgbi[3] = color.rgbi[3];
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpGetColor" gets the attributes values.
// Parameters.: unsigned short int &r
//				unsigned short int &g
//              unsigned short int &b
//              unsigned short int &i
// Return.....: -

void VPColor :: vpGetColor( unsigned short int &r,
						   unsigned short int &g,
			               unsigned short int &b,
			               unsigned short int &i ) {
	r = rgbi[0];
	g = rgbi[1];
	b = rgbi[2];
	i = rgbi[3];
}


///////////////////////////////////////////////////////////////////
// Description: Method "vpSetColor" sends a new value to the b attribute.
// Parameters.: unsigned short int r
//				unsigned short int g
//              unsigned short int b
//              unsigned short int i
// Return.....: -

void VPColor :: vpSetColor( unsigned short int r,
						   unsigned short int g,
			               unsigned short int b,
			               unsigned short int i ) {
	rgbi[0] = r;
	rgbi[1] = g;
	rgbi[2] = b;
	rgbi[3] = i;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPColor color (object that has the new value for the
//					       attributes of the class)
// Return.....: VPColor (current object)

VPColor VPColor :: operator= ( VPColor color ) {
	rgbi[0] = color.rgbi[0];
	rgbi[1] = color.rgbi[1];
	rgbi[2] = color.rgbi[2];
	rgbi[3] = color.rgbi[3];
	return *this;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPColor color (object that has the new value for the
//					       attributes of the class)
// Return.....: VPColor (current object)

VPColor VPColor :: operator+ ( VPColor color ) {
	VPColor c(rgbi[0]+color.rgbi[0], rgbi[1]+color.rgbi[1],
			  rgbi[2]+color.rgbi[2], rgbi[3]+color.rgbi[3]);
	return c;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPColor color (object that has the new value for the
//					       attributes of the class)
// Return.....: VPColor (current object)

VPColor VPColor :: operator- ( VPColor color ) {
	VPColor c(rgbi[0]-color.rgbi[0], rgbi[1]-color.rgbi[1],
			  rgbi[2]-color.rgbi[2], rgbi[3]-color.rgbi[3]);
	return c;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPColor color (object that has the new value for the
//					       attributes of the class)
// Return.....: bool (current object)

bool VPColor :: operator== ( VPColor color ) {
	if ( rgbi[0] == color.rgbi[0] &&
		 rgbi[1] == color.rgbi[1] &&
		 rgbi[2] == color.rgbi[2] &&
		 rgbi[3] == color.rgbi[3] )
		return true;
	else
		return false;
}


///////////////////////////////////////////////////////////////////
// Description: Method that implements the overload of = operator
// Parameters.: VPColor color (object that has the new value for the
//					       attributes of the class)
// Return.....: bool (current object)

bool VPColor :: operator!= ( VPColor color ) {
	if ( rgbi[0] != color.rgbi[0] ||
		 rgbi[1] != color.rgbi[1] ||
		 rgbi[2] != color.rgbi[2] ||
		 rgbi[3] != color.rgbi[3] )
		return true;
	else
		return false;
}
