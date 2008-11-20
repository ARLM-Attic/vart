#ifndef GLUTVIEW
#define GLUTVIEW

	#include <iostream>
	#include <math.h>
	#include "GL/glut.h"

	#include "keyboard.h"
	#include "camera.h"
//	#include "desenhaCilindros.h"

	class GLUTView
	{
		private:
			static GLUTView* myView;

		public:			
			GLfloat width;					
			GLfloat height;

			Camera	 myCamera;
			Keyboard* myKeyboard;

			GLUTView(void);
			~GLUTView(void);

			void init(void);
			static void initStatic(void)
			{
				myView->init();
			};

			void display(void);
			static void displayStatic(void)
			{	myView->display();				
			};


			void reshape(int w, int h);
			static void reshapeStatic(int w, int h)
			{	myView->reshape(w, h);
			};

			void keyboard(unsigned char key, int x, int y) 
			{
				myKeyboard->keyboard(key, x, y);	  				
			};
			static void keyboardStatic(unsigned char key, int x, int y) 
			{
				myView->keyboard(key, x, y);	  			
			};

			};

#endif