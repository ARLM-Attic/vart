#ifndef KEYBOARD
#define KEYBOARD

	#define PI			3.1415926535897932384626433832795

	#include <stdlib.h> // Utilizado para comando exit.
	#include <iostream>

	class Keyboard
	{

		public:

			Keyboard(void);
			~Keyboard(void);

			void keyboard(unsigned char key, int x, int y);
	};

#endif
