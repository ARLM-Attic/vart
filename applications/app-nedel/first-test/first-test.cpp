// first-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GLUTView.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("First test");

	GLUTView::initStatic();

	glutDisplayFunc  (GLUTView::displayStatic);
	glutReshapeFunc  (GLUTView::reshapeStatic);
	glutKeyboardFunc (GLUTView::keyboardStatic);
	glutMainLoop();

	return 0;
}

