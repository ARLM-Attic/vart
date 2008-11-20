#include "keyboard.h"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::keyboard(unsigned char key, int x, int y)
{
  switch(key) 
  {
	case 27: // Tecla 'Esc'		
		exit(0);
		break;
	default:
		printf("Tecla: %u \n", key);
		return;
  }

//  glutPostRedisplay();
}
