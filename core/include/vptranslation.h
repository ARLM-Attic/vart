//deprecated
///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vptranslation.h
//  DESCRIPTION.: Contain the VPTranslation class declarations.
//
//  AUTHOR......: Fernando Sola Pereira
//  DATE........: January/23/2001
//  DESCRIPTION.: Class declarations.
//
//  AUTHOR......: Isabel Harb Manssour
//  DATE........: March/01/2001
//  DESCRIPTION.: File and variables rename.
//
///////////////////////////////////////////////////////////////////


#ifndef __VPTRANSLATION_H
#define __VPTRANSLATION_H
///\deprecated


class VPTranslation {
	private:
		// translation values 
		float positionX, positionY;
		float previousPositionX, previousPositionY;
		float deltaX, deltaY;
	public:
		VPTranslation();
		
		void vpSetPositionX (float v);
		void vpSetPositionY (float v);
		void vpSetPreviousPositionX (float v);
		void vpSetPreviousPositionY (float v);
		void vpSetDeltaX (float v);
		void vpSetDeltaY (float v);

		float vpGetPositionX ();
		float vpGetPositionY ();
		float vpGetPreviousPositionX ();
		float vpGetPreviousPositionY ();
		float vpGetDeltaX ();
		float vpGetDeltaY ();
		
		void vpSetTranslation (float posX,float posY,float posAntX,
			         float posAntY,float dX,float dY);
		
		void vpGetTranslation (float &pos_X,float &pos_Y,float &posAnt_X,
			         float &posAnt_Y,float &dX,float &dY);
};

#endif  // __VPTRANSLATION_H
