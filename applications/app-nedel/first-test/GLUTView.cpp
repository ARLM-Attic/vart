#include "GLUTView.h"

GLUTView* GLUTView::myView = new GLUTView();

void GLUTView::init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);  
  
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);  //  glShadeModel(GL_SMOOTH);
  
//	cilindro.criaCilindro();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // glPolygonMode(GL_BACK, GL_LINE);
}

void GLUTView::display(void)
{
//  glPushMatrix();
//	  glTranslatef(x, y, z);
//	  glRotatef(angulo, x, y, z);
//	  glScalef(tamanho em x, em y, em z);    
//	  glCallList(NUMERO);
//  glPopMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, width/height, 1.0, 1000.0);

	if ( myCamera.angleZY > 0 && myCamera.angleZY < 180 )
		gluLookAt(myCamera.camX, myCamera.camY, myCamera.camZ, myCamera.posX, myCamera.posY, myCamera.posZ, 0, 1, 0);  
	else
	{
		if ( myCamera.angleZY > 180 && myCamera.angleZY < 360 )
			gluLookAt(myCamera.camX, myCamera.camY, myCamera.camZ, myCamera.posX, myCamera.posY, myCamera.posZ, 0, -1, 0);  
		else
		{
			if ( myCamera.angleZY == 180 )
				gluLookAt(myCamera.camX, myCamera.camY, myCamera.camZ, myCamera.posX, myCamera.posY, myCamera.posZ, sin(myCamera.angleZX * PI / 180), 0, cos(myCamera.angleZX * PI / 180) );  
			else
			{
				if ( myCamera.angleZY == 0 || myCamera.angleZY == 360)
					gluLookAt(myCamera.camX, myCamera.camY, myCamera.camZ, myCamera.posX, myCamera.posY, myCamera.posZ, - sin(myCamera.angleZX * PI / 180), 0, - cos(myCamera.angleZX * PI / 180) );  
			}
		}
	}

	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();

	glPushMatrix();
  		glBegin (GL_LINES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f (0,  0, 0);
			glVertex3f (50, 0, 0);

			glColor3f(0.0, 1.0, 0.0);
			glVertex3f (0, 0,  0);
			glVertex3f (0, 50, 0);		
			
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f (0, 0,  0);
			glVertex3f (0, 0, 50);		
		glEnd();
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);

//	cout<<"Desenha cilindros\n";
//	cilindro.desenhaCilindros();

	glutSwapBuffers();
}

void GLUTView::reshape(int w, int h)
{
	if (h == 0) { h = 1; }

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	width = (GLfloat) w;
	height = (GLfloat) h;

	glutPostRedisplay();
}

GLUTView::GLUTView(void)
{
	width = 0.0;
	height = 0.0;
	myKeyboard = new Keyboard();
}


GLUTView::~GLUTView(void)
{
	delete myKeyboard;
}