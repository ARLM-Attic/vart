///////////////////////////////////////////////////////////////////
//
//  PROJECT.....: vpat - Creating Virtual Patients
//  RESPONSIBLE.: Carla Freitas e Luciana Nedel
//
//  FILE........: vprotation.h
//  DESCRIPTION.: Contain the VPRotation class declarations.
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


#ifndef __VPROTATION_H
#define __VPROTATION_H


class VPRotation {
	private:
		// rotation values 
		float positionX, positionY;
		float previousPositionX, previousPositionY;
		float angleX, angleY;
	public:
		VPRotation();
		
		void vpSetPositionX (float v);
		void vpSetPositionY (float v);
		void vpSetPreviousPositionX (float v);
		void vpSetPreviousPositionY (float v);
		void vpSetAngleX (float v);
		void vpSetAngleY (float v);

		float vpGetPositionX ();
		float vpGetPositionY ();
		float vpGetPreviousPositionX ();
		float vpGetPreviousPositionY ();
		float vpGetAngleX ();
		float vpGetAngleY ();
		
		void vpSetRotation (float posX,float posY,float posAntX,
			         float posAntY,float angleX,float angleY);
		
		void vpGetRotation (float &pos_X,float &pos_Y,float &posAnt_X,
			         float &posAnt_Y,float &angle_X,float &angle_Y);
};

#endif  // __VPROTATION_H
