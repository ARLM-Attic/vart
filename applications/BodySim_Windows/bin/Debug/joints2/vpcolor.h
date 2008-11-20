///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vpcolor.h
//  DESCRIPTION.: Contain the VPColor class declarations. 
//
//  AUTHOR......: Fernando Sola Pereira
//  DATE........: June/13/2000
//  DESCRIPTION.: Attributes and methods declaration.
//
///////////////////////////////////////////////////////////////////


// To foresee several "includes"
#ifndef __VPCOLOR_H	
#define __VPCOLOR_H




//-----------------------------------------------------------------
// C O L O R    
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// Class Name: VPColor (storage and manipulation of rgb colors)
// Superclass: -
// Subclasses: -

class VPColor {

	private:
		unsigned int rgbi[4];
		
	public:	
		VPColor();
		VPColor( unsigned short int r,
				     unsigned short int g,
				     unsigned short int b,
				     unsigned short int i );
		void vpSetR( unsigned short int r );
		void vpSetG( unsigned short int g );
		void vpSetB( unsigned short int b );
		void vpSetI( unsigned short int i );
		void vpSetColor( VPColor color );
		void vpSetColor( unsigned short int r,
											unsigned short int g,
											unsigned short int b,
											unsigned short int i );
		unsigned short int vpGetR();
		unsigned short int vpGetG();
		unsigned short int vpGetB();
		unsigned short int vpGetI();
		void vpGetColor( VPColor &color);
		void vpGetColor( unsigned short int &r,
											unsigned short int &g,
											unsigned short int &b,
											unsigned short int &i );	
		VPColor operator= ( VPColor color );
		VPColor operator+ ( VPColor color );
		VPColor operator- ( VPColor color );
		bool operator== ( VPColor color );
		bool operator!= ( VPColor color );
};


#endif // _VPCOLOR_H
